#include "Std_OPUS.h"
#include "opusfile.h"
#include <stdio.h>
#include <string>
#include <filesystem>

using namespace System::Audio;

#define MAX_PACKET (1500)
#define MAX_FRAME_SAMP (5760)
struct OPUSStream :ZeroStruct<OPUSStream>
{
	OggOpusFile * File;
	AudioInfo Info;
	Int64 DecodePosMs;
	char* Buffer;
	size_t BufferSize;
	size_t decoder_returnzero;
};

Std_OPUS::Std_OPUS()
{

}

Std_OPUS::~Std_OPUS()
{
}

std::vector<std::wstring> Std_OPUS::GetSupportFormats() const
{
	return { L".opus" };
}

void Std_OPUS::OpenLocal(const wchar_t* localPath, const char* nouse, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	std::string path = Converter::StringToUTF8(localPath);
	if (path.empty())
	{
		result.State = AudioDecoderError::InvalidPath;
		return;
	}

	int err = -1;
	OggOpusFile * dec = op_open_file(path.c_str(), &err);
	if (dec == nullptr || err != 0)
	{
		result.WriteMessage(L"unsupported file");
		result.State = AudioDecoderError::UnSupported;
		return;
	}
	OPUSStream* Handle = new OPUSStream();
	Handle->File = dec;
	switch (op_channel_count(Handle->File, -1))
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

	Handle->Info.BitsPerSample = BitsPerSample::Float32;

	Handle->Info.SampleLength = op_pcm_total(Handle->File, -1);
	Handle->Info.SampleRate = 48000;
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->Info.BitRate = op_bitrate(Handle->File, -1);
	Handle->DecodePosMs = 0.0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
}

void Std_OPUS::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& ret)
{
	ret.Value = nullptr;
	ret.State = AudioDecoderError::UnSupported;
}

bool Std_OPUS::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OPUSStream));
		if (pipePtr)
		{
			OPUSStream* m_OPUS = static_cast<OPUSStream*>(pipePtr);
			if (m_OPUS) return false;
		}
	}
	catch (...) {}
	return false;
}

bool Std_OPUS::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OPUSStream));
		if (pipePtr)
		{
			OPUSStream* m_OPUS = static_cast<OPUSStream*>(pipePtr);
			if (m_OPUS)
			{
				info = m_OPUS->Info;
				return true;
			}
		}
	}
	catch (...) {}
	return false;

}

AudioDecoderError Std_OPUS::TryGetLocalAudioInfo(const wchar_t * localPath, const char* nouse, AudioInfo& info)
{
	std::string path = Converter::StringToUTF8(localPath);
	if (path.empty())
	{
		return AudioDecoderError::InvalidPath;
	}

	int err = -1;
	OggOpusFile * dec = op_open_file(path.c_str(), &err);
	if (dec == nullptr || err != 0)
	{
		return AudioDecoderError::UnSupported;
	}
	switch (op_channel_count(dec, -1))
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

	info.BitsPerSample = BitsPerSample::Float32;

	info.SampleLength = op_pcm_total(dec, -1);
	info.SampleRate = 48000;
	info.TimeSec = (double)info.SampleLength / info.SampleRate;
	info.TimeMs = info.TimeSec * 1000;
	info.BitRate = op_bitrate(dec, -1);

	op_free(dec);
	dec = nullptr;
	return AudioDecoderError::OK;
}

void Std_OPUS::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OPUSStream));
		if (pipePtr)
		{
			OPUSStream* m_OPUS = static_cast<OPUSStream*>(pipePtr);
			if (m_OPUS)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done < desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples > 512) NSamples = 512;
					UInt64 need = bytes + NSamples * m_OPUS->Info.BlockAlign;
					if (m_OPUS->Buffer)
					{
						if (m_OPUS->BufferSize < need)
						{
							char* oldmem = m_OPUS->Buffer;
							m_OPUS->BufferSize = need;
							if ((m_OPUS->Buffer = (char*)realloc(m_OPUS->Buffer, need)) == NULL)
							{
								m_OPUS->Buffer = (char*)malloc(need);
								if (!m_OPUS->Buffer)
								{
									m_OPUS->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return;
								}
								memcpy(m_OPUS->Buffer, oldmem, m_OPUS->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_OPUS->Buffer = (char*)malloc(need);
						m_OPUS->BufferSize = need;
					}
					if (!m_OPUS->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return;
					}
					size_t tsamples = op_read_float(m_OPUS->File, (float*)((char*)m_OPUS->Buffer + Done * m_OPUS->Info.BlockAlign), NSamples*m_OPUS->Info.Channels, nullptr);
					size_t tbytes = tsamples * m_OPUS->Info.BlockAlign;
					if (tbytes == 0)
					{
						m_OPUS->decoder_returnzero++;
					}
					else
					{
						m_OPUS->decoder_returnzero == 0;
					}
					if (m_OPUS->decoder_returnzero >= 4u)
					{
						end = true;
						break;
					}
					bytes += tbytes;
					Done += (tbytes / m_OPUS->Info.BlockAlign);
				}
				buffer.SetData(m_OPUS->Buffer, m_OPUS->Info.SampleRate, m_OPUS->Info.BitsPerSample, Done, m_OPUS->Info.ChannelLayout, m_OPUS->DecodePosMs);

				ogg_int64_t rr = op_pcm_tell(m_OPUS->File);
				m_OPUS->DecodePosMs = (double)rr* m_OPUS->Info.TimeMs / (double)m_OPUS->Info.SampleLength;

				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...) {}
}

bool Std_OPUS::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OPUSStream));
		if (pipePtr)
		{
			OPUSStream* m_OPUS = static_cast<OPUSStream*>(pipePtr);
			if (m_OPUS)
			{
				ogg_int64_t cc = posMS / m_OPUS->Info.TimeMs* m_OPUS->Info.SampleLength;
				if (op_pcm_seek(m_OPUS->File, cc) == 0)
				{
					ogg_int64_t rr = op_pcm_tell(m_OPUS->File);
					m_OPUS->DecodePosMs = (double)rr* m_OPUS->Info.TimeMs / (double)m_OPUS->Info.SampleLength;
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

bool Std_OPUS::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OPUSStream));
		if (pipePtr)
		{
			OPUSStream* m_OPUS = (OPUSStream*)(pipePtr);
			if (m_OPUS)
			{
				if (m_OPUS->File)
				{
					op_free(m_OPUS->File);
				}
				if (m_OPUS->Buffer) { free(m_OPUS->Buffer); m_OPUS->Buffer = NULL; }
				SAFE_DELETE(m_OPUS);
			}
		}
	}
	catch (...) {}
	return true;
}

double Std_OPUS::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OPUSStream));
		if (pipePtr)
		{
			OPUSStream* m_OPUS = static_cast<OPUSStream*>(pipePtr);
			if (m_OPUS) return m_OPUS->DecodePosMs;
		}
	}
	catch (...) {}
	return 0.0;
}
