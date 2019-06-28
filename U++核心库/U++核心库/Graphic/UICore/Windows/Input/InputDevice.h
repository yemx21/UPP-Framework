#pragma once
#include "..\..\RoutedEvent.h"
namespace System
{
	namespace UI
	{
		namespace Windows
		{
			class Window_impl;
			class HwndHost;
			typedef std::shared_ptr<HwndHost> HwndHostPtr;
			class IInputElement;
			typedef std::shared_ptr<IInputElement> IInputElementPtr;

			namespace Input
			{
				class InputDevice;
				typedef std::shared_ptr<InputDevice> InputDevicePtr;

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API InputEventArgs : public RoutedEventArgs
				{
				private:
					friend InputDevice;
					Int64 _timeStamp;
					InputDevicePtr _inputDevice;
				public:
					__udefctor InputEventArgs();
					__uctor InputEventArgs(Int64 timeStamp);
					__uctor InputEventArgs(InputDevicePtr device, Int64 timeStamp);
					__uctor InputEventArgs(const InputEventArgs&);
					virtual ~InputEventArgs();

					Int64 GetTimeStamp() const;

					InputDevicePtr GetDevice() const;

					__uproperty(get = GetDevice) InputDevicePtr Device;
					__uproperty(get = GetTimeStamp) Int64 TimeStamp;

					REFLECT_CLASS(InputEventArgs)
				};

				__uclass __uobjdef(asm = UICore, ns = System::UI::Windows::Input, ver = 1.0) UICORE_API InputDevice : __uobject, std::enable_shared_from_this<InputDevice>
				{
				private:
					friend HwndHost;
					friend Window_impl;
					HwndHostPtr AttachTarget;
					EventToken ProcessToken;
					bool IsAttached(HwndHostPtr);
					bool Attach(HwndHostPtr);
					bool Detach(HwndHostPtr);;
				protected:
					HwndHostPtr GetTarget() const;
					__declspec(property(get = GetTarget)) HwndHostPtr Target;
					InputDevice();
					virtual void OnProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LONG msgTime);
					static void OnProcessMessageCore(InputDevicePtr device, HWND window, UINT message, WPARAM wparam, LPARAM lparam);

					void SetInputDevice(InputEventArgs& eventArgs);
					void SetTimeStamp(InputEventArgs& eventArgs, LONG timeStamp);
					void SetInputDeviceTimeStamp(InputEventArgs& eventArgs, LONG timeStamp);
				public:
					virtual ~InputDevice();
					IInputElementPtr GetHost() const;
					__declspec(property(get = GetHost)) IInputElementPtr Host;

					REFLECT_CLASS(InputDevice)
				};
			}
		}
	}
}