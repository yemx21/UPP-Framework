#include <memory>
#include "BitmapBrush.h"
#include "..\Media\Bitmap.h"

using namespace System;

using namespace System::UI::Graphic::d2d1;
using namespace Microsoft::WRL;
using namespace std;

BitmapBrush::BitmapBrush(const BitmapPtr& image) : m_image(image),
                                                m_extendModeX(ExtendMode::Clamp), 
												m_extendModeY(ExtendMode::Clamp),
                                                m_sourceRect(0,0,0,0),
												m_interpolationMode(InterpolationMode::Linear)
{
    auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

    if(bitmap)
    {
        auto size = bitmap->GetSize();
        m_sourceRect.Bottom = size.Height;
		m_sourceRect.Right = size.Width;
    }
}

BitmapBrush::BitmapBrush(const BitmapPtr& image, ExtendMode modeX, ExtendMode modeY, Rect sourceRect, InterpolationMode interpolation) : m_image(image),
m_extendModeX(modeX),
m_extendModeY(modeY),
m_sourceRect(sourceRect),
m_interpolationMode(interpolation)
{
	auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

	if (bitmap)
	{
		auto size = bitmap->GetSize();
		m_sourceRect.Bottom = size.Height;
		m_sourceRect.Right = size.Width;
	}
}

BitmapBrush::BitmapBrush() :
m_extendModeX(ExtendMode::Clamp),
m_extendModeY(ExtendMode::Clamp),
m_sourceRect(0, 0, 0, 0), 
m_interpolationMode(InterpolationMode::Linear)
{

}


BitmapBrush::~BitmapBrush(void)
{
}


auto BitmapBrush::OnCreateResource(ID2D1DeviceContext* deviceContext) -> ComPtr<IUnknown>
{
    if(m_image)
    {
        auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

        if(bitmap)
        {
            auto size = bitmap->GetSize();
			m_sourceRect.Bottom = size.Height;
			m_sourceRect.Right = size.Width;
        }

        ComPtr<ID2D1ImageBrush> brush = nullptr;

		auto image = GetResourceFrom<ID2D1Image>(m_image);
		deviceContext->CreateImageBrush(image, D2D1::ImageBrushProperties(D2DRect(m_sourceRect), (D2D1_EXTEND_MODE)(UInt)m_extendModeX,
			(D2D1_EXTEND_MODE)(UInt)m_extendModeY, (D2D1_INTERPOLATION_MODE)(UInt)m_interpolationMode), D2D1::BrushProperties(m_Opacity, D2DMatrix(m_transform)), &brush);

        
        return brush;
    }
    else
    {
         ComPtr<ID2D1SolidColorBrush> brush = nullptr;
		 deviceContext->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);

         return brush;
    }
}

auto BitmapBrush::IsImageSet() -> bool
{
    return m_image && IsResourceValid();
}

ExtendMode BitmapBrush::ExtendModeX() const
{
    return m_extendModeX;
}

ExtendMode BitmapBrush::ExtendModeY() const
{
    return m_extendModeY;
}

void BitmapBrush::ExtendModeX(ExtendMode extentMode)
{
    m_extendModeX = extentMode;
    if(IsImageSet())
    {
        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

		brush->SetExtendModeX((D2D1_EXTEND_MODE)(UInt)m_extendModeX);
		NotifyInvalidatables(InvalidateFlags_internal::Visual);
    }
}

auto BitmapBrush::SourceRect() const -> Rect
{
    return m_sourceRect;
}

void BitmapBrush::SourceRect(const Rect& rect)
{
	m_sourceRect = rect;

	if (IsImageSet())
	{
		auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

		D2D1_RECT_F r =D2DRect(m_sourceRect);
		brush->SetSourceRectangle(&r);
		NotifyInvalidatables(InvalidateFlags_internal::Visual);
	}
}

void BitmapBrush::ExtendModeY(ExtendMode extentMode)
{
    m_extendModeY = extentMode;
    if(IsImageSet())
    {
		auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

		brush->SetExtendModeY((D2D1_EXTEND_MODE)(UInt)m_extendModeY);
		NotifyInvalidatables(InvalidateFlags_internal::Visual);
    }
}

void BitmapBrush::Image(const BitmapPtr& image)
{
    m_image = image;

    auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

    if(bitmap)
    {
        auto size = bitmap->GetSize();
		m_sourceRect.Bottom = size.Height;
		m_sourceRect.Right =size.Width;
    }

    if(IsImageSet() && IsResourceValidFrom(image))
    {


        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();
		auto img = GetResourceUnsafeFrom<ID2D1Image>(image);

		brush->SetImage(img);
		NotifyInvalidatables(InvalidateFlags_internal::Visual);
    }
    else
    {
        Invalidate();
    }
}

auto BitmapBrush::Image() -> BitmapPtr
{
    return m_image;
}

auto BitmapBrush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen) -> Matrix
{
    Rect bounds;

    bounds = geometry->GetBounds(pen, Matrix::Identity());

	auto size = D2D1::SizeF(m_sourceRect.Right - m_sourceRect.Left,
                      m_sourceRect.Bottom - m_sourceRect.Top);

    auto widthRatio = (bounds.Right - bounds.Left) / size.width;
    auto heightRatio = (bounds.Bottom - bounds.Top) / size.height;

    auto matrix = Matrix::CreateTranslation(bounds.Left, bounds.Top);
    matrix = matrix * Matrix::CreateScaling(widthRatio, heightRatio,bounds.Left, bounds.Top);
    return matrix;
}

auto BitmapBrush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, float thickness) -> Matrix
{
	Rect bounds;

	bounds = geometry->GetBounds(strokeStyle,thickness, Matrix::Identity());

	auto size = D2D1::SizeF(m_sourceRect.Right - m_sourceRect.Left,
		m_sourceRect.Bottom - m_sourceRect.Top);

	auto widthRatio = (bounds.Right - bounds.Left) / size.width;
	auto heightRatio = (bounds.Bottom - bounds.Top) / size.height;

	auto matrix = Matrix::CreateTranslation(bounds.Left, bounds.Top);
	matrix = matrix * Matrix::CreateScaling(widthRatio, heightRatio, bounds.Left, bounds.Top);
	return matrix;
}

void BitmapBrush::Interpolation(InterpolationMode mode)
{
	m_interpolationMode = mode;

	if (IsImageSet())
	{
		auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

		brush->SetInterpolationMode((D2D1_INTERPOLATION_MODE)(UInt)m_interpolationMode);
		NotifyInvalidatables(InvalidateFlags_internal::Visual);
	}
}

auto BitmapBrush::Interpolation() const -> InterpolationMode
{
	return m_interpolationMode;
}

auto BitmapBrush::GetTransformToBounds(const Rect& rect) -> Matrix
{
    auto bounds = rect;

	auto size = D2D1::SizeF(m_sourceRect.Right - m_sourceRect.Left,
                      m_sourceRect.Bottom - m_sourceRect.Top);

    auto widthRatio = (bounds.Right - bounds.Left) / size.width;
    auto heightRatio = (bounds.Bottom - bounds.Top) / size.height;

    auto matrix = Matrix::CreateTranslation(bounds.Left, bounds.Top);
    matrix = matrix * Matrix::CreateScaling(widthRatio, heightRatio, bounds.Left, bounds.Top);
    return matrix;
}