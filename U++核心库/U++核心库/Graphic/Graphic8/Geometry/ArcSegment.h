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
				class ArcSegment : public ISegment
				{
				public:
					ArcSegment(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize);
					virtual ~ArcSegment(void);
					virtual void Decompose(wstring& result) override;
				protected:
					virtual void Create(const ID2D1GeometrySinkPtr& sink) override;
				private:
					Point           m_point;
					Size            m_size;
					float           m_rotation;
					SweepDirection  m_sweepDirection;
					ArcSize         m_arcSize;
				};
			}
		}
	}
}
