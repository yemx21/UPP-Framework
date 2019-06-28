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
				class QuadraticBezierSegment : public ISegment
				{
				public:
					QuadraticBezierSegment(const Point& point1, const Point& point2);
					virtual ~QuadraticBezierSegment(void);
					virtual void Decompose(wstring& result) override;
				protected:
					virtual void Create(const ID2D1GeometrySinkPtr& sink) override;
				private:
					Point m_point1;
					Point m_point2;
				};
			}
		}
	}
}


