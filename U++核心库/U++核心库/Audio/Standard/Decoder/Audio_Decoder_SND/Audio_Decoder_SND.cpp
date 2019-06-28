#include "Decoder_SND.h"

#include "U++\Debug_x64\uentry.uh"
#ifdef UPP_Debug_x64
#if _UCUSTOMENTRY
System::Result<bool, void*> __uentry_before(HMODULE, const System::safe_void_ptr&)
{
	auto tmp = SNDAPI::DynmaicInit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uentry_after(bool loaded, HMODULE, const System::safe_void_ptr&, void*)
{
	System::Result<bool> result(false);
	if (loaded)
		result.Value = true;
	else
		result = SNDAPI::DynamicExit();
	return result;
}

System::Result<bool, void*> __uexit_before(HMODULE)
{
	auto tmp = SNDAPI::DynamicExit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uexit_after(bool unloaded, HMODULE, void*)
{
	System::Result<bool> result(false);
	if (unloaded)
		result.Value = true;
	else
		result = SNDAPI::DynamicExit();
	return result;
}
#endif
#endif

#include "U++\Release_x64\uentry.uh"
#ifdef UPP_Release_x64
#if _UCUSTOMENTRY
System::Result<bool, void*> __uentry_before(HMODULE, const System::safe_void_ptr&)
{
	auto tmp = SNDAPI::DynmaicInit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uentry_after(bool loaded, HMODULE, const System::safe_void_ptr&, void*)
{
	System::Result<bool> result(false);
	if (loaded)
		result.Value = true;
	else
		result = SNDAPI::DynamicExit();
	return result;
}

System::Result<bool, void*> __uexit_before(HMODULE)
{
	auto tmp = SNDAPI::DynamicExit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uexit_after(bool unloaded, HMODULE, void*)
{
	System::Result<bool> result(false);
	if (unloaded)
		result.Value = true;
	else
		result = SNDAPI::DynamicExit();
	return result;
}
#endif
#endif
