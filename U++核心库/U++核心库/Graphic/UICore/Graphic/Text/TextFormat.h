#pragma once
#include "..\Resource.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class TextFormat;
			typedef std::shared_ptr<TextFormat> TextFormatPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API TextFormat : public FeatureIndependantResource
			{
			public:
				__uctor TextFormat(const std::wstring& fontFamily, double fontSize, Graphic::FontWeight fontWeight, Graphic::FontStyle fontStyle, Graphic::FontStretch fontStretch);
				~TextFormat();
			public:
				void SetFontFamily(const std::wstring& fontFamily);
				std::wstring GetFontFamily() const;

				void SetFontSize(double fontSize);
				double GetFontSize() const;

				void SetFontWeight(Graphic::FontWeight fontWeight);
				Graphic::FontWeight GetFontWeight() const;

				void SetFontStyle(Graphic::FontStyle fontStyle);
				Graphic::FontStyle GetFontStyle() const;

				void SetFontStretch(Graphic::FontStretch fontStretch);
				Graphic::FontStretch GetFontStretch() const;

			protected:
				virtual FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool FontFamilyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool FontSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool FontWeightChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool FontStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool FontStretchChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);

			public:
				static const DependencyPropertyPtr FontFamilyProperty;
				static const DependencyPropertyPtr FontSizeProperty;
				static const DependencyPropertyPtr FontWeightProperty;
				static const DependencyPropertyPtr FontStyleProperty;
				static const DependencyPropertyPtr FontStretchProperty;

				__uproperty(get = GetFontFamily, put = SetFontFamily) std::wstring FontFamily;
				__uproperty(get = GetFontSize, put = SetFontSize) double FontSize;
				__uproperty(get = GetFontWeight, put = SetFontWeight) Graphic::FontWeight FontWeight;
				__uproperty(get = GetFontStyle, put = SetFontStyle) Graphic::FontStyle FontStyle;
				__uproperty(get = GetFontStretch, put = SetFontStretch) Graphic::FontStretch FontStretch;

				__usctordef(TextFormat)
				REFLECT_CLASS(TextFormat)
			};
		}
	}
}