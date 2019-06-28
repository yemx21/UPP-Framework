#include "Bitmap.h"
#include <shcore.h>
#include <memory>
using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace Microsoft::WRL;
using namespace Concurrency;
using namespace std;

ComPtr<IUnknown> Bitmap::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
	ComPtr<ID2D1Bitmap1> bitmap;
	
    if(m_bitmapSource)
	{
		if (deviceContext->CreateBitmapFromWicBitmap(m_bitmapSource.Get(), bitmap.GetAddressOf()) != S_OK)
		{
			bitmap = nullptr;
			return nullptr;
		}
    }
    else if(m_surface)
    {
        D2D1_BITMAP_PROPERTIES1 props;
        float dpiX, dpiY;
        deviceContext->GetDpi(&dpiX, &dpiY);
        
		props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY,
			nullptr);
		HRESULT hr = deviceContext->CreateBitmapFromDxgiSurface(m_surface.Get(), &props, bitmap.GetAddressOf());
        
		if(hr != S_OK)
		{
			bitmap = nullptr;
			return nullptr;
		}
        
        DXGI_SURFACE_DESC desc;
        m_surface->GetDesc(&desc);
        m_size.Width = desc.Width;
        m_size.Height = desc.Height;
    }
    else
    {
        D2D1_SIZE_U size = {m_size.Width, m_size.Height};
        D2D1_BITMAP_PROPERTIES1 props;
        float dpiX, dpiY;
        deviceContext->GetDpi(&dpiX, &dpiY);
        
		props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY,
			nullptr);


		ComPtr<ID2D1Bitmap1> bmp = nullptr;
		HRESULT hr = deviceContext->CreateBitmap(size, nullptr, 0, props, bmp.GetAddressOf());
		if(hr != S_OK)
		{
			bitmap = nullptr;
			return nullptr;
		}
        return bmp;
    }

    return bitmap;
}

Bitmap::Bitmap(const WICBitmapSourcePtr& bitmapSource) :flag(1)
{
    m_bitmapSource = bitmapSource;

    D2D1_SIZE_U size;

    m_bitmapSource->GetSize(&size.width, &size.height);

    m_size.Width = size.width;
    m_size.Height = size.height;
}

Bitmap::Bitmap(const ComPtr<IDXGISurface>& surface) :flag(2)
{
    m_surface = surface;
}

Bitmap::Bitmap(const System::Size& size) : flag(3)
{
    m_size = size;
}

Bitmap::~Bitmap()
{
	
}

const System::Size& Bitmap::GetSize() const
{
    return m_size;
}

Concurrency::task<BitmapPtr> Bitmap::LoadFromPathAsync( std::wstring& path )
{
    return Concurrency::task<BitmapPtr>([=]
    {
        ComPtr<IWICImagingFactory> pFactory;
        ComPtr<IWICBitmapDecoder>  pDecoder; 
        ComPtr<IStream>            istream;

        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            (LPVOID*)&pFactory);

        if(FAILED(hr))
            throw std::exception("Could not create WIC factory");
   
        ComPtr<IWICBitmapDecoder> decoder;
        hr = pFactory->CreateDecoderFromFilename(
                path.c_str(),
                nullptr,
                GENERIC_READ,
                WICDecodeMetadataCacheOnDemand,
                &decoder
                );

        ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);

        if(FAILED(hr))
            throw std::exception("Could not CreateStreamOverRandomAccessStream");

        ComPtr<IWICFormatConverter> converter;
        hr = pFactory->CreateFormatConverter(&converter);
            
        if(FAILED(hr))
            throw std::exception("Could not CreateFormatConverter");

        hr = converter->Initialize(
            frame.Get(),
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0f,
            WICBitmapPaletteTypeCustom
            );

            if(FAILED(hr))
            throw std::exception("Could not initialize WIC converter");

        auto bitmap = make_shared<Bitmap>(converter);

        return bitmap;
    });
}

Result<BitmapPtr> Bitmap::LoadFromPath(std::wstring& path)
{
	return Bitmap::LoadFromPath(path.c_str());
}

Result<BitmapPtr> Bitmap::LoadFromPath(const wchar_t* path)
{
	Result<BitmapPtr> result(nullptr);
	ComPtr<IWICImagingFactory> pFactory;
	ComPtr<IWICBitmapDecoder>  pDecoder;
	ComPtr<IStream>            istream;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pFactory);

	if (FAILED(hr))
	{
		result.WriteMessage(L"Could not create WIC factory");
		return result;
	}

	ComPtr<IWICBitmapDecoder> decoder;
	hr = pFactory->CreateDecoderFromFilename(
		path,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder
		);

	if (FAILED(hr))
	{
		result.WriteMessage(L"Can not find file");
		return result;
	} 

	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);

	if (FAILED(hr))
	{
		result.WriteMessage(L"Could not CreateStreamOverRandomAccessStream");
		return result;
	}

	ComPtr<IWICFormatConverter> converter;
	hr = pFactory->CreateFormatConverter(&converter);

	if (FAILED(hr))	
	{
		result.WriteMessage(L"Could not CreateFormatConverter");
		return result;
	}


	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
		);

	if (FAILED(hr))
	{
		result.WriteMessage(L"Could not initialize WIC converter");
		return result;
	}
	result.Value = make_shared<Bitmap>(converter);

	return result;
}


Result<GraphicError> Bitmap::Map(D3D11_MAPPED_SUBRESOURCE*  data, ID3D11DeviceContext* d3dDeviceContext, unsigned int& w, unsigned int& h)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (data)
	{
		if (d3dDeviceContext)
		{
			ID2D1Bitmap1* bmp = GetResourceUnsafe<ID2D1Bitmap1>();
			if (bmp)
			{
				if (flag == 2)
				{
					ID3D11Resource* res = nullptr;//dispose
					IDXGISurface* surface = m_surface.Get();
					if (surface)
					{
						if (surface->QueryInterface(&res) != S_OK)
						{
							result.Value = GraphicError::Unsupported;
							result.WriteMessage(L"ID3D11Resource interface is invalid");
							return result;
						}
						if (S_OK != d3dDeviceContext->Map(res, 0, D3D11_MAP::D3D11_MAP_READ, 0, data))
							result.WriteMessage(L"map read failed");
						else
						{
							result.Value = GraphicError::OK;
							DXGI_SURFACE_DESC suf;
							surface->GetDesc(&suf);
							w = suf.Width;
							h = suf.Height;
							res->Release();//disposed
						}
					}
					else
					{
						result.Value = GraphicError::Unsupported;
						result.WriteMessage(L"can not get IDXGISurface interface");
					}
				}
				else if (flag == 1)
				{
					D2D1_MAPPED_RECT rect;
					if (S_OK == bmp->Map(D2D1_MAP_OPTIONS_READ, &rect))
					{
						data->pData = rect.bits;
						data->DepthPitch = rect.pitch;
						data->RowPitch = rect.pitch;
						auto s = bmp->GetPixelSize();
						w = s.width;
						h = s.height;
						result.Value = GraphicError::OK;
					}
					else
						result.WriteMessage(L"map read failed");
				}
				else if (flag == 3)
				{					
					result.WriteMessage(L"unsupported");
				}
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	else
		result.Value = GraphicError::InvalidParameters;
	return result;
}

Result<GraphicError> Bitmap::Unmap(ID3D11DeviceContext* d3dDeviceContext)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (d3dDeviceContext)
	{
		ID2D1Bitmap1* bmp = GetResourceUnsafe<ID2D1Bitmap1>();
		if (bmp)
		{
			IDXGISurface* surface = nullptr;//dispose
			if (bmp->QueryInterface(&surface) != S_OK)
			{
				result.Value = GraphicError::Unsupported;
				result.WriteMessage(L"can not get IDXGISurface interface");
				return result;
			}
			if (!surface)
			{
				result.Value = GraphicError::Unsupported;
				result.WriteMessage(L"IDXGISurface interface is invalid");
				return result;
			}

			ID3D11Resource* res = nullptr;//dispose
			if (surface->QueryInterface(&res) != S_OK)
			{
				surface->Release();//disposed
				result.Value = GraphicError::Unsupported;
				result.WriteMessage(L"ID3D11Resource interface is invalid");
				return result;
			}
			d3dDeviceContext->Unmap(res, 0);
			surface->Release();//disposed
			res->Release();//disposed
			result.Value = GraphicError::OK;
		}
	}
	else
		result.Value = GraphicError::InvalidParameters;
	return result;
}