#include "DropShadowEffect.h"

using namespace System;
using namespace std;
using namespace System::UI::Graphic::d2d1;

DropShadowEffect::DropShadowEffect(void) : m_enableTransform(false), m_direction(315.0f), m_shadowDepth(0.0f)
{
    m_shadowEffect = make_shared<EffectResource>(CLSID_D2D1Shadow);
    m_affineEffect = make_shared<EffectResource>(CLSID_D2D12DAffineTransform);
    m_compositeEffect = make_shared<EffectResource>(CLSID_D2D1Composite);

    //m_shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, 0.0f);
	//m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, D2D1::Matrix3x2F::Translation(0, 0));
   

    SetEffect(m_compositeEffect);
}

DropShadowEffect::~DropShadowEffect(void)
{
}

void DropShadowEffect::SetInput(const BitmapPtr& image)
{
    if(image == m_inputImage)
        return;

    m_inputImage = image;

    m_shadowEffect->SetInput(0, m_inputImage);
    m_affineEffect->SetInput(0, m_shadowEffect);
    m_compositeEffect->SetInput(0, m_affineEffect);
    m_compositeEffect->SetInput(1, m_inputImage);
}

float DropShadowEffect::BlurSize() const
{
    return m_blurSize;
}

void DropShadowEffect::BlurSize(float blurSize)
{
    m_blurSize = blurSize;
    m_shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, m_blurSize);
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}

auto DropShadowEffect::Optimization() const->BlurOptimization
{
	return m_optimization;
}

void DropShadowEffect::Optimization(BlurOptimization optimization)
{
	m_optimization = optimization;
	m_shadowEffect->SetValue(D2D1_SHADOW_PROP_OPTIMIZATION, m_optimization);
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}


void DropShadowEffect::ShadowTransform( const TransformPtr& transform )
{
    m_transform = transform;
	if (m_enableTransform)
	{
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, m_transform->GetTransformMatrix());
		NotifyInvalidatables(InvalidateFlags_internal::Visual);
	}
}

auto DropShadowEffect::ShadowTransform() const -> const TransformPtr
{
	return m_transform;
}

System::Color DropShadowEffect::ShadowColor()
{
	return m_shadowColor;
}
void DropShadowEffect::ShadowColor( const System::Color& color )
{
    m_shadowColor = color;

    auto colorVector = D2D1::Vector4F(color.ScR, color.ScG, color.ScB, color.ScA);
    m_shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, colorVector);
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}

bool DropShadowEffect::EnableTransform() const
{
	return m_enableTransform;
}

void DropShadowEffect::EnableTransform(bool enableTransform)
{
	if (m_enableTransform == enableTransform) return;
	m_enableTransform = enableTransform;
	if (m_enableTransform)
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, m_transform->GetTransformMatrix());		
	else
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, D2D1::Matrix3x2F::Translation(cosf(m_direction)* m_shadowDepth, sinf(m_direction)* m_shadowDepth));

	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}

float DropShadowEffect::Direction() const
{
	return m_direction;
}

void DropShadowEffect::Direction(float direction)
{
	if (m_direction == direction) return;
	m_direction = direction;
	if (m_enableTransform)
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, m_transform->GetTransformMatrix());
	else
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, D2D1::Matrix3x2F::Translation(cosf(m_direction)* m_shadowDepth, sinf(m_direction)* m_shadowDepth));
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}

float DropShadowEffect::ShadowDepth() const
{
	return m_shadowDepth;
}

void DropShadowEffect::ShadowDepth(float shadowDepth)
{
	if (m_shadowDepth == shadowDepth) return;
	m_shadowDepth = shadowDepth;
	if (m_enableTransform)
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, m_transform->GetTransformMatrix());
	else
		m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, D2D1::Matrix3x2F::Translation(cosf(m_direction)* m_shadowDepth, sinf(m_direction)* m_shadowDepth));
	NotifyInvalidatables(InvalidateFlags_internal::Visual);
}