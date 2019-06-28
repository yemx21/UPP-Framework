#pragma once
#include "..\Header.h"
#include "..\Resource.h"
#include <ppltasks.h>
#include <wrl.h>
#include <wincodec.h>
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Bitmap;
				typedef std::shared_ptr<Bitmap>  BitmapPtr;

				typedef Microsoft::WRL::ComPtr<IWICBitmapSource> WICBitmapSourcePtr;

				class Bitmap :public DeviceDependantResource
				{
				public:
					Bitmap(const WICBitmapSourcePtr& bitmapSource);
					Bitmap(const Microsoft::WRL::ComPtr<IDXGISurface>& surface);
					Bitmap(const Size& size);
					virtual ~Bitmap();
				public:
					static Concurrency::task<BitmapPtr> LoadFromPathAsync(std::wstring& path);
					static Result<BitmapPtr> Bitmap::LoadFromPath(std::wstring& path);
					static Result<BitmapPtr> Bitmap::LoadFromPath(const wchar_t* path);
				public:
					auto GetSize() const -> const Size&;

					Result<GraphicError> Map(D3D11_MAPPED_SUBRESOURCE* data, ID3D11DeviceContext* d3dDeviceContext, unsigned int& w, unsigned int& h);
					Result<GraphicError> Unmap(ID3D11DeviceContext* d3dDeviceContext);

				private:
					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr < IUnknown > ;
				private:
					mutable Size m_size;
					WICBitmapSourcePtr  m_bitmapSource;
					char flag;
					Microsoft::WRL::ComPtr<IDXGISurface> m_surface;
				};
			}
		}
	}
}