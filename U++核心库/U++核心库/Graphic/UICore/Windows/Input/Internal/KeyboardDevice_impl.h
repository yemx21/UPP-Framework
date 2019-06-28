#pragma once
#include "..\Keyboard.h"

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Input
			{
				class KeyboardDevice_impl
				{
				public:
					static bool IsDown(unsigned char* keyStates, Key key);
					static KeyStates GetKeyStates(unsigned char* keyStates, Key key);
				};
			}
		}
	}
}
