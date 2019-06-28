#pragma once
#include "UICore_Config.h"
#include "Graphic\Graphic.h"

using namespace System::UI::Graphic;

namespace System
{
	namespace UI
	{
		class Application_impl;
		class DistributedApplication_impl;

		namespace Windows
		{
			class Window;
		}

		class Application;

		class UICORE_API DistributedApplication
		{
		private:
			Application* _parent;

		protected:
			friend Application;
			DistributedApplication(Application* app);
			virtual ~DistributedApplication();

		protected:
			virtual void OnCultureChanged(const CultureInfo&, const CultureInfo&);
			virtual bool OnAssembliesChanging(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey);
			virtual void OnAssembliesChanged(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey);

		};

		__uclass __uobjdef(asm = UICore, ns = System::UI, ver = 1.0) UICORE_API Application final:__uobject, public IApplication
		{
		protected:
			Application(const CultureInfo& culture);
			~Application();

			friend DistributedApplication_impl;

			bool Register(DistributedApplication* disapp);
			void Unregister(DistributedApplication* disapp);

			virtual void OnCultureChanged(const CultureInfo&, const CultureInfo&);
			virtual bool OnAssembliesChanging(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey);
			virtual void OnAssembliesChanged(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey);

		public:
			static Result<Application*> Create(HINSTANCE hInstance, FeatureLevel level, const CultureInfo& culture);

			HINSTANCE GetNativeNameSpaceHandle() const;

			_declspec(property(get = GetNativeNameSpaceHandle)) HINSTANCE NativeNameSpaceHandle;

		private:
			friend Application_impl;
			friend Windows::Window;
			Application_impl* impl;
			CriticalSection cs;
			std::list<DistributedApplication*> disapps;
			REFLECT_CLASS(Application)
		};
	}
}