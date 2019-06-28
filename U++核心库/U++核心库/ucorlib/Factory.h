#pragma once 

#include "Reflection.h"

namespace System
{
	class Application_impl;

	class IApplication;

	class AssemblyEnumerator;

	class Factory final: _noncopyable
	{
	public:
		static const Assembly*  FindAssembly(const safe_void_ptr& handle, const wchar_t* name);

		static const Assembly*  FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver);

		static const Assembly*  FindAssembly(const safe_void_ptr& handle, const wchar_t* name, uGUID publicKey);

		static const Assembly*  FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver, uGUID publicKey);

		static const Assembly*  FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver, char* privateKey, unsigned int privateKeySize);

		static const Assembly*  FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize);

		static Result<FactoryError, safe_void_ptr> RegisiterModule(const safe_void_ptr& handle, void* nativeModule);

		static FactoryError AddAssembly(const safe_void_ptr& handle, const safe_void_ptr& module, Assembly* assembly, char* privateKey, unsigned int privateKeySize);

		static FactoryError RemoveAssembly(const safe_void_ptr& handle, const safe_void_ptr& module, Assembly*& assembly, char* privateKey, unsigned int privateKeySize);

		static Result<FactoryDllError, safe_void_ptr> LoadDll(const wchar_t* path);

		static Result<FactoryDllError> UnLoadDll(const safe_void_ptr& module);

		~Factory();

		static Result<FactoryError, safe_void_ptr> RegisiterModule(void* nativeModule);

		static std::unique_ptr<AssemblyEnumerator> GetAssemblyEnumerator(const safe_void_ptr& handle);

		static std::unique_ptr<AssemblyEnumerator> GetAssemblyEnumerator(const safe_void_ptr& handle, const safe_void_ptr& module);

		static FactoryError AddAssembly(const safe_void_ptr& module, Assembly* assembly, char* privateKey, unsigned int privateKeySize);

		static const Assembly*  FindAssembly(const wchar_t* name, System::Version ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize);

		static FactoryError RemoveAssembly(const safe_void_ptr& module, Assembly*& assembly, char* privateKey, unsigned int privateKeySize);

		static void RegisterApplication(const safe_void_ptr& handle, IApplication* app);

		static void UnregisterApplication(const safe_void_ptr& handle, IApplication* app);

	protected:
		friend class AppDomain_impl;
		friend class IApplication;

		static safe_void_ptr Create();
		static void Destory(safe_void_ptr& handle);

	private:
		safe_void_ptr unknown;
	};
}