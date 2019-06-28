#pragma once
#include <System\Audio\Shared.h>

using namespace System;
using namespace System::Audio;

#ifndef AUDIO_CAPI
#ifdef AUDIO_EXPORTS
#define AUDIO_CAPI extern "C" __declspec(dllexport)
#else
#define AUDIO_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef AUDIO_API
#ifdef AUDIO_EXPORTS
#define AUDIO_API  __declspec(dllexport)
#else
#define AUDIO_API  __declspec(dllimport)
#endif
#endif