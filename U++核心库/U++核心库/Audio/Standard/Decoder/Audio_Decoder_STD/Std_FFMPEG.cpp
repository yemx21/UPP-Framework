#include <string>
#include <filesystem>
#include "Std_FFMPEG.h"
#include "FFMPEGAPI.h"

using namespace System::Audio;

class FFMPEGStream
{
public:
	AVCodec* Codec;
	AVCodecContext* Ctx;
	AVFormatContext* FmtCtx;
	AVPacket Avpkt;
	AVFrame* Frame;
	int StreamIndex;


	AudioInfo Info;
	Int64 DecodePosMs;
	bool InterleavedBuffer;
	char** Buffer;
	int BufferChs;
	size_t BufferSizePerCh;
	size_t BufferValidSizePerCh;
	int BufferSamples;

	FFMPEGStream():Codec(nullptr), Ctx(nullptr), FmtCtx(nullptr), Frame(nullptr), InterleavedBuffer(true), Buffer(nullptr), BufferChs(0), BufferSamples(0), BufferSizePerCh(0ull), BufferValidSizePerCh(0ull), DecodePosMs(0ll)
	{

	}

	~FFMPEGStream()
	{
		if (Buffer)
		{
			for (int ch = 0; ch < BufferChs; ch++)
			{
				SAFE_DELETE(Buffer[ch]);
			}
			SAFE_DELETE1(Buffer);
		}

		if (FmtCtx)
		{
			FFMPEGAPI::avformat_close_input(&FmtCtx);
			FFMPEGAPI::avformat_free_context(FmtCtx);
		}
	}
};

Std_FFMPEG::Std_FFMPEG()
{
	
}

Std_FFMPEG::~Std_FFMPEG()
{
}

std::vector<std::wstring> Std_FFMPEG::GetSupportFormats() const
{
	return { L".wma", L".m4a", L".mp3", L".wav", L".aiff"};
}

void Std_FFMPEG::OpenLocal(const wchar_t* localPath, const char* path, Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	FFMPEGStream* Handle = new FFMPEGStream();
	
	Handle->FmtCtx = FFMPEGAPI::avformat_alloc_context();

	if (FFMPEGAPI::avformat_open_input(&Handle->FmtCtx, path, NULL, NULL) < 0)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return;
	}

	if (FFMPEGAPI::avformat_find_stream_info(Handle->FmtCtx, NULL) < 0)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		result.WriteMessage(L"can not find stream");
		return;
	}

	Handle->StreamIndex = FFMPEGAPI::av_find_best_stream(Handle->FmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &Handle->Codec, 0);

	if (Handle->StreamIndex < 0)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		result.WriteMessage(L"can not find codec");
		return;
	}

	auto stream = Handle->FmtCtx->streams[Handle->StreamIndex];

	Handle->Ctx = FFMPEGAPI::avcodec_alloc_context3(Handle->Codec);

	FFMPEGAPI::avcodec_parameters_to_context(Handle->Ctx, stream->codecpar);
	FFMPEGAPI::av_opt_set_int(Handle->Ctx, "refcounted_frames", 1, 0);
	Handle->Ctx->request_sample_fmt= FFMPEGAPI::av_get_alt_sample_fmt(Handle->Ctx->sample_fmt, 0);

	if (FFMPEGAPI::avcodec_open2(Handle->Ctx, Handle->Codec, NULL) < 0)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		result.WriteMessage(L"can not open codec");
		return;
	}

	Handle->Frame = FFMPEGAPI::av_frame_alloc();
	if (!Handle->Frame)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::Failed;
		result.WriteMessage(L"can not alloc frame");
		return;
	}

	Handle->Info.SampleRate = Handle->Ctx->sample_rate;
	switch (Handle->Ctx->channels)
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

	switch (Handle->Ctx->sample_fmt)
	{
	case AV_SAMPLE_FMT_FLT:
		Handle->Info.BitsPerSample = BitsPerSample::Float32;
		//Handle->InterleavedBuffer = true;
		break;
	}

	Handle->InterleavedBuffer = !FFMPEGAPI::av_sample_fmt_is_planar(Handle->Ctx->sample_fmt);
	if (Handle->InterleavedBuffer)
	{
		Handle->Buffer = new char*[1];
		Handle->Buffer[0] = nullptr;
	}


	//Handle->Buffer = AVCODEC_MAX_AUDIO_FRAME_SIZE + AV_INPUT_BUFFER_PADDING_SIZE;

	//Handle->Info.BitsPerSample = BitsPerSample::SInt16;
	Handle->Info.Codec = Codec::Unknown;
	Handle->Info.BitRate = Handle->Ctx->bit_rate;

	double divideFactor;
	divideFactor = 1.0 / av_q2d(stream->time_base);

	Handle->Info.TimeSec = (double)(stream->duration / divideFactor);
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->Info.SampleLength = Handle->Info.TimeMs* Handle->Info.SampleRate* Handle->Info.Channels;
	
	Handle->DecodePosMs = 0;

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
}

void Std_FFMPEG::CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& result)
{
	result.Value = nullptr;
	result.State = AudioDecoderError::UnSupported;
}

bool Std_FFMPEG::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FFMPEGStream));
		if (pipePtr)
		{
			FFMPEGStream* m_FFMPEG = static_cast<FFMPEGStream*>(pipePtr);
			if (m_FFMPEG) return false;
		}
	}
	catch (...) {}
	return false;
}

bool Std_FFMPEG::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FFMPEGStream));
		if (pipePtr)
		{
			FFMPEGStream* m_FFMPEG = static_cast<FFMPEGStream*>(pipePtr);
			if (m_FFMPEG)
			{
				info = m_FFMPEG->Info;
				return true;
			}
		}
	}
	catch (...) {}
	return false;

}

AudioDecoderError Std_FFMPEG::TryGetLocalAudioInfo(const wchar_t * localPath, const char* path, AudioInfo& info)
{
	/*FFMPEGStream* Handle = new FFMPEGStream();
	int errorval;
	Handle->m_pFFMPEGDecompress = CreateIFFMPEGDecompress(localPath, &errorval);
	if (!Handle->m_pFFMPEGDecompress)
	{
		SAFE_DELETE(Handle->m_pFFMPEGDecompress);
		SAFE_DELETE(Handle);
		return AudioDecoderError::UnSupported;
	}

	info.ChannelLayout = ChannelLayout::GetDefaultLayout(Handle->m_pFFMPEGDecompress->GetInfo(FFMPEG_INFO_CHANNELS));


	switch (Handle->m_pFFMPEGDecompress->GetInfo(FFMPEG_INFO_BITS_PER_SAMPLE))
	{
	case 8u:
		info.BitsPerSample = BitsPerSample::SInt8;
		break;
	case 16u:
		info.BitsPerSample = BitsPerSample::SInt16LE;
		break;
	case 24u:
		info.BitsPerSample = BitsPerSample::SInt24LE;
		break;
	case 32u:
	{
		info.BitsPerSample = BitsPerSample::SInt32LE;
	}
	break;
	}
	info.Codec = Codec::MonkeyAudio;
	info.TimeMs = Handle->m_pFFMPEGDecompress->GetInfo(FFMPEG_INFO_LENGTH_MS);
	info.SampleLength = Handle->m_pFFMPEGDecompress->GetInfo(FFMPEG_DECOMPRESS_TOTAL_BLOCKS);
	info.SampleRate = Handle->m_pFFMPEGDecompress->GetInfo(FFMPEG_INFO_SAMPLE_RATE);
	info.TimeSec = info.TimeMs / 1000.0;
	info.BitRate = Handle->m_pFFMPEGDecompress->GetInfo(FFMPEG_INFO_AVERAGE_BITRATE);

	SAFE_DELETE(Handle->m_pFFMPEGDecompress);
	SAFE_DELETE(Handle);*/
	return AudioDecoderError::OK;
}

static void HandleFrame(FFMPEGStream* stream)
{
	auto num = stream->Frame->nb_samples;
	if (num < 1) return;
	if (stream->Info.Channels != stream->Frame->channels) return;
	if (stream->InterleavedBuffer)
	{
		size_t framebufsize = num * stream->Info.BlockAlign;
		size_t needbytesperch = stream->BufferValidSizePerCh + framebufsize;
		if (stream->BufferSizePerCh < needbytesperch)
		{
			if (stream->Buffer[0])
			{
				char* oldmem = stream->Buffer[0];
				if ((stream->Buffer[0] = (char*)realloc(stream->Buffer[0], needbytesperch)) == NULL)
				{
					stream->Buffer[0] = (char*)malloc(needbytesperch);
					if (!stream->Buffer[0])
					{
						stream->BufferSizePerCh = 0;
						stream->BufferValidSizePerCh = 0;
						return;
					}
					memcpy(stream->Buffer[0], oldmem, stream->BufferSizePerCh);
					free(oldmem);
					stream->BufferSizePerCh = needbytesperch;
				}
			}
			else
			{
				stream->Buffer[0] = (char*)malloc(needbytesperch);
				stream->BufferSizePerCh = needbytesperch;
			}
		}
		memcpy(stream->Buffer[0] + stream->BufferValidSizePerCh, stream->Frame->extended_data[0], framebufsize);
		stream->BufferValidSizePerCh += framebufsize;
		stream->BufferSamples = stream->BufferValidSizePerCh / stream->Info.BlockAlign;
		double divideFactor;
		divideFactor = 1.0 / av_q2d(stream->Ctx->time_base);

		stream->DecodePosMs = 1000.0* stream->Frame->pkt_pts / divideFactor;
	}
	else
	{
		
	}
}

static int ReceiveHandle(FFMPEGStream* stream)
{
	int err = 0;
	while ((err = FFMPEGAPI::avcodec_receive_frame(stream->Ctx, stream->Frame)) == 0)
	{
		HandleFrame(stream);
		FFMPEGAPI::av_frame_unref(stream->Frame);
	}
	return err;
}

static bool FillBuffer(FFMPEGStream* stream, int samples, bool& eof)
{
	bool needcleanup = false;
	int err = 0;

	while (stream->BufferSamples< samples)
	{
		err = FFMPEGAPI::av_read_frame(stream->FmtCtx, &stream->Avpkt);
		if (err == AVERROR_EOF)
		{
			eof = true;
			break;
		}
		if (err != 0)
		{
			FFMPEGAPI::av_packet_unref(&stream->Avpkt);
			continue;
		}

		if (stream->Avpkt.stream_index != stream->StreamIndex)
		{
			FFMPEGAPI::av_packet_unref(&stream->Avpkt);
			continue;
		}

		if ((err = FFMPEGAPI::avcodec_send_packet(stream->Ctx, &stream->Avpkt)) == 0)
			FFMPEGAPI::av_packet_unref(&stream->Avpkt);
		else
		{
			needcleanup = true;
			break;
		}

		if ((err = ReceiveHandle(stream)) != AVERROR(EAGAIN))
		{
			needcleanup = true;
			break;
		}
	}

	if (needcleanup)
	{

	}

	return stream->BufferSamples > 0;
}

static int WriteBuffer(FFMPEGStream* stream, int samples, AudioBuffer& buffer)
{
	int written = stream->BufferSamples < samples ? stream->BufferSamples : samples;
	int left = stream->BufferSamples - written;
	if (stream->InterleavedBuffer)
	{
		buffer.AddData(stream->Buffer[0], written);
		if (left>0)
		{
			memcpy(stream->Buffer[0], stream->Buffer[0] + written * stream->Info.BlockAlign, left*stream->Info.BlockAlign);
		}
		stream->BufferValidSizePerCh -= written * stream->Info.BlockAlign;
		stream->BufferSamples -= written;
	}
	return written;
}

void Std_FFMPEG::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& result)
{
	result.Value = 0ull;
	result.State = AudioDecoderError::End;
	try
	{
		void* pipePtr = pipe.get(sizeof(FFMPEGStream));
		if (pipePtr)
		{
			FFMPEGStream* m_FFMPEG = static_cast<FFMPEGStream*>(pipePtr);
			if (m_FFMPEG)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				buffer.StartResetData(m_FFMPEG->Info.SampleRate, m_FFMPEG->Info.BitsPerSample, m_FFMPEG->Info.ChannelLayout, m_FFMPEG->DecodePosMs);

				while (Done < desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples > 512) NSamples = 512;
					FillBuffer(m_FFMPEG, NSamples, end);
					Done += WriteBuffer(m_FFMPEG, NSamples, buffer);
				}
				buffer.CloseResetData(false);

				//m_FFMPEG->DecodePosMs = m_FFMPEG->m_pFFMPEGDecompress->GetInfo(FFMPEG_DECOMPRESS_CURRENT_MS);

				result.Value = Done;
				result.State = end ? (m_FFMPEG->BufferSamples? AudioDecoderError::OK: AudioDecoderError::End) : AudioDecoderError::OK;
			}
		}
	}
	catch (...) {}
}

bool Std_FFMPEG::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FFMPEGStream));
		if (pipePtr)
		{
			FFMPEGStream* m_FFMPEG = static_cast<FFMPEGStream*>(pipePtr);
			if (m_FFMPEG)
			{
				Int64 cc = m_FFMPEG->Info.SampleLength*posMS / (m_FFMPEG->Info.TimeMs);
				//if (m_FFMPEG->m_pFFMPEGDecompress->Seek(cc) == ERROR_SUCCESS)
				//{
					//m_FFMPEG->DecodePosMs = m_FFMPEG->m_pFFMPEGDecompress->GetInfo(FFMPEG_DECOMPRESS_CURRENT_MS);
					//return true;
				//}
				//else
					//return false;
			}
		}
	}
	catch (...) {}
	return false;
}

bool Std_FFMPEG::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FFMPEGStream));
		if (pipePtr)
		{
			FFMPEGStream* m_FFMPEG = (FFMPEGStream*)(pipePtr);
			SAFE_DELETE(m_FFMPEG);
			return true;
		}
	}
	catch (...) {}
	return false;
}

double Std_FFMPEG::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(FFMPEGStream));
		if (pipePtr)
		{
			FFMPEGStream* m_FFMPEG = static_cast<FFMPEGStream*>(pipePtr);
			if (m_FFMPEG) return m_FFMPEG->DecodePosMs;
		}
	}
	catch (...) {}
	return 0.0;
}
