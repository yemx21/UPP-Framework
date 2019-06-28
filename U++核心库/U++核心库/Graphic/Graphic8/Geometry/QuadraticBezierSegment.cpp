#include "QuadraticBezierSegment.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;


QuadraticBezierSegment::QuadraticBezierSegment(const Point& point1, const Point& point2)
    : m_point1(point1), m_point2(point2)
{
}


QuadraticBezierSegment::~QuadraticBezierSegment(void)
{
}

void QuadraticBezierSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(D2DPoint(m_point1), D2DPoint(m_point2)));
}

void QuadraticBezierSegment::Decompose(wstring& result)
{
	result.push_back('Q');
	result.append(m_point1.ToString());
	result.push_back(',');
	result.append(m_point2.ToString());
	result.push_back(' ');
}