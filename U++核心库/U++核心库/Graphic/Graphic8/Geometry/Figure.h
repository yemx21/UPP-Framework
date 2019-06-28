#pragma once
#include "ISegment.h"
#include "Geometry.h"
#include <vector>

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Figure;
				typedef std::shared_ptr<Figure> FigurePtr;

				class PathGeometry;
				class StreamGeometry;

				class Figure
				{
				public:
					Figure(const Point& start, bool filled, bool closed) : m_StartPoint(start),
						m_filled(filled),
						m_closed(closed)
					{
					}
					~Figure() {}
				public:
					void AddSegment(const ISegmentPtr& segment, bool isStroked = true, bool isSmoothJoined = true);
				private:
					void Create(const ID2D1GeometrySinkPtr& sink);
				private:
					struct SegmentData
					{
						SegmentData(ISegmentPtr s, bool stroked, bool smoothJoined) :
							segment(s), isSmoothJoined(smoothJoined), isStroked(stroked)
						{
						}
						ISegmentPtr segment;
						bool isStroked;
						bool isSmoothJoined;
					};
				private:
					std::vector<SegmentData> m_Segments;
					bool m_closed;
					bool m_filled;
					Point m_StartPoint;
					friend PathGeometry;
					friend StreamGeometry;
				};
			}
		}
	}
}
