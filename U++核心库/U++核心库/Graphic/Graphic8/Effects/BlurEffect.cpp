#include "BlurEffect.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace std;

BlurEffect::BlurEffect(void) :m_optimization(BlurOptimization::Balanced)
{
    m_blurEffect = make_shared<EffectResource>(CLSID_D2D1GaussianBlur);

    SetEffect(m_blurEffect);
}

BlurEffect::~BlurEffect(void)
{
}

void BlurEffect::SetInput(const BitmapPtr& image)
{
    if(image == m_inputImage)
        return;

    m_inputImage = image;

    m_blurEffect->SetInput(0, m_inputImage);
}

float BlurEffect::BlurSize() const
{
    return m_blurSize;
}

void BlurEffect::BlurSize(float blurSize)
{
    if(m_blurSize == blurSize)
        return;

    m_blurSize = blurSize;
    m_blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, m_blurSize);
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}

BlurOptimization BlurEffect::Optimization() const
{
	return m_optimization;
}

void BlurEffect::Optimization(BlurOptimization optimization)
{
	if (m_optimization == optimization)
		return;

	m_optimization = optimization;

	m_blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, m_optimization);
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}

BlurBorderMode BlurEffect::BorderMode() const
{
	return m_blurBorderMode;
}

void BlurEffect::BorderMode(BlurBorderMode borderMode)
{
	if (m_blurBorderMode == borderMode)
		return;

	m_blurBorderMode = borderMode;

	m_blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, m_blurBorderMode);
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}