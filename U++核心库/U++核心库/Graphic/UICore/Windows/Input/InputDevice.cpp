#include "InputDevice.h"
#include "..\HwndHost.h"
using namespace System::UI::Windows;
using namespace System::UI::Windows::Input;

InputEventArgs::InputEventArgs() : RoutedEventArgs(), _timeStamp(DateTime::Now().Ticks)
{
	
}
InputEventArgs::InputEventArgs(Int64 timeStamp) : RoutedEventArgs(), _timeStamp(timeStamp)
{

}

InputEventArgs::InputEventArgs(InputDevicePtr device, Int64 timeStamp) : RoutedEventArgs(), _timeStamp(timeStamp), _inputDevice(device)
{

}

InputEventArgs::InputEventArgs(const InputEventArgs& args)
{
	_timeStamp = args._timeStamp;
}

InputEventArgs::~InputEventArgs()
{

}

InputDevicePtr InputEventArgs::GetDevice() const
{
	return _inputDevice; 
}

Int64 InputEventArgs::GetTimeStamp() const
{
	return _timeStamp;
}

InputDevice::InputDevice() : AttachTarget(nullptr), ProcessToken(-1)
{

}

InputDevice::~InputDevice()
{

}

bool InputDevice::IsAttached(HwndHostPtr host)
{
	if (!host) return false;
	return AttachTarget == host;
}

bool InputDevice::Attach(HwndHostPtr host)
{
	if (!host) return false;	
	ProcessToken = host->m_inputDelegates += std::bind(&InputDevice::OnProcessMessageCore, std::shared_ptr<InputDevice>(this, null_deleter()), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	if (ProcessToken < 0) return false;
	AttachTarget = host;
	return true;
}

bool InputDevice::Detach(HwndHostPtr host)
{
	if (!host) return false;
	if (!AttachTarget) return false;
	if (AttachTarget == host)
	{
		host->m_inputDelegates -= ProcessToken;
		AttachTarget = nullptr;
		return true;
	}
	return false;
}

HwndHostPtr InputDevice::GetTarget() const
{
	return AttachTarget;
}

void InputDevice::SetInputDevice(InputEventArgs& eventArgs)
{
	eventArgs._inputDevice = std::shared_ptr<InputDevice>(this, null_deleter());
}

void InputDevice::SetTimeStamp(InputEventArgs& eventArgs, LONG timeStamp)
{
	eventArgs._timeStamp = timeStamp;
}

void InputDevice::SetInputDeviceTimeStamp(InputEventArgs& eventArgs, LONG timeStamp)
{
	eventArgs._inputDevice = std::shared_ptr<InputDevice>(this, null_deleter());
	eventArgs._timeStamp = timeStamp;
}

IInputElementPtr InputDevice::GetHost() const
{
	return nullptr;  //ÐèÐÞ¸Ä
}

void InputDevice::OnProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LONG msgTime) {}

void InputDevice::OnProcessMessageCore(InputDevicePtr device, HWND window, UINT message, WPARAM wparam, LPARAM lparam) 
{
	if (device) device->OnProcessMessage(window, message, wparam, lparam, GetMessageTime());
}
