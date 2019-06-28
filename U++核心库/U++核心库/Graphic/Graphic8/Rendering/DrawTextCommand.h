#pragma once
#include "..\Text\TextLayout.h"
#include "..\Brush\Brush.h"
#include "ExecutableBase.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class DrawTextCommand : public ExecutableBase
				{
					typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext>  ID2D1DeviceContextPtr;
				public:
					DrawTextCommand(void);
					virtual ~DrawTextCommand(void);
				public:
					TextLayoutPtr TextLayout() const { return m_textLayout; }
					void TextLayout(const TextLayoutPtr& val) { m_textLayout = val; }

					BrushPtr Brush() const { return m_brush; }
					void Brush(const BrushPtr& val) { m_brush = val; }

					Point Origin() const { return m_origin; }
					void Origin(Point val) { m_origin = val; }

					ID2D1DeviceContextPtr DeviceContext() const { return m_deviceContext; }
					void DeviceContext(const ID2D1DeviceContextPtr& val) { m_deviceContext = val; }

					virtual void Run();
				private:
					Point         m_origin;
					BrushPtr      m_brush;
					TextLayoutPtr m_textLayout;
					ID2D1DeviceContextPtr  m_deviceContext;
				};
			}
		}
	}
}


