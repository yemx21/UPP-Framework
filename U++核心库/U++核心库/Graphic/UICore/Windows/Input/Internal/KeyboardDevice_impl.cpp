#include "KeyboardDevice_impl.h"

using namespace System::UI;
using namespace System::UI::Windows::Input;

bool KeyboardDevice_impl::IsDown(unsigned char* keyStates, Key key)
{
	return (GetKeyStates(keyStates, key) & KeyStates::Down) != 0;
}


KeyStates KeyboardDevice_impl::GetKeyStates(unsigned char* keyStates, Key key)
{
	int vk = KeyInterop::VirtualKeyFromKey(key);
	byte state = keyStates[vk];
	KeyStates result = KeyStates::None;

	if ((state & 0x80) != 0)
	{
		result |= KeyStates::Down;
	}

	if ((state & 0x01) != 0)
	{
		result |= KeyStates::Toggled;
	}

	return result;
}