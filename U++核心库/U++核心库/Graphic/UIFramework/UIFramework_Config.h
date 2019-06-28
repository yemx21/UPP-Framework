#include "U++\UPlusPlus.h"
#include "UICore.h"
#ifndef UIFRAMEWORK_CAPI
#ifdef UIFRAMEWORK_EXPORTS
#define UIFRAMEWORK_CAPI extern "C" __declspec(dllexport)
#else
#define UIFRAMEWORK_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef UIFRAMEWORK_API
#ifdef UIFRAMEWORK_EXPORTS
#define UIFRAMEWORK_API  __declspec(dllexport)
#else
#define UIFRAMEWORK_API  __declspec(dllimport)
#endif
#endif