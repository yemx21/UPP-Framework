#include "Image.h"
#include "Graphic\Brush\BitmapBrush.h"
#include "Graphic\GraphicFactory.h"
using namespace System::UI;
using namespace System::UI::Graphic;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;

const DependencyPropertyPtr Image::SourceProperty = make_dependencyproperty<Image>();
const DependencyPropertyPtr Image::StretchProperty = make_dependencyproperty<Image>();
const DependencyPropertyPtr Image::StretchDirectionProperty = make_dependencyproperty<Image>();

__usctor(Image)
{
	Image::SourceProperty.get()->Register<Image>(L"Source", FrameworkPropertyMetadata::Create<BitmapPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	Image::StretchProperty.get()->Register<Image>(L"Stretch", FrameworkPropertyMetadata::Create<Graphic::Stretch>((Graphic::Stretch)Graphic::Stretch::Uniform, FrameworkPropertyMetadataOptions::AffectsRender));
	Image::StretchDirectionProperty.get()->Register<Image>(L"StretchDirection", FrameworkPropertyMetadata::Create<Graphic::StretchDirection>((Graphic::StretchDirection)Graphic::StretchDirection::Both, FrameworkPropertyMetadataOptions::AffectsRender));

	return true;
}

Image::Image(void)
{
}

Image::~Image(void)
{
}

bool Image::HitTest(const Point& point)
{
	return Visual::HitTest(point);
}

void Image::RenderOverride(const RenderContextPtr& renderContext)
{
	BitmapPtr  m_bitmap = GetSource();
	if (!m_bitmap) return;
	auto renderSize = GetRenderSize();

	renderContext->DrawBitmap(m_bitmap, EffectCompositeMode::SourceOver, renderSize);
}

System::Size Image::ArrangeOverride(const System::Size& finalSize)
{
	BitmapPtr  m_bitmap = GetSource();
	if (!m_bitmap) return System::Size();

	double scaleX = 1.0;
	double scaleY = 1.0;

	auto bitmapSize = m_bitmap->PixelSize;

	bool isConstrainedWidth = !FloatingHelper::IsNaNOrInfinity(finalSize.Width);
	bool isConstrainedHeight = !FloatingHelper::IsNaNOrInfinity(finalSize.Height);

	Graphic::Stretch m_stretch = GetStretch();

	if ((m_stretch == Graphic::Stretch::Uniform || m_stretch == Graphic::Stretch::UniformToFill || m_stretch == Graphic::Stretch::Fill)
		&& (isConstrainedWidth || isConstrainedHeight))
	{
		scaleX = bitmapSize.Width == 0 ? 0.0f : finalSize.Width / bitmapSize.Width;
		scaleY = bitmapSize.Height == 0 ? 0.0f : finalSize.Height / bitmapSize.Height;

		if (!isConstrainedWidth)        scaleX = scaleY;
		else if (!isConstrainedHeight)  scaleY = scaleX;
		else
		{
			if (m_stretch == Graphic::Stretch::Uniform)
			{
				double minscale = scaleX < scaleY ? scaleX : scaleY;
				scaleX = scaleY = minscale;
			}
			else if (m_stretch == Stretch::UniformToFill)
			{
				double maxscale = scaleX > scaleY ? scaleX : scaleY;
				scaleX = scaleY = maxscale;
			}
		}

		Graphic::StretchDirection m_stretchDirection = GetStretchDirection();

		if (m_stretchDirection == StretchDirection::UpOnly)
		{
			if (scaleX < 1.0f) scaleX = 1.0f;
			if (scaleY < 1.0f) scaleY = 1.0f;
		}
		else if (m_stretchDirection == StretchDirection::DownOnly)
		{
			if (scaleX > 1.0f) scaleX = 1.0f;
			if (scaleY > 1.0f) scaleY = 1.0f;
		}
	}

	return System::Size(bitmapSize.Width * scaleX, bitmapSize.Height * scaleY);
}

Size Image::MeasureOverride(const System::Size& availableSize)
{
	BitmapPtr  m_bitmap = GetSource();
	if (!m_bitmap) return System::Size();

	double scaleX = 1.0;
	double scaleY = 1.0;

	auto bitmapSize = m_bitmap->PixelSize;

	bool isConstrainedWidth = !FloatingHelper::IsNaNOrInfinity(availableSize.Width);
	bool isConstrainedHeight = !FloatingHelper::IsNaNOrInfinity(availableSize.Height);

	Graphic::Stretch m_stretch = GetStretch();

	if ((m_stretch == Graphic::Stretch::Uniform || m_stretch == Graphic::Stretch::UniformToFill || m_stretch == Graphic::Stretch::Fill)
		&& (isConstrainedWidth || isConstrainedHeight))
	{
		scaleX = bitmapSize.Width == 0 ? 0.0f : availableSize.Width / bitmapSize.Width;
		scaleY = bitmapSize.Height == 0 ? 0.0f : availableSize.Height / bitmapSize.Height;

		if (!isConstrainedWidth)        scaleX = scaleY;
		else if (!isConstrainedHeight)  scaleY = scaleX;
		else
		{
			if (m_stretch == Graphic::Stretch::Uniform)
			{
				double minscale = scaleX < scaleY ? scaleX : scaleY;
				scaleX = scaleY = minscale;
			}
			else if (m_stretch == Stretch::UniformToFill)
			{
				double maxscale = scaleX > scaleY ? scaleX : scaleY;
				scaleX = scaleY = maxscale;
			}
		}

		Graphic::StretchDirection m_stretchDirection = GetStretchDirection();

		if (m_stretchDirection == StretchDirection::UpOnly)
		{
			if (scaleX < 1.0f) scaleX = 1.0f;
			if (scaleY < 1.0f) scaleY = 1.0f;
		}
		else if (m_stretchDirection == StretchDirection::DownOnly)
		{
			if (scaleX > 1.0f) scaleX = 1.0f;
			if (scaleY > 1.0f) scaleY = 1.0f;
		}
	}

	return System::Size(bitmapSize.Width * scaleX, bitmapSize.Height * scaleY);
}

const BitmapPtr Image::GetSource() const
{
	return GetValuePtr<Bitmap>(SourceProperty);
}

void Image::SetSource(const BitmapPtr& bitmapSource)
{
	SetValue(SourceProperty, bitmapSource);
}

Graphic::StretchDirection Image::GetStretchDirection() const
{
	return GetValue<Graphic::StretchDirection>(StretchDirectionProperty);
}

void Image::SetStretchDirection(Graphic::StretchDirection val)
{
	SetValue(StretchDirectionProperty, val);
}

Graphic::Stretch Image::GetStretch() const
{
	return GetValue<Graphic::Stretch>(StretchProperty);
}

void Image::SetStretch(Graphic::Stretch stretch)
{
	SetValue(StretchProperty, stretch);
}