#pragma once
#include "Config.h"
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libavutil\avutil.h"
#include "WindowsEnvironment.h"

using namespace System::OS;

namespace System
{
	namespace Audio
	{
		class FFMPEGAPI
		{
		public:
			static void(*avcodec_register_all)();
			static void(*av_init_packet)(AVPacket*);
			static AVCodec* (*avcodec_find_decoder)(enum AVCodecID id);
			static AVCodecContext* (*avcodec_alloc_context3)(const AVCodec *codec);
			static int(*avcodec_open2)(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
			static AVFrame* (*av_frame_alloc)();
			static void(*av_frame_unref)(AVFrame *frame);
			static int(*avcodec_decode_audio4)(AVCodecContext *avctx, AVFrame *frame, int *got_frame_ptr, const AVPacket *avpkt);
			static int(*av_samples_get_buffer_size)(int *linesize, int nb_channels, int nb_samples, enum AVSampleFormat sample_fmt, int align);
			static int(*avcodec_close)(AVCodecContext *avctx);
			static void(*av_free)(void *ptr);
			static void(*av_frame_free)(AVFrame **frame);
			static int(*av_read_frame)(AVFormatContext *s, AVPacket *pkt);
			static void(*av_packet_unref)(AVPacket *pkt);
			static int(*avcodec_send_packet)(AVCodecContext *avctx, const AVPacket *avpkt);
			static int(*avcodec_receive_frame)(AVCodecContext *avctx, AVFrame *frame);

			static enum AVSampleFormat (*av_get_alt_sample_fmt)(enum AVSampleFormat sample_fmt, int planar);
			static int(*av_sample_fmt_is_planar)(enum AVSampleFormat sample_fmt);

			//*****************
			static AVFormatContext* (*avformat_alloc_context)();
			static void(*avformat_close_input)(AVFormatContext **s);
			static void(*avformat_free_context)(AVFormatContext *s);
			static int(*avformat_open_input)(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
			static int(*avformat_find_stream_info)(AVFormatContext* ic, AVDictionary** options);
			static int(*av_find_best_stream)(AVFormatContext *ic, enum AVMediaType type, int wanted_stream_nb, int related_stream, AVCodec **decoder_ret, int flags);
			static int(*avcodec_parameters_to_context)(AVCodecContext *codec, const AVCodecParameters *par);
			static int(*av_opt_set_int)(void *obj, const char *name, int64_t val, int search_flags);
			static int(*av_strerror)(int errnum, char *errbuf, size_t errbuf_size);
			static void(*av_register_all)();

			static std::vector<AVCodecID> decoder_ids;

#define CHECK_API(test) if((test)==nullptr) goto ERR

			static API_PTR avcodec;
			static API_PTR avutil;
			static API_PTR avformat;
			static Result<bool> DynmaicInit()
			{
				Result<bool> result(false);
				avcodec = Microsoft::winapi_LoadLibrary(L"avcodec-u.dll");
				avutil = Microsoft::winapi_LoadLibrary(L"avutil-u.dll");
				avformat = Microsoft::winapi_LoadLibrary(L"avformat-u.dll");
				if (avcodec && avutil && avformat)
				{
					CHECK_API(avcodec_register_all = (void(*)()) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_register_all"));
					CHECK_API(av_init_packet = (void(*)(AVPacket*)) Microsoft::winapi_GetProcAddress(avcodec, "av_init_packet"));
					CHECK_API(avcodec_find_decoder = (AVCodec*(*)(enum AVCodecID)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_find_decoder"));

					CHECK_API(avcodec_alloc_context3 = (AVCodecContext* (*)(const AVCodec *codec)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_alloc_context3"));
					CHECK_API(avcodec_open2 = (int(*)(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_open2"));

					CHECK_API(av_frame_alloc = (AVFrame* (*)()) Microsoft::winapi_GetProcAddress(avutil, "av_frame_alloc"));
					CHECK_API(av_frame_unref = (void(*)(AVFrame *frame)) Microsoft::winapi_GetProcAddress(avutil, "av_frame_unref"));
					CHECK_API(avcodec_decode_audio4 = (int(*)(AVCodecContext *avctx, AVFrame *frame, int *got_frame_ptr, const AVPacket *avpkt)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_decode_audio4"));
					CHECK_API(av_samples_get_buffer_size = (int(*)(int *linesize, int nb_channels, int nb_samples, enum AVSampleFormat sample_fmt, int align)) Microsoft::winapi_GetProcAddress(avutil, "av_samples_get_buffer_size"));
					CHECK_API(avcodec_close = (int(*)(AVCodecContext *)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_close"));
					CHECK_API(av_free = (void(*)(void *)) Microsoft::winapi_GetProcAddress(avutil, "av_free"));
					CHECK_API(av_frame_free = (void(*)(AVFrame **)) Microsoft::winapi_GetProcAddress(avutil, "av_frame_free"));

					CHECK_API(av_read_frame = (int(*)(AVFormatContext*,AVPacket*)) Microsoft::winapi_GetProcAddress(avformat, "av_read_frame"));
					CHECK_API(av_packet_unref = (void(*)(AVPacket*)) Microsoft::winapi_GetProcAddress(avcodec, "av_packet_unref"));
					CHECK_API(avcodec_send_packet = (int(*)(AVCodecContext *avctx, const AVPacket *avpkt)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_send_packet"));
					CHECK_API(avcodec_receive_frame = (int(*)(AVCodecContext*, AVFrame*)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_receive_frame"));

					CHECK_API(av_get_alt_sample_fmt = (enum AVSampleFormat(*)(enum AVSampleFormat, int)) Microsoft::winapi_GetProcAddress(avutil, "av_get_alt_sample_fmt"));
					CHECK_API(av_sample_fmt_is_planar = (int(*)(enum AVSampleFormat)) Microsoft::winapi_GetProcAddress(avutil, "av_sample_fmt_is_planar"));

					CHECK_API(avformat_alloc_context = (AVFormatContext*(*)()) Microsoft::winapi_GetProcAddress(avformat, "avformat_alloc_context"));
					CHECK_API(avformat_close_input = (void(*)(AVFormatContext**)) Microsoft::winapi_GetProcAddress(avformat, "avformat_close_input"));
					CHECK_API(avformat_free_context = (void(*)(AVFormatContext*)) Microsoft::winapi_GetProcAddress(avformat, "avformat_free_context"));
					CHECK_API(avformat_open_input = (int(*)(AVFormatContext**, const char*, AVInputFormat*, AVDictionary**)) Microsoft::winapi_GetProcAddress(avformat, "avformat_open_input"));
					CHECK_API(avformat_find_stream_info = (int(*)(AVFormatContext*, AVDictionary**)) Microsoft::winapi_GetProcAddress(avformat, "avformat_find_stream_info"));
					
					CHECK_API(av_find_best_stream = (int(*)(AVFormatContext*,AVMediaType,int,int,AVCodec**,int)) Microsoft::winapi_GetProcAddress(avformat, "av_find_best_stream"));

					CHECK_API(avcodec_parameters_to_context = (int(*)(AVCodecContext*, const AVCodecParameters*)) Microsoft::winapi_GetProcAddress(avcodec, "avcodec_parameters_to_context"));
					CHECK_API(av_opt_set_int = (int(*)(void*, const char*, int64_t, int)) Microsoft::winapi_GetProcAddress(avutil, "av_opt_set_int"));
					CHECK_API(av_strerror = (int(*)(int, char*, size_t)) Microsoft::winapi_GetProcAddress(avutil, "av_strerror"));
					CHECK_API(av_register_all = (void(*)()) Microsoft::winapi_GetProcAddress(avformat, "av_register_all"));
					
					avcodec_register_all();
					av_register_all();

					result.Value = true;
					return result;
				ERR:
					if (Microsoft::winapi_FreeLibrary(avcodec))
						avcodec = NULL;
					else
						result.WriteMessage(L"can not free avcodec-u.dll");

					if (Microsoft::winapi_FreeLibrary(avutil))
						avutil = NULL;
					else
						result.WriteMessage(L"can not free avutil-u.dll");

					if (Microsoft::winapi_FreeLibrary(avformat))
						avformat = NULL;
					else
						result.WriteMessage(L"can not free avformat-u.dll");
					

					return result;
				}
				else
				{
					result.WriteMessage(L"can not find or load avcodec-u.dll and avutil-u.dll");
				}
				return result;
			}

			static Result<bool> DynamicExit()
			{
				Result<bool> result(false);
				if (avcodec)
				{
					if (Microsoft::winapi_FreeLibrary(avcodec))
					{
						avcodec = NULL;
						result.Value = true;
					}
					else
						result.WriteMessage(L"can not free avcodec-u.dll");
				}
				else
				{
					result.Value = true;
					result.WriteMessage(L"already free avcodec-u.dll");
				}

				if (avutil)
				{
					if (Microsoft::winapi_FreeLibrary(avutil))
					{
						avutil = NULL;
						result.Value = true;
					}
					else
						result.WriteMessage(L"can not free avutil-u.dll");
				}
				else
				{
					result.Value = true;
					result.WriteMessage(L"already free avutil-u.dll");
				}
				if (avformat)
				{
					if (Microsoft::winapi_FreeLibrary(avformat))
					{
						avformat = NULL;
						result.Value = true;
					}
					else
						result.WriteMessage(L"can not free avformat-u.dll");
				}
				else
				{
					result.Value = true;
					result.WriteMessage(L"already free avformat-u.dll");
				}
				
				return result;
			}
		};
	}
}

