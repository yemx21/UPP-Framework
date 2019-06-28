#pragma once
#include "Uaml_Config.h"
#include "UamlError.h"

namespace System
{
	namespace Uaml
	{
		class UamlServiceManager;
		typedef std::shared_ptr<UamlServiceManager> UamlServiceManagerPtr;

		class UamlReader;
		typedef std::shared_ptr<UamlReader> UamlReaderPtr;


		class UamlAssemblyManager;

		__uclass __uobjdef(asm = Uaml, ns = System::Uaml, ver = 1.0) UAML_API UamlServiceManager final:__uobject, public System::UI::DistributedApplication
		{
		private:
			UamlAssemblyManager* AssemblyManager;

		protected:
			void OnCultureChanged(const CultureInfo& oldinfo, const CultureInfo& newinfo) override;
			bool OnAssembliesChanging(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey) override;
			void OnAssembliesChanged(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey) override;

		public:
			UamlServiceManager(System::UI::Application* app);
			~UamlServiceManager();
			

			static Result<UamlServiceManagerPtr> Create(System::UI::Application* app);

			Result<UamlReaderPtr> CreateReader();

			REFLECT_CLASS(UamlServiceManager)
		};
	}
}