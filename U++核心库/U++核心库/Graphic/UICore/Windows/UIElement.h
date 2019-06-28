#pragma once
#include "Visual.h"
#include "IInputElement.h"

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			__uenum __uobjdef(asm = UICore, ns = System::UI::Windows, ver = 1.0) UICORE_API Visibility : __uobject
			{
				__uvalue_begin(Visibility, UInt8)
				__uvalue(Visible,0)
				__uvalue(Hidden)
				__uvalue(Collapsed)
				__uvalue_end(Visibility, Visible)
			};

			__uclass __uobjdef(asm = UICore, ns = System::UI::Windows, ver = 1.0) UICORE_API UIElement: public IInputElement, public Visual
			{
			protected:
				virtual void OnRenderSizeChanged(const Size& newSize);
			public:
				__udefctor UIElement();
				~UIElement();
			private:
				void OnDesiredSizeChangedCore(const System::Size& size) override final;
				void OnRenderSizeChangedCore(const System::Size& size) override final;

				System::UI::Graphic::EffectPtr GetEffectCore() const override final;
				EffectCompositeMode GetCompositeModeCore() const override final;

				System::UI::Graphic::GeometryPtr GetClipCore() const override final;
				double GetOpacityCore() const override final;
				System::UI::Graphic::BrushPtr GetOpacityMaskCore() const override final;

				bool GetClipToBoundsCore() const override final;
				System::Thickness GetMarginCore() const override final;

				System::Size GetSizeCore() const override final;
				System::Size GetMinimumSizeCore() const override final;
				System::Size GetMaximumSizeCore() const override final;

				System::Size GetDesiredSizeCore() const override final;
				System::Size GetRenderSizeCore() const override final;

				System::UI::Graphic::VerticalAlignment GetVerticalAlignmentCore() const override final;
				System::UI::Graphic::HorizontalAlignment GetHorizontalAlignmentCore() const override final;

				bool GetBitmapCacheEnabledCore()  const override final;
				double GetBitmapCacheRenderScaleCore()  const override final;

			private:
				static bool OnFocusableChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool OnIsFocusedChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool OnIsHitTestVisibleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool CoerceIsHitTestVisible(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal);
				static bool OnRenderTransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool OnRenderTransformOriginChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool CoerceRenderTransformOrigin(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal);
				static bool OnIsVisibleChanged(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal);
				static bool OnVisibilityChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool CoerceVisibility(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal);
				
				static bool CoerceMargin(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal);
				static bool CoerceBitmapCacheRenderScale(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal);
				static bool OnBitmapCacheRenderScaleChanged(DependencyObject* const dependencyObject, const PropertyChangedEventArgs& changedArgs);

			public:
				static const DependencyPropertyPtr AllowDropProperty;
				static const DependencyPropertyPtr ClipProperty;
				static const DependencyPropertyPtr ClipToBoundsProperty; 
				static const DependencyPropertyPtr EffectProperty;
				static const DependencyPropertyPtr FocusableProperty;
				static const DependencyPropertyPtr IsFocusedProperty;
				static const DependencyPropertyPtr IsHitTestVisibleProperty;
				static const DependencyPropertyPtr OpacityProperty;
				static const DependencyPropertyPtr OpacityMaskProperty;
				static const DependencyPropertyPtr RenderTransformProperty;
				static const DependencyPropertyPtr RenderTransformOriginProperty;
				static const DependencyPropertyPtr IsVisibleProperty;
				static const DependencyPropertyPtr VisibilityProperty;

				static const DependencyPropertyPtr SizeProperty;
				static const DependencyPropertyPtr MinimumSizeProperty;
				static const DependencyPropertyPtr MaximumSizeProperty;
				static const DependencyPropertyPtr DesiredSizeProperty;
				static const DependencyPropertyPtr RenderSizeProperty;
				static const DependencyPropertyPtr MarginProperty;
				static const DependencyPropertyPtr VerticalAlignmentProperty;
				static const DependencyPropertyPtr HorizontalAlignmentProperty;

				static const DependencyPropertyPtr BitmapCacheEnabledProperty;
				static const DependencyPropertyPtr BitmapCacheRenderScaleProperty;

			public:
				static const RoutedEventPtr PreviewMouseMoveEvent;
				static const RoutedEventPtr MouseMoveEvent;
				static const RoutedEventPtr PreviewMouseDownEvent;
				static const RoutedEventPtr MouseDownEvent;
				static const RoutedEventPtr PreviewMouseUpEvent;
				static const RoutedEventPtr MouseUpEvent;
				static const RoutedEventPtr PreviewMouseWheelEvent;
				static const RoutedEventPtr MouseWheelEvent;

				static const RoutedEventPtr PreviewMouseLeftButtonDownEvent;
				static const RoutedEventPtr MouseLeftButtonDownEvent;
				static const RoutedEventPtr PreviewMouseLeftButtonUpEvent;
				static const RoutedEventPtr MouseLeftButtonUpEvent;
				static const RoutedEventPtr PreviewMouseRightButtonDownEvent;
				static const RoutedEventPtr MouseRightButtonDownEvent;
				static const RoutedEventPtr PreviewMouseRightButtonUpEvent;
				static const RoutedEventPtr MouseRightButtonUpEvent;

				static const RoutedEventPtr MouseEnterEvent;
				static const RoutedEventPtr MouseLeaveEvent;

				static const RoutedEventPtr GotMouseCaptureEvent;
				static const RoutedEventPtr LostMouseCaptureEvent;
			public:
				bool GetAllowDrop() const;
				void SetAllowDrop(bool val);

				GeometryPtr GetClip() const;
				void SetClip(const GeometryPtr&);
				bool GetClipToBounds() const;
				void SetClipToBounds(bool);
				EffectPtr GetEffect() const;
				void SetEffect(const EffectPtr&);
				bool GetFocusable() const;
				void SetFocusable(bool);
				bool GetIsFocused() const;
				bool GetIsHitTestVisible() const;
				void SetIsHitTestVisible(bool);
				
				double GetOpacity() const;
				void SetOpacity(double opac);
				BrushPtr GetOpacityMask() const;
				void SetOpacityMask(const BrushPtr&);

				TransformPtr GetRenderTransform() const;
				void SetRenderTransform(const TransformPtr&);
				Point GetRenderTransformOrigin() const;
				void SetRenderTransformOrigin(const Point&);
				bool GetIsVisible() const;
				System::UI::Windows::Visibility GetVisibility() const;
				void SetVisibility(const System::UI::Windows::Visibility&);

				System::Size GetSize() const;
				void SetSize(const System::Size& val);
				System::Size GetMinimumSize() const;
				void SetMinimumSize(const System::Size& val);
				System::Size GetMaximumSize() const;
				void SetMaximumSize(const System::Size& val);
				System::Size GetDesiredSize() const;
				System::Size GetRenderSize() const;
				System::Thickness GetMargin() const;
				void SetMargin(const System::Thickness& val);
				System::UI::Graphic::VerticalAlignment GetVerticalAlignment() const;
				void SetVerticalAlignment(const System::UI::Graphic::VerticalAlignment& val);
				System::UI::Graphic::HorizontalAlignment GetHorizontalAlignment() const;
				void SetHorizontalAlignment(const System::UI::Graphic::HorizontalAlignment& val);

				bool GetBitmapCacheEnabled() const;
				void SetBitmapCacheEnabled(bool);
				double GetBitmapCacheRenderScale() const;
				void SetBitmapCacheRenderScale(double);
					
			public:
				__uproperty(get = GetAllowDrop, put = SetAllowDrop) bool AllowDrop;
				__uproperty(get = GetClip, put = SetClip) GeometryPtr Clip;
				__uproperty(get = GetClipToBounds, put = SetClipToBounds) bool ClipToBounds;
				__uproperty(get = GetEffect, put = SetEffect) EffectPtr Effect;
				__uproperty(get = GetFocusable, put = SetFocusable) bool Focusable;
				__uproperty(get = GetIsFocused) bool IsFocused;
				__uproperty(get = GetIsHitTestVisible, put = SetIsHitTestVisible) bool IsHitTestVisible;
				__uproperty(get = GetOpacity, put = SetOpacity) double Opacity;
				__uproperty(get = GetOpacityMask, put = SetOpacityMask) BrushPtr OpacityMask;
				__uproperty(get = GetRenderTransform, put = SetRenderTransform) TransformPtr RenderTransform;
				__uproperty(get = GetRenderTransformOrigin, put = SetRenderTransformOrigin) Point RenderTransformOrigin;
				__uproperty(get = GetIsVisible) bool IsVisible;
				__uproperty(get = GetVisibility, put = SetVisibility) System::UI::Windows::Visibility Visibility;

				__uproperty(get = GetSize, put = SetSize) System::Size Size;
				__uproperty(get = GetMinimumSize, put = SetMinimumSize) System::Size MinimumSize;
				__uproperty(get = GetMaximumSize, put = SetMaximumSize) System::Size MaximumSize;
				__uproperty(get = GetDesiredSize) System::Size DesiredSize;
				__uproperty(get = GetRenderSize) System::Size RenderSize;

				__uproperty(get = GetMargin, put = SetMargin) Thickness Margin;
				__uproperty(get = GetVerticalAlignment, put = SetVerticalAlignment) System::UI::Graphic::VerticalAlignment VerticalAlignment;
				__uproperty(get = GetHorizontalAlignment, put = SetHorizontalAlignment) System::UI::Graphic::HorizontalAlignment HorizontalAlignment;
				__uproperty(get = GetBitmapCacheEnabled, put = SetBitmapCacheEnabled) bool BitmapCacheEnabled;
				__uproperty(get = GetBitmapCacheRenderScale, put = SetBitmapCacheRenderScale) double BitmapCacheRenderScale;

			protected:
				void RenderOverrideCore(const RenderContextPtr& renderContext) override final;
				System::Size MeasureOverrideCore(const System::Size& availableSize) override final;
				System::Size ArrangeOverrideCore(const System::Size& finalSize) override final;
				virtual void RenderOverride(const RenderContextPtr& renderContext);
				virtual System::Size MeasureOverride(const System::Size& availableSize);
				virtual System::Size ArrangeOverride(const System::Size& finalSize);

			private:
				//static 

				static void OnPreviewMouseMoveCore(DependencyObjectPtr, MouseEventArgs& e);
				static void OnMouseMoveCore(DependencyObjectPtr, MouseEventArgs& e);

				static void OnPreviewMouseDownCore(DependencyObjectPtr, MouseButtonEventArgs& e);
				static void OnMouseDownCore(DependencyObjectPtr, MouseButtonEventArgs& e);

				static void OnPreviewMouseUpCore(DependencyObjectPtr, MouseButtonEventArgs& e);
				static void OnMouseUpCore(DependencyObjectPtr, MouseButtonEventArgs& e);

				static void OnPreviewMouseWheelCore(DependencyObjectPtr, MouseWheelEventArgs& e);
				static void OnMouseWheelCore(DependencyObjectPtr, MouseWheelEventArgs& e);
				
				static void OnMouseEnterCore(DependencyObjectPtr, MouseEventArgs& e);
				static void OnMouseLeaveCore(DependencyObjectPtr, MouseEventArgs& e);

				static void OnGotMouseCaptureCore(DependencyObjectPtr, MouseEventArgs& e);
				static void OnLostMouseCaptureCore(DependencyObjectPtr, MouseEventArgs& e);
			protected:
				virtual void OnPreviewMouseMove(MouseEventArgs& e);
				virtual void OnMouseMove(MouseEventArgs& e);

				virtual void OnPreviewMouseDown(MouseButtonEventArgs& e);
				virtual void OnMouseDown(MouseButtonEventArgs& e);

				virtual void OnPreviewMouseUp(MouseButtonEventArgs& e);
				virtual void OnMouseUp(MouseButtonEventArgs& e);

				virtual void OnPreviewMouseWheel(MouseWheelEventArgs& e);
				virtual void OnMouseWheel(MouseWheelEventArgs& e);

				virtual void OnMouseEnter(MouseEventArgs& e);
				virtual void OnMouseLeave(MouseEventArgs& e);

				virtual void OnGotMouseCapture(MouseEventArgs& e);
				virtual void OnLostMouseCapture(MouseEventArgs& e);

				__usctordef(UIElement)
				REFLECT_CLASS(UIElement)
			};

			typedef std::shared_ptr<UIElement> UIElementPtr;
		}
	}
}