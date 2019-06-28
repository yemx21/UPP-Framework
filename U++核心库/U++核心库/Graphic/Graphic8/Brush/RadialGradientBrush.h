#pragma once
#include "..\Resource.h"
#include "Brush.h"
#include "GradientStopCollection.h"
#include "Pen.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class RadialGradientBrush;
				typedef std::shared_ptr<RadialGradientBrush> RadialGradientBrushPtr;

				class RadialGradientBrush : public Brush
				{
				public:
					RadialGradientBrush(
						GradientStopCollectionPtr gsc, Point center, Point gradientOriginOffset, float radiusX, float radiusY)
						: m_gradientStopCollection(gsc),
						m_center(center),
						m_gradientOriginOffset(gradientOriginOffset),
						m_radiusX(radiusX),
						m_radiusY(radiusY)
					{
					}

					virtual ~RadialGradientBrush() {}
				public:
					auto GradientStopCollection() const -> const GradientStopCollectionPtr&;
					void GradientStopCollection(const GradientStopCollectionPtr& stopCollection);

					Point Center() const;
					void Center(Point& val);

					Point GradientOriginOffset() const;
					void GradientOriginOffset(Point& val);

					float RadiusX() const;
					void RadiusX(float val);

					float RadiusY() const;
					void RadiusY(float val);

					virtual auto GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen)->Matrix override;
					virtual auto GetTransformToGeometryBounds(const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, float thickness)->Matrix override;
					virtual auto GetTransformToBounds(const Rect& rect)->Matrix override;
				protected:
					virtual auto IsResourceValid() const -> bool override;
					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr<IUnknown> override;
				private:
					Point                     m_center;
					Point                     m_gradientOriginOffset;
					float                     m_radiusX;
					float                     m_radiusY;
					GradientStopCollectionPtr m_gradientStopCollection;
				};
			}
		}
	}
}
