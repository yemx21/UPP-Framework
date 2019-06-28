#include "Mouse.h"
#include "..\HwndHost.h"
#include "..\IInputElement.h"
#include <System\Reflection.h>
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Input;


void MouseButton::Validate(MouseButton button)
{
	switch (button)
	{
	case MouseButton::Left:
	case MouseButton::Middle:
	case MouseButton::Right:
	case MouseButton::XButton1:
	case MouseButton::XButton2:
		break;
	default:
		throw std::invalid_argument("button");
	}
}



#define GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))

MouseEventArgs::MouseEventArgs()
{

}

MouseEventArgs::MouseEventArgs(MouseDevicePtr mouse, Int64 timestamp): InputEventArgs(std::dynamic_pointer_cast<InputDevice>(mouse), timestamp)
{

}

MouseEventArgs::~MouseEventArgs()
{

}

MouseButtonState MouseEventArgs::GetLeftButton() const
{
	return MouseButtonState::Released;
}

MouseButtonState MouseEventArgs::GetMiddleButton() const
{
	return MouseButtonState::Released;
}

MouseButtonState MouseEventArgs::GetRightButton() const
{
	return MouseButtonState::Released;
}

MouseButtonState MouseEventArgs::GetXButton1() const
{
	return MouseButtonState::Released;
}

MouseButtonState MouseEventArgs::GetXButton2() const
{
	return MouseButtonState::Released;
}


Point MouseEventArgs::GetPosition(IInputElementPtr relativeTo) const
{
	try
	{
		VisualPtr visual = std::dynamic_pointer_cast<Visual>(relativeTo);
		if (visual)
		{
			auto dev = Device;
			if (dev)
			{
				auto mdev = std::dynamic_pointer_cast<MouseDevice>(dev);
				if (mdev)
				{
					return mdev->GetPosition(relativeTo);
				}
			}
		}
		
	}
	catch (...)
	{

	}
	return Point();
}

MouseButtonEventArgs::MouseButtonEventArgs() :_button(MouseButton::Left), MouseEventArgs()
{

}

MouseButtonEventArgs::MouseButtonEventArgs(MouseDevicePtr mouse, Int64 timestamp, MouseButton button):_button(button), MouseEventArgs(mouse, timestamp)
{

}

MouseButton MouseButtonEventArgs::GetChangedButton() const
{
	return _button;
}

MouseButtonState MouseButtonEventArgs::GetButtonState() const
{
	return MouseButtonState::Released;
}

int MouseButtonEventArgs::GetClickCount()const
{
	return _count;
}

void MouseButtonEventArgs::InvokeEventHandler(IRoutedEventDelegatePtr generichandler, DependencyObjectPtr sender)
{
	std::shared_ptr<MouseButtonEventHandler> handler = std::dynamic_pointer_cast<MouseButtonEventHandler>(generichandler);
	if (handler)
	{
		handler->Invoke(sender, *this);
	}
}


MouseDevice::MouseDevice():impl(nullptr)
{
	/*need*/
}

MouseDevice::~MouseDevice()
{

}

Point MouseDevice::GetPosition(IInputElementPtr relativeTo) const
{
	try
	{
		HwndHostPtr target = Target;
		if (target)
		{
			VisualPtr visual = std::dynamic_pointer_cast<Visual>(relativeTo);
			if (visual)
			{
				POINT cp;
				if (GetCursorPos(&cp))
				{
					return target->PointFromScreen(Point(cp.x, cp.y));
				}
			}
		}
	}
	catch (...)
	{

	}
	return Point();
}

void MouseDevice::OnProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LONG msgTime)
{
	HwndHostPtr target = Target;
	if (!target) return;
	if (target->GetHwnd() != window) return;
	switch (message)
	{
		case WM_MOUSEMOVE:
		{
			double x=  GET_X_LPARAM(lparam);
			double y = GET_Y_LPARAM(lparam);
			Point p(x, y);
			VisualPtr visual;
			target->HitTestTree(p, visual);
			if (visual)
			{
				MouseEventArgs arg;
				SetInputDeviceTimeStamp(arg, msgTime);
				arg.SetRoutedEvent(Mouse::PreviewMouseMoveEvent);
				visual->RaiseEvent(arg);
			}
		}
	}
}

MouseWheelEventArgs::MouseWheelEventArgs() : MouseEventArgs(), _delta(0)
{

}

MouseWheelEventArgs::MouseWheelEventArgs(MouseDevicePtr mouse, Int64 timestamp, int delta) : MouseEventArgs(mouse, timestamp), _delta(delta)
{

}

int MouseWheelEventArgs::GetDelta() const
{
	return _delta;
}

void MouseWheelEventArgs::InvokeEventHandler(IRoutedEventDelegatePtr generichandler, DependencyObjectPtr sender)
{
	std::shared_ptr<MouseWheelEventHandler> handler = std::dynamic_pointer_cast<MouseWheelEventHandler>(generichandler);
	if (handler)
	{
		handler->Invoke(sender, *this);
	}
}


const RoutedEventPtr Mouse::PreviewMouseMoveEvent = make_routedevent<Mouse>(); 
const RoutedEventPtr Mouse::MouseMoveEvent = make_routedevent<Mouse>();



__usctor(Mouse)
{
	Mouse::PreviewMouseMoveEvent.get()->Register<Mouse, MouseEventArgs>(L"PreviewMouseMove", EventStrategy::Tunnel);
	Mouse::MouseMoveEvent.get()->Register<Mouse, MouseEventArgs>(L"MouseMove", EventStrategy::Bubble);
	Mouse::PreviewMouseDownEvent.get()->Register<Mouse, MouseButtonEventArgs>(L"PreviewMouseDown", EventStrategy::Tunnel);
	Mouse::MouseDownEvent.get()->Register<Mouse, MouseButtonEventArgs>(L"MouseDown", EventStrategy::Bubble);
	Mouse::PreviewMouseUpEvent.get()->Register<Mouse, MouseButtonEventArgs>(L"PreviewMouseUp", EventStrategy::Tunnel);
	Mouse::MouseUpEvent.get()->Register<Mouse, MouseButtonEventArgs>(L"MouseUp", EventStrategy::Bubble);
	Mouse::PreviewMouseWheelEvent.get()->Register<Mouse, MouseWheelEventArgs>(L"PreviewMouseWheel", EventStrategy::Tunnel);
	Mouse::MouseWheelEvent.get()->Register<Mouse, MouseWheelEventArgs>(L"MouseWheel", EventStrategy::Bubble);

	Mouse::MouseEnterEvent.get()->Register<Mouse, MouseEventArgs>(L"MouseEnter", EventStrategy::Direct);
	Mouse::MouseLeaveEvent.get()->Register<Mouse, MouseEventArgs>(L"MouseLeave", EventStrategy::Direct);

	Mouse::GotMouseCaptureEvent.get()->Register<Mouse, MouseEventArgs>(L"GotMouseCapture", EventStrategy::Bubble);
	Mouse::LostMouseCaptureEvent.get()->Register<Mouse, MouseEventArgs>(L"LostMouseCapture", EventStrategy::Bubble);


	return true;
}


