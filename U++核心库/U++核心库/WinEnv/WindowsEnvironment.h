#pragma once
#include "WindowsEnvironment_Marcos.h"

namespace System
{
	namespace OS
	{
		class Microsoft
		{
		public:
				 
			static API_UINT32 winapi_FormatMessage_ANSI(API_UINT32 dwFlags,
				const API_PTR lpSource,
				API_UINT32 dwMessageId,
				API_UINT32 dwLanguageId,
				char* lpBuffer,
				API_UINT32 nSize,
				API_VALIST Arguments);

			static API_PTR winapi_LocalFree(API_PTR hMem);

			static API_UINT32 winapi_GetLastError();

			static bool winapi_IsBadReadPtr(const API_PTR hp, API_UINT64 ucb);
			static bool winapi_IsBadWritePtr(API_PTR hp, API_UINT64 ucb);

			static API_PTR winapi_CreateFile_ANSI(const char* lpFileName,
				API_UINT32 dwDesiredAccess,
				API_UINT32 dwShareMode,
				API_PTR lpSecurityAttributes,
				API_UINT32 dwCreationDisposition,
				API_UINT32 dwFlagsAndAttributes,
				API_PTR hTemplateFile);

			static API_PTR winapi_CreateFile(const wchar_t* lpFileName,
				API_UINT32 dwDesiredAccess,
				API_UINT32 dwShareMode,
				API_PTR lpSecurityAttributes,
				API_UINT32 dwCreationDisposition,
				API_UINT32 dwFlagsAndAttributes,
				API_PTR hTemplateFile);

			static API_UINT32 winapi_GetFileType(API_PTR hFile);

			static bool winapi_ReadFile(API_PTR hFile,
				API_PTR lpBuffer,
				API_UINT32 nNumberOfBytesToRead,
				API_UINT32_PTR lpNumberOfBytesRead,
				API_PTR lpOverlapped);

			static bool winapi_WriteFile(API_PTR hFile,
				API_PTR lpBuffer,
				API_UINT32 nNumberOfBytesToWrite,
				API_UINT32_PTR lpNumberOfBytesWritten,
				API_PTR lpOverlapped);

			static API_INT64 winapi_SetFilePointer(API_PTR hFile, API_INT64 nMove, API_UINT32 dwmethod);

			static bool winapi_SetEndOfFile(API_PTR hFile);

			static API_INT64 winapi_GetFileSize(API_PTR hFile);

			static bool winapi_FlushFileBuffers(API_PTR hFile);

			static bool winapi_LockFile(API_PTR hFile,
					API_INT64 offset,
					API_INT64 nNumberOfBytesToLock);

			static bool winapi_UnlockFile(API_PTR hFile,
				API_INT64 offset,
				API_INT64 nNumberOfBytesToUnlock);
				

			static API_PTR winapi_GetCurrentProcess();

			static bool winapi_DuplicateHandle(API_PTR hSourceProcessHandle,
				API_PTR hSourceHandle,
				API_PTR hTargetProcessHandle,
				API_PTR2 lpTargetHandle,
				API_UINT32 dwDesiredAccess,
				bool bInheritHandle,
				API_UINT32 dwOptions);

			static bool winapi_CloseHandle(API_PTR handle);

			static API_PTR winapi_InitializeCriticalSection();

			static void winapi_DeleteCriticalSection(API_PTR& hCS);

			static bool winapi_TryEnterCriticalSection(API_PTR hCS);

			static void winapi_EnterCriticalSection(API_PTR hCS);

			static void winapi_LeaveCriticalSection(API_PTR hCS);

			static API_PTR winapi_CreateMutex(API_PTR securityptr,
				bool bInitialOwner,
				const wchar_t* lpName);

			static bool winapi_ReleaseMutex(API_PTR hMutex);

			static API_UINT32 winapi_WaitForSingleObject(API_PTR hHandle, API_UINT32 dwMilliseconds);

			static API_PTR winapi_CreateThread(API_PTR lpThreadAttributes,
					API_UINT64 dwStackSize,
					API_THREAD_START_ROUTINE lpStartAddress,
					API_PTR lpParameter,
					API_UINT32 dwCreationFlags,
					API_UINT32* lpThreadId);

			static API_UINT32 winapi_SuspendThread(API_PTR hThread);

			static API_UINT32 winapi_ResumeThread(API_PTR hThread);

			static bool winapi_TerminateThread(API_PTR hThread, API_UINT32 dwExitCode);

			static bool winapi_SetThreadPriority(API_PTR hThread, int nPriority);

			static void winapi_Sleep(API_UINT32 dwMilliseconds);

			static API_PTR winapi_CreateWaitableTimer(API_PTR lpTimerAttributes, bool bManualReset, const wchar_t* lpTimerName);

			static bool winapi_SetWaitableTimer(API_PTR hTimer,
				API_INT64& lpDueTime,
				long lPeriod,
				API_TIMERAPCROUTINE pfnCompletionRoutine,
				API_PTR lpArgToCompletionRoutine,
				bool fResume);

			static API_PTR winapi_CreateEvent(API_PTR lpEventAttributes, bool bManualReset, bool bInitialState, const wchar_t* lpName);

			static bool winapi_SetEvent(API_PTR hEvent);
			static bool winapi_ResetEvent(API_PTR hEvent);

			static API_PTR winapi_GetModuleHandle(const wchar_t* lpName);

			static API_PTR winapi_LoadLibrary(const wchar_t* lpFileName);

			static bool winapi_FreeLibrary(API_PTR hModule);

			static API_PTR winapi_GetProcAddress(API_PTR hModule, const char* lpName);


			static bool winapi_CoCreateGuid(API_PTR lpGuid);

			static void winapi_GetLocalTime(API_SYSTEMTIME& time);

			static void winapi_GetSystemTime(API_SYSTEMTIME& time);

			static API_UINT32 winapi_GetTimeZoneInformation(API_TIME_ZONE_INFORMATION& timezone);

			static API_UINT32 winapi_GetThreadLocale();

			static int winapi_GetLocaleInfo_ANSI(API_UINT32 Locale,
				API_UINT32 LCType,
				const char* lpLCData,
				int cchData);

			static int winapi_GetLocaleInfo(API_UINT32 Locale,
				API_UINT32 LCType,
				const wchar_t* lpLCData,
				int cchData);


			static API_UINT16 winapi_GetSystemDefaultUILanguage();

			static API_UINT32 winapi_LocaleNameToLCID(const wchar_t* lpName, API_UINT32 dwFlags);

			static bool winapi_EnumCalendarInfo(API_CALINFO_ENUMPROCEXEX pCalInfoEnumProcExEx,
					const wchar_t* lpLocaleName,
					API_UINT32 Calendar,
					const wchar_t* lpReserved,
					API_UINT32 CalType,
					API_PTR lParam);

			static int winapi_GetCalendarInfo(const wchar_t* lpLocaleName,
					API_UINT32 Calendar,
					const wchar_t* lpReserved,
					API_UINT32 CalType,
					wchar_t* lpCalData,
					int cchData,
					API_UINT32* lpValue);

			static bool winapi_EnumTimeFormats(API_TIMEFMT_ENUMPROCEX lpTimeFmtEnumProcEx,
					const wchar_t* lpLocaleName,
					API_UINT32 dwFlags,
					API_PTR lParam);

			static API_INT32 winapi_RegOpenKey(API_PTR hKey,
					const wchar_t* lpSubKey,
					API_UINT32 ulOptions,
					API_UINT32 samDesired,
					API_PTR2 phkResult);

			static API_INT32 winapi_RegQueryInfoKey(
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
					API_FILETIME* lpftLastWriteTime);

			static API_INT32 winapi_RegEnumKey(API_PTR hKey,
					API_UINT32 dwIndex,
					wchar_t* lpName,
					API_UINT32* lpcchName,
					API_UINT32* lpReserved,
					wchar_t* lpClass,
					API_UINT32* lpcchClass,
					API_FILETIME* lpftLastWriteTime);

			static API_INT32 winapi_RegQueryValue(API_PTR hKey,
					const wchar_t* lpValueName,
					API_UINT32* lpReserved,
					API_UINT32* lpType,
					API_BYTE* lpData,
					API_UINT32* lpcbData);

			static API_INT32 winapi_RegCloseKey(API_PTR hKey);

			static API_PTR winapi_CreateFiber(API_UINT64 dwStackSize, API_FIBER_START_ROUTINE lpStartAddress, API_PTR lpParameter);

			static void winapi_SwitchToFiber(API_PTR lpFiber);

			static void winapi_DeleteFiber(API_PTR lpFiber);

			static API_PTR winapi_GetCurrentFiber();

			static API_PTR winapi_ConvertThreadToFiber(API_PTR lpParameter);


			static API_PTR winapi_LoadCursor(API_PTR hInstance, const wchar_t* lpCursorName);

			static API_UINT16 winapi_RegisterClass(API_WNDCLASS* lpWndClass);

			static void winapi_PostQuitMessage(int nExitCode);

			static bool winapi_SendNotifyMessage(API_PTR hWnd, API_UINT32 Msg, API_UINT64 wParam, API_INT64 lParam);


			static API_INT64 winapi_DefWindowProc(API_PTR hWnd,
				API_UINT32 Msg,
				API_UINT64 wParam,
				API_INT64 lParam);

			static API_INT64 winapi_GetWindowLongPtr(API_PTR hWnd, int nIndex);

			static API_INT64 winapi_SetWindowLongPtr(API_PTR hWnd, int nIndex, API_INT64 dwNewLong);

			static API_PTR winapi_CreateWindow(API_UINT32 dwExStyle,
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
				API_PTR lpParam);

			static bool winapi_DestroyWindow(API_PTR hWnd);

			static bool winapi_IsWindowsXPOrGreater();
			static bool winapi_IsWindowsXPSP1OrGreater();
			static bool winapi_IsWindowsXPSP2OrGreater();
			static bool winapi_IsWindowsXPSP3OrGreater();
			static bool winapi_IsWindowsVistaOrGreater();
			static bool winapi_IsWindowsVistaSP1OrGreater();
			static bool winapi_IsWindowsVistaSP2OrGreater();
			static bool winapi_IsWindows7OrGreater();
			static bool winapi_IsWindows7SP1OrGreater();
			static bool winapi_IsWindows8OrGreater();
			static bool winapi_IsWindows8Point1OrGreater();
			static bool winapi_IsWindowsThresholdOrGreater();
			static bool winapi_IsWindows10OrGreater();
			static bool winapi_IsWindowsServer();
		};
	}
}
