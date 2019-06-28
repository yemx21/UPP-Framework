#pragma once
#include "Async.h"
#include <thread>

namespace System
{
	namespace Threading
	{
		typedef std::thread Thread;
		typedef std::shared_ptr<Thread> ThreadPtr;

	}
}