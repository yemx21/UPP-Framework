#include "Std_Vorbis.h"
#include "..\..\..\..\Third Party\libvorbis\include\vorbis\vorbisfile.h"

struct OGG_Stream :ZeroStruct<OGG_Stream>
{
	AudioInfo Info;
	OggVorbis_File* OF;
	Int64 DecodePosMs;
	char* Buffer;
	size_t BufferSize;
};

Std_Vorbis::Std_Vorbis()
{

}

Std_Vorbis::~Std_Vorbis()
{
}

std::vector<std::wstring> Std_Vorbis::GetSupportFormats() const
{
	return { L".ogg" };
}

void Std_Vorbis::OpenLocal(const wchar_t* nouse, const char* path, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	OGG_Stream* Handle = new OGG_Stream();
	Handle->OF = new OggVorbis_File();

	if (ov_fopen(path, Handle->OF) != 0)
	{
		SAFE_DELETE(Handle->OF);
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return;
	}

	Handle->Info.SampleRate = Handle->OF->vi->rate;
	switch (Handle->OF->vi->channels)
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
	Handle->Info.BitsPerSample = BitsPerSample::SInt16;
	Handle->Info.Codec = Codec::Vorbis;
	Handle->Info.BitRate = ov_bitrate(Handle->OF, -1) / 1000.0;
	Handle->Info.SampleLength = ov_pcm_total(Handle->OF, -1);
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->DecodePosMs = 0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;

}

void Std_Vorbis::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& ret)
{
	ret.Value = nullptr;
	ret.State = AudioDecoderError::UnSupported;
}

bool Std_Vorbis::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OGG_Stream));
		if (pipePtr)
		{
			OGG_Stream* m_OGG = static_cast<OGG_Stream*>(pipePtr);
			if (m_OGG) return false;
		}
	}
	catch (...) {}
	return false;
}

bool Std_Vorbis::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OGG_Stream));
		if (pipePtr)
		{
			OGG_Stream* m_OGG = static_cast<OGG_Stream*>(pipePtr);
			if (m_OGG)
			{
				info = m_OGG->Info;
				return true;
			}
		}
	}
	catch (...) {}
	return false;

}

AudioDecoderError Std_Vorbis::TryGetLocalAudioInfo(const wchar_t * nouse, const char* path, AudioInfo& info)
{
	OggVorbis_File* OF = new OggVorbis_File();

	if (ov_fopen(path, OF) != 0)
	{
		SAFE_DELETE(OF);
		return AudioDecoderError::UnSupported;
	}

	info.SampleRate = OF->vi->rate;
	switch (OF->vi->channels)
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
	info.BitsPerSample = BitsPerSample::SInt16;
	info.Codec = Codec::Vorbis;
	info.BitRate = ov_bitrate(OF, -1) / 1000.0;
	info.SampleLength = ov_pcm_total(OF, -1);
	info.TimeSec = (double)info.SampleLength / info.SampleRate;
	info.TimeMs = info.TimeSec * 1000;

	ov_clear(OF);
	SAFE_DELETE(OF);
	return AudioDecoderError::OK;
}

void Std_Vorbis::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OGG_Stream));
		if (pipePtr)
		{
			OGG_Stream* m_OGG = static_cast<OGG_Stream*>(pipePtr);
			if (m_OGG)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done < desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples > 512) NSamples = 512;
					UInt64 need = bytes + NSamples * m_OGG->Info.BlockAlign;

					if (m_OGG->Buffer)
					{
						if (m_OGG->BufferSize < need)
						{
							char* oldmem = m_OGG->Buffer;
							if ((m_OGG->Buffer = (char*)realloc(m_OGG->Buffer, need)) == NULL)
							{
								m_OGG->Buffer = (char*)malloc(need);
								if (!m_OGG->Buffer)
								{
									free(oldmem);
									m_OGG->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return;
								}
								memcpy(m_OGG->Buffer, oldmem, m_OGG->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_OGG->Buffer = (char*)malloc(need);
						m_OGG->BufferSize = need;
					}
					if (!m_OGG->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return;
					}

					size_t tbytes = 0;
					long ovres = 0;
					size_t bytes_read = 0;
					while (bytes_read < NSamples*m_OGG->Info.BlockAlign)
					{
						ovres = ov_read(m_OGG->OF, m_OGG->Buffer + Done * m_OGG->Info.BlockAlign + bytes_read, NSamples*m_OGG->Info.BlockAlign - bytes_read, 0, 2, 1, NULL);
						if (ovres == 0)
						{
							tbytes = bytes_read;
							end = true;
							break;
						}
						else if (ovres < 0) {}
						else bytes_read += ovres;
					}
					tbytes = bytes_read;
					Done += (tbytes / m_OGG->Info.BlockAlign);
					if (end)
						break;
				}
				buffer.SetData(m_OGG->Buffer, m_OGG->Info.SampleRate, m_OGG->Info.BitsPerSample, Done, m_OGG->Info.ChannelLayout, m_OGG->DecodePosMs);
				m_OGG->DecodePosMs = ov_time_tell(m_OGG->OF) * 1000.0;
				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...) {}
}

bool Std_Vorbis::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OGG_Stream));
		if (pipePtr)
		{
			OGG_Stream* m_OGG = static_cast<OGG_Stream*>(pipePtr);
			if (m_OGG)
			{
				if (ov_time_seek(m_OGG->OF, posMS / 1000.0) == 0)
				{
					m_OGG->DecodePosMs = ov_time_tell(m_OGG->OF) * 1000.0;
					return true;
				}
			}
		}
	}
	catch (...) {}
	return false;
}

bool Std_Vorbis::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OGG_Stream));
		if (pipePtr)
		{
			OGG_Stream* m_OGG = (OGG_Stream*)(pipePtr);
			if (m_OGG)
			{
				if (m_OGG->OF) ov_clear(m_OGG->OF);
				SAFE_DELETE(m_OGG->OF);
				if (m_OGG->Buffer) { free(m_OGG->Buffer); m_OGG->Buffer = NULL; }
				SAFE_DELETE(m_OGG);
				return true;
			}
		}
	}
	catch (...) {}
	return false;
}

double Std_Vorbis::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(OGG_Stream));
		if (pipePtr)
		{
			OGG_Stream* m_OGG = static_cast<OGG_Stream*>(pipePtr);
			if (m_OGG) return m_OGG->DecodePosMs;
		}
	}
	catch (...) {}
	return 0.0;
}
