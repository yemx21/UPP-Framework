#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class EllipseGeometry;
			typedef std::shared_ptr<EllipseGeometry> EllipseGeometryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API EllipseGeometry : public Geometry
			{
			public:
				__uctor EllipseGeometry(const Point& center, double radiusX, double radiusY);
				~EllipseGeometry();
			public:
				double GetRadiusX() const;
				void SetRadiusX(double rx);

				double GetRadiusY() const;
				void SetRadiusY(double ry);

				Point GetCenter() const;
				void SetCenter(const Point& center);

			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool RadiusXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool RadiusYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool CenterChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr RadiusXProperty;
				static const DependencyPropertyPtr RadiusYProperty;
				static const DependencyPropertyPtr CenterProperty;

				__uproperty(get = GetRadiusX, put = SetRadiusX) double RadiusX;
				__uproperty(get = GetRadiusY, put = SetRadiusY) double RadiusY;
				__uproperty(get = GetCenter, put = SetCenter) Point Center;

				__usctordef(EllipseGeometry)
				REFLECT_CLASS(EllipseGeometry)
			};
		}
	}
}
