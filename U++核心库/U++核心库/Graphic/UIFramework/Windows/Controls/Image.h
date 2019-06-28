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
				class Image;
				typedef std::shared_ptr<Image> ImagePtr;

				__uclass __uobjdef(asm = UIFramework, ns = System::UI::Windows::Controls, ver = 1.0) UIFRAMEWORK_API Image : public UIElement
				{
				protected:
					virtual void RenderOverride(const RenderContextPtr& renderContext) override;
					virtual System::Size MeasureOverride(const System::Size& availableSize) override;
					virtual System::Size ArrangeOverride(const System::Size& finalSize) override;
					virtual bool HitTest(const Point& point) override;
				public:
					static const DependencyPropertyPtr SourceProperty;
					static const DependencyPropertyPtr StretchProperty;
					static const DependencyPropertyPtr StretchDirectionProperty;
				public:
					__udefctor Image(void);
					virtual ~Image(void);

					const BitmapPtr GetSource() const;
					void SetSource(const BitmapPtr& content);

					Graphic::Stretch GetStretch() const;
					void SetStretch(Graphic::Stretch stretch);

					Graphic::StretchDirection GetStretchDirection() const;
					void SetStretchDirection(Graphic::StretchDirection stretchDirection);

					__uproperty(get = GetSource, put = SetSource) BitmapPtr Source;
					__uproperty(get = GetStretch, put = SetStretch) Graphic::Stretch Stretch;
					__uproperty(get = GetStretchDirection, put = SetStretchDirection) Graphic::StretchDirection StretchDirection;

					__usctordef(Image)
					REFLECT_CLASS(Image)
				};
			}
		}
	}
}