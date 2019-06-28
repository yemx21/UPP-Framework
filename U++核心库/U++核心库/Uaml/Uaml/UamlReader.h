#pragma once
#include "UamlServiceManager.h"

namespace System
{
	namespace Uaml
	{

		__uclass __uobjdef(asm = Uaml, ns = System::Uaml, ver = 1.0) UAML_API UamlReader final:__uobject
		{
		private:
			UamlServiceManagerPtr _service;
		public:
			UamlReader(const UamlServiceManagerPtr& serviceManager);
			~UamlReader();

			Result<bool, UamlError> Open(const wchar_t* path);

			void Close();

			REFLECT_CLASS(UamlReader)
		};
	}
}