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
			class RadialGradientBrush;
			typedef std::shared_ptr<RadialGradientBrush> RadialGradientBrushPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API RadialGradientBrush : public Brush
			{
			public:
				__uctor explicit RadialGradientBrush(const GradientStopCollectionPtr& gradientStops, Point center, Point gradientOriginOffset, float radiusX, float radiusY);
				~RadialGradientBrush() {}
			public:
				GradientStopCollectionPtr GetGradientStops()const;
				void SetGradientStops(const GradientStopCollectionPtr& gradientStops);

				Point GetCenter() const;
				void SetCenter(const Point& center);

				Point GetGradientOriginOffset() const;
				void SetGradientOriginOffset(const Point& gradientOriginOffset);

				double GetRadiusX() const;
				void SetRadiusX(double radiusX);

				double GetRadiusY() const;
				void SetRadiusY(double radiusY);

			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			private:
				static bool RadiusXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool RadiusYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool CenterChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool GradientOriginOffsetChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool GradientStopsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr RadiusXProperty;
				static const DependencyPropertyPtr RadiusYProperty;
				static const DependencyPropertyPtr CenterProperty;
				static const DependencyPropertyPtr GradientOriginOffsetProperty;
				static const DependencyPropertyPtr GradientStopsProperty;

				__uproperty(get = GetCenter, put = SetCenter) Point Center;
				__uproperty(get = GetGradientOriginOffset, put = SetGradientOriginOffset) Point GradientOriginOffset;
				__uproperty(get = GetRadiusX, put = SetRadiusX) double RadiusX;
				__uproperty(get = GetRadiusY, put = SetRadiusY) double RadiusY;
				__uproperty(get = GetGradientStops, put = SetGradientStops) GradientStopCollectionPtr GradientStops;

				__usctordef(RadialGradientBrush)
				REFLECT_CLASS(RadialGradientBrush)
			};
		}
	}
}