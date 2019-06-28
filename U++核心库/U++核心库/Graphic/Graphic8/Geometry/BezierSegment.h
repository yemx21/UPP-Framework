#pragma once
#include "ISegment.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class BezierSegment : public ISegment
				{
				public:
					BezierSegment(const Point& p1, const Point& p2, const Point& p3) : m_P1(p1), m_P2(p2), m_P3(p3) {}
					virtual ~BezierSegment() {}
					virtual void Decompose(wstring& result) override;
				protected:
					virtual void Create(const ID2D1GeometrySinkPtr& sink);
				private:
					Point m_P1;
					Point m_P2;
					Point m_P3;
				};
			}
		}
	}
}
