#pragma once
#include "Brush.h"
using namespace System;
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class SolidColorBrush;
			typedef std::shared_ptr<SolidColorBrush> SolidColorBrushPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API SolidColorBrush : public Brush
			{
			public:
				__uctor explicit SolidColorBrush(const System::Color& color);
				~SolidColorBrush() {}
			public:
				void SetColor(const System::Color& color);
				System::Color GetColor() const;
			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			private:
				static bool ColorChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr ColorProperty;

				__uproperty(get = GetColor, put = SetColor) System::Color Color;

				__usctordef(SolidColorBrush)
				REFLECT_CLASS(SolidColorBrush)
			};
		}
	}
}