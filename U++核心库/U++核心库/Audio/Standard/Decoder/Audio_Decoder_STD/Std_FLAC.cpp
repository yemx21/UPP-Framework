#include "Std_FLAC.h"
#include <stdio.h>
#include <string>
#include <filesystem>
#include "FLAC.h"
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;

class FLAC_Wrapper
{
public:
	FLAC_Stream* Stream;
	int DecodePosMs;
	AudioInfo Info;

	FLAC_Wrapper()
	{
		Stream = new FLAC_Stream();
	}

	~FLAC_Wrapper()
	{
		if (Stream)
		{
			if (Stream->Decoder)
			{
				Stream->FLAC_plugin__decoder_finish(Stream->Decoder);
				Stream->FLAC_plugin__decoder_delete(Stream->Decoder);
				Stream->Decoder = nullptr;
			}
		}
		SAFE_DELETE(Stream);
	}
};

Std_FLAC::Std_FLAC()
{

}

Std_FLAC::~Std_FLAC()
{

}

std::vector<std::wstring> Std_FLAC::GetSupportFormats() const
{
	return { L".flac", L".fla" };
}

__forceinline Int64 FileSize(const char *fileName)
{
	auto hFile = Microsoft::winapi_CreateFile_ANSI(fileName, API_GENERIC_READ, API_FILE_SHARE_READ | API_FILE_SHARE_WRITE, NULL, API_OPEN_EXISTING, API_FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_API_PTR) return 0;
	auto ret = Microsoft::winapi_GetFileSize(hFile);
	Microsoft::winapi_CloseHandle(hFile);
	return ret;
}

void Std_FLAC::OpenLocal(const wchar_t* nouse, const char* path, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	FLAC_Wrapper* Handle = new FLAC_Wrapper();
	Handle->Stream->Decoder = FLAC__stream_decoder_new();
	if (!Handle->Stream->Decoder)
	{ 
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::Failed;
		return;
	}
	auto filesize = FileSize(path);

	if (!Handle->Stream->FLAC_plugin__decoder_init(Handle->Stream->Decoder, path, filesize, &Handle->Stream->StreamData))
	{
		SAFE_DELETE(Handle); 
		result.State = AudioDecoderError::UnSupported;
		return;
	}

	Handle->Info.SampleRate = Handle->Stream->StreamData.sample_rate;
	Handle->Info.ChannelLayout = ChannelLayout::GetDefaultLayout(Handle->Stream->StreamData.channels);
	

	Handle->Info.BitsPerSample = BitsPerSample::GetOSBitsPerSample(Handle->Stream->StreamData.bits_per_sample);
	Handle->Info.Codec = Codec::FLAC;
	Handle->Info.BitRate = Handle->Stream->StreamData.average_bps;
	Handle->Info.SampleLength = Handle->Stream->StreamData.total_samples;
	Handle->Info.TimeSec = Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;

	Handle->DecodePosMs = 0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
}

void Std_FLAC::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	result.Value = nullptr;
	result.State = AudioDecoderError::UnSupported;
}

bool Std_FLAC::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FLAC_Wrapper));
		if (pipePtr)
		{
			FLAC_Wrapper* m_Flac = static_cast<FLAC_Wrapper*>(pipePtr);
			if (m_Flac) return false;
		}
	}
	catch (...){}
	return false;
}

bool Std_FLAC::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FLAC_Wrapper));
		if (pipePtr)
		{
			FLAC_Wrapper* m_Flac = static_cast<FLAC_Wrapper*>(pipePtr);
			if (m_Flac)
			{
				info = m_Flac->Info;
				return true;
			}
		}
	}
	catch (...){}
	return false;

}

AudioDecoderError Std_FLAC::TryGetLocalAudioInfo(const wchar_t* nouse, const char* path, AudioInfo& info)
{
	FLAC_Wrapper* Handle = new FLAC_Wrapper();
	Handle->Stream->Decoder = FLAC__stream_decoder_new();
	if (!Handle->Stream->Decoder)
	{
		SAFE_DELETE(Handle);
		return AudioDecoderError::Failed;
	}
	auto filesize = FileSize(path);

	if (!Handle->Stream->FLAC_plugin__decoder_init(Handle->Stream->Decoder, path, filesize, &Handle->Stream->StreamData))
	{
		SAFE_DELETE(Handle);
		return AudioDecoderError::UnSupported;
	}

	info.ChannelLayout = ChannelLayout::GetDefaultLayout(Handle->Stream->StreamData.channels);
	info.SampleRate = Handle->Stream->StreamData.sample_rate;
	info.BitsPerSample = BitsPerSample::GetOSBitsPerSample(Handle->Stream->StreamData.bits_per_sample);
	info.Codec = Codec::FLAC;
	info.BitRate = Handle->Stream->StreamData.average_bps;
	info.SampleLength = Handle->Stream->StreamData.total_samples;
	info.TimeSec = Handle->Info.SampleLength / Handle->Info.SampleRate;
	info.TimeMs = Handle->Info.TimeSec * 1000;

	SAFE_DELETE(Handle);
	return AudioDecoderError::OK;
}

void Std_FLAC::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	result.Value = 0ull;
	result.State = AudioDecoderError::End;
	try
	{
		void* pipePtr = pipe.get(sizeof(FLAC_Wrapper));
		if (pipePtr)
		{
			FLAC_Wrapper* m_Flac = static_cast<FLAC_Wrapper*>(pipePtr);
			if (m_Flac)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				auto sres = buffer.StartResetData(m_Flac->Info.SampleRate, m_Flac->Info.BitsPerSample, m_Flac->Info.ChannelLayout, m_Flac->DecodePosMs);
				if (sres.Value)
				{
					while (Done < desiredNumSamples)
					{
						size_t num = desiredNumSamples - Done;
						if (num > SAMPLES_PER_WRITE) num = SAMPLES_PER_WRITE;
						if (m_Flac->Stream->StreamData.eof)
						{
							result.State = AudioDecoderError::End;
							buffer.CloseResetData();
							return;
						}
						
						size_t ret = m_Flac->Stream->FLAC_plugin__decode(m_Flac->Stream->Decoder, &m_Flac->Stream->StreamData, [&buffer](void* lambda_data, UInt lambda_samples) {buffer.AddData(lambda_data, lambda_samples); }, num);
						if (ret > 0)
						{
							Done += ret;
						}
					}
					buffer.CloseResetData();
				}
				else
				{
					buffer.CloseResetData();
					result.State = AudioDecoderError::Failed;
					result.WriteMessage(sres.Message);
				}
				m_Flac->DecodePosMs = m_Flac->Stream->StreamData.decode_position * 1000.0 / (double)m_Flac->Info.AvgBytesPerSec;
				result.Value = Done;
				result.State = m_Flac->Stream->StreamData.eof ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...){}
}

bool Std_FLAC::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FLAC_Wrapper));
		if (pipePtr)
		{
			FLAC_Wrapper* m_Flac = static_cast<FLAC_Wrapper*>(pipePtr);
			if (m_Flac)
			{
				m_Flac->Stream->StreamData.seek_to = posMS;
				if (m_Flac->Stream->FLAC_plugin__seek(m_Flac->Stream->Decoder, &m_Flac->Stream->StreamData) != -1)
				{
					m_Flac->DecodePosMs = m_Flac->Stream->StreamData.decode_position * 1000.0 / (double)m_Flac->Info.AvgBytesPerSec;
					return true;
				}
			}
		}
	}
	catch (...){}
	return false;
}

bool Std_FLAC::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FLAC_Wrapper));
		if (pipePtr)
		{
			FLAC_Wrapper* m_Flac = (FLAC_Wrapper*)(pipePtr);
			if (m_Flac)
			{
				SAFE_DELETE(m_Flac);
			}
		}
	}
	catch (...){}
	return true;
}

double Std_FLAC::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FLAC_Wrapper));
		if (pipePtr)
		{
			FLAC_Wrapper* m_Flac = static_cast<FLAC_Wrapper*>(pipePtr);
			if (m_Flac) return m_Flac->DecodePosMs;
		}
	}
	catch (...){}
	return 0.0;
}
