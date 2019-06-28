#pragma once

#include "FillCommand.h"
#include "StrokeCommand.h"
#include "RenderState.h"

#include "..\Brush\RadialGradientBrush.h"
#include "..\Brush\GradientStopCollection.h"
#include "..\Media\Bitmap.h"

#include "..\Effects\EffectResource.h"
#include "..\Effects\ElementEffect.h"
#include "..\Brush\Pen.h"
#include "..\Text\TextLayout.h"

#include <memory>
#include <wrl.h>
#include <queue>


namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class RenderContext;
				typedef std::shared_ptr<RenderContext> RenderContextPtr;

				class RenderContext
				{
					typedef Microsoft::WRL::ComPtr<IDXGISurface>        IDXGISurfacePtr;
					typedef Microsoft::WRL::ComPtr<ID3D11Device>        ID3D11DevicePtr;
					typedef Microsoft::WRL::ComPtr<ID2D1Device>         ID2D1DevicePtr;
					typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext>  ID2D1DeviceContextPtr;
					typedef Microsoft::WRL::ComPtr<ID2D1Factory1>       ID2D1Factory1Ptr;
					typedef Microsoft::WRL::ComPtr<IDWriteFactory>      IDWriteFactoryPtr;
				public:
					RenderContext(const ID3D11DevicePtr& device);
					~RenderContext(void);
				public:
					bool PushRenderSurface(const IDXGISurfacePtr& surface);
					auto GetTarget()->BitmapPtr&;
					void* GetCurrentRenderTarget() const;
					bool GetCurrentTargetSize(System::Size&) const;

					auto GetDevice(ID3D11DevicePtr& device)->HRESULT;
					auto Get2DDevice(ID2D1DevicePtr& device)->HRESULT;

					auto Get2DDeviceContext() const                           -> const ID2D1DeviceContextPtr&;

					auto GetRenderBitmap()->BitmapPtr;
					static auto GetFactory()                                  -> const ID2D1Factory1Ptr&;
					static auto GetDirectWriteFactory()                       -> const IDWriteFactoryPtr;
				public:
					virtual auto CreateBitmap(const IDXGISurfacePtr& surface)->BitmapPtr;
				public:
					virtual void Clear(const System::Color& c);
					virtual void DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const PenPtr& pen);
					virtual void DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const BrushPtr& stroke, const StrokeStylePtr& strokeStyle, float thickness);
					virtual void SetTransform(const D2D1::Matrix3x2F& matrix);
					virtual void SetTransform(const System::Matrix& matrix);
					virtual void SetRenderTarget(const BitmapPtr& renderTarget);
					virtual void DrawText(const TextLayoutPtr& textLayout, const BrushPtr& brush, const System::Point& origin);
					virtual void DrawBitmap(const BitmapPtr& bitmap, const EffectCompositeMode compositeMode, const System::Size& targetSize);


					void PushRenderTarget(const BitmapPtr& renderTarget, bool clear = true);
					void PopRenderTarget();

					void PushAxisAlignedClip(const System::Rect& rect, AntialiasMode mode);
					void PopAxisAlignedClip();

					bool PushGeometryClip(const GeometryPtr& geometry);
					void PopGeometryClip();

					bool PushOpacity(float opacity);
					void PopOpacity();

					bool PushOpacityMask(const System::Rect& rect, const BrushPtr& brush);
					void PopOpacityMask();

					void DrawEffect(const BitmapPtr& bitmap, const ElementEffectPtr& elementEffect, const EffectCompositeMode compositeMode, const System::Size& size);

					auto MaximumImageSize() const -> const int;

				private:
					void Initialize();
					void PopLayer();
				private:
					static Microsoft::WRL::ComPtr<ID2D1Factory1>  m_d2dFactory;
					static Microsoft::WRL::ComPtr<IDWriteFactory> m_directWriteFactory;
					Microsoft::WRL::ComPtr<ID3D11Device>          m_device;
					Microsoft::WRL::ComPtr<ID2D1Device>           m_d2dDevice;
					Microsoft::WRL::ComPtr<ID2D1DeviceContext>    m_d2dDeviceContext;
					std::deque<RenderState>                       m_renderStateQueue;
					std::stack<Microsoft::WRL::ComPtr<ID2D1Layer>> m_layers;
					std::stack<Microsoft::WRL::ComPtr<ID2D1Layer>> m_layerPool;
					void*  m_lastRenderTarget;
				};
			}
		}
	}
}
