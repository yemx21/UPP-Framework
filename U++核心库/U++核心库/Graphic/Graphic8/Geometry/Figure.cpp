#include "Figure.h"
#include <algorithm>
using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace std;

void Figure::AddSegment(const ISegmentPtr& s, bool isStroked, bool isSmoothJoined)
{
    m_Segments.emplace_back(SegmentData(s, isStroked, isSmoothJoined));
}

void Figure::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->BeginFigure(D2D1::Point2F(m_StartPoint.X, m_StartPoint.Y), m_filled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

    for (auto& segmentData : m_Segments)
    {
        auto flags = D2D1_PATH_SEGMENT_NONE;

        if(!segmentData.isStroked)
            flags |= D2D1_PATH_SEGMENT_FORCE_UNSTROKED;

        if(segmentData.isSmoothJoined)
            flags |= D2D1_PATH_SEGMENT_FORCE_ROUND_LINE_JOIN;

        
        sink->SetSegmentFlags(flags);

        segmentData.segment->Create(sink);
    }

    D2D1_FIGURE_END end = m_closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN;
    sink->EndFigure(end);
}
