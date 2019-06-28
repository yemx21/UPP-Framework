#include "U++\UPlusPlus.h"
#ifndef UICORE_CAPI
#ifdef UICORE_EXPORTS
#define UICORE_CAPI extern "C" __declspec(dllexport)
#else
#define UICORE_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef UICORE_API
#ifdef UICORE_EXPORTS
#define UICORE_API  __declspec(dllexport)
#else
#define UICORE_API  __declspec(dllimport)
#endif
#endif

