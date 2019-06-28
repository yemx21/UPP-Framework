#pragma once
#include "..\Geometry\Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Ellipse;
				typedef std::shared_ptr<Ellipse> EllipsePtr;

				class Ellipse : public Geometry
				{
				public:
					Ellipse(Point center, float rx, float ry) : m_Center(center), m_RX(rx), m_RY(ry) {}
					virtual ~Ellipse() {}
				public:
					auto RadiusX() const -> float;
					void RadiusX(float rx);

					auto RadiusY() const -> float;
					void RadiusY(float ry);

					auto Center() const->Point;
					void Center(const Point& center);
				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					Point m_Center;
					float            m_RX;
					float            m_RY;
				};
			}
		}
	}
}
