#pragma once
#include "..\Brush\Pen.h"
#include <map>
#include "..\Resource.h"
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				typedef Microsoft::WRL::ComPtr<ID2D1Bitmap>       ID2D1BitmapPtr;
				typedef Microsoft::WRL::ComPtr<ID2D1GeometrySink> ID2D1GeometrySinkPtr;
				typedef Microsoft::WRL::ComPtr<ID2D1Factory>      ID2D1FactoryPtr;

				class RenderContext;

				class Figure;
				typedef std::shared_ptr<Figure>       FigurePtr;

				class ISegment;
				typedef std::shared_ptr<ISegment>     ISegmentPtr;

				class Geometry;
				typedef std::shared_ptr<Geometry> GeometryPtr;

				struct MaskData;
				typedef std::shared_ptr<MaskData>     MaskDataPtr;

				class Geometry : public IndependantResource
				{
				public:
					typedef std::map<float, MaskDataPtr> MaskDataPtrMap;

					Geometry();
					virtual ~Geometry() {}
				public:
					auto GetFillMask(ID2D1DeviceContext* rt)->ID2D1BitmapPtr;
					auto GetFillMaskTransform()       const -> const Matrix&;

					auto GetStrokeMask(ID2D1DeviceContext* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle)->ID2D1BitmapPtr;
					auto GetStrokeMaskTransform(float strokeWidth) -> const Matrix;

					bool HitTest(const Point& point, const Matrix& matrix = Matrix::Identity());
					bool HitTestStroke(const Point& point, const PenPtr& pen, const Matrix& matrix = Matrix::Identity());
					bool HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness, const Matrix& matrix);

					auto GetBounds(const PenPtr& pen, const Matrix& worldTransform)->Rect;
					auto GetBounds(const StrokeStylePtr& strokeStyle, float thickness, const Matrix& worldTransform)->Rect;

					void GeometryCacheEnabled(bool enabled);
					bool GeometryCacheEnabled();

					void Transform(const TransformPtr& transform);

				protected:
					ID2D1FactoryPtr m_Factory;
				protected:
					virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource() final;
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() = 0;
				private:
					auto CreateFillMask(ID2D1DeviceContext* rt)->MaskDataPtr;
					auto CreateStrokeMask(ID2D1DeviceContext* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle)->MaskDataPtr;
				private:
					mutable MaskDataPtr    m_FillMask;
					mutable MaskDataPtrMap m_StrokeMasks;
					static const int       m_MaskUsageThreshold = 1000;
					bool                   m_geometryCacheEnabled;
					friend RenderContext;
					Matrix m_transform;
				};

				struct MaskData
				{
					MaskData(ID2D1BitmapPtr mask, Matrix transform) : Mask(mask),
						Transform(transform)
					{
					}
					ID2D1BitmapPtr Mask;
					Matrix Transform;
				private:
					// non copyable
					MaskData(const MaskData&);
					MaskData& operator = (const MaskData&);
				};
			}
		}
	}
}