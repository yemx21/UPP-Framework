#pragma once
#include "Visual.h"
#include "VisualRoot.h"
#include "..\Graphic\Media\Surface.h"
#include "..\Graphic\Media\Texture2D.h"
#include "Input\InputDevice.h"

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Input
			{
				class MouseDevice;
				class KeyboardDevice;
			}

			class Window;
			class Window_impl;

			class UICORE_API HwndHost: std::enable_shared_from_this<HwndHost>
			{
			public:
				HwndHost(const RenderContextPtr& renderContext);
				virtual ~HwndHost(void);

				void SetRoot(const VisualPtr& rootElement);
				LRESULT ProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

				HWND GetHwnd() const;

				void HitTestTree(const Point& point, VisualPtr& visualResult) const;

				static Point Translate(const Point& fromPoint, const VisualPtr& fromVisual, const VisualPtr& toVisual);

				static HwndHostPtr FromVisual(const VisualPtr& visual);

				bool SetTopMost();

				Point GetLocation() const;

				bool SetLocation(const Point&);

				Size GetSize() const;

				bool SetSize(const Size&);

				Point PointFromScreen(const Point&) const;

				Point PointToScreen(const Point&) const;
			private:
				friend Window;
				friend Window_impl;
				void SetHwnd(HWND hwnd);
				void Present();
				UPDATELAYEREDWINDOWINFO info;
				POINT pptDst;
				POINT pptSrc;
				SIZE psize;
				BLENDFUNCTION BF;
				Size hwndSize;
			private:
				static void  __stdcall TimerCallback(HWND hwnd, UINT p1, UINT_PTR p2, DWORD p3);
				VisualRootPtr m_visualRoot;
				RenderContextPtr  m_renderContext;
				friend InputDevice;
				MultiCastDelegate<HWND, UINT, WPARAM, LPARAM> m_inputDelegates;
				HWND m_hwnd;
				Texture2DPtr m_hold;
				Texture2DPtr m_texture;
				SurfacePtr m_hold_surf;
				SurfacePtr m_texture_surf;
				GraphicObject m_device;

				bool m_resizeHappend;
			public:
				void RaiseEvent(RoutedEventArgs& args)
				{
					if (m_visualRoot) m_visualRoot->RaiseEvent(args);
				}
			};
		}
	}
}