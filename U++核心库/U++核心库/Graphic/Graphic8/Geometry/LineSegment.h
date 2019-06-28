#pragma once
#include "Geometry.h"
#include "ISegment.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class LineSegment : public ISegment
				{
				public:
					explicit LineSegment(const Point& p) : m_P(p) {}
					virtual ~LineSegment() {}
					virtual void Decompose(wstring& result) override;
				protected:
					virtual void Create(const ID2D1GeometrySinkPtr& sink) override;
				private:
					Point m_P;
				};
			}
		}
	}
}
