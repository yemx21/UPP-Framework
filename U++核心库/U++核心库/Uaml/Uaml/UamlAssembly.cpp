#include "UamlAssembly.h"
using namespace System::Uaml;

UamlAssembly::UamlAssembly(const wchar_t* name, const System::Version& ver, const UInt128& publicKey) : Reference(nullptr), Name(name), Version(ver), PublicKey(publicKey), RefCount(0u)
{
	Resolve();
}

UamlAssembly::~UamlAssembly()
{

}

bool UamlAssembly::Resolve()
{
	GUID guid{};
	memcpy(&guid, &PublicKey, sizeof(GUID));
	Reference = AppDomain::FindAssembly(Name, Version, guid, nullptr, 0u);
	return Reference != nullptr;
}