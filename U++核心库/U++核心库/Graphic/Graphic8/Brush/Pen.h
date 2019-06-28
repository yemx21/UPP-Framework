#pragma once
#include "..\Resource.h"
#include "GradientStopCollection.h"
#include "StrokeStyle.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Brush;
				typedef std::shared_ptr<Brush> BrushPtr;

				class Pen;
				typedef std::shared_ptr<Pen> PenPtr;

				class Pen
				{
				public:
					Pen(void);
					virtual ~Pen(void);
				public:
					void Thickness(float thickness);
					auto Thickness() -> float;

					void Brush(const BrushPtr& brush);
					auto Brush() -> const BrushPtr&;

					const StrokeStylePtr& StrokeStyle() const;
					void StrokeStyle(const StrokeStylePtr& val);
				private:
					float          m_thickness;
					BrushPtr       m_brush;
					StrokeStylePtr m_strokeStyle;
				};
			}
		}
	}
}
