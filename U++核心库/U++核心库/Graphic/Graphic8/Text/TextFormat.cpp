#include "TextFormat.h"
#include "..\Rendering\RenderContext.h"

using namespace System::UI;
using namespace System::UI::Graphic::d2d1;
using namespace Microsoft::WRL;
using namespace std;

TextFormat::TextFormat(const wchar_t* fontFamily, float fontSize, Graphic::FontWeight fontWeight, Graphic::FontStyle fontStyle, Graphic::FontStretch fontStretch) : m_fontFamily(fontFamily), m_fontSize(fontSize), m_fontStretch(fontStretch), m_fontStyle(fontStyle), m_fontWeight(fontWeight)
{
    m_factory = RenderContext::GetDirectWriteFactory();
}

TextFormat::~TextFormat(void)
{
}

ComPtr<IUnknown> TextFormat::OnCreateResource()
{
    ComPtr<IDWriteTextFormat> textFormat;

	DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
	switch ((UInt)m_fontWeight)
	{
	case 0:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_THIN;
		break;
	case 1:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
		break;
	case 2:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_ULTRA_LIGHT;
		break;
	case 3:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT;
		break;
	case 4:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_SEMI_LIGHT;
		break;
	case 5:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		break;
	case 6:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_REGULAR;
		break;
	case 7:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_MEDIUM;
		break;
	case 8:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_DEMI_BOLD;
		break;
	case 9:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_SEMI_BOLD;
		break;
	case 10:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD;
		break;
	case 11:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_EXTRA_BOLD;
		break;
	case 12:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_ULTRA_BOLD;
		break;
	case 13:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BLACK;
		break;
	case 14:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_HEAVY;
		break;
	case 15:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_EXTRA_BLACK;
		break;
	case 16:
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_ULTRA_BLACK;
		break;
	}

	auto hr = m_factory->CreateTextFormat(L"Î¢ÈíÑÅºÚ",
            //m_fontFamily.c_str(),
            nullptr,
			DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
			(DWRITE_FONT_STYLE)(UInt)m_fontStyle,
			DWRITE_FONT_STRETCH_NORMAL,
            m_fontSize,
            L"", //locale
            &textFormat
           );
    
	if (hr != S_OK)
	{
		textFormat = nullptr;
	}
    return textFormat;
}

void TextFormat::FontSize(float fontSize)
{
    m_fontSize = fontSize;
    Invalidate();
}

float TextFormat::FontSize() const
{
    return m_fontSize;
}

const std::wstring& TextFormat::FontFamily() const
{
    return m_fontFamily;
}

void TextFormat::FontFamily(const wstring& fontFamily)
{
    m_fontFamily = fontFamily;
    Invalidate();
}

const wchar_t* TextFormat::FontFamily1() const
{
	return m_fontFamily.c_str();
}

void TextFormat::FontFamily1(const wchar_t* fontFamily)
{
	m_fontFamily = fontFamily;
	Invalidate();
}


void TextFormat::FontWeight(Graphic::FontWeight fontWeight)
{
	m_fontWeight = fontWeight;
	Invalidate();
}

Graphic::FontWeight TextFormat::FontWeight() const
{
	return m_fontWeight;
}

void TextFormat::FontStyle(Graphic::FontStyle fontStyle)
{
	m_fontStyle = fontStyle;
	Invalidate();
}

Graphic::FontStyle TextFormat::FontStyle() const
{
	return m_fontStyle;
}

void TextFormat::FontStretch(Graphic::FontStretch fontStretch)
{
	m_fontStretch = fontStretch;
	Invalidate();
}

Graphic::FontStretch TextFormat::FontStretch() const
{
	return m_fontStretch;
}