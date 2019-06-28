#include "BezierSegment.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;


void BezierSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
	sink->AddBezier(D2D1::BezierSegment(D2DPoint(m_P1), D2DPoint(m_P2), D2DPoint(m_P3)));
    Invalidate();
}

void BezierSegment::Decompose(wstring& result)
{
	result.push_back('C');
	result.append(m_P1.ToString());
	result.push_back(',');
	result.append(m_P2.ToString());
	result.push_back(',');
	result.append(m_P3.ToString());
	result.push_back(' ');
}