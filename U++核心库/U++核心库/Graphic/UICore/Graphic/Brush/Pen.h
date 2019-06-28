#pragma once
#include "Brush.h"
#include "StrokeStyle.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class Pen;
			typedef std::shared_ptr<Pen> PenPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Pen : public FeatureDependantResource
			{
			public:
				__uctor Pen();
				~Pen();
			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			public:
				void SetThickness(double thickness);
				double GetThickness() const;

				void SetBrush(const BrushPtr& brush);
				BrushPtr GetBrush() const;

				StrokeStylePtr GetStrokeStyle() const;
				void SetStrokeStyle(const StrokeStylePtr& strokeStyle);
			public:
				static const DependencyPropertyPtr ThicknessProperty;
				static const DependencyPropertyPtr BrushProperty;
				static const DependencyPropertyPtr StrokeStyleProperty;

				__uproperty(get = GetThickness, put = SetThickness) double Thickness;
				__uproperty(get = GetBrush, put = SetBrush) BrushPtr Brush;
				__uproperty(get = GetStrokeStyle, put = SetStrokeStyle) StrokeStylePtr StrokeStyle;

				__usctordef(Pen)
				REFLECT_CLASS(Pen)
			};
		}
	}
}
