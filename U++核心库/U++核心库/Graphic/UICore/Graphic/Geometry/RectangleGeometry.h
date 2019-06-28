#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class RectangleGeometry;
			typedef std::shared_ptr<RectangleGeometry> RectangleGeometryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API RectangleGeometry : public Geometry
			{
			public:
				__uctor RectangleGeometry(const System::Rect& rect);
				~RectangleGeometry();
			public:
				System::Rect GetRect() const;
				void SetRect(const System::Rect& rect);

			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool RectChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);

			public:
				static const DependencyPropertyPtr RectProperty;

				__uproperty(get = GetRect, put = SetRect) System::Rect Rect;

				__usctordef(RectangleGeometry)
				REFLECT_CLASS(RectangleGeometry)
			};
		}
	}
}
