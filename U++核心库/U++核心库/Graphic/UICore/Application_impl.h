#pragma once
#include "Windows\Window.h"

using namespace System::UI::Windows;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class FeatureDependantResource;
			class FeatureIndependantResource;
		}

		class Application_impl
		{
		protected:
			friend Application;
			friend Windows::Window;
			vector<Window*> allWindows;
			GraphicFactory* factory;
			HINSTANCE instance;

			Application_impl(HINSTANCE hInstance);

			static Result<Application_impl*> Create(HINSTANCE hInstance, FeatureLevel level);


		protected:
			friend FeatureDependantResource;
			friend FeatureIndependantResource;

			static GraphicFactory* GetGraphicFactory(Application* app);
		};

		class DistributedApplication_impl
		{
		protected:
			friend DistributedApplication;

			static bool Register(Application* app, DistributedApplication* disapp);
			static void Unregister(Application* app, DistributedApplication* disapp);
		};

	}
}