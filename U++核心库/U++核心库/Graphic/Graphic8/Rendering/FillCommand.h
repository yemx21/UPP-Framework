#pragma once
#include "DrawCommand.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class FillCommand : public DrawCommand
				{
				public:
					FillCommand() {}
					virtual ~FillCommand() {}
				public:
					virtual void Run();
				};
			}
		}
	}
}
