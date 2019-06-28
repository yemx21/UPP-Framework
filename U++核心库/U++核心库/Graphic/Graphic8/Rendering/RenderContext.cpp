#include "RenderContext.h"
#include "BrushTransformToBounds.h"
#include "DrawImageCommand.h"
#include "DrawEffectCommand.h"
#include "DrawTextCommand.h"


using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace Microsoft::WRL;
using namespace std;
using namespace D2D1;

ComPtr<ID2D1Factory1>  RenderContext::m_d2dFactory;
ComPtr<IDWriteFactory> RenderContext::m_directWriteFactory;

auto RenderContext::GetDirectWriteFactory() -> const ComPtr<IDWriteFactory>
{
    if(!m_directWriteFactory)
    {
        HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                                         __uuidof(IDWriteFactory),
                                         reinterpret_cast<IUnknown**>(m_directWriteFactory.ReleaseAndGetAddressOf()));
		if (hr != S_OK)
		{
			m_directWriteFactory = nullptr;
			return nullptr;
		}
    }

    return m_directWriteFactory;
}

auto RenderContext::GetFactory() -> const ComPtr<ID2D1Factory1>& 
{
    if(!m_d2dFactory)
    {
        D2D1_FACTORY_OPTIONS options;
#if _DEBUG
        options.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
#else
        options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,  
                                       options,
                                       m_d2dFactory.ReleaseAndGetAddressOf());

		if(hr != S_OK)
		{
			m_d2dFactory = nullptr;
			return nullptr;
		}
               
    }

    return m_d2dFactory;
}

RenderContext::RenderContext(const ComPtr<ID3D11Device>& device) :m_lastRenderTarget(nullptr)
{
    m_device = device;

    Initialize();
}

RenderContext::~RenderContext(void)
{
}

void RenderContext::Initialize()
{
    ComPtr<IDXGIDevice> dxgiDevice;
    m_device.As(&dxgiDevice);
                            
    HRESULT hr = S_OK;
    auto factory = GetFactory();
    
    hr = m_d2dFactory->CreateDevice(dxgiDevice.Get(),
                                    m_d2dDevice.ReleaseAndGetAddressOf());

	if(hr != S_OK)
	{
		throw std::exception("dxgiDevice error");
	}
	
    hr = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, 
                                          &m_d2dDeviceContext);

	if(hr != S_OK)
	{
		throw std::exception("d2dDeviceContext error");
	}


	m_d2dDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
	m_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

}

HRESULT RenderContext::GetDevice(ComPtr<ID3D11Device>& device)
{
    device = m_device;
    return S_OK;
}

HRESULT RenderContext::Get2DDevice(ComPtr<ID2D1Device>& device)
{
    device = m_d2dDevice;
    return S_OK;
}

void* RenderContext::GetCurrentRenderTarget() const
{
	return m_lastRenderTarget;
}

bool RenderContext::GetCurrentTargetSize(System::Size& size) const
{
	auto bmp = m_renderStateQueue.front().renderTarget;
	if (bmp)
	{
		size=bmp->GetSize();
		return true;
	}
	return false;
}

void RenderContext::SetTransform(const Matrix3x2F& matrix)
{
    m_d2dDeviceContext->SetTransform(matrix);
}

void RenderContext::SetTransform(const System::Matrix& matrix)
{
	m_d2dDeviceContext->SetTransform(D2DMatrix(matrix));
}

bool RenderContext::PushRenderSurface(const IDXGISurfacePtr& surface)
{
	//ID3D11Device* d3dDevice = nullptr;
	//surface->GetDevice(__uuidof(ID3D11Device), (void**)&d3dDevice);
	//ID3D11Debug *d3dDebug = nullptr;
	//if (SUCCEEDED(d3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug)))
	//{
	//	d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	//}
	//d3dDebug->Release();
	//d3dDevice->Release();

    auto renderBitmap = CreateBitmap(surface);
	if (!renderBitmap) return false;
    PushRenderTarget(renderBitmap, false);
	m_lastRenderTarget = surface.Get();
    return true;
}

auto RenderContext::GetRenderBitmap() -> BitmapPtr
{
    return m_renderStateQueue.front().renderTarget;
}

auto RenderContext::Get2DDeviceContext() const -> const ID2D1DeviceContextPtr& 
{
    return m_d2dDeviceContext;
}

void RenderContext::DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const PenPtr& pen)
{
    if(brush)
    {
        BrushTransformToBounds brushTransformSwap;
        brushTransformSwap.Setup(brush.get(), geometry, pen);

        FillCommand fillCommand;
        fillCommand.SetRenderTarget(m_d2dDeviceContext.Get());
        fillCommand.SetBrush(brush.get());
        fillCommand.SetGeometry(geometry.get());
        fillCommand.Run();
    }

    if(pen)
    {
        auto strokeThickness = pen->Thickness();

        if(strokeThickness > 0)
        {
            auto strokeBrush = pen->Brush();

            BrushTransformToBounds brushTransformSwap;
            brushTransformSwap.Setup(strokeBrush.get(), geometry, pen);

            StrokeCommand strokeCommand;
            strokeCommand.SetRenderTarget(m_d2dDeviceContext.Get());
            strokeCommand.SetBrush(pen->Brush().get());
            strokeCommand.SetGeometry(geometry.get());
            strokeCommand.SetPen(pen);
            strokeCommand.Run();
        }
    }
}

void RenderContext::DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const BrushPtr& stroke, const StrokeStylePtr& strokeStyle, float thickness)
{
	if (brush)
	{
		BrushTransformToBounds brushTransformSwap;
		brushTransformSwap.Setup(brush.get(), geometry,strokeStyle,thickness);

		FillCommand fillCommand;
		fillCommand.SetRenderTarget(m_d2dDeviceContext.Get());
		fillCommand.SetBrush(brush.get());
		fillCommand.SetGeometry(geometry.get());
		fillCommand.Run();
	}

	if (strokeStyle)
	{
		if (thickness > 0)
		{
			BrushTransformToBounds brushTransformSwap;
			brushTransformSwap.Setup(stroke.get(), geometry,strokeStyle, thickness);

			StrokeCommand1 strokeCommand;
			strokeCommand.SetRenderTarget(m_d2dDeviceContext.Get());
			strokeCommand.SetBrush(stroke.get());
			strokeCommand.SetGeometry(geometry.get());
			strokeCommand.SetStrokeStyle(strokeStyle);
			strokeCommand.SetThickness(thickness);
			strokeCommand.Run();
		}
	}
}

void RenderContext::Clear(const System::Color& c)
{
    m_d2dDeviceContext->Clear(D2DColor(c));
}

void RenderContext::PushAxisAlignedClip(const System::Rect& clipRect, AntialiasMode mode)
{
    auto rect = clipRect;
    Matrix3x2F currentTransform;
    m_d2dDeviceContext->GetTransform(&currentTransform);

	m_d2dDeviceContext->PushAxisAlignedClip(D2DRect(rect), (D2D1_ANTIALIAS_MODE)(UInt)mode);
    auto clipState = ClipState(clipRect, currentTransform);

    auto& currentState = m_renderStateQueue.front();
    currentState.clipQueue.push_back(clipState);
}

void RenderContext::PopAxisAlignedClip()
{
    m_d2dDeviceContext->PopAxisAlignedClip();
    auto& currentState = m_renderStateQueue.front();
    currentState.clipQueue.pop_back();
}

void RenderContext::PopLayer()
{
	auto layer = m_layers.top();
	m_layers.pop();
	if (layer)
	{
		m_d2dDeviceContext->PopLayer();
		m_layerPool.push(layer);
	}
}

bool RenderContext::PushGeometryClip(const GeometryPtr& geometry)
{
	auto native_geometry = geometry->GetResource<ID2D1Geometry>();
	if (native_geometry)
	{
		ComPtr<ID2D1Layer> layerptr = nullptr;
		if (!m_layerPool.empty())
		{
			layerptr = m_layerPool.top();
			m_layerPool.pop();
			return true;
		}
		else
		{
			if(m_d2dDeviceContext->CreateLayer(&layerptr) == S_OK)
			{
				m_d2dDeviceContext->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(), native_geometry), layerptr.Get());
				m_layers.push(layerptr);
				return true;
			}
		}
	}
	return false;
}

void RenderContext::PopGeometryClip()
{
	PopLayer();
}

bool RenderContext::PushOpacity(float opacity)
{
	if (opacity<1.0f)
	{
		ComPtr<ID2D1Layer> layerptr = nullptr;
		if (!m_layerPool.empty())
		{
			layerptr = m_layerPool.top();
			m_layerPool.pop();
			return true;
		}
		else
		{
			if (m_d2dDeviceContext->CreateLayer(&layerptr) == S_OK)
			{
				m_d2dDeviceContext->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(),0, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(), opacity), layerptr.Get());
				m_layers.push(layerptr);
				return true;
			}
		}
	}
	return false;
}

void RenderContext::PopOpacity()
{
	PopLayer();
}

bool RenderContext::PushOpacityMask(const System::Rect& rect, const BrushPtr& brush)
{
	auto native_brush = brush->GetResource<ID2D1Brush>(m_d2dDeviceContext.Get());
	if (native_brush)
	{
		ComPtr<ID2D1Layer> layerptr = nullptr;
		if (!m_layerPool.empty())
		{
			layerptr = m_layerPool.top();
			m_layerPool.pop();
			return true;
		}
		else
		{
			if (m_d2dDeviceContext->CreateLayer(&layerptr) == S_OK)
			{
				m_d2dDeviceContext->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(),0, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(), 1.0f, native_brush), layerptr.Get());
				m_layers.push(layerptr);
				return true;
			}
		}
	}
	return false;
}

void RenderContext::PopOpacityMask()
{
	PopLayer();
}

auto RenderContext::CreateBitmap(const ComPtr<IDXGISurface>& surface) ->BitmapPtr 
{
     auto bitmap = make_shared<Bitmap>(surface);
     bitmap->SetDeviceContext(m_d2dDeviceContext.Get());
     return bitmap;
}

void RenderContext::PopRenderTarget()
{
    m_d2dDeviceContext->EndDraw();
    m_d2dDeviceContext->SetTarget(nullptr);
    m_renderStateQueue.pop_front();

    if(!m_renderStateQueue.empty())
    {
        auto& state = m_renderStateQueue.front();

        SetRenderTarget(state.renderTarget);
        m_d2dDeviceContext->BeginDraw();
        for(size_t i = 0; i < state.clipQueue.size(); i++)
        {
            state.clipQueue[i].PushClip(m_d2dDeviceContext);
        }
    }
}

void RenderContext::SetRenderTarget(const BitmapPtr& renderTarget)
{
    m_d2dDeviceContext->SetTarget(renderTarget->GetResource<ID2D1Bitmap>(m_d2dDeviceContext.Get()));
}

void RenderContext::PushRenderTarget(const BitmapPtr& renderTarget, bool clear)
{
    if(!m_renderStateQueue.empty())
    {
        auto& currentState = m_renderStateQueue.front();
        
        for(size_t i = currentState.clipQueue.size(); i-- > 0;)
        {
            currentState.clipQueue[i].PopClip(m_d2dDeviceContext);
        }

        m_d2dDeviceContext->EndDraw();
    }

    m_renderStateQueue.emplace_front(renderTarget);
   
    SetRenderTarget(renderTarget);
    m_d2dDeviceContext->BeginDraw();

    if(clear)
        Clear(Colors::Transparent);
}


void RenderContext::DrawBitmap(const BitmapPtr& bitmap, 
                               const EffectCompositeMode compositeMode, 
                               const System::Size& targetSize)
{
    DrawImageCommand command;

    command.Bitmap(bitmap.get());
    command.CompositeMode(compositeMode);
    command.TargetSize(targetSize);
    command.DeviceContext(m_d2dDeviceContext.Get());
    
    command.Run();
}


void RenderContext::DrawEffect(const BitmapPtr& bitmap, 
                               const ElementEffectPtr& elementEffect, 
                               const EffectCompositeMode compositeMode, 
                               const System::Size& targetSize)
{
    DrawEffectCommand command;

    command.Bitmap(bitmap);
    command.ElementEffect(elementEffect.get());
    command.CompositeMode(compositeMode);
    command.TargetSize(targetSize);
    command.DeviceContext(m_d2dDeviceContext.Get());
    
    command.Run();
}


void RenderContext::DrawText(const TextLayoutPtr& textLayout, 
                             const BrushPtr& brush, 
                             const System::Point& origin)
{
    DrawTextCommand command;
    command.DeviceContext(m_d2dDeviceContext);
    command.Brush(brush);
    command.Origin(origin);
    command.TextLayout(textLayout);

    command.Run();
}

auto RenderContext::MaximumImageSize() const -> const int
{
    return m_d2dDeviceContext->GetMaximumBitmapSize();
}
