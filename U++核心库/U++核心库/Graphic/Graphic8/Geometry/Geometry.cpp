#include "Geometry.h"
#include "..\Rendering\RenderContext.h"
using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;


Geometry::Geometry() : m_geometryCacheEnabled(false), m_transform(Matrix::Identity())
{
    m_Factory = RenderContext::GetFactory();
}

bool Geometry::HitTest(const Point& point, const Matrix& matrix)
{
    BOOL result = false;
    GetResource<ID2D1Geometry>()->FillContainsPoint(D2DPoint(point), 
                                                    D2DMatrix(matrix), &result);

    return result != 0;
}

bool Geometry::HitTestStroke(const Point& point, const PenPtr& pen, const Matrix& matrix)
{
    ID2D1StrokeStyle* strokeStyle = nullptr;
    BOOL result = false;
    if(pen->StrokeStyle())
        strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(pen->StrokeStyle());

	GetResource<ID2D1Geometry>()->StrokeContainsPoint(D2DPoint(point),
                                                      pen->Thickness(), 
                                                      strokeStyle, 
													  D2DMatrix(matrix),
                                                      &result);

    return result != 0;
}

bool Geometry::HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness, const Matrix& matrix)
{
	ID2D1StrokeStyle* ss = strokeStyle != NULL ? GetResourceFrom<ID2D1StrokeStyle>(strokeStyle) : nullptr;
	BOOL result = false;

	GetResource<ID2D1Geometry>()->StrokeContainsPoint(D2DPoint(point),
		thickness,
		ss,
		D2DMatrix(matrix),
		&result);

	return result != 0;
}


auto Geometry::GetFillMask(ID2D1DeviceContext* rt) -> ID2D1BitmapPtr
{
    if(!m_geometryCacheEnabled)
    {
        return nullptr;
    }

    if (!m_FillMask)
        m_FillMask = CreateFillMask(rt);

	return (m_FillMask ? m_FillMask->Mask : ID2D1BitmapPtr());
}


auto Geometry::GetStrokeMask(ID2D1DeviceContext* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle) -> ID2D1BitmapPtr
{
    if(!m_geometryCacheEnabled)
    {
        return nullptr;
    }

    if (m_StrokeMasks.find(strokeWidth) == m_StrokeMasks.end())
    {
        MaskDataPtr mask = nullptr;
            
        mask = CreateStrokeMask(rt, strokeWidth, strokeStyle);
           
        m_StrokeMasks[strokeWidth] = mask;
    }
	return (m_StrokeMasks[strokeWidth] ? m_StrokeMasks[strokeWidth]->Mask : ID2D1BitmapPtr());
}

auto Geometry::GetStrokeMaskTransform(float strokeWidth) -> const Matrix
{
    if (m_StrokeMasks.find(strokeWidth) != m_StrokeMasks.end())
    {
        return m_StrokeMasks[strokeWidth]->Transform;
    }
	else
    {
        return Matrix::Identity();
    }
}

auto Geometry::CreateFillMask(ID2D1DeviceContext* rt) -> MaskDataPtr 
{
    auto geometry = GetResource<ID2D1Geometry>();

    D2D1_RECT_F boundingBox;
    geometry->GetBounds(D2D1::IdentityMatrix(), &boundingBox);

    if (boundingBox.left > boundingBox.right)
    {
        // empty bounds
        return MaskDataPtr();
    }

    //we grow the rectangle by one unit on all sizes 
    //so that the mask alpha smoothly falls off to 0 at boundaries

	boundingBox.left -= 1;
	boundingBox.top -= 1;
	boundingBox.right += 1;
	boundingBox.bottom += 1;

    auto size = D2D1::SizeF(boundingBox.right - boundingBox.left, 
                            boundingBox.bottom - boundingBox.top);

    if ((size.width > m_MaskUsageThreshold) || (size.height > m_MaskUsageThreshold))
    {
        // too big to use bitmap masks
        return MaskDataPtr();
    }

    // create intermediate render target. We only set a desired dip size, so our render target
    // actually may contain more pixels in high dpi.
    auto pf = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    ComPtr<ID2D1BitmapRenderTarget> intermediate;

	if (rt->CreateCompatibleRenderTarget(&size,
		NULL/*desired pixel size*/,
		&pf,
		D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
		&intermediate) != S_OK)
	{
		intermediate = nullptr;
		return MaskDataPtr();
	}

    // create opaque brush
    ComPtr<ID2D1SolidColorBrush> opaqueBrush;
	if(intermediate->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1), &opaqueBrush) != S_OK)
	{
		opaqueBrush = nullptr;
		intermediate = nullptr;
		return MaskDataPtr();
	}

    // render to intermediate
    intermediate->BeginDraw();
	intermediate->Clear(D2D1::ColorF(0, 0, 0, 0));

    // move geometry inside the render target
	intermediate->SetTransform(D2D1::Matrix3x2F::Translation(-boundingBox.left,
                                                       -boundingBox.top));

    auto fillMaskTransform = Matrix::CreateTranslation(boundingBox.left, boundingBox.top);

    // render
    intermediate->FillGeometry(GetResource<ID2D1Geometry>(), opaqueBrush.Get());

	if(intermediate->EndDraw() != S_OK)
	{
		opaqueBrush = nullptr;
		intermediate = nullptr;
		return MaskDataPtr();
	}

    ComPtr<ID2D1Bitmap> mask;
	if(intermediate->GetBitmap(&mask) != S_OK)
	{
		intermediate = nullptr;
		mask = nullptr;
		return MaskDataPtr();
	}

    return MaskDataPtr(new MaskData(mask, fillMaskTransform));
}

auto Geometry::CreateStrokeMask(ID2D1DeviceContext* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle) -> MaskDataPtr
{
    auto geometry = GetResource<ID2D1Geometry>();

    D2D1_RECT_F boundingBox;
    geometry->GetWidenedBounds(strokeWidth, 
                               strokeStyle, 
                               nullptr/*transform*/, 
                               &boundingBox);

    if (boundingBox.left > boundingBox.right)
    {
        // empty bounds
        return MaskDataPtr();
    }

    //we grow the rectangle by one unit on all sizes 
    //so that the mask alpha smoothly falls off to 0 at boundaries
	boundingBox.left -= 1;
	boundingBox.top -= 1;
	boundingBox.right += 1;
	boundingBox.bottom += 1;

	auto size = D2D1::SizeF(boundingBox.right - boundingBox.left,
                      boundingBox.bottom - boundingBox.top);

    if ((size.width > m_MaskUsageThreshold) || 
        (size.height > m_MaskUsageThreshold))
    {
        // too big to use bitmap masks
        return MaskDataPtr();
    }

    // create intermediate render target and brush
    auto pf = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    ComPtr<ID2D1BitmapRenderTarget> intermediate;
	if(rt->CreateCompatibleRenderTarget(&size,
		NULL/*desired pixel size*/,
		&pf,
		D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
		&intermediate) != S_OK)
	{
		intermediate = nullptr;
		return MaskDataPtr();
	}
        

    // create opaque brush
    ComPtr<ID2D1SolidColorBrush> opaqueBrush;
	if(intermediate->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1), &opaqueBrush) != S_OK)
	{
		intermediate = nullptr;
		opaqueBrush = nullptr;
		return MaskDataPtr();
	}
        

    // render to intermediate
    intermediate->BeginDraw();
    intermediate->Clear(D2D1::ColorF(0, 0, 0, 0));

    // move geometry inside the render target
	intermediate->SetTransform(D2D1::Matrix3x2F::Translation(-boundingBox.left, -boundingBox.top));
    auto strokeMaskTransform = Matrix::CreateTranslation(boundingBox.left, boundingBox.top);

    // render
    intermediate->DrawGeometry(geometry, opaqueBrush.Get(), strokeWidth);

	if(intermediate->EndDraw() != S_OK)
	{
		intermediate = nullptr;
		opaqueBrush = nullptr;
		return MaskDataPtr();
	}

    ComPtr<ID2D1Bitmap> mask;
	if(intermediate->GetBitmap(&mask) != S_OK)
	{
		intermediate = nullptr;
		opaqueBrush = nullptr;
		mask = nullptr;
		return MaskDataPtr();
	}

    return MaskDataPtr(new MaskData(mask, strokeMaskTransform));
}

auto Geometry::GetBounds(const PenPtr& pen, const Matrix& worldTransform) -> Rect
{
    auto geometry = GetResource<ID2D1Geometry>();
    D2D1_RECT_F bounds;

    if(pen)
    {
        ID2D1StrokeStyle* strokeStyle = nullptr;

        if(pen->StrokeStyle())
            strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(pen->StrokeStyle());

        geometry->GetWidenedBounds(pen->Thickness(), 
                                   strokeStyle, 
                                   D2DMatrix(worldTransform), 
                                   &bounds);

		float temp = pen->Thickness() / 2;

		bounds.left -= temp;
		bounds.top -= temp;
		bounds.right += temp;
		bounds.bottom += temp;
    }    
    else
		geometry->GetBounds(D2DMatrix(worldTransform), &bounds);
        

    return Rect(bounds.left, bounds.top , bounds.right, bounds.bottom);
}

auto Geometry::GetBounds(const StrokeStylePtr& strokeStyle, float thickness, const Matrix& worldTransform) -> Rect
{
	auto geometry = GetResource<ID2D1Geometry>();
	D2D1_RECT_F bounds;

	if (strokeStyle)
	{
		ID2D1StrokeStyle* ss = nullptr;

		ss = GetResourceFrom<ID2D1StrokeStyle>(strokeStyle);

		geometry->GetWidenedBounds(thickness,
			ss,
			D2DMatrix(worldTransform),
			&bounds);

		float temp = thickness / 2;

		bounds.left -= temp;
		bounds.top -= temp;
		bounds.right += temp;
		bounds.bottom += temp;
	}
	else
		geometry->GetBounds(D2DMatrix(worldTransform), &bounds);


	return Rect(bounds.left, bounds.top, bounds.right, bounds.bottom);
}

auto Geometry::GetFillMaskTransform() const -> const Matrix&
{
    return m_FillMask->Transform;
}

void Geometry::GeometryCacheEnabled( bool enabled )
{
    m_geometryCacheEnabled = enabled;
    if(!enabled)
    {
        m_FillMask = nullptr;
        m_StrokeMasks.clear();
    }
}

bool Geometry::GeometryCacheEnabled()
{
    return m_geometryCacheEnabled;
}

Microsoft::WRL::ComPtr<IUnknown> Geometry::OnCreateResource()
{
    auto geometry = OnCreateGeometry();

    if(!m_transform.IsIdentity)
    {
        ComPtr<ID2D1TransformedGeometry> transformedGeometry;
        //auto matrix = m_transform->GetTransformMatrix();

        HRESULT hr = m_Factory->CreateTransformedGeometry(geometry.Get(), D2DMatrix(m_transform), 
                                                          transformedGeometry.ReleaseAndGetAddressOf());
        return transformedGeometry;
    }
    else
    {
        return geometry;
    }
}

void Geometry::Transform( const TransformPtr& transform )
{
    /*if(m_transform != nullptr && transform != nullptr)
    {
        auto m1 = m_transform->GetTransformMatrix();
        auto m2 = transform->GetTransformMatrix();

        if(m1._11 == m2._11 && 
            m1._12 == m2._12 && 
            m1._21 == m2._21 && 
            m1._22 == m2._22 && 
            m1._31 == m2._31 && 
            m1._32 == m2._32)
            return;

        if(m2.IsIdentity())
            return;
    }*/
    m_transform = m_transform * transform->GetTransformMatrix();
    Invalidate();
}
