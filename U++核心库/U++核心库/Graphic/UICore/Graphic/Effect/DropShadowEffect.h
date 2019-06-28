#pragma once
#include "Effect.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class DropShadowEffect;
			typedef std::shared_ptr<DropShadowEffect> DropShadowEffectPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API DropShadowEffect : public Effect
			{
			public:
				__uctor DropShadowEffect(double blurSize, BlurOptimization optimization, const System::Color& color, const TransformPtr& transform);
				__uctor DropShadowEffect(double blurSize, BlurOptimization optimization, const System::Color& color, double direction, double shadowDepth);
				~DropShadowEffect();
			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;
			public:
				double GetBlurSize() const;
				void SetBlurSize(double blurSize);

				BlurOptimization GetOptimization() const;
				void SetOptimization(BlurOptimization optimization);

				System::Color GetColor() const;
				void SetColor(const System::Color& color);

				bool GetEnableTransform() const;
				void SetEnableTransform(bool enableTransform);

				void SetShadowTransform(const TransformPtr& transform);
				TransformPtr GetShadowTransform() const;

				double GetDirection() const;
				void SetDirection(double direction);

				double GetShadowDepth() const;
				void SetShadowDepth(double shadowDepth);
			private:
				static bool BlurSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool OptimizationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ColorChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool EnableTransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ShadowTransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool DirectionChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ShadowDepthChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr BlurSizeProperty;
				static const DependencyPropertyPtr OptimizationProperty;
				static const DependencyPropertyPtr ColorProperty;
				static const DependencyPropertyPtr EnableTransformProperty;
				static const DependencyPropertyPtr ShadowTransformProperty;
				static const DependencyPropertyPtr DirectionProperty;
				static const DependencyPropertyPtr ShadowDepthProperty;

				__uproperty(get = GetBlurSize, put = SetBlurSize) double BlurSize;
				__uproperty(get = GetOptimization, put = SetOptimization) BlurOptimization Optimization;
				__uproperty(get = GetColor, put = SetColor) System::Color Color;
				__uproperty(get = GetEnableTransform, put = SetEnableTransform) bool EnableTransform;
				__uproperty(get = GetShadowTransform, put = SetShadowTransform) TransformPtr ShadowTransform;
				__uproperty(get = GetDirection, put = SetDirection) double Direction;
				__uproperty(get = GetShadowDepth, put = SetShadowDepth) double ShadowDepth;

				__usctordef(DropShadowEffect)
				REFLECT_CLASS(DropShadowEffect)
			};
		}
	}
}
