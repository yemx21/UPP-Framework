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
			class TextLayout;
			typedef std::shared_ptr<TextLayout> TextLayoutPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API TextLayout : public FeatureIndependantResource
			{
			public:
				__uctor TextLayout(const TextFormatPtr& format, const std::wstring& text);
				~TextLayout();

				void SetMaxSize(const Size& size);
				Size GetMaxSize() const;

				void SetText(const std::wstring& text);
				std::wstring GetText() const;

				void SetTextFormat(const TextFormatPtr& fontFamily);
				TextFormatPtr GetTextFormat() const;

				__uref Rect GetLayoutBounds() const;

				__uref bool HitTest(const Point& point);

			protected:
				virtual FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool MaxSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool TextChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool TextFormatChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr MaxSizeProperty;
				static const DependencyPropertyPtr TextProperty;
				static const DependencyPropertyPtr TextFormatProperty;
				static const DependencyPropertyPtr LayoutBoundsProperty;

				__uproperty(get = GetMaxSize, put = SetMaxSize) Size MaxSize;
				__uproperty(get = GetText, put = SetText) std::wstring Text;
				__uproperty(get = GetTextFormat, put = SetTextFormat) TextFormatPtr TextFormat;
				__uproperty(get = GetLayoutBounds) Rect LayoutBounds;

				__usctordef(TextLayout)
				REFLECT_CLASS(TextLayout)
			};
		}
	}
}
