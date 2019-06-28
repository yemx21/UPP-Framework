#pragma once
#include "..\..\UIFramework_Config.h"
using namespace System::UI::Graphic;

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Controls
			{
				class TextBlock;
				typedef std::shared_ptr<TextBlock> TextBlockPtr;

				__uclass __uobjdef(asm = UIFramework, ns = System::UI::Windows::Controls, ver = 1.0) UIFRAMEWORK_API TextBlock : public UIElement
				{
				protected:
					virtual void RenderOverride(const RenderContextPtr& renderContext) override;
					virtual System::Size MeasureOverride(const System::Size& availableSize) override;
					virtual System::Size ArrangeOverride(const System::Size& finalSize) override;
					virtual bool HitTest(const Point& point)  override;
				private:
					static bool FontSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changed);
					static bool FontFamilyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changed);
					static bool TextChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changed);
					static bool FontWeightChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
					static bool FontStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
					static bool FontStretchChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
					TextLayoutPtr  m_textLayout;
					TextFormatPtr m_textFormat;
					RectangleGeometryPtr m_back;
					System::Size m_lastSize;
				public:
					static const DependencyPropertyPtr FontSizeProperty;
					static const DependencyPropertyPtr FontFamilyProperty;
					static const DependencyPropertyPtr FontWeightProperty;
					static const DependencyPropertyPtr FontStyleProperty;
					static const DependencyPropertyPtr FontStretchProperty;
					static const DependencyPropertyPtr TextProperty;
					static const DependencyPropertyPtr BackgroundProperty;
					static const DependencyPropertyPtr ForegroundProperty;
				public:
					__udefctor TextBlock(void);
					virtual ~TextBlock(void);

					void SetText(const wstring& text);
					wstring GetText() const;

					void SetFontFamily(const wstring& fontFamily);
					wstring GetFontFamily()const;

					void SetFontSize(double fontSize);
					double GetFontSize() const;

					void SetFontWeight(Graphic::FontWeight fontWeight);
					Graphic::FontWeight GetFontWeight() const;

					void SetFontStyle(Graphic::FontStyle fontStyle);
					Graphic::FontStyle GetFontStyle() const;

					void SetFontStretch(Graphic::FontStretch fontStretch);
					Graphic::FontStretch GetFontStretch() const;

					void SetBackground(const BrushPtr& brush);
					const BrushPtr GetBackground() const;

					void SetForeground(const BrushPtr& brush);
					const BrushPtr GetForeground() const;

					__uproperty(get = GetText, put = SetText) wstring Text;
					__uproperty(get = GetFontFamily, put = SetFontFamily) wstring FontFamily;
					__uproperty(get = GetFontSize, put = SetFontSize) double FontSize;
					__uproperty(get = GetFontWeight, put = SetFontWeight) Graphic::FontWeight FontWeight;
					__uproperty(get = GetFontStyle, put = SetFontStyle) Graphic::FontStyle FontStyle;
					__uproperty(get = GetFontStretch, put = SetFontStretch) Graphic::FontStretch FontStretch;
					__uproperty(get = GetBackground, put = SetBackground) BrushPtr Background;
					__uproperty(get = GetForeground, put = SetForeground) BrushPtr Foreground;

					__usctordef(TextBlock)
					REFLECT_CLASS(TextBlock)
				};
			}
		}
	}
}
