#include "LineSegment.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;


void LineSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->AddLine(D2DPoint(m_P));
    Invalidate();
}

void LineSegment::Decompose(wstring& result)
{
	result.push_back('M');
	result.push_back(' ');
	result.append(m_P.ToString());
	result.push_back(' ');
}