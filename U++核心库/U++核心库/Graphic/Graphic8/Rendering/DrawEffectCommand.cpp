#include "DrawEffectCommand.h"
#include "..\Media\Bitmap.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace D2D1;

DrawEffectCommand::DrawEffectCommand() : m_compositeMode(EffectCompositeMode::SourceOver)
{
}


DrawEffectCommand::~DrawEffectCommand(void)
{
}

void DrawEffectCommand::Run()
{
    auto bitmapSize = m_bitmap->GetSize();

    m_elementEffect->SetInput(m_bitmap);

    auto outputImage = m_elementEffect->GetOutputImage(m_deviceContext);

    Matrix3x2F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    D2D1_RECT_F imageBounds;
    m_deviceContext->GetImageLocalBounds(outputImage.Get(), &imageBounds);

    Matrix3x2F newTrans =  Matrix3x2F::Scale(m_targetSize.Width/bitmapSize.Width, 
                                             m_targetSize.Height/bitmapSize.Height, 
                                             Point2F(0, 0));


   
    newTrans = newTrans * Matrix3x2F::Translation(imageBounds.left, imageBounds.top);

    newTrans = newTrans * oldTransform;

    m_deviceContext->SetTransform(newTrans);

    m_deviceContext->DrawImage(outputImage.Get(), 
                               Point2F(0, 0), 
                               imageBounds,
                               D2D1_INTERPOLATION_MODE_LINEAR, 
                               (D2D1_COMPOSITE_MODE)(UInt)m_compositeMode);

    m_deviceContext->SetTransform(oldTransform);

	outputImage.Reset();
}
