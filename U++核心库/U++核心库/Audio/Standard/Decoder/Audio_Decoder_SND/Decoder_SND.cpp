#include "Decoder_SND.h"
#include <stdio.h>
#include <string>
#include <filesystem>
#define	PSF_SEEK_ERROR			((sf_count_t) -1)
using namespace System::Audio;

struct SNDStream:ZeroStruct<SNDStream>
{
	SNDFILE* File;
	AudioInfo Info;
	Int64 DecodePosMs;
	char* Buffer;
	size_t BufferSize;
	UShort outbps;
};


mutex AudioDecoder_SND::locker;
AudioDecoder_SND* AudioDecoder_SND::instance = nullptr;
const wchar_t* AudioDecoder_SND::_author = L"xworkteam powered by libsndfile";
const wchar_t* AudioDecoder_SND::_description=L"wav½âÂë¿â";
const wchar_t* AudioDecoder_SND::_supports = L".wav;.aiff;.aifc;.mat;au;.snd";
UInt64 AudioDecoder_SND::pipeNum = 0Ul;

decltype(SNDAPI::sf_open) SNDAPI::sf_open = nullptr;
decltype(SNDAPI::sf_close) SNDAPI::sf_close= nullptr;
decltype(SNDAPI::sf_command) SNDAPI::sf_command = nullptr;
decltype(SNDAPI::sf_read_short) SNDAPI::sf_read_short = nullptr;
decltype(SNDAPI::sf_read_int) SNDAPI::sf_read_int = nullptr;
decltype(SNDAPI::sf_read_float) SNDAPI::sf_read_float= nullptr;
decltype(SNDAPI::sf_read_double) SNDAPI::sf_read_double = nullptr;
decltype(SNDAPI::sf_read_raw) SNDAPI::sf_read_raw = nullptr;
decltype(SNDAPI::sf_seek) SNDAPI::sf_seek = nullptr;
decltype(SNDAPI::sf_strerror) SNDAPI::sf_strerror = nullptr;
HMODULE SNDAPI::module = nullptr;


AudioDecoder_SND::AudioDecoder_SND() : AudioDecoder_base()
{

}

AudioDecoder_SND* AudioDecoder_SND::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			instance = new AudioDecoder_SND();
		}
	}
	return instance;
}

AudioDecoder_SND::~AudioDecoder_SND()
{
}

Version AudioDecoder_SND::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDecoder_SND::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDecoder_SND::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDecoder_SND::GetSupportFormat() const
{
	return _supports;
}

__forceinline int file_length(const char* Path)
{
	FILE* f = fopen(Path, "rb");
	if (f)
	{
		long end = 0;
		long cur = ftell(f);
		fseek(f, 0, SEEK_END);
		end = ftell(f);
		fseek(f, cur, SEEK_SET);
		return end;
	}
	else
		return 0;
}
Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_SND::OpenLocal(const wchar_t* localPath)
{
	Result<AudioDecoderPipe, AudioDecoderError> result(nullptr, AudioDecoderError::UnSupported);

	
	const char* path = Converter::CPlusNew(localPath, CultureInfo::Current().CPlusLocale);
	if (!path)
	{
		result.State = AudioDecoderError::InvalidPath;
		return result;
	}

	if (!experimental::filesystem::exists(experimental::filesystem::path(path)))
	{
		result.State = AudioDecoderError::InvalidPath;
		SAFE_DELETE(path);
		return result;
	}
	SNDStream* Handle = new SNDStream();
	SF_INFO sfinfo;
	Handle->File = SNDAPI::sf_open(path, SFM_READ, &sfinfo);
	if (Handle->File == NULL)
	{
		SAFE_DELETE(Handle);
		SAFE_DELETE(path);
		result.State = AudioDecoderError::UnSupported;
		return result;
	}

	const char* dss = SNDAPI::sf_strerror(Handle->File);

	switch (sfinfo.channels)
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
	
	if ((sfinfo.format & SF_FORMAT_PCM_S8) == SF_FORMAT_PCM_S8)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt8;
		Handle->outbps = 8;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_U8) == SF_FORMAT_PCM_U8)
	{
		Handle->Info.BitsPerSample = BitsPerSample::UInt8;
		Handle->outbps = 8;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_16) == SF_FORMAT_PCM_16)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt16LE;
		Handle->outbps = 16;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_24) == SF_FORMAT_PCM_24)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt24LE;
		Handle->outbps = 32;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_32) == SF_FORMAT_PCM_32)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt32LE;
		Handle->outbps = 32;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_32) == SF_FORMAT_FLOAT)
	{
		Handle->Info.BitsPerSample = BitsPerSample::Float32LE;
		Handle->outbps = -32;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_32) == SF_FORMAT_DOUBLE)
	{
		Handle->Info.BitsPerSample = BitsPerSample::Float64LE;
		Handle->outbps = -64;
	}
	else
	{
		SNDAPI::sf_command(Handle->File, SFC_SET_SCALE_FLOAT_INT_READ, NULL, SF_TRUE);
		Handle->Info.BitsPerSample = BitsPerSample::Float32LE;
		Handle->outbps = -32;
	}
	
	Handle->Info.SampleLength = sfinfo.frames;
	Handle->Info.SampleRate = sfinfo.samplerate;
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->Info.BitRate = ((double)file_length(path)*8.0 / Handle->Info.TimeMs);
	Handle->DecodePosMs = 0.0;

	SAFE_DELETE(path);

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
	pipeNum++;

	return result;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_SND::CreateStream()
{
	return Result<AudioDecoderPipe, AudioDecoderError>(nullptr, AudioDecoderError::UnSupported);
}

bool AudioDecoder_SND::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(SNDStream));
		if (pipePtr)
		{
			SNDStream* m_SND = static_cast<SNDStream*>(pipePtr);
			if (m_SND) return false;
		}
	}
	catch (...){}
	return false;
}

UInt64 AudioDecoder_SND::GetNumberOfPipes()
{
	return pipeNum;
}

bool AudioDecoder_SND::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(SNDStream));
		if (pipePtr)
		{
			SNDStream* m_SND = static_cast<SNDStream*>(pipePtr);
			if (m_SND)
			{
				info = m_SND->Info;
				return true;
			}
		}
	}
	catch (...){}
	return false;

}

AudioDecoderError AudioDecoder_SND::TryGetLocalAudioInfo(const wchar_t * localPath, AudioInfo& info)
{
	const char* path = Converter::CPlusNew(localPath, CultureInfo::Current().CPlusLocale);
	if (!path)
	{
		return AudioDecoderError::InvalidPath;
	}

	if (!experimental::filesystem::exists(experimental::filesystem::path(path)))
	{
		SAFE_DELETE(path);
		return AudioDecoderError::InvalidPath;
	}

	SNDFILE* File;
	SNDStream* Handle = new SNDStream();
	SF_INFO sfinfo;
	Handle->File = SNDAPI::sf_open(path, SFM_READ, &sfinfo);
	if (Handle->File == NULL)
	{
		SAFE_DELETE(Handle);
		SAFE_DELETE(path);
		return AudioDecoderError::UnSupported;
	}

	switch (sfinfo.channels)
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
	
	if ((sfinfo.format & SF_FORMAT_PCM_S8) == SF_FORMAT_PCM_S8)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt8;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_U8) == SF_FORMAT_PCM_U8)
	{
		Handle->Info.BitsPerSample = BitsPerSample::UInt8;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_16) == SF_FORMAT_PCM_16)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt16LE;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_24) == SF_FORMAT_PCM_24)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt24LE;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_32) == SF_FORMAT_PCM_32)
	{
		Handle->Info.BitsPerSample = BitsPerSample::SInt32LE;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_32) == SF_FORMAT_FLOAT)
	{
		Handle->Info.BitsPerSample = BitsPerSample::Float32LE;
	}
	else if ((sfinfo.format & SF_FORMAT_PCM_32) == SF_FORMAT_DOUBLE)
	{
		Handle->Info.BitsPerSample = BitsPerSample::Float64LE;
	}

	Handle->Info.SampleLength = sfinfo.frames;
	Handle->Info.SampleRate = sfinfo.samplerate;
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->Info.BitRate = ((double)file_length(path)*8.0 / Handle->Info.TimeMs);
	Handle->DecodePosMs = 0.0;

	SAFE_DELETE(path);
	return AudioDecoderError::OK;
}

Result<UInt64, AudioDecoderError> AudioDecoder_SND::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples)
{
	Result<UInt64, AudioDecoderError> result(0ull, AudioDecoderError::End);
	try
	{
		void* pipePtr = pipe.get(sizeof(SNDStream));
		if (pipePtr)
		{
			SNDStream* m_SND = static_cast<SNDStream*>(pipePtr);
			if (m_SND)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done<desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples>512) NSamples = 512;
					UInt64 need = bytes + NSamples*m_SND->Info.BlockAlign;
					if (m_SND->Buffer)
					{
						if (m_SND->BufferSize<need)
						{
							char* oldmem = m_SND->Buffer;
							if ((m_SND->Buffer = (char*)realloc(m_SND->Buffer, need)) == NULL)
							{
								m_SND->Buffer = (char*)malloc(need);
								if (!m_SND->Buffer)
								{
									m_SND->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return result;
								}
								memcpy(m_SND->Buffer, oldmem, m_SND->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_SND->Buffer = (char*)malloc(need);
						m_SND->BufferSize = need;
					}
					if (!m_SND->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return result;
					}
					size_t tbytes = 0;
					switch (m_SND->outbps)
					{
					case 8:
						tbytes = SNDAPI::sf_read_raw(m_SND->File, ((UChar*)m_SND->Buffer) + Done*m_SND->Info.BlockAlign, NSamples*m_SND->Info.Channels);
						break;
					case 16:
						tbytes = SNDAPI::sf_read_short(m_SND->File, (Int16*)(((UChar*)m_SND->Buffer) + Done*m_SND->Info.BlockAlign), NSamples*m_SND->Info.Channels)* sizeof(short);
						break;
					case 32:
						tbytes = SNDAPI::sf_read_int(m_SND->File, (Int32*)(((UChar*)m_SND->Buffer) + Done*m_SND->Info.BlockAlign), NSamples*m_SND->Info.Channels)* sizeof(int);
						break;
					case -32:
						tbytes = SNDAPI::sf_read_float(m_SND->File, (float*)(((UChar*)m_SND->Buffer) + Done*m_SND->Info.BlockAlign), NSamples*m_SND->Info.Channels)* sizeof(float);
						break;
					case -64:
						tbytes = SNDAPI::sf_read_double(m_SND->File, (double*)(((UChar*)m_SND->Buffer) + Done*m_SND->Info.BlockAlign), NSamples*m_SND->Info.Channels)* sizeof(double);
						break;
					}
					
					bytes += tbytes;
					Done += (tbytes / m_SND->Info.BlockAlign);
					if (tbytes==0)
					{
						end = true;
						break;
					}
				}
				buffer.SetData(m_SND->Buffer, m_SND->Info.SampleRate, m_SND->Info.BitsPerSample, Done, m_SND->Info.ChannelLayout, m_SND->DecodePosMs);
				sf_count_t  count = SNDAPI::sf_seek(m_SND->File, (sf_count_t)0, SEEK_CUR);
				if (count != PSF_SEEK_ERROR)
				{
					m_SND->DecodePosMs = count*1000.0 / m_SND->Info.SampleRate;
				}
				else
				{
					m_SND->DecodePosMs += (double)Done*1000.0 / (double)m_SND->Info.SampleRate;
				}
				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...){}
	return result;

}

bool AudioDecoder_SND::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(SNDStream));
		if (pipePtr)
		{
			SNDStream* m_SND = static_cast<SNDStream*>(pipePtr);
			if (m_SND)
			{
				sf_count_t  count = SNDAPI::sf_seek(m_SND->File, (sf_count_t)(m_SND->Info.SampleRate / 1000.0 * posMS), 0);
				if (count != PSF_SEEK_ERROR)
				{
					m_SND->DecodePosMs = count*1000.0 / m_SND->Info.SampleRate;
					return true;
				}
			}
		}
	}
	catch (...){}
	return false;
}

bool AudioDecoder_SND::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(SNDStream));
		if (pipePtr)
		{
			SNDStream* m_SND = (SNDStream*)(pipePtr);
			if (m_SND)
			{
				if (m_SND->File)
				{
					int res = SNDAPI::sf_close(m_SND->File);
					m_SND->File = nullptr;
				}
				if (m_SND->Buffer){ free(m_SND->Buffer); m_SND->Buffer = NULL; }
				SAFE_DELETE(m_SND);
				pipeNum--;
			}
		}
	}
	catch (...){}
	return true;
}

double AudioDecoder_SND::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(SNDStream));
		if (pipePtr)
		{
			SNDStream* m_SND = static_cast<SNDStream*>(pipePtr);
			if (m_SND) return m_SND->DecodePosMs;
		}
	}
	catch (...){}
	return 0.0;
}
