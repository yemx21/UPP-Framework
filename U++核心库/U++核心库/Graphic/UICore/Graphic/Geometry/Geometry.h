#pragma once
#include <map>
#include "..\Resource.h"
__uimport("..\\Brush\\StrokeStyle.h")

using namespace System;
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class RenderContext;

			class StrokeStyle;
			typedef std::shared_ptr<StrokeStyle> StrokeStylePtr;

			class Geometry;
			typedef std::shared_ptr<Geometry> GeometryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Geometry : public FeatureIndependantResource
			{
			public:
				Geometry();
				virtual ~Geometry() {}
			public:
				__uref bool HitTest(const Point& point);
				__uref bool HitTest(const Point& point, const Matrix& matrix);				
				__uref bool HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness);
				__uref bool HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness, const Matrix& matrix);
				__uref Rect GetBounds(const StrokeStylePtr& strokeStyle, double thickness, const Matrix& worldTransform);
				__uref bool PushTransform(const TransformPtr& transform);
			protected:
				virtual FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory);
			private:
				friend RenderContext;
				REFLECT_CLASS(Geometry)
			};
		}
	}
}