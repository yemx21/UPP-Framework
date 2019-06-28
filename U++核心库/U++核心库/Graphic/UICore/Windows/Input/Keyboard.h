#pragma once
#include "..\..\EventManager.h"
#include "InputDevice.h"
#include "InputManager.h"
__uimport("..\IInputElement.h")

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			class IInputElement;
			typedef std::shared_ptr<IInputElement> IInputElementPtr;
		}
	}
}

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			class Window_impl;
			namespace Input
			{
				class KeyInterop
				{
				public:
					static Key KeyFromVirtualKey(int virtualKey);

					static int VirtualKeyFromKey(Key key);
				};

				class KeyboardDevice_impl;
				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API KeyboardDevice : public InputDevice
				{
				private:
					friend Window_impl;
					friend KeyboardDevice_impl;
					KeyboardDevice_impl* impl;
				protected:
					void OnProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LONG msgTime) override;
				public:
					KeyboardDevice();
					~KeyboardDevice();
					IInputElementPtr GetFocusedElement() const;

				public:
					static System::UI::Windows::Input::InputModifiers GetCurrentInputModifier();


					REFLECT_CLASS(KeyboardDevice)
				};
				typedef std::shared_ptr<KeyboardDevice> KeyboardDevicePtr;

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API KeyboardEventArgs : public InputEventArgs
				{
				public:
					__udefctor KeyboardEventArgs();
					__uctor KeyboardEventArgs(KeyboardDevicePtr mouse, Int64 timestamp);
					virtual ~KeyboardEventArgs();

					REFLECT_CLASS(KeyboardEventArgs)
				};

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API KeyEventArgs : public KeyboardEventArgs
				{
				private:
					friend KeyboardDevice;
					System::UI::Windows::Input::Key _key;
				public:
					__udefctor KeyEventArgs();
					__uctor KeyEventArgs(KeyboardDevicePtr mouse, Int64 timestamp, System::UI::Windows::Input::Key key);
					virtual ~KeyEventArgs();

					System::UI::Windows::Input::Key GetKey() const;
					System::UI::Windows::Input::InputModifiers GetModifier() const;

					__uproperty(get = GetKey) System::UI::Windows::Input::Key Key;
					__uproperty(get = GetModifier) System::UI::Windows::Input::InputModifiers Modifier;

				protected:
					void InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender) override;

					REFLECT_CLASS(KeyEventArgs)
				};

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API KeyboardFocusChangedEventArgs : public KeyboardEventArgs
				{ 
				private:
					friend KeyboardDevice;
					IInputElementPtr _oldFocus;
					IInputElementPtr _newFocus;
				public:
					__udefctor KeyboardFocusChangedEventArgs();
					__uctor KeyboardFocusChangedEventArgs(KeyboardDevicePtr keyboard, Int64 timestamp, IInputElementPtr oldFocus, IInputElementPtr newFocus);
					
					IInputElementPtr GetOldFocus() const;
					IInputElementPtr GetNewFocus() const;

					__uproperty(get = GetOldFocus) IInputElementPtr OldFocus;
					__uproperty(get = GetNewFocus) IInputElementPtr NewFocus;

				protected:
					void InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender) override;

					REFLECT_CLASS(KeyboardFocusChangedEventArgs)
				};

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API TextInputEventArgs : public InputEventArgs
				{
				private:
					friend KeyboardDevice;
					std::wstring _text;
				public:
					__udefctor TextInputEventArgs();
					__uctor TextInputEventArgs(KeyboardDevicePtr keyboard, Int64 timestamp, const std::wstring& text);

					const std::wstring& GetText() const;

					__uproperty(get = GetText) const std::wstring& Text;

				protected:
					void InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender) override;

					REFLECT_CLASS(TextInputEventArgs)
				};

				typedef RoutedEventHandler<KeyEventArgs>  KeyEventHandler;
				typedef RoutedEventHandler<KeyboardFocusChangedEventArgs> KeyboardFocusChangedEventHandler;
				typedef RoutedEventHandler<TextInputEventArgs> TextInputEventHandler;

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API Keyboard: public DependencyObject
				{
				public:
					static const RoutedEventPtr PreviewKeyDownEvent;
					static const RoutedEventPtr PreviewKeyUpEvent;
					static const RoutedEventPtr PreviewTextInputEvent;


					__usctordef(Keyboard)
					REFLECT_CLASS(Keyboard)
				};
			}
		}
	}
}
