#pragma once
#include "Brush.h"
#include "GradientStopCollection.h"
using namespace System;
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class LinearGradientBrush;
			typedef std::shared_ptr<LinearGradientBrush> LinearGradientBrushPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API LinearGradientBrush : public Brush
			{
			public:
				__uctor explicit LinearGradientBrush(const GradientStopCollectionPtr& gradientStops, const Point& start, const Point& end);
				~LinearGradientBrush() {}
			public:
				GradientStopCollectionPtr GetGradientStops() const;
				void SetGradientStops(const GradientStopCollectionPtr& gradientStops);

				Point GetStart() const;
				void SetStart(const Point& start);

				Point GetEnd() const;
				void SetEnd(const Point& end);
			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			private:
				static bool StartChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool EndChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool GradientStopsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr StartProperty;
				static const DependencyPropertyPtr EndProperty;
				static const DependencyPropertyPtr GradientStopsProperty;

				__uproperty(get = GetStart, put = SetStart) Point Start;
				__uproperty(get = GetEnd, put = SetEnd) Point End;
				__uproperty(get = GetGradientStops, put = SetGradientStops) GradientStopCollectionPtr GradientStops;

				__usctordef(LinearGradientBrush)
				REFLECT_CLASS(LinearGradientBrush)
			};
		}
	}
}