#include "FillCommand.h"
using namespace D2D1;
using namespace System::UI::Graphic::d2d1;
using namespace System;

void FillCommand::Run()
{
    if (!IsResourceValidFrom(m_Geometry))
    {
        PerPrimitiveAliasingScope perPrimitive(m_RT);
        auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
        auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);

        m_RT->FillGeometry(geometry, 
                           brush);
    }
    else
    {
        // if geometry is small enough there can be a fill mask
        ID2D1BitmapPtr mask = m_Geometry->GetFillMask(m_RT);

        if (mask)
        {
            AliasModeScope aliasScope(m_RT);
            TemporaryTransformScope tempTransform(m_RT, 
                                                  m_Brush, 
                                                  m_Geometry->GetFillMaskTransform());

            auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);

            m_RT->FillOpacityMask(mask.Get(), 
                                  brush, 
                                  D2D1_OPACITY_MASK_CONTENT_GRAPHICS);
        }
        else
        {
            // geometry is too big so we do not have a mask 
            PerPrimitiveAliasingScope perPrimitive(m_RT);
            auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
            auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
            m_RT->FillGeometry(geometry, brush);
        }
    }
}
