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
				class BlurEffect;
				typedef std::shared_ptr<BlurEffect> BlurEffectPtr;

				class BlurEffect : public ElementEffect
				{
				public:
					BlurEffect(void);
					virtual ~BlurEffect(void);
				public:
					virtual void SetInput(const BitmapPtr& image) override;
				public:
					auto BlurSize() const -> float;
					void BlurSize(float blurSize);
					auto Optimization() const->BlurOptimization;
					void Optimization(BlurOptimization optimization);
					auto BorderMode() const->BlurBorderMode;
					void BorderMode(BlurBorderMode borderMode);
				private:
					EffectResourcePtr m_blurEffect;
					BitmapPtr          m_inputImage;
					float             m_blurSize;
					BlurOptimization m_optimization;
					BlurBorderMode m_blurBorderMode;
				};
			}
		}
	}
}
