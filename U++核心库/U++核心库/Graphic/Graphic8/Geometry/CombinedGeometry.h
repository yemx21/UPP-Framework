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
				class CombinedGeometry;
				typedef std::shared_ptr<CombinedGeometry> CombinedGeometryPtr;

				class CombinedGeometry : public Geometry
				{
				public:
					CombinedGeometry(GeometryPtr g1, GeometryPtr g2, Graphic::CombineMode mode)
						: m_Geometry1(g1), m_Geometry2(g2), m_Mode(mode)
					{
					}
					virtual ~CombinedGeometry() {}


					void CombineMode(Graphic::CombineMode mode)
					{
						m_Mode = mode;
						Invalidate();
					}

					Graphic::CombineMode CombineMode()
					{
						return m_Mode;
					}

				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					GeometryPtr       m_Geometry1;
					GeometryPtr       m_Geometry2;
					Graphic::CombineMode m_Mode;
				};
			}
		}
	}
}