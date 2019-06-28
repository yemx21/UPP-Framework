
#include "DrawTextCommand.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace D2D1;

DrawTextCommand::DrawTextCommand(void)
{
}


DrawTextCommand::~DrawTextCommand(void)
{
}

void DrawTextCommand::Run()
{
    auto layout    = GetResourceFrom<IDWriteTextLayout>(m_textLayout);
    auto textBrush = GetResourceFrom<ID2D1Brush>(m_brush, m_deviceContext.Get());

    auto originalTransform = m_brush->Transform();

    auto layoutBounds = m_textLayout->GetLayoutBounds();

    layoutBounds.Top    += m_origin.Y;
	layoutBounds.Left += m_origin.X;
	layoutBounds.Bottom += m_origin.Y;
	layoutBounds.Right += m_origin.X;

    auto boundsTransform = m_brush->GetTransformToBounds(layoutBounds);

    boundsTransform = boundsTransform * originalTransform;

    m_brush->Transform(boundsTransform);

    m_deviceContext->DrawTextLayout(D2DPoint(m_origin), layout, textBrush);

    m_brush->Transform(originalTransform);
}
