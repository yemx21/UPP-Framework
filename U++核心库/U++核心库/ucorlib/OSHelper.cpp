#include "OSHelper.h"
#include <string>
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;

std::exception OSHelper::GetErrorException(ULong errorCode)
{
	if (errorCode)
	{
		API_PTR lpMsgBuf;
		auto bufLen = Microsoft::winapi_FormatMessage_ANSI(
			API_FORMAT_MESSAGE_ALLOCATE_BUFFER |
			API_FORMAT_MESSAGE_FROM_SYSTEM |
			API_FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			API_MAKELANGID(API_LANG_NEUTRAL, API_SUBLANG_DEFAULT),
			(char*)&lpMsgBuf,
			0, NULL);

		if (bufLen)
		{
			const char* lpMsgStr = (const char*)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			Microsoft::winapi_LocalFree(lpMsgBuf);
			return std::exception(result.c_str(), errorCode);
		}
	}
	return std::exception("", errorCode);
}


std::exception OSHelper::GetLastErrorException()
{
	auto error = Microsoft::winapi_GetLastError();
	if (error)
	{
		API_PTR lpMsgBuf;
		auto bufLen = Microsoft::winapi_FormatMessage_ANSI(
			API_FORMAT_MESSAGE_ALLOCATE_BUFFER |
			API_FORMAT_MESSAGE_FROM_SYSTEM |
			API_FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			API_MAKELANGID(API_LANG_NEUTRAL, API_SUBLANG_DEFAULT),
			(char*)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			const char* lpMsgStr = (const char*)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			Microsoft::winapi_LocalFree(lpMsgBuf);
			return std::exception(result.c_str(), error);
		}
	}
	return std::exception("", error);
}


__inline bool OSHelper::IsValidAddress(const void* lp, UInt64 nBytes, bool readWrite)
{
	return (lp != NULL && !Microsoft::winapi_IsBadReadPtr((API_PTR)lp, nBytes) &&
		(!readWrite || !Microsoft::winapi_IsBadWritePtr((void*)lp, nBytes)));
};

static OSVersion InternalGetOSVersion()
{
	if (Microsoft::winapi_IsWindows10OrGreater())
		return OSVersion::Win10;
	else if (Microsoft::winapi_IsWindows8Point1OrGreater())
		return OSVersion::Win8_1;
	else if (Microsoft::winapi_IsWindows8OrGreater())
		return OSVersion::Win8_0;
	else if (Microsoft::winapi_IsWindows7SP1OrGreater())
		return OSVersion::Win7_1;
	else if (Microsoft::winapi_IsWindows7OrGreater())
		return OSVersion::Win7_0;
	else if (Microsoft::winapi_IsWindowsVistaSP2OrGreater())
		return OSVersion::Vista_2;
	else if (Microsoft::winapi_IsWindowsVistaSP1OrGreater())
		return OSVersion::Vista_1;
	else if (Microsoft::winapi_IsWindowsVistaOrGreater())
		return OSVersion::Vista_0;
	else if (Microsoft::winapi_IsWindowsXPSP3OrGreater())
		return OSVersion::XP_3;
	else if (Microsoft::winapi_IsWindowsXPSP2OrGreater())
		return OSVersion::XP_2;
	else if (Microsoft::winapi_IsWindowsXPSP1OrGreater())
		return OSVersion::XP_1;
	else if (Microsoft::winapi_IsWindowsXPOrGreater())
		return OSVersion::XP_0;
	return OSVersion::UnSupported;
}

__inline OSVersion OSHelper::GetOSVersion()
{
	static OSVersion osver = InternalGetOSVersion();
	return osver;
}

safe_void_ptr::safe_void_ptr() : _ptr(nullptr)
{

}

safe_void_ptr::safe_void_ptr(void* ptr) : _ptr(ptr)
{

}

safe_void_ptr::safe_void_ptr(const safe_void_ptr& ptr)
{
	_ptr = ptr._ptr;
}

safe_void_ptr::safe_void_ptr(safe_void_ptr&& ptr)
{
	_ptr = ptr._ptr;
}

safe_void_ptr::~safe_void_ptr()
{

}

safe_void_ptr& safe_void_ptr::operator = (void* newPtr)
{
	_ptr = newPtr;
	return *this;
}

safe_void_ptr& safe_void_ptr::operator = (const safe_void_ptr& ptr)
{
	_ptr = ptr._ptr;
	return *this;
}

void* safe_void_ptr::get(UInt64 checkSize) const
{
	return OSHelper::IsValidAddress(_ptr, checkSize, false) ? _ptr : nullptr;
}

void* safe_void_ptr::operator * () const noexcept
{
	return _ptr;
}

size_t safe_void_ptr::getHash() const
{
	return std::hash<intptr_t>().operator()((intptr_t)_ptr);
}