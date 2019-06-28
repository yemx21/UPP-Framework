#include <string>
#include <filesystem>
#include "MACAll.h"
#include "MACLib.h"
#include "APETag.h"
#include "Std_APE.h"

using namespace System::Audio;
using namespace APE;


struct APEStream :ZeroStruct<APEStream>
{
	IAPEDecompress* m_pAPEDecompress;
	AudioInfo Info;
	Int64 DecodePosMs;
	char* Buffer;
	size_t BufferSize;
};

Std_APE::Std_APE()
{

}

Std_APE::~Std_APE()
{
}

std::vector<std::wstring> Std_APE::GetSupportFormats() const
{
	return { L".ape", L".mac" };
}

void Std_APE::OpenLocal(const wchar_t* localPath, const char* path, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	APEStream* Handle = new APEStream();
	int errorval;
	Handle->m_pAPEDecompress = CreateIAPEDecompress(localPath, &errorval);
	if (!Handle->m_pAPEDecompress)
	{
		SAFE_DELETE(Handle->m_pAPEDecompress);
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return;
	}

	Handle->Info.ChannelLayout = ChannelLayout::GetDefaultLayout(Handle->m_pAPEDecompress->GetInfo(APE_INFO_CHANNELS));

	switch (Handle->m_pAPEDecompress->GetInfo(APE_INFO_BITS_PER_SAMPLE))
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
		Handle->Info.BitsPerSample = BitsPerSample::SInt32;
	}
	break;
	}
	Handle->Info.Codec = Codec::MonkeyAudio;
	Handle->Info.TimeMs = Handle->m_pAPEDecompress->GetInfo(APE_INFO_LENGTH_MS);
	Handle->Info.SampleLength = Handle->m_pAPEDecompress->GetInfo(APE_DECOMPRESS_TOTAL_BLOCKS);
	Handle->Info.SampleRate = Handle->m_pAPEDecompress->GetInfo(APE_INFO_SAMPLE_RATE);
	Handle->Info.TimeSec = Handle->Info.TimeMs / 1000.0;
	Handle->Info.BitRate = Handle->m_pAPEDecompress->GetInfo(APE_INFO_AVERAGE_BITRATE);

	Handle->DecodePosMs = 0.0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;

}

void Std_APE::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	result.Value = nullptr;
	result.State = AudioDecoderError::UnSupported;
}

bool Std_APE::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(APEStream));
		if (pipePtr)
		{
			APEStream* m_APE = static_cast<APEStream*>(pipePtr);
			if (m_APE) return false;
		}
	}
	catch (...) {}
	return false;
}

bool Std_APE::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(APEStream));
		if (pipePtr)
		{
			APEStream* m_APE = static_cast<APEStream*>(pipePtr);
			if (m_APE)
			{
				info = m_APE->Info;
				return true;
			}
		}
	}
	catch (...) {}
	return false;

}

AudioDecoderError Std_APE::TryGetLocalAudioInfo(const wchar_t * localPath, const char* path, AudioInfo& info)
{
	APEStream* Handle = new APEStream();
	int errorval;
	Handle->m_pAPEDecompress = CreateIAPEDecompress(localPath, &errorval);
	if (!Handle->m_pAPEDecompress)
	{
		SAFE_DELETE(Handle->m_pAPEDecompress);
		SAFE_DELETE(Handle);
		return AudioDecoderError::UnSupported;
	}

	info.ChannelLayout = ChannelLayout::GetDefaultLayout(Handle->m_pAPEDecompress->GetInfo(APE_INFO_CHANNELS));


	switch (Handle->m_pAPEDecompress->GetInfo(APE_INFO_BITS_PER_SAMPLE))
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
		info.BitsPerSample = BitsPerSample::SInt32;
	}
	break;
	}
	info.Codec = Codec::MonkeyAudio;
	info.TimeMs = Handle->m_pAPEDecompress->GetInfo(APE_INFO_LENGTH_MS);
	info.SampleLength = Handle->m_pAPEDecompress->GetInfo(APE_DECOMPRESS_TOTAL_BLOCKS);
	info.SampleRate = Handle->m_pAPEDecompress->GetInfo(APE_INFO_SAMPLE_RATE);
	info.TimeSec = info.TimeMs / 1000.0;
	info.BitRate = Handle->m_pAPEDecompress->GetInfo(APE_INFO_AVERAGE_BITRATE);

	SAFE_DELETE(Handle->m_pAPEDecompress);
	SAFE_DELETE(Handle);
	return AudioDecoderError::OK;
}

void Std_APE::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	result.Value = 0ull;
	result.State = AudioDecoderError::End;
	try
	{
		void* pipePtr = pipe.get(sizeof(APEStream));
		if (pipePtr)
		{
			APEStream* m_APE = static_cast<APEStream*>(pipePtr);
			if (m_APE)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done < desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples > 512) NSamples = 512;
					UInt64 need = bytes + NSamples * m_APE->Info.BlockAlign;
					if (m_APE->Buffer)
					{
						if (m_APE->BufferSize < need)
						{
							char* oldmem = m_APE->Buffer;
							if ((m_APE->Buffer = (char*)realloc(m_APE->Buffer, need)) == NULL)
							{
								m_APE->Buffer = (char*)malloc(need);
								if (!m_APE->Buffer)
								{
									m_APE->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return;
								}
								memcpy(m_APE->Buffer, oldmem, m_APE->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_APE->Buffer = (char*)malloc(need);
						m_APE->BufferSize = need;
					}
					if (!m_APE->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return;
					}
					APE::MAC__intn nBlocksRetrieved = 0;

					int apeerr = m_APE->m_pAPEDecompress->GetData(((char*)m_APE->Buffer) + Done * m_APE->Info.BlockAlign, NSamples, &nBlocksRetrieved);
					size_t tbytes = nBlocksRetrieved * m_APE->Info.BlockAlign;

					if (tbytes == 0)
					{
						end = true;
						break;
					}
					bytes += tbytes;
					Done += (tbytes / m_APE->Info.BlockAlign);
				}
				buffer.SetData(m_APE->Buffer, m_APE->Info.SampleRate, m_APE->Info.BitsPerSample, Done, m_APE->Info.ChannelLayout, m_APE->DecodePosMs);

				m_APE->DecodePosMs = m_APE->m_pAPEDecompress->GetInfo(APE_DECOMPRESS_CURRENT_MS);

				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...) {}
}

bool Std_APE::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(APEStream));
		if (pipePtr)
		{
			APEStream* m_APE = static_cast<APEStream*>(pipePtr);
			if (m_APE)
			{
				Int64 cc = m_APE->Info.SampleLength*posMS / (m_APE->Info.TimeMs);
				if (m_APE->m_pAPEDecompress->Seek(cc) == ERROR_SUCCESS)
				{
					m_APE->DecodePosMs = m_APE->m_pAPEDecompress->GetInfo(APE_DECOMPRESS_CURRENT_MS);
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

bool Std_APE::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(APEStream));
		if (pipePtr)
		{
			APEStream* m_APE = (APEStream*)(pipePtr);
			if (m_APE)
			{
				SAFE_DELETE(m_APE->m_pAPEDecompress);
				if (m_APE->Buffer) { free(m_APE->Buffer); m_APE->Buffer = NULL; }
				SAFE_DELETE(m_APE);
				return true;
			}
		}
	}
	catch (...) {}
	return false;
}

double Std_APE::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(APEStream));
		if (pipePtr)
		{
			APEStream* m_APE = static_cast<APEStream*>(pipePtr);
			if (m_APE) return m_APE->DecodePosMs;
		}
	}
	catch (...) {}
	return 0.0;
}
