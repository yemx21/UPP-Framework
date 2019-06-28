#include "Rectangle.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace D2D1;
using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1Geometry> Rectangle::OnCreateGeometry()
{
	ComPtr<ID2D1RectangleGeometry> geometry;

	m_Factory->CreateRectangleGeometry(D2DRect(m_rect), &geometry);

	return geometry;
}


void Rectangle::Rect(const System::Rect& rect)
{
	m_rect = rect;
	Invalidate();
}

auto Rectangle::Rect() const -> System::Rect
{
	return m_rect;
}
