#include <Windows.h>
#include <VersionHelpers.h>
#include "WindowsEnvironment.h"

using namespace System::OS;

#pragma region

API_UINT32 Microsoft::winapi_FormatMessage_ANSI(API_UINT32 dwFlags,
	const API_PTR lpSource,
	API_UINT32 dwMessageId,
	API_UINT32 dwLanguageId,
	char* lpBuffer,
	API_UINT32 nSize,
	API_VALIST Arguments)
{
	return (API_UINT32)::FormatMessageA((DWORD)dwFlags,
		(LPCVOID)lpSource,
		(DWORD)dwMessageId,
		(DWORD)dwLanguageId,
		(LPSTR)lpBuffer,
		(DWORD)nSize,
		(va_list*)Arguments);

}

#pragma endregion


#pragma region IO

API_PTR Microsoft::winapi_CreateFile_ANSI(const char* lpFileName,
	API_UINT32 dwDesiredAccess,
	API_UINT32 dwShareMode,
	API_PTR lpSecurityAttributes,
	API_UINT32 dwCreationDisposition,
	API_UINT32 dwFlagsAndAttributes,
	API_PTR hTemplateFile)
{
	return ::CreateFileA((LPCSTR)lpFileName,
		(DWORD)dwDesiredAccess,
		(DWORD)dwShareMode,
		(LPSECURITY_ATTRIBUTES)lpSecurityAttributes,
		(DWORD)dwCreationDisposition,
		(DWORD)dwFlagsAndAttributes,
		(HANDLE)hTemplateFile);
}


API_PTR Microsoft::winapi_CreateFile(const wchar_t* lpFileName,
	API_UINT32 dwDesiredAccess,
	API_UINT32 dwShareMode,
	API_PTR lpSecurityAttributes,
	API_UINT32 dwCreationDisposition,
	API_UINT32 dwFlagsAndAttributes,
	API_PTR hTemplateFile)
{
	return ::CreateFileW((LPCWSTR)lpFileName,
		(DWORD)dwDesiredAccess,
		(DWORD)dwShareMode,
		(LPSECURITY_ATTRIBUTES)lpSecurityAttributes,
		(DWORD)dwCreationDisposition,
		(DWORD)dwFlagsAndAttributes,
		(HANDLE)hTemplateFile);
}

API_UINT32 Microsoft::winapi_GetFileType(API_PTR handle)
{
	return (API_UINT32)::GetFileType((HANDLE)handle);
}


bool Microsoft::winapi_ReadFile(API_PTR hFile,
	API_PTR lpBuffer,
	API_UINT32 nNumberOfBytesToRead,
	API_UINT32_PTR lpNumberOfBytesRead,
	API_PTR lpOverlapped)
{
	return ::ReadFile((HANDLE)hFile, 
		(LPVOID)lpBuffer,
		(DWORD)nNumberOfBytesToRead,
		(LPDWORD)lpNumberOfBytesRead,
		(LPOVERLAPPED)lpOverlapped) != 0;
}


bool Microsoft::winapi_WriteFile(API_PTR hFile,
	API_PTR lpBuffer,
	API_UINT32 nNumberOfBytesToWrite,
	API_UINT32_PTR lpNumberOfBytesWritten,
	API_PTR lpOverlapped)
{
	return ::WriteFile((HANDLE)hFile,
		(LPVOID)lpBuffer,
		(DWORD)nNumberOfBytesToWrite,
		(LPDWORD)lpNumberOfBytesWritten,
		(LPOVERLAPPED)lpOverlapped) != 0;
}

API_INT64 Microsoft::winapi_SetFilePointer(API_PTR hFile, API_INT64 nMove, API_UINT32 dwmethod)
{
	API_INT64 ret = -1;

	LARGE_INTEGER in_int;
	in_int.QuadPart = nMove;

	LARGE_INTEGER out_int;
	if (::SetFilePointerEx((HANDLE)hFile, in_int, &out_int, (DWORD)dwmethod) != 0)
	{
		ret = out_int.QuadPart;
	}
	return ret;
}

bool Microsoft::winapi_SetEndOfFile(API_PTR hFile)
{
	return ::SetEndOfFile((HANDLE)hFile) != 0;
}

API_INT64 Microsoft::winapi_GetFileSize(API_PTR hFile)
{
	LARGE_INTEGER out_int;
	if (!::GetFileSizeEx((HANDLE)hFile, &out_int))
	{
		return 0;
	}
	return out_int.QuadPart;
}

bool Microsoft::winapi_FlushFileBuffers(API_PTR hFile)
{
	return ::FlushFileBuffers((HANDLE)hFile) != 0;
}

bool Microsoft::winapi_LockFile(API_PTR hFile,
	API_INT64 offset,
	API_INT64 nNumberOfBytesToLock)
{
	LARGE_INTEGER li_pos;
	li_pos.QuadPart = offset;

	LARGE_INTEGER li_count;
	li_count.QuadPart = nNumberOfBytesToLock;

	return	::LockFile(
			(HANDLE)hFile,
			(DWORD)li_pos.LowPart,
			(DWORD)li_pos.HighPart,
			(DWORD)li_count.LowPart,
			(DWORD)li_count.HighPart) != 0;
}

bool Microsoft::winapi_UnlockFile(API_PTR hFile,
	API_INT64 offset,
	API_INT64 nNumberOfBytesToUnlock)
{
	LARGE_INTEGER li_pos;
	li_pos.QuadPart = offset;

	LARGE_INTEGER li_count;
	li_count.QuadPart = nNumberOfBytesToUnlock;

	return	::UnlockFile(
		(HANDLE)hFile,
		(DWORD)li_pos.LowPart,
		(DWORD)li_pos.HighPart,
		(DWORD)li_count.LowPart,
		(DWORD)li_count.HighPart) != 0;
}



#pragma endregion

#pragma region Common

API_PTR Microsoft::winapi_LocalFree(API_PTR hMem)
{
	return (API_PTR)::LocalFree((HLOCAL)hMem);
}

API_UINT32 Microsoft::winapi_GetLastError()
{
	return (API_UINT32)::GetLastError();
}

bool Microsoft::winapi_IsBadReadPtr(const API_PTR hp, API_UINT64 ucb)
{
	return ::IsBadReadPtr((CONST VOID *)hp, (UINT_PTR)ucb) != 0;
}

bool Microsoft::winapi_IsBadWritePtr(API_PTR hp, API_UINT64 ucb)
{
	return ::IsBadWritePtr((LPVOID)hp, (UINT_PTR)ucb) != 0;
}


bool Microsoft::winapi_DuplicateHandle(API_PTR hSourceProcessHandle,
	API_PTR hSourceHandle,
	API_PTR hTargetProcessHandle,
	API_PTR2 lpTargetHandle,
	API_UINT32 dwDesiredAccess,
	bool bInheritHandle,
	API_UINT32 dwOptions)
{
	return ::DuplicateHandle((HANDLE)hSourceProcessHandle,
	(HANDLE)hSourceHandle,
		(HANDLE)hTargetProcessHandle,
		(LPHANDLE)lpTargetHandle,
		(DWORD)dwDesiredAccess,
		(BOOL)bInheritHandle,
		(DWORD)dwOptions)!=0;
}

bool Microsoft::winapi_CloseHandle(API_PTR handle)
{
	return ::CloseHandle((HANDLE)handle)>0;
}

#pragma endregion

#pragma region Process
API_PTR Microsoft::winapi_GetCurrentProcess()
{
	return (API_PTR)::GetCurrentProcess();
}

API_PTR Microsoft::winapi_InitializeCriticalSection()
{
	CRITICAL_SECTION* cs = new CRITICAL_SECTION();
	::InitializeCriticalSection(cs);
	return cs;
}

void Microsoft::winapi_DeleteCriticalSection(API_PTR& hCS)
{
	if (!hCS) return;
	::DeleteCriticalSection((LPCRITICAL_SECTION)hCS);
	delete hCS;
	hCS = nullptr;
}

bool Microsoft::winapi_TryEnterCriticalSection(API_PTR hCS)
{
	return ::TryEnterCriticalSection((LPCRITICAL_SECTION)hCS) != 0;
}

void Microsoft::winapi_EnterCriticalSection(API_PTR hCS)
{
	::EnterCriticalSection((LPCRITICAL_SECTION)hCS);
}

void Microsoft::winapi_LeaveCriticalSection(API_PTR hCS)
{
	::LeaveCriticalSection((LPCRITICAL_SECTION)hCS);
}

#pragma endregion


#pragma region Mutex

API_PTR Microsoft::winapi_CreateMutex(API_PTR securityptr,
	bool bInitialOwner,
	const wchar_t* lpName)
{
	return (API_PTR)::CreateMutexW((LPSECURITY_ATTRIBUTES)securityptr, 
		(BOOL)bInitialOwner, (LPCWSTR)lpName);
}

bool Microsoft::winapi_ReleaseMutex(API_PTR hMutex)
{
	return ::ReleaseMutex((HANDLE)hMutex) != 0;
}


API_UINT32 Microsoft::winapi_WaitForSingleObject(API_PTR hHandle, API_UINT32 dwMilliseconds)
{
	return (API_UINT32)::WaitForSingleObject((HANDLE)hHandle, (DWORD)dwMilliseconds);
}
#pragma endregion


#pragma region Thread

API_PTR Microsoft::winapi_CreateThread(API_PTR lpThreadAttributes,
	API_UINT64 dwStackSize,
	API_THREAD_START_ROUTINE lpStartAddress,
	API_PTR lpParameter,
	API_UINT32 dwCreationFlags,
	API_UINT32* lpThreadId)
{
	return (API_PTR)::CreateThread((LPSECURITY_ATTRIBUTES)lpThreadAttributes,
		(SIZE_T)dwStackSize,
		(LPTHREAD_START_ROUTINE)lpStartAddress,
		(LPVOID)lpParameter,
		(DWORD)dwCreationFlags,
		(LPDWORD)lpThreadId);
}

API_UINT32 Microsoft::winapi_SuspendThread(API_PTR hThread)
{
	return (API_UINT32)::SuspendThread((HANDLE)hThread);
}

API_UINT32 Microsoft::winapi_ResumeThread(API_PTR hThread)
{
	return (API_UINT32)::ResumeThread((HANDLE)hThread);
}

bool Microsoft::winapi_TerminateThread(API_PTR hThread, API_UINT32 dwExitCode)
{
	return ::TerminateThread((HANDLE)hThread, (DWORD)dwExitCode)!=0;
}


bool Microsoft::winapi_SetThreadPriority(API_PTR hThread, int nPriority)
{
	return ::SetThreadPriority((HANDLE)hThread, nPriority) != 0;
}

void Microsoft::winapi_Sleep(API_UINT32 dwMilliseconds)
{
	::Sleep(dwMilliseconds);
}

API_PTR Microsoft::winapi_CreateWaitableTimer(API_PTR lpTimerAttributes, bool bManualReset, const wchar_t* lpTimerName)
{
	return (API_PTR)::CreateWaitableTimerW((LPSECURITY_ATTRIBUTES)lpTimerAttributes,
		(BOOL)bManualReset,
		(LPCWSTR)lpTimerName);
}

bool Microsoft::winapi_SetWaitableTimer(API_PTR hTimer,
	API_INT64& lpDueTime, 
	long lPeriod,
	API_TIMERAPCROUTINE pfnCompletionRoutine,
	API_PTR lpArgToCompletionRoutine,
	bool fResume)
{
	LARGE_INTEGER li;
	li.QuadPart = lpDueTime;

	auto ret = ::SetWaitableTimer((HANDLE)hTimer,
		(const LARGE_INTEGER*)&li,
		(LONG)lPeriod,
		(PTIMERAPCROUTINE)pfnCompletionRoutine,
		(LPVOID)lpArgToCompletionRoutine,
		(BOOL)fResume)!=0;

	lpDueTime = li.QuadPart;

	return ret;
}

API_PTR Microsoft::winapi_CreateEvent(API_PTR lpEventAttributes, bool bManualReset, bool bInitialState, const wchar_t* lpName)
{
	return (API_PTR)::CreateEventW((LPSECURITY_ATTRIBUTES)lpEventAttributes,
		(BOOL)bManualReset,
		(BOOL)bInitialState,
		(LPCWSTR)lpName);
}

bool Microsoft::winapi_SetEvent(API_PTR hEvent)
{
	return ::SetEvent((HANDLE)hEvent) != 0;
}

bool Microsoft::winapi_ResetEvent(API_PTR hEvent)
{
	return ::ResetEvent((HANDLE)hEvent) != 0;
}

#pragma endregion

#pragma region Window

API_PTR Microsoft::winapi_LoadCursor(API_PTR hInstance, const wchar_t* lpCursorName)
{
	return (API_PTR)::LoadCursorW((HINSTANCE)hInstance, (LPCWSTR)lpCursorName);
}

API_UINT16 Microsoft::winapi_RegisterClass(API_WNDCLASS* lpWndClass)
{
	return (API_UINT16)::RegisterClassW((WNDCLASSW*) lpWndClass);
}

API_PTR Microsoft::winapi_CreateWindow(API_UINT32 dwExStyle,
	const wchar_t* lpClassName,
	const wchar_t* lpWindowName,
	API_UINT32 dwStyle,
	int X,
	int Y,
	int nWidth,
	int nHeight,
	API_PTR hWndParent,
	API_PTR hMenu,
	API_PTR hInstance,
	API_PTR lpParam)
{
	return (API_PTR)::CreateWindowExW((DWORD)dwExStyle,
		(LPCWSTR)lpClassName,
		(LPCWSTR)lpWindowName,
		(DWORD)dwStyle,
		X, Y, nWidth, nHeight,
		(HWND)hWndParent,
		(HMENU)hMenu,
		(HINSTANCE)hInstance,
		(LPVOID)lpParam);
}

API_INT64 Microsoft::winapi_SetWindowLongPtr(API_PTR hWnd, int nIndex, API_INT64 dwNewLong)
{
	return (API_INT64)::SetWindowLongPtrW((HWND)hWnd, nIndex, (LONG_PTR)dwNewLong);
}

void Microsoft::winapi_PostQuitMessage(int nExitCode)
{
	::PostQuitMessage(nExitCode);
}

bool Microsoft::winapi_SendNotifyMessage(API_PTR hWnd, API_UINT32 Msg, API_UINT64 wParam, API_INT64 lParam)
{
	return ::SendNotifyMessageW((HWND) hWnd,
		(UINT) Msg,
		(WPARAM) wParam,
		(LPARAM) lParam) != 0;
}

API_INT64 Microsoft::winapi_DefWindowProc(API_PTR hWnd,
	API_UINT32 Msg,
	API_UINT64 wParam,
	API_INT64 lParam)
{
	return (API_INT64)::DefWindowProcW((HWND)hWnd,
		(UINT) Msg, (WPARAM) wParam, (LPARAM) lParam);
}

API_INT64 Microsoft::winapi_GetWindowLongPtr(API_PTR hWnd, int nIndex)
{
	return (API_INT64)::GetWindowLongPtrW((HWND)hWnd, nIndex);
}

bool Microsoft::winapi_DestroyWindow(API_PTR hWnd)
{
	return ::DestroyWindow((HWND)hWnd) != 0;
}

#pragma endregion


#pragma region Module_Library

API_PTR Microsoft::winapi_LoadLibrary(const wchar_t* lpFileName)
{
	return (API_PTR) ::LoadLibraryW(lpFileName);
}

bool Microsoft::winapi_FreeLibrary(API_PTR hModule)
{
	return ::FreeLibrary((HMODULE)hModule) != 0;
}

API_PTR Microsoft::winapi_GetProcAddress(API_PTR hModule, const char* lpName)
{
	return (API_PTR)::GetProcAddress((HMODULE)hModule, (LPCSTR)lpName);
}

API_PTR Microsoft::winapi_GetModuleHandle(const wchar_t* lpName)
{
	return (API_PTR)::GetModuleHandleW(lpName);
}


#pragma endregion


#pragma region

bool Microsoft::winapi_CoCreateGuid(API_PTR lpGuid)
{
	return ::CoCreateGuid((GUID*)lpGuid)==S_OK;
}

#pragma endregion


#pragma region Times

void Microsoft::winapi_GetLocalTime(API_SYSTEMTIME& time)
{
	::GetLocalTime((SYSTEMTIME*)&time);
}

void Microsoft::winapi_GetSystemTime(API_SYSTEMTIME& time)
{
	::GetSystemTime((SYSTEMTIME*)&time);
}

API_UINT32 Microsoft::winapi_GetTimeZoneInformation(API_TIME_ZONE_INFORMATION& timezone)
{
	return (API_UINT32)::GetTimeZoneInformation((LPTIME_ZONE_INFORMATION)&timezone);
}

#pragma endregion


#pragma region Culture

API_UINT32 Microsoft::winapi_GetThreadLocale()
{
	return (API_UINT32)::GetThreadLocale();
}

int Microsoft::winapi_GetLocaleInfo_ANSI(API_UINT32 Locale,
	API_UINT32 LCType,
	const char* lpLCData,
	int cchData)
{
	return ::GetLocaleInfoA((DWORD)Locale, (LCTYPE)LCType, (LPSTR)lpLCData, cchData);
}

int Microsoft::winapi_GetLocaleInfo(API_UINT32 Locale,
	API_UINT32 LCType,
	const wchar_t* lpLCData,
	int cchData)
{
	return ::GetLocaleInfoW((DWORD)Locale, (LCTYPE)LCType, (LPWSTR)lpLCData, cchData);
}

API_UINT16 Microsoft::winapi_GetSystemDefaultUILanguage()
{
	return (API_UINT16)::GetSystemDefaultUILanguage();
}

API_UINT32 Microsoft::winapi_LocaleNameToLCID(const wchar_t* lpName, API_UINT32 dwFlags)
{
	return (API_UINT32)::LocaleNameToLCID((LPCWSTR)lpName, (DWORD)dwFlags);
}


bool Microsoft::winapi_EnumCalendarInfo(API_CALINFO_ENUMPROCEXEX pCalInfoEnumProcExEx,
	const wchar_t* lpLocaleName,
	API_UINT32 Calendar,
	const wchar_t* lpReserved,
	API_UINT32 CalType,
	API_PTR lParam)
{
	return ::EnumCalendarInfoExEx((CALINFO_ENUMPROCEXEX) pCalInfoEnumProcExEx,
		(LPCWSTR) lpLocaleName,
		(CALID)Calendar,
		(LPCWSTR)lpReserved,
		(CALTYPE)CalType,
		(LPARAM)lParam) != 0;
}

int Microsoft::winapi_GetCalendarInfo(const wchar_t* lpLocaleName,
	API_UINT32 Calendar,
	const wchar_t* lpReserved,
	API_UINT32 CalType,
	wchar_t* lpCalData,
	int cchData,
	API_UINT32* lpValue)
{
	return ::GetCalendarInfoEx((LPCWSTR)lpLocaleName,
		(CALID)Calendar,
		(LPCWSTR)lpReserved,
		(CALTYPE)CalType,
		(LPWSTR) lpCalData,
		cchData,
		(LPDWORD)lpValue);
}

bool Microsoft::winapi_EnumTimeFormats(API_TIMEFMT_ENUMPROCEX lpTimeFmtEnumProcEx,
	const wchar_t* lpLocaleName,
	API_UINT32 dwFlags,
	API_PTR lParam)
{
	return ::EnumTimeFormatsEx((TIMEFMT_ENUMPROCEX)lpTimeFmtEnumProcEx,
		(LPCWSTR)lpLocaleName,
		(DWORD)dwFlags,
		(LPARAM)lParam)!= 0;
}

#pragma endregion


#pragma region OSVersions

bool Microsoft::winapi_IsWindowsXPOrGreater()
{
	return ::IsWindowsXPOrGreater();
}

bool Microsoft::winapi_IsWindowsXPSP1OrGreater()
{
	return ::IsWindowsXPSP1OrGreater();
}

bool Microsoft::winapi_IsWindowsXPSP2OrGreater()
{
	return ::IsWindowsXPSP2OrGreater();
}

bool Microsoft::winapi_IsWindowsXPSP3OrGreater()
{
	return ::IsWindowsXPSP3OrGreater();
}

bool Microsoft::winapi_IsWindowsVistaOrGreater()
{
	return ::IsWindowsVistaOrGreater();
}

bool Microsoft::winapi_IsWindowsVistaSP1OrGreater()
{
	return ::IsWindowsVistaSP1OrGreater();
}

bool Microsoft::winapi_IsWindowsVistaSP2OrGreater()
{
	return ::IsWindowsVistaSP2OrGreater();
}

bool Microsoft::winapi_IsWindows7OrGreater()
{
	return ::IsWindows7OrGreater();
}

bool Microsoft::winapi_IsWindows7SP1OrGreater()
{
	return ::IsWindows7SP1OrGreater();
}

bool Microsoft::winapi_IsWindows8OrGreater()
{
	return ::IsWindows8OrGreater();
}

bool Microsoft::winapi_IsWindows8Point1OrGreater()
{
	return ::IsWindows8Point1OrGreater();
}

bool Microsoft::winapi_IsWindowsThresholdOrGreater()
{
	return ::IsWindowsThresholdOrGreater();
}

bool Microsoft::winapi_IsWindows10OrGreater()
{
	return ::IsWindows10OrGreater();
}

bool Microsoft::winapi_IsWindowsServer()
{
	return ::IsWindowsServer();
}

#pragma endregion

#pragma region regedit

API_INT32 Microsoft::winapi_RegOpenKey(API_PTR hKey,
	const wchar_t* lpSubKey,
	API_UINT32 ulOptions,
	API_UINT32 samDesired,
	API_PTR2 phkResult)
{
	return (API_INT32)::RegOpenKeyExW((HKEY)hKey,
		(LPCWSTR)lpSubKey,
		(DWORD)ulOptions,
		(REGSAM)samDesired,
		(PHKEY)phkResult);
}

API_INT32 Microsoft::winapi_RegQueryInfoKey(
	API_PTR hKey,
	wchar_t* lpClass,
	API_UINT32* lpcchClass,
	API_UINT32* lpReserved,
	API_UINT32* lpcSubKeys,
	API_UINT32* lpcbMaxSubKeyLen,
	API_UINT32* lpcbMaxClassLen,
	API_UINT32* lpcValues,
	API_UINT32* lpcbMaxValueNameLen,
	API_UINT32* lpcbMaxValueLen,
	API_UINT32* lpcbSecurityDescriptor,
	API_FILETIME* lpftLastWriteTime)
{
	return (API_INT32)::RegQueryInfoKeyW((HKEY)hKey,
			(LPWSTR) lpClass,
			(LPDWORD) lpcchClass,
			(LPDWORD)lpReserved,
			(LPDWORD)lpcSubKeys,
			(LPDWORD) lpcbMaxSubKeyLen,
			(LPDWORD)lpcbMaxClassLen,
			(LPDWORD)lpcValues,
			(LPDWORD) lpcbMaxValueNameLen,
			(LPDWORD)lpcbMaxValueLen,
			(LPDWORD)lpcbSecurityDescriptor,
			(PFILETIME) lpftLastWriteTime);
}

API_INT32 Microsoft::winapi_RegEnumKey(API_PTR hKey,
	API_UINT32 dwIndex,
	wchar_t* lpName,
	API_UINT32* lpcchName,
	API_UINT32* lpReserved,
	wchar_t* lpClass,
	API_UINT32* lpcchClass,
	API_FILETIME* lpftLastWriteTime)
{
	return (API_INT32)::RegEnumKeyExW((HKEY)hKey,
		(DWORD)dwIndex,
		(LPWSTR)lpName,
		(LPDWORD)lpcchName,
		(LPDWORD)lpReserved,
		(LPWSTR)lpClass,
		(LPDWORD)lpcchClass,
		(PFILETIME)lpftLastWriteTime);
}

API_INT32 Microsoft::winapi_RegQueryValue(API_PTR hKey,
	const wchar_t* lpValueName,
	API_UINT32* lpReserved,
	API_UINT32* lpType,
	API_BYTE* lpData,
	API_UINT32* lpcbData)
{
	return (API_INT32)::RegQueryValueExW((HKEY)hKey,
		(LPCWSTR) lpValueName,
		(LPDWORD) lpReserved,
		(LPDWORD)lpType,
		(LPBYTE)lpData,
		(LPDWORD)lpcbData);
}

API_INT32 Microsoft::winapi_RegCloseKey(API_PTR hKey)
{
	return (API_INT32)::RegCloseKey((HKEY)hKey);
}

#pragma endregion

#pragma region Fibers

API_PTR Microsoft::winapi_CreateFiber(API_UINT64 dwStackSize, API_FIBER_START_ROUTINE lpStartAddress, API_PTR lpParameter)
{
	return (API_PTR)::CreateFiber((SIZE_T)dwStackSize,
		(LPFIBER_START_ROUTINE)lpStartAddress,
		(LPVOID)lpParameter);
}

void Microsoft::winapi_SwitchToFiber(API_PTR lpFiber)
{
	::SwitchToFiber((LPVOID)lpFiber);
}

void Microsoft::winapi_DeleteFiber(API_PTR lpFiber)
{
	::DeleteFiber((LPVOID)lpFiber);
}

API_PTR Microsoft::winapi_GetCurrentFiber()
{
	return (API_PTR)::GetCurrentFiber();
}

API_PTR Microsoft::winapi_ConvertThreadToFiber(API_PTR lpParameter)
{
	return (API_PTR)::ConvertThreadToFiber((LPVOID)lpParameter);
}

#pragma endregion