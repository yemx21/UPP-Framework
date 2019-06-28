#include "Factory.h"
#include "Shared.h"
#include "Assembly.h"
#include <algorithm>
//#include "GuidHelper.h"
#include "BuiltInAssembly.h"
#include "AppDomain.h"
#include "AppDomain_impl.h"
#include "IApplication.h"
#include "OSHelper.h"
#include "AES.h"
#include "MD5.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::Encryption;
using namespace System::OS;


__unknown_ptr::__unknown_ptr() : _ptr(nullptr), _ptr2(nullptr) {}
__unknown_ptr::__unknown_ptr(void* ptr, const char* key, unsigned int keySize)
{
	if (key == nullptr || keySize == 0)
	{
		_ptr = ptr; 
		_ptr2 = nullptr;
		return;
	}
	else
	{
		_ptr = nullptr;
	}
	try
	{
		if (OSHelper::IsValidAddress(key, keySize))
		{
			MD5Context ctx;
			MD5::Hash(ctx, key, keySize);
			AES_KEY key;
			AES::SetEncryptKey(ctx.digest, 128, &key);
			intptr_t pointer = (intptr_t)ptr;
			intptr_t vp = pointer;
			_ptr2 = new unsigned char[16]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			memcpy(_ptr2, &vp, sizeof(intptr_t));
			AES::Encrypt(_ptr2, _ptr2, &key);
		}
	}
	catch (...)
	{
		SAFE_DELETE(_ptr2);
	}

}
__unknown_ptr::__unknown_ptr(const __unknown_ptr& ptr)
{
	_ptr = ptr._ptr;
	if (ptr._ptr2)
	{
		_ptr2 = new unsigned char[16];
		memcpy(_ptr2, ptr._ptr2, sizeof(unsigned char)* 16);
	}
	else
		_ptr2 = nullptr;
}

__unknown_ptr& __unknown_ptr::operator = (const __unknown_ptr& ptr)
{
	_ptr = ptr._ptr;
	if (ptr._ptr2)
	{
		_ptr2 = new unsigned char[16];
		memcpy(_ptr2, ptr._ptr2, sizeof(unsigned char) * 16);
	}
	else
		_ptr2 = nullptr;
	return *this;
}

__unknown_ptr::__unknown_ptr(__unknown_ptr&& ptr)
{
	_ptr = ptr._ptr;
	if (ptr._ptr2)
	{
		_ptr2 = new unsigned char[16];
		memcpy(_ptr2, ptr._ptr2, sizeof(unsigned char)* 16);
		delete ptr._ptr2;
		ptr._ptr2 = nullptr;
	}
	else
		_ptr2 = nullptr;
}

__unknown_ptr::~__unknown_ptr()
{
	SAFE_DELETE(_ptr2);
}

void* __unknown_ptr::get(UInt32 checkSize, const char* key, unsigned int keySize) const
{
	if (_ptr && key==nullptr)
	{
		if (OSHelper::IsValidAddress(_ptr, checkSize))
		{
			return _ptr;
		}
	}
	if (_ptr2 && key!=nullptr && keySize>0)
	{
		if (OSHelper::IsValidAddress(key, keySize))
		{
			MD5Context ctx;
			MD5::Hash(ctx, key, keySize);
			AES_KEY key;
			AES::SetDecryptKey(ctx.digest, 128, &key);
			unsigned char res[16] = { 0 };
			AES::Decrypt(_ptr2, res, &key);
			intptr_t vp = 0;
			memcpy(&vp, res, sizeof(intptr_t));			
			if (vp != 0)
			{
				void* op = (void*)vp;
				if (OSHelper::IsValidAddress(op, checkSize))
				{
					return op;
				}
			}		
		}
	}
	return nullptr;
}


struct __unknown_002
{
	std::vector<__unknown_ptr> __i2;

	std::vector<Module*> mods;

	std::list<safe_void_ptr> apps;

	__unknown_002()
	{

	}

	static Assembly* get(const __unknown_ptr& ptr, const char* key, unsigned int keySize)
	{
		try
		{
			void* ass = ptr.get(sizeof(Assembly), key, keySize);
			if (ass)
			{
				return static_cast<Assembly*>(ass);
			}
		}
		catch (...){}
		return nullptr;
	}

};

class jnrbva
{
private:
	void* m;
public:
	explicit jnrbva(void* nm) :m(nm)
	{
		
	}
	bool operator() (Module* ptr)
	{
		if (!ptr) return false;
		return ptr->id == m;
	}
};

namespace System
{
	class cn2c791m13f928xm
	{
	private:
		const Version* ver;
		const wchar_t* name;
		uGUID token;
	public:
		explicit cn2c791m13f928xm(Assembly* assembly)
		{
			if (assembly)
			{
				ver = assembly->Version;
				name = assembly->Name;
				token = assembly->Token;
			}
			else
			{
				ver = 0;
				name = 0;
			}
		}
		bool operator() (const __unknown_ptr& ptr)
		{
			try
			{
				Assembly* d3278cm34 = __unknown_002::get(ptr, 0, 0);
				return name == 0 || d3278cm34 == 0 ? false : wcscmp(d3278cm34->Name, name) && ver ->operator==(*d3278cm34->Version) && memcmp(&token, &d3278cm34->Token, sizeof(uGUID)) == 0;
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class c4_08139vbnc083
	{
	private:
		const wchar_t* _name;
	public:
		explicit c4_08139vbnc083(const wchar_t* name) :_name(name) {}
		bool operator() (const __unknown_ptr& ptr)
		{
			try
			{
				Assembly* c324cv2vh = __unknown_002::get(ptr, 0, 0);
				if (_name == 0 || c324cv2vh == 0) return false;
				if (c324cv2vh->Name == 0 || c324cv2vh->Token != UGUID_NULL) return false;
				if (wcscmp(c324cv2vh->Name, _name) != 0) return false;
				if (c324cv2vh->Version)
				if (!c324cv2vh->Version->IsEmpty)return false;
				return true;
			}
			catch (...)
			{
				return false;
			}
			
		}
	};

	class cb3248v_m308g
	{
	private:
		Version _ver;
		const wchar_t* _name;
	public:
		explicit cb3248v_m308g(const wchar_t* name, Version ver) :_name(name), _ver(ver) {}
		bool operator() (const __unknown_ptr& ptr)
		{
			try
			{
				Assembly* xn3478cdasf4g = __unknown_002::get(ptr, 0, 0);
				if (_name == 0 || xn3478cdasf4g == 0) return false;
				if (xn3478cdasf4g->Name == 0 || xn3478cdasf4g->Token != UGUID_NULL || xn3478cdasf4g->Version == NULL) return false;
				if (wcscmp(xn3478cdasf4g->Name, _name) != 0) return false;
				if (xn3478cdasf4g->Version->operator!=(_ver)) return false;
				return true;
			}
			catch (...)
			{
				return false;
			}

		}
	};

	class mbcd32cb90564
	{
	private:
		Version _ver;
		const wchar_t* _name;
		uGUID token;
		char* ncvwevv;
		unsigned int c84fmera;
	public:
		explicit mbcd32cb90564(const wchar_t* name, Version ver, uGUID key, char* cn3v8, unsigned int cfn32) :_name(name), _ver(ver), token(key), ncvwevv(cn3v8), c84fmera(cfn32) 
		{

		}

		bool operator() (const __unknown_ptr& ptr)
		{
			try
			{
				Assembly* rcbece72 = __unknown_002::get(ptr, ncvwevv, c84fmera);
				if (_name == 0 || rcbece72 == 0) return false;
				if (rcbece72->Name == 0 || rcbece72->Version == NULL) return false;
				if (wcscmp(rcbece72->Name, _name) != 0) return false;
				if (rcbece72->Version->operator!=(_ver)) return false;
				return rcbece72->Token == token;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class zdnasfuw
	{
	private:
		Version _ver;
		const wchar_t* _name;
		char* ncvwevv;
		unsigned int c84fmera;
	public:
		explicit zdnasfuw(const wchar_t* name, Version ver,  char* cn3v8, unsigned int cfn32) :_name(name), _ver(ver), ncvwevv(cn3v8), c84fmera(cfn32)
		{

		}

		bool operator() (const __unknown_ptr& ptr)
		{
			try
			{
				Assembly* rcbece72 = __unknown_002::get(ptr, ncvwevv, c84fmera);
				if (_name == 0 || rcbece72 == 0) return false;
				if (rcbece72->Name == 0 || rcbece72->Version == NULL) return false;
				if (wcscmp(rcbece72->Name, _name) != 0) return false;
				return rcbece72->Version->operator!=(_ver);
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class lndqwdvad
	{
	private:
		Assembly* host;
		char* ncvwevv;
		unsigned int c84fmera;
	public:
		explicit lndqwdvad(Assembly* ass, char* cn3v8, unsigned int cfn32) : host(ass), ncvwevv(cn3v8), c84fmera(cfn32)
		{

		}

		bool operator() (const __unknown_ptr& ptr)
		{
			try
			{
				Assembly* rcbece72 = __unknown_002::get(ptr, ncvwevv, c84fmera);
				return rcbece72 == host;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class AssemblyEnumerator_impl
	{
	protected:
		friend class Factory;
		friend class AssemblyEnumerator;
		typedef std::vector<__unknown_ptr>* base;
		typedef std::vector<__unknown_ptr>::const_iterator base_iterator;
		typedef std::vector<__unknown_ptr>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		InertiaString c_name;
		Version c_ver;
		uGUID c_pk;
		const Assembly* c_asm;
		AssemblyEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();

			Assembly* nbrerb4 = __unknown_002::get(*citer, 0, 0);
			if (nbrerb4)
			{
				c_name = nbrerb4->Name;
				c_ver = *nbrerb4->Version;
				c_pk = nbrerb4->Token;
			}
			else
			{
				c_name = nullptr;
				c_ver = Version();
				memset(&c_pk, 0, sizeof(uGUID));
			}
		}

		static AssemblyEnumerator* CreateEnumerator(base owner)
		{
			AssemblyEnumerator* result= new AssemblyEnumerator();
			if (!OSHelper::IsValidAddress(owner, sizeof(owner), false)) return nullptr;
			result->impl = new AssemblyEnumerator_impl(owner);
			return result;
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const wchar_t* GetCurrentName() const
		{
			return c_name->c_str();
		}

		Version GetCurrentVersion() const
		{
			return c_ver;
		}

		uGUID GetCurrentPublicKey() const
		{
			return c_pk;
		}

		const Assembly* GetCurrent(char* privateKey = nullptr, unsigned int privateKeySize = 0) const
		{
			try
			{
				return __unknown_002::get(*citer, privateKey, privateKeySize);
			}
			catch (...){}
			return nullptr;
		}

		void Next()
		{
			citer++; 
			if (citer != iterOwner->end())
			{
				Assembly* nbrerb4 = __unknown_002::get(*citer, 0, 0);
				if (nbrerb4)
				{
					c_name = nbrerb4->Name;
					c_ver = *nbrerb4->Version;
					c_pk = nbrerb4->Token;
				}
				else
				{
					c_name = nullptr;
					c_ver = Version();
					memset(&c_pk, 0, sizeof(uGUID));
				}
			}
		}

		void Previous()
		{
			citer--;			
			if (citer != iterOwner->begin())
			{
				Assembly* nbrerb4 = __unknown_002::get(*citer, 0, 0);
				if (nbrerb4)
				{
					c_name = nbrerb4->Name;
					c_ver = *nbrerb4->Version;
					c_pk = nbrerb4->Token;
				}
				else
				{
					c_name = nullptr;
					c_ver = Version();
					memset(&c_pk, 0, sizeof(uGUID));
				}
			}
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	AssemblyEnumerator::AssemblyEnumerator() :impl(nullptr)
	{

	}

	AssemblyEnumerator::~AssemblyEnumerator()
	{
		if (impl){ delete impl; impl = nullptr; }
	}

	const wchar_t* AssemblyEnumerator::GetCurrentName() const
	{
		if (impl) return impl->GetCurrentName();
		return nullptr;
	}

	Version AssemblyEnumerator::GetCurrentVersion() const
	{
		if (impl) return impl->GetCurrentVersion();
		return Version();
	}

	uGUID AssemblyEnumerator::GetCurrentPublicKey() const
	{
		if (impl) return impl->GetCurrentPublicKey(); 
		return UGUID_NULL;
	}

	const Assembly* AssemblyEnumerator::GetCurrent(char* privateKey, unsigned int privateKeySize) const
	{
		if (impl) return impl->GetCurrent(privateKey, privateKeySize);
		return nullptr;
	}

	unsigned int AssemblyEnumerator::GetCount() const
	{
		return impl != nullptr ? impl->Count() : 0;
	}

	void AssemblyEnumerator::Next()
	{
		if (impl) impl->Next();
	}

	void AssemblyEnumerator::Previous()
	{
		if (impl) impl->Previous();
	}

	bool AssemblyEnumerator::GetIsEnd() const
	{
		if (impl) return impl->IsEnd();
		return true;
	}
}

safe_void_ptr Factory::Create()
{
	Factory* instance_ = new Factory();
	__unknown_002* _u2 = new __unknown_002();
	_u2->__i2.push_back(__unknown_ptr((void*)(intptr_t)BuiltInAssemblyFactory::System, NULL, 0));
	instance_->unknown = _u2;
	return  instance_;
}

std::unique_ptr<AssemblyEnumerator> Factory::GetAssemblyEnumerator(const safe_void_ptr& handle)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						AssemblyEnumerator* miter = AssemblyEnumerator_impl::CreateEnumerator(&_un2->__i2);
						if (miter) return std::unique_ptr<AssemblyEnumerator>(miter);
					}
				}
			}
		}
	}
	catch (...){ return nullptr; }
	return nullptr;
}

std::unique_ptr<AssemblyEnumerator> Factory::GetAssemblyEnumerator(const safe_void_ptr& handle, const safe_void_ptr& module)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						void* modp = module.get(sizeof(Module));
						if (modp)
						{
							Module* mod = static_cast<Module*>(modp);
							if (mod)
							{
								if (std::find(_un2->mods.begin(), _un2->mods.end(), mod) != _un2->mods.end())
								{
									AssemblyEnumerator* miter = AssemblyEnumerator_impl::CreateEnumerator(&mod->Registery);
									if (miter) return std::unique_ptr<AssemblyEnumerator>(miter);
								}
							}
						}
					}
				}
			}
		}
	}
	catch (...){ return nullptr; }
	return nullptr;
}

Result<FactoryError, safe_void_ptr> Factory::RegisiterModule(const safe_void_ptr& handle, void* nativeModule)
{
	Result<FactoryError, safe_void_ptr> result = Result<FactoryError, safe_void_ptr>(FactoryError::Unknown, nullptr);
	if (nativeModule == nullptr)
	{
		return result;
	}
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);

					auto iter = std::find_if(_un2->mods.begin(), _un2->mods.end(), jnrbva(nativeModule));
					if (iter != _un2->mods.end())
					{
						result.Value = FactoryError::AlreadyExist;
						result.State = *iter;
						return result;
					}
					Module* mod = new Module(nativeModule);
					_un2->mods.push_back(mod);
					result.Value = FactoryError::OK;
					result.State = mod;
					return result;
				}
			}
		}
	}
	catch (...){ result.Value= FactoryError::Unknown; }
	return result;
}

Result<FactoryDllError> Factory::UnLoadDll(const safe_void_ptr& module)
{
	Result<FactoryDllError> result(FactoryDllError::Unknown);
	try
	{
		void* modp = module.get(sizeof(Module));
		if (modp)
		{
			Module* mod = static_cast<Module*>(modp);
			auto ID = mod->id;
			if (ID)
			{
				CoreDllMainEntry pInfo = (CoreDllMainEntry)Microsoft::winapi_GetProcAddress(ID, "CoreDllMain");
				if (pInfo)
				{
					CoreDllMain* dm = pInfo();
					if (dm)
					{
						if (dm->Exit)
						{
							Result<bool> eres = dm->Exit();
							if (eres.Value)
							{
								if (Microsoft::winapi_FreeLibrary(ID))
								{
									result.Value = FactoryDllError::DllFreeFailed;
									result.WriteMessage(L"free library fails, should try unload function");
								}
								else
								{
									result.WriteMessage(eres.Message);
									result.Value = FactoryDllError::OK;
								}
							}
							else
							{
								result.Value = FactoryDllError::DllExitFailed;
								result.WriteMessage(eres.Message);
								return result;
							}
						}
						else
						{
							result.Value = FactoryDllError::InitExitEmpty;
							return result;
						}
					}
					else
					{
						result.Value = FactoryDllError::EntryNotFound;
						return result;
					}
				}
				else
				{
					result.Value = FactoryDllError::EntryNotFound;
					return result;
				}
			}
			else
			{
				result.Value = FactoryDllError::DllLoadFailed;
			}
		}
		else
		{
			result.Value = FactoryDllError::DllLoadFailed;
		}
	}
	catch (...){  }
	return result;
}

FactoryError Factory::AddAssembly(const safe_void_ptr& handle, const safe_void_ptr& module, Assembly* assembly, char* privateKey, unsigned int privateKeySize)
{
	if (assembly == BuiltInAssemblyFactory::System) return FactoryError::UnSupported;
	if (!assembly)return FactoryError::EmptyAssembly;
	if (!assembly->Name) return FactoryError::InvalidAssembly;
	if (privateKeySize>2048)return FactoryError::UnSupportedKey;
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					void* _mm = module.get(sizeof(Module));
					if (_mm)
					{
						Module* mod = static_cast<Module*>(_mm);
						__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
						if (_un2 && mod)
						{
							if (!privateKey || privateKeySize == 0)
							{
								mod->Registery.push_back(__unknown_ptr(assembly, nullptr, 0));
								_un2->__i2.push_back(__unknown_ptr(assembly, nullptr, 0));
								for (const safe_void_ptr& appptr : _un2->apps)
								{
									void* rawappptr = appptr.get(sizeof(IApplication));
									if (rawappptr)
									{
										IApplication* app = static_cast<IApplication*>(rawappptr);
										if (app)
										{
											UInt128 asmid{};
											const uGUID asmtoken = assembly->Token;
											memcpy(&asmid, &asmtoken, sizeof(uGUID));
											app->CoreAssembliesChanging(AssembliesChangedNotification::Add, assembly->Name, *assembly->Version, asmid);
											app->CoreAssembliesChanged(AssembliesChangedNotification::Add, assembly->Name, *assembly->Version, asmid);
										}
									}
								}
								return FactoryError::OK;
							}
							else
							{
								if (OSHelper::IsValidAddress(privateKey, privateKeySize, false))
								{
									mod->Registery.push_back(__unknown_ptr(assembly, privateKey, privateKeySize));
									_un2->__i2.push_back(__unknown_ptr(assembly, privateKey, privateKeySize));
									for (const safe_void_ptr& appptr : _un2->apps)
									{
										void* rawappptr = appptr.get(sizeof(IApplication));
										if (rawappptr)
										{
											IApplication* app = static_cast<IApplication*>(rawappptr);
											if (app)
											{
												UInt128 asmid{};
												const uGUID asmtoken = assembly->Token;
												memcpy(&asmid, &asmtoken, sizeof(uGUID));
												app->CoreAssembliesChanging(AssembliesChangedNotification::Add, assembly->Name, *assembly->Version, asmid);
												app->CoreAssembliesChanged(AssembliesChangedNotification::Add, assembly->Name, *assembly->Version, asmid);
											}
										}
									}
									return FactoryError::OK;
								}
								else
									return FactoryError::UnSupportedKey;
							}
						}
					}
				}
			}
		}
	}
	catch (...){ return FactoryError::Unknown; }
	return FactoryError::Unknown;
}

const Assembly*  Factory::FindAssembly(const safe_void_ptr& handle, const wchar_t* name)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), c4_08139vbnc083(name));
						if (iter != _un2->__i2.end())
						{
							return __unknown_002::get(*iter, 0, 0);
						}
					}
				}
			}
		}
	}
	catch (...){}
	return nullptr;
}

const Assembly*  Factory::FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), cb3248v_m308g(name, ver));
						if (iter != _un2->__i2.end())
						{
							return __unknown_002::get(*iter, 0, 0);
						}
					}
				}
			}
		}
	}
	catch (...){}
	return nullptr;
}

const Assembly*  Factory::FindAssembly(const safe_void_ptr& handle, const wchar_t* name, uGUID publicKey)
{
	return FindAssembly(handle, name, Version(), publicKey);
}

const Assembly*  Factory::FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver, uGUID publicKey)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), mbcd32cb90564(name, ver, publicKey, NULL, 0));
						if (iter != _un2->__i2.end())
						{
							return __unknown_002::get(*iter, 0, 0);
						}
					}
				}
			}
		}
	}
	catch (...){}
	return nullptr;
}

const Assembly*  Factory::FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver, char* privateKey, unsigned int privateKeySize)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), zdnasfuw(name, ver, privateKey, privateKeySize));
						if (iter != _un2->__i2.end())
						{
							return __unknown_002::get(*iter, privateKey, privateKeySize);
						}
					}
				}
			}
		}
	}
	catch (...){}
	return nullptr;
}

const Assembly* Factory::FindAssembly(const safe_void_ptr& handle, const wchar_t* name, const System::Version& ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), mbcd32cb90564(name, ver, publicKey, privateKey, privateKeySize));
						if (iter != _un2->__i2.end())
						{
							return __unknown_002::get(*iter, privateKey, privateKeySize);
						}
					}
				}
			}
		}
	}
	catch (...){}
	return nullptr;
}

Result<FactoryDllError, safe_void_ptr> Factory::LoadDll(const wchar_t* path)
{
	Result<FactoryDllError, safe_void_ptr> result = Result<FactoryDllError, safe_void_ptr>(FactoryDllError::Unknown, nullptr);
	if (!path)
	{
		result.Value = FactoryDllError::FileNotFound;
		return result;
	}
	auto ID = Microsoft::winapi_LoadLibrary(path); 
	if (ID)
	{
		CoreDllMainEntry pInfo = (CoreDllMainEntry)Microsoft::winapi_GetProcAddress(ID, "CoreDllMain");
		if (pInfo)
		{
			CoreDllMain* dm = pInfo();
			if (dm)
			{
				if (dm->Init && dm->Exit)
				{
					FactoryHelper* fh = new FactoryHelper();
					fh->AddAssembly = Factory::AddAssembly;
					fh->FindAssembly = Factory::FindAssembly;
					fh->RegisiterModule = Factory::RegisiterModule;
					fh->RemoveAssembly = Factory::RemoveAssembly;
					dm->Helper = fh;

					Result<bool, safe_void_ptr> res = dm->Init();
					if (!res.Value)
					{
						if (dm->Exit)
						{
							Result<bool> eres = dm->Exit();
							if (eres.Value)
							{
								result.Value = FactoryDllError::DllInitFailed;
							}
							else
							{
								result.Value = FactoryDllError::DllExitFailedDuringInitFailed;
								result.State = res.State;
								result.WriteMessage(L"free library fails, should try unload function");
							}
						}
						else
						{
							result.Value = FactoryDllError::ExitNotFoundDuringInitFailed;
							result.WriteMessage(L"can not find exit function during initialization failure, this may lead to an assembly leak");
						}
						delete fh;
						fh = nullptr;
						dm->Helper = nullptr;
						if (Microsoft::winapi_FreeLibrary(ID))
						{
							result.State = res.State;
							result.Value = FactoryDllError::DllFreeFailed;
							result.WriteMessage(L"free library fails, should try unload function");
						}
						else
							result.WriteMessage(res.Message);
						return result;
					}
					result.State = res.State;
					result.Value = FactoryDllError::OK;
					return result;
				}
				else
				{
					result.Value = FactoryDllError::InitExitEmpty;
					return result;
				}
			}
			else
			{
				result.Value = FactoryDllError::EntryNotFound;
				return result;
			}
		}
		else
		{
			result.Value = FactoryDllError::EntryNotFound;
			return result;
		}
	}
	result.Value = FactoryDllError::DllLoadFailed;
	return result;
}

FactoryError Factory::RemoveAssembly(const safe_void_ptr& handle, const safe_void_ptr& module, Assembly*& assembly, char* privateKey, unsigned int privateKeySize)
{
	if (assembly == BuiltInAssemblyFactory::System) return FactoryError::UnSupported;
	if (!assembly)return FactoryError::EmptyAssembly;
	if (!assembly->Name) return FactoryError::InvalidAssembly;
	if (privateKeySize>2048)return FactoryError::UnSupportedKey;
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						void* _mm = module.get(sizeof(Module));
						if (_mm)
						{
							Module* mod = static_cast<Module*>(_mm);

							if (!privateKey || privateKeySize == 0)
							{
								auto miter = std::find_if(mod->Registery.begin(), mod->Registery.end(), lndqwdvad(assembly, privateKey, privateKeySize));
								if (miter != mod->Registery.end())
								{
									auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), lndqwdvad(assembly, privateKey, privateKeySize));
									if (iter != _un2->__i2.end())
									{
										void* pass = iter->get(sizeof(Assembly), privateKey, privateKeySize);
										if (pass)
										{
											Assembly* ass = static_cast<Assembly*>(pass);
											bool canremove = true;
											for (const safe_void_ptr& appptr : _un2->apps)
											{
												void* rawappptr = appptr.get(sizeof(IApplication));
												if (rawappptr)
												{
													IApplication* app = static_cast<IApplication*>(rawappptr);
													if (app)
													{
														UInt128 asmid{};
														const uGUID asmtoken = assembly->Token;
														memcpy(&asmid, &asmtoken, sizeof(uGUID));
														if (app->CoreAssembliesChanging(AssembliesChangedNotification::Remove, assembly->Name, *assembly->Version, asmid))
														{
															app->CoreAssembliesChanged(AssembliesChangedNotification::Remove, assembly->Name, *assembly->Version, asmid);
														}
														else
														{
															canremove = false;
														}
													}
												}
											}
											if (canremove)
											{
												mod->Registery.erase(miter);
												_un2->__i2.erase(iter);
												SAFE_DELETE(ass);
												if (mod->Registery.size() == 0)
												{
													auto mmt = std::find(_un2->mods.begin(), _un2->mods.end(), mod);
													if (mmt != _un2->mods.end())
													{
														_un2->mods.erase(mmt);
													}
												}
												return FactoryError::OK;
											}
											else
												return FactoryError::Locked;
										}
									}
								}
								return FactoryError::Nonexistent;
							}
							else
							{
								if (OSHelper::IsValidAddress(privateKey, privateKeySize, false))
								{
									auto miter = std::find_if(mod->Registery.begin(), mod->Registery.end(), lndqwdvad(assembly, privateKey, privateKeySize));
									if (miter != mod->Registery.end())
									{
										auto iter = std::find_if(_un2->__i2.begin(), _un2->__i2.end(), lndqwdvad(assembly, privateKey, privateKeySize));
										if (iter != _un2->__i2.end())
										{
											void* pass = iter->get(sizeof(Assembly), privateKey, privateKeySize);
											if (pass)
											{
												Assembly* ass = static_cast<Assembly*>(pass);
												bool canremove = true;
												for (const safe_void_ptr& appptr : _un2->apps)
												{
													void* rawappptr = appptr.get(sizeof(IApplication));
													if (rawappptr)
													{
														IApplication* app = static_cast<IApplication*>(rawappptr);
														if (app)
														{
															UInt128 asmid{};
															const uGUID asmtoken = assembly->Token;
															memcpy(&asmid, &asmtoken, sizeof(uGUID));
															if (app->CoreAssembliesChanging(AssembliesChangedNotification::Remove, assembly->Name, *assembly->Version, asmid))
															{
																app->CoreAssembliesChanged(AssembliesChangedNotification::Remove, assembly->Name, *assembly->Version, asmid);
															}
															else
															{
																canremove = false;
															}
														}
													}
												}
												if (canremove)
												{
													mod->Registery.erase(miter);
													_un2->__i2.erase(iter);
													SAFE_DELETE(ass);
													if (mod->Registery.size() == 0)
													{
														auto mmt = std::find(_un2->mods.begin(), _un2->mods.end(), mod);
														if (mmt != _un2->mods.end())
														{
															_un2->mods.erase(mmt);
															for (const safe_void_ptr& appptr : _un2->apps)
															{
																void* rawappptr = appptr.get(sizeof(IApplication));
																if (rawappptr)
																{
																	IApplication* app = static_cast<IApplication*>(rawappptr);
																	if (app)
																	{
																		UInt128 asmid{};
																		const uGUID asmtoken = assembly->Token;
																		memcpy(&asmid, &asmtoken, sizeof(uGUID));
																		app->CoreAssembliesChanging(AssembliesChangedNotification::Remove, assembly->Name, *assembly->Version, asmid);
																		app->CoreAssembliesChanged(AssembliesChangedNotification::Remove, assembly->Name, *assembly->Version, asmid);
																	}
																}
															}
														}
													}
													return FactoryError::OK;
												}
												else
													return FactoryError::Locked;
											}
										}
									}
									return FactoryError::Nonexistent;
								}
								else
									return FactoryError::UnSupportedKey;
							}
						}
					}
				}
			}
		}
	}
	catch (...){ return FactoryError::Unknown; }
	return FactoryError::Unknown;
}

Factory::~Factory()
{
	if (unknown)
	{
		void* _un = unknown.get(sizeof(__unknown_002));
		if (_un)
		{
			__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
			if (_un2)
			{
				delete _un2;
				_un2 = nullptr;
				unknown = 0;
			}
		}
	}
}

void Factory::Destory(safe_void_ptr& handle)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				delete Factory_Singleton;
				Factory_Singleton = nullptr;
				handle = nullptr;
			}
		}
	}
	catch (...){}
}

void Factory::RegisterApplication(const safe_void_ptr& handle, IApplication* app)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						_un2->apps.push_back(app);
					}
				}
			}
		}
	}
	catch (...){}
}

void Factory::UnregisterApplication(const safe_void_ptr& handle, IApplication* app)
{
	try
	{
		void* handlePtr = handle.get(sizeof(Factory));
		if (handlePtr)
		{
			Factory* Factory_Singleton = static_cast<Factory*>(handlePtr);
			if (Factory_Singleton)
			{
				void* _un = Factory_Singleton->unknown.get(sizeof(__unknown_002));
				if (_un)
				{
					__unknown_002* _un2 = static_cast<__unknown_002*>(_un);
					if (_un2)
					{
						_un2->apps.remove(app);
					}
				}
			}
		}
	}
	catch (...){}
}
