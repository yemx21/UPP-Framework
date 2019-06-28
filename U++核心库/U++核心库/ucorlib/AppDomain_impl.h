#pragma once
#include "Factory.h"
#include <unordered_set> 
#include <random>
#include <mutex>
#include <vector>


class __unknown_ptr
{
private:
	void* _ptr;
	unsigned char* _ptr2;
public:
	__unknown_ptr();
	__unknown_ptr(void* ptr, const char* key, unsigned int keySize);
	__unknown_ptr(const __unknown_ptr& ptr);
	
	__unknown_ptr(__unknown_ptr&& ptr);
	__unknown_ptr& operator = (const __unknown_ptr& ptr);

	~__unknown_ptr();

	void* get(UInt32 checkSize, const char* key, unsigned int keySize) const;
};

namespace System
{
	class AppDomain;

	class Factory;

	class Module final:_noncopyable
	{
	public:
		void* id;
		Module(void* v);
		operator void*() const;
		std::vector<__unknown_ptr> Registery;
	};


	class AppDomain_impl: _noncopyable
	{
	public:
		static AppDomain* Create();
	private:
		mutable std::unordered_set<UInt128> Uuids;
		mutable std::mt19937_64 tokenizer64;
		mutable std::mt19937 tokenizer;

		void TimeGenerateGUID(UInt128* guid) const;
	protected:
		friend class AppDomain;
		friend class IApplication;
		safe_void_ptr factory_instance;

		friend class Factory;

		AppDomain_impl();

		~AppDomain_impl();

		Result<FactoryDllError, safe_void_ptr> LoadDll(const wchar_t* path);

		Result<FactoryDllError> UnLoadDll(const safe_void_ptr& module);

		std::unique_ptr<AssemblyEnumerator> GetAssemblyEnumerator() const;

		std::unique_ptr<AssemblyEnumerator> GetAssemblyEnumerator(const safe_void_ptr& module) const;

		const Assembly* FindAssembly(const wchar_t* name, const System::Version& ver) const;

		const Assembly* FindAssembly(const wchar_t* name, const System::Version& ver, char* privateKey, unsigned int privateKeySize) const;

		const Assembly* FindAssembly(const wchar_t* name, const System::Version& ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize) const;

		void RegisterApplication(IApplication* app);

		void UnregisterApplication(IApplication* app);

		static AppDomain_impl* GetCurrentAppDomainSingleton();

		UInt128 GenerateHashId128() const;

		void DestoryHashId128(const UInt128& id) const;
	};
}