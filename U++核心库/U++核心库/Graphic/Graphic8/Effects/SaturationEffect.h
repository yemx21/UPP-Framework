#pragma once
#include "..\Resource.h"

#include "EffectResource.h"
#include "ElementEffect.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class SaturationEffect;
				typedef std::shared_ptr<SaturationEffect> SaturationEffectPtr;

				class SaturationEffect : public ElementEffect
				{
				public:
					SaturationEffect(void);
					virtual ~SaturationEffect(void);
				public:
					virtual void SetInput(const BitmapPtr& image) override;
				public:
					auto SaturationAmount() const -> float;
					void SaturationAmount(float blurSize);
				private:
					EffectResourcePtr m_saturationEffect;
					BitmapPtr          m_inputImage;
					float             m_saturationAmount;
				};
			}
		}
	}
}
