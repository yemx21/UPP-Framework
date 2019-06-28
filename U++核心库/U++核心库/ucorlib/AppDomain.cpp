#include "AppDomain.h"
#include "AppDomain_impl.h"
#include <time.h>
#include "Factory.h"
using namespace System;

const Assembly*  Factory::FindAssembly(const wchar_t* name, System::Version ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize)
{
	return AppDomain::FindAssembly(name, ver, publicKey, privateKey, privateKeySize);
}

FactoryError Factory::AddAssembly(const safe_void_ptr& mod, Assembly* assembly, char* privateKey, unsigned int privateKeySize)
{
	return Factory::AddAssembly(AppDomain::impl->factory_instance, mod,assembly, privateKey, privateKeySize);
}

FactoryError Factory::RemoveAssembly(const safe_void_ptr& module, Assembly*& assembly, char* privateKey, unsigned int privateKeySize)
{
	return Factory::RemoveAssembly(AppDomain::impl->factory_instance, module, assembly, privateKey, privateKeySize);
}

Result<FactoryError, safe_void_ptr> Factory::RegisiterModule(void* nativeModule)
{
	return Factory::RegisiterModule(AppDomain::impl->factory_instance, nativeModule);
}

AppDomain::AppDomain() 
{
}

AppDomain::~AppDomain()
{
}

Result<FactoryDllError, safe_void_ptr> AppDomain::LoadDll(const wchar_t* path)
{
	return impl ? impl->LoadDll(path) : Result<FactoryDllError, safe_void_ptr>(FactoryDllError::Unknown, nullptr);
}

Result<FactoryDllError> AppDomain::UnLoadDll(const safe_void_ptr& module)
{
	return impl ? impl->UnLoadDll(module) : Result<FactoryDllError>(FactoryDllError::Unknown);
}

std::unique_ptr<AssemblyEnumerator> AppDomain::GetAssemblyEnumerator()
{
	return impl ? impl->GetAssemblyEnumerator() : nullptr;
}

std::unique_ptr<AssemblyEnumerator> AppDomain::GetAssemblyEnumerator(const safe_void_ptr& module)
{
	return impl ? impl->GetAssemblyEnumerator(module) : nullptr;
}

const Assembly* AppDomain::FindAssembly(const wchar_t* name, const System::Version& ver)
{
	return impl? impl->FindAssembly(name, ver) : nullptr;
}

const Assembly* AppDomain::FindAssembly(const wchar_t* name, const System::Version& ver, char* privateKey, unsigned int privateKeySize)
{
	return impl ? impl->FindAssembly(name, ver, privateKey, privateKeySize) : nullptr;
}


const Assembly* AppDomain::FindAssembly(const wchar_t* name, const System::Version& ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize)
{
	return impl ? impl->FindAssembly(name, ver, publicKey, privateKey, privateKeySize) : nullptr;
}


UInt128 AppDomain::GenerateHashId128()
{
	return impl ? impl->GenerateHashId128() : 0U;
}

void AppDomain::DestoryHashId128(const UInt128& id)
{
	if(impl) impl->DestoryHashId128(id);
}


AppDomain_impl* AppDomain_impl::GetCurrentAppDomainSingleton()
{
	return AppDomain::impl;
}