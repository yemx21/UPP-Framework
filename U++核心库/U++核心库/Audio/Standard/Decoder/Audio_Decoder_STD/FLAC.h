#pragma once

extern "C"
{
#include "FLAC\all.h"
//#include "plugin_common\all.h"
}

#include <functional>

#define SAMPLES_PER_WRITE           512

#define BITRATE_HIST_SEGMENT_MSEC   500
#define BITRATE_HIST_SIZE           64
namespace System
{

	namespace Audio
	{
		typedef struct
		{
			FLAC__int32  buffer[FLAC__MAX_BLOCK_SIZE * 2 * 8]; 
			unsigned buffervalid;
			unsigned bh_index_last_w, bh_index_last_o, written_time_last;
			FLAC__int64 decode_position, decode_position_last;

			volatile FLAC__bool is_playing;
			volatile FLAC__bool abort_flag;
			volatile FLAC__bool eof;
			volatile int seek_to;
			FLAC__uint64 total_samples;
			unsigned bits_per_sample;
			unsigned channels;
			unsigned sample_rate;
			int length_in_msec;
			unsigned average_bps;
		} stream_data_struct;

		class FLAC_Stream
		{
		public:
			FLAC_Stream();
			~FLAC_Stream();

			stream_data_struct StreamData;
			FLAC__StreamDecoder *Decoder;

			FLAC__bool FLAC_plugin__decoder_init(FLAC__StreamDecoder *decoder, const char *filename, FLAC__int64 filesize, stream_data_struct *stream_data);
			void FLAC_plugin__decoder_finish(FLAC__StreamDecoder *decoder);
			void FLAC_plugin__decoder_delete(FLAC__StreamDecoder *decoder);

			int FLAC_plugin__seek(FLAC__StreamDecoder *decoder, stream_data_struct *stream_data);
			unsigned FLAC_plugin__decode(FLAC__StreamDecoder *decoder, stream_data_struct *stream_data, const std::function<void(void*,unsigned int)>& xbuffer, size_t numsamples);
			int FLAC_plugin__get_rate(unsigned written_time, unsigned output_time, stream_data_struct *stream_data);
		};
	}
}
