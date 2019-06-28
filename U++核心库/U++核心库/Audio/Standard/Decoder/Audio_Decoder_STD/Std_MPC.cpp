#include "Std_MPC.h"
#include "mpc\mpcdec.h"
#include "mpc\minimax.h"
#include <stdio.h>
#include <string>
#include <filesystem>
using namespace System::Audio;

struct MPC_Stream :ZeroStruct<MPC_Stream>
{
	AudioInfo Info;
	mpc_streaminfo si;
	mpc_reader reader;
	mpc_demux* demux;
	MPC_SAMPLE_FORMAT Buffers[MPC_DECODER_BUFFER_LENGTH];
	size_t BufferValidStart;
	size_t BufferValidCount;
	char* Buffer;
	size_t BufferSize;
	Int64 DecodePosMs;
};

Std_MPC::Std_MPC()
{

}

Std_MPC::~Std_MPC()
{
}

std::vector<std::wstring> Std_MPC::GetSupportFormats() const
{
	return { L".mpc", L".mp+" };
}

void Std_MPC::OpenLocal(const wchar_t* localPath, const char* path, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	MPC_Stream* Handle = new MPC_Stream();
	if (mpc_reader_init_stdio(&Handle->reader, path) != mpc_status::MPC_STATUS_OK)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return;
	}

	Handle->demux = mpc_demux_init(&Handle->reader);
	if (!Handle->demux)
	{
		mpc_reader_exit_stdio(&Handle->reader);
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return;
	}
	mpc_demux_get_info(Handle->demux, &Handle->si);
	Handle->Info.SampleRate = Handle->si.sample_freq;
	switch (Handle->si.channels)
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
	Handle->Info.Codec = Codec::Musepack;
	Handle->Info.BitRate = Handle->si.average_bitrate / 1000.0;
	Handle->Info.SampleLength = Handle->si.samples;
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->DecodePosMs = 0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
}

void Std_MPC::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	result.Value = nullptr;
	result.State = AudioDecoderError::UnSupported;
}

bool Std_MPC::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPC_Stream));
		if (pipePtr)
		{
			MPC_Stream* m_MPC = static_cast<MPC_Stream*>(pipePtr);
			if (m_MPC) return false;
		}
	}
	catch (...) {}
	return false;
}

bool Std_MPC::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPC_Stream));
		if (pipePtr)
		{
			MPC_Stream* m_MPC = static_cast<MPC_Stream*>(pipePtr);
			if (m_MPC)
			{
				info = m_MPC->Info;
				return true;
			}
		}
	}
	catch (...) {}
	return false;

}

AudioDecoderError Std_MPC::TryGetLocalAudioInfo(const wchar_t * localPath, const char* path, AudioInfo& info)
{
	mpc_streaminfo m_si;
	mpc_reader m_reader;
	mpc_demux* m_demux;

	if (mpc_reader_init_stdio(&m_reader, path) == mpc_status::MPC_STATUS_OK)
	{
		m_demux = mpc_demux_init(&m_reader);
		if (!m_demux)
		{
			mpc_reader_exit_stdio(&m_reader);
			return AudioDecoderError::UnSupported;
		}

		mpc_demux_get_info(m_demux, &m_si);
		info.SampleRate = m_si.sample_freq;
		switch (m_si.channels)
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
		info.Codec = Codec::Musepack;
		info.BitRate = m_si.average_bitrate / 1000.0;
		info.SampleLength = m_si.samples;
		info.TimeSec = (double)info.SampleLength / info.SampleRate;
		info.TimeMs = info.TimeSec * 1000;

		mpc_demux_exit(m_demux);
		mpc_reader_exit_stdio(&m_reader);
		return AudioDecoderError::OK;
	}
	return AudioDecoderError::UnSupported;
}

void Std_MPC::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	result.Value = 0ull;
	result.State = AudioDecoderError::End;
	try
	{
		void* pipePtr = pipe.get(sizeof(MPC_Stream));
		if (pipePtr)
		{
			MPC_Stream* m_MPC = static_cast<MPC_Stream*>(pipePtr);
			if (m_MPC)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done < desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples > MPC_FRAME_LENGTH) NSamples = MPC_FRAME_LENGTH;
					UInt64 need = bytes + NSamples * m_MPC->Info.BlockAlign;

					if (m_MPC->Buffer)
					{
						if (m_MPC->BufferSize < need)
						{
							char* oldmem = m_MPC->Buffer;
							m_MPC->BufferSize = need;
							if ((m_MPC->Buffer = (char*)realloc(m_MPC->Buffer, need)) == NULL)
							{
								m_MPC->Buffer = (char*)malloc(need);
								if (!m_MPC->Buffer)
								{
									m_MPC->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return;
								}
								memcpy(m_MPC->Buffer, oldmem, m_MPC->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_MPC->Buffer = (char*)malloc(need);
						m_MPC->BufferSize = need;
					}
					if (!m_MPC->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return;
					}

					if (m_MPC->BufferValidCount >= NSamples)
					{
						size_t nbytes = NSamples * m_MPC->Info.BlockAlign;
						memcpy(m_MPC->Buffer + Done * m_MPC->Info.BlockAlign, m_MPC->Buffers + m_MPC->BufferValidStart * m_MPC->Info.Channels, nbytes);
						m_MPC->BufferValidStart += NSamples;
						m_MPC->BufferValidCount -= NSamples;
						Done += NSamples;
					}
					else
					{
						if (m_MPC->BufferValidCount > 0)
						{
							size_t nbytes = m_MPC->BufferValidCount *m_MPC->Info.BlockAlign;
							memcpy(m_MPC->Buffer + Done * m_MPC->Info.BlockAlign, m_MPC->Buffers + m_MPC->BufferValidStart * m_MPC->Info.Channels, nbytes);
							Done += m_MPC->BufferValidCount;
							NSamples -= m_MPC->BufferValidCount;
							m_MPC->BufferValidStart = 0;
							m_MPC->BufferValidCount = 0;
						}
						mpc_frame_info frame;
						frame.buffer = m_MPC->Buffers;

						for (int m = 0; m < MPC_DECODER_BUFFER_LENGTH; m++)
						{
							m_MPC->Buffers[m] = 0.0f;
						}

						mpc_demux_decode(m_MPC->demux, &frame);
						m_MPC->BufferValidStart = 0;
						m_MPC->BufferValidCount = frame.samples;
						if (frame.samples <= 0 || frame.bits == -1)
						{
							end = true;
							break;
						}
					}
				}
				buffer.SetData(m_MPC->Buffer, m_MPC->Info.SampleRate, m_MPC->Info.BitsPerSample, Done, m_MPC->Info.ChannelLayout, m_MPC->DecodePosMs);
				m_MPC->DecodePosMs += Done * 1000.0 / m_MPC->Info.SampleRate;
				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...) {}
}

bool Std_MPC::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPC_Stream));
		if (pipePtr)
		{
			MPC_Stream* m_MPC = static_cast<MPC_Stream*>(pipePtr);
			if (m_MPC)
			{
				if (mpc_demux_seek_second(m_MPC->demux, posMS / 1000.0) == mpc_status::MPC_STATUS_OK)
				{
					m_MPC->BufferValidStart = 0;
					m_MPC->BufferValidCount = 0;
					m_MPC->DecodePosMs = posMS;
					return true;
				}
			}
		}
	}
	catch (...) {}
	return false;
}

bool Std_MPC::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPC_Stream));
		if (pipePtr)
		{
			MPC_Stream* m_MPC = (MPC_Stream*)(pipePtr);
			if (m_MPC)
			{
				if (m_MPC->demux)
				{
					mpc_reader_exit_stdio(&m_MPC->reader);
					mpc_demux_exit(m_MPC->demux);
					m_MPC->demux = 0;
					if (m_MPC->Buffer) { free(m_MPC);  m_MPC = nullptr; }
				}
				SAFE_DELETE(m_MPC);
				true;
			}
		}
	}
	catch (...) {}
	return false;
}

double Std_MPC::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPC_Stream));
		if (pipePtr)
		{
			MPC_Stream* m_MPC = static_cast<MPC_Stream*>(pipePtr);
			if (m_MPC) return m_MPC->DecodePosMs;
		}
	}
	catch (...) {}
	return 0.0;
}
