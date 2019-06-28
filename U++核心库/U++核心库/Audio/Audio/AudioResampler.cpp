#include "AudioResampler.h"
#include "Dither.h"
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;


namespace System
{
	namespace Audio
	{
		class LibSoxrAPI
		{
		public:

			static soxr_io_spec_t(*soxr_io_spec)(soxr_datatype_t itype, soxr_datatype_t otype);
			static double(*soxr_delay)(soxr_t);
			static soxr_error_t(*soxr_clear)(soxr_t);
			static void(*soxr_delete)(soxr_t);
			static soxr_error_t(*soxr_process)(soxr_t resampler, soxr_in_t   in, size_t ilen, size_t* idone, soxr_out_t out, size_t olen, size_t* odone);
			static soxr_t (*soxr_create)(double input_rate, double output_rate, unsigned num_channels, soxr_error_t *, soxr_io_spec_t const *, soxr_quality_spec_t const *, soxr_runtime_spec_t const *);
#define CHECK_API(test) if((test)==nullptr) goto ERR

			static API_PTR module;
			static bool DynmaicInit()
			{
				module = Microsoft::winapi_LoadLibrary(L"libsoxr.dll");
				if (module)
				{
					CHECK_API(soxr_io_spec = (soxr_io_spec_t (*)(soxr_datatype_t, soxr_datatype_t)) Microsoft::winapi_GetProcAddress(module, "soxr_io_spec"));
					CHECK_API(soxr_delay = (double(*)(soxr_t)) Microsoft::winapi_GetProcAddress(module, "soxr_delay"));
					CHECK_API(soxr_clear = (soxr_error_t(*)(soxr_t)) Microsoft::winapi_GetProcAddress(module, "soxr_clear"));
					CHECK_API(soxr_delete = (void(*)(soxr_t)) Microsoft::winapi_GetProcAddress(module, "soxr_delete"));
					CHECK_API(soxr_process = (soxr_error_t(*)(soxr_t , soxr_in_t , size_t , size_t* , soxr_out_t, size_t, size_t*)) Microsoft::winapi_GetProcAddress(module, "soxr_process"));
					CHECK_API(soxr_create = (soxr_t(*)(double, double, unsigned , soxr_error_t *, soxr_io_spec_t const *, soxr_quality_spec_t const *, soxr_runtime_spec_t const *)) Microsoft::winapi_GetProcAddress(module, "soxr_create"));
					return true;
				ERR:
					Microsoft::winapi_FreeLibrary(module);
					module = NULL;
					return false;
				}
				return false;
			}

			static void DynamicExit()
			{
				if (module) Microsoft::winapi_FreeLibrary(module);
			}

		};

		API_PTR LibSoxrAPI::module = nullptr;
		decltype(LibSoxrAPI::soxr_io_spec) LibSoxrAPI::soxr_io_spec = nullptr;
		decltype(LibSoxrAPI::soxr_delay) LibSoxrAPI::soxr_delay = nullptr;
		decltype(LibSoxrAPI::soxr_clear) LibSoxrAPI::soxr_clear = nullptr;
		decltype(LibSoxrAPI::soxr_delete) LibSoxrAPI::soxr_delete = nullptr;
		decltype(LibSoxrAPI::soxr_process) LibSoxrAPI::soxr_process = nullptr;
		decltype(LibSoxrAPI::soxr_create) LibSoxrAPI::soxr_create = nullptr;
	}
}

bool AudioResampler::APILoaded = LibSoxrAPI::DynmaicInit();

AudioResampler::AudioResampler() :outpool(nullptr), floatpool(nullptr), outpoolsize(0), outpoolvalid(0), floatpoolsize(0), floatpoolvalid(0), outfloat(nullptr), outfloatsize(0), outfloatvalid(0), src(nullptr), srcsize(0), ratio(1.0)
{
	Dither = new DitherGenerator();
	InitializeDitherState(Dither);
}

AudioResampler::~AudioResampler()
{
	if (APILoaded)
	{
		if (src)
		{
			LibSoxrAPI::soxr_delete(src);
			src = nullptr;
		}
	}
	SAFE_DELETE(Dither);
	if (outpool){ free(outpool); outpoolsize = 0u; outpoolvalid = 0u; }
	if (floatpool){ free(floatpool); floatpoolsize = 0u; floatpoolvalid = 0u; }
}

void AudioResampler::unsafe_SetInputInfo(const SampleInfo& info)
{
	srcinfo = info;
	finalinfo = info;
}

bool AudioResampler::Config(const ResampleConfig& cfg)
{
	if (!APILoaded) return false;
	UInt re_sr = srcinfo.SampleRate;
	bool needreset = cfg.EnableSampleRate;
	if (needreset)
	{
		if (cfg.SampleRate < SAMPLERATE_MIN || cfg.SampleRate > SAMPLERATE_MAX) return false;
		re_sr = cfg.SampleRate;
		needreset = cfg.SampleRate != srcinfo.SampleRate;
	}
	bool needsrc = needreset;

	if (!needreset)
	{
		if (cfg.EnableBitsPerSample)
		{
			if (!cfg.BitsPerSample.IsValid) return false;
			needreset = cfg.BitsPerSample != srcinfo.BitsPerSample;
		}
	}
	UShort re_ch = srcinfo.Channels;
	if (cfg.EnableChannels)
	{
		if (!cfg.ChannelLayout.IsValid) return false;
		re_ch = cfg.Channels;
		needreset = re_ch != srcinfo.Channels;
	}
	if (needreset)
	{
		if (needsrc)
		{
			soxr_error_t err;
			ratio = (double)re_sr / srcinfo.SampleRate;
			auto iospec = LibSoxrAPI::soxr_io_spec(SOXR_FLOAT32, SOXR_FLOAT32);
			if (!src)
			{
				src = LibSoxrAPI::soxr_create(srcinfo.SampleRate, re_sr, srcinfo.Channels, &err, &iospec, 0, 0);
			}
			else
			{
				LibSoxrAPI::soxr_delete(src);
				src = nullptr;
				src = LibSoxrAPI::soxr_create(srcinfo.SampleRate, re_sr, srcinfo.Channels, &err, &iospec, 0, 0);
			}
		}
		else
		{
			if (src)
			{
				LibSoxrAPI::soxr_delete(src);
				src = nullptr;
			}
		}

	}
	dstcfg = cfg;
	
	finalinfo.SampleRate = re_sr;
	finalinfo.BitsPerSample = cfg.EnableBitsPerSample ? cfg.BitsPerSample : srcinfo.BitsPerSample;
	finalinfo.ChannelLayout = cfg.EnableChannels? cfg.ChannelLayout : srcinfo.ChannelLayout;

	/*FIXME 移除下一行并增加多通道转换*/
	if (finalinfo.ChannelLayout != srcinfo.ChannelLayout) return false;

	return true;
}

float* AudioResampler::EnsureFloatPool(const void* buf, UShort ch, size_t samplecount, BitsPerSample bps, size_t& rbytes)
{	
	if (!buf || !AudioBuffer::IsValid_Channels(ch) || samplecount == 0u || !bps.IsValid)
	{
		rbytes = 0u;
		return false;
	}
	try
	{
		UInt count = samplecount*ch;
		UInt bytes = sizeof(float)*count;
		UInt n = 0;
		if (!floatpool)
		{
			floatpool = (float*)malloc(bytes);
			if (!floatpool) throw - 32;
			floatpoolsize = bytes;
			floatpoolvalid = 0;
		}
		else
		{
			if (floatpoolsize<bytes)
			{
				float* oldpool = floatpool;
				if ((floatpool = (float*)realloc(floatpool, bytes)) == nullptr)
				{
					free(oldpool);
					floatpool = (float*)malloc(bytes);
					if (!floatpool) throw - 32;
					floatpoolsize = bytes;
					floatpoolvalid = 0;
				}
			}
			else
				floatpoolvalid = 0;
		}
		switch (bps.BPS)
		{
		case 8:
		{
			Int8_To_Float32(floatpool, 1, buf, 1, count);
			floatpoolvalid = bytes;
		}
		break;
		case 16:
		{
			Int16_To_Float32(floatpool, 1, buf, 1, count);
			floatpoolvalid = bytes;
		}
		break;
		case 24:
		{
			Int24_To_Float32(floatpool, 1, buf, 1, count);
			floatpoolvalid = bytes;
		}
		break;
		case 32:
		{
			if (!bps.IsFloating)
			{
				Int32_To_Float32(floatpool, 1, buf, 1, count);
				floatpoolvalid = bytes;
			}
			else
			{
				rbytes = bytes;
				return (float*)buf;
			}
		}
		break;
		case 64:
		{
			Float64_To_Float32(floatpool, 1, buf, 1, count);
			floatpoolvalid = bytes;
		}
		break;
		}
		rbytes = floatpoolvalid;
		return floatpool;
	}
	catch (int e)
	{
		if (e == -32)
		{
			floatpoolsize = 0;
			floatpoolvalid = 0;
		}
	}
	rbytes = 0u;
	return nullptr;
}

const void* AudioResampler::Write(const void* buf, size_t bufbytes, const SampleInfo& info, size_t& bytes, bool& direct, Audio::SampleInfo& FinalInfo)
{
	direct = true;
	if (!buf)
	{
		bytes = 0u;
		return nullptr;
	}
	bool needconfig = false;
	if (info.SampleRate != srcinfo.SampleRate)
	{
		srcinfo.SampleRate = info.SampleRate;
		needconfig = true;
	}
	if (info.BitsPerSample != srcinfo.BitsPerSample)
	{
		srcinfo.BitsPerSample = info.BitsPerSample;
		needconfig = true;
	}
	if (info.ChannelLayout != srcinfo.ChannelLayout)
	{
		srcinfo.ChannelLayout = info.ChannelLayout;
		needconfig = true;
	}
	if (needconfig) Config(dstcfg);
	if (src)
	{
		size_t pfs = 0u;
		float* pf = EnsureFloatPool(buf, info.Channels, bufbytes / info.BlockAlign, info.BitsPerSample, pfs);
		if (pf && pfs)
		{
			direct = false;

			size_t precount = pfs / sizeof(float) / srcinfo.Channels;
			size_t ecount =floor((float)precount* ratio+0.5)+256;

			size_t ebytes = ecount* sizeof(float)*srcinfo.Channels;

			if (ebytes > outfloatsize)
			{
				outfloat = (float*)realloc(outfloat, ebytes);
				outfloatsize = ebytes;
			}
			outfloatvalid = 0;
			if (APILoaded)
			{
				LibSoxrAPI::soxr_process(src, pf, precount, 0, outfloat, outfloatsize / sizeof(float) / srcinfo.Channels, &outfloatvalid);
			}
			outfloatvalid *= sizeof(float)*srcinfo.Channels;
		}
	}
	else
	{
		FinalInfo = finalinfo;
		if (srcinfo.ChannelLayout == finalinfo.ChannelLayout && srcinfo.BitsPerSample== finalinfo.BitsPerSample)
		{
			bytes = bufbytes;
			return buf;
		}
		else if (srcinfo.ChannelLayout == finalinfo.ChannelLayout && srcinfo.BitsPerSample != finalinfo.BitsPerSample)
		{
			UInt count = bufbytes / (info.BitsPerSample.BPS >> 3);
			UInt poolneedbytes = count * (finalinfo.BitsPerSample.BPS>>3);
			if (outpool)
			{
				if (outpoolsize<poolneedbytes)
				{
					void* oldpool = outpool;
					if ((outpool = realloc(outpool, poolneedbytes)) == nullptr)
					{
						outpool = malloc(poolneedbytes);
						if (!outpool)
						{
							free(outpool);
							outpoolsize = 0;
							outpoolvalid = 0;
						}
						memcpy(outpool, oldpool, outpoolvalid);
						free(oldpool);
						outpoolsize = poolneedbytes;
						outpoolvalid = 0;
					}
					else
					{
						outpoolsize = poolneedbytes;
						outpoolvalid = 0;
					};
				}
			}
			else
			{
				outpool = malloc(poolneedbytes);
				if (!outpool)
					outpoolsize = 0;
				else
					outpoolsize = poolneedbytes;
				outpoolvalid = 0;
			}

			size_t pfs = 0u;
			float* pf = EnsureFloatPool(buf, info.Channels, bufbytes / info.BlockAlign, info.BitsPerSample, pfs);
			if (pf && pfs)
			{
				count = pfs / sizeof(float);
				switch (finalinfo.BitsPerSample.BPS)
				{
				case 8:
				{
					Int8* finalpool = (Int8*)outpool;
					for (int i = 0; i < count; i++)
					{
						float sum = pf[i];
						float dither = GenerateFloatDither(Dither);
						sum = (sum * (126.0f)) + dither;
						finalpool[i] = (Int8)CLAMP_f8(sum);
					}
					outpoolvalid = count* sizeof(Int8);
				}
				break;
				case 16:
				{
					Int16* finalpool = (Int16*)outpool;
					for (int i = 0; i < count; i++)
					{
						float sum = pf[i];
						float dither = GenerateFloatDither(Dither);
						sum = (sum * (32766.0f)) + dither;
						finalpool[i] = (Int16)CLAMP_f16(sum);
					}
					outpoolvalid = count* sizeof(Int16);
				}
				break;
				case 24:
				{
					Int24* finalpool = (Int24*)outpool;
					for (int i = 0; i < count; i++)
					{
						float sum = pf[i];
						float dither = GenerateFloatDither(Dither);
						sum = (sum * (8388606.0f)) + dither;

						finalpool[i] = (int)CLAMP_f24(sum);
					}
					outpoolvalid = count* sizeof(Int24);
				}
				break;
				case 32:
				{
					if (!finalinfo.BitsPerSample.IsFloating)
					{
						Int32* finalpool = (Int32*)outpool;
						for (int i = 0; i < count; i++)
						{
							float sum = pf[i];
							float dither = GenerateFloatDither(Dither);
							sum = (sum * (2147483646.0f)) + dither;
							finalpool[i] = (Int32)CLAMP_f32(sum);
						}
						outpoolvalid = count* sizeof(float);
					}
					else
					{
						outpoolvalid = count* sizeof(float);
						memcpy(outpool, pf, outpoolvalid);
					}
				}
				break;
				case 64:
				{
					double* finalpool = (double*)outpool;
					for (int i = 0; i < count; i++)
					{
						finalpool[i] = (double)(pf[i]);
					}
					outpoolvalid = count* sizeof(double);
				}
				break;
				}

				bytes = outpoolvalid;
				return outpoolvalid > 0u ? outpool : nullptr;
			}
		}
		/*FIXME  增加多通道转换*/
	}
	bytes = 0u;
	return nullptr;

}

const void* AudioResampler::Write(AudioBuffer* buf, size_t& bytes, bool& direct)
{
	direct = true;
	if (!buf)
	{
		bytes = 0u;
		return nullptr;
	}
	bool needconfig = false;
	if (buf->SampleRate != srcinfo.SampleRate)
	{
		srcinfo.SampleRate = buf->SampleRate;
		needconfig = true;
	}
	if (buf->BitsPerSample != srcinfo.BitsPerSample)
	{
		srcinfo.BitsPerSample = buf->BitsPerSample;
		needconfig = true;
	}
	if (buf->ChannelLayout != srcinfo.ChannelLayout)
	{
		srcinfo.ChannelLayout = buf->ChannelLayout;
		needconfig = true;
	}
	if (needconfig) Config(dstcfg);
	if (src)
	{
		direct = false;
		auto res = buf->EnsureFloatPool();
		if (res.Value)
		{
			if (buf->FloatPoolBytes)
			{
				size_t precount = buf->FloatPoolBytes / sizeof(float) / srcinfo.Channels;
				size_t ecount = precount* ratio;

				size_t ebytes = ecount* sizeof(float)*srcinfo.Channels;

				if (ebytes > outfloatsize)
				{
					outfloat = (float*)realloc(outfloat, ebytes);
					outfloatsize = ebytes;
				}
				outfloatvalid = 0;
				if (APILoaded)
				{
					LibSoxrAPI::soxr_process(src, buf->FloatPool, precount, 0, outfloat, outfloatsize / sizeof(float) / srcinfo.Channels, &outfloatvalid);
				}
				outfloatvalid *= sizeof(float)*srcinfo.Channels;
			}
			else
				outfloatvalid = 0;
		}
		else
			outfloatvalid = 0;
	}
	else
	{
		if (srcinfo.ChannelLayout == finalinfo.ChannelLayout)
		{
			switch (finalinfo.BitsPerSample.BPS)
			{
			case 8u:
			{
				auto res8 = buf->EnsurePool8();
				if (res8.Value)
				{
					bytes = buf->Pool8Bytes;
					return (void*)buf->Pool8;
				}
			}
			break;
			case 16u:
			{
				auto res16 = buf->EnsurePool16();
				if (res16.Value)
				{
					bytes = buf->Pool16Bytes;
					return (void*)buf->Pool16;
				}
			}
			break;
			case 24u:
			{
				auto res24 = buf->EnsurePool24();
				if (res24.Value)
				{
					bytes = buf->Pool24Bytes;
					return (void*)buf->Pool24;
				}
			}
			break;
			case 32u:
			{
				if (!buf->BitsPerSample.IsFloating)
				{
					auto res32 = buf->EnsurePool32();
					if (res32.Value)
					{
						bytes = buf->Pool32Bytes;
						return (void*)buf->Pool8;
					}
				}
				else
				{
					auto resf = buf->EnsureFloatPool();
					if (resf.Value)
					{
						bytes = buf->FloatPoolBytes;
						return (void*)buf->FloatPool;
					}
				}
			}
			break;
			case 64u:
			{
				if (buf->BitsPerSample.IsFloating)
				{
					auto resd = buf->EnsureDoublePool();
					if (resd.Value)
					{
						bytes = buf->DoublePoolBytes;
						return (void*)buf->DoublePool;
					}
				}
			}
			break;
			}
		}
		/*FIXME  增加多通道转换*/
	}
	bytes = 0u;
	return nullptr;
}

const void* AudioResampler::Write(AudioBuffer* buf, size_t& bytes, bool& direct, Audio::SampleInfo& FinalInfo)
{
	direct = true;
	if (!buf)
	{
		bytes = 0u;
		return nullptr;
	}
	bool needconfig = false;
	if (buf->SampleRate != srcinfo.SampleRate) 
	{
		srcinfo.SampleRate = buf->SampleRate; 
		needconfig = true;
	}
	if (buf->BitsPerSample != srcinfo.BitsPerSample)
	{
		srcinfo.BitsPerSample = buf->BitsPerSample;	
		needconfig = true;
	}
	if (buf->ChannelLayout != srcinfo.ChannelLayout)
	{
		srcinfo.ChannelLayout = buf->ChannelLayout;	
		needconfig = true;
	}
	if (needconfig) Config(dstcfg);
	if (src)
	{
		direct = false;
		auto res = buf->EnsurePool32();
		if (res.Value)
		{
			if (buf->FloatPoolBytes)
			{
				size_t precount = buf->FloatPoolBytes / sizeof(float) / srcinfo.Channels;
				size_t ecount = precount* ratio;

				size_t ebytes = ecount* sizeof(float)*srcinfo.Channels;

				if (ebytes > outfloatsize)
				{
					outfloat = (float*)realloc(outfloat, ebytes);
					outfloatsize = ebytes;
				}
				outfloatvalid = 0;
				if (APILoaded)
				{
					LibSoxrAPI::soxr_process(src, buf->FloatPool, precount, 0, outfloat, outfloatsize / sizeof(float) / srcinfo.Channels, &outfloatvalid);
				}
				outfloatvalid *= sizeof(float)*srcinfo.Channels;
			}
			else
				outfloatvalid = 0;
		}
		else
			outfloatvalid = 0;
	}
	else
	{	
		if (srcinfo.ChannelLayout == finalinfo.ChannelLayout)
		{
			FinalInfo = finalinfo;
			switch (finalinfo.BitsPerSample.BPS)
			{
			case 8u:
			{
				auto res8 = buf->EnsurePool8();
				if (res8.Value)
				{
					bytes = buf->Pool8Bytes;
					return (void*)buf->Pool8;
				}
			}
			break;
			case 16u:
			{
				auto res16 = buf->EnsurePool16();
				if (res16.Value)
				{
					bytes = buf->Pool16Bytes;
					return (void*)buf->Pool16;
				}
			}
			break;
			case 24u:
			{
				auto res24 = buf->EnsurePool24();
				if (res24.Value)
				{
					bytes = buf->Pool24Bytes;
					return (void*)buf->Pool24;
				}
			}
			break;
			case 32u:
			{
				if (!buf->BitsPerSample.IsFloating)
				{
					auto res32 = buf->EnsurePool32();
					if (res32.Value)
					{
						bytes = buf->Pool32Bytes;
						return (void*)buf->Pool32;
					}
				}
				else
				{
					auto resf = buf->EnsureFloatPool();
					if (resf.Value)
					{
						bytes = buf->FloatPoolBytes;
						return (void*)buf->FloatPool;
					}
				}
			}
			break;
			case 64u:
			{
				if (buf->BitsPerSample.IsFloating)
				{
					auto resd = buf->EnsureDoublePool();
					if (resd.Value)
					{
						bytes = buf->DoublePoolBytes;
						return (void*)buf->DoublePool;
					}
				}
			}
			break;
			}
		}
		/*FIXME  增加多通道转换*/
	}
	bytes = 0u;
	return nullptr;
}

const void* AudioResampler::Read(size_t& bytes, Audio::SampleInfo& FinalInfo, bool& needmore)
{
	FinalInfo = finalinfo;
	if (!FinalInfo.IsValid)
	{
		bytes = 0u;
		return nullptr;
	}
	if (src)
	{
		UInt srcvalid = outfloatvalid;
		float* srcout = outfloat;
		if (srcvalid && srcout)
		{
			UInt count = srcvalid /sizeof(float);

			UInt poolneedbytes = count * (finalinfo.BitsPerSample.BPS>>3);
			if (outpool)
			{
				if (outpoolsize<poolneedbytes)
				{
					void* oldpool = outpool;
					if ((outpool = realloc(outpool, poolneedbytes)) == nullptr)
					{
						outpool = malloc(poolneedbytes);
						if (!outpool)
						{
							free(outpool);
							outpoolsize = 0;
							outpoolvalid = 0;
						}
						memcpy(outpool, oldpool, outpoolvalid);
						free(oldpool);
						outpoolsize = poolneedbytes;
						outpoolvalid = 0;
					}
					else
					{
						outpoolsize = poolneedbytes;
						outpoolvalid = 0;
					}
				}
			}
			else
			{
				outpool = malloc(poolneedbytes);
				if (!outpool)
					outpoolsize = 0;
				else
					outpoolsize = poolneedbytes;
				outpoolvalid = 0;
			}


			switch (FinalInfo.BitsPerSample.BPS)
			{
			case 8:
			{
				Int8* finalpool = (Int8*)outpool;
				for (size_t i = 0u; i<count; i++)
				{
					float sum = srcout[i];
					float dither = GenerateFloatDither(Dither);
					sum = (sum * (126.0f)) + dither;
					finalpool[i] = (Int8)CLAMP_f8(sum);
				}
				outpoolvalid = count* sizeof(Int8);
			}
			break;
			case 16:
			{
				Int16* finalpool = (Int16*)outpool;
				for (size_t i = 0u; i<count; i++)
				{
					float sum = srcout[i];
					float dither = GenerateFloatDither(Dither);
					sum = (sum * (32766.0f)) + dither;
					finalpool[i] = (Int16)CLAMP_f16(sum);
				}
				outpoolvalid = count* sizeof(Int16);
			}
			break;
			case 24:
			{
				Int24* finalpool = (Int24*)outpool;
				for (size_t i = 0u; i<count; i++)
				{
					float sum = srcout[i];
					float dither = GenerateFloatDither(Dither);
					sum = (sum * (8388606.0f)) + dither;

					finalpool[i] = (int)CLAMP_f24(sum);
				}
				outpoolvalid = count* sizeof(Int24);
			}
			break;
			case 32:
			{
				if (!FinalInfo.BitsPerSample.IsFloating)
				{
					Int32* finalpool = (Int32*)outpool;
					for (size_t i = 0u; i<count; i++)
					{
						float sum = srcout[i];
						float dither = GenerateFloatDither(Dither);
						sum = (sum * (2147483646.0f)) + dither;
						finalpool[i] = (Int32)CLAMP_f32(sum);
					}
					outpoolvalid = count* sizeof(Int32);
				}
				else
				{
					outpoolvalid = count* sizeof(float);
					memcpy(outpool, srcout, outpoolvalid);
				}
			}
			break;
			case 64:
			{
				double* finalpool = (double*)outpool;
				for (size_t i = 0u; i<count; i++)
				{
					finalpool[i] = (double)(srcout[i]);
				}
				outpoolvalid = count* sizeof(double);
			}
			break;
			}
		}
		else
		{
			needmore = APILoaded? !FloatingHelper::IsZero(LibSoxrAPI::soxr_delay(src)) : true;
			bytes = 0u; 
			return nullptr;
		}
		bytes = outpoolvalid;
		outpoolvalid = 0u;
		return bytes > 0 ? outpool : nullptr;
	}
	bytes = 0u;
	return nullptr;
}

void AudioResampler::Flush()
{
	if (APILoaded)
	{
		if (src)
		{
			float tmpv = 0.0f;
			LibSoxrAPI::soxr_process(src, &tmpv, 0, 0, outfloat, outfloatsize / sizeof(float) / srcinfo.Channels, &outfloatvalid);
		}
	}
}
