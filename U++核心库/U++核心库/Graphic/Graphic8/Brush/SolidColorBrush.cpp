#include "SolidColorBrush.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;

auto SolidColorBrush::OnCreateResource(ID2D1DeviceContext* deviceContext) -> ComPtr<IUnknown> 
{
    ComPtr<ID2D1SolidColorBrush> solidColorBrush;
	if (deviceContext->CreateSolidColorBrush(D2DColor(m_Color), D2D1::BrushProperties(m_Opacity), &solidColorBrush) != S_OK)
		solidColorBrush = nullptr;
	else
		solidColorBrush->SetTransform(D2DMatrix(m_transform));
    return solidColorBrush;
}

 void SolidColorBrush::FillColor(const Color& c)
 {
     m_Color = c;
     Invalidate();
 }

auto SolidColorBrush::FillColor() const -> Color
 {
     return m_Color;
 }
