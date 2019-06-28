#include "UamlAssemblyManager.h"
#include <algorithm>
using namespace System::Uaml; 

namespace System
{
	namespace Uaml
	{
		class UamlAssembly_impl : public std::unary_function<UamlAssembly*, bool>
		{
		private:
			Version _ver;
			const wchar_t* _name;
			UInt128 token;
		public:
			explicit UamlAssembly_impl(const wchar_t* name, Version ver, const UInt128& key) :_name(name), _ver(ver), token(key)
			{

			}

			bool operator() (UamlAssembly* ptr)
			{
				if (wcscoll(ptr->Name, _name) != 0) return false;
				if (ptr->Version != _ver) return false;
				return ptr->PublicKey == token;
			}
		};
	}
}


UamlAssemblyManager::UamlAssemblyManager()
{

}

UamlAssemblyManager::~UamlAssemblyManager()
{

}

bool UamlAssemblyManager::IsOccupied(const wchar_t* name, const Version& ver, const UInt128& publicKey)
{
	auto iter = std::find_if(Assemblies.begin(), Assemblies.end(), UamlAssembly_impl(name, ver, publicKey));
	if (iter != Assemblies.end())
	{
		return (*iter)->RefCount > 0;
	}
	return false;
}