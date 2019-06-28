#include "Keyboard.h"
#include "Internal\KeyboardDevice_impl.h"
#include "..\HwndHost.h"
#include "..\IInputElement.h"
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Input;


#if _WIN64
#define GET_KEY_WPARAM(wp) ((int)(wp & 0xffffffff))
#else
#define GET_KEY_WPARAM(wp) ((int)(wp))
#endif

#if _WIN64
#define GET_CHAR_WPARAM(wp) ((int)(wp & 0xffffffff))
#else
#define GET_CHAR_WPARAM(wp) ((int)(wp))
#endif

System::UI::Windows::Input::InputModifiers KeyboardDevice::GetCurrentInputModifier()
{
	unsigned char _keyStates[256];
	if (GetKeyboardState(_keyStates))
	{
		InputModifiers ret = 0;
		if (KeyboardDevice_impl::IsDown(_keyStates, Key::LeftAlt) || KeyboardDevice_impl::IsDown(_keyStates, Key::RightAlt))
		{
			ret |= InputModifiers::Alt;
		}

		if (KeyboardDevice_impl::IsDown(_keyStates, Key::LeftCtrl) || KeyboardDevice_impl::IsDown(_keyStates, Key::RightCtrl))
		{
			ret |= InputModifiers::Control;
		}

		if (KeyboardDevice_impl::IsDown(_keyStates, Key::LeftShift) || KeyboardDevice_impl::IsDown(_keyStates, Key::RightShift))
		{
			ret |= InputModifiers::Shift;
		}

		if (KeyboardDevice_impl::IsDown(_keyStates, Key::LWin) || KeyboardDevice_impl::IsDown(_keyStates, Key::RWin))
		{
			ret |= InputModifiers::Windows;
		}
		return ret;
	}
	return System::UI::Windows::Input::InputModifiers::None;
}

void KeyboardDevice::OnProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LONG msgTime)
{
	HwndHostPtr target = Target;
	if (!target) return;
	if (target->GetHwnd() != window) return;
	switch (message)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			KeyEventArgs arg;
			SetInputDeviceTimeStamp(arg, msgTime);
			arg._key = KeyInterop::KeyFromVirtualKey(GET_KEY_WPARAM(wparam));
			arg.SetRoutedEvent(Keyboard::PreviewKeyDownEvent);
			target->RaiseEvent(arg);
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			KeyEventArgs arg;
			SetInputDeviceTimeStamp(arg, msgTime);
			arg._key = KeyInterop::KeyFromVirtualKey(GET_KEY_WPARAM(wparam));
			arg.SetRoutedEvent(Keyboard::PreviewKeyUpEvent);
			target->RaiseEvent(arg);
			break;
		}
		case WM_CHAR:
		{
			// Ignore control chars
			if (GET_CHAR_WPARAM(wparam) >= 32)
			{
				TextInputEventArgs arg;
				SetInputDeviceTimeStamp(arg, msgTime);
				arg._text = wstring((wchar_t)GET_CHAR_WPARAM(wparam), 1);
				arg.SetRoutedEvent(Keyboard::PreviewTextInputEvent);
				target->RaiseEvent(arg);
			}

			break;
		}
	}
}

KeyboardDevice::KeyboardDevice() :impl(nullptr)
{
	/*need*/

}

KeyboardDevice::~KeyboardDevice()
{

}

IInputElementPtr KeyboardDevice::GetFocusedElement() const
{
	/*need*/
	return nullptr;
}


KeyboardEventArgs::KeyboardEventArgs() :InputEventArgs()
{

}

KeyboardEventArgs::KeyboardEventArgs(KeyboardDevicePtr keyboard, Int64 timestamp) : InputEventArgs(std::dynamic_pointer_cast<InputDevice>(keyboard), timestamp)
{

}
KeyboardEventArgs::~KeyboardEventArgs()
{

}

KeyEventArgs::KeyEventArgs():KeyboardEventArgs()
{

}

KeyEventArgs::KeyEventArgs(KeyboardDevicePtr keyboard, Int64 timestamp, System::UI::Windows::Input::Key key):_key(key), KeyboardEventArgs(keyboard, timestamp)
{

}

KeyEventArgs::~KeyEventArgs()
{

}

System::UI::Windows::Input::InputModifiers KeyEventArgs::GetModifier() const
{
	return KeyboardDevice::GetCurrentInputModifier();
}

System::UI::Windows::Input::Key KeyEventArgs::GetKey() const
{
	return _key;
}

void KeyEventArgs::InvokeEventHandler(IRoutedEventDelegatePtr generichandler, DependencyObjectPtr sender)
{
	std::shared_ptr<KeyEventHandler> handler = std::dynamic_pointer_cast<KeyEventHandler>(generichandler);
	if (handler)
	{
		handler->Invoke(sender, *this);
	}
}

KeyboardFocusChangedEventArgs::KeyboardFocusChangedEventArgs(): KeyboardEventArgs()
{

}
KeyboardFocusChangedEventArgs::KeyboardFocusChangedEventArgs(KeyboardDevicePtr keyboard, Int64 timestamp, IInputElementPtr oldFocus, IInputElementPtr newFocus):_oldFocus(oldFocus), _newFocus(newFocus), KeyboardEventArgs(keyboard, timestamp)
{

}

IInputElementPtr KeyboardFocusChangedEventArgs::GetOldFocus() const
{
	return _oldFocus;
}

IInputElementPtr KeyboardFocusChangedEventArgs::GetNewFocus() const
{
	return _newFocus;
}

void KeyboardFocusChangedEventArgs::InvokeEventHandler(IRoutedEventDelegatePtr generichandler, DependencyObjectPtr sender)
{
	std::shared_ptr<KeyboardFocusChangedEventHandler> handler = std::dynamic_pointer_cast<KeyboardFocusChangedEventHandler>(generichandler);
	if (handler)
	{
		handler->Invoke(sender, *this);
	}
}

TextInputEventArgs::TextInputEventArgs():InputEventArgs()
{

}
TextInputEventArgs::TextInputEventArgs(KeyboardDevicePtr keyboard, Int64 timestamp, const std::wstring& text) : _text(text), InputEventArgs(std::dynamic_pointer_cast<InputDevice>(keyboard), timestamp)
{

}
const std::wstring& TextInputEventArgs::GetText() const
{
	return _text;
}

void TextInputEventArgs::InvokeEventHandler(IRoutedEventDelegatePtr generichandler, DependencyObjectPtr sender)
{
	std::shared_ptr<TextInputEventHandler> handler = std::dynamic_pointer_cast<TextInputEventHandler>(generichandler);
	if (handler)
	{
		handler->Invoke(sender, *this);
	}
}

const RoutedEventPtr Keyboard::PreviewKeyDownEvent = make_routedevent<Keyboard>();
const RoutedEventPtr Keyboard::PreviewKeyUpEvent = make_routedevent<Keyboard>();
const RoutedEventPtr Keyboard::PreviewTextInputEvent = make_routedevent<Keyboard>();


__usctor(Keyboard)
{
	Keyboard::PreviewKeyDownEvent.get()->Register<Keyboard, KeyEventArgs>(L"PreviewKeyDown", EventStrategy::Tunnel);
	Keyboard::PreviewKeyUpEvent.get()->Register<Keyboard, KeyEventArgs>(L"PreviewKeyUp", EventStrategy::Tunnel);
	Keyboard::PreviewTextInputEvent.get()->Register<Keyboard, TextInputEventArgs>(L"PreviewTextInput", EventStrategy::Tunnel);

	return true;
}
