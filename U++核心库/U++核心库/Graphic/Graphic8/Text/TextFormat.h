#pragma once
#include "..\Resource.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class TextFormat;
				typedef std::shared_ptr<TextFormat> TextFormatPtr;

				class TextFormat : public IndependantResource
				{
				public:
					TextFormat(const wchar_t* fontFamily, float fontSize, Graphic::FontWeight fontWeight, Graphic::FontStyle fontStyle, Graphic::FontStretch fontStretch);
					virtual ~TextFormat(void);
				public:
					void FontFamily(const std::wstring& fontFamily);
					const std::wstring& FontFamily() const;

					void FontFamily1(const wchar_t* fontFamily);
					const wchar_t* FontFamily1() const;

					void FontSize(float fontSize);
					float FontSize() const;

					void FontWeight(Graphic::FontWeight fontWeight);
					Graphic::FontWeight FontWeight() const;

					void FontStyle(Graphic::FontStyle fontStyle);
					Graphic::FontStyle FontStyle() const;

					void FontStretch(Graphic::FontStretch fontStretch);
					Graphic::FontStretch FontStretch() const;

				protected:
					Microsoft::WRL::ComPtr<IUnknown> OnCreateResource();
				private:
					std::wstring m_fontFamily;
					float m_fontSize;
					Graphic::FontWeight m_fontWeight;
					Graphic::FontStyle m_fontStyle;
					Graphic::FontStretch m_fontStretch;
					Microsoft::WRL::ComPtr<IDWriteFactory> m_factory;
				};
			}
		}
	}
}