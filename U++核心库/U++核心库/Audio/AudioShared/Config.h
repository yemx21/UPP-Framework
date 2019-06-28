#pragma once
#include <System\System.h>

#ifndef AUDIOSHARED_CAPI
#ifdef AUDIOSHARED_EXPORTS
#define AUDIOSHARED_CAPI extern "C" __declspec(dllexport)
#else
#define AUDIOSHARED_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef AUDIOSHARED_API
#ifdef AUDIOSHARED_EXPORTS
#define AUDIOSHARED_API  __declspec(dllexport)
#else
#define AUDIOSHARED_API  __declspec(dllimport)
#endif
#endif

#ifndef AUDIOSHARED_CLAMP
#define AUDIOSHARED_CLAMP
template <typename T> 
__forceinline T CLAMP(T value, T low, T high)
{
	return (value < low) ? low : ((value > high) ? high : value);
}

#define CLAMP_f01(value)(CLAMP<float>(value,0.0f,1.0f))
#define CLAMP_i8(value)(CLAMP<int>(value,-128,127))
#define CLAMP_f8(value)(CLAMP<float>(value,-128.0f,127.0f))
#define CLAMP_d8(value)(CLAMP<double>(value,-128,127))
#define CLAMP_f16(value)(CLAMP<float>(value,-32768.0f,32767.0f))
#define CLAMP_d16(value)(CLAMP<double>(value,-32768,32767))
#define CLAMP_i16(value)(CLAMP<int>(value,-32768,32767))
#define CLAMP_s16(value)(CLAMP<short>(value,-32768,32767))
#define CLAMP_l16(value)(CLAMP<long>(value,-32768,32767))
#define CLAMP_f24(value)(CLAMP<float>(value,-8388608.0f,8388607.0f))
#define CLAMP_d24(value)(CLAMP<float>(value,-8388608.0f,8388607.0f))
#define CLAMP_i24(value)(CLAMP<int>(value,-8388608,8388607))
#define CLAMP_f32(value)(CLAMP<float>(value,-2147483648.0f,2147483647.0f))
#define CLAMP_d32(value)(CLAMP<float>(value,-2147483648.0f,2147483647.0f))
#define CLAMP_ff32(value)(CLAMP<float>(value,-1.0f,1.0f))
#define CLAMP_d64(value)(CLAMP<double>(value,-1.0f,1.0f))

#endif
