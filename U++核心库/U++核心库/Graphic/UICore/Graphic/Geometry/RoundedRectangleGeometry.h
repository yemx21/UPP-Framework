#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class RoundedRectangleGeometry;
			typedef std::shared_ptr<RoundedRectangleGeometry> RoundedRectangleGeometryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API RoundedRectangleGeometry : public Geometry
			{
			public:
				__uctor RoundedRectangleGeometry(const System::Rect& rect, double radiusX, double radiusY);
				~RoundedRectangleGeometry();
			public:
				double GetRadiusX() const;
				void SetRadiusX(double rx);

				double GetRadiusY() const;
				void SetRadiusY(double ry);

				System::Rect GetRect() const;
				void SetRect(const System::Rect& rect);

			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool RadiusXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool RadiusYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool RectChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);

			public:
				static const DependencyPropertyPtr RectProperty;
				static const DependencyPropertyPtr RadiusXProperty;
				static const DependencyPropertyPtr RadiusYProperty;

				__uproperty(get = GetRadiusX, put = SetRadiusX) double RadiusX;
				__uproperty(get = GetRadiusY, put = SetRadiusY) double RadiusY;
				__uproperty(get = GetRect, put = SetRect) System::Rect Rect;

				__usctordef(RoundedRectangleGeometry)
				REFLECT_CLASS(RoundedRectangleGeometry)
			};
		}
	}
}
