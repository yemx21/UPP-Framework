#include "FFMPEGAPI.h"

using namespace System::Audio;

decltype(FFMPEGAPI::avcodec_register_all) FFMPEGAPI::avcodec_register_all = nullptr;
decltype(FFMPEGAPI::av_init_packet) FFMPEGAPI::av_init_packet = nullptr;
decltype(FFMPEGAPI::avcodec_find_decoder) FFMPEGAPI::avcodec_find_decoder = nullptr;
decltype(FFMPEGAPI::avcodec_alloc_context3) FFMPEGAPI::avcodec_alloc_context3 = nullptr;
decltype(FFMPEGAPI::avcodec_open2) FFMPEGAPI::avcodec_open2 = nullptr;
decltype(FFMPEGAPI::av_frame_alloc) FFMPEGAPI::av_frame_alloc = nullptr;
decltype(FFMPEGAPI::av_frame_unref) FFMPEGAPI::av_frame_unref = nullptr;
decltype(FFMPEGAPI::avcodec_decode_audio4) FFMPEGAPI::avcodec_decode_audio4 = nullptr;
decltype(FFMPEGAPI::av_samples_get_buffer_size) FFMPEGAPI::av_samples_get_buffer_size = nullptr;
decltype(FFMPEGAPI::avcodec_close) FFMPEGAPI::avcodec_close = nullptr;
decltype(FFMPEGAPI::av_free) FFMPEGAPI::av_free = nullptr;
decltype(FFMPEGAPI::av_frame_free) FFMPEGAPI::av_frame_free = nullptr;
decltype(FFMPEGAPI::av_read_frame) FFMPEGAPI::av_read_frame = nullptr;
decltype(FFMPEGAPI::av_packet_unref) FFMPEGAPI::av_packet_unref = nullptr;
decltype(FFMPEGAPI::avcodec_send_packet) FFMPEGAPI::avcodec_send_packet = nullptr;
decltype(FFMPEGAPI::avcodec_receive_frame) FFMPEGAPI::avcodec_receive_frame = nullptr;
decltype(FFMPEGAPI::av_sample_fmt_is_planar) FFMPEGAPI::av_sample_fmt_is_planar = nullptr;
decltype(FFMPEGAPI::av_get_alt_sample_fmt) FFMPEGAPI::av_get_alt_sample_fmt = nullptr;

decltype(FFMPEGAPI::avformat_alloc_context) FFMPEGAPI::avformat_alloc_context = nullptr;
decltype(FFMPEGAPI::avformat_close_input) FFMPEGAPI::avformat_close_input = nullptr;
decltype(FFMPEGAPI::avformat_free_context) FFMPEGAPI::avformat_free_context = nullptr;
decltype(FFMPEGAPI::avformat_open_input) FFMPEGAPI::avformat_open_input = nullptr;
decltype(FFMPEGAPI::avformat_find_stream_info) FFMPEGAPI::avformat_find_stream_info = nullptr;
decltype(FFMPEGAPI::av_find_best_stream) FFMPEGAPI::av_find_best_stream = nullptr;
decltype(FFMPEGAPI::avcodec_parameters_to_context) FFMPEGAPI::avcodec_parameters_to_context = nullptr;
decltype(FFMPEGAPI::av_opt_set_int) FFMPEGAPI::av_opt_set_int = nullptr;
decltype(FFMPEGAPI::av_strerror) FFMPEGAPI::av_strerror = nullptr;
decltype(FFMPEGAPI::av_register_all) FFMPEGAPI::av_register_all = nullptr;

decltype(FFMPEGAPI::decoder_ids) FFMPEGAPI::decoder_ids;

API_PTR FFMPEGAPI::avcodec = nullptr;
API_PTR FFMPEGAPI::avutil = nullptr;
API_PTR FFMPEGAPI::avformat = nullptr;