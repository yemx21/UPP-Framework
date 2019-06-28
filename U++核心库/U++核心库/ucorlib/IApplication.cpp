#include "Assembly.h"
#include "Factory.h"
#include "Shared.h"
#include "AppDomain.h"
#include "AppDomain_impl.h"
#include "IApplication.h"
using namespace System;

IApplication::IApplication(const UInt128& appGuid, const CultureInfo& defCulture) :Identifier(appGuid), DefaultCulture(defCulture), CurrentCulture(defCulture)
{
	auto appdomain = AppDomain_impl::GetCurrentAppDomainSingleton();
	if (!appdomain) throw std::exception("critical error on application domain");
	appdomain->RegisterApplication(this);
}

IApplication::~IApplication()
{
	auto appdomain = AppDomain_impl::GetCurrentAppDomainSingleton();
	if (!appdomain) throw std::exception("critical error on application domain");
	appdomain->UnregisterApplication(this);
}

void IApplication::CoreCultureChanged(const CultureInfo& oldc, const CultureInfo& newc)
{
	OnCultureChanged(oldc, newc);
	CultureChanged(oldc, newc);
}

bool IApplication::CoreAssembliesChanging(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	return OnAssembliesChanging(notify, name, ver, publicKey);
}


void IApplication::CoreAssembliesChanged(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	OnAssembliesChanged(notify, name, ver, publicKey);
	AssembliesChanged(notify, name, ver, publicKey);
}

void IApplication::OnCultureChanged(const CultureInfo&, const CultureInfo&)
{

}

bool IApplication::OnAssembliesChanging(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	return true;
}

void IApplication::OnAssembliesChanged(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{

}