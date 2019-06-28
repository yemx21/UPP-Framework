#pragma once
#include "UamlAssembly.h"

namespace System
{
	namespace Uaml
	{
		class UamlAssemblyManager
		{
		protected:
			std::vector<UamlAssembly*> Assemblies;

		public:
			UamlAssemblyManager();
			~UamlAssemblyManager();

			bool IsOccupied(const wchar_t* name, const Version& ver, const UInt128& publicKey);

			void AddImport(const wchar_t* name, const Version& ver, const UInt128& publicKey);

		};
	}
}