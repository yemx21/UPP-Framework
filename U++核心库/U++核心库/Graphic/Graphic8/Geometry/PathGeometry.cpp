#include "PathGeometry.h"
#include "LineSegment.h"
#include "ArcSegment.h"
#include "BezierSegment.h"
#include "QuadraticBezierSegment.h"
#include "..\..\UICore\Graphic\Geometry\PathFigure.h"
#include "..\..\UICore\Graphic\Geometry\ArcSegment.h"
#include "..\..\UICore\Graphic\Geometry\BezierSegment.h"
#include "..\..\UICore\Graphic\Geometry\LineSegment.h"
#include "..\..\UICore\Graphic\Geometry\QuadraticBezierSegment.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;
using namespace std;

d2d1::PathGeometry::PathGeometry(void) : m_fillMode(FillMode::Alternate)
{
}


d2d1::PathGeometry::~PathGeometry(void)
{
}

void d2d1::PathGeometry::SetFigures(const Graphic::PathFigurePtr* figures, unsigned int count)
{
	m_count = count;
	m_figures = figures;
}

Microsoft::WRL::ComPtr<ID2D1Geometry> d2d1::PathGeometry::OnCreateGeometry()
{
    ComPtr<ID2D1PathGeometry> pathGeometry;
    ComPtr<ID2D1GeometrySink> sink;

    m_Factory->CreatePathGeometry(&pathGeometry);

    pathGeometry->Open(&sink);
    sink->SetFillMode((D2D1_FILL_MODE)(UInt)m_fillMode);

	for (unsigned int i = 0; i < m_count; i++)
    {
		Graphic::PathFigurePtr figure = m_figures[i];
		if (figure)
		{
			if (figure->Segments.size>0)
			{
				sink->BeginFigure(d2d1::D2DPoint(figure->StartPoint), figure->IsFilled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

				for (auto& seg : figure->Segments)
				{
					if (seg)
					{
						auto flags = D2D1_PATH_SEGMENT_NONE;

						if (!seg->IsStroked)
							flags |= D2D1_PATH_SEGMENT_FORCE_UNSTROKED;

						if (seg->IsSmoothJoin)
							flags |= D2D1_PATH_SEGMENT_FORCE_ROUND_LINE_JOIN;

						sink->SetSegmentFlags(flags);

						const Type* ty = seg->GetType();
						if (ty ==  _typeof(Graphic::ArcSegment))
						{
							Graphic::ArcSegmentPtr arc = dynamic_pointer_cast<Graphic::ArcSegment, Graphic::PathSegment>(seg);
							if (arc)
							{
								sink->AddArc(D2D1::ArcSegment(d2d1::D2DPoint(arc->Point), d2d1::D2DSize(arc->Size), arc->RotationAngle, (D2D1_SWEEP_DIRECTION)(UInt)arc->SweepDirection, (D2D1_ARC_SIZE)(UInt)arc->ArcSize));
							}
						}
						else if (ty == _typeof(Graphic::BezierSegment))
						{
							Graphic::BezierSegmentPtr bezier = dynamic_pointer_cast<Graphic::BezierSegment, Graphic::PathSegment>(seg);
							if (bezier)
							{
								sink->AddBezier(D2D1::BezierSegment(d2d1::D2DPoint(bezier->Point1), d2d1::D2DPoint(bezier->Point2), d2d1::D2DPoint(bezier->Point3)));
							}
						}
						else if (ty == _typeof(Graphic::LineSegment))
						{
							Graphic::LineSegmentPtr line = dynamic_pointer_cast<Graphic::LineSegment, Graphic::PathSegment>(seg);
							if (line)
							{
								sink->AddLine(d2d1::D2DPoint(line->Point));
							}
						}
						else if (ty == _typeof(Graphic::QuadraticBezierSegment))
						{
							Graphic::QuadraticBezierSegmentPtr bezier = dynamic_pointer_cast<Graphic::QuadraticBezierSegment, Graphic::PathSegment>(seg);
							if (bezier)
							{
								sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(d2d1::D2DPoint(bezier->Point1), d2d1::D2DPoint(bezier->Point2)));
							}
						}
					}
				}
				D2D1_FIGURE_END end = figure->IsClosed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN;
				sink->EndFigure(end);
			}
		}
    }

    HRESULT hr = sink->Close();
	if (hr != S_OK)
	{
		sink = nullptr;
		pathGeometry = nullptr;
		return nullptr;
	}
    return pathGeometry;
}

void d2d1::PathGeometry::FillMode(Graphic::FillMode mode)
{
    if(m_fillMode == mode)
        return;

    m_fillMode = mode;
    Invalidate();
}

Graphic::FillMode d2d1::PathGeometry::FillMode()
{
    return m_fillMode;
}
