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
				class Rectangle;
				typedef std::shared_ptr<Rectangle> RectanglePtr;

				class Rectangle : public Geometry
				{
				public:
					Rectangle(Rect rect) :
						m_rect(rect)
					{
					}

					virtual ~Rectangle() {}
				public:
					auto Rect() const->Rect;
					void Rect(const System::Rect& rect);
				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					System::Rect m_rect;
				};
			}
		}
	}
}

