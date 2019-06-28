#pragma once
#include "..\Resource.h"
#include "TextFormat.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class TextLayout;
				typedef std::shared_ptr<TextLayout> TextLayoutPtr;

				typedef Microsoft::WRL::ComPtr<IDWriteFactory> IDWriteFactoryPtr;

				class TextLayout : public IndependantResource
				{
				public:
					TextLayout(const TextFormatPtr& fontFormat, const wchar_t* text, const Size& maxSize);
					TextLayout(void);
					virtual ~TextLayout(void);

					void MaxSize(const Size& size);
					auto MaxSize() const->Size;
					void MaxWidth(float width);
					void MaxHeight(float height);

					void Text(const std::wstring& text);
					auto Text() const -> const std::wstring&;

					void FontFamily(const std::wstring& fontFamily);
					auto FontFamily() const -> const std::wstring&;

					void Text1(const wchar_t* text);
					auto Text1() const -> const wchar_t*;

					void FontFamily1(const wchar_t* fontFamily);
					auto FontFamily1() const -> const wchar_t*;

					void FontSize(float fontSize);
					float FontSize() const;

					void FontFormat(const TextFormatPtr& fontFormat);
					TextFormatPtr FontFormat() const;

					auto GetLayoutBounds() -> const Rect;

					bool HitTest(const Point& point);
				protected:
					virtual auto OnCreateResource()->Microsoft::WRL::ComPtr<IUnknown> override;
					virtual auto IsResourceValid() const -> bool override;
				private:
					std::wstring      m_text;
					Size   m_maxSize;
					IDWriteFactoryPtr m_factory;
					TextFormatPtr     m_textFormat;
				};
			}
		}
	}
}
