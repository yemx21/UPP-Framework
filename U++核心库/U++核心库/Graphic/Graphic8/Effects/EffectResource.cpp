#include "EffectResource.h"


using namespace System;
using namespace Microsoft::WRL;
using namespace System::UI::Graphic::d2d1;
EffectResource::EffectResource(const IID& effectCLSID) : m_effectCLSID(effectCLSID)
{
}


EffectResource::~EffectResource(void)
{
}

auto EffectResource::IsResourceValid() const -> bool
{
    if(!DeviceDependantResource::IsResourceValid())
        return false;

    for (auto& inputs : m_imageInputs)
    {
       if(IsResourceValidFrom(inputs.second))
           return true;
    }

    for (auto& inputs : m_effectInputs)
    {
       if(IsResourceValidFrom(inputs.second))
           return true;
    }

    return false;
}


Microsoft::WRL::ComPtr<IUnknown> EffectResource::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
    Microsoft::WRL::ComPtr<ID2D1Effect> effect;
	if (deviceContext->CreateEffect(m_effectCLSID, &effect) != S_OK)
		effect = nullptr;
	else    
		ApplyAllInputs(effect);
   
    return effect;
}

void EffectResource::SetInput(int index, const BitmapPtr& image)
{
    m_imageInputs[index] = image;

    if(IsResourceValid())
    {
        ComPtr<ID2D1Image> currentImage;
        GetResourceUnsafe<ID2D1Effect>()->GetInput(index, &currentImage);

        if(currentImage.Get() == nullptr ||
           currentImage.Get() != GetResourceUnsafeFrom<ID2D1Image>(image))
        {
             GetResourceUnsafe<ID2D1Effect>()->SetInput(index, 
                                                        GetResourceUnsafeFrom<ID2D1Image>(image));
        }
    }

    auto it = m_effectInputs.find(index);

    if(it != m_effectInputs.end())
    {
        m_effectInputs.erase(index);
    }
}

void EffectResource::SetInput(int index, const EffectResourcePtr& effect)
{
    if(m_effectInputs[index] == effect)
        return;

    m_effectInputs[index] = effect;

    if(IsResourceValid())
    {
         GetResourceUnsafe<ID2D1Effect>()->SetInputEffect(index, 
                                                          GetResourceUnsafeFrom<ID2D1Effect>(effect));
    }

    auto it = m_imageInputs.find(index);

    if(it != m_imageInputs.end())
    {
        m_imageInputs.erase(index);
    }

    if(IsResourceValid())
    {
         GetResourceUnsafe<ID2D1Effect>()->SetInputCount((UINT32)m_imageInputs.size() + (UINT32)m_effectInputs.size());
    }
}

void EffectResource::ApplyAllInputs(const Microsoft::WRL::ComPtr<ID2D1Effect>& effect)
{
    for (auto& inputs : m_imageInputs)
    {
        auto input = GetResourceFrom<ID2D1Image>(inputs.second, GetDeviceContext());

        effect->SetInput(inputs.first, 
                         input);
    }

    for (auto& inputs : m_effectInputs)
    {
        auto input = GetResourceFrom<ID2D1Effect>(inputs.second, GetDeviceContext());
      
        effect->SetInputEffect(inputs.first, 
                               input);
    }

    for(auto& settingData : m_settings)
    {
        HRESULT hr = effect->SetValue(settingData.first, 
                                     (BYTE*)&settingData.second[0], 
                                     (UINT32)settingData.second.size());

		if (hr != S_OK) return;
    }
    
    effect->SetInputCount((UINT32)m_imageInputs.size() + (UINT32)m_effectInputs.size());
}

void EffectResource::ClearInputs()
{
    if(IsResourceValid())
    {
        auto resource = GetResourceUnsafe<ID2D1Effect>();
        for (int i = 0; i < (int)m_imageInputs.size(); i++)
        {
            resource->SetInput(i, nullptr);
        }

        resource->SetInputCount(0);
    }

    m_imageInputs.clear();
    m_effectInputs.clear();
}
