#pragma once
#include "Action.h"

namespace System
{
	namespace Runtime
	{
		class INotifyCompletion
		{
		public:
			virtual void OnCompleted(const System::Action<>& continuation) = 0;
		};
	}
}