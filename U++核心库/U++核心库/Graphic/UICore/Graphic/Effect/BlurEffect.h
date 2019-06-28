#pragma once
#include "Effect.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class BlurEffect;
			typedef std::shared_ptr<BlurEffect> BlurEffectPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API BlurEffect : public Effect
			{
			public:
				__uctor BlurEffect(double blurSize, BlurOptimization optimization, BlurBorderMode borderMode);
				~BlurEffect();
			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;
			public:
				double GetBlurSize() const;
				void SetBlurSize(double blurSize);
				BlurOptimization GetOptimization() const;
				void SetOptimization(BlurOptimization optimization);
				BlurBorderMode GetBorderMode() const;
				void SetBorderMode(BlurBorderMode borderMode);
			private:
				static bool BlurSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool OptimizationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool BorderModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr BlurSizeProperty;
				static const DependencyPropertyPtr OptimizationProperty;
				static const DependencyPropertyPtr BorderModeProperty;

				__uproperty(get = GetBlurSize, put = SetBlurSize) double BlurSize;
				__uproperty(get = GetOptimization, put = SetOptimization) BlurOptimization Optimization;
				__uproperty(get = GetBorderMode, put = SetBorderMode) BlurBorderMode BorderMode;

				__usctordef(BlurEffect)
				REFLECT_CLASS(BlurEffect)
			};
		}
	}
}
