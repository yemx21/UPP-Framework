#include "AppDomain_impl.h"
#include "AppDomain.h"
#include "WindowsEnvironment.h"
using namespace System;
using namespace System::OS;

Module::Module(void* v) : id(v)
{
}

Module::operator void*() const 
{ 
	return id; 
}

AppDomain_impl::AppDomain_impl()
{
	factory_instance = Factory::Create();
	std::random_device rd;
	tokenizer64.seed(rd());
}

AppDomain_impl::~AppDomain_impl()
{
	if (factory_instance) { Factory::Destory(factory_instance); }
}

Result<FactoryDllError, safe_void_ptr> AppDomain_impl::LoadDll(const wchar_t* path)
{
	return Factory::LoadDll(path);
}

Result<FactoryDllError> AppDomain_impl::UnLoadDll(const safe_void_ptr& module)
{
	return Factory::UnLoadDll(module);
}

std::unique_ptr<AssemblyEnumerator> AppDomain_impl::GetAssemblyEnumerator() const
{
	return Factory::GetAssemblyEnumerator(factory_instance);
}

std::unique_ptr<AssemblyEnumerator> AppDomain_impl::GetAssemblyEnumerator(const safe_void_ptr& module) const
{
	return Factory::GetAssemblyEnumerator(factory_instance, module);
}

const Assembly* AppDomain_impl::FindAssembly(const wchar_t* name, const System::Version& ver) const
{
	return Factory::FindAssembly(factory_instance, name, ver);
}

const Assembly* AppDomain_impl::FindAssembly(const wchar_t* name, const System::Version& ver, char* privateKey, unsigned int privateKeySize) const
{
	return Factory::FindAssembly(factory_instance, name, ver, privateKey, privateKeySize);
}

const Assembly* AppDomain_impl::FindAssembly(const wchar_t* name, const System::Version& ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize) const
{
	return Factory::FindAssembly(factory_instance, name, ver, publicKey, privateKey, privateKeySize);
}

void AppDomain_impl::RegisterApplication(IApplication* app)
{
	return Factory::RegisterApplication(factory_instance, app);
}

void AppDomain_impl::UnregisterApplication(IApplication* app)
{
	return Factory::UnregisterApplication(factory_instance, app);
}

#define NSec100_Per_Sec  10000000
#define USec_Per_Sec   1000000
#define USec_Per_MSec   1000
#define NSec_Since_1582  ((UInt64)(0x01B21DD213814000))

void AppDomain_impl::TimeGenerateGUID(UInt128* guid) const
{
	static bool  initialized = false;
	static Int64    timestamp;
	static UInt32    advance;
	static UInt16    clockseq;
	static UInt16    node_high;
	static UInt32    node_low;
	Int64            time;
	UInt16            nowseq;
	int                r;

	API_SYSTEMTIME tv;
	Microsoft::winapi_GetLocalTime(tv);

	time = ((UInt64)tv.wSecond) * USec_Per_Sec + ((UInt64)tv.wMilliseconds*USec_Per_MSec);
	time = time * 10 + NSec_Since_1582;

	if (!initialized)
	{
		timestamp = time;
		advance = 0;

		r = tokenizer();

		clockseq = r >> 16;
		node_high = r | 0x0100;

		node_low = tokenizer();

		initialized = true;
	}
	else if (time < timestamp)
	{
		timestamp = time;
		advance = 0;
		clockseq++;
	}
	else if (time == timestamp)
	{
		advance++;
		time += advance;
	}
	else
	{
		timestamp = time;
		advance = 0;
	}
	nowseq = clockseq;

	UInt tmp1 = (UInt)time;
	UShort tmp2 = (UShort)((time >> 32) & 0xffff);
	UShort tmp3 = (UShort)(((time >> 48) & 0x0ffff) | 0x1000);
	UShort tmp4 = (UShort)((nowseq & 0x3fff) | 0x8000);

	memcpy(&guid, &tmp1, sizeof(UInt));
	memcpy(&guid + sizeof(UInt), &tmp2, sizeof(UShort));
	memcpy(&guid + sizeof(UInt) + sizeof(UShort), &tmp3, sizeof(UShort));
	memcpy(&guid + sizeof(UInt) + sizeof(UShort) + sizeof(UShort), &node_low, sizeof(UInt));
	memcpy(&guid + sizeof(UInt) + sizeof(UShort) + sizeof(UShort) + sizeof(UInt), &node_high, sizeof(UShort));
	memcpy(&guid + sizeof(UInt) + sizeof(UShort) + sizeof(UShort) + sizeof(UInt) + sizeof(UShort), &tmp4, sizeof(UShort));
}

UInt128 AppDomain_impl::GenerateHashId128() const
{
	UInt128 result;
	uGUID guid;
#ifdef __WIN__
	if (!Microsoft::winapi_CoCreateGuid(&guid)) TimeGenerateGUID(&result);
	while (Uuids.find(result) != Uuids.end() || result.IsZero)
	{
		TimeGenerateGUID(&result);
	}
	memcpy(&result, &guid, sizeof(uGUID));
#else
	uuid_generate(reinterpret_cast<unsigned char *>(&guid));
	memcpy(&result, &guid, sizeof(uGUID));
	Uuids.insert(result);
#endif

	return result;
}

void AppDomain_impl::DestoryHashId128(const UInt128& id) const
{
	Uuids.erase(id);
}
