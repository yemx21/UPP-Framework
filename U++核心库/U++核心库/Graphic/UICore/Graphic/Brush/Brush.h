#pragma once
#include "..\Resource.h"
#include "..\Geometry\Geometry.h"

__uimport("Pen.h")

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class Brush;
			typedef std::shared_ptr<Brush> BrushPtr;

			class Pen;
			typedef std::shared_ptr<Pen> PenPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Brush : public FeatureDependantResource
			{
			public:
				Brush();
				virtual ~Brush() {}
			private:
				static bool OpacityChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool TransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				void SetOpacity(double o);
				double GetOpacity() const;
				void SetTransform(const Matrix& transform);
				Matrix GetTransform() const;

				__uref Matrix GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen);

				__uref Matrix GetTransformToGeometryBounds(const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, double thickness);

				__uref Matrix GetTransformToBounds(const Rect& rect);

			protected:
				virtual FeatureObjectPtr OnCreateResource(RenderContext* renderContext);
			public:
				static const DependencyPropertyPtr OpacityProperty;
				static const DependencyPropertyPtr TransformProperty;

				__uproperty(get = GetOpacity, put = SetOpacity) double Opacity;
				__uproperty(get = GetTransform, put = SetTransform) Matrix Transform;

				__usctordef(Brush)
				REFLECT_CLASS(Brush)
			};
		}
	}
}