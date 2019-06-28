#pragma once
#include "..\UICore_Config.h"
namespace System
{
	namespace UI
	{
		class Application;

		namespace Graphic
		{
			class RenderContext;
			typedef std::shared_ptr<RenderContext> RenderContextPtr;
		}
		namespace Windows
		{
			class Visual;
			typedef std::shared_ptr<Visual> VisualPtr;
			class Window;
			class HwndHost;

			class Window_impl
			{
			protected:
				friend Window;
				friend HwndHost;
				static map<Application*, vector<Window*>> stores;
				static mutex locker;
				HwndHostPtr host;
				MouseDevicePtr mouseDevice;
				KeyboardDevicePtr keyboardDevice;

				Application* app;
				Window_impl();

				bool CreateNativeWindow(Application* ap, Window* winInstance);

				static Window_impl* Create(const Graphic::RenderContextPtr& renderContext);

				static void Register(Window* win);

				static void Unregister(Window* win);

				static HwndHostPtr GetHostFromVisual(const VisualPtr&);

				void Show();

				void Hide();

				void Minimize();

				void Maximize();

				void Restore();
			};
		}
	}
}