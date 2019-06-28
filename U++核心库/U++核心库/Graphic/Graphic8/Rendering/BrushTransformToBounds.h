#pragma once
#include "..\Brush\Brush.h"
#include "..\Brush\Pen.h"
#include "..\Geometry\Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class BrushTransformToBounds
				{
				public:
					void Setup(Brush* brush, const GeometryPtr& geometry, const PenPtr& pen)
					{
						m_brush = brush;

						m_originalTransform = m_brush->Transform();

						auto boundsTransform = m_brush->GetTransformToGeometryBounds(geometry, pen);

						boundsTransform = boundsTransform * m_originalTransform;

						m_brush->Transform(boundsTransform);
					}

					void Setup(Brush* brush, const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, float thickness)
					{
						m_brush = brush;

						m_originalTransform = m_brush->Transform();

						auto boundsTransform = m_brush->GetTransformToGeometryBounds(geometry, strokeStyle, thickness);

						boundsTransform = boundsTransform * m_originalTransform;

						m_brush->Transform(boundsTransform);
					}

					BrushTransformToBounds() : m_brush(nullptr)
					{

					}

					~BrushTransformToBounds()
					{
						if (m_brush)
							m_brush->Transform(m_originalTransform);
					}
				private:
					Brush* m_brush;
					Matrix      m_originalTransform;
				};
			}
		}
	}
}