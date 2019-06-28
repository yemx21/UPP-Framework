#include "StrokeStyle.h"
#include <wrl.h>
#include "..\Rendering\RenderContext.h"

using namespace Microsoft::WRL;
using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace D2D1;

StrokeStyle::StrokeStyle(void) : m_startCap   (CapStyle::Flat), 
m_endCap(CapStyle::Flat),
m_dashCap(CapStyle::Flat),
m_lineJoin(LineJoin::Miter),
m_dashStyle(DashStyle::Solid),
                                 m_miterLimit (0),
                                 m_dashOffset (0), 
								 m_dashes(NULL),
								 m_dashCount(0),
								 m_dashCacheSize(0),
                                 m_factory    (RenderContext::GetFactory())
{
	InitializeCriticalSection(&cs);
}


StrokeStyle::~StrokeStyle(void)
{
	LeaveCriticalSection(&cs); 
	DeleteCriticalSection(&cs);
	SAFE_FREE(m_dashes);
	m_dashCacheSize = 0;
	m_dashCount = 0;
}

auto StrokeStyle::OnCreateResource() -> ComPtr<IUnknown> 
{
    ComPtr<ID2D1StrokeStyle> strokeStyle;
	EnterCriticalSection(&cs);
    HRESULT hr = S_OK;
	hr = m_factory->CreateStrokeStyle(StrokeStyleProperties((D2D1_CAP_STYLE)(UInt)m_startCap,
		(D2D1_CAP_STYLE)(UInt)m_endCap,
															(D2D1_CAP_STYLE)(UInt)m_dashCap,
                                                            (D2D1_LINE_JOIN)(UInt)m_lineJoin, 
                                                            m_miterLimit, 
															(D2D1_DASH_STYLE)(UInt)m_dashStyle,
                                                            m_dashOffset), 
                                                            m_dashes, 
                                                            m_dashCount,
                                                            strokeStyle.GetAddressOf());
	LeaveCriticalSection(&cs);

    if(FAILED(hr))
        throw std::exception("Failed to create StrokeStyle");

    return strokeStyle;
}

CapStyle StrokeStyle::StartCap() const
{
    return m_startCap;
}

void StrokeStyle::StartCap(CapStyle val)
{
    m_startCap = val;
    Invalidate();
}

CapStyle StrokeStyle::EndCap() const
{
    return m_endCap;
}

void StrokeStyle::EndCap(CapStyle val)
{
    m_endCap = val;
    Invalidate();
}

CapStyle StrokeStyle::DashCap() const
{
    return m_dashCap;
}

void StrokeStyle::DashCap(CapStyle val)
{
    m_dashCap = val;
    Invalidate();
}

LineJoin StrokeStyle::LineJoinStyle() const
{
    return m_lineJoin;
}

void StrokeStyle::LineJoinStyle(LineJoin val)
{
    m_lineJoin = val;
    Invalidate();
}

float StrokeStyle::MiterLimit() const
{
    return m_miterLimit;
}

DashStyle StrokeStyle::LineDashStyle() const
{
    return m_dashStyle;
}

float StrokeStyle::DashOffset() const
{
    return m_dashOffset;
}

void StrokeStyle::DashOffset(float val)
{
    m_dashOffset = val;
    Invalidate();
}

void StrokeStyle::Dashes(const float* val, unsigned int count)
{
	EnterCriticalSection(&cs);
	if (val)
	{
		if (m_dashes)
		{
			unsigned int need = count * sizeof(float);
			if (m_dashCacheSize < need)
			{
				float* old = m_dashes;
				if ((m_dashes = (float*)realloc(m_dashes, need)) == NULL)
				{
					free(old);
					old = NULL;
					m_dashes = (float*)malloc(need);
				}
				if (m_dashes) m_dashCacheSize = need;	
			}
			if (m_dashes)
			{
				m_dashCount = count;
				memcpy_s(m_dashes, sizeof(m_dashes), val, count* sizeof(float));
			}
		}
	}
	else
	{
		m_dashCacheSize = 0;
		m_dashCount = 0;
		SAFE_FREE(m_dashes);
	}
	
	LeaveCriticalSection(&cs);
	Invalidate();
}

float* StrokeStyle::Dashes(unsigned int* count) const
{
	*count = m_dashCount;
	return m_dashes;
}
