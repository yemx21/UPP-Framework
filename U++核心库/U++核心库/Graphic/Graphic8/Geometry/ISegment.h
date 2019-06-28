#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Figure;

				class ISegment
				{
				public:
					ISegment() : m_isDirty(false) {}
					virtual void Decompose(wstring& result) = 0;
					virtual ~ISegment() {}
				protected:
					virtual void Create(const ID2D1GeometrySinkPtr& sink) = 0;
				private:
					bool m_isDirty;
				protected:
					void Invalidate()
					{
						m_isDirty = true;
					}
					auto IsValid() -> bool
					{
						return !m_isDirty;
					}
					friend Figure;
				};
			}
		}
	}
}
