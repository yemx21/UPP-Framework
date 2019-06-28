#pragma once
#include "Input\Inputs.h"
#include "..\RoutedEvent.h"

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			__uclass __uobjdef(asm = UICore, ns = System::UI::Windows, ver = 1.0) UICORE_API IInputElement : __vuobject
			{
			protected:
				IInputElement();

				virtual bool GetIsMouseOverCore() const;
				virtual bool GetIsMouseDirectlyOverCore() const;
				virtual bool GetIsMouseCapturedCore() const;
				virtual bool GetIsKeyboardFocusWithinCore() const;
				virtual bool GetIsKeyboardFocusedCore() const;
				virtual bool GetIsEnabledCore() const;
				virtual bool GetFocusableCore() const;

			public:
				bool GetIsMouseOver() const;
				bool GetIsMouseDirectlyOver() const;
				bool GetIsMouseCaptured() const;
				bool GetIsKeyboardFocusWithin() const;
				bool GetIsKeyboardFocused() const;
				bool GetIsEnabled() const;
				bool GetFocusable() const;
			public:
				__uproperty(get = GetIsMouseOver) bool IsMouseOver;
				__uproperty(get = GetIsMouseDirectlyOver) bool IsMouseDirectlyOver;
				__uproperty(get = GetIsMouseCaptured) bool IsMouseCaptured;
				__uproperty(get = GetIsKeyboardFocusWithin) bool IsKeyboardFocusWithin;
				__uproperty(get = GetIsKeyboardFocused) bool IsKeyboardFocused;
				__uproperty(get = GetIsEnabled) bool IsEnabled;
				__uproperty(get = GetFocusable) bool Focusable;

				virtual bool CaptureMouse();
				virtual bool ReleaseMouseCapture();
				virtual bool Focus();

			public:
				System::UI::Windows::Input::MouseButtonEventHandler PreviewMouseDown;
				System::UI::Windows::Input::MouseButtonEventHandler MouseDown;
				System::UI::Windows::Input::MouseButtonEventHandler PreviewMouseUp;
				System::UI::Windows::Input::MouseButtonEventHandler MouseUp;
				System::UI::Windows::Input::MouseWheelEventHandler PreviewMouseWheel;
				System::UI::Windows::Input::MouseWheelEventHandler MouseWheel;

				System::UI::Windows::Input::MouseButtonEventHandler PreviewMouseLeftButtonDown;
				System::UI::Windows::Input::MouseButtonEventHandler MouseLeftButtonDown;
				System::UI::Windows::Input::MouseButtonEventHandler PreviewMouseLeftButtonUp;
				System::UI::Windows::Input::MouseButtonEventHandler MouseLeftButtonUp;
				System::UI::Windows::Input::MouseButtonEventHandler PreviewMouseRightButtonDown;
				System::UI::Windows::Input::MouseButtonEventHandler MouseRightButtonDown;
				System::UI::Windows::Input::MouseButtonEventHandler PreviewMouseRightButtonUp;
				System::UI::Windows::Input::MouseButtonEventHandler MouseRightButtonUp;
				System::UI::Windows::Input::MouseEventHandler PreviewMouseMove;
				System::UI::Windows::Input::MouseEventHandler MouseMove;
				System::UI::Windows::Input::MouseEventHandler MouseEnter;
				System::UI::Windows::Input::MouseEventHandler MouseLeave;
				System::UI::Windows::Input::MouseEventHandler GotMouseCapture;
				System::UI::Windows::Input::MouseEventHandler LostMouseCapture;
				System::UI::Windows::Input::KeyEventHandler PreviewKeyDown;
				System::UI::Windows::Input::KeyEventHandler KeyDown;
				System::UI::Windows::Input::KeyEventHandler PreviewKeyUp;
				System::UI::Windows::Input::KeyEventHandler KeyUp;
				System::UI::Windows::Input::KeyboardFocusChangedEventHandler PreviewGotKeyboardFocus;
				System::UI::Windows::Input::KeyboardFocusChangedEventHandler GotKeyboardFocus;
				System::UI::Windows::Input::KeyboardFocusChangedEventHandler PreviewLostKeyboardFocus;
				System::UI::Windows::Input::KeyboardFocusChangedEventHandler LostKeyboardFocus;
				System::UI::Windows::Input::TextInputEventHandler PreviewTextInput;
				System::UI::Windows::Input::TextInputEventHandler TextInput;

				REFLECT_CLASS(IInputElement)
			};
		}
	}
}