#pragma once
#include "Uaml_Config.h"

namespace System
{
	namespace Uaml
	{
		class UamlAssemblyManager;
		class UamlAssembly;

		class UamlNamespace
		{
		protected:
			friend UamlAssemblyManager;

			const UamlAssembly*  Reference;

			size_t RefCount;

			UamlNamespace(const wchar_t* scope, const wchar_t* asm_name, const System::Version& asm_ver, const UInt128& asm_publicKey);
			~UamlNamespace();
		};
	}
}