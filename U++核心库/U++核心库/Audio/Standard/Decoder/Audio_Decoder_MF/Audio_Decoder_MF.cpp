#include "Decoder_MF.h"

#include "U++\Debug_x64\uentry.uh"
#ifdef UPP_Debug_x64
#if _UCUSTOMENTRY
System::Result<bool, void*> __uentry_before(HMODULE, const System::safe_void_ptr&)
{
	System::Result<bool, void*> result(false, nullptr);
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		if (SUCCEEDED(MFStartup(MF_VERSION)))
			result.Value = true;
		else
			result.WriteMessage(L"media foundation initializaion failed");
	}
	else
		result.WriteMessage(L"com component initializaion failed");
	return result;
}

System::Result<bool> __uentry_after(bool loaded, HMODULE, const System::safe_void_ptr&, void*)
{
	System::Result<bool> result(false);
	if (loaded)
		result.Value = true;
	else
	{
		if (SUCCEEDED(MFShutdown()))
		{
			CoUninitialize();
			result.Value = true;
		}
		else
			result.WriteMessage(L"media foundation exit failed");
	}
	return result;
}

System::Result<bool, void*> __uexit_before(HMODULE)
{
	System::Result<bool, void*> result(false, nullptr);
	if (SUCCEEDED(MFShutdown()))
	{
		CoUninitialize();
		result.Value = true;
	}
	else
		result.WriteMessage(L"media foundation exit failed");
	return result;
}

System::Result<bool> __uexit_after(bool unloaded, HMODULE, void*)
{
	System::Result<bool> result(false);
	if (unloaded)
		result.Value = true;
	else
	{
		if (SUCCEEDED(MFShutdown()))
		{
			CoUninitialize();
			result.Value = true;
		}
		else
			result.WriteMessage(L"media foundation exit failed");
	}
	return result;
}
#endif
#endif


#include "U++\Release_x64\uentry.uh"
#ifdef UPP_Release_x64
#if _UCUSTOMENTRY
System::Result<bool, void*> __uentry_before(HMODULE, const System::safe_void_ptr&)
{
	System::Result<bool, void*> result(false, nullptr);
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		if (SUCCEEDED(MFStartup(MF_VERSION)))
			result.Value = true;
		else
			result.WriteMessage(L"media foundation initializaion failed");
	}
	else
		result.WriteMessage(L"com component initializaion failed");
	return result;
}

System::Result<bool> __uentry_after(bool loaded, HMODULE, const System::safe_void_ptr&, void*)
{
	System::Result<bool> result(false);
	if (loaded)
		result.Value = true;
	else
	{
		if (SUCCEEDED(MFShutdown()))
		{
			CoUninitialize();
			result.Value = true;
		}
		else
			result.WriteMessage(L"media foundation exit failed");
	}
	return result;
}

System::Result<bool, void*> __uexit_before(HMODULE)
{
	System::Result<bool, void*> result(false, nullptr);
	if (SUCCEEDED(MFShutdown()))
	{
		CoUninitialize();
		result.Value = true;
	}
	else
		result.WriteMessage(L"media foundation exit failed");
	return result;
}

System::Result<bool> __uexit_after(bool unloaded, HMODULE, void*)
{
	System::Result<bool> result(false);
	if (unloaded)
		result.Value = true;
	else
	{
		if (SUCCEEDED(MFShutdown()))
		{
			CoUninitialize();
			result.Value = true;
		}
		else
			result.WriteMessage(L"media foundation exit failed");
	}
	return result;
}
#endif
#endif
