#pragma once
#include "U++\UPlusPlus.h"
#include "UICore.h"
#ifndef UAML_CAPI
#ifdef UAML_EXPORTS
#define UAML_CAPI extern "C" __declspec(dllexport)
#else
#define UAML_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef UAML_API
#ifdef UAML_EXPORTS
#define UAML_API  __declspec(dllexport)
#else
#define UAML_API  __declspec(dllimport)
#endif
#endif