#include "Decoder_MPG123.h"
#include <stdio.h>
#include <string>
#include "VbrTag.h"
#include <filesystem>
using namespace System::Audio;
using namespace System::OS;

struct MPG123_Stream :ZeroStruct<MPG123_Stream>
{
	char* Buffer;
	UInt BufferSize;
	mpg123_handle* MH;
	bool IsStream;
	AudioInfo Info;
	double DecodePosMs;
};

mutex AudioDecoder_MPG123::locker;
AudioDecoder_MPG123* AudioDecoder_MPG123::instance = nullptr;
const wchar_t* AudioDecoder_MPG123::_author = L"xworkteam powered by libmpg123";
const wchar_t* AudioDecoder_MPG123::_description=L"mpg123½âÂë¿â";
const wchar_t* AudioDecoder_MPG123::_supports=L".mp1;.mp2;.mp3";
UInt64 AudioDecoder_MPG123::pipeNum = 0U;

decltype(MPG123API::mpg123_init) MPG123API::mpg123_init = nullptr;
decltype(MPG123API::mpg123_exit) MPG123API::mpg123_exit = nullptr;
decltype(MPG123API::mpg123_new) MPG123API::mpg123_new = nullptr;
decltype(MPG123API::mpg123_open) MPG123API::mpg123_open = nullptr;
decltype(MPG123API::mpg123_close) MPG123API::mpg123_close = nullptr;
decltype(MPG123API::mpg123_format_none) MPG123API::mpg123_format_none = nullptr;
decltype(MPG123API::mpg123_format) MPG123API::mpg123_format = nullptr;
decltype(MPG123API::mpg123_getformat) MPG123API::mpg123_getformat = nullptr;
decltype(MPG123API::mpg123_info) MPG123API::mpg123_info = nullptr;
decltype(MPG123API::mpg123_scan) MPG123API::mpg123_scan = nullptr;
decltype(MPG123API::mpg123_length) MPG123API::mpg123_length = nullptr;
decltype(MPG123API::mpg123_tell) MPG123API::mpg123_tell = nullptr;
decltype(MPG123API::mpg123_delete) MPG123API::mpg123_delete = nullptr;
decltype(MPG123API::mpg123_read) MPG123API::mpg123_read = nullptr;
decltype(MPG123API::mpg123_seek) MPG123API::mpg123_seek = nullptr;

API_PTR MPG123API::module = nullptr;


#define         MPG_MD_STEREO           0
#define         MPG_MD_JOINT_STEREO     1
#define         MPG_MD_DUCHANNEL     2
#define         MPG_MD_MONO             3
const int tabsel_123[2][3][16] = {
	{ { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, },
	{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, },
	{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, } },

	{ { 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, },
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, },
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, } }
};
const long freqs[9] = { 44100, 48000, 32000,
22050, 24000, 16000,
11025, 12000, 8000 };
const int samples_per_frame[4][4] =
{
	{ -1, 384, 1152, 1152 },	/* MPEG 1 */
	{ -1, 384, 1152, 576 },	/* MPEG 2 */
	{ -1, 384, 1152, 576 },	/* MPEG 2.5 */
	{ -1, -1, -1, -1 },		/* Unknown */
};
typedef struct id3tag
{
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	unsigned char genre;
} id3tag;
id3tag current_id3;
VBRTAGDATA current_vbrtag;
typedef struct mp3info
{
	int nbytes;
	int	freq;
	int nch;
	int hpos;
	int hasVbrtag;
	int lsf;
	int bitrate;
	int isStream;
	double length;
} mp3info;

mp3info cur_mp3info;
unsigned int pipeNum = 0;
int head_check(unsigned long head, int check_layer)
{
	/*
	look for a valid header.
	if check_layer > 0, then require that
	nLayer = check_layer.
	*/

	/* bits 13-14 = layer 3 */
	int nLayer = 4 - ((head >> 17) & 3);

	if ((head & 0xffe00000) != 0xffe00000) {
		/* syncword */
		return API_FALSE;
	}
#if 0
	if (!((head >> 17) & 3)) {
		/* bits 13-14 = layer 3 */
		return API_FALSE;
	}
#endif

	if (3 != nLayer)
	{
#if defined (USE_LAYER_1) || defined (USE_LAYER_2)
		if (4 == nLayer)
			return API_FALSE;
#else
		return API_FALSE;
#endif
	}

	if (check_layer>0) {
		if (nLayer != check_layer) return API_FALSE;
	}

	if (((head >> 12) & 0xf) == 0xf) {
		/* bits 16,17,18,19 = 1111  invalid bitrate */
		return API_FALSE;
	}
	if (((head >> 10) & 0x3) == 0x3) {
		/* bits 20,21 = 11  invalid sampling freq */
		return API_FALSE;
	}
	return API_TRUE;
}

int getmp3info(const char* fn, mp3info*	info, id3tag* id3, VBRTAGDATA*	vbr)
{
	unsigned long	head;
	unsigned long	len;
	int				lsf;
	int				srate;
	int				bitrate_index;
	int				mode;
	int				nch;
	int				lay;
	int				extension;
	int				mpeg25;
	int				padding;
	int				hpos;
	int				i;
	API_PTR			hInputFile = INVALID_API_PTR;

	hInputFile = Microsoft::winapi_CreateFile_ANSI(fn,
		API_GENERIC_READ,
		API_FILE_SHARE_READ | API_FILE_SHARE_WRITE,
		NULL,
		API_OPEN_EXISTING,
		API_FILE_ATTRIBUTE_NORMAL,
		NULL);

	// check result operning the file
	if (hInputFile == INVALID_API_PTR)
	{
		return 0;
	}

	info->isStream = 0;
	info->nbytes = Microsoft::winapi_GetFileSize(hInputFile);

	hpos = 0;

	// skip ID3v2 tag
	{
		unsigned char c1, c2, c3, c4;

		Microsoft::winapi_ReadFile(hInputFile, &c1, 1, &len, NULL);
		Microsoft::winapi_ReadFile(hInputFile, &c2, 1, &len, NULL);
		Microsoft::winapi_ReadFile(hInputFile, &c3, 1, &len, NULL);
		Microsoft::winapi_ReadFile(hInputFile, &c4, 1, &len, NULL);

		if (c1 == 'I' && c2 == 'D' && c3 == '3' && c4 == 2)
		{
			Microsoft::winapi_SetFilePointer(hInputFile, 6, API_FILE_BEGIN);
			Microsoft::winapi_ReadFile(hInputFile, &c1, 1, &len, NULL);
			Microsoft::winapi_ReadFile(hInputFile, &c2, 1, &len, NULL);
			Microsoft::winapi_ReadFile(hInputFile, &c3, 1, &len, NULL);
			Microsoft::winapi_ReadFile(hInputFile, &c4, 1, &len, NULL);

			// calculate tag length
			hpos = c1 * 2097152 + c2 * 16384 + c3 * 128 + c4;
		}
	}

	for (i = 0; i < 65536; i++)
	{
		Microsoft::winapi_SetFilePointer(hInputFile, hpos + i, API_FILE_BEGIN);

		Microsoft::winapi_ReadFile(hInputFile, 3 + (char *)&head, 1, &len, NULL);
		Microsoft::winapi_ReadFile(hInputFile, 2 + (char *)&head, 1, &len, NULL);
		Microsoft::winapi_ReadFile(hInputFile, 1 + (char *)&head, 1, &len, NULL);
		Microsoft::winapi_ReadFile(hInputFile, 0 + (char *)&head, 1, &len, NULL);

		lay = 4 - ((head >> 17) & 3);

		if (len == 0 ||
			head_check(head, lay))
		{
			break;
		}
	}

	if (len == 0 || i == 65536)
	{
		Microsoft::winapi_CloseHandle(hInputFile);
		hInputFile = INVALID_API_PTR;
		return 0;
	}

	hpos += i;

	info->hpos = hpos;

	{
		info->nbytes -= hpos;

		// read ID3 tag
		Microsoft::winapi_SetFilePointer(hInputFile, -128, API_FILE_END);
		Microsoft::winapi_ReadFile(hInputFile, id3, 128, &len, NULL);
		if (strncmp(id3->tag, "TAG", 3) == 0) info->nbytes -= 128;
	}

	Microsoft::winapi_SetFilePointer(hInputFile, hpos, API_FILE_BEGIN);

	if (head & (1 << 20)) {
		lsf = (head & (1 << 19)) ? 0x0 : 0x1;
		mpeg25 = 0;
	}
	else {
		lsf = 1;
		mpeg25 = 1;
	}

	bitrate_index = ((head >> 12) & 0xf);
	padding = ((head >> 9) & 0x1);
	mode = ((head >> 6) & 0x3);
	nch = (mode == MPG_MD_MONO) ? 1 : 2;
	lay = 4 - ((head >> 17) & 3);
	extension = ((head >> 8) & 0x1);

	if (mpeg25)
	{
		srate = 6 + ((head >> 10) & 0x3);
	}
	else
	{
		srate = ((head >> 10) & 0x3) + (lsf * 3);
	}

	// read VBR tag
	Microsoft::winapi_SetFilePointer(hInputFile, hpos, API_FILE_BEGIN);

	info->lsf = lsf;
	info->freq = freqs[srate];
	info->nch = nch;

	{
		unsigned char buf[sizeof(VBRTAGDATA)+36];

		Microsoft::winapi_ReadFile(hInputFile, &buf, sizeof(buf), &len, NULL);

		if (GetVbrTag(&current_vbrtag, buf))
		{
			int cur_bitrate = (int)(current_vbrtag.bytes * 8 / (current_vbrtag.frames*576.0*(lsf ? 1 : 2) / freqs[srate]));
			info->length = current_vbrtag.frames*576.0*(lsf ? 1 : 2) / freqs[srate];
			info->nbytes = current_vbrtag.bytes;
			info->hasVbrtag = 1;
			info->bitrate = cur_bitrate;
		}
		else {
			int framesize = tabsel_123[lsf][lay - 1][bitrate_index] * 144000 / (freqs[srate] << lsf) + padding;
			info->length = info->nbytes / framesize*576.0*(lsf ? 1 : 2) / freqs[srate];
			info->hasVbrtag = 0;
			info->bitrate = tabsel_123[lsf][lay - 1][bitrate_index] * 1000;
			//info->length = info->nbytes * 8 / info->bitrate;
		}
	}

	Microsoft::winapi_CloseHandle(hInputFile);

	return 1;
}


AudioDecoder_MPG123::AudioDecoder_MPG123() : AudioDecoder_base()
{

}

AudioDecoder_MPG123* AudioDecoder_MPG123::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			if (MPG123API::mpg123_init() != MPG123_OK) return nullptr;
			instance = new AudioDecoder_MPG123();
		}
	}
	return instance;
}

AudioDecoder_MPG123::~AudioDecoder_MPG123()
{
	lock_guard<mutex> lock(locker);
	MPG123API::mpg123_exit();
}

Version AudioDecoder_MPG123::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDecoder_MPG123::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDecoder_MPG123::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDecoder_MPG123::GetSupportFormat() const
{
	return _supports;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_MPG123::OpenLocal(const wchar_t* localPath)
{
	Result<AudioDecoderPipe, AudioDecoderError> result(nullptr, AudioDecoderError::UnSupported);

	int ret = 0;

	MPG123_Stream* Handle = new MPG123_Stream();

	Handle->MH = MPG123API::mpg123_new(nullptr, &ret);
	if (Handle->MH == nullptr || ret != MPG123_OK)
	{
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::Failed;
		return result;
	}

	const char* path = Converter::CPlusNew(localPath, CultureInfo::Current().CPlusLocale);
	if (!path)
	{
		result.State = AudioDecoderError::InvalidPath;
		SAFE_DELETE(path);
		return result;
	}

	if (!experimental::filesystem::exists(experimental::filesystem::path(path)))
	{
		result.State = AudioDecoderError::InvalidPath;
		SAFE_DELETE(path);
		return result;
	}

	if (MPG123API::mpg123_open(Handle->MH, path) != MPG123_OK)
	{
		SAFE_DELETE(path);
		result.State = AudioDecoderError::UnSupported;
		return result;
	}
	//MPG123API::mpg123_format_none(Handle->MH);
	//MPG123API::mpg123_format(Handle->MH, 44100, MPG123_STEREO, MPG123_ENC_FLOAT_32);

	Handle->IsStream = false;
	long a;
	int b, c;
	MPG123API::mpg123_getformat(Handle->MH, &a, &b, &c);
	if ((c & MPG123_ENC_SIGNED_16) == MPG123_ENC_SIGNED_16)
#ifdef LITTLE_ENDIAN 
		Handle->Info.BitsPerSample = BitsPerSample::SInt16;
#else
		Handle->Info.BitsPerSample = BitsPerSample::SInt16BE;
#endif
	else if ((c & MPG123_ENC_UNSIGNED_16) == MPG123_ENC_UNSIGNED_16)
#ifdef LITTLE_ENDIAN 
		Handle->Info.BitsPerSample = BitsPerSample::UInt16;
#else
		Handle->Info.BitsPerSample = BitsPerSample::UInt16BE;
#endif
	else if ((c & MPG123_ENC_UNSIGNED_32) == MPG123_ENC_UNSIGNED_32)
#ifdef LITTLE_ENDIAN 
		Handle->Info.BitsPerSample = BitsPerSample::UInt32;
#else
		Handle->Info.BitsPerSample = BitsPerSample::UInt32BE;
#endif
	else if ((c & MPG123_ENC_SIGNED_32) == MPG123_ENC_SIGNED_32)
#ifdef LITTLE_ENDIAN 
		Handle->Info.BitsPerSample = BitsPerSample::SInt32;
#else
		Handle->Info.BitsPerSample = BitsPerSample::SInt32BE;
#endif	
	else if ((c & MPG123_ENC_SIGNED_8) == MPG123_ENC_SIGNED_8)
		Handle->Info.BitsPerSample = BitsPerSample::SInt8;
	else if ((c & MPG123_ENC_UNSIGNED_8) == MPG123_ENC_UNSIGNED_8)
		Handle->Info.BitsPerSample = BitsPerSample::UInt8;
	else if ((c & MPG123_ENC_FLOAT_32) == MPG123_ENC_FLOAT_32)
#ifdef LITTLE_ENDIAN 
		Handle->Info.BitsPerSample = BitsPerSample::Float32;
#else
		Handle->Info.BitsPerSample = BitsPerSample::Float32BE;
#endif
	else if ((c & MPG123_ENC_FLOAT_64) == MPG123_ENC_FLOAT_64)
#ifdef LITTLE_ENDIAN 
		Handle->Info.BitsPerSample = BitsPerSample::Float64;
#else
		Handle->Info.BitsPerSample = BitsPerSample::Float64BE;
#endif	
	mpg123_frameinfo aa;
	MPG123API::mpg123_info(Handle->MH, &aa);
	Handle->Info.SampleRate = a;
	if (b == 1)
		Handle->Info.ChannelLayout = ChannelLayout::Mono;
	else if(b==2)
		Handle->Info.ChannelLayout = ChannelLayout::Stereo;
	else
	{
		MPG123API::mpg123_close(Handle->MH);
		MPG123API::mpg123_delete(Handle->MH);
		SAFE_DELETE(path);
		result.State = AudioDecoderError::UnSupported;
		return result;
	}
	if (aa.layer == 1)Handle->Info.Codec = Codec::MP1;
	else if (aa.layer == 2)Handle->Info.Codec = Codec::MP2;
	else if (aa.layer == 3)Handle->Info.Codec = Codec::MP3;
	else Handle->Info.Codec = Codec::Unknown;
	Handle->Info.BitRate = 0;
	if (aa.vbr == mpg123_vbr::MPG123_CBR)
	{
		if (aa.bitrate != 0)
			Handle->Info.BitRate = aa.bitrate;
		else
			Handle->Info.BitRate = (ULong)((double)(aa.framesize + 4) * 8 * aa.rate*0.001 / samples_per_frame[aa.version][aa.layer] + 0.5);
	}
	if (aa.vbr == mpg123_vbr::MPG123_ABR) Handle->Info.BitRate = aa.abr_rate;
	//MPG123API::mpg123_scan(Handle->MH);
	Handle->Info.SampleLength = MPG123API::mpg123_length(Handle->MH);
	Handle->Info.TimeSec = (double)Handle->Info.SampleLength / Handle->Info.SampleRate;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	if (aa.vbr == mpg123_vbr::MPG123_VBR || Handle->Info.BitRate == 0)
	{
		getmp3info(path, &cur_mp3info, &current_id3, &current_vbrtag);
		Handle->Info.BitRate = cur_mp3info.bitrate / 1000;
	}
	Handle->DecodePosMs = 0.0;

	SAFE_DELETE(path);

	result.Value = Handle;
	result.State = AudioDecoderError::OK;
	pipeNum++;

	return result;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_MPG123::CreateStream()
{
	return Result<AudioDecoderPipe, AudioDecoderError>(nullptr, AudioDecoderError::UnSupported);
}

bool AudioDecoder_MPG123::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPG123_Stream));
		if (pipePtr)
		{
			MPG123_Stream* m_MPG = static_cast<MPG123_Stream*>(pipePtr);
			if (m_MPG) return m_MPG->IsStream;
		}
	}
	catch (...){}
	return false;
}

UInt64 AudioDecoder_MPG123::GetNumberOfPipes()
{
	return pipeNum;
}

bool AudioDecoder_MPG123::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPG123_Stream));
		if (pipePtr)
		{
			MPG123_Stream* m_MPG = static_cast<MPG123_Stream*>(pipePtr);
			if (m_MPG)
			{
				info = m_MPG->Info;
				return true;
			}
		}
	}
	catch (...){}
	return false;

}

AudioDecoderError AudioDecoder_MPG123::TryGetLocalAudioInfo(const wchar_t * localPath, AudioInfo& info)
{
	int ret = 0;
	mpg123_handle* MH = MPG123API::mpg123_new(nullptr, &ret);
	if (MH == nullptr || ret != MPG123_OK) return false;
	MPG123API::mpg123_format_none(MH);
	MPG123API::mpg123_format(MH, 44100, MPG123_STEREO, MPG123_ENC_FLOAT_32);

	char* path = Converter::CPlusNew(localPath, CultureInfo::Current().CPlusLocale);
	if (!path)
	{
		SAFE_DELETE(path);
		return AudioDecoderError::InvalidPath;
	}

	if (!experimental::filesystem::exists(experimental::filesystem::path(path)))
	{
		SAFE_DELETE(path);
		return AudioDecoderError::InvalidPath;
	}

	if (MPG123API::mpg123_open(MH, path) != MPG123_OK)
	{
		SAFE_DELETE(path);
		return AudioDecoderError::UnSupported;
	}

	long a;
	int b, c;
	MPG123API::mpg123_getformat(MH, &a, &b, &c);
	if ((c & MPG123_ENC_16) == MPG123_ENC_16)
		c = 16;
	else if ((c & MPG123_ENC_32) == MPG123_ENC_32)
		c = 32;
	else
		c = 8;
	mpg123_frameinfo aa;
	MPG123API::mpg123_info(MH, &aa);
	info.SampleRate = a;
	if (b == 1)
		info.ChannelLayout = ChannelLayout::Mono;
	else if (b == 2)
		info.ChannelLayout = ChannelLayout::Stereo;
	else
	{
		MPG123API::mpg123_close(MH);
		MPG123API::mpg123_delete(MH);
		SAFE_DELETE(path);
		return AudioDecoderError::UnSupported;
	}
#ifdef LITTLE_ENDIAN 
	info.BitsPerSample = BitsPerSample::Float32;
#else
	info.BitsPerSample = BitsPerSample::Float32BE;
#endif
	if (aa.layer == 1)info.Codec = Codec::MP1;
	else if (aa.layer == 2)info.Codec = Codec::MP2;
	else if (aa.layer == 3)info.Codec = Codec::MP3;
	else info.Codec = Codec::Unknown;
	info.BitRate = 0;
	if (aa.vbr == mpg123_vbr::MPG123_CBR)
	{
		if (aa.bitrate != 0)
			info.BitRate = aa.bitrate;
		else
			info.BitRate = (ULong)((double)(aa.framesize + 4) * 8 * aa.rate*0.001 / samples_per_frame[aa.version][aa.layer] + 0.5);
	}
	if (aa.vbr == mpg123_vbr::MPG123_ABR) info.BitRate = aa.abr_rate;
	//MPG123API::mpg123_scan(MH);
	info.SampleLength = MPG123API::mpg123_length(MH);
	info.TimeSec = (double)info.SampleLength / info.SampleRate;
	info.TimeMs = info.TimeSec * 1000;
	if (aa.vbr == mpg123_vbr::MPG123_VBR || info.BitRate == 0)
	{
		getmp3info(path, &cur_mp3info, &current_id3, &current_vbrtag);
		info.BitRate = cur_mp3info.bitrate / 1000;
	}		
	MPG123API::mpg123_close(MH);
	MPG123API::mpg123_delete(MH);
	SAFE_DELETE(path);
	return AudioDecoderError::OK;
}

Result<UInt64, AudioDecoderError> AudioDecoder_MPG123::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples)
{
	Result<UInt64, AudioDecoderError> result(0ull, AudioDecoderError::End);
	try
	{
		void* pipePtr = pipe.get(sizeof(MPG123_Stream));
		if (pipePtr)
		{
			MPG123_Stream* m_MPG = static_cast<MPG123_Stream*>(pipePtr);
			if (m_MPG)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done<desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples>512) NSamples = 512;
					UInt64 need = bytes + NSamples*m_MPG->Info.BlockAlign;
					if (m_MPG->Buffer)
					{
						if (m_MPG->BufferSize<need)
						{
							char* oldmem = m_MPG->Buffer;
							if ((m_MPG->Buffer = (char*)realloc(m_MPG->Buffer, need)) == NULL)
							{
								m_MPG->Buffer = (char*)malloc(need);
								if (!m_MPG->Buffer)
								{
									m_MPG->BufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return result;
								}
								memcpy(m_MPG->Buffer, oldmem, m_MPG->BufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_MPG->Buffer = (char*)malloc(need);
						m_MPG->BufferSize = need;
					}
					if (!m_MPG->Buffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return result;
					}
					size_t tbytes = 0;
					int error = MPG123API::mpg123_read(m_MPG->MH, ((UChar*)m_MPG->Buffer) + Done*m_MPG->Info.BlockAlign, NSamples*m_MPG->Info.BlockAlign, &tbytes);
					bytes += tbytes;
					Done += (tbytes / m_MPG->Info.BlockAlign);
					if (error == MPG123_DONE)
					{
						end = true;
						break;
					}
				}				
				buffer.SetData(m_MPG->Buffer, m_MPG->Info.SampleRate, m_MPG->Info.BitsPerSample, Done, m_MPG->Info.ChannelLayout, m_MPG->DecodePosMs);
				m_MPG->DecodePosMs = ((double)MPG123API::mpg123_tell(m_MPG->MH) * 1000.0 / (double)m_MPG->Info.SampleRate);
				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;

			}
		}
	}
	catch (...){}
	return result;

}

bool AudioDecoder_MPG123::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPG123_Stream));
		if (pipePtr)
		{
			MPG123_Stream* m_MPG = static_cast<MPG123_Stream*>(pipePtr);
			if (m_MPG)
			{
				if (MPG123API::mpg123_seek(m_MPG->MH, (long)(m_MPG->Info.SampleRate / 1000.0 * posMS), 0) >= 0)
				{
					m_MPG->DecodePosMs = ((double)MPG123API::mpg123_tell(m_MPG->MH) * 1000.0 / (double)m_MPG->Info.SampleRate);
					return true;
				}
			}
		}
	}
	catch (...){}
	return false;
}

bool AudioDecoder_MPG123::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPG123_Stream));
		if (pipePtr)
		{
			MPG123_Stream* m_MPG = (MPG123_Stream*)(pipePtr);
			if (m_MPG)
			{
				if (m_MPG->MH)
				{
					int res=MPG123API::mpg123_close(m_MPG->MH);
					MPG123API::mpg123_delete(m_MPG->MH);
					m_MPG->MH = NULL;
				}
				if (m_MPG->Buffer){ free(m_MPG->Buffer); m_MPG->Buffer = NULL; }
				SAFE_DELETE(m_MPG);
				pipeNum--;
			}
		}
	}
	catch (...){}
	return true;
}

double AudioDecoder_MPG123::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MPG123_Stream));
		if (pipePtr)
		{
			MPG123_Stream* m_MPG = static_cast<MPG123_Stream*>(pipePtr);
			if (m_MPG) return m_MPG->DecodePosMs;
		}
	}
	catch (...){}
	return 0.0;
}
