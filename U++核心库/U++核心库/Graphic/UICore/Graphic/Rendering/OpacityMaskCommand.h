#pragma once
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
				class OpacityMaskCommand
				{
				public:
					OpacityMaskCommand(const Rect& rec, const BrushPtr& brush, RenderContext* renderContext)
					{
						m_renderContext = renderContext;
						m_success = m_renderContext->PushOpacityMask(rec, brush) == GraphicError::OK;
					}

					~OpacityMaskCommand()
					{
						if(m_success) m_renderContext->PopOpacityMask();
					}
				private:
					bool m_success;
					RenderContext* m_renderContext;
				};
			}
		}
	}
}

