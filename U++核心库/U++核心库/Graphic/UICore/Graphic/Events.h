#pragma once
#include "..\Delegate.h"
using namespace System;

namespace System
{
	namespace UI
	{
		class SizeChangedEvent : public MulticastDelegate<Size>
		{

		};
	}
}

