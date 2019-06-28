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
				class DropShadowEffect;
				typedef std::shared_ptr<DropShadowEffect> DropShadowEffectPtr;

				class DropShadowEffect : public ElementEffect
				{
				public:
					DropShadowEffect(void);
					virtual ~DropShadowEffect(void);
				public:
					virtual void SetInput(const BitmapPtr& image) override;
				public:
					auto BlurSize() const -> float;
					void BlurSize(float blurSize);

					System::Color ShadowColor();
					void ShadowColor(const System::Color& color);

					auto Optimization() const->BlurOptimization;
					void Optimization(BlurOptimization optimization);

					bool EnableTransform() const;
					void EnableTransform(bool enableTransform);

					void ShadowTransform(const TransformPtr& transform);
					auto ShadowTransform() const -> const TransformPtr;

					float Direction() const;
					void Direction(float direction);

					float ShadowDepth() const;
					void ShadowDepth(float shadowDepth);


				private:
					EffectResourcePtr m_shadowEffect;
					EffectResourcePtr m_affineEffect;
					EffectResourcePtr m_compositeEffect;
					BitmapPtr          m_inputImage;
					float             m_blurSize;
					float             m_direction;
					float             m_shadowDepth;
					bool				m_enableTransform;
					TransformPtr      m_transform;
					BlurOptimization m_optimization;
					System::Color  m_shadowColor;
				};
			}
		}
	}
}
