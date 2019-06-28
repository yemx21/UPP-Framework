#pragma once
#include "Decorator.h"

using namespace System::UI::Graphic;
namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Controls
			{
				class Border;
				typedef std::shared_ptr<Border> BorderPtr;

				__uclass __uobjdef(asm = UIFramework, ns = System::UI::Windows::Controls, ver = 1.0) UIFRAMEWORK_API Border : public Decorator, public Windows::Controls::IContentControl
				{
				public:
					VisualPtr GetContent() const override;
					void SetContent(const VisualPtr& content) override;
				protected:
					virtual bool HitTest(const Point& point) override;
					virtual void RenderOverride(const RenderContextPtr& renderContext) override;
				protected:
					virtual System::Size MeasureOverride(const System::Size& availableSize) override;
					virtual System::Size ArrangeOverride(const System::Size& finalSize) override;
				private:
					void CreateGeometry();
				private:
					PathGeometryPtr  m_pathGeometry;
					System::Size m_lastSize;
				public:
					static const DependencyPropertyPtr BorderBrushProperty;
					static const DependencyPropertyPtr BackgroundProperty;
					static const DependencyPropertyPtr StrokeStyleProperty;
					static const DependencyPropertyPtr PaddingProperty;
					static const DependencyPropertyPtr CornerRadiusProperty;
					static const DependencyPropertyPtr BorderThicknessProperty;
				public:
					__udefctor Border();
					virtual ~Border();

					const BrushPtr GetBorderBrush() const;
					void SetBorderBrush(const BrushPtr& borderBrush);

					const BrushPtr GetBackground() const;
					void SetBackground(const BrushPtr& backgroundBrush);

					const StrokeStylePtr GetStrokeStyle() const;
					void SetStrokeStyle(const StrokeStylePtr& strokeStyle);

					Thickness GetPadding() const;
					void SetPadding(const Thickness& thickness);

					System::CornerRadius GetCornerRadius() const;
					void SetCornerRadius(const System::CornerRadius& cornerRadius);

					double GetBorderThickness() const;
					void SetBorderThickness(double thickness);

					__uproperty(get = GetBorderBrush, put = SetBorderBrush) BrushPtr BorderBrush;
					__uproperty(get = GetBackground, put = SetBackground) BrushPtr Background;
					__uproperty(get = GetStrokeStyle, put = SetStrokeStyle) StrokeStylePtr StrokeStyle;
					__uproperty(get = GetPadding, put = SetPadding) Thickness Padding;
					__uproperty(get = GetCornerRadius, put = SetCornerRadius) System::CornerRadius CornerRadius;
					__uproperty(get = GetBorderThickness, put = SetBorderThickness) double BorderThickness;

					__usctordef(Border)
					REFLECT_CLASS(Border)
				};
			}
		}
	}
}