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
				class GeometryClipCommand
				{
				public:
					GeometryClipCommand(const GeometryPtr& geo, RenderContext* renderContext)
					{
						m_renderContext = renderContext;
						m_success = m_renderContext->PushGeometryClip(geo) == GraphicError::OK;

					}

					~GeometryClipCommand()
					{
						if(m_success) m_renderContext->PopGeometryClip();
					}
				private:
					bool m_success;
					RenderContext* m_renderContext;
				};
			}
		}
	}
}
