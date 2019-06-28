#pragma once
#include "RenderContext.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class ClipCommand
				{
				public:
					ClipCommand(bool clipEnabled, Rect* clipRect, RenderContext* renderContext)
					{
						m_renderContext = renderContext;
						m_clipEnabled = clipEnabled;

						if (clipEnabled)
							m_renderContext->BeginClip(*clipRect);
					}

					~ClipCommand()
					{
						if (m_clipEnabled)
							m_renderContext->EndClip();

						m_clipEnabled = false;
					}
				private:
					bool            m_clipEnabled;
					RenderContext* m_renderContext;
				};
			}
		}
	}
}