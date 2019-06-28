#include "Ellipse.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1Geometry> Ellipse::OnCreateGeometry()
{
    ComPtr<ID2D1EllipseGeometry> ellipse;
	if (m_Factory->CreateEllipseGeometry(D2D1::Ellipse(D2DPoint(m_Center), m_RX, m_RY), &ellipse) != S_OK)
	{
		ellipse = nullptr;
	}
    return ellipse;
}

void Ellipse::RadiusX(float rx)
{
    m_RX = rx;
    Invalidate();
}

auto Ellipse::RadiusX() const -> float
{
    return m_RX;
}

auto Ellipse::RadiusY() const -> float
{
    return m_RY;
}

void Ellipse::RadiusY(float ry)
{
    m_RY = ry;
    Invalidate();
}

void Ellipse::Center(const System::Point& center)
{
    m_Center = center;
    Invalidate();
}

auto Ellipse::Center() const -> System::Point
{
    return m_Center;
}
