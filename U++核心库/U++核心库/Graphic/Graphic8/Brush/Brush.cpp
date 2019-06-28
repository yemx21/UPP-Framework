#include "Brush.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;


Brush::Brush() : m_Opacity(1.0f), 
                         m_transform(Matrix::Identity())
{
}

auto Brush::GetTransformToGeometryBounds(const GeometryPtr& /*geometry*/, 
                                             const PenPtr& /*pen*/) -> Matrix
{
    return Matrix::Identity();
}

auto Brush::GetTransformToGeometryBounds(const GeometryPtr& /*geometry*/,
	const StrokeStylePtr& /*pen*/,  float thickness) -> Matrix
{
	return Matrix::Identity();
}

auto Brush::GetTransformToBounds(const Rect& /*rect*/) -> Matrix
{
    return Matrix::Identity();
}

void Brush::Opacity(const float o)
{
    m_Opacity = o;

    if(IsResourceValid())
    {
        GetResourceUnsafe<ID2D1Brush>()->SetOpacity(0);
    }
}

void Brush::Transform(const Matrix& transform)
{
    m_transform = transform;

    if(IsResourceValid())
    {
		D2D1_MATRIX_3X2_F tmp = D2DMatrix(transform);
		GetResourceUnsafe<ID2D1Brush>()->SetTransform(&tmp);
    }
}

auto Brush::Transform() const -> const Matrix&
{
    return m_transform;
}

auto Brush::Opacity() const -> float
{
    return m_Opacity;
}

