#include "U++\UPlusPlus.h"

#ifndef GRAPHICCORE_CAPI
#ifdef GRAPHICCORE_EXPORTS
#define GRAPHICCORE_CAPI extern "C" __declspec(dllexport)
#else
#define GRAPHICCORE_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef GRAPHICCORE_API
#ifdef GRAPHICCORE_EXPORTS
#define GRAPHICCORE_API  __declspec(dllexport)
#else
#define GRAPHICCORE_API  __declspec(dllimport)
#endif
#endif
