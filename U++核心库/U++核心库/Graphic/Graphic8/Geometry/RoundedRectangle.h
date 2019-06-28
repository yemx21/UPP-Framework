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
				class RoundedRectangle;
				typedef std::shared_ptr<RoundedRectangle> RoundedRectanglePtr;

				class RoundedRectangle : public Geometry
				{
				public:
					RoundedRectangle(const Rect& rect, float rx, float ry) :
						m_rect(rect),
						m_RX(rx),
						m_RY(ry)
					{
					}

					virtual ~RoundedRectangle() {}
				public:
					auto RadiusX() const -> float;
					void RadiusX(float rx);

					auto RadiusY() const -> float;
					void RadiusY(float ry);

					auto Rect() const->Rect;
					void Rect(const System::Rect& rect);
				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					System::Rect m_rect;
					float                m_RX;
					float                m_RY;
				};
			}
		}
	}
}

