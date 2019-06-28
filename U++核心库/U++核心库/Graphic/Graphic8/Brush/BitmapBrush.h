#pragma once
#include "Brush.h"
#include "..\Media\Bitmap.h"
#include "..\Geometry\Geometry.h"
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

				class BitmapBrush;
				typedef std::shared_ptr<BitmapBrush> BitmapBrushPtr;

				class BitmapBrush : public Brush
				{
				public:
					BitmapBrush(const BitmapPtr& image);
					BitmapBrush(const BitmapPtr& image, ExtendMode modeX, ExtendMode modeY, Rect sourceRect, InterpolationMode interpolation);
					BitmapBrush();
					virtual ~BitmapBrush(void);

					auto ExtendModeX() const->ExtendMode;
					void ExtendModeX(ExtendMode extendMode);

					auto ExtendModeY() const->ExtendMode;
					void ExtendModeY(ExtendMode extendMode);

					auto SourceRect() const->Rect;
					void BitmapBrush::SourceRect(const Rect& rect);

					auto Image()->BitmapPtr;
					void Image(const BitmapPtr& image);

					auto Interpolation() const->InterpolationMode;
					void Interpolation(InterpolationMode mode);

					virtual auto GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen)->Matrix override;
					virtual auto GetTransformToGeometryBounds(const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, float thickness)->Matrix override;
					virtual auto GetTransformToBounds(const Rect& rect)->Matrix override;
				protected:
					auto IsImageSet() -> bool;
					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr<IUnknown> override;
				private:
					BitmapPtr             m_image;
					InterpolationMode    m_interpolationMode;
					ExtendMode           m_extendModeX;
					ExtendMode           m_extendModeY;
					mutable Rect m_sourceRect;
				};
			}
		}
	}
}
