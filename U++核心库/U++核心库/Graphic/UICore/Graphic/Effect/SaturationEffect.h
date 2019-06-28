#pragma once
#include "Effect.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class SaturationEffect;
			typedef std::shared_ptr<SaturationEffect> SaturationEffectPtr;
			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API SaturationEffect : public Effect
			{
			public:
				__uctor SaturationEffect(float amount);
				~SaturationEffect();
			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;
			public:
				double GetAmount() const;
				void SetAmount(double blurSize);
			private:
				static bool AmountChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr AmountProperty;

				__uproperty(get = GetAmount, put = SetAmount) double Amount;

				__usctordef(SaturationEffect)
				REFLECT_CLASS(SaturationEffect)
			};
		}
	}
}
