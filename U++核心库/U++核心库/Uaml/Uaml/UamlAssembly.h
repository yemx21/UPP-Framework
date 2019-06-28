#pragma once
#include "Uaml_Config.h"

namespace System
{
	namespace Uaml
	{
		class UamlAssemblyManager;
		class UamlAssembly_impl;


		class UamlAssembly
		{
		protected:
			friend UamlAssemblyManager;
			friend UamlAssembly_impl;

			const System::Assembly* Reference;

			const wchar_t* Name;
			System::Version Version;
			UInt128 PublicKey;

			size_t RefCount;

			bool Resolve();

			UamlAssembly(const wchar_t* name, const System::Version& ver, const UInt128& publicKey);
			~UamlAssembly();
		};
	}
}