#pragma once
#include "Brush.h"
#include "..\Media\BitmapSource.h"
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class BitmapBrush;
			typedef std::shared_ptr<BitmapBrush> BitmapBrushPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API BitmapBrush : public Brush
			{ 
			public:
				__uctor BitmapBrush();
				~BitmapBrush();
			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			public:
				ExtendMode GetExtendModeX() const;
				void SetExtendModeX(ExtendMode extendMode);

				ExtendMode GetExtendModeY() const;
				void SetExtendModeY(ExtendMode extendMode);

				Rect GetSourceRect() const;
				void SetSourceRect(const Rect& rect);

				BitmapSourcePtr GetImageSource() const;
				void SetImageSource(const BitmapSourcePtr& image);

				InterpolationMode GetInterpolation()  const;
				void SetInterpolation(InterpolationMode mode);
			private:
				static bool ExtendModeXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ExtendModeYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool SourceRectChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ImageSourceChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool InterpolationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr ExtendModeXProperty;
				static const DependencyPropertyPtr ExtendModeYProperty;
				static const DependencyPropertyPtr SourceRectProperty;
				static const DependencyPropertyPtr ImageSourceProperty;
				static const DependencyPropertyPtr InterpolationProperty;

				__uproperty(get = GetExtendModeX, put = SetExtendModeX) ExtendMode ExtendModeX;
				__uproperty(get = GetExtendModeY, put = SetExtendModeY) ExtendMode ExtendModeY;
				__uproperty(get = GetSourceRect, put = SetSourceRect) Rect SourceRect;
				__uproperty(get = GetImageSource, put = SetImageSource) BitmapSourcePtr ImageSource;
				__uproperty(get = GetInterpolation, put = SetInterpolation) InterpolationMode Interpolation;

				__usctordef(BitmapBrush)
				REFLECT_CLASS(BitmapBrush)
			};
		}
	}
}