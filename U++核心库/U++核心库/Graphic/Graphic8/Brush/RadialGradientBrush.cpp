#include "RadialGradientBrush.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;

ComPtr<IUnknown> RadialGradientBrush::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
    ComPtr<ID2D1RadialGradientBrush> brush;
    auto gsc = GetResourceFrom<ID2D1GradientStopCollection>(m_gradientStopCollection);

	if (deviceContext->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties(D2DPoint(m_center),
		D2DPoint(m_gradientOriginOffset),
		m_radiusX,
		m_radiusY),
		D2D1::BrushProperties(m_Opacity, D2DMatrix(m_transform)),
		gsc,
		&brush) != S_OK)
	{
		brush = nullptr;
	}
    
    return brush;
}

Matrix RadialGradientBrush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen)
{
    Rect geometryBounds;
 
    geometryBounds = geometry->GetBounds(pen, Matrix::Identity());

    D2D1_RECT_F brushRect;
    brushRect.left = m_center.X- m_radiusX;
    brushRect.right = m_center.X+ m_radiusX;
    brushRect.top = m_center.Y - m_radiusY;
    brushRect.bottom = m_center.Y + m_radiusY;

    auto brushRectSize = D2D1::SizeF(brushRect.right - brushRect.left, brushRect.bottom - brushRect.top);


    auto widthRatio = (geometryBounds.Right - geometryBounds.Left) / brushRectSize.width;
    auto heightRatio = (geometryBounds.Bottom - geometryBounds.Top) / brushRectSize.height;

    auto sourceCenterX = m_center.X;
    auto sourceCenterY =  m_center.Y;

    auto destCenterX = (geometryBounds.Right + geometryBounds.Left) / 2;
    auto destCenterY =  (geometryBounds.Bottom + geometryBounds.Top) / 2;

    auto matrix = Matrix::CreateScaling(widthRatio, heightRatio, sourceCenterX, sourceCenterY);
//    matrix = matrix *  MatrixF::Translation((destCenterX - sourceCenterX) - widthRatio, (destCenterY - sourceCenterY) - heightRatio);
    matrix = matrix *  Matrix::CreateTranslation((destCenterX * 2) * sourceCenterX, (destCenterY * 2) * sourceCenterY);
    return matrix;
}

Matrix RadialGradientBrush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, float thickness)
{
	Rect geometryBounds;

	geometryBounds = geometry->GetBounds(strokeStyle, thickness, Matrix::Identity());

	D2D1_RECT_F brushRect;
	brushRect.left = m_center.X- m_radiusX;
	brushRect.right = m_center.X+ m_radiusX;
	brushRect.top = m_center.Y - m_radiusY;
	brushRect.bottom = m_center.Y + m_radiusY;

	auto brushRectSize = D2D1::SizeF(brushRect.right - brushRect.left, brushRect.bottom - brushRect.top);


	auto widthRatio = (geometryBounds.Right - geometryBounds.Left) / brushRectSize.width;
	auto heightRatio = (geometryBounds.Bottom - geometryBounds.Top) / brushRectSize.height;

	auto sourceCenterX = m_center.X;
	auto sourceCenterY = m_center.Y;

	auto destCenterX = (geometryBounds.Right + geometryBounds.Left) / 2;
	auto destCenterY = (geometryBounds.Bottom + geometryBounds.Top) / 2;

	auto matrix = Matrix::CreateScaling(widthRatio, heightRatio, sourceCenterX, sourceCenterY);
	//    matrix = matrix *  MatrixF::Translation((destCenterX - sourceCenterX) - widthRatio, (destCenterY - sourceCenterY) - heightRatio);
	matrix = matrix *  Matrix::CreateTranslation((destCenterX * 2) * sourceCenterX, (destCenterY * 2) * sourceCenterY);
	return matrix;
}

auto RadialGradientBrush::GetTransformToBounds(const Rect& rect) -> Matrix
{
    auto bounds = rect;

	auto size = D2D1::SizeF(1, 1);

    auto widthRatio = (bounds.Right - bounds.Left) / size.width;
    auto heightRatio = (bounds.Bottom - bounds.Top) / size.height;

	auto matrix = Matrix::CreateTranslation(bounds.Left, bounds.Top);
	matrix = matrix * Matrix::CreateScaling(widthRatio, heightRatio,bounds.Left, bounds.Top);
    return matrix;
}

auto RadialGradientBrush::GradientStopCollection() const -> const GradientStopCollectionPtr&
{
    return m_gradientStopCollection;
}

void RadialGradientBrush::GradientStopCollection(const GradientStopCollectionPtr& stopCollection)
{
	m_gradientStopCollection = stopCollection;
	Invalidate();
}
auto RadialGradientBrush::IsResourceValid() const -> bool
{
    if(!IsResourceValidFrom(m_gradientStopCollection))
        return false;

    return Brush::IsResourceValid();
}

void RadialGradientBrush::Center(Point& val)
{
    m_center = val; 

    if(IsResourceValid())
    {
        auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
		brush->SetCenter(D2DPoint(m_center));
    }
}

Point RadialGradientBrush::Center() const
{
    return m_center;
}

void RadialGradientBrush::GradientOriginOffset(Point& val)
{
    m_gradientOriginOffset = val; 
    if(IsResourceValid())
    {
        auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
		brush->SetGradientOriginOffset(D2DPoint(m_gradientOriginOffset));
    }
}

Point RadialGradientBrush::GradientOriginOffset() const
{
    return m_gradientOriginOffset;
}

void RadialGradientBrush::RadiusX(float val)
{
    m_radiusX = val;

    if(IsResourceValid())
    {
         auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
        brush->SetRadiusX(m_radiusX);
    }
}

float RadialGradientBrush::RadiusX() const
{
    return m_radiusX;
}

void RadialGradientBrush::RadiusY(float val)
{
    m_radiusY = val; 
    if(IsResourceValid())
    {
        auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
        brush->SetRadiusY(m_radiusY);
    }
}

float RadialGradientBrush::RadiusY() const
{
    return m_radiusY;
}
