
#include "StrokeCommand.h"
using namespace System::UI::Graphic::d2d1;

void StrokeCommand::Run()
{
    if (!IsResourceValidFrom(m_Geometry))
    {
        PerPrimitiveAliasingScope perPrimitive(m_RT);
        auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
        auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);

        m_RT->DrawGeometry(geometry, brush, m_pen->Thickness());
    }
    else
    {
        auto thickness = m_pen->Thickness();

        ID2D1StrokeStyle* strokeStyle = nullptr;

        if(m_pen->StrokeStyle())
            strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(m_pen->StrokeStyle());

        /* If geometry is small enough there can be a stroke mask */
        ID2D1BitmapPtr mask = m_Geometry->GetStrokeMask(m_RT, thickness, strokeStyle);

        if (mask)
        {
            AliasModeScope aliasScope(m_RT);
            TemporaryTransformScope tempTransform(m_RT, m_Brush, m_Geometry->GetStrokeMaskTransform(thickness));

                
            auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
            m_RT->FillOpacityMask(mask.Get(), brush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS);
        }
        else
        {
            /* No mask for the geometry */
            PerPrimitiveAliasingScope perPrimitive(m_RT);
                
            auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
            auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
            ID2D1StrokeStyle* strokeStyle = nullptr;

            if(m_pen->StrokeStyle())
            {
                strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(m_pen->StrokeStyle());
            }
             
            m_RT->DrawGeometry(geometry, brush, thickness, strokeStyle);
        }
    }
}

void StrokeCommand::SetPen(const PenPtr& pen)
{
    m_pen = pen;
}


void StrokeCommand1::Run()
{
	if (!IsResourceValidFrom(m_Geometry))
	{
		PerPrimitiveAliasingScope perPrimitive(m_RT);
		auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
		auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
		if (m_thickness>0) m_RT->DrawGeometry(geometry, brush, m_thickness);
	}
	else
	{
		ID2D1StrokeStyle* strokeStyle = nullptr;

		if (m_strokeStyle)
			strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(m_strokeStyle);

		/* If geometry is small enough there can be a stroke mask */
		ID2D1BitmapPtr mask = m_Geometry->GetStrokeMask(m_RT, m_thickness, strokeStyle);

		if (mask)
		{
			AliasModeScope aliasScope(m_RT);
			TemporaryTransformScope tempTransform(m_RT, m_Brush, m_Geometry->GetStrokeMaskTransform(m_thickness));


			auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
			m_RT->FillOpacityMask(mask.Get(), brush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS);
		}
		else
		{
			/* No mask for the geometry */
			PerPrimitiveAliasingScope perPrimitive(m_RT);

			auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
			auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
			ID2D1StrokeStyle* strokeStyle = nullptr;

			if (m_strokeStyle)
			{
				strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(m_strokeStyle);
			}

			m_RT->DrawGeometry(geometry, brush, m_thickness, strokeStyle);
		}
	}
}

void StrokeCommand1::SetStrokeStyle(const StrokeStylePtr& strokeStyle)
{
	m_strokeStyle = strokeStyle;
}

void StrokeCommand1::SetThickness(float thickness)
{
	m_thickness=thickness;
}