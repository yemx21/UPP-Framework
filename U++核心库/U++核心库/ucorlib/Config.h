#pragma once

#define STRING2(x) #x
#define STRING(x) STRING2(x)

#if _MSC_VER <1900
	#error only support vs2015 and later, STRING(_MSC_VER)
#endif

#ifndef _uxx17
#ifdef _MSVC_LANG
#if _MSVC_LANG > 201402L
#define _uxx17	1
#else
#define _uxx17	0
#endif
#else
#if __cplusplus > 201402L
#define _uxx17	1
#else /* __cplusplus > 201402L */
#define _uxx17	0
#endif /* __cplusplus > 201402L */
#endif
#endif

#if _uxx17
//#define byte ::byte
#endif

#pragma warning( disable : 4251 75 )
#pragma warning( disable : 4520)

#if defined(_WIN32) || defined(WIN32) 
#define LITTLE_ENDIAN 
#define __WIN__
#elif defined(_WIN64) || defined(WIN64) 
#define LITTLE_ENDIAN 
#define __WIN__	
#else

#endif

#ifndef RUNTIME_CAPI
#ifdef RUNTIME_EXPORTS
#define RUNTIME_CAPI extern "C" __declspec(dllexport)
#else
#define RUNTIME_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef RUNTIME_API
#ifdef RUNTIME_EXPORTS
#define RUNTIME_API  __declspec(dllexport)
#else
#define RUNTIME_API  __declspec(dllimport)
#endif
#endif

#include "RuntimeTypes.h"
#include "KeyWords.h"

#ifndef RUNTIME_SAFE_DECONSTRUCTOR
#define RUNTIME_SAFE_DECONSTRUCTOR
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p);  (p)=NULL; } }
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p) { if(p) { free(p);  (p)=NULL; } }
#endif
#ifndef SAFE_DELETE1
#define SAFE_DELETE1(p) { if(p) { delete[] (p);  (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release();  (p)=NULL; } }
#endif
#endif

#ifndef ZEROSTRUCT
#define ZEROSTRUCT
template <typename T>
class ZeroStruct
{
public:
	ZeroStruct() { memset(this, 0, sizeof(T)); }
};
#endif

#ifndef RUNTIME_ENUM
#define RUNTIME_ENUM

#include <type_traits>

namespace System
{
	namespace Enums
	{
		template<typename T>
		inline typename std::enable_if<std::is_enum<T>::value, bool>::type Contains(T x, T testFlag)
		{
			typedef typename std::underlying_type<T>::type base;

			return static_cast<base>(x & testFlag) == static_cast<base>(testFlag);
		}

		template<typename BASE, typename T>
		inline typename std::enable_if<std::is_convertible<T, BASE>::value, bool>::type Contains(T x, T testFlag)
		{
			return static_cast<BASE>(x & testFlag) == static_cast<BASE>(testFlag);
		}
	};
}

#define ENUM_OPERATORS(T) \
enum class T;	\
	inline T	operator	&	(T x, T y) { return static_cast<T>	(static_cast<intptr_t>(x)& static_cast<intptr_t>(y)); }; \
	inline T	operator	|	(T x, T y) { return static_cast<T>	(static_cast<intptr_t>(x) | static_cast<intptr_t>(y)); }; \
	inline T	operator	^	(T x, T y) { return static_cast<T>	(static_cast<intptr_t>(x) ^ static_cast<intptr_t>(y)); }; \
	inline T	operator	~	(T x) { return static_cast<T>	(~static_cast<intptr_t>(x)); }; \
	inline T&	operator	&=	(T& x, T y) { x = x & y;	return x; }; \
	inline T&	operator	|=	(T& x, T y) { x = x | y;	return x; }; \
	inline T&	operator	^=	(T& x, T y) { x = x ^ y;	return x; }; 

#define ENUM_FLAGS(T) ENUM_OPERATORS(T)


#define ENUM_OPERATORSEX(T, TYPE) \
enum class T: TYPE;	\
	inline T	operator	&	(T x, T y) { return static_cast<T>	(static_cast<TYPE>(x)& static_cast<TYPE>(y)); }; \
	inline T	operator	|	(T x, T y) { return static_cast<T>	(static_cast<TYPE>(x) | static_cast<TYPE>(y)); }; \
	inline T	operator	^	(T x, T y) { return static_cast<T>	(static_cast<TYPE>(x) ^ static_cast<TYPE>(y)); }; \
	inline T	operator	~	(T x) { return static_cast<T>	(~static_cast<TYPE>(x)); }; \
	inline T&	operator	&=	(T& x, T y) { x = x & y;	return x; }; \
	inline T&	operator	|=	(T& x, T y) { x = x | y;	return x; }; \
	inline T&	operator	^=	(T& x, T y) { x = x ^ y;	return x; }; 

#define ENUM_FLAGSEX(T, TYPE) ENUM_OPERATORSEX(T, TYPE)

#endif