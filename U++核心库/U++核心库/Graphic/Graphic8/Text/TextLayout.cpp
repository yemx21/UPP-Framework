#include "TextLayout.h"
#include "..\Rendering\RenderContext.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;
using namespace Microsoft::WRL;
using namespace std;

TextLayout::TextLayout(const TextFormatPtr& fontFormat, const wchar_t* text, const Size& maxSize) : m_maxSize(maxSize), m_text(text)
{
    m_factory = RenderContext::GetDirectWriteFactory();
	m_textFormat = fontFormat;
}

TextLayout::~TextLayout(void)
{
}

auto TextLayout::OnCreateResource() -> ComPtr<IUnknown> 
{
     auto textFormat = GetResourceFrom<IDWriteTextFormat>(m_textFormat);
	 
     ComPtr<IDWriteTextLayout> textLayout;
     HRESULT hr = m_factory->CreateTextLayout(m_text.c_str(), 
                                              (UINT32)m_text.length(), 
                                              textFormat, 
                                              m_maxSize.Width, 
                                              m_maxSize.Height, 
                                              &textLayout);
	 if(hr != S_OK)
	 {
		 textLayout = nullptr;
	 }
     return textLayout;
}

auto TextLayout::GetLayoutBounds() -> const Rect
{
    auto textLayout = GetResource<IDWriteTextLayout>();

    DWRITE_TEXT_METRICS metrics;
    textLayout->GetMetrics(&metrics);
    
    return Rect(metrics.left,
                                metrics.top, 
                                metrics.widthIncludingTrailingWhitespace + metrics.left, 
                                metrics.height + metrics.top);
}

void TextLayout::MaxSize(const Size& maxsize)
{
    if (maxsize != m_maxSize)
    {
        m_maxSize = maxsize;
        auto textLayout = GetResource<IDWriteTextLayout>();
        textLayout->SetMaxHeight(maxsize.Height);
        textLayout->SetMaxWidth(maxsize.Width);
    } 
}
auto TextLayout::MaxSize() const->Size
{
	return m_maxSize;
}
void TextLayout::MaxWidth(float width)
{
	if (width != m_maxSize.Width)
	{
		m_maxSize.Width = width;
		auto textLayout = GetResource<IDWriteTextLayout>();
		textLayout->SetMaxHeight(m_maxSize.Height);
		textLayout->SetMaxWidth(m_maxSize.Width);
	}
}

void TextLayout::MaxHeight(float height)
{
	if (height != m_maxSize.Height)
	{
		m_maxSize.Height = height;
		auto textLayout = GetResource<IDWriteTextLayout>();
		textLayout->SetMaxHeight(m_maxSize.Height);
		textLayout->SetMaxWidth(m_maxSize.Width);
	}
}

void TextLayout::Text(const std::wstring& text)
{
	if(text == m_text)
		return;

    m_text = text;
    Invalidate();
}

auto TextLayout::Text() const -> const wstring&
{
    return m_text;
}

void TextLayout::Text1(const wchar_t* text)
{
	if (text == m_text)
		return;

	m_text = text;
	Invalidate();
}

auto TextLayout::Text1() const -> const wchar_t*
{
	return m_text.c_str();
}

auto TextLayout::IsResourceValid() const -> bool
{
    if(!IndependantResource::IsResourceValid())
        return false;


    if(!IndependantResource::IsResourceValid(m_textFormat))
        return false;

    return true;
}

bool TextLayout::HitTest( const Point& point)
{
    bool result = false;
    auto txtLayout = GetResource<IDWriteTextLayout>();

    BOOL isTrailingHit = false;
    BOOL isInside = false;

    DWRITE_HIT_TEST_METRICS metrics;

    txtLayout->HitTestPoint(point.X, point.Y, &isTrailingHit, &isInside, &metrics);
   
    result = isInside && metrics.isText /* this correct? */;

    return result != 0;
}


void TextLayout::FontSize(float fontSize)
{
    m_textFormat->FontSize(fontSize);
    Invalidate();
}

float TextLayout::FontSize() const
{
	return m_textFormat->FontSize();
}

const std::wstring& TextLayout::FontFamily() const
{
	return m_textFormat->FontFamily();
}



void TextLayout::FontFamily(const wstring& fontFamily)
{
    m_textFormat->FontFamily(fontFamily);
    Invalidate();
}

const wchar_t* TextLayout::FontFamily1() const
{
	return m_textFormat->FontFamily1();
}


void TextLayout::FontFamily1(const wchar_t* fontFamily)
{
	m_textFormat->FontFamily1(fontFamily);
	Invalidate();
}

void TextLayout::FontFormat(const TextFormatPtr& fontFormat)
{
	m_textFormat = fontFormat;
	Invalidate();
}

TextFormatPtr TextLayout::FontFormat() const
{
	return m_textFormat;
}