#include "Std_MV.h"
#include <stdio.h>
#include <string>
#include <filesystem>
#include "wavpack.h"

#define MAX_NCH 8
#define REPLAYGAIN_TRACK        0x2
#define REPLAYGAIN_ALBUM        0x4
#define SOFTEN_CLIPPING         0x8
#define PREVENT_CLIPPING        0x10

using namespace System::Audio;

struct wpcnxt :ZeroStruct<wpcnxt>
{
	WavpackContext *wpc;    // WavPack context provided by library
	float play_gain;        // playback gain (for replaygain support)
	int soft_clipping;      // soft clipping active for playback
	int output_bits;        // 16, 24, or 32 bits / sample
	int32_t sample_buffer[512 * MAX_NCH * 2];  // sample buffer
	float error[8];  // error term for noise shaping
};

struct WVStream :ZeroStruct<WVStream>
{
	wpcnxt* File;
	AudioInfo Info;
	Int64 DecodePosMs;
	char* Buffer;
	size_t BufferSize;
	float Scaler;
};

void CloseEx(wpcnxt* WV)
{
	if (WV->wpc)WV->wpc = WavpackCloseFile(WV->wpc);
	if (WV != NULL) { delete WV; WV = NULL; }
}
int read_samples(struct WVStream *wv, int num_samples)
{
	int num_chans = wv->Info.Channels, samples, tsamples;
	samples = WavpackUnpackSamples(wv->File->wpc, (int32_t*)wv->File->sample_buffer, num_samples);
	tsamples = samples * num_chans;

	if (tsamples)
	{
		if (!(WavpackGetMode(wv->File->wpc) & MODE_FLOAT))
		{
			float scaler = (float)(1.0 / ((unsigned long)1 << (WavpackGetBytesPerSample(wv->File->wpc) * 8 - 1)));
			scaler = 0;
		}
		if (!wv->Info.BitsPerSample.IsFloating)
		{
			float *fptr = (float *)wv->File->sample_buffer;
			int32_t *lptr = wv->File->sample_buffer;
			int cnt = tsamples;

			while (cnt--)
				*fptr++ = *lptr++ * wv->Scaler;
		}

		if (wv->File->play_gain != 1.0)
		{
			float *fptr = (float*)wv->File->sample_buffer;
			if (!wv->File->soft_clipping)
			{
				switch (wv->File->output_bits)
				{
				case 8:
				{
					Int8 *sptr = (Int8 *)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							Int8 dst;

							float val = *fptr - wv->File->error[ch];
							val *= wv->File->play_gain;

							if (val >= 1.0f)
								dst = 127;
							else if (val <= -1.0f)
								dst = -128;
							else
								dst = CLAMP_f8(floor(val * 255.0f));

							wv->File->error[ch] = (float)(dst / 255.0f - *fptr++);
							*sptr++ = dst;
						}
				}
				break;
				case 16:
				{
					short *sptr = (short *)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							int dst;

							float val = *fptr - wv->File->error[ch];
							val *= wv->File->play_gain;

							if (val >= 1.0f)
								dst = 32767;
							else if (val <= -1.0f)
								dst = -32768;
							else
								dst = (int)floor(val * 32768.0);

							wv->File->error[ch] = (float)(dst / 32768.0f - *fptr++);
							*sptr++ = dst;
						}
				}
				break;
				case 24:
				{
					Int24 *sptr = (Int24*)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							Int24 dst;

							float val = *fptr - wv->File->error[ch];
							val *= wv->File->play_gain;

							if (val >= 1.0)
								dst = 8388607;
							else if (val <= -1.0)
								dst = -8388608;
							else
								dst = CLAMP_f24(floor(val * 8388608.0f));

							wv->File->error[ch] = (float)((float)dst / 8388608.0f - *fptr++);
							*sptr++ = dst;
						}
				}
				break;
				case 32:
				{
					if (!wv->Info.BitsPerSample.IsFloating)
					{
						Int32 *sptr = (Int32 *)wv->File->sample_buffer;
						int cnt = tsamples, ch;

						while (cnt--)
							for (ch = 0; ch < num_chans; ++ch)
							{
								Int32 dst;
								float val = *fptr - wv->File->error[ch];
								val *= wv->File->play_gain;

								if (val >= 1.0)
									dst = 2147483647;
								else if (val <= -1.0)
									dst = -2147483648;
								else
									dst = CLAMP_f32(floor(val * 2147483648.0f));

								wv->File->error[ch] = (float)(dst / 2147483648.0f - *fptr++);
								*sptr++ = dst;
							}
					}
					else
					{
						float *sptr = (float *)wv->File->sample_buffer;
						int cnt = tsamples, ch;

						while (cnt--)
							for (ch = 0; ch < num_chans; ++ch)
							{
								float val = *fptr;
								val *= wv->File->play_gain;
								val = CLAMP_ff32(val);
								*sptr++ = val;
							}
					}
				}
				break;
				}
			}
			else
			{
				switch (wv->File->output_bits)
				{
				case 8:
				{
					Int8 *sptr = (Int8 *)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							Int8 dst;

							float val = *fptr - wv->File->error[ch];
							val *= wv->File->play_gain;

							if (val > 0.75)
								val = 1.0 - (0.0625 / (val - 0.5));
							else if (val < -0.75)
								val = -1.0 - (0.0625 / (val + 0.5));

							if (val >= 1.0f)
								dst = 127;
							else if (val <= -1.0f)
								dst = -128;
							else
								dst = CLAMP_f8(floor(val * 255.0f));

							wv->File->error[ch] = (float)(dst / 255.0f - val);
							fptr++;
							*sptr++ = dst;
						}
				}
				break;
				case 16:
				{
					short *sptr = (short *)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							int dst;

							float val = *fptr - wv->File->error[ch];
							val *= wv->File->play_gain;

							if (val > 0.75)
								val = 1.0 - (0.0625 / (val - 0.5));
							else if (val < -0.75)
								val = -1.0 - (0.0625 / (val + 0.5));

							if (val >= 1.0f)
								dst = 32767;
							else if (val <= -1.0f)
								dst = -32768;
							else
								dst = (int)floor(val * 32768.0);

							wv->File->error[ch] = (float)(dst / 32768.0f - val);
							fptr++;
							*sptr++ = dst;
						}
				}
				break;
				case 24:
				{
					Int24 *sptr = (Int24*)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							Int24 dst;

							float val = *fptr - wv->File->error[ch];
							val *= wv->File->play_gain;

							if (val > 0.75)
								val = 1.0 - (0.0625 / (val - 0.5));
							else if (val < -0.75)
								val = -1.0 - (0.0625 / (val + 0.5));

							if (val >= 1.0)
								dst = 8388607;
							else if (val <= -1.0)
								dst = -8388608;
							else
								dst = CLAMP_f24(floor(val * 8388608.0f));

							wv->File->error[ch] = (float)((float)dst / 8388608.0f - val);
							fptr++;
							*sptr++ = dst;
						}
				}
				break;
				case 32:
				{
					if (!wv->Info.BitsPerSample.IsFloating)
					{
						Int32 *sptr = (Int32 *)wv->File->sample_buffer;
						int cnt = tsamples, ch;

						while (cnt--)
							for (ch = 0; ch < num_chans; ++ch)
							{
								Int32 dst;
								float val = *fptr - wv->File->error[ch];
								val *= wv->File->play_gain;

								if (val > 0.75)
									val = 1.0 - (0.0625 / (val - 0.5));
								else if (val < -0.75)
									val = -1.0 - (0.0625 / (val + 0.5));

								if (val >= 1.0)
									dst = 2147483647;
								else if (val <= -1.0)
									dst = -2147483648;
								else
									dst = CLAMP_f32(floor(val * 2147483648.0f));

								wv->File->error[ch] = (float)(dst / 2147483648.0f - val);
								fptr++;
								*sptr++ = dst;
							}
					}
					else
					{
						float *sptr = (float *)wv->File->sample_buffer;
						int cnt = tsamples, ch;

						while (cnt--)
							for (ch = 0; ch < num_chans; ++ch)
							{
								float val = *fptr;
								val *= wv->File->play_gain;

								if (val > 0.75)
									val = 1.0 - (0.0625 / (val - 0.5));
								else if (val < -0.75)
									val = -1.0 - (0.0625 / (val + 0.5));

								val = CLAMP_ff32(val);
								*sptr++ = val;
							}
					}
				}
				break;
				}
			}
		}
		else
		{
			float *fptr = (float*)wv->File->sample_buffer;
			switch (wv->File->output_bits)
			{
			case 8:
			{
				Int8 *sptr = (Int8 *)wv->File->sample_buffer;
				int cnt = tsamples, ch;

				while (cnt--)
					for (ch = 0; ch < num_chans; ++ch)
					{
						Int8 dst;

						float val = *fptr - wv->File->error[ch];

						if (val >= 1.0f)
							dst = 127;
						else if (val <= -1.0f)
							dst = -128;
						else
							dst = CLAMP_f8(floor(val * 255.0f));

						wv->File->error[ch] = (float)(dst / 255.0f - *fptr++);
						*sptr++ = dst;
					}
			}
			break;
			case 16:
			{
				short *sptr = (short *)wv->File->sample_buffer;
				int cnt = tsamples, ch;

				while (cnt--)
					for (ch = 0; ch < num_chans; ++ch)
					{
						int dst;

						float val = *fptr;// -wv->File->error[ch];

						if (val >= 1.0f)
							dst = 32767;
						else if (val <= -1.0f)
							dst = -32768;
						else
							dst = (int)floor(val * 32768.0);

						wv->File->error[ch] = (float)(dst / 32768.0f - *fptr++);
						*sptr++ = dst;
					}
			}
			break;
			case 24:
			{
				Int24 *sptr = (Int24*)wv->File->sample_buffer;
				int cnt = tsamples, ch;

				while (cnt--)
					for (ch = 0; ch < num_chans; ++ch)
					{
						Int24 dst;

						float val = *fptr - wv->File->error[ch];

						if (val >= 1.0)
							dst = 8388607;
						else if (val <= -1.0)
							dst = -8388608;
						else
							dst = CLAMP_f24(floor(val * 8388608.0f));

						wv->File->error[ch] = (float)((float)dst / 8388608.0f - *fptr++);
						*sptr++ = dst;
					}
			}
			break;
			case 32:
			{
				if (!wv->Info.BitsPerSample.IsFloating)
				{
					Int32 *sptr = (Int32 *)wv->File->sample_buffer;
					int cnt = tsamples, ch;

					while (cnt--)
						for (ch = 0; ch < num_chans; ++ch)
						{
							Int32 dst;
							float val = *fptr - wv->File->error[ch];

							if (val >= 1.0)
								dst = 2147483647;
							else if (val <= -1.0)
								dst = -2147483648;
							else
								dst = CLAMP_f32(floor(val * 2147483648.0f));

							wv->File->error[ch] = (float)(dst / 2147483648.0f - *fptr++);
							*sptr++ = dst;
						}
				}
			}
			break;
			}
		}
	}
	return samples;
}

Std_MV::Std_MV()
{

}

Std_MV::~Std_MV()
{
}

std::vector<std::wstring> Std_MV::GetSupportFormats() const
{
	return {L".wv"};
}

void Std_MV::OpenLocal(const wchar_t* localPath, const char* path, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	WVStream* Handle = new WVStream();
	Handle->File = new wpcnxt();
	Handle->File->wpc = WavpackOpenFileInput(path, 0, OPEN_WVC | OPEN_NORMALIZE, 0);
	if (Handle->File->wpc == NULL)
	{
		SAFE_DELETE(Handle->File);
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return;
	}
	Handle->File->play_gain = 1.0;
	Handle->File->output_bits = WavpackGetBitsPerSample(Handle->File->wpc);
	switch (WavpackGetReducedChannels(Handle->File->wpc))
	{
	case 1u:
		Handle->Info.ChannelLayout = ChannelLayout::Mono;
		break;
	case 2u:
		Handle->Info.ChannelLayout = ChannelLayout::Stereo;
		break;
	case 3u:
		Handle->Info.ChannelLayout = ChannelLayout::Surround;
		break;
	case 4u:
		Handle->Info.ChannelLayout = ChannelLayout::Quad;
		break;
	case 6u:
		Handle->Info.ChannelLayout = ChannelLayout::Point5_1;
		break;
	case 8u:
		Handle->Info.ChannelLayout = ChannelLayout::Point7_1;
		break;
	default:
		Handle->Info.ChannelLayout = ChannelLayout::Unknown;
		break;
	}

	switch (Handle->File->output_bits)
	{
	case 8u:
		Handle->Info.BitsPerSample = BitsPerSample::SInt8;
		break;
	case 16u:
		Handle->Info.BitsPerSample = BitsPerSample::SInt16;
		break;
	case 24u:
		Handle->Info.BitsPerSample = BitsPerSample::SInt24;
		break;
	case 32u:
	{
		if ((WavpackGetMode(Handle->File->wpc) & MODE_FLOAT) == MODE_FLOAT)
		{
			Handle->Info.BitsPerSample = BitsPerSample::Float32;
		}
		else
		{
			Handle->Info.BitsPerSample = BitsPerSample::SInt32;
		}
	}
	break;
	}

	Handle->Scaler = (float)(1.0 / ((Int64)1 << (WavpackGetBytesPerSample(Handle->File->wpc) * 8 - 1)));

	Handle->Info.SampleLength = WavpackGetNumSamples(Handle->File->wpc);
	Handle->Info.SampleRate = WavpackGetSampleRate(Handle->File->wpc);
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->Info.BitRate = WavpackGetAverageBitrate(Handle->File->wpc, true) / 1000;
	Handle->DecodePosMs = 0.0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
}

void Std_MV::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	result.Value = nullptr;
	result.State = AudioDecoderError::UnSupported;
}

bool Std_MV::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WVStream));
		if (pipePtr)
		{
			WVStream* m_WV = static_cast<WVStream*>(pipePtr);
			if (m_WV) return false;
		}
	}
	catch (...) {}
	return false;
}

bool Std_MV::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WVStream));
		if (pipePtr)
		{
			WVStream* m_WV = static_cast<WVStream*>(pipePtr);
			if (m_WV)
			{
				info = m_WV->Info;
				return true;
			}
		}
	}
	catch (...) {}
	return false;

}

AudioDecoderError Std_MV::TryGetLocalAudioInfo(const wchar_t * localPath, const char* path, AudioInfo& info)
{
	WVStream* Handle = new WVStream();
	Handle->File = new wpcnxt();
	Handle->File->wpc = WavpackOpenFileInput(path, 0, OPEN_WVC | OPEN_NORMALIZE, 0);
	if (Handle->File == NULL)
	{
		SAFE_DELETE(Handle->File);
		SAFE_DELETE(Handle);
		return AudioDecoderError::UnSupported;
	}
	Handle->File->output_bits = WavpackGetBitsPerSample(Handle->File->wpc);
	switch (WavpackGetReducedChannels(Handle->File->wpc))
	{
	case 1u:
		info.ChannelLayout = ChannelLayout::Mono;
		break;
	case 2u:
		info.ChannelLayout = ChannelLayout::Stereo;
		break;
	case 3u:
		info.ChannelLayout = ChannelLayout::Surround;
		break;
	case 4u:
		info.ChannelLayout = ChannelLayout::Quad;
		break;
	case 6u:
		info.ChannelLayout = ChannelLayout::Point5_1;
		break;
	case 8u:
		info.ChannelLayout = ChannelLayout::Point7_1;
		break;
	default:
		info.ChannelLayout = ChannelLayout::Unknown;
		break;
	}

	switch (Handle->File->output_bits)
	{
	case 8u:
		info.BitsPerSample = BitsPerSample::SInt8;
		break;
	case 16u:
		info.BitsPerSample = BitsPerSample::SInt16;
		break;
	case 24u:
		info.BitsPerSample = BitsPerSample::SInt24;
		break;
	case 32u:
	{
		if ((WavpackGetMode(Handle->File->wpc) & MODE_FLOAT) == MODE_FLOAT)
		{
			Handle->Info.BitsPerSample = BitsPerSample::Float32;
		}
		else
		{
			Handle->Info.BitsPerSample = BitsPerSample::SInt32;
		}
	}
	break;
	}

	info.SampleLength = WavpackGetNumSamples(Handle->File->wpc);
	info.SampleRate = WavpackGetSampleRate(Handle->File->wpc);
	info.TimeSec = (double)info.SampleLength / info.SampleRate;
	info.TimeMs = info.TimeSec * 1000;
	info.BitRate = WavpackGetAverageBitrate(Handle->File->wpc, true) / 1000;

	SAFE_DELETE(Handle->File);
	SAFE_DELETE(Handle);
	return AudioDecoderError::OK;
}

void Std_MV::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WVStream));
		if (pipePtr)
		{
			WVStream* m_WV = static_cast<WVStream*>(pipePtr);
			if (m_WV)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done < desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples > 512) NSamples = 512;
					UInt64 need = bytes + NSamples * m_WV->Info.BlockAlign;
					if (m_WV->Buffer)
					{
						if (m_WV->BufferSize < need)
						{
							char* oldmem = m_WV->Buffer;
							if ((m_WV->Buffer = (char*)realloc(m_WV->Buffer, need)) == NULL)
							{
								m_WV->Buffer = (char*)malloc(need);
								if (!m_WV->Buffer)
								{
									free(oldmem);
									m_WV->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return;
								}
								memcpy(m_WV->Buffer, oldmem, m_WV->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_WV->Buffer = (char*)malloc(need);
						m_WV->BufferSize = need;
					}
					if (!m_WV->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return;
					}
					size_t tsamples = read_samples(m_WV, NSamples) * m_WV->Info.Channels;
					size_t tbytes = tsamples * (m_WV->File->output_bits / 8);
					if (tbytes == 0)
					{
						end = true;
						break;
					}
					memcpy(m_WV->Buffer + Done * m_WV->Info.BlockAlign, m_WV->File->sample_buffer, tbytes);
					bytes += tbytes;
					Done += (tbytes / m_WV->Info.BlockAlign);
				}
				buffer.SetData(m_WV->Buffer, m_WV->Info.SampleRate, m_WV->Info.BitsPerSample, Done, m_WV->Info.ChannelLayout, m_WV->DecodePosMs);

				m_WV->DecodePosMs = (int)(WavpackGetSampleIndex(m_WV->File->wpc) * 1000.0 / m_WV->Info.SampleRate);

				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...) {}
}

bool Std_MV::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WVStream));
		if (pipePtr)
		{
			WVStream* m_WV = static_cast<WVStream*>(pipePtr);
			if (m_WV)
			{
				if (WavpackSeekSample(m_WV->File->wpc, (int)(m_WV->Info.SampleRate / 1000.0 * posMS)))
				{
					m_WV->DecodePosMs = (int)(WavpackGetSampleIndex(m_WV->File->wpc) * 1000.0 / m_WV->Info.SampleRate);
					return true;
				}
				else
					return false;
			}
		}
	}
	catch (...) {}
	return false;
}

bool Std_MV::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WVStream));
		if (pipePtr)
		{
			WVStream* m_WV = (WVStream*)(pipePtr);
			if (m_WV)
			{
				if (m_WV->File)
				{
					CloseEx(m_WV->File);
				}
				if (m_WV->Buffer) { free(m_WV->Buffer); m_WV->Buffer = NULL; }
				SAFE_DELETE(m_WV);
			}
		}
	}
	catch (...) {}
	return true;
}

double Std_MV::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WVStream));
		if (pipePtr)
		{
			WVStream* m_WV = static_cast<WVStream*>(pipePtr);
			if (m_WV) return m_WV->DecodePosMs;
		}
	}
	catch (...) {}
	return 0.0;
}
