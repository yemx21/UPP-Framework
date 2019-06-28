#pragma once
#include "..\Rendering\DrawCommand.h"
#include "..\Brush\Pen.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class StrokeCommand : public DrawCommand
				{
				public:
					StrokeCommand() {}
					virtual ~StrokeCommand() {}
				public:
					virtual void Run();
					void SetPen(const PenPtr& pen);
				private:
					PenPtr m_pen;
				};

				class StrokeCommand1 : public DrawCommand
				{
				public:
					StrokeCommand1() {}
					virtual ~StrokeCommand1() {}
				public:
					virtual void Run();
					void SetStrokeStyle(const StrokeStylePtr& strokeStyle);
					void SetThickness(float thickness);
				private:
					StrokeStylePtr m_strokeStyle;
					float m_thickness;
				};
			}
		}
	}
}
