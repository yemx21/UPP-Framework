#pragma once
#include "..\..\EventManager.h"
#include "InputDevice.h"
namespace System
{
	namespace UI
	{
		namespace Windows
		{
			class Window_impl;

			namespace Input
			{
				__uenum __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API MouseButtonState : __uobject
				{
					__uvalue_begin(MouseButtonState, UInt)
					__uvalue(Released)
					__uvalue(Pressed)
					__uvalue_end(MouseButtonState, Released)
				};

				__uenum __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API MouseButton : __uobject
				{
					__uvalue_begin(MouseButton, UInt)
					__uvalue(Left)
					__uvalue(Middle)
					__uvalue(Right)
					__uvalue(XButton1)
					__uvalue(XButton2)
					__uvalue_end(MouseButton, Left)

					static void Validate(MouseButton button);
				};

				class MouseDevice_impl;
				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API MouseDevice: public InputDevice
				{
				private:
					friend Window_impl;
					friend MouseDevice_impl;
					MouseDevice_impl* impl;
				protected:
					void OnProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LONG msgTime) override;
				public:
					__udefctor MouseDevice();
					~MouseDevice();
					Point GetPosition(IInputElementPtr relativeTo) const;

					REFLECT_CLASS(MouseDevice)
				};
				typedef std::shared_ptr<MouseDevice> MouseDevicePtr;

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API MouseEventArgs : public InputEventArgs
				{
				public:
					__udefctor MouseEventArgs();
					__uctor MouseEventArgs(MouseDevicePtr mouse, Int64 timestamp);
					virtual ~MouseEventArgs();

					MouseButtonState GetLeftButton() const;

					MouseButtonState GetMiddleButton() const;

					MouseButtonState GetRightButton() const;

					MouseButtonState GetXButton1() const;

					MouseButtonState GetXButton2() const;

					Point GetPosition(IInputElementPtr relativeTo) const;

					__uproperty(get = GetLeftButton) MouseButtonState LeftButton;
					__uproperty(get = GetMiddleButton) MouseButtonState MiddleButton;
					__uproperty(get = GetRightButton) MouseButtonState RightButton;
					__uproperty(get = GetXButton1) MouseButtonState XButton1;
					__uproperty(get = GetXButton2) MouseButtonState XButton2;

					REFLECT_CLASS(MouseEventArgs)
				};

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API MouseButtonEventArgs : public MouseEventArgs
				{
				private:
					MouseButton _button;
					int _count;
				public:
					__udefctor MouseButtonEventArgs();
					__uctor MouseButtonEventArgs(MouseDevicePtr mouse, Int64 timestamp, MouseButton button);
					
					MouseButton GetChangedButton() const;
					MouseButtonState GetButtonState() const;
					int GetClickCount()const;
					
					__uproperty(get = GetChangedButton) MouseButton ChangedButton;
					__uproperty(get = GetButtonState) MouseButtonState ButtonState;
					__uproperty(get = GetClickCount) int ClickCount;
				protected:
					void InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender) override;

					REFLECT_CLASS(MouseButtonEventArgs)
				};

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API MouseWheelEventArgs : public MouseEventArgs
				{
				private:
					int _delta;
				public:
					__udefctor MouseWheelEventArgs();
					__uctor MouseWheelEventArgs(MouseDevicePtr mouse, Int64 timestamp, int delta);

					int GetDelta() const;

				protected:
					void InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender) override;

					REFLECT_CLASS(MouseWheelEventArgs)
				};


				typedef RoutedEventHandler<MouseButtonEventArgs>  MouseButtonEventHandler;
				typedef RoutedEventHandler<MouseEventArgs>  MouseEventHandler;
				typedef RoutedEventHandler<MouseWheelEventArgs>  MouseWheelEventHandler;

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API  Mouse : public DependencyObject
				{
				public:
					static const RoutedEventPtr PreviewMouseMoveEvent;
					static const RoutedEventPtr MouseMoveEvent;
					static const RoutedEventPtr PreviewMouseDownEvent;
					static const RoutedEventPtr MouseDownEvent;
					static const RoutedEventPtr PreviewMouseUpEvent;
					static const RoutedEventPtr MouseUpEvent;
					static const RoutedEventPtr PreviewMouseWheelEvent;
					static const RoutedEventPtr MouseWheelEvent;

					static const RoutedEventPtr MouseEnterEvent;
					static const RoutedEventPtr MouseLeaveEvent;

					static const RoutedEventPtr GotMouseCaptureEvent;
					static const RoutedEventPtr LostMouseCaptureEvent;

					__usctordef(Mouse)
					REFLECT_CLASS(Mouse)
				};
			}
		}
	}
}