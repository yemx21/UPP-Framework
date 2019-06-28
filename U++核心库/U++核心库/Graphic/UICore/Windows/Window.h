#pragma once
#include "..\UICore_Config.h"
#include "..\DependencyObject.h"
#include "..\DependencyProperty.h"
#include "HwndHost.h"
#include "Controls\IContentControl.h"
__uimport("..\\Application.h")

using namespace System;

namespace System
{
	namespace UI
	{
		class Application;

		namespace Windows
		{
			class Window;
			typedef std::shared_ptr<Window> WindowPtr;

			class Window_impl;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Windows, ver = 1.0) UICORE_API Window : std::enable_shared_from_this<Window>, public DependencyObject, public System::UI::Windows::Controls::IContentControl
			{
			public:
				__uctor Window(Application* app);
				virtual ~Window();
			public:
				static const DependencyPropertyPtr IconProperty;
				static const DependencyPropertyPtr IsActiveProperty;
				static const DependencyPropertyPtr ShowInTaskbarProperty;
				static const DependencyPropertyPtr TaskbarItemInfoProperty;
				static const DependencyPropertyPtr TitleProperty;
				static const DependencyPropertyPtr TopmostProperty;
				static const DependencyPropertyPtr NameProperty;
				static const DependencyPropertyPtr RestoreBoundsProperty;
				static const DependencyPropertyPtr SizeToContentProperty;
				static const DependencyPropertyPtr IsVisibleProperty;
				static const DependencyPropertyPtr VisibilityProperty;
				static const DependencyPropertyPtr WindowStartupLocationProperty;
				static const DependencyPropertyPtr WindowStateProperty;


				static const DependencyPropertyPtr LeftProperty;
				static const DependencyPropertyPtr TopProperty;
				static const DependencyPropertyPtr RightProperty;
				static const DependencyPropertyPtr BottomProperty;
				static const DependencyPropertyPtr WidthProperty;
				static const DependencyPropertyPtr HeightProperty;

				static const DependencyPropertyKeyPtr ActualWidthPropertyKey;
				static const DependencyPropertyKeyPtr ActualHeightPropertyKey;
				static const DependencyPropertyPtr ActualWidthProperty;
				static const DependencyPropertyPtr ActualHeightProperty;

				static const DependencyPropertyPtr MaxHeightProperty;
				static const DependencyPropertyPtr MaxWidthProperty;
				static const DependencyPropertyPtr MinHeightProperty;
				static const DependencyPropertyPtr MinWidthProperty;

			public:
				__uref void Show();
				__uref void Hide();
				__uref void Minimize();
				__uref void Maximize();
				__uref void Restore();

			public:
				VisualPtr GetContent() const override;
				void SetContent(const VisualPtr& content) override;

			public:

				void SetLeft(double left);
				double GetLeft();

				void SetTop(double top);
				double GetTop();

				void SetRight(double right);
				double GetRight();

				void SetBottom(double bottom);
				double GetBottom();

				void SetWidth(double width);
				double GetWidth();

				void SetHeight(double height);
				double GetHeight();

				double GetActualWidth();

				double GetActualHeight();
			public:

				__uproperty(get = GetContent, put = SetContent) VisualPtr Content;

				__uproperty(get = GetLeft, put = SetLeft) double Left;
				__uproperty(get = GetTop, put = SetTop) double Top;
				__uproperty(get = GetRight, put = SetRight) double Right;
				__uproperty(get = GetBottom, put = SetBottom) double Bottom;
				__uproperty(get = GetWidth, put = SetWidth) double Width;
				__uproperty(get = GetHeight, put = SetHeight) double Height;

				__uproperty(get = GetActualWidth) double ActualWidth;
				__uproperty(get = GetActualHeight) double ActualHeight;
			private:
				static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
				friend class Window_impl;
				Window_impl* impl;

				__usctordef(Window)
				REFLECT_CLASS(Window)
			};
		}
	}
}