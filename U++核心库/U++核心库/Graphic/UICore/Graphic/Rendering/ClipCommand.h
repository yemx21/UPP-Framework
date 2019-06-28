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
				class ClipCommand
				{
				public:
					ClipCommand(bool clipEnabled, const Rect& clipRect, RenderContext* renderContext)
					{
						m_renderContext = renderContext;
						m_clipEnabled = clipEnabled;

						if (clipEnabled)
						{
							m_success = m_renderContext->PushAxisAlignedClip(clipRect, AntialiasMode::PerPrimitive) == GraphicError::OK;
						}
					}

					~ClipCommand()
					{
						if (m_clipEnabled && m_success)
							m_renderContext->PopAxisAlignedClip();

						m_clipEnabled = false;
					}
				private:
					bool m_success;
					bool m_clipEnabled;
					RenderContext* m_renderContext;
				};
			}
		}
	}
}