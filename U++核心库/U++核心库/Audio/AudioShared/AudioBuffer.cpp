#include "AudioBuffer.h"
#include "dither.h"
using namespace System::Audio;
namespace System
{
	namespace Audio
	{
		struct AudioBufferHistory : ZeroStruct<AudioBufferHistory>
		{
			UInt old_sr;
			BitsPerSample old_bps;
			UShort old_bpsnum;
			ChannelLayout old_chcfg;
			UShort old_ch;

			IChunk8 ipool8;
			IChunk16 ipool16;
			IChunk24 ipool24;
			IChunk32 ipool32;
			IChunkF32 ipoolf32;
			IChunkF64 ipoolf64;

			SChunk8 spool8;
			SChunk16 spool16;
			SChunk24 spool24;
			SChunk32 spool32;
			SChunkF32 spoolf32;
			SChunkF64 spoolf64;

			IChunk ipoolout;
			SChunk spoolout;

			UInt64 samplecount;
			double duration;
			double StartMs;

			Optional<bool> interleaved;
			bool chunkrollback;
			AudioChunk* chunk;
		};
	}
}



const wchar_t* SAMPLERATE_TOOLOW = L"SampleRate is too low";
const wchar_t* SAMPLERATE_TOOLARGE = L"SampleRate is too large";
const wchar_t* BITSPERSAMPLE_UNSUPPORTED = L"BitsPerSample is not supported";
const wchar_t* CHANNELCONFIG_INVALID = L"ChannelLayout is invalid";
const wchar_t* SAMPLECOUNT_INVALID = L"no sample";

const wchar_t* MEMORY_ALLOCATE_ERROR = L"memory pool allocate error";

template <typename T, class ALLOC>
__forceinline void CopySplittedChunk(AudioSplitedChunk<T, ALLOC>* dst, T** src, UInt perbytes, UShort ch, UInt lastcur=0)
{
	T** dst_ptr = dst->Ptr();
	for (UShort c = 0; c < ch; c++)
		memcpy(dst_ptr[c]+ lastcur, src[c], perbytes);
}

template<typename _Ty>
struct audio_buffer_unsigned_of
{
	using type =
		conditional_t<std::_Is_any_of_v<_Ty, long, unsigned long>, unsigned long,
		conditional_t<sizeof(_Ty) == 1, unsigned char,
		conditional_t<sizeof(_Ty) == 2, unsigned short,
		conditional_t<sizeof(_Ty) == 3, UInt24,
		conditional_t<sizeof(_Ty) == 4, unsigned int,
		unsigned long long>>>>>;
};

template <typename T, class ALLOC>
__forceinline void CopySplittedChunk2(AudioSplitedChunk<T, ALLOC>* dst, typename audio_buffer_unsigned_of<T>::type** src, UInt count, UShort ch, UInt lastcur = 0)
{
	constexpr int sign_swap = (1 << (8 * sizeof(T) - 1)) - 1;

	T** dst_ptr = dst->Ptr();
	for (UShort c = 0; c < ch; c++)
	{
		T* c_dst_ptr = dst_ptr[c] + lastcur;
		typename audio_buffer_unsigned_of<T>::type* c_src_ptr = src[c];
		for (UInt n = 0; n < count; n++)
			c_dst_ptr[n] = (T)((int)c_src_ptr[n] ^ sign_swap);
	}
}

namespace System
{
	namespace Audio
	{
		class AudioChunk_impl
		{
		public:
			AudioBuffer* host;
			BitsPerSample bps;
			bool interleaved;

			AudioChunk_impl() : host(nullptr), interleaved(true)
			{

			}

			static AudioChunk* Create(AudioBuffer* buf, BitsPerSample bps)
			{
				AudioChunk* ret = new AudioChunk();
				ret->impl = new AudioChunk_impl();
				ret->impl->host = buf;
				ret->impl->bps = bps;
				return ret;
			}

			void* GetInterleaved()
			{
				if (!interleaved) return nullptr;
				if (!bps.IsValid) return nullptr;
				auto bpsnum = bps.BPS;
				auto isfloating = bps.IsFloating;

				switch (bpsnum)
				{
				case 8u:
					if (!host->EnsurePool8().Value) return nullptr;
					return host->ipool8.Ptr();
				case 16u:
					if (!host->EnsurePool16().Value) return nullptr;
					return host->ipool16.Ptr();
				case 24u:
					if (!host->EnsurePool24().Value) return nullptr;
					return host->ipool24.Ptr();
				case 32u:
					if (isfloating)
					{
						if (!host->EnsureFloatPool().Value) return nullptr;
						return host->ipoolf32.Ptr();
					}
					if (!host->EnsurePool32().Value) return nullptr;
					return host->ipool32.Ptr();
				case 64u:
					if (!host->EnsureDoublePool().Value) return nullptr;
					return host->ipoolf64.Ptr();
				}
				return nullptr;
			}

			void** GetSplitted()
			{
				if (interleaved) return nullptr;
				if (!bps.IsValid) return nullptr;
				auto bpsnum = bps.BPS;
				auto isfloating = bps.IsFloating;

				switch (bpsnum)
				{
				case 8u:
					if (!host->EnsurePool8(false).Value) return nullptr;
					return (void**)host->spool8.Ptr();
				case 16u:
					if (!host->EnsurePool16(false).Value) return nullptr;
					return (void**)host->spool16.Ptr();
				case 24u:
					if (!host->EnsurePool24(false).Value) return nullptr;
					return (void**)host->spool24.Ptr();
				case 32u:
					if (isfloating)
					{
						if (!host->EnsureFloatPool(false).Value) return nullptr;
						return (void**)host->spoolf32.Ptr();
					}
					if (!host->EnsurePool32(false).Value) return nullptr;
					return (void**)host->spool32.Ptr();
				case 64u:
					if (!host->EnsureDoublePool(false).Value) return nullptr;
					return (void**)host->spoolf64.Ptr();
				}
				return nullptr;
			}

			bool GetInterleaved() const
			{
				return interleaved;
			}

			void SetInterleaved(bool val)
			{
				interleaved = val;
			}

			UInt GetSampleCount() const
			{
				return host->samplecount;
			}

			bool SetSampleCount(UInt samplecount) const
			{
				if (!bps.IsValid) return false;
				auto bpsnum = bps.BPS;
				auto isfloating = bps.IsFloating;

				UInt capcity = 0u;
				if (interleaved)
				{
					switch (bpsnum)
					{
					case 8u:
						if (!host->EnsurePool8().Value) return nullptr;
						capcity = (UInt)(host->ipool8.Size() / sizeof(Int8) / host->ch);
						break;
					case 16u:
						if (!host->EnsurePool16().Value) return nullptr;
						capcity = (UInt)(host->ipool16.Size() / sizeof(Int16) / host->ch);
						break;
					case 24u:
						if (!host->EnsurePool24().Value) return nullptr;
						capcity = (UInt)(host->ipool24.Size() / sizeof(Int24) / host->ch);
						break;
					case 32u:
						if (isfloating)
						{
							if (!host->EnsureFloatPool().Value) return nullptr;
							capcity = (UInt)(host->ipoolf32.Size() / sizeof(float) / host->ch);
							break;
						}
						if (!host->EnsurePool32().Value) return nullptr;
						capcity = (UInt)(host->ipool32.Size() / sizeof(Int32) / host->ch);
						break;
					case 64u:
						if (!host->EnsureDoublePool().Value) return nullptr;
						capcity = (UInt)(host->ipoolf64.Size() / sizeof(double) / host->ch);
						break;
					}
				}
				else
				{
					switch (bpsnum)
					{
					case 8u:
						if (!host->EnsurePool8(false).Value) return false;
						capcity = (UInt)host->spool8.Size(0);
						break;
					case 16u:
						if (!host->EnsurePool16(false).Value) return false;
						capcity = (UInt)host->spool16.Size(0);
						break;
					case 24u:
						if (!host->EnsurePool24(false).Value) return false;
						capcity = (UInt)host->spool24.Size(0);
						break;
					case 32u:
						if (isfloating)
						{
							if (!host->EnsureFloatPool(false).Value) return false;
							capcity = (UInt)host->spoolf32.Size(0);
							break;
						}
						if (!host->EnsurePool32(false).Value) return false;
						capcity = (UInt)host->spool32.Size(0);
						break;
					case 64u:
						if (!host->EnsureDoublePool(false).Value) return false;
						capcity = (UInt)host->spoolf64.Size(0);
						break;
					}

				}

				if (capcity < samplecount)
				{
					return false;
				}

				if (interleaved)
				{
					switch (bpsnum)
					{
					case 8u:
						if (!host->EnsurePool8().Value) return nullptr;
						host->ipool8.SetValid(samplecount* host->ch * sizeof(Int8));
						break;
					case 16u:
						if (!host->EnsurePool16().Value) return nullptr;
						host->ipool16.SetValid(samplecount* host->ch * sizeof(Int16));
						break;
					case 24u:
						if (!host->EnsurePool24().Value) return nullptr;
						host->ipool24.SetValid(samplecount* host->ch * sizeof(Int24));
						break;
					case 32u:
						if (isfloating)
						{
							if (!host->EnsureFloatPool().Value) return nullptr;
							host->ipoolf32.SetValid(samplecount* host->ch * sizeof(float));
							break;
						}
						if (!host->EnsurePool32().Value) return nullptr;
						host->ipool32.SetValid(samplecount* host->ch * sizeof(Int32));
						break;
					case 64u:
						if (!host->EnsureDoublePool().Value) return nullptr;
						host->ipoolf64.SetValid(samplecount* host->ch * sizeof(double));
						break;
					}
				}
				else
				{
					switch (bpsnum)
					{
					case 8u:
						if (!host->EnsurePool8(false).Value) return false;
						host->spool8.SetValid(samplecount*sizeof(Int8), host->ch);
						break;
					case 16u:
						if (!host->EnsurePool16(false).Value) return false;
						host->spool16.SetValid(samplecount * sizeof(Int16), host->ch);
						break;
					case 24u:
						if (!host->EnsurePool24(false).Value) return false;
						host->spool24.SetValid(samplecount * sizeof(Int24), host->ch);
						break;
					case 32u:
						if (isfloating)
						{
							if (!host->EnsureFloatPool(false).Value) return false;
							host->spoolf32.SetValid(samplecount * sizeof(float), host->ch);
							break;
						}
						if (!host->EnsurePool32(false).Value) return false;
						host->spool32.SetValid(samplecount * sizeof(Int32), host->ch);
						break;
					case 64u:
						if (!host->EnsureDoublePool(false).Value) return false;
						host->spoolf64.SetValid(samplecount * sizeof(double), host->ch);
						break;
					}

				}

				host->samplecount = samplecount;
			}

			UInt GetSampleRate() const
			{
				return host->sr;
			}

			Result<bool> SetSampleRate(UInt sampleRate) const
			{
				Result<bool> result(false);
				if (sampleRate < SAMPLERATE_MIN)
				{
					result.WriteMessage(SAMPLERATE_TOOLOW);
					return result;
				}
				else if (sampleRate > SAMPLERATE_MAX)
				{
					result.WriteMessage(SAMPLERATE_TOOLARGE);
					return result;
				}
				host->sr = sampleRate;
				result.Value = true;
				return result;
			}

			BitsPerSample GetBitsPerSample() const
			{
				return bps;
			}

			UShort GetChannels() const
			{
				return host->Channels;
			}

			Result<bool> Reset(UInt sampleRate, BitsPerSample bitsPerSample, Audio::ChannelLayout channelConfig, bool val)
			{
				Result<bool> result(false);
				if (sampleRate < SAMPLERATE_MIN)
				{
					result.WriteMessage(SAMPLERATE_TOOLOW);
					return result;
				}
				else if (sampleRate > SAMPLERATE_MAX)
				{
					result.WriteMessage(SAMPLERATE_TOOLARGE);
					return result;
				}
				if (!bitsPerSample.IsValid)
				{
					result.WriteMessage(BITSPERSAMPLE_UNSUPPORTED);
					return result;
				}
				if (channelConfig.IsValid)
				{
					result.WriteMessage(CHANNELCONFIG_INVALID);
					return result;
				}
				if (channelConfig.Count < CHANNELS_MIN && channelConfig.Count > CHANNELS_MAX)
				{
					result.WriteMessage(CHANNELCONFIG_INVALID);
					return result;
				}
				switch (host->bpsnum)
				{
				case 8:
					if (!host->EnsurePool8(interleaved).Value)
					{
						result.WriteMessage(L"unable to allocate pool");
						return result;
					}
					break;
				case 16:
					if (!host->EnsurePool16(interleaved).Value)
					{
						result.WriteMessage(L"unable to allocate pool");
						return result;
					}
					break;
				case 24:
					if (!host->EnsurePool24(interleaved).Value)
					{
						result.WriteMessage(L"unable to allocate pool");
						return result;
					}
					break;
				case 32:
					if (host->bps.IsFloating)
					{
						if (!host->EnsureFloatPool(interleaved).Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
					}
					else
					{
						if (!host->EnsurePool32(interleaved).Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
					}
					break;
				case 64:
					if (!host->EnsureDoublePool(interleaved).Value)
					{
						result.WriteMessage(L"unable to allocate pool");
						return result;
					}
					break;
				}

				host->sr = sampleRate;
				host->bps = bitsPerSample;
				host->bpsnum = bps.BPS;
				this->bps = bps;
				SetInterleaved(val);

				host->ch = channelConfig.Count;
				host->chlayout = channelConfig;
				result.Value = true;
				return result;
			}

			Result<bool> EnsureSampleCount(UInt samplecount, bool interleaved)
			{
				Result<bool> result(false);
				if (!bps.IsValid)
				{
					result.WriteMessage(L"invalid chunk bps");
					return result;
				}

				auto bpsnum = bps.BPS;
				auto isfloating = bps.IsFloating;

				if (interleaved)
				{
					switch (bpsnum)
					{
					case 8u:
						if (!host->EnsurePool8().Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->ipool8.EnsureSize(sizeof(Int8)*host->ch*samplecount);
						break;
					case 16u:
						if (!host->EnsurePool16().Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->ipool16.EnsureSize(sizeof(Int16)*host->ch*samplecount);
						break;
					case 24u:
						if (!host->EnsurePool24().Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->ipool24.EnsureSize(sizeof(Int24)*host->ch*samplecount);
						break;
					case 32u:
						if (isfloating)
						{
							if (!host->EnsureFloatPool().Value)
							{
								result.WriteMessage(L"unable to allocate pool");
								return result;
							}
							host->ipoolf32.EnsureSize(sizeof(float)*host->ch*samplecount);
						}
						else
						{
							if (!host->EnsurePool32().Value)
							{
								result.WriteMessage(L"unable to allocate pool");
								return result;
							}
							host->ipool32.EnsureSize(sizeof(Int32)*host->ch*samplecount);
						}
						break;
					case 64u:
						if (!host->EnsureDoublePool().Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->ipoolf64.EnsureSize(sizeof(double)*host->ch*samplecount);
						break;
					}
				}
				else
				{
					switch (bpsnum)
					{
					case 8u:
						if (!host->EnsurePool8(false).Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->spool8.EnsureSize(sizeof(Int8) * samplecount, host->ch);
						break;
					case 16u:
						if (!host->EnsurePool16(false).Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->spool16.EnsureSize(sizeof(Int16) * samplecount, host->ch);
						break;
					case 24u:
						if (!host->EnsurePool24(false).Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->spool24.EnsureSize(sizeof(Int24) * samplecount, host->ch);
						break;
					case 32u:
						if (isfloating)
						{
							if (!host->EnsureFloatPool(false).Value)
							{
								result.WriteMessage(L"unable to allocate pool");
								return result;
							}
							host->spoolf32.EnsureSize(sizeof(float) * samplecount, host->ch);
						}
						else
						{
							if (!host->EnsurePool32(false).Value)
							{
								result.WriteMessage(L"unable to allocate pool");
								return result;
							}
							host->spool32.EnsureSize(sizeof(Int32) * samplecount, host->ch);
						}
						break;
					case 64u:
						if (!host->EnsureDoublePool(false).Value)
						{
							result.WriteMessage(L"unable to allocate pool");
							return result;
						}
						host->spoolf64.EnsureSize(sizeof(double) * samplecount, host->ch);
						break;
					}
				}

				result.Value = true;
				return result;
			}
		};
	}
}

AudioBuffer::AudioBuffer(UInt sampleRate, Audio::BitsPerSample bitsPerSample, Audio::ChannelLayout channelConfig) :samplecount(0ul), duration(0.0), internal(nullptr)
{
	if (sampleRate < SAMPLERATE_MIN) throw 1;
	if (sampleRate > SAMPLERATE_MAX) throw 2;
	
	sr=sampleRate;

	if (bitsPerSample.IsValid)
	{
		bps = bitsPerSample;
		bpsnum = bps.BPS;
	}
	else
		throw 3;
	
	if (channelConfig.IsValid)
	{
		ch = channelConfig.Count;
		if (ch >= CHANNELS_MIN && ch <= CHANNELS_MAX)
		{
			chlayout = channelConfig;
		}
		else
			throw 4;
	}
	else
		throw 4;

	dither=new DitherGenerator();
	InitializeDitherState(dither);
}

AudioBuffer::~AudioBuffer()
{
	SAFE_DELETE(dither);

	if (internal)
	{
		AudioBufferHistory* history = static_cast<AudioBufferHistory*>(internal);
		if (history)
		{
			delete history;
			history = nullptr;
			internal = nullptr;
		}
	}
}

Result<bool> AudioBuffer::Reset(UInt sampleRate, Audio::BitsPerSample bitsPerSample, Audio::ChannelLayout channelConfig)
{
	Result<bool> result(false);
	ipool8.Reset();
	ipool16.Reset();
	ipool24.Reset();
	ipool32.Reset();
	ipoolf32.Reset();
	ipoolf64.Reset();
	ipoolout.Reset();
	spool8.Reset();
	spool16.Reset();
	spool24.Reset();
	spool32.Reset();
	spoolf32.Reset();
	spoolf64.Reset();
	spoolout.Reset();

	if (internal)
	{
		AudioBufferHistory* history = (AudioBufferHistory*)internal;
		if (history->chunk)
		{
			delete history->chunk;
		}
		delete history;
		internal = nullptr;
	}

	samplecount = 0ul;
	duration = 0.0;

	if (sampleRate < SAMPLERATE_MIN)
	{
		result.WriteMessage(SAMPLERATE_TOOLOW);
	}
	else if (sampleRate > SAMPLERATE_MAX)
	{
		result.WriteMessage(SAMPLERATE_TOOLARGE);
	}
	else
	{
		sr = sampleRate;

		if (bitsPerSample.IsValid)
		{
			bps = bitsPerSample;
			bpsnum = bps.BPS;

			if (channelConfig.IsValid)
			{
				ch = channelConfig.Count;
				if (ch >= CHANNELS_MIN && ch <= CHANNELS_MAX)
				{
					chlayout = channelConfig;
					result.Value = true;
				}
				else
					result.WriteMessage(CHANNELCONFIG_INVALID);
			}
			else
				result.WriteMessage(CHANNELCONFIG_INVALID);
		}
		else
		{
			result.WriteMessage(BITSPERSAMPLE_UNSUPPORTED);
		}
	}
	return result;
}

Result<bool> AudioBuffer::EnsurePool8(bool interleaved)
{
	Result<bool> result(false);
	if (!samplecount)
	{
		result.WriteMessage(SAMPLECOUNT_INVALID);
		return result;
	}

	if (interleaved)
	{
		if (ipool8.Valid() != 0 && ipool8.Ptr())
		{
			result.Value = true;
			return result;
		}
	}
	else
	{
		if (spool8.Valid() != 0 && spool8.Ptr())
		{
			result.Value = true;
			return result;
		}
	}

	bool selfinterleaved = *interleavedchunk;

	if (selfinterleaved)
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int8)*count;
				ipool8.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_Int8(ipool8.Ptr(), ipool16.Ptr(), count);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_Int8_Dither(ipool8.Ptr(), ipool24.Ptr(), count, dither);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_Int8_Dither(ipool8.Ptr(), ipoolf32.Ptr(), count, dither);
							ipool8.SetValid(bytes);
						}
						else
							ipool8.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_Int8_Dither(ipool8.Ptr(), ipool32.Ptr(), count, dither);
							ipool8.SetValid(bytes);
						}
						else
							ipool8.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_Int8_Dither(ipool8.Ptr(), ipoolf64.Ptr(), count, dither);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int8)*samplecount;
				spool8.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_SplittedInt8((void**)spool8.Ptr(), ipool8.Ptr(), count, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_SplittedInt8((void**)spool8.Ptr(), ipool16.Ptr(), count, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_SplittedInt8_Dither((void**)spool8.Ptr(), ipool24.Ptr(), count, dither, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_SplittedInt8_Dither((void**)spool8.Ptr(), ipoolf32.Ptr(), count, dither, ch);
							spool8.SetValid(perbytes, ch);
						}
						else
							spool8.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_SplittedInt8_Dither((void**)spool8.Ptr(), ipool32.Ptr(), count, dither, ch);
							spool8.SetValid(perbytes, ch);
						}
						else
							spool8.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_SplittedInt8_Dither((void**)spool8.Ptr(), ipoolf64.Ptr(), count, dither, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	else
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int8)*count;
				ipool8.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_Int8(ipool8.Ptr(), (const void**)ipool8.Ptr(), count, ch);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_Int8(ipool8.Ptr(), (const void**)ipool16.Ptr(), count, ch);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_Int8_Dither(ipool8.Ptr(), (const void**)ipool24.Ptr(), count, dither, ch);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_Int8_Dither(ipool8.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							ipool8.SetValid(bytes);
						}
						else
							ipool8.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_Int8_Dither(ipool8.Ptr(), (const void**)spool32.Ptr(), count, dither, ch);
							ipool8.SetValid(bytes);
						}
						else
							ipool8.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_Int8_Dither(ipool8.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						ipool8.SetValid(bytes);
					}
					else
						ipool8.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int8)*samplecount;
				spool8.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_SplittedInt8((void**)spool8.Ptr(), (const void**)spool16.Ptr(), count, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spool24.Ptr(), count, dither, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							spool8.SetValid(perbytes, ch);
						}
						else
							spool8.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spool32.Ptr(), count, dither, ch);
							spool8.SetValid(perbytes, ch);
						}
						else
							spool8.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						spool8.SetValid(perbytes, ch);
					}
					else
						spool8.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	return result;
}

Result<bool> AudioBuffer::EnsurePool16(bool interleaved)
{
	Result<bool> result(false);
	if (!samplecount)
	{
		result.WriteMessage(SAMPLECOUNT_INVALID);
		return result;
	}

	if (interleaved)
	{
		if (ipool16.Valid() != 0 && ipool16.Ptr())
		{
			result.Value = true;
			return result;
		}
	}
	else
	{
		if (spool16.Valid() != 0 && spool16.Ptr())
		{
			result.Value = true;
			return result;
		}
	}

	bool selfinterleaved = *interleavedchunk;

	if (selfinterleaved)
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int16)*count;
				ipool16.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int16)*count)
					{
						Int8_To_Int16(ipool16.Ptr(), ipool8.Ptr(), count);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_Int16_Dither(ipool16.Ptr(), ipool24.Ptr(), count, dither);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_Int16_Dither(ipool16.Ptr(), ipoolf32.Ptr(), count, dither);
							ipool16.SetValid(bytes);
						}
						else
							ipool16.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_Int16_Dither(ipool16.Ptr(), ipool32.Ptr(), count, dither);
							ipool16.SetValid(bytes);
						}
						else
							ipool16.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_Int16_Dither(ipool16.Ptr(), ipoolf64.Ptr(), count, dither);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int16)*samplecount;
				spool16.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_SplittedInt16((void**)spool16.Ptr(), ipool8.Ptr(), count, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_SplittedInt16((void**)spool16.Ptr(), ipool16.Ptr(), count, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_SplittedInt16_Dither((void**)spool16.Ptr(), ipool24.Ptr(), count, dither, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_SplittedInt16_Dither((void**)spool16.Ptr(), ipoolf32.Ptr(), count, dither, ch);
							spool16.SetValid(perbytes, ch);
						}
						else
							spool16.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_SplittedInt16_Dither((void**)spool16.Ptr(), ipool32.Ptr(), count, dither, ch);
							spool16.SetValid(perbytes, ch);
						}
						else
							spool16.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_SplittedInt16_Dither((void**)spool16.Ptr(), ipoolf64.Ptr(), count, dither, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	else
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int16)*count;
				ipool16.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_Int16(ipool16.Ptr(), (const void**)ipool8.Ptr(), count, ch);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_Int16(ipool16.Ptr(), (const void**)ipool16.Ptr(), count, ch);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_Int16_Dither(ipool16.Ptr(), (const void**)ipool24.Ptr(), count, dither, ch);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_Int16_Dither(ipool16.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							ipool16.SetValid(bytes);
						}
						else
							ipool16.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_Int16_Dither(ipool16.Ptr(), (const void**)spool32.Ptr(), count, dither, ch);
							ipool16.SetValid(bytes);
						}
						else
							ipool16.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_Int16_Dither(ipool16.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						ipool16.SetValid(bytes);
					}
					else
						ipool16.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int16)*samplecount;
				spool16.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_SplittedInt16((void**)spool16.Ptr(), (const void**)spool8.Ptr(), count, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spool24.Ptr(), count, dither, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							spool16.SetValid(perbytes, ch);
						}
						else
							spool16.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spool32.Ptr(), count, dither, ch);
							spool16.SetValid(perbytes, ch);
						}
						else
							spool16.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						spool16.SetValid(perbytes, ch);
					}
					else
						spool16.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	return result;
}

Result<bool> AudioBuffer::EnsurePool24(bool interleaved)
{
	Result<bool> result(false);
	if (!samplecount)
	{
		result.WriteMessage(SAMPLECOUNT_INVALID);
		return result;
	}

	if (interleaved)
	{
		if (ipool24.Valid() != 0 && ipool24.Ptr())
		{
			result.Value = true;
			return result;
		}
	}
	else
	{
		if (spool24.Valid() != 0 && spool24.Ptr())
		{
			result.Value = true;
			return result;
		}
	}

	bool selfinterleaved = *interleavedchunk;

	if (selfinterleaved)
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int24)*count;
				ipool24.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_Int24(ipool24.Ptr(), ipool8.Ptr(), count);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_Int24(ipool24.Ptr(), ipool16.Ptr(), count);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_Int24_Dither(ipool24.Ptr(), ipoolf32.Ptr(), count, dither);
							ipool24.SetValid(bytes);
						}
						else
							ipool24.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_Int24(ipool24.Ptr(), ipool32.Ptr(), count);
							ipool24.SetValid(bytes);
						}
						else
							ipool24.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_Int24_Dither(ipool24.Ptr(), ipoolf64.Ptr(), count, dither);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int24)*samplecount;
				spool24.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_SplittedInt24((void**)spool24.Ptr(), ipool8.Ptr(), count, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_SplittedInt24((void**)spool24.Ptr(), ipool16.Ptr(), count, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_SplittedInt24((void**)spool24.Ptr(), ipool24.Ptr(), count, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_SplittedInt24_Dither((void**)spool24.Ptr(), ipoolf32.Ptr(), count, dither, ch);
							spool24.SetValid(perbytes, ch);
						}
						else
							spool24.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_SplittedInt24((void**)spool24.Ptr(), ipool32.Ptr(), count, ch);
							spool24.SetValid(perbytes, ch);
						}
						else
							spool24.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_SplittedInt24_Dither((void**)spool24.Ptr(), ipoolf64.Ptr(), count, dither, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	else
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int24)*count;
				ipool24.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_Int24(ipool24.Ptr(), (const void**)ipool8.Ptr(), count, ch);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_Int24(ipool24.Ptr(), (const void**)ipool16.Ptr(), count, ch);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_Int24(ipool24.Ptr(), (const void**)ipool24.Ptr(), count, ch);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_Int24_Dither(ipool24.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							ipool24.SetValid(bytes);
						}
						else
							ipool24.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_Int24(ipool24.Ptr(), (const void**)spool32.Ptr(), count, ch);
							ipool24.SetValid(bytes);
						}
						else
							ipool24.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_Int24_Dither(ipool24.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						ipool24.SetValid(bytes);
					}
					else
						ipool24.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int24)*samplecount;
				spool24.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_SplittedInt24((void**)spool24.Ptr(), (const void**)spool8.Ptr(), count, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_SplittedInt24((void**)spool24.Ptr(), (const void**)spool16.Ptr(), count, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_SplittedInt24_Dither((void**)spool24.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							spool24.SetValid(perbytes, ch);
						}
						else
							spool24.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_SplittedInt24((void**)spool24.Ptr(), (const void**)spool32.Ptr(), count, ch);
							spool24.SetValid(perbytes, ch);
						}
						else
							spool24.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_SplittedInt24_Dither((void**)spool24.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						spool24.SetValid(perbytes, ch);
					}
					else
						spool24.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	return result;
}

Result<bool> AudioBuffer::EnsurePool32(bool interleaved)
{
	Result<bool> result(false);
	if (!samplecount)
	{
		result.WriteMessage(SAMPLECOUNT_INVALID);
		return result;
	}

	if (interleaved)
	{
		if (ipool32.Valid() != 0 && ipool32.Ptr())
		{
			result.Value = true;
			return result;
		}
	}
	else
	{
		if (spool32.Valid() != 0 && spool32.Ptr())
		{
			result.Value = true;
			return result;
		}
	}

	bool selfinterleaved = *interleavedchunk;

	if (selfinterleaved)
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int32)*count;
				ipool32.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_Int32(ipool32.Ptr(), ipool8.Ptr(), count);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_Int32(ipool32.Ptr(), ipool16.Ptr(), count);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_Int32(ipool32.Ptr(), ipool24.Ptr(), count);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_Int32_Dither(ipool32.Ptr(), ipoolf32.Ptr(), count, dither);
							ipool32.SetValid(bytes);
						}
						else
							ipool32.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_Int32_Dither(ipool32.Ptr(), ipoolf64.Ptr(), count, dither);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int32)*samplecount;
				spool32.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_SplittedInt32((void**)spool32.Ptr(), ipool8.Ptr(), count, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_SplittedInt32((void**)spool32.Ptr(), ipool16.Ptr(), count, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_SplittedInt32((void**)spool32.Ptr(), ipool24.Ptr(), count, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_SplittedInt32_Dither((void**)spool32.Ptr(), ipoolf32.Ptr(), count, dither, ch);
							spool32.SetValid(perbytes, ch);
						}
						else
							spool32.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_SplittedInt32((void**)spool32.Ptr(), ipool32.Ptr(), count, ch);
							spool32.SetValid(perbytes, ch);
						}
						else
							spool32.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_SplittedInt32_Dither((void**)spool32.Ptr(), ipoolf64.Ptr(), count, dither, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	else
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Int32)*count;
				ipool32.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_Int32(ipool32.Ptr(), (const void**)ipool8.Ptr(), count, ch);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_Int32(ipool32.Ptr(), (const void**)ipool16.Ptr(), count, ch);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_Int32(ipool32.Ptr(), (const void**)ipool24.Ptr(), count, ch);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_Int32_Dither(ipool32.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							ipool32.SetValid(bytes);
						}
						else
							ipool32.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_Int32(ipool32.Ptr(), (const void**)spool32.Ptr(), count, ch);
							ipool32.SetValid(bytes);
						}
						else
							ipool32.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_Int32_Dither(ipool32.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						ipool32.SetValid(bytes);
					}
					else
						ipool32.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Int32)*samplecount;
				spool32.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_SplittedInt32((void**)spool32.Ptr(), (const void**)spool8.Ptr(), count, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_SplittedInt32((void**)spool32.Ptr(), (const void**)spool16.Ptr(), count, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_SplittedInt32((void**)spool32.Ptr(), (const void**)spool24.Ptr(), count, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_SplittedInt32_Dither((void**)spool32.Ptr(), (const void**)spoolf32.Ptr(), count, dither, ch);
							spool32.SetValid(perbytes, ch);
						}
						else
							spool32.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_SplittedInt32_Dither((void**)spool32.Ptr(), (const void**)spoolf64.Ptr(), count, dither, ch);
						spool32.SetValid(perbytes, ch);
					}
					else
						spool32.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	return result;
}

Result<bool> AudioBuffer::EnsureFloatPool(bool interleaved)
{
	Result<bool> result(false);
	if (!samplecount)
	{
		result.WriteMessage(SAMPLECOUNT_INVALID);
		return result;
	}

	if (interleaved)
	{
		if (ipoolf32.Valid() != 0 && ipoolf32.Ptr())
		{
			result.Value = true;
			return result;
		}
	}
	else
	{
		if (spoolf32.Valid() != 0 && spoolf32.Ptr())
		{
			result.Value = true;
			return result;
		}
	}

	bool selfinterleaved = *interleavedchunk;

	if (selfinterleaved)
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Float32)*count;
				ipoolf32.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_Float32(ipoolf32.Ptr(), ipool8.Ptr(), count);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_Float32(ipoolf32.Ptr(), ipool16.Ptr(), count);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_Float32(ipoolf32.Ptr(), ipool24.Ptr(), count);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				case 32:
				{
					if (!bps.IsFloating)
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_Float32(ipoolf32.Ptr(), ipool32.Ptr(), count);
							ipoolf32.SetValid(bytes);
						}
						else
							ipoolf32.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_Float32(ipoolf32.Ptr(), ipoolf64.Ptr(), count);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Float32)*samplecount;
				spoolf32.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_SplittedFloat32((void**)spoolf32.Ptr(), ipool8.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_SplittedFloat32((void**)spoolf32.Ptr(), ipool16.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_SplittedFloat32((void**)spoolf32.Ptr(), ipool24.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_SplittedFloat32((void**)spoolf32.Ptr(), ipoolf32.Ptr(), count, ch);
							spoolf32.SetValid(perbytes, ch);
						}
						else
							spoolf32.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_SplittedFloat32((void**)spoolf32.Ptr(), ipool32.Ptr(), count, ch);
							spoolf32.SetValid(perbytes, ch);
						}
						else
							spoolf32.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_SplittedFloat32((void**)spoolf32.Ptr(), ipoolf64.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	else
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Float32)*count;
				ipoolf32.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_Float32(ipoolf32.Ptr(), (const void**)ipool8.Ptr(), count, ch);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_Float32(ipoolf32.Ptr(), (const void**)ipool16.Ptr(), count, ch);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_Float32(ipoolf32.Ptr(), (const void**)ipool24.Ptr(), count, ch);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_Float32(ipoolf32.Ptr(), (const void**)spoolf32.Ptr(), count, ch);
							ipoolf32.SetValid(bytes);
						}
						else
							ipoolf32.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_Float32(ipoolf32.Ptr(), (const void**)spool32.Ptr(), count, ch);
							ipoolf32.SetValid(bytes);
						}
						else
							ipoolf32.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_Float32(ipoolf32.Ptr(), (const void**)spoolf64.Ptr(), count, ch);
						ipoolf32.SetValid(bytes);
					}
					else
						ipoolf32.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Float32)*samplecount;
				spoolf32.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool8.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool16.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool24.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				case 32:
				{
					if (!bps.IsFloating)
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool32.Ptr(), count, ch);
							spoolf32.SetValid(perbytes, ch);
						}
						else
							spoolf32.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spoolf64.Ptr(), count, ch);
						spoolf32.SetValid(perbytes, ch);
					}
					else
						spoolf32.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	return result;
}

Result<bool> AudioBuffer::EnsureDoublePool(bool interleaved)
{
	Result<bool> result(false);
	if (!samplecount)
	{
		result.WriteMessage(SAMPLECOUNT_INVALID);
		return result;
	}

	if (interleaved)
	{
		if (ipoolf64.Valid() != 0 && ipoolf64.Ptr())
		{
			result.Value = true;
			return result;
		}
	}
	else
	{
		if (spoolf64.Valid() != 0 && spoolf64.Ptr())
		{
			result.Value = true;
			return result;
		}
	}

	bool selfinterleaved = *interleavedchunk;

	if (selfinterleaved)
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Float64)*count;
				ipoolf64.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_Float64(ipoolf64.Ptr(), ipool8.Ptr(), count);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_Float64(ipoolf64.Ptr(), ipool16.Ptr(), count);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_Float64(ipoolf64.Ptr(), ipool24.Ptr(), count);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipool32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_Float64(ipoolf64.Ptr(), ipoolf32.Ptr(), count);
							ipoolf64.SetValid(bytes);
						}
						else
							ipoolf64.Clear();
						
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_Float64(ipoolf64.Ptr(), ipool32.Ptr(), count);
							ipoolf64.SetValid(bytes);
						}
						else
							ipoolf64.Clear();
					}
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Float64)*samplecount;
				spoolf64.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (ipool8.Valid() == sizeof(Int8)*count)
					{
						Int8_To_SplittedFloat64((void**)spoolf64.Ptr(), ipool8.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				case 16:
				{
					if (ipool16.Valid() == sizeof(Int16)*count)
					{
						Int16_To_SplittedFloat64((void**)spoolf64.Ptr(), ipool16.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				case 24:
				{
					if (ipool24.Valid() == sizeof(Int24)*count)
					{
						Int24_To_SplittedFloat64((void**)spoolf64.Ptr(), ipool24.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (ipoolf32.Valid() == sizeof(Float32)*count)
						{
							Float32_To_SplittedFloat64((void**)spoolf64.Ptr(), ipoolf32.Ptr(), count, ch);
							spoolf64.SetValid(perbytes, ch);
						}
						else
							spoolf64.Clear();
					}
					else
					{
						if (ipool32.Valid() == sizeof(Int32)*count)
						{
							Int32_To_SplittedFloat64((void**)spoolf64.Ptr(), ipool32.Ptr(), count, ch);
							spoolf64.SetValid(perbytes, ch);
						}
						else
							spoolf64.Clear();
					}
				}
				break;
				case 64:
				{
					if (ipoolf64.Valid() == sizeof(Float64)*count)
					{
						Float64_To_SplittedFloat64((void**)spoolf64.Ptr(), ipoolf64.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	else
	{
		try
		{
			if (interleaved)
			{
				UInt count = samplecount * ch;
				UInt bytes = sizeof(Float64)*count;
				ipoolf64.EnsureSize(bytes);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_Float64(ipoolf64.Ptr(), (const void**)ipool8.Ptr(), count, ch);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_Float64(ipoolf64.Ptr(), (const void**)ipool16.Ptr(), count, ch);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_Float64(ipoolf64.Ptr(), (const void**)ipool24.Ptr(), count, ch);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spoolf32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_Float64(ipoolf64.Ptr(), (const void**)spoolf32.Ptr(), count, ch);
							ipoolf64.SetValid(bytes);
						}
						else
							ipoolf64.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_Float64(ipoolf64.Ptr(), (const void**)spool32.Ptr(), count, ch);
							ipoolf64.SetValid(bytes);
						}
						else
							ipoolf64.Clear();
					}
				}
				break;
				case 64:
				{
					if (spoolf64.Valid() == sizeof(Float64)*count)
					{
						SplittedFloat64_To_Float64(ipoolf64.Ptr(), (const void**)spoolf64.Ptr(), count, ch);
						ipoolf64.SetValid(bytes);
					}
					else
						ipoolf64.Clear();
				}
				break;
				}
				result.Value = true;
			}
			else
			{
				UInt count = samplecount * ch;
				UInt perbytes = sizeof(Float64)*samplecount;
				spoolf64.EnsureSize(perbytes, ch);
				switch (bpsnum)
				{
				case 8:
				{
					if (spool8.Valid() == sizeof(Int8)*count)
					{
						SplittedInt8_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool8.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				case 16:
				{
					if (spool16.Valid() == sizeof(Int16)*count)
					{
						SplittedInt16_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool16.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				case 24:
				{
					if (spool24.Valid() == sizeof(Int24)*count)
					{
						SplittedInt24_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool24.Ptr(), count, ch);
						spoolf64.SetValid(perbytes, ch);
					}
					else
						spoolf64.Clear();
				}
				break;
				case 32:
				{
					if (bps.IsFloating)
					{
						if (spool32.Valid() == sizeof(Float32)*count)
						{
							SplittedFloat32_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spoolf32.Ptr(), count, ch);
							spoolf64.SetValid(perbytes, ch);
						}
						else
							spoolf64.Clear();
					}
					else
					{
						if (spool32.Valid() == sizeof(Int32)*count)
						{
							SplittedInt32_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool32.Ptr(), count, ch);
							spoolf64.SetValid(perbytes, ch);
						}
						else
							spoolf64.Clear();
					}
				}
				break;
				}
				result.Value = true;
			}
		}
		catch (...)
		{
			result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		}
	}
	return result;
}

Result<bool> AudioBuffer::SetData(void* data, UInt sampleRate, Audio::BitsPerSample bitsPerSample, UInt samples, Audio::ChannelLayout channelConfig, double startMs)
{
	Result<bool> result(false);

	auto old_sr = sr;
	auto old_bps = bps;
	auto old_bpsnum = bpsnum;
	auto old_chcfg = chlayout;
	auto old_ch = ch;

	if(startMs<0.0) startms=0.0;
	if (sampleRate != sr)
	{
		if (sampleRate < SAMPLERATE_MIN)
		{
			result.WriteMessage(SAMPLERATE_TOOLOW);
			return result;
		}
		else if (sampleRate > SAMPLERATE_MAX)
		{
			result.WriteMessage(SAMPLERATE_TOOLARGE);
			return result;
		}
		else
			sr = sampleRate;
	}
	if (bitsPerSample != bps)
	{
		if (bitsPerSample.IsValid)
		{
			bps = bitsPerSample;
			bpsnum = bps.BPS;
		}
		else
		{
			sr = old_sr;
			result.WriteMessage(BITSPERSAMPLE_UNSUPPORTED);
			return result;
		}
	}
	if (chlayout != channelConfig)
	{
		if (channelConfig.IsValid)
		{
			ch = channelConfig.Count;
			if (ch >= CHANNELS_MIN && ch <= CHANNELS_MAX)
			{
				chlayout = channelConfig;
			}
			else
			{
				sr = old_sr;
				bps = old_bps;
				bpsnum = old_bpsnum;
				result.WriteMessage(CHANNELCONFIG_INVALID);
				return result;
			}
		}
		else
		{
			sr = old_sr;
			bps = old_bps;
			bpsnum = old_bpsnum;
			result.WriteMessage(CHANNELCONFIG_INVALID);
			return result;
		}
	}

	if(!data)
	{
		ipool8.Clear();
		ipool16.Clear();
		ipool24.Clear();
		ipool32.Clear();
		ipoolf32.Clear();
		ipoolf64.Clear();
		ipoolout.Clear();
		spool8.Clear();
		spool16.Clear();
		spool24.Clear();
		spool32.Clear();
		spoolf32.Clear();
		spoolf64.Clear();
		spoolout.Clear();
		sr = old_sr;
		bps = old_bps;
		bpsnum = old_bpsnum;
		chlayout = old_chcfg;
		ch = old_ch;
		interleavedchunk.Reset();
		return result;
	}

	samplecount = samples;

	duration = (double)samples / (double)sr;

	try
	{
		UInt n=0;
		UInt count=samplecount*ch;
		UInt bytes=count * bpsnum>>3;
		switch (bps)
		{
			case BitsPerSample::SInt8:
			{
				ipool8.EnsureSize(bytes);
				memcpy(ipool8.Ptr(), data, bytes);
				ipool8.SetValid(bytes);
				ipool16.Clear();
				ipool24.Clear();
				ipool32.Clear();
				ipoolf32.Clear();
				ipoolf64.Clear();
				ipoolout.Clear();
				spool8.Clear();
				spool16.Clear();
				spool24.Clear();
				spool32.Clear();
				spoolf32.Clear();
				spoolf64.Clear();
				spoolout.Clear();
			}
				break;
			case BitsPerSample::UInt8:
				{
					UInt8* mem=(UInt8*)data;
					ipool8.EnsureSize(bytes);
					Int8* pool8 = ipool8.Ptr();
					for (n = 0; n<count; n++)
					{
						pool8[n] = (Int8)(mem[n] ^ 127);
					}
					ipool8.SetValid(bytes);
					ipool16.Clear();
					ipool24.Clear();
					ipool32.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;
			case BitsPerSample::SInt16:
				{
					Int16* mem = (Int16*)data;
					ipool16.EnsureSize(bytes);
					Int16* pool16 = ipool16.Ptr();
					memcpy(pool16,data,bytes);
					ipool16.SetValid(bytes);
					ipool8.Clear();
					ipool24.Clear();
					ipool32.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;
			case BitsPerSample::UInt16:
				{
					UInt16* mem = (UInt16*)data;
					ipool16.EnsureSize(bytes);
					Int16* pool16 = ipool16.Ptr();
					for(n=0;n<count;n++)
					{
						pool16[n]=(Int16)(mem[n]^32767);
					}
					ipool16.SetValid(bytes);
					ipool8.Clear();
					ipool24.Clear();
					ipool32.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;
			case BitsPerSample::SInt24:
				{
					Int24* mem = (Int24*)data;
					ipool24.EnsureSize(bytes);
					Int24* pool24 = ipool24.Ptr();
					memcpy(pool24, data, bytes); 
					ipool24.SetValid(bytes);
					ipool8.Clear();
					ipool16.Clear();
					ipool32.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;
			case BitsPerSample::UInt24:
				{
					UInt24* mem = (UInt24*)data;
					ipool24.EnsureSize(bytes);
					Int24* pool24 = ipool24.Ptr();
					for(n=0;n<count;n++)
					{
						pool24[n]= (int)mem[n]^8388607;
					}
					ipool24.SetValid(bytes);
					ipool8.Clear();
					ipool16.Clear();
					ipool32.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;
			case BitsPerSample::SInt32:
				{
					Int32* mem = (Int32*)data;
					ipool32.EnsureSize(bytes);
					Int32* pool32 = ipool32.Ptr();
					memcpy(pool32, data, bytes);
					ipool32.SetValid(bytes);
					ipool8.Clear();
					ipool16.Clear();
					ipool24.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;
			case BitsPerSample::UInt32:
				{
					UInt32* mem = (UInt32*)data;
					ipool32.EnsureSize(bytes);
					Int32* pool32 = ipool32.Ptr();
					for(n=0;n<count;n++)
					{
						pool32[n]=mem[n]^2147483647;
					}
					ipool32.SetValid(bytes);
					ipool8.Clear();
					ipool16.Clear();
					ipool24.Clear();
					ipoolf32.Clear();
					ipoolf64.Clear();
					ipoolout.Clear();
					spool8.Clear();
					spool16.Clear();
					spool24.Clear();
					spool32.Clear();
					spoolf32.Clear();
					spoolf64.Clear();
					spoolout.Clear();
				}
				break;			
			case BitsPerSample::Float32:
			{
				Float32* mem = (Float32*)data;
				ipoolf32.EnsureSize(bytes);
				Float32* floatpool = ipoolf32.Ptr();
				memcpy(floatpool, data, bytes);
				ipoolf32.SetValid(bytes);
				ipool8.Clear();
				ipool16.Clear();
				ipool24.Clear();
				ipool32.Clear();
				ipoolf64.Clear();
				ipoolout.Clear();
				spool8.Clear();
				spool16.Clear();
				spool24.Clear();
				spool32.Clear();
				spoolf32.Clear();
				spoolf64.Clear();
				spoolout.Clear();
			}
			break;
			case BitsPerSample::Float64:
			{
				Float64* mem = (Float64*)data;
				ipoolf64.EnsureSize(bytes);
				Float64* doublepool = ipoolf64.Ptr();
				memcpy(doublepool, data, bytes);
				ipoolf64.SetValid(bytes);
				ipool8.Clear();
				ipool16.Clear();
				ipool24.Clear();
				ipool32.Clear();
				ipoolf32.Clear();
				ipoolout.Clear();
				spool8.Clear();
				spool16.Clear();
				spool24.Clear();
				spool32.Clear();
				spoolf32.Clear();
				spoolf64.Clear();
				spoolout.Clear();
			}
			break;
		}
		interleavedchunk = true;
		result.Value = true;
	}
	catch(...)
	{
		sr = old_sr;
		bps = old_bps;
		bpsnum = old_bpsnum;
		chlayout = old_chcfg;
		ch = old_ch;
		samplecount = 0;
		duration = 0;
		result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		ipool8.Clear();
		ipool16.Clear();
		ipool24.Clear();
		ipool32.Clear();
		ipoolf32.Clear();
		ipoolf64.Clear();
		ipoolout.Clear();
		spool8.Clear();
		spool16.Clear();
		spool24.Clear();
		spool32.Clear();
		spoolf32.Clear();
		spoolf64.Clear();
		spoolout.Clear();
		interleavedchunk.Reset();
	}
	return result;
}

Result<bool> AudioBuffer::SetSplittedData(void** data, UInt sampleRate, Audio::BitsPerSample bitsPerSample, UInt samples, Audio::ChannelLayout channelConfig, double startMs)
{
	Result<bool> result(false);

	auto old_sr = sr;
	auto old_bps = bps;
	auto old_bpsnum = bpsnum;
	auto old_chcfg = chlayout;
	auto old_ch = ch;

	if (startMs < 0.0) startms = 0.0;
	if (sampleRate != sr)
	{
		if (sampleRate < SAMPLERATE_MIN)
		{
			result.WriteMessage(SAMPLERATE_TOOLOW);
			return result;
		}
		else if (sampleRate > SAMPLERATE_MAX)
		{
			result.WriteMessage(SAMPLERATE_TOOLARGE);
			return result;
		}
		else
			sr = sampleRate;
	}
	if (bitsPerSample != bps)
	{
		if (bitsPerSample.IsValid)
		{
			bps = bitsPerSample;
			bpsnum = bps.BPS;
		}
		else
		{
			sr = old_sr;
			result.WriteMessage(BITSPERSAMPLE_UNSUPPORTED);
			return result;
		}
	}
	if (chlayout != channelConfig)
	{
		if (channelConfig.IsValid)
		{
			ch = channelConfig.Count;
			if (ch >= CHANNELS_MIN && ch <= CHANNELS_MAX)
			{
				chlayout = channelConfig;
			}
			else
			{
				sr = old_sr;
				bps = old_bps;
				bpsnum = old_bpsnum;
				result.WriteMessage(CHANNELCONFIG_INVALID);
				return result;
			}
		}
		else
		{
			sr = old_sr;
			bps = old_bps;
			bpsnum = old_bpsnum;
			result.WriteMessage(CHANNELCONFIG_INVALID);
			return result;
		}
	}

	if (!data)
	{
		ipool8.Clear();
		ipool16.Clear();
		ipool24.Clear();
		ipool32.Clear();
		ipoolf32.Clear();
		ipoolf64.Clear();
		ipoolout.Clear();
		spool8.Clear();
		spool16.Clear();
		spool24.Clear();
		spool32.Clear();
		spoolf32.Clear();
		spoolf64.Clear();
		spoolout.Clear();
		sr = old_sr;
		bps = old_bps;
		bpsnum = old_bpsnum;
		chlayout = old_chcfg;
		ch = old_ch;
		interleavedchunk.Reset();
		return result;
	}

	samplecount = samples;

	duration = (double)samples / (double)sr;

	try
	{
		UInt n = 0;
		UInt count = samplecount;
		UInt bytes = count * bpsnum >> 3;
		switch (bps)
		{
		case BitsPerSample::SInt8:
		{
			spool8.EnsureSize(bytes, ch);
			CopySplittedChunk(&spool8, (Int8**)data, bytes, ch);
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt8:
		{
			spool8.EnsureSize(bytes, ch);
			CopySplittedChunk2(&spool8, (UInt8**)data, count, ch);
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::SInt16:
		{
			spool16.EnsureSize(bytes, ch);
			CopySplittedChunk(&spool16, (Int16**)data, bytes, ch);
			spool8.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt16:
		{
			spool16.EnsureSize(bytes, ch);
			CopySplittedChunk2(&spool16, (UInt16**)data, bytes, ch);
			spool8.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::SInt24:
		{
			spool24.EnsureSize(bytes, ch);
			CopySplittedChunk(&spool24, (Int24**)data, bytes, ch);
			spool8.Clear();
			spool16.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt24:
		{
			spool24.EnsureSize(bytes, ch);
			CopySplittedChunk2(&spool24, (UInt24**)data, bytes, ch);
			spool8.Clear();
			spool16.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::SInt32:
		{
			spool32.EnsureSize(bytes, ch);
			CopySplittedChunk(&spool32, (Int32**)data, bytes, ch);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt32:
		{
			spool32.EnsureSize(bytes, ch);
			CopySplittedChunk2(&spool32, (UInt32**)data, bytes, ch);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::Float32:
		{
			spoolf32.EnsureSize(bytes, ch);
			CopySplittedChunk(&spoolf32, (Float32**)data, bytes, ch);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::Float64:
		{
			spoolf64.EnsureSize(bytes, ch);
			CopySplittedChunk(&spoolf64, (Float64**)data, bytes, ch);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		}
		interleavedchunk = false;
		result.Value = true;
	}
	catch (...)
	{
		sr = old_sr;
		bps = old_bps;
		bpsnum = old_bpsnum;
		chlayout = old_chcfg;
		ch = old_ch;
		samplecount = 0;
		duration = 0;
		result.WriteMessage(MEMORY_ALLOCATE_ERROR);
		ipool8.Clear();
		ipool16.Clear();
		ipool24.Clear();
		ipool32.Clear();
		ipoolf32.Clear();
		ipoolf64.Clear();
		ipoolout.Clear();
		spool8.Clear();
		spool16.Clear();
		spool24.Clear();
		spool32.Clear();
		spoolf32.Clear();
		spoolf64.Clear();
		spoolout.Clear();
		interleavedchunk.Reset();
	}
	return result;
}

Result<bool> AudioBuffer::StartResetData(UInt sampleRate, Audio::BitsPerSample bitsPerSample, Audio::ChannelLayout channelConfig, double startMs)
{
	Result<bool> result(false);

	if (internal)
	{
		result.WriteMessage(L"invalid op");
		return result;
	}

	auto old_sr = sr;
	auto old_bps = bps;
	auto old_bpsnum = bpsnum;
	auto old_chcfg = chlayout;
	auto old_ch = ch;
	auto old_startms = startms;

	if (sampleRate != sr)
	{
		if (sampleRate < SAMPLERATE_MIN)
		{
			result.WriteMessage(SAMPLERATE_TOOLOW);
			return result;
		}
		else if (sampleRate > SAMPLERATE_MAX)
		{
			result.WriteMessage(SAMPLERATE_TOOLARGE);
			return result;
		}
		else
			sr = sampleRate;
	}
	if (bitsPerSample != bps)
	{
		if (bitsPerSample.IsValid)
		{
			bps = bitsPerSample;
			bpsnum = bps.BPS;
		}
		else
		{
			sr = old_sr;
			result.WriteMessage(BITSPERSAMPLE_UNSUPPORTED);
			return result;
		}
	}
	if (chlayout != channelConfig)
	{
		if (channelConfig.IsValid)
		{
			ch = channelConfig.Count;
			if (ch >= CHANNELS_MIN && ch <= CHANNELS_MAX)
			{
				chlayout = channelConfig;
			}
			else
			{
				sr = old_sr;
				bps = old_bps;
				bpsnum = old_bpsnum;
				result.WriteMessage(CHANNELCONFIG_INVALID);
				return result;
			}
		}
		else
		{
			sr = old_sr;
			bps = old_bps;
			bpsnum = old_bpsnum;
			result.WriteMessage(CHANNELCONFIG_INVALID);
			return result;
		}
	}
	if (startMs<0.0) startms = 0.0;

	AudioBufferHistory* history =new AudioBufferHistory();

	history->ipool8 = std::move(ipool8);
	history->ipool16 = std::move(ipool16);
	history->ipool24 = std::move(ipool24);
	history->ipool32= std::move(ipool32);
	history->ipoolf32 = std::move(ipoolf32);
	history->ipoolf64 = std::move(ipoolf64);
	history->spool8 = std::move(spool8);
	history->spool16 = std::move(spool16);
	history->spool24 = std::move(spool24);
	history->spool32 = std::move(spool32);
	history->spoolf32 = std::move(spoolf32);
	history->spoolf64 = std::move(spoolf64);
	history->interleaved = interleavedchunk;

	history->duration = duration;
	history->old_bps = old_bps;
	history->old_bpsnum = old_bpsnum;
	history->old_ch = old_ch;
	history->old_chcfg = old_chcfg;
	history->old_sr = old_sr;
	history->samplecount = samplecount;
	history->StartMs = old_startms;
	result.Value = true;
	internal = (void*)history;

	samplecount = 0;
	duration = 0.0;
	startms = startMs;
	interleavedchunk.Reset();

	return result;
}

Result<bool> AudioBuffer::AddData(void* data, UInt samples)
{
	Result<bool> result(false);
	if (!internal)
	{
		result.WriteMessage(L"no reset record");
		return result;
	}
	if (interleavedchunk)
	{
		if (!*interleavedchunk)
		{
			result.WriteMessage(L"invalid due to splitted mode");
			return result;
		}
	}

	size_t lastsamplecount = samplecount;
	size_t lastcur = lastsamplecount* ch;
	size_t newsamplecount = lastsamplecount + samples;
	try
	{
		UInt n = 0;
		UInt count = samples*ch;
		UInt bytes = count * bpsnum >> 3;
		size_t abytes = newsamplecount* ch* bpsnum >> 3;
		switch (bps)
		{
		case BitsPerSample::SInt8:
		{
			ipool8.EnsureSize(abytes);
			memcpy(ipool8.Ptr() + lastcur, data, bytes);
			ipool8.SetValid(abytes);
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
			break;
		case BitsPerSample::UInt8:
		{
			ipool8.EnsureSize(abytes);
			Int8* pool8 = ipool8.Ptr() + lastcur;
			UInt8* mem = (UInt8*)data;
			for (n = 0; n<count; n++)
			{
				pool8[n] = (Int8)(mem[n] ^ 127);
			}
			ipool8.SetValid(abytes);
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::SInt16:
		{
			ipool16.EnsureSize(abytes);
			Int16* pool16 = (Int16*)((char*)ipool16.Ptr() + lastcur);
			memcpy(pool16, data, bytes);
			ipool16.SetValid(abytes);
			ipool8.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::UInt16:
		{
			ipool16.EnsureSize(abytes);
			Int16* pool16 = (Int16*)((char*)ipool16.Ptr() + lastcur);
			UInt16* mem = (UInt16*)data;
			for (n = 0; n<count; n++)
			{
				pool16[n] = (Int16)(mem[n] ^ 32767);
			}
			ipool16.SetValid(abytes);
			ipool8.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::SInt24:
		{
			ipool24.EnsureSize(abytes);
			Int24* pool24 = (Int24*)((char*)ipool24.Ptr() + lastcur);
			memcpy(pool24, data, bytes);
			ipool24.SetValid(abytes);
			ipool8.Clear();
			ipool16.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::UInt24:
		{
			ipool24.EnsureSize(abytes);
			Int24* pool24 = (Int24*)((char*)ipool24.Ptr() + lastcur);
			UInt24* mem = (UInt24*)data;
			for (n = 0; n<count; n++)
			{
				pool24[n] = (int)mem[n] ^ 8388607;
			}
			ipool24.SetValid(abytes);
			ipool8.Clear();
			ipool16.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::SInt32:
		{
			ipool32.EnsureSize(abytes);
			Int32* pool32 = (Int32*)((char*)ipool32.Ptr() + lastcur);
			memcpy(pool32, data, bytes);
			ipool32.SetValid(abytes);
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::UInt32:
		{
			ipool32.EnsureSize(abytes);
			Int32* pool32 = (Int32*)((char*)ipool32.Ptr() + lastcur);
			UInt32* mem = (UInt32*)data;
			for (n = 0; n<count; n++)
			{
				pool32[n] = mem[n] ^ 2147483647;
			}
			ipool32.SetValid(abytes);
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::Float32:
		{
			ipoolf32.EnsureSize(abytes);
			Float32* floatpool = (Float32*)((char*)ipoolf32.Ptr() + lastcur);
			memcpy(floatpool, data, bytes);
			ipoolf32.SetValid(abytes);
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
		case BitsPerSample::Float64:
		{
			ipoolf64.EnsureSize(abytes);
			Float64* doublepool = (Float64*)((char*)ipoolf64.Ptr() + lastcur);
			memcpy(doublepool, data, bytes);
			ipoolf64.SetValid(abytes);
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolout.Clear();
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
		}
		break;
	}
		samplecount = newsamplecount;
		duration = (double)samplecount / (double)sr;
		interleavedchunk = true;
		result.Value = true;
	}
	catch (...)
	{
		samplecount = 0;
		duration = 0;
		ipool8.Clear();
		ipool16.Clear();
		ipool24.Clear();
		ipool32.Clear();
		ipoolf32.Clear();
		ipoolf64.Clear();
		ipoolout.Clear();
		spool8.Clear();
		spool16.Clear();
		spool24.Clear();
		spool32.Clear();
		spoolf32.Clear();
		spoolf64.Clear();
		spoolout.Clear();
		interleavedchunk.Reset();
		result.WriteMessage(MEMORY_ALLOCATE_ERROR);
	}
	return result;
}

Result<bool> AudioBuffer::AddSplittedData(void** data, UInt samples)
{
	Result<bool> result(false);
	if (!internal)
	{
		result.WriteMessage(L"no reset record");
		return result;
	}
	if (interleavedchunk)
	{
		if (*interleavedchunk)
		{
			result.WriteMessage(L"invalid due to interleaved mode");
			return result;
		}
	}

	size_t lastsamplecount = samplecount;
	size_t lastcur = lastsamplecount;
	size_t newsamplecount = lastsamplecount + samples;
	try
	{
		UInt n = 0;
		UInt count = samples;
		UInt bytes = count * bpsnum >> 3;
		size_t abytes = newsamplecount* bpsnum >> 3;
		switch (bps)
		{
		case BitsPerSample::SInt8:
		{
			spool8.EnsureSize(abytes, ch);
			CopySplittedChunk(&spool8, (Int8**)data, bytes, ch, lastcur);
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt8:
		{
			spool8.EnsureSize(abytes, ch);
			CopySplittedChunk2(&spool8, (UInt8**)data, bytes, ch, lastcur);
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::SInt16:
		{
			spool16.EnsureSize(abytes, ch);
			CopySplittedChunk(&spool16, (Int16**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt16:
		{
			spool16.EnsureSize(abytes, ch);
			CopySplittedChunk2(&spool16, (UInt16**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::SInt24:
		{
			spool24.EnsureSize(abytes, ch);
			CopySplittedChunk(&spool24, (Int24**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool16.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt24:
		{
			spool24.EnsureSize(abytes, ch);
			CopySplittedChunk2(&spool24, (UInt24**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool16.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::SInt32:
		{
			spool32.EnsureSize(abytes, ch);
			CopySplittedChunk(&spool32, (Int32**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::UInt32:
		{
			spool32.EnsureSize(abytes, ch);
			CopySplittedChunk2(&spool32, (UInt32**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spoolf32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::Float32:
		{
			spoolf32.EnsureSize(abytes, ch);
			CopySplittedChunk(&spoolf32, (Float32**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf64.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		case BitsPerSample::Float64:
		{
			spoolf64.EnsureSize(abytes, ch);
			CopySplittedChunk(&spoolf64, (Float64**)data, bytes, ch, lastcur);
			spool8.Clear();
			spool16.Clear();
			spool24.Clear();
			spool32.Clear();
			spoolf32.Clear();
			spoolout.Clear();
			ipool8.Clear();
			ipool16.Clear();
			ipool24.Clear();
			ipool32.Clear();
			ipoolf32.Clear();
			ipoolf64.Clear();
			ipoolout.Clear();
		}
		break;
		}
		samplecount = newsamplecount;
		duration = (double)samplecount / (double)sr;
		interleavedchunk = false;
		result.Value = true;
	}
	catch (...)
	{
		samplecount = 0;
		duration = 0;
		ipool8.Clear();
		ipool16.Clear();
		ipool24.Clear();
		ipool32.Clear();
		ipoolf32.Clear();
		ipoolf64.Clear();
		ipoolout.Clear();
		spool8.Clear();
		spool16.Clear();
		spool24.Clear();
		spool32.Clear();
		spoolf32.Clear();
		spoolf64.Clear();
		spoolout.Clear();
		interleavedchunk.Reset();
		result.WriteMessage(MEMORY_ALLOCATE_ERROR);
	}
	return result;
}

Result<bool> AudioBuffer::CloseResetData(bool rollBackBeforeReset)
{
	Result<bool> result(true);
	if (internal)
	{
		AudioBufferHistory* history = static_cast<AudioBufferHistory*>(internal);
		if (history)
		{
			if (rollBackBeforeReset)
			{
				ipool8.Reset();
				ipool16.Reset();
				ipool24.Reset();
				ipool32.Reset();
				ipoolf32.Reset();
				ipoolf64.Reset();
				ipoolout.Reset();
				spool8.Reset();
				spool16.Reset();
				spool24.Reset();
				spool32.Reset();
				spoolf32.Reset();
				spoolf64.Reset();
				spoolout.Reset();
				interleavedchunk.Reset();
				
				ipool8 = std::move(history->ipool8);
				ipool16 = std::move(history->ipool16);
				ipool24 = std::move(history->ipool24);
				ipool32 = std::move(history->ipool32);
				ipoolf32 = std::move(history->ipoolf32);
				ipoolf64 = std::move(history->ipoolf64);
				ipoolout = std::move(history->ipoolout);
				spool8 = std::move(history->spool8);
				spool16 = std::move(history->spool16);
				spool24 = std::move(history->spool24);
				spool32 = std::move(history->spool32);
				spoolf32 = std::move(history->spoolf32);
				spoolf64 = std::move(history->spoolf64);
				spoolout = std::move(history->spoolout);
				interleavedchunk = history->interleaved;

				samplecount = history->samplecount;
				duration = history->duration;
				ch = history->old_ch;
				chlayout = history->old_chcfg;
				sr = history->old_sr;
				bps = history->old_bps;
				bpsnum = history->old_bpsnum;
				startms = history->StartMs;
				delete history;
				history = nullptr;
				internal = nullptr;
			}
			else
			{
				delete history;
				history = nullptr;
				internal = nullptr;
			}
		}
		else
		{
			result.WriteMessage(L"invalid record");
			result.Value = false;
		}
	}
	else
	{
		result.WriteMessage(L"no reset record");
		result.Value = false;
	}
	return result;
}

void AudioBuffer::Clear()
{
	ipool8.Clear();
	ipool16.Clear();
	ipool24.Clear();
	ipool32.Clear();
	ipoolf32.Clear();
	ipoolf64.Clear();
	ipoolout.Clear();
	spool8.Clear();
	spool16.Clear();
	spool24.Clear();
	spool32.Clear();
	spoolf32.Clear();
	spoolf64.Clear();
	spoolout.Clear();
	interleavedchunk.Reset();
}

__inline UInt AudioBuffer::GetSampleRate()const
{
	return sr; 
}

__inline UShort AudioBuffer::GetChannels()const
{
	return ch; 
}

__inline void AudioBuffer::SetChannelLayout(Audio::ChannelLayout channelConfig)
{
	if(channelConfig.IsValid) chlayout=channelConfig;
}

__inline Audio::ChannelLayout AudioBuffer::GetChannelLayout()const
{
	return chlayout; 
}

__inline UShort AudioBuffer::GetBitsPerSampleNum()const
{
	return bpsnum;
}

__inline BitsPerSample AudioBuffer::GetBitsPerSample()const
{
	return bps;
}

__inline UInt AudioBuffer::GetSampleNum()const
{
	return samplecount;
}

__inline UInt64 AudioBuffer::GetPool8Bytes()const
{
	return ipool8.Valid();
}

__inline UInt64 AudioBuffer::GetPool16Bytes()const
{
	return ipool16.Valid();
}

__inline UInt64 AudioBuffer::GetPool24Bytes()const
{
	return ipool24.Valid();
}

__inline UInt64 AudioBuffer::GetPool32Bytes()const
{
	return ipool32.Valid();
}

__inline UInt64 AudioBuffer::GetFloatPoolBytes()const
{
	return ipoolf32.Valid();
}

__inline UInt64 AudioBuffer::GetDoublePoolBytes()const
{
	return ipoolf64.Valid();
}

__inline const Int8* AudioBuffer::GetPool8()const
{
	return ipool8.Ptr();
}

__inline const Int16* AudioBuffer::GetPool16()const
{
	return ipool16.Ptr();
}

__inline const Int24* AudioBuffer::GetPool24()const
{
	return ipool24.Ptr();
}

__inline const Int32* AudioBuffer::GetPool32()const
{
	return ipool32.Ptr();
}

__inline const float* AudioBuffer::GetFloatPool()const
{
	return ipoolf32.Ptr();
}

__inline const double* AudioBuffer::GetDoublePool()const
{
	return ipoolf64.Ptr();
}

__inline const Int8** AudioBuffer::GetSplittedPool8()const
{
	return spool8.Ptr();
}

__inline const Int16** AudioBuffer::GetSplittedPool16()const
{
	return spool16.Ptr();
}

__inline const Int24** AudioBuffer::GetSplittedPool24()const
{
	return spool24.Ptr();
}

__inline const Int32** AudioBuffer::GetSplittedPool32()const
{
	return spool32.Ptr();
}

__inline const float** AudioBuffer::GetSplittedFloatPool()const
{
	return spoolf32.Ptr();
}

__inline const double** AudioBuffer::GetSplittedDoublePool()const
{
	return spoolf64.Ptr();
}

_inline double AudioBuffer::GetDuration()const
{
	if(samplecount>0) return duration; else return 0.0;
}

Result<void*> AudioBuffer::GetPool(UInt64& bytes,  Audio::SampleInfo& FinalInfo)
{
	Result<void*> result(nullptr);
	bytes = 0u;

	FinalInfo.SampleRate = sr;
	FinalInfo.ChannelLayout = chlayout;
	FinalInfo.BitsPerSample = bps;

	switch (FinalInfo.BitsPerSample.BPS)
	{
		case 8u:
		{
			auto res8 = EnsurePool8();
			if (res8.Value)
			{
				bytes = Pool8Bytes;
				result.Value = (void*)Pool8;
			}
		}
		break;
		case 16u:
		{
			auto res16 = EnsurePool16();
			if (res16.Value)
			{
				bytes = Pool16Bytes;
				result.Value = (void*)Pool16;
			}
		}
		break;
		case 24u:
		{
			auto res24 = EnsurePool24();
			if (res24.Value)
			{
				bytes = Pool24Bytes;
				result.Value = (void*)Pool24;
			}
		}
		break;
		case 32u:
		{
			if (!BitsPerSample.IsFloating)
			{
				auto res32 = EnsurePool32();
				if (res32.Value)
				{
					bytes = Pool32Bytes;
					result.Value = (void*)Pool32;
				}
			}
			else
			{
				auto resf = EnsureFloatPool();
				if (resf.Value)
				{
					bytes = FloatPoolBytes;
					result.Value = (void*)FloatPool;
				}
			}
		}
		break;
		case 64u:
		{
			if (BitsPerSample.IsFloating)
			{
				auto resd = EnsureDoublePool();
				if (resd.Value)
				{
					bytes = DoublePoolBytes;
					result.Value = (void*)DoublePool;
				}
			}
		}
		break;
	}
	return result;
}

Result<void**> AudioBuffer::GetSplittedPool(UInt64& bytes, Audio::SampleInfo& FinalInfo)
{
	Result<void**> result(nullptr);
	bytes = 0u;

	FinalInfo.SampleRate = sr;
	FinalInfo.ChannelLayout = chlayout;
	FinalInfo.BitsPerSample = bps;

	switch (FinalInfo.BitsPerSample.BPS)
	{
	case 8u:
	{
		auto res8 = EnsurePool8(false);
		if (res8.Value)
		{
			bytes = Pool8Bytes;
			result.Value = (void**)SplittedPool8;
		}
	}
	break;
	case 16u:
	{
		auto res16 = EnsurePool16(false);
		if (res16.Value)
		{
			bytes = Pool16Bytes;
			result.Value = (void**)SplittedPool16;
		}
	}
	break;
	case 24u:
	{
		auto res24 = EnsurePool24(false);
		if (res24.Value)
		{
			bytes = Pool24Bytes;
			result.Value = (void**)SplittedPool24;
		}
	}
	break;
	case 32u:
	{
		if (!BitsPerSample.IsFloating)
		{
			auto res32 = EnsurePool32(false);
			if (res32.Value)
			{
				bytes = Pool32Bytes;
				result.Value = (void**)SplittedPool32;
			}
		}
		else
		{
			auto resf = EnsureFloatPool(false);
			if (resf.Value)
			{
				bytes = FloatPoolBytes;
				result.Value = (void**)SplittedFloatPool;
			}
		}
	}
	break;
	case 64u:
	{
		if (BitsPerSample.IsFloating)
		{
			auto resd = EnsureDoublePool(false);
			if (resd.Value)
			{
				bytes = DoublePoolBytes;
				result.Value = (void**)SplittedDoublePool;
			}
		}
	}
	break;
	}
	return result;
}

double AudioBuffer::GetStartTimeMs() const
{
	return startms;
}

void AudioBuffer::SetStartTimeMs(double startMs)
{
	startms = startMs;
}

Result<AudioChunk*> AudioBuffer::BeginChunk(UShort bps, bool isfloating, bool enableRollBack)
{
	Result<AudioChunk*> result(nullptr);

	if (internal)
	{
		result.WriteMessage(L"invalid op");
		return result;
	}

	if (!interleavedchunk || samplecount==0)
	{
		result.WriteMessage(L"no samples");
		return result;
	}
	
	AudioBufferHistory* history = new AudioBufferHistory();
	history->chunkrollback = enableRollBack;
	if (enableRollBack)
	{
		history->ipool8 = ipool8;
		history->ipool16 = ipool16;
		history->ipool24 = ipool24;
		history->ipool32 = ipool32;
		history->ipoolf32 = ipoolf32;
		history->ipoolf64 = ipoolf64;
		history->spool8 = spool8;
		history->spool16 = spool16;
		history->spool24 = spool24;
		history->spool32 = spool32;
		history->spoolf32 = spoolf32;
		history->spoolf64 = spoolf64;
		history->old_bps = bps;
		history->old_bpsnum = bpsnum;
		history->old_ch = ch;
		history->old_chcfg = chlayout;
		history->old_sr = sr;
		history->samplecount = samplecount;
	}
	internal = (void*)history;

	System::Audio::BitsPerSample chunkbps = System::Audio::BitsPerSample::Float32;

	switch (bps)
	{
	case 8:
		chunkbps = System::Audio::BitsPerSample::SInt8;
		break;
	case 16:
		chunkbps = System::Audio::BitsPerSample::SInt16;
		break;
	case 24:
		chunkbps = System::Audio::BitsPerSample::SInt24;
		break;
	case 32:
		chunkbps = isfloating? System::Audio::BitsPerSample::Float32 : System::Audio::BitsPerSample::SInt32;
		break;
	case 64:
		chunkbps = System::Audio::BitsPerSample::Float64;
		break;
	}

	result.Value = AudioChunk_impl::Create(this, chunkbps);
	result.Value->impl->interleaved = *interleavedchunk;

	history->chunk = result.Value;

	return result;
}

Result<bool> AudioBuffer::EndChunk(AudioChunk*& chunk, bool rollBackBeforeReset)
{
	Result<bool> result(false);
	if (!internal)
	{
		result.WriteMessage(L"no chunk is requested");
		return result;
	}
	if (!chunk)
	{
		result.WriteMessage(L"no chunk");
		return result;
	}

	if (internal)
	{
		auto history = (AudioBufferHistory*)internal;

		if (chunk != history->chunk)
		{
			result.WriteMessage(L"chunk is invalid");
			return result;
		}

		if (!history->chunkrollback && rollBackBeforeReset)
		{
			result.WriteMessage(L"rollback is not enabled");
			return result;
		}
		else if (history->chunkrollback && rollBackBeforeReset)
		{
			ipool8.Reset();
			ipool16.Reset();
			ipool24.Reset();
			ipool32.Reset();
			ipoolf32.Reset();
			ipoolf64.Reset();
			ipoolout.Reset();
			spool8.Reset();
			spool16.Reset();
			spool24.Reset();
			spool32.Reset();
			spoolf32.Reset();
			spoolf64.Reset();
			spoolout.Reset();

			ipool8 = std::move(history->ipool8);
			ipool16 = std::move(history->ipool16);
			ipool24 = std::move(history->ipool24);
			ipool32 = std::move(history->ipool32);
			ipoolf32 = std::move(history->ipoolf32);
			ipoolf64 = std::move(history->ipoolf64);
			ipoolout = std::move(history->ipoolout);
			spool8 = std::move(history->spool8);
			spool16 = std::move(history->spool16);
			spool24 = std::move(history->spool24);
			spool32 = std::move(history->spool32);
			spoolf32 = std::move(history->spoolf32);
			spoolf64 = std::move(history->spoolf64);
			spoolout = std::move(history->spoolout);

			samplecount = history->samplecount;
			duration = (double)samplecount / (double)sr;
		}
		else if (!rollBackBeforeReset)
		{
			/*apply chunk*/
			//bool samebps = false;
			//UShort chunkbpsnum = chunk->impl->bps.BPS;
			//if (chunkbpsnum == bpsnum)
			//{
			//	if (bpsnum == 32)
			//	{
			//		samebps = (bps.IsFloating == chunk->impl->bps.IsFloating);
			//	}
			//	else
			//	{
			//		samebps = true;
			//	}
			//}
			//if (!interleavedchunk) interleavedchunk = chunk->impl->interleaved;
			//if (samebps)
			//{
			//	if (*interleavedchunk != chunk->impl->interleaved)
			//	{
			//		if (chunk->impl->interleaved)
			//		{
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				spool8.EnsureSize(sizeof(Int8)*samplecount, ch);
			//				Int8_To_SplittedInt8((void**)spool8.Ptr(), ipool8.Ptr(), samplecount, ch);
			//				spool8.SetValid(sizeof(Int8)*samplecount, ch);
			//				break;
			//			case 16:
			//				spool16.EnsureSize(sizeof(Int16)*samplecount, ch);
			//				Int16_To_SplittedInt16((void**)spool16.Ptr(), ipool16.Ptr(), samplecount, ch);
			//				spool16.SetValid(sizeof(Int16)*samplecount, ch);
			//				break;
			//			case 24:
			//				spool24.EnsureSize(sizeof(Int24)*samplecount, ch);
			//				Int24_To_SplittedInt24((void**)spool24.Ptr(), ipool24.Ptr(), samplecount, ch);
			//				spool24.SetValid(sizeof(Int24)*samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//				{
			//					spoolf32.EnsureSize(sizeof(float)*samplecount, ch);
			//					Float32_To_SplittedFloat32((void**)spoolf32.Ptr(), ipoolf32.Ptr(), samplecount, ch);
			//					spoolf32.SetValid(sizeof(float)*samplecount, ch);
			//				}
			//				else
			//				{
			//					spool32.EnsureSize(sizeof(Int32)*samplecount, ch);
			//					Int32_To_SplittedInt32((void**)spool32.Ptr(), ipool32.Ptr(), samplecount, ch);
			//					spool32.SetValid(sizeof(Int32)*samplecount, ch);
			//				}
			//				break;
			//			case 64:
			//				spoolf64.EnsureSize(sizeof(double)*samplecount, ch);
			//				Float64_To_SplittedFloat64((void**)spoolf64.Ptr(), ipoolf64.Ptr(), samplecount, ch);
			//				spoolf64.SetValid(sizeof(double)*samplecount, ch);
			//				break;
			//			}
			//		}
			//		else
			//		{
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				ipool8.EnsureSize(sizeof(Int8)*samplecount * ch);
			//				SplittedInt8_To_Int8(ipool8.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//				ipool8.SetValid(sizeof(Int8)*samplecount * ch);
			//				break;
			//			case 16:
			//				ipool16.EnsureSize(sizeof(Int16)*samplecount * ch);
			//				SplittedInt16_To_Int16(ipool16.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//				ipool16.SetValid(sizeof(Int16)*samplecount * ch);
			//				break;
			//			case 24:
			//				ipool24.EnsureSize(sizeof(Int24)*samplecount * ch);
			//				SplittedInt24_To_Int24(ipool24.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//				ipool24.SetValid(sizeof(Int24)*samplecount * ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//				{
			//					ipoolf32.EnsureSize(sizeof(float)*samplecount * ch);
			//					SplittedFloat32_To_Float32(ipoolf32.Ptr(), (const void**)spoolf32.Ptr(), samplecount, ch);
			//					ipoolf32.SetValid(sizeof(float)*samplecount * ch);
			//				}
			//				else
			//				{
			//					ipool32.EnsureSize(sizeof(Int32)*samplecount * ch);
			//					SplittedInt32_To_Int32(ipool32.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//					ipool32.SetValid(sizeof(Int32)*samplecount * ch);
			//				}
			//				break;
			//			case 64:
			//				ipoolf64.EnsureSize(sizeof(double)*samplecount * ch);
			//				SplittedFloat64_To_Float64(ipoolf64.Ptr(), (const void**)spoolf64.Ptr(), samplecount, ch);
			//				ipoolf64.SetValid(sizeof(double)*samplecount * ch);
			//				break;
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//	if (*interleavedchunk == chunk->impl->interleaved)
			//	{
			//		if (chunk->impl->interleaved)
			//		{
			//			if (chunkbpsnum == 8)
			//			{
			//				ipool8.EnsureSize(sizeof(Int8)*samplecount * ch);
			//				ipool8.SetValid(sizeof(Int8)*samplecount * ch);
			//				switch (bpsnum)
			//				{
			//				case 16:						
			//					Int16_To_Int8(ipool8.Ptr(), ipool16.Ptr(), samplecount);
			//					break;
			//				case 24:
			//					Int24_To_Int8_Dither(ipool8.Ptr(), ipool24.Ptr(), samplecount, dither);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						Float32_To_Int8_Dither(ipool8.Ptr(), ipoolf32.Ptr(), samplecount, dither);
			//					else
			//						Int32_To_Int8_Dither(ipool8.Ptr(), ipool32.Ptr(), samplecount, dither);
			//					break;
			//				case 64:
			//					Float64_To_Int8_Dither(ipool8.Ptr(), ipoolf64.Ptr(), samplecount, dither);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 16)
			//			{
			//				ipool16.EnsureSize(sizeof(Int16)*samplecount * ch);
			//				ipool16.SetValid(sizeof(Int16)*samplecount * ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					Int8_To_Int16(ipool16.Ptr(), ipool8.Ptr(), samplecount);
			//					break;
			//				case 24:
			//					Int24_To_Int16_Dither(ipool16.Ptr(), ipool24.Ptr(), samplecount, dither);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						Float32_To_Int16_Dither(ipool16.Ptr(), ipoolf32.Ptr(), samplecount, dither);
			//					else
			//						Int32_To_Int16_Dither(ipool16.Ptr(), ipool32.Ptr(), samplecount, dither);
			//					break;
			//				case 64:
			//					Float64_To_Int16_Dither(ipool16.Ptr(), ipoolf64.Ptr(), samplecount, dither);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 24)
			//			{
			//				ipool24.EnsureSize(sizeof(Int24)*samplecount * ch);
			//				ipool24.SetValid(sizeof(Int24)*samplecount * ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					Int8_To_Int24(ipool24.Ptr(), ipool8.Ptr(), samplecount);
			//					break;
			//				case 16:
			//					Int16_To_Int24(ipool24.Ptr(), ipool16.Ptr(), samplecount);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						Float32_To_Int24_Dither(ipool24.Ptr(), ipoolf32.Ptr(), samplecount, dither);
			//					else
			//						Int32_To_Int24(ipool24.Ptr(), ipool32.Ptr(), samplecount);
			//					break;
			//				case 64:
			//					Float64_To_Int24_Dither(ipool24.Ptr(), ipoolf64.Ptr(), samplecount, dither);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 32 && !bps.IsFloating)
			//			{
			//				ipool32.EnsureSize(sizeof(Int32)*samplecount * ch);
			//				ipool32.SetValid(sizeof(Int32)*samplecount * ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					Int8_To_Int32(ipool32.Ptr(), ipool8.Ptr(), samplecount);
			//					break;
			//				case 16:
			//					Int16_To_Int32(ipool32.Ptr(), ipool16.Ptr(), samplecount);
			//					break;
			//				case 24:
			//					Int24_To_Int32(ipool32.Ptr(), ipool24.Ptr(), samplecount);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						Float32_To_Int32_Dither(ipool32.Ptr(), ipoolf32.Ptr(), samplecount, dither);
			//					break;
			//				case 64:
			//					Float64_To_Int32_Dither(ipool32.Ptr(), ipoolf64.Ptr(), samplecount, dither);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 32 && bps.IsFloating)
			//			{
			//				ipoolf32.EnsureSize(sizeof(float)*samplecount * ch);
			//				ipoolf32.SetValid(sizeof(float)*samplecount * ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					Int8_To_Float32(ipoolf32.Ptr(), ipool8.Ptr(), samplecount);
			//					break;
			//				case 16:
			//					Int16_To_Float32(ipoolf32.Ptr(), ipool16.Ptr(), samplecount);
			//					break;
			//				case 24:
			//					Int24_To_Float32(ipoolf32.Ptr(), ipool24.Ptr(), samplecount);
			//					break;
			//				case 32:
			//					if (!chunk->impl->bps.IsFloating)
			//						Int32_To_Float32(ipoolf32.Ptr(), ipool32.Ptr(), samplecount);
			//					break;
			//				case 64:
			//					Float64_To_Float32(ipoolf32.Ptr(), ipoolf64.Ptr(), samplecount);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 64)
			//			{
			//				ipoolf64.EnsureSize(sizeof(double)*samplecount * ch);
			//				ipoolf64.SetValid(sizeof(double)*samplecount * ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					Int8_To_Float64(ipoolf64.Ptr(), ipool8.Ptr(), samplecount);
			//					break;
			//				case 16:
			//					Int16_To_Float64(ipoolf64.Ptr(), ipool16.Ptr(), samplecount);
			//					break;
			//				case 24:
			//					Int24_To_Float64(ipoolf64.Ptr(), ipool24.Ptr(), samplecount);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						Float32_To_Float64(ipoolf64.Ptr(), ipoolf32.Ptr(), samplecount);
			//					else
			//						Int32_To_Float64(ipoolf64.Ptr(), ipool32.Ptr(), samplecount);
			//					break;
			//				}
			//			}
			//		}
			//		else
			//		{
			//			if (chunkbpsnum == 8)
			//			{
			//				spool8.EnsureSize(sizeof(Int8)*samplecount, ch);
			//				spool8.SetValid(sizeof(Int8)*samplecount, ch);
			//				switch (bpsnum)
			//				{
			//				case 16:
			//					SplittedInt16_To_SplittedInt8((void**)spool8.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//					break;
			//				case 24:
			//					SplittedInt24_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spool24.Ptr(), samplecount, dither, ch);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						SplittedFloat32_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//					else
			//						SplittedInt32_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spool32.Ptr(), samplecount, dither, ch);
			//					break;
			//				case 64:
			//					SplittedFloat64_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 16)
			//			{
			//				spool16.EnsureSize(sizeof(Int16)*samplecount, ch);
			//				spool16.SetValid(sizeof(Int16)*samplecount, ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					SplittedInt8_To_SplittedInt16((void**)spool16.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//					break;
			//				case 24:
			//					SplittedInt24_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spool24.Ptr(), samplecount, dither, ch);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						SplittedFloat32_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//					else
			//						SplittedInt32_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spool32.Ptr(), samplecount, dither, ch);
			//					break;
			//				case 64:
			//					SplittedFloat64_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 24)
			//			{
			//				spool24.EnsureSize(sizeof(Int24)*samplecount, ch);
			//				spool24.SetValid(sizeof(Int24)*samplecount, ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					SplittedInt8_To_SplittedInt24((void**)spool24.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//					break;
			//				case 16:
			//					SplittedInt16_To_SplittedInt24((void**)spool24.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						SplittedFloat32_To_SplittedInt24_Dither((void**)spool24.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//					else
			//						SplittedInt32_To_SplittedInt24((void**)spool24.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//					break;
			//				case 64:
			//					SplittedFloat64_To_SplittedInt24_Dither((void**)spool24.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 32 && !bps.IsFloating)
			//			{
			//				spool32.EnsureSize(sizeof(Int32)*samplecount, ch);
			//				spool32.SetValid(sizeof(Int32)*samplecount, ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					SplittedInt8_To_SplittedInt32((void**)spool32.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//					break;
			//				case 16:
			//					SplittedInt16_To_SplittedInt32((void**)spool32.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//					break;
			//				case 24:
			//					SplittedInt24_To_SplittedInt32((void**)spool32.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						SplittedFloat32_To_SplittedInt32_Dither((void**)spool32.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//					break;
			//				case 64:
			//					SplittedFloat64_To_SplittedInt32_Dither((void**)spool32.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 32 && bps.IsFloating)
			//			{
			//				spoolf32.EnsureSize(sizeof(float)*samplecount, ch);
			//				spoolf32.SetValid(sizeof(float)*samplecount, ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					SplittedInt8_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//					break;
			//				case 16:
			//					SplittedInt16_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//					break;
			//				case 24:
			//					SplittedInt24_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//					break;
			//				case 32:
			//					if (!chunk->impl->bps.IsFloating)
			//						SplittedInt32_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//					break;
			//				case 64:
			//					SplittedFloat64_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void**)spoolf64.Ptr(), samplecount, ch);
			//					break;
			//				}
			//			}
			//			else if (chunkbpsnum == 64)
			//			{
			//				spoolf64.EnsureSize(sizeof(double)*samplecount, ch);
			//				spoolf64.SetValid(sizeof(double)*samplecount, ch);
			//				switch (bpsnum)
			//				{
			//				case 8:
			//					SplittedInt8_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//					break;
			//				case 16:
			//					SplittedInt16_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//					break;
			//				case 24:
			//					SplittedInt24_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//					break;
			//				case 32:
			//					if (chunk->impl->bps.IsFloating)
			//						SplittedFloat32_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spoolf32.Ptr(), samplecount, ch);
			//					else
			//						SplittedInt32_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//					break;
			//				}
			//			}
			//		}
			//	}
			//	else if (*interleavedchunk)
			//	{
			//		if (chunkbpsnum == 8)
			//		{
			//			ipool8.EnsureSize(sizeof(Int8)*samplecount * ch);
			//			ipool8.SetValid(sizeof(Int8)*samplecount * ch);
			//			switch (bpsnum)
			//			{
			//			case 16:
			//				SplittedInt16_To_Int8(ipool8.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				SplittedInt24_To_Int8_Dither(ipool8.Ptr(), (const void**)spool24.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					SplittedFloat32_To_Int8_Dither(ipool8.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//				else
			//					SplittedInt32_To_Int8_Dither(ipool8.Ptr(), (const void**)spool32.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 64:
			//				SplittedFloat64_To_Int8_Dither(ipool8.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 16)
			//		{
			//			ipool16.EnsureSize(sizeof(Int16)*samplecount * ch);
			//			ipool16.SetValid(sizeof(Int16)*samplecount * ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				SplittedInt8_To_Int16(ipool16.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				SplittedInt24_To_Int16_Dither(ipool16.Ptr(), (const void**)spool24.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					SplittedFloat32_To_Int16_Dither(ipool16.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//				else
			//					SplittedInt32_To_Int16_Dither(ipool16.Ptr(), (const void**)spool32.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 64:
			//				SplittedFloat64_To_Int16_Dither(ipool16.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 24)
			//		{
			//			ipool24.EnsureSize(sizeof(Int24)*samplecount * ch);
			//			ipool24.SetValid(sizeof(Int24)*samplecount * ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				SplittedInt8_To_Int24(ipool24.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				SplittedInt16_To_Int24(ipool24.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					SplittedFloat32_To_Int24_Dither(ipool24.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//				else
			//					SplittedInt32_To_Int24(ipool24.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//				break;
			//			case 64:
			//				SplittedFloat64_To_Int24_Dither(ipool24.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 32 && !bps.IsFloating)
			//		{
			//			ipool32.EnsureSize(sizeof(Int32)*samplecount * ch);
			//			ipool32.SetValid(sizeof(Int32)*samplecount * ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				SplittedInt8_To_Int32(ipool32.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				SplittedInt16_To_Int32(ipool32.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				SplittedInt24_To_Int32(ipool32.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					SplittedFloat32_To_Int32_Dither(ipool32.Ptr(), (const void**)spoolf32.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 64:
			//				SplittedFloat64_To_Int32_Dither(ipool32.Ptr(), (const void**)spoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 32 && bps.IsFloating)
			//		{
			//			ipoolf32.EnsureSize(sizeof(float)*samplecount * ch);
			//			ipoolf32.SetValid(sizeof(float)*samplecount * ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				SplittedInt8_To_Float32(ipoolf32.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				SplittedInt16_To_Float32(ipoolf32.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				SplittedInt24_To_Float32(ipoolf32.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (!chunk->impl->bps.IsFloating)
			//					SplittedInt32_To_Float32(ipoolf32.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//				break;
			//			case 64:
			//				SplittedFloat64_To_Float32(ipoolf32.Ptr(), (const void**)spoolf64.Ptr(), samplecount, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 64)
			//		{
			//			ipoolf64.EnsureSize(sizeof(double)*samplecount * ch);
			//			ipoolf64.SetValid(sizeof(double)*samplecount * ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				SplittedInt8_To_Float64(ipoolf64.Ptr(), (const void**)spool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				SplittedInt16_To_Float64(ipoolf64.Ptr(), (const void**)spool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				SplittedInt24_To_Float64(ipoolf64.Ptr(), (const void**)spool24.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					SplittedFloat32_To_Float64(ipoolf64.Ptr(), (const void**)spoolf32.Ptr(), samplecount, ch);
			//				else
			//					SplittedInt32_To_Float64(ipoolf64.Ptr(), (const void**)spool32.Ptr(), samplecount, ch);
			//				break;
			//			}
			//		}
			//	}
			//	else
			//	{
			//		if (chunkbpsnum == 8)
			//		{
			//			spool8.EnsureSize(sizeof(Int8)*samplecount, ch);
			//			spool8.SetValid(sizeof(Int8)*samplecount, ch);
			//			switch (bpsnum)
			//			{
			//			case 16:
			//				Int16_To_SplittedInt8((void**)spool8.Ptr(), (const void*)ipool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				Int24_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void*)ipool24.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					Float32_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void*)ipoolf32.Ptr(), samplecount, dither, ch);
			//				else
			//					Int32_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void*)ipool32.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 64:
			//				Float64_To_SplittedInt8_Dither((void**)spool8.Ptr(), (const void*)ipoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 16)
			//		{
			//			spool16.EnsureSize(sizeof(Int16)*samplecount, ch);
			//			spool16.SetValid(sizeof(Int16)*samplecount, ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				Int8_To_SplittedInt16((void**)spool16.Ptr(), (const void*)ipool8.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				Int24_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void*)ipool24.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					Float32_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void*)ipoolf32.Ptr(), samplecount, dither, ch);
			//				else
			//					Int32_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void*)ipool32.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 64:
			//				Float64_To_SplittedInt16_Dither((void**)spool16.Ptr(), (const void*)ipoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 24)
			//		{
			//			spool24.EnsureSize(sizeof(Int24)*samplecount, ch);
			//			spool24.SetValid(sizeof(Int24)*samplecount, ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				Int8_To_SplittedInt24((void**)spool24.Ptr(), (const void*)ipool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				Int16_To_SplittedInt24((void**)spool24.Ptr(), (const void*)ipool16.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					Float32_To_SplittedInt24_Dither((void**)spool24.Ptr(), (const void*)ipoolf32.Ptr(), samplecount, dither, ch);
			//				else
			//					Int32_To_SplittedInt24((void**)spool24.Ptr(), (const void*)ipool32.Ptr(), samplecount, ch);
			//				break;
			//			case 64:
			//				Float64_To_SplittedInt24_Dither((void**)spool24.Ptr(), (const void*)ipoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 32 && !bps.IsFloating)
			//		{
			//			spool32.EnsureSize(sizeof(Int32)*samplecount, ch);
			//			spool32.SetValid(sizeof(Int32)*samplecount, ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				Int8_To_SplittedInt32((void**)spool32.Ptr(), (const void*)ipool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				Int16_To_SplittedInt32((void**)spool32.Ptr(), (const void*)ipool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				Int24_To_SplittedInt32((void**)spool32.Ptr(), (const void*)ipool24.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					Float32_To_SplittedInt32_Dither((void**)spool32.Ptr(), (const void*)ipoolf32.Ptr(), samplecount, dither, ch);
			//				break;
			//			case 64:
			//				Float64_To_SplittedInt32_Dither((void**)spool32.Ptr(), (const void*)ipoolf64.Ptr(), samplecount, dither, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 32 && bps.IsFloating)
			//		{
			//			spoolf32.EnsureSize(sizeof(float)*samplecount, ch);
			//			spoolf32.SetValid(sizeof(float)*samplecount, ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				Int8_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void*)ipool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				Int16_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void*)ipool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				Int24_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void*)ipool24.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (!chunk->impl->bps.IsFloating)
			//					Int32_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void*)ipool32.Ptr(), samplecount, ch);
			//				break;
			//			case 64:
			//				Float64_To_SplittedFloat32((void**)spoolf32.Ptr(), (const void*)ipoolf64.Ptr(), samplecount, ch);
			//				break;
			//			}
			//		}
			//		else if (chunkbpsnum == 64)
			//		{
			//			spoolf64.EnsureSize(sizeof(double)*samplecount, ch);
			//			spoolf64.SetValid(sizeof(double)*samplecount, ch);
			//			switch (bpsnum)
			//			{
			//			case 8:
			//				Int8_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void*)ipool8.Ptr(), samplecount, ch);
			//				break;
			//			case 16:
			//				Int16_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void*)ipool16.Ptr(), samplecount, ch);
			//				break;
			//			case 24:
			//				Int24_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void*)ipool24.Ptr(), samplecount, ch);
			//				break;
			//			case 32:
			//				if (chunk->impl->bps.IsFloating)
			//					Float32_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void*)ipoolf32.Ptr(), samplecount, ch);
			//				else
			//					Int32_To_SplittedFloat64((void**)spoolf64.Ptr(), (const void*)ipool32.Ptr(), samplecount, ch);
			//				break;
			//			}
			//		}
			//	}
			//}

			if (*interleavedchunk)
			{
				switch (chunk->impl->bps.BPS)
				{
				case 8:
					ipool16.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool16.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				case 16:
					ipool8.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool16.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				case 24:
					ipool8.SetValid(0);
					ipool16.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool16.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				case 32:
					if (chunk->impl->bps.IsFloating)
					{
						ipool8.SetValid(0);
						ipool16.SetValid(0);
						ipool24.SetValid(0);
						ipool32.SetValid(0);
						ipoolf64.SetValid(0);
						ipoolout.SetValid(0);
						spool8.SetValid(0, ch);
						spool16.SetValid(0, ch);
						spool24.SetValid(0, ch);
						spool32.SetValid(0, ch);
						spoolf32.SetValid(0, ch);
						spoolf64.SetValid(0, ch);
						spoolout.SetValid(0, ch);
					}
					else
					{
						ipool8.SetValid(0);
						ipool16.SetValid(0);
						ipool24.SetValid(0);
						ipoolf32.SetValid(0);
						ipoolf64.SetValid(0);
						ipoolout.SetValid(0);
						spool8.SetValid(0, ch);
						spool16.SetValid(0, ch);
						spool24.SetValid(0, ch);
						spool32.SetValid(0, ch);
						spoolf32.SetValid(0, ch);
						spoolf64.SetValid(0, ch);
						spoolout.SetValid(0, ch);
					}
					break;
				case 64:
					ipool8.SetValid(0);
					ipool16.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool16.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				}
			}
			else
			{
				switch (chunk->impl->bps.BPS)
				{
				case 8:
					ipool8.SetValid(0);
					ipool16.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool16.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				case 16:
					ipool8.SetValid(0);
					ipool16.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				case 24:
					ipool8.SetValid(0);
					ipool16.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool16.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolf64.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				case 32:
					if (chunk->impl->bps.IsFloating)
					{
						ipool8.SetValid(0);
						ipool16.SetValid(0);
						ipool24.SetValid(0);
						ipool32.SetValid(0);
						ipoolf32.SetValid(0);
						ipoolf64.SetValid(0);
						ipoolout.SetValid(0);
						spool8.SetValid(0, ch);
						spool16.SetValid(0, ch);
						spool24.SetValid(0, ch);
						spool32.SetValid(0, ch);
						spoolf64.SetValid(0, ch);
						spoolout.SetValid(0, ch);
					}
					else
					{
						ipool8.SetValid(0);
						ipool16.SetValid(0);
						ipool24.SetValid(0);
						ipool32.SetValid(0);
						ipoolf32.SetValid(0);
						ipoolf64.SetValid(0);
						ipoolout.SetValid(0);
						spool8.SetValid(0, ch);
						spool16.SetValid(0, ch);
						spool24.SetValid(0, ch);
						spoolf32.SetValid(0, ch);
						spoolf64.SetValid(0, ch);
						spoolout.SetValid(0, ch);
					}
					break;
				case 64:
					ipool8.SetValid(0);
					ipool16.SetValid(0);
					ipool24.SetValid(0);
					ipool32.SetValid(0);
					ipoolf32.SetValid(0);
					ipoolf64.SetValid(0);
					ipoolout.SetValid(0);
					spool8.SetValid(0, ch);
					spool16.SetValid(0, ch);
					spool24.SetValid(0, ch);
					spool32.SetValid(0, ch);
					spoolf32.SetValid(0, ch);
					spoolout.SetValid(0, ch);
					break;
				}
			}

			bpsnum = chunk->impl->bps.BPS;
			bps = chunk->impl->bps;

			duration = (double)samplecount / (double)sr;
		}
		delete history;
		history = nullptr;
		internal = nullptr;

		delete chunk;
		chunk = nullptr;
		result.Value = true;
	}

	return result;
}

AudioChunk::AudioChunk():impl(nullptr)
{
}

AudioChunk::~AudioChunk()
{
	SAFE_DELETE(impl);
}

void* AudioChunk::Interleaved()
{
	if (!impl) return nullptr;
	return impl->GetInterleaved();
}

void** AudioChunk::Splitted()
{
	if (!impl) return nullptr;
	return impl->GetSplitted();
}

bool AudioChunk::GetInterleaved() const
{
	if (!impl) return nullptr;
	return impl->GetInterleaved();
}

void AudioChunk::SetInterleaved(bool interleaved)
{
	if (impl) impl->SetInterleaved(interleaved);
}

BitsPerSample AudioChunk::GetBitsPerSample() const
{
	if (!impl) return BitsPerSample::Unknown;
	return impl->bps;
}

UShort AudioChunk::GetChannels() const
{
	if (!impl) return 0;
	return impl->GetChannels();
}

UInt AudioChunk::GetSampleCount() const
{
	if (!impl) return 0u;
	return impl->GetSampleCount();
}

void AudioChunk::SetSampleCount(UInt samplecount)
{
	if (impl) impl->SetSampleCount(samplecount);
}

Result<bool> AudioChunk::EnsureSampleCount(UInt samplecount, bool interleaved)
{
	if (!impl) return Result<bool>{false};
	return impl->EnsureSampleCount(samplecount, interleaved);
}

UInt AudioChunk::GetSampleRate() const
{
	if (!impl) return 0u;
	return impl->GetSampleRate();
}

Result<bool> AudioChunk::SetSampleRate(UInt sr)
{
	if (!impl) return Result<bool>{false};
	return impl->SetSampleRate(sr);
}

Result<bool> AudioChunk::Reset(UInt sampleRate, BitsPerSample bitsPerSample, Audio::ChannelLayout channelLayout, bool interleaved)
{
	if (!impl) return Result<bool>(false);
	return impl->Reset(sampleRate, bitsPerSample, channelLayout, interleaved);
}