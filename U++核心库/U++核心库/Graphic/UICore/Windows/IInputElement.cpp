#include "IInputElement.h"
using namespace System::UI::Windows;

IInputElement::IInputElement()
{

}

bool IInputElement::GetIsMouseOverCore() const
{
	return false;
}
bool IInputElement::GetIsMouseDirectlyOverCore() const
{
	return false;
}
bool IInputElement::GetIsMouseCapturedCore() const
{
	return false;
}
bool IInputElement::GetIsKeyboardFocusWithinCore() const
{
	return false;
}
bool IInputElement::GetIsKeyboardFocusedCore() const
{
	return false;
}
bool IInputElement::GetIsEnabledCore() const
{
	return false;
}
bool IInputElement::GetFocusableCore() const
{
	return false;
}

bool IInputElement::GetIsMouseOver() const
{
	return GetIsMouseOverCore();
}
bool IInputElement::GetIsMouseDirectlyOver() const
{
	return GetIsMouseDirectlyOverCore();
}
bool IInputElement::GetIsMouseCaptured() const
{
	return GetIsMouseCapturedCore();
}
bool IInputElement::GetIsKeyboardFocusWithin() const
{
	return GetIsKeyboardFocusWithinCore();
}
bool IInputElement::GetIsKeyboardFocused() const
{
	return GetIsKeyboardFocusedCore();
}
bool IInputElement::GetIsEnabled() const
{
	return GetIsEnabledCore();
}
bool IInputElement::GetFocusable() const
{
	return GetFocusableCore();
}

bool IInputElement::CaptureMouse()
{
	return false;
}
bool IInputElement::ReleaseMouseCapture()
{
	return false;
}
bool IInputElement::Focus()
{
	return false;
}