#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class CombinedGeometry;
			typedef std::shared_ptr<CombinedGeometry> CombinedGeometryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API CombinedGeometry : public Geometry
			{
			public:
				__uctor CombinedGeometry(const GeometryPtr& g1, const GeometryPtr& g2, CombineMode mode);
				~CombinedGeometry();
			public:
				CombineMode GetMode() const;
				void SetMode(CombineMode mode);

				GeometryPtr GetGeometry1() const;
				void SetGeometry1(const GeometryPtr& geometry1);

				GeometryPtr GetGeometry2() const;
				void SetGeometry2(const GeometryPtr& geometry2);

			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool ModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr ModeProperty;
				static const DependencyPropertyPtr Geometry1Property;
				static const DependencyPropertyPtr Geometry2Property;

				__uproperty(get = GetMode, put = SetMode) CombineMode Mode;
				__uproperty(get = GetGeometry1, put = SetGeometry1) GeometryPtr Geometry1;
				__uproperty(get = GetGeometry2, put = SetGeometry2) GeometryPtr Geometry2;

				__usctordef(CombinedGeometry)
				REFLECT_CLASS(CombinedGeometry)
			};
		}
	}
}