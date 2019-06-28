#include "LinearGradientBrush.h"

using namespace System;

using namespace System::UI::Graphic::d2d1;
using namespace D2D1;
using namespace Microsoft::WRL;

LinearGradientBrush::~LinearGradientBrush(void)
{
}

Microsoft::WRL::ComPtr<IUnknown> LinearGradientBrush::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
    ComPtr<ID2D1LinearGradientBrush> brush;

    auto gsc = GetResourceFrom<ID2D1GradientStopCollection>(m_gradientStopCollection, 
                                                            deviceContext);
	if (
		deviceContext->CreateLinearGradientBrush(
		LinearGradientBrushProperties(D2DPoint(m_Start), D2DPoint(m_End)),
		BrushProperties(m_Opacity),
		gsc,
		&brush
		) != S_OK)
	{ 
		brush = nullptr;
	}
    
    return brush;
}

auto LinearGradientBrush::IsResourceValid() const -> bool 
{
    if(!IsResourceValidFrom(m_gradientStopCollection))
        return false;

    return Brush::IsResourceValid();
}

auto LinearGradientBrush::GradientStopCollection() const -> const GradientStopCollectionPtr
{
    return m_gradientStopCollection;
}

LinearGradientBrush::LinearGradientBrush(GradientStopCollectionPtr gradientStopCollection, Point start, Point end)
    : m_gradientStopCollection(gradientStopCollection), 
      m_Start(start), 
      m_End(end)
{
}

void LinearGradientBrush::Start(Point val)
{
    m_Start = val;

    if(IsResourceValid())
    {
        ComPtr<ID2D1LinearGradientBrush> brush;
        brush = GetResourceUnsafe<ID2D1LinearGradientBrush>();
		brush->SetEndPoint(D2DPoint(m_Start));
    }
}

void LinearGradientBrush::End(Point val)
{
    m_End = val; 

    if(IsResourceValid())
    {
        ComPtr<ID2D1LinearGradientBrush> brush;
        brush = GetResourceUnsafe<ID2D1LinearGradientBrush>();
		brush->SetEndPoint(D2DPoint(m_End));
    }
}

auto LinearGradientBrush::Start() const -> Point 
{
    return m_Start;
}

auto LinearGradientBrush::End() const -> Point
{
    return m_End;
}

LinearGradientBrush::LinearGradientBrush()
{

}

void LinearGradientBrush::GradientStopCollection( const GradientStopCollectionPtr& stopCollection )
{
    m_gradientStopCollection = stopCollection;
    Invalidate();
}
