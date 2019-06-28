#include "ElementEffect.h"

using namespace System;
using namespace Microsoft::WRL;
using namespace System::UI::Graphic::d2d1;

ElementEffect::ElementEffect() 
{

}

ElementEffect::~ElementEffect()
{

}

void ElementEffect::SetEffect(const EffectResourcePtr& effect)
{
    m_effect = effect;
}

ComPtr<ID2D1Image> ElementEffect::GetOutputImage(ID2D1DeviceContext* deviceContext)
{
    auto d2dEffect = m_effect->GetResource<ID2D1Effect>(deviceContext);

    ComPtr<ID2D1Image> outputImage;

    d2dEffect->GetOutput(outputImage.GetAddressOf());

    return outputImage;
}


auto ElementEffect::GetEffectResource() -> EffectResourcePtr
{
    return m_effect;
}

auto ElementEffect::GetOutputImageBrush( ID2D1DeviceContext* /* deviceContext*/ ) -> ID2D1ImageBrush* 
{
    return nullptr;
}
