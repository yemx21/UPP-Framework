#include "ArcSegment.h"
using namespace System;
using namespace System::UI::Graphic::d2d1;


ArcSegment::ArcSegment(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize) :
     m_point(point), m_size(size), m_rotation(rotation), m_sweepDirection(sweepDirection), m_arcSize(arcSize)
{
}


ArcSegment::~ArcSegment(void)
{
}

void ArcSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
	sink->AddArc(D2D1::ArcSegment(D2DPoint(m_point), D2DSize(m_size), m_rotation, (D2D1_SWEEP_DIRECTION)(UInt)m_sweepDirection, (D2D1_ARC_SIZE)(UInt)m_arcSize));
}
void ArcSegment::Decompose(wstring& result)
{
	result.push_back('A');
	result.append(m_size.ToString());
	result.push_back(',');
	result.append(Formatter::Format(L"r",m_rotation));
	result.push_back(',');
	result.push_back(m_arcSize == ArcSize::Large ? '1' : '0');
	result.push_back(',');
	result.push_back(m_sweepDirection == SweepDirection::Clockwise ? '1' : '0');
	result.append(m_point.ToString());
	result.push_back(' ');
}
