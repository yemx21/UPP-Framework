#include "DrawImageCommand.h"
#include "..\Media\Bitmap.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace D2D1;

DrawImageCommand::DrawImageCommand() : m_compositeMode(EffectCompositeMode::SourceOver)
{
}


DrawImageCommand::~DrawImageCommand(void)
{
}

void DrawImageCommand::Run()
{
    auto bmp = GetResourceFrom<ID2D1Bitmap1>(m_bitmap, m_deviceContext);
	if (!bmp) return;

    auto bitmapSize = m_bitmap->GetSize();

    Matrix3x2F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    D2D1_RECT_F imageBounds;
    m_deviceContext->GetImageLocalBounds(bmp, &imageBounds);

	Matrix3x2F newTrans = Matrix3x2F::Scale(m_targetSize.Width / bitmapSize.Width,
                                             m_targetSize.Height/bitmapSize.Height, 
                                             Point2F(0, 0));

	newTrans = newTrans * Matrix3x2F::Translation(imageBounds.left, imageBounds.top);

    newTrans = newTrans * oldTransform;

    m_deviceContext->SetTransform(newTrans);

    m_deviceContext->DrawImage(bmp, 
                               Point2F(0, 0), 
                               imageBounds,
                               D2D1_INTERPOLATION_MODE_LINEAR, 
                               (D2D1_COMPOSITE_MODE)(UInt)m_compositeMode);

    m_deviceContext->SetTransform(oldTransform);
}
