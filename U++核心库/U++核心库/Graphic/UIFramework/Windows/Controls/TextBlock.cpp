#include "TextBlock.h"
#include "Graphic\GraphicFactory.h"
#include "PreCoerceCallbackHelper.h"
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;

const DependencyPropertyPtr TextBlock::BackgroundProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::ForegroundProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::TextProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::FontSizeProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::FontFamilyProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::FontWeightProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::FontStyleProperty = make_dependencyproperty<TextBlock>();
const DependencyPropertyPtr TextBlock::FontStretchProperty = make_dependencyproperty<TextBlock>();

__usctor(TextBlock)
{
	TextBlock::BackgroundProperty.get()->Register<TextBlock>(L"Background", FrameworkPropertyMetadata::Create<BrushPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	TextBlock::ForegroundProperty.get()->Register<TextBlock>(L"Foreground", FrameworkPropertyMetadata::Create<BrushPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	TextBlock::TextProperty.get()->Register<TextBlock>(L"Text", FrameworkPropertyMetadata::Create<wstring>(wstring(), FrameworkPropertyMetadataOptions::AffectsRender, &TextChanged));
	TextBlock::FontSizeProperty.get()->Register<TextBlock>(L"FontSize", FrameworkPropertyMetadata::Create<double>(10.0, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::Inherits, &FontSizeChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 1.0, 1.0, 1.0)));
	TextBlock::FontFamilyProperty.get()->Register<TextBlock>(L"FontFamily", FrameworkPropertyMetadata::Create<wstring>(wstring(L"Arial"), FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::Inherits, &FontFamilyChanged));
	TextBlock::FontWeightProperty.get()->Register<TextBlock>(L"FontWeight", FrameworkPropertyMetadata::Create<Graphic::FontWeight>((Graphic::FontWeight)Graphic::FontWeight::Normal, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::Inherits, &FontWeightChanged));
	TextBlock::FontStyleProperty.get()->Register<TextBlock>(L"FontStyle", FrameworkPropertyMetadata::Create<Graphic::FontStyle>((Graphic::FontStyle)Graphic::FontStyle::Normal, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::Inherits, &FontStyleChanged));
	TextBlock::FontStretchProperty.get()->Register<TextBlock>(L"FontStretch", FrameworkPropertyMetadata::Create<Graphic::FontStretch>((Graphic::FontStretch)Graphic::FontStretch::Normal, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::Inherits, &FontStretchChanged));
	
	return true;
}

bool TextBlock::FontSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto textBlock = static_cast<TextBlock*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	if (textBlock) textBlock->m_textFormat->FontSize = newValue;
	return true;
}

bool TextBlock::FontFamilyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto textBlock = static_cast<TextBlock*>(ptr);
	auto newValue = changedArgs.NewValue<wstring>();
	if (textBlock) textBlock->m_textFormat->FontFamily = newValue;
	return true;
}

bool TextBlock::FontWeightChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto textBlock = static_cast<TextBlock*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FontWeight>();
	if (textBlock) textBlock->m_textFormat->FontWeight = newValue;
	return true;
}

bool TextBlock::FontStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto textBlock = static_cast<TextBlock*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FontStyle>();
	if (textBlock) textBlock->m_textFormat->FontStyle = newValue;
	return true;
}

bool TextBlock::FontStretchChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto textBlock = static_cast<TextBlock*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FontStretch>();
	if (textBlock) textBlock->m_textFormat->FontStretch = newValue;
	return true;
}

bool TextBlock::TextChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto textBlock = static_cast<TextBlock*>(ptr);
	auto newVal = changedArgs.NewValue<wstring>();
	if (textBlock) textBlock->m_textLayout->Text = newVal;
	return true;
}


void TextBlock::SetText(const std::wstring& text)
{
	SetValue(TextProperty, text);
}

std::wstring TextBlock::GetText() const
{
	return GetValue<wstring>(TextProperty);
}

std::wstring TextBlock::GetFontFamily() const
{
	return GetValue<wstring>(FontFamilyProperty);
}

void TextBlock::SetFontFamily(const wstring& fontFamily)
{
	SetValue(FontFamilyProperty, fontFamily);
}

void TextBlock::SetFontSize(double fontSize)
{
	SetValue(FontSizeProperty, fontSize);
}

double TextBlock::GetFontSize() const
{
	return GetValue<double>(FontSizeProperty);
}

void TextBlock::SetFontWeight(Graphic::FontWeight fontWeight)
{
	SetValue(FontWeightProperty, fontWeight);
}

Graphic::FontWeight TextBlock::GetFontWeight() const
{
	return GetValue<Graphic::FontWeight>(FontWeightProperty);
}

void TextBlock::SetFontStyle(Graphic::FontStyle fontStyle)
{
	SetValue(FontStyleProperty, fontStyle);
}

Graphic::FontStyle TextBlock::GetFontStyle() const
{
	return GetValue<Graphic::FontStyle>(FontStyleProperty);
}

void TextBlock::SetFontStretch(Graphic::FontStretch fontStretch)
{
	SetValue(FontStretchProperty, fontStretch);
}

Graphic::FontStretch TextBlock::GetFontStretch() const
{
	return GetValue<Graphic::FontStretch>(FontStretchProperty);
}

TextBlock::TextBlock(void)
{	
	m_textFormat = make_shared<TextFormat>(FontFamily, FontSize, FontWeight, FontStyle, FontStretch);
	m_textLayout = make_shared<TextLayout>(m_textFormat, Text);
}

TextBlock::~TextBlock(void)
{
}

System::Size TextBlock::MeasureOverride(const System::Size& availableSize)
{
	m_textLayout->MaxSize = availableSize;
	auto bounds = m_textLayout->GetLayoutBounds();

	return bounds.Size;
}

System::Size TextBlock::ArrangeOverride(const System::Size& finalSize)
{
	m_textLayout->MaxSize = finalSize;

	auto bounds = m_textLayout->GetLayoutBounds();
	return finalSize;//bounds.Size();
}

void TextBlock::RenderOverride(const RenderContextPtr& renderContext)
{
	auto backGround = Background;

	if (backGround)
	{
		auto bounds = m_textLayout->GetLayoutBounds();
		auto boundsSize = bounds.Size;
		if (!m_back || boundsSize != m_lastSize)
		{
			Rect rect(0, 0, boundsSize.Width, boundsSize.Height);
			m_back = make_shared<RectangleGeometry>(rect);
			m_lastSize = bounds.Size;
		}
		renderContext->DrawGeometry(m_back, backGround, nullptr, nullptr, 0.0);
	}

	auto foreGround = Foreground;
	if (foreGround)
	{
		renderContext->DrawTextLayout(m_textLayout, foreGround, Point(0, 0));
	}
	return;
}


void TextBlock::SetBackground(const BrushPtr& brush)
{
	SetValue(BackgroundProperty, brush);
}

const BrushPtr TextBlock::GetBackground() const
{
	return GetValuePtr<Brush>(BackgroundProperty);
}

void TextBlock::SetForeground(const BrushPtr& brush)
{
	SetValue(ForegroundProperty, brush);
}

const BrushPtr TextBlock::GetForeground() const
{
	return GetValuePtr<Brush>(ForegroundProperty);
}

bool TextBlock::HitTest(const Point& point)
{
	bool result = false;
	auto background = Background;

	if (background)
		result = m_back->HitTest(point);

	if (!m_textLayout)
		return false;

	if (result)
		return result;

	return m_textLayout->HitTest(point);
}