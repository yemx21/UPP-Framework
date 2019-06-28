#include "Decoder_MPG123.h"

#include "U++\Debug_x64\uentry.uh"
#ifdef UPP_Debug_x64
#if _UCUSTOMENTRY
System::Result<bool, void*> __uentry_before(void*, const System::safe_void_ptr&)
{
	auto tmp = MPG123API::DynmaicInit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uentry_after(bool loaded, void*, const System::safe_void_ptr&, void*)
{
	System::Result<bool> result(false);
	if (loaded)
		result.Value = true;
	else
		result = MPG123API::DynamicExit();
	return result;
}

System::Result<bool, void*> __uexit_before(void*)
{
	auto tmp = MPG123API::DynamicExit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uexit_after(bool unloaded, void*, void*)
{
	System::Result<bool> result(false);
	if (unloaded)
		result.Value = true;
	else
		result = MPG123API::DynamicExit();
	return result;
}
#endif
#endif

#include "U++\Release_x64\uentry.uh"
#ifdef UPP_Release_x64
#if _UCUSTOMENTRY
System::Result<bool, void*> __uentry_before(void*, const System::safe_void_ptr&)
{
	auto tmp = MPG123API::DynmaicInit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uentry_after(bool loaded, void*, const System::safe_void_ptr&, void*)
{
	System::Result<bool> result(false);
	if (loaded)
		result.Value = true;
	else
		result = MPG123API::DynamicExit();
	return result;
}

System::Result<bool, void*> __uexit_before(void*)
{
	auto tmp = MPG123API::DynamicExit();
	return System::Result<bool, void*>(tmp.Value, nullptr, tmp.Message);
}

System::Result<bool> __uexit_after(bool unloaded, void*, void*)
{
	System::Result<bool> result(false);
	if (unloaded)
		result.Value = true;
	else
		result = MPG123API::DynamicExit();
	return result;
}
#endif
#endif