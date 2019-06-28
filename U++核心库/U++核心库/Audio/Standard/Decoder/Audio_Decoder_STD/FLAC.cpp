#include "FLAC.h"
#include <limits.h> 
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include "plugin_common\all.h"
#include "share\grabbag.h"
#include "share\replaygain_synthesis.h"
#include "share\replaygain_analysis.h"
}
using namespace System::Audio;


const int INT24_MAX = 8388607;

class FLAC__int24
{
protected:
	unsigned char m_Internal[3];
public:
	FLAC__int24()
	{
	}

	FLAC__int24(const int val)
	{
		*this = val;
	}

	FLAC__int24(const FLAC__int24& val)
	{
		*this = val;
	}

	operator int() const
	{
		if (m_Internal[2] & 0x80) // Is this a negative?  Then we need to siingn extend.
		{
			return (0xff << 24) | (m_Internal[2] << 16) | (m_Internal[1] << 8) | (m_Internal[0] << 0);
		}
		else
		{
			return (m_Internal[2] << 16) | (m_Internal[1] << 8) | (m_Internal[0] << 0);
		}
	}

	operator float() const
	{
		return (float)this->operator int();
	}

	FLAC__int24& operator =(const FLAC__int24& input)
	{
		m_Internal[0] = input.m_Internal[0];
		m_Internal[1] = input.m_Internal[1];
		m_Internal[2] = input.m_Internal[2];

		return *this;
	}

	FLAC__int24& operator =(const int input)
	{
		m_Internal[0] = ((unsigned char*)&input)[0];
		m_Internal[1] = ((unsigned char*)&input)[1];
		m_Internal[2] = ((unsigned char*)&input)[2];

		return *this;
	}

	/***********************************************/

	FLAC__int24 operator +(const FLAC__int24& val) const
	{
		return FLAC__int24((int)*this + (int)val);
	}

	FLAC__int24 operator -(const FLAC__int24& val) const
	{
		return FLAC__int24((int)*this - (int)val);
	}

	FLAC__int24 operator *(const FLAC__int24& val) const
	{
		return FLAC__int24((int)*this * (int)val);
	}

	FLAC__int24 operator /(const FLAC__int24& val) const
	{
		return FLAC__int24((int)*this / (int)val);
	}

	/***********************************************/

	FLAC__int24 operator +(const int val) const
	{
		return FLAC__int24((int)*this + val);
	}

	FLAC__int24 operator -(const int val) const
	{
		return FLAC__int24((int)*this - val);
	}

	FLAC__int24 operator *(const int val) const
	{
		return FLAC__int24((int)*this * val);
	}

	FLAC__int24 operator /(const int val) const
	{
		return FLAC__int24((int)*this / val);
	}

	/***********************************************/
	/***********************************************/


	FLAC__int24& operator +=(const FLAC__int24& val)
	{
		*this = *this + val;
		return *this;
	}

	FLAC__int24& operator -=(const FLAC__int24& val)
	{
		*this = *this - val;
		return *this;
	}

	FLAC__int24& operator *=(const FLAC__int24& val)
	{
		*this = *this * val;
		return *this;
	}

	FLAC__int24& operator /=(const FLAC__int24& val)
	{
		*this = *this / val;
		return *this;
	}

	/***********************************************/

	FLAC__int24& operator +=(const int val)
	{
		*this = *this + val;
		return *this;
	}

	FLAC__int24& operator -=(const int val)
	{
		*this = *this - val;
		return *this;
	}

	FLAC__int24& operator *=(const int val)
	{
		*this = *this * val;
		return *this;
	}

	FLAC__int24& operator /=(const int val)
	{
		*this = *this / val;
		return *this;
	}

	/***********************************************/
	/***********************************************/

	FLAC__int24 operator >>(const int val) const
	{
		return FLAC__int24((int)*this >> val);
	}

	FLAC__int24 operator <<(const int val) const
	{
		return FLAC__int24((int)*this << val);
	}

	/***********************************************/

	FLAC__int24& operator >>=(const int val)
	{
		*this = *this >> val;
		return *this;
	}

	FLAC__int24& operator <<=(const int val)
	{
		*this = *this << val;
		return *this;
	}

	/***********************************************/
	/***********************************************/

	operator bool() const
	{
		return (int)*this != 0;
	}

	bool operator !() const
	{
		return !((int)*this);
	}

	FLAC__int24 operator -()
	{
		return FLAC__int24(-(int)*this);
	}

	/***********************************************/
	/***********************************************/

	bool operator ==(const FLAC__int24& val) const
	{
		return (int)*this == (int)val;
	}

	bool operator !=(const FLAC__int24& val) const
	{
		return (int)*this != (int)val;
	}

	bool operator >=(const FLAC__int24& val) const
	{
		return (int)*this >= (int)val;
	}

	bool operator <=(const FLAC__int24& val) const
	{
		return (int)*this <= (int)val;
	}

	bool operator >(const FLAC__int24& val) const
	{
		return (int)*this > (int)val;
	}

	bool operator <(const FLAC__int24& val) const
	{
		return (int)*this < (int)val;
	}

	/***********************************************/

	bool operator ==(const int val) const
	{
		return (int)*this == val;
	}

	bool operator !=(const int val) const
	{
		return (int)*this != val;
	}

	bool operator >=(const int val) const
	{
		return (int)*this >= val;
	}

	bool operator <=(const int val) const
	{
		return (int)*this <= val;
	}

	bool operator >(const int val) const
	{
		return ((int)*this) > val;
	}

	bool operator <(const int val) const
	{
		return (int)*this < val;
	}

	/***********************************************/
	/***********************************************/
};


#ifdef flacmin
#undef flacmin
#endif
#define flacmin(x,y) ((x)<(y)?(x):(y))

FLAC_Stream::FLAC_Stream()
{

}
FLAC_Stream::~FLAC_Stream()
{
}

FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	stream_data_struct *stream_data = (stream_data_struct*)client_data;
	const unsigned channels = stream_data->channels, wide_samples = frame->header.blocksize;
	(void)decoder;

	if (stream_data->abort_flag)
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;

	

	switch (stream_data->bits_per_sample)
	{
	case 8u:
	{
		for (unsigned ch = 0; ch < channels; ch++)
		{
			unsigned samples = wide_samples;
			FLAC__int8* cbuffer = (FLAC__int8*)&buffer[ch][0];
			FLAC__int8* xbuffer = (FLAC__int8*)&stream_data->buffer[0] + stream_data->buffervalid;
			while (samples--)
			{
				*xbuffer = *cbuffer++;
				xbuffer += channels;
			}
		}
	}
	break;
	case 16u:
	{
		size_t st = stream_data->buffervalid* stream_data->channels;
		FLAC__int16* xbuffer = (FLAC__int16*)&stream_data->buffer;
		for (size_t n = 0; n < wide_samples* channels; n+=channels)
		{
			for (unsigned ch = 0; ch < channels; ch++)
			{
				xbuffer[st + n + ch] = buffer[ch][n/channels];
			}
		}

		/*unsigned chbytes = channels;
		for (unsigned ch = 0; ch < channels; ch++)
		{
			unsigned samples = wide_samples;
			Int32* cbuffer = (Int32*)&buffer[ch][0];
			Int16* xbuffer = ((Int16*)&stream_data->buffer[0])+stream_data->buffervalid* stream_data->channels+ch;

			

			while (samples--)
			{
				*xbuffer =*cbuffer++;
				xbuffer += chbytes;
			}
		}*/
	}
	break;
	case 24u:
	{
		size_t st = stream_data->buffervalid* stream_data->channels;
		FLAC__int24* xbuffer = (FLAC__int24*)&stream_data->buffer;
		for (size_t n = 0; n < wide_samples* channels; n += channels)
		{
			for (unsigned ch = 0; ch < channels; ch++)
			{
				xbuffer[st + n + ch] = buffer[ch][n / channels];
			}
		}
	}
	break;
	case 32u:
	{
		for (unsigned ch = 0; ch < channels; ch++)
		{
			unsigned samples = wide_samples;
			FLAC__int32* cbuffer = (FLAC__int32*)&buffer[ch][0];
			FLAC__int32* xbuffer = (FLAC__int32*)&stream_data->buffer[0] + stream_data->buffervalid;
			while (samples--)
			{
				*xbuffer = *cbuffer++;
				xbuffer += channels;
			}
		}
	}
	break;

	}

	stream_data->buffervalid += wide_samples;

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	stream_data_struct *stream_data = (stream_data_struct*)client_data;
	(void)decoder;

	if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
	{
		stream_data->total_samples = metadata->data.stream_info.total_samples;
		stream_data->bits_per_sample = metadata->data.stream_info.bits_per_sample;
		stream_data->channels = metadata->data.stream_info.channels;
		stream_data->sample_rate = metadata->data.stream_info.sample_rate;

		if (stream_data->bits_per_sample!=8 && stream_data->bits_per_sample!=16 && stream_data->bits_per_sample!=24)
		{
			////FLAC_plugin__show_error("This plugin can only handle 8/16/24-bit samples.");
			stream_data->abort_flag = true;
			return;
		}

		{
			/* with VC++ you have to spoon feed it the casting from UInt64->int64->double */
			FLAC__uint64 l = (FLAC__uint64)((double)(FLAC__int64)stream_data->total_samples / (double)stream_data->sample_rate * 1000.0 + 0.5);
			if (l > INT_MAX)
				l = INT_MAX;
			stream_data->length_in_msec = (int)l;
		}
	}
}
static void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{

}
/*
 *  init/delete
 */

FLAC__bool FLAC_Stream::FLAC_plugin__decoder_init(FLAC__StreamDecoder *decoder, const char *filename, FLAC__int64 filesize, stream_data_struct *stream_data)
{
	FLAC__StreamDecoderInitStatus init_status;

	FLAC__ASSERT(decoder);
	FLAC_plugin__decoder_finish(decoder);
	/* init decoder */
	FLAC__stream_decoder_set_md5_checking(decoder, false);
	FLAC__stream_decoder_set_metadata_ignore_all(decoder);
	FLAC__stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_STREAMINFO);

	if ((init_status = FLAC__stream_decoder_init_file(decoder, filename, write_callback, metadata_callback, error_callback, /*client_data=*/stream_data)) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
	{
		////FLAC_plugin__show_error("Error while initializing decoder (%s [%s]).", FLAC__StreamDecoderInitStatusString[init_status], FLAC__stream_decoder_get_resolved_state_string(decoder));
		return false;
	}
	/* process */
	stream_data->buffervalid = 0;
	stream_data->is_playing = false;
	stream_data->abort_flag = false;

	if (!FLAC__stream_decoder_process_until_end_of_metadata(decoder))
	{
		//FLAC_plugin__show_error("Error while processing metadata (%s).", FLAC__stream_decoder_get_resolved_state_string(decoder));
		return false;
	}
	/* check results */
	if (stream_data->abort_flag) return false;                /* metadata callback already popped up the error dialog */

	/* more inits */
	stream_data->eof = false;
	stream_data->seek_to = -1;
	stream_data->is_playing = true;
	stream_data->average_bps = (unsigned)(filesize / (125.*(double)(FLAC__int64)stream_data->total_samples/(double)stream_data->sample_rate));
	
	stream_data->bh_index_last_w = 0;
	stream_data->bh_index_last_o = BITRATE_HIST_SIZE;
	stream_data->decode_position = 0;
	stream_data->decode_position_last = 0;
	stream_data->written_time_last = 0;

	return true;
}

void FLAC_Stream::FLAC_plugin__decoder_finish(FLAC__StreamDecoder *decoder)
{
	if (decoder && FLAC__stream_decoder_get_state(decoder) != FLAC__STREAM_DECODER_UNINITIALIZED)
		FLAC__stream_decoder_finish(decoder);
}

void FLAC_Stream::FLAC_plugin__decoder_delete(FLAC__StreamDecoder *decoder)
{
	if (decoder)
	{
		FLAC_plugin__decoder_finish(decoder);
		FLAC__stream_decoder_delete(decoder);
	}
}

/*
 *  decode
 */

int FLAC_Stream::FLAC_plugin__seek(FLAC__StreamDecoder *decoder, stream_data_struct *stream_data)
{
	int pos;
	FLAC__uint64 target_sample = stream_data->total_samples * stream_data->seek_to / stream_data->length_in_msec;

	if (stream_data->total_samples > 0 && target_sample >= stream_data->total_samples && target_sample > 0)
		target_sample = stream_data->total_samples - 1;

	/* even if the seek fails we have to reset these so that we don't repeat the seek */
	stream_data->seek_to = -1;
	stream_data->eof = false;
	stream_data->buffervalid = 0;
	pos = (int)(target_sample*1000 / stream_data->sample_rate);

	if (!FLAC__stream_decoder_seek_absolute(decoder, target_sample)) {
		if (FLAC__stream_decoder_get_state(decoder) == FLAC__STREAM_DECODER_SEEK_ERROR)
			FLAC__stream_decoder_flush(decoder);
		pos = -1;
	}

	stream_data->bh_index_last_o = stream_data->bh_index_last_w = (pos/BITRATE_HIST_SEGMENT_MSEC) % BITRATE_HIST_SIZE;
	if (!FLAC__stream_decoder_get_decode_position(decoder, (FLAC__uint64*)&stream_data->decode_position))
		stream_data->decode_position = 0;

	return pos;
}

unsigned FLAC_Stream::FLAC_plugin__decode(FLAC__StreamDecoder *decoder, stream_data_struct *stream_data, const std::function<void(void*, unsigned int)>& xbuffer, size_t numsamples)
{
	/* fill reservoir */
	while (stream_data->buffervalid < SAMPLES_PER_WRITE)
	{
		if (FLAC__stream_decoder_get_state(decoder) == FLAC__STREAM_DECODER_END_OF_STREAM)
		{
			stream_data->eof = true;
			break;
		}
		else if (!FLAC__stream_decoder_process_single(decoder))
		{
			//FLAC_plugin__show_error("Error while processing frame (%s).", FLAC__stream_decoder_get_resolved_state_string(decoder));
			stream_data->eof = true;
			break;
		}
		if (!FLAC__stream_decoder_get_decode_position(decoder, (FLAC__uint64*)&stream_data->decode_position))
			stream_data->decode_position = 0;
	}
	/* output samples */
	if (stream_data->buffervalid> 0)
	{
		unsigned n = stream_data->buffervalid;
		if (n > SAMPLES_PER_WRITE) n = SAMPLES_PER_WRITE;
		const unsigned channels = stream_data->channels;

		xbuffer(&stream_data->buffer[0], n);

		stream_data->buffervalid -= n;

		char* st = (char*)&stream_data->buffer[0];
		char* et = st + ((stream_data->bits_per_sample >> 3) * stream_data->channels*n);
		memmove(st, et , (stream_data->bits_per_sample >> 3) * stream_data->channels* stream_data->buffervalid);

		return n;
	}
	else
	{
		stream_data->eof = true;
		return 0;
	}
	return 0;
}

int FLAC_Stream::FLAC_plugin__get_rate(unsigned written_time, unsigned output_time, stream_data_struct *stream_data)
{
	static int bitrate_history_[BITRATE_HIST_SIZE];
	unsigned bh_index_w = (written_time/BITRATE_HIST_SEGMENT_MSEC) % BITRATE_HIST_SIZE;
	unsigned bh_index_o = (output_time/BITRATE_HIST_SEGMENT_MSEC) % BITRATE_HIST_SIZE;

	/* written bitrate */
	if (bh_index_w != stream_data->bh_index_last_w)
	{
		bitrate_history_[(bh_index_w + BITRATE_HIST_SIZE-1)%BITRATE_HIST_SIZE] =
			stream_data->decode_position>stream_data->decode_position_last && written_time > stream_data->written_time_last ?
			(unsigned)(8000*(stream_data->decode_position - stream_data->decode_position_last)/(written_time - stream_data->written_time_last)) :
			stream_data->average_bps;

		stream_data->bh_index_last_w = bh_index_w;
		stream_data->written_time_last = written_time;
		stream_data->decode_position_last = stream_data->decode_position;
	}

	/* output bitrate */
	if (bh_index_o!=stream_data->bh_index_last_o && bh_index_o!=stream_data->bh_index_last_w)
	{
		stream_data->bh_index_last_o = bh_index_o;
		return bitrate_history_[bh_index_o];
	}

	return 0;
}
