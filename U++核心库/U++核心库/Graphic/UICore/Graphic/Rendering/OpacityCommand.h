#pragma once
#include "..\GraphicFactory.h"
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace Internal
			{
				class OpacityCommand
				{
				public:
					OpacityCommand(float opac, RenderContext* renderContext)
					{
						m_renderContext = renderContext;
						m_success = m_renderContext->PushOpacity(opac) == GraphicError::OK;
					}

					~OpacityCommand()
					{
						if(m_success) m_renderContext->PopOpacity();
					}
				private:
					bool m_success;
					RenderContext* m_renderContext;
				};
			}
		}
	}
}

