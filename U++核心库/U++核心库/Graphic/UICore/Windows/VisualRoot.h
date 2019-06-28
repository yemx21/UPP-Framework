#pragma once
#include "Visual.h"

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			class VisualRoot;
			typedef std::shared_ptr<VisualRoot> VisualRootPtr;

			class VisualRoot
			{
			public:
				VisualRoot(const RenderContextPtr& renderContext);
				Result<bool> Render();
				void SetRootElement(const VisualPtr& elementRoot);
				VisualPtr GetRootElement() const { return m_elementRoot; }
				const RenderContextPtr& GetRenderContext() const;
				void SetRenderContext(const RenderContextPtr& context);
				void ForceDirty();
				bool IsSceneDirty() const;
			public:
				void HitTestTree(const Point& point, VisualPtr& visualElementResult);
			private:
				bool m_forceDirty;
				RenderContextPtr m_renderContext;
				VisualPtr m_elementRoot;
				GraphicObject  m_lastRenderTarget;
			protected:
				friend HwndHost;
				void RaiseEvent(RoutedEventArgs& args);
			};
		}
	}
}