#include "UIElement.h"
#include "..\Graphic\Graphics.h"
using namespace System::UI;
using namespace System::UI::Windows;


const DependencyPropertyPtr UIElement::AllowDropProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::ClipProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::ClipToBoundsProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::EffectProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::FocusableProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::IsFocusedProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::IsHitTestVisibleProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::OpacityProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::OpacityMaskProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::RenderTransformProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::RenderTransformOriginProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::IsVisibleProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::VisibilityProperty = make_dependencyproperty<UIElement>();

const DependencyPropertyPtr UIElement::SizeProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::MinimumSizeProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::MaximumSizeProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::DesiredSizeProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::RenderSizeProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::MarginProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::VerticalAlignmentProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::HorizontalAlignmentProperty = make_dependencyproperty<UIElement>();

const DependencyPropertyPtr UIElement::BitmapCacheEnabledProperty = make_dependencyproperty<UIElement>();
const DependencyPropertyPtr UIElement::BitmapCacheRenderScaleProperty = make_dependencyproperty<UIElement>();

const RoutedEventPtr UIElement::PreviewMouseMoveEvent = make_routedevent<UIElement>();


__usctor(UIElement)
{
	UIElement::AllowDropProperty.get()->Register<UIElement>(L"AllowDrop", FrameworkPropertyMetadata::Create<bool>(false));
	UIElement::ClipProperty.get()->Register<UIElement>(L"Clip", FrameworkPropertyMetadata::Create<GeometryPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	UIElement::ClipToBoundsProperty.get()->Register<UIElement>(L"ClipToBounds", FrameworkPropertyMetadata::Create<bool>(false, FrameworkPropertyMetadataOptions::AffectsArrange));
	UIElement::EffectProperty.get()->Register<UIElement>(L"Effect", FrameworkPropertyMetadata::Create<EffectPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	UIElement::FocusableProperty.get()->Register<UIElement>(L"Focusable", FrameworkPropertyMetadata::Create<bool>(false, &OnFocusableChanged));
	UIElement::IsFocusedProperty.get()->RegisterReadOnly<UIElement>(L"IsFocused", FrameworkPropertyMetadata::Create<bool>(false, &OnIsFocusedChanged));
	UIElement::IsHitTestVisibleProperty.get()->Register<UIElement>(L"IsHitTestVisible", FrameworkPropertyMetadata::Create<bool>(false, &OnIsHitTestVisibleChanged, &CoerceIsHitTestVisible));
	UIElement::OpacityProperty.get()->Register<UIElement>(L"Opacity", FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsRender));
	UIElement::OpacityMaskProperty.get()->Register<UIElement>(L"OpacityMask", FrameworkPropertyMetadata::Create<BrushPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	UIElement::RenderTransformProperty.get()->Register<UIElement>(L"RenderTransform", FrameworkPropertyMetadata::Create<Transform>(Transform::Identity(), &OnRenderTransformChanged));
	UIElement::RenderTransformOriginProperty.get()->Register<UIElement>(L"RenderTransformOrigin", FrameworkPropertyMetadata::Create<System::Point>(System::Point{ 0.0,0.0 }, &OnRenderTransformOriginChanged, &CoerceRenderTransformOrigin));
	UIElement::IsVisibleProperty.get()->RegisterReadOnly<UIElement>(L"IsVisible", FrameworkPropertyMetadata::Create<bool>(false, &OnIsVisibleChanged));
	UIElement::VisibilityProperty.get()->Register<UIElement>(L"Visibility", FrameworkPropertyMetadata::Create<System::UI::Windows::Visibility>((System::UI::Windows::Visibility)System::UI::Windows::Visibility::Visible, &OnVisibilityChanged, &CoerceVisibility));

	UIElement::SizeProperty.get()->Register<UIElement>(L"Size", FrameworkPropertyMetadata::Create<System::Size>(System::Size::Infitite(), FrameworkPropertyMetadataOptions::AffectsMeasure));
	UIElement::MinimumSizeProperty.get()->Register<UIElement>(L"MinimumSize", FrameworkPropertyMetadata::Create<System::Size>(System::Size{ 0.0,0.0 }, FrameworkPropertyMetadataOptions::AffectsMeasure));
	UIElement::MaximumSizeProperty.get()->Register<UIElement>(L"MaximumSize", FrameworkPropertyMetadata::Create<System::Size>(System::Size::Infitite(), FrameworkPropertyMetadataOptions::AffectsMeasure));
	UIElement::DesiredSizeProperty.get()->RegisterReadOnly<UIElement>(L"DesiredSize", FrameworkPropertyMetadata::Create<System::Size>(System::Size{ 0.0,0.0 }));
	UIElement::RenderSizeProperty.get()->RegisterReadOnly<UIElement>(L"RenderSize", FrameworkPropertyMetadata::Create<System::Size>(System::Size{ 0.0,0.0 }));
	UIElement::MarginProperty.get()->Register<UIElement>(L"Margin", FrameworkPropertyMetadata::Create<System::Thickness>(System::Thickness{ 0.0,0.0,0.0,0.0 }, FrameworkPropertyMetadataOptions::AffectsArrange, &CoerceMargin));
	UIElement::HorizontalAlignmentProperty.get()->Register<UIElement>(L"HorizontalAlignment", FrameworkPropertyMetadata::Create<System::UI::Graphic::HorizontalAlignment>((System::UI::Graphic::HorizontalAlignment)System::UI::Graphic::HorizontalAlignment::Stretch, FrameworkPropertyMetadataOptions::AffectsArrange));
	UIElement::VerticalAlignmentProperty.get()->Register<UIElement>(L"VerticalAlignment", FrameworkPropertyMetadata::Create<System::UI::Graphic::VerticalAlignment>((System::UI::Graphic::VerticalAlignment)System::UI::Graphic::VerticalAlignment::Stretch, FrameworkPropertyMetadataOptions::AffectsArrange));

	UIElement::BitmapCacheEnabledProperty.get()->Register<UIElement>(L"BitmapCacheEnabled", FrameworkPropertyMetadata::Create<bool>(false, FrameworkPropertyMetadataOptions::AffectsRender));
	UIElement::BitmapCacheRenderScaleProperty.get()->Register<UIElement>(L"BitmapCacheRenderScale", FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsRender, &OnBitmapCacheRenderScaleChanged, &CoerceBitmapCacheRenderScale));


	UIElement::PreviewMouseMoveEvent.get()->Register<UIElement, Mouse, MouseEventArgs>(L"PreviewMouseMove", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseEventArgs>(Mouse::PreviewMouseMoveEvent, UIElement::OnPreviewMouseMoveCore, false);
	UIElement::MouseMoveEvent.get()->Register<UIElement, Mouse, MouseEventArgs>(L"MouseMove", EventStrategy::Bubble);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseEventArgs>(Mouse::MouseMoveEvent, UIElement::OnMouseMoveCore, false);

	static const RoutedEventPtr PreviewMouseUpEvent;
	static const RoutedEventPtr MouseUpEvent;
	static const RoutedEventPtr PreviewMouseWheelEvent;
	static const RoutedEventPtr MouseWheelEvent;

	static const RoutedEventPtr MouseEnterEvent;
	static const RoutedEventPtr MouseLeaveEvent;

	static const RoutedEventPtr GotMouseCaptureEvent;
	static const RoutedEventPtr LostMouseCaptureEvent;

	UIElement::PreviewMouseDownEvent.get()->Register<UIElement, Mouse, MouseButtonEventArgs>(L"PreviewMouseDown", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseButtonEventArgs>(Mouse::PreviewMouseDownEvent, UIElement::OnPreviewMouseDownCore, true);
	UIElement::MouseDownEvent.get()->Register<UIElement, Mouse, MouseButtonEventArgs>(L"MouseDown", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseButtonEventArgs>(Mouse::MouseDownEvent, UIElement::OnMouseDownCore, true);

	UIElement::PreviewMouseUpEvent.get()->Register<UIElement, Mouse, MouseButtonEventArgs>(L"PreviewMouseUp", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseButtonEventArgs>(Mouse::PreviewMouseUpEvent, UIElement::OnPreviewMouseUpCore, true);
	UIElement::MouseUpEvent.get()->Register<UIElement, Mouse, MouseButtonEventArgs>(L"MouseUp", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseButtonEventArgs>(Mouse::MouseUpEvent, UIElement::OnMouseUpCore, true);

	UIElement::PreviewMouseWheelEvent.get()->Register<UIElement, Mouse, MouseWheelEventArgs>(L"PreviewMouseWheel", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseWheelEventArgs>(Mouse::PreviewMouseWheelEvent, UIElement::OnPreviewMouseWheelCore, false);
	UIElement::MouseWheelEvent.get()->Register<UIElement, Mouse, MouseWheelEventArgs>(L"MouseWheel", EventStrategy::Tunnel);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseWheelEventArgs>(Mouse::MouseWheelEvent, UIElement::OnMouseWheelCore, false);

	UIElement::PreviewMouseLeftButtonDownEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"PreviewMouseLeftButtonDown", EventStrategy::Direct);
	UIElement::PreviewMouseLeftButtonUpEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"PreviewMouseLeftButtonUp", EventStrategy::Direct);
	UIElement::PreviewMouseRightButtonDownEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"PreviewMouseRightButtonDown", EventStrategy::Direct);
	UIElement::PreviewMouseRightButtonUpEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"PreviewMouseRightButtonUp", EventStrategy::Direct);
	UIElement::MouseLeftButtonDownEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"MouseLeftButtonDown", EventStrategy::Direct);
	UIElement::MouseLeftButtonUpEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"MouseLeftButtonUp", EventStrategy::Direct);
	UIElement::MouseRightButtonDownEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"MouseRightButtonDown", EventStrategy::Direct);
	UIElement::MouseRightButtonUpEvent.get()->Register<UIElement, MouseButtonEventArgs>(L"MouseRightButtonUp", EventStrategy::Direct);

	UIElement::MouseEnterEvent.get()->Register<UIElement, Mouse, MouseEventArgs>(L"MouseEnter", EventStrategy::Direct);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseEventArgs>(Mouse::MouseEnterEvent, UIElement::OnMouseEnterCore, false);

	UIElement::MouseLeaveEvent.get()->Register<UIElement, Mouse, MouseEventArgs>(L"MouseLeave", EventStrategy::Direct);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseEventArgs>(Mouse::MouseLeaveEvent, UIElement::OnMouseLeaveCore, false);

	UIElement::GotMouseCaptureEvent.get()->Register<UIElement, Mouse, MouseEventArgs>(L"GotMouseCapture", EventStrategy::Bubble);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseEventArgs>(Mouse::GotMouseCaptureEvent, UIElement::OnGotMouseCaptureCore, false);

	UIElement::LostMouseCaptureEvent.get()->Register<UIElement, Mouse, MouseEventArgs>(L"LostMouseCapture", EventStrategy::Bubble);
	EventManager::RegisterClassRoutedEvent<UIElement, MouseEventArgs>(Mouse::LostMouseCaptureEvent, UIElement::OnLostMouseCaptureCore, false);


	return true;
}

UIElement::UIElement()
{
	RegisterRoutedEvent<MouseEventArgs>(&PreviewMouseMove, UIElement::PreviewMouseMoveEvent);
	RegisterRoutedEvent<MouseEventArgs>(&MouseMove, UIElement::MouseMoveEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&PreviewMouseDown, UIElement::PreviewMouseDownEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&MouseDown, UIElement::MouseDownEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&PreviewMouseUp, UIElement::PreviewMouseUpEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&MouseUp, UIElement::MouseUpEvent);
	RegisterRoutedEvent<MouseWheelEventArgs>(&PreviewMouseWheel, UIElement::PreviewMouseWheelEvent);
	RegisterRoutedEvent<MouseWheelEventArgs>(&MouseWheel, UIElement::MouseWheelEvent);

	RegisterRoutedEvent<MouseButtonEventArgs>(&PreviewMouseLeftButtonDown, UIElement::PreviewMouseLeftButtonDownEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&MouseLeftButtonDown, UIElement::MouseLeftButtonDownEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&PreviewMouseLeftButtonUp, UIElement::PreviewMouseLeftButtonUpEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&MouseLeftButtonUp, UIElement::MouseLeftButtonUpEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&PreviewMouseRightButtonDown, UIElement::PreviewMouseRightButtonDownEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&MouseRightButtonDown, UIElement::MouseRightButtonDownEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&PreviewMouseRightButtonUp, UIElement::PreviewMouseRightButtonUpEvent);
	RegisterRoutedEvent<MouseButtonEventArgs>(&MouseRightButtonUp, UIElement::MouseRightButtonUpEvent);

	RegisterRoutedEvent<MouseEventArgs>(&MouseEnter, UIElement::MouseEnterEvent);
	RegisterRoutedEvent<MouseEventArgs>(&MouseLeave, UIElement::MouseLeaveEvent);
	RegisterRoutedEvent<MouseEventArgs>(&GotMouseCapture, UIElement::GotMouseCaptureEvent);
	RegisterRoutedEvent<MouseEventArgs>(&LostMouseCapture, UIElement::LostMouseCaptureEvent);

}

UIElement::~UIElement()
{
}

void UIElement::RenderOverrideCore(const RenderContextPtr& renderContext)
{
	RenderOverride(renderContext);
}

System::Size UIElement::MeasureOverrideCore(const System::Size& availableSize)
{
	return MeasureOverride(availableSize);
}

System::Size UIElement::ArrangeOverrideCore(const System::Size& finalSize)
{
	return ArrangeOverride(finalSize);
}

void UIElement::RenderOverride(const RenderContextPtr& renderContext)
{
	
}

System::Size UIElement::MeasureOverride(const System::Size& availableSize)
{
	return System::Size{ 0.0,0.0 };
}

System::Size UIElement::ArrangeOverride(const System::Size& finalSize)
{
	return System::Size{ 0.0,0.0 };
}

#pragma region Properties

bool UIElement::GetAllowDrop() const
{
	return GetValue<bool>(AllowDropProperty);
}

void UIElement::SetAllowDrop(bool val)
{
	SetValue(AllowDropProperty, val);
}

GeometryPtr UIElement::GetClip() const
{
	return GetValuePtr<Geometry>(ClipProperty);
}

void UIElement::SetClip(const GeometryPtr& val)
{
	SetValue(ClipProperty, val);
}

bool UIElement::GetClipToBounds() const
{
	return GetValue<bool>(ClipToBoundsProperty);
}

void UIElement::SetClipToBounds(bool val)
{
	SetValue(ClipToBoundsProperty, val);
}

EffectPtr UIElement::GetEffect() const
{
	return GetValuePtr<System::UI::Graphic::Effect>(EffectProperty);
}

void UIElement::SetEffect(const EffectPtr& val)
{
	SetValue(EffectProperty, val);
}

bool UIElement::GetFocusable() const
{
	return GetValue<bool>(FocusableProperty);
}

void UIElement::SetFocusable(bool val)
{
	SetValue(FocusableProperty, val);
}

bool UIElement::GetIsFocused() const
{
	/*need*/
	return false;
}

bool UIElement::GetIsHitTestVisible() const
{
	return GetValue<bool>(IsHitTestVisibleProperty);
}

void UIElement::SetIsHitTestVisible(bool val)
{
	SetValue(IsHitTestVisibleProperty, val);
}

double UIElement::GetOpacity() const
{
	return GetValue<double>(OpacityProperty);
}

void UIElement::SetOpacity(double val)
{
	SetValue(OpacityProperty, val);
}

BrushPtr UIElement::GetOpacityMask() const
{
	return GetValuePtr<System::UI::Graphic::Brush>(OpacityMaskProperty);
}

void UIElement::SetOpacityMask(const BrushPtr& val)
{
	SetValue(OpacityMaskProperty, val);
}

TransformPtr UIElement::GetRenderTransform() const
{
	return GetValuePtr<System::UI::Media::Transform>(RenderTransformProperty);
}

void UIElement::SetRenderTransform(const TransformPtr& val)
{
	SetValue(RenderTransformProperty, val);
}

System::Point UIElement::GetRenderTransformOrigin() const
{
	return GetValue<System::Point>(RenderTransformOriginProperty);
}

void UIElement::SetRenderTransformOrigin(const System::Point& val)
{
	SetValue(RenderTransformOriginProperty, val);
}

bool UIElement::GetIsVisible() const
{
	/*need*/
	return true;
}

System::UI::Windows::Visibility UIElement::GetVisibility() const
{
	return GetValue<System::UI::Windows::Visibility>(VisibilityProperty);
}

void UIElement::SetVisibility(const System::UI::Windows::Visibility& val)
{
	SetValue(VisibilityProperty, val);
}

System::Size UIElement::GetSize() const
{
	return GetValue<System::Size>(SizeProperty);
}

void UIElement::SetSize(const System::Size& val)
{
	SetValue(SizeProperty, val);
}

System::Size UIElement::GetMinimumSize() const
{
	return GetValue<System::Size>(MinimumSizeProperty);
}

void UIElement::SetMinimumSize(const System::Size& val)
{
	SetValue(MinimumSizeProperty, val);
}

System::Size UIElement::GetMaximumSize() const
{
	return GetValue<System::Size>(MaximumSizeProperty);
}

void UIElement::SetMaximumSize(const System::Size& val)
{
	SetValue(MaximumSizeProperty, val);
}

Size UIElement::GetDesiredSize() const
{
	return GetValue<System::Size>(DesiredSizeProperty);
}

Size UIElement::GetRenderSize() const
{
	return GetValue<System::Size>(RenderSizeProperty);
}

System::Thickness UIElement::GetMargin() const
{
	return GetValue<System::Thickness>(MarginProperty);
}

void UIElement::SetMargin(const System::Thickness& val)
{
	SetValue(MarginProperty, val);
}

System::UI::Graphic::VerticalAlignment UIElement::GetVerticalAlignment() const
{
	return GetValue<System::UI::Graphic::VerticalAlignment>(VerticalAlignmentProperty);
}

void UIElement::SetVerticalAlignment(const System::UI::Graphic::VerticalAlignment& val)
{
	SetValue(VerticalAlignmentProperty, val);
}

System::UI::Graphic::HorizontalAlignment UIElement::GetHorizontalAlignment() const
{
	return GetValue<System::UI::Graphic::HorizontalAlignment>(HorizontalAlignmentProperty);
}

void UIElement::SetHorizontalAlignment(const System::UI::Graphic::HorizontalAlignment& val)
{
	SetValue(HorizontalAlignmentProperty, val);
}

bool UIElement::GetBitmapCacheEnabled() const
{
	return GetValue<bool>(BitmapCacheEnabledProperty);
}

void UIElement::SetBitmapCacheEnabled(bool val)
{
	SetValue(BitmapCacheEnabledProperty, val);
}

double UIElement::GetBitmapCacheRenderScale() const
{
	return GetValue<double>(BitmapCacheRenderScaleProperty);
}

void UIElement::SetBitmapCacheRenderScale(double val)
{
	SetValue(BitmapCacheRenderScaleProperty, val);
}

#pragma endregion

#pragma region GetValueCore

System::UI::Graphic::EffectPtr UIElement::GetEffectCore() const
{
	return GetValuePtr<System::UI::Graphic::Effect>(EffectProperty);
}

EffectCompositeMode UIElement::GetCompositeModeCore() const
{
	return EffectCompositeMode::SourceOver;
}

System::UI::Graphic::GeometryPtr UIElement::GetClipCore() const
{
	return GetValuePtr<Geometry>(ClipProperty);
}

double UIElement::GetOpacityCore() const
{
	return GetValue<double>(OpacityProperty);
}

System::UI::Graphic::BrushPtr UIElement::GetOpacityMaskCore() const
{
	return GetValuePtr<Brush>(OpacityMaskProperty);
}

bool UIElement::GetClipToBoundsCore() const
{
	return GetValue<bool>(ClipToBoundsProperty);
}

System::Thickness UIElement::GetMarginCore() const
{
	return GetValue<System::Thickness>(MarginProperty);
}

System::Size UIElement::GetSizeCore() const
{
	return GetValue<System::Size>(SizeProperty);
}

System::Size UIElement::GetMinimumSizeCore() const
{
	return GetValue<System::Size>(MinimumSizeProperty);
}

System::Size UIElement::GetMaximumSizeCore() const
{
	return GetValue<System::Size>(MaximumSizeProperty);
}

System::Size UIElement::GetDesiredSizeCore() const
{
	return GetValue<System::Size>(DesiredSizeProperty);
}

System::Size UIElement::GetRenderSizeCore() const
{
	return GetValue<System::Size>(RenderSizeProperty);
}

System::UI::Graphic::VerticalAlignment UIElement::GetVerticalAlignmentCore() const
{
	return GetValue<System::UI::Graphic::VerticalAlignment>(VerticalAlignmentProperty);
}

System::UI::Graphic::HorizontalAlignment UIElement::GetHorizontalAlignmentCore() const
{
	return GetValue<System::UI::Graphic::HorizontalAlignment>(HorizontalAlignmentProperty);
}

bool UIElement::GetBitmapCacheEnabledCore() const
{
	return GetValue<bool>(BitmapCacheEnabledProperty);
}

double UIElement::GetBitmapCacheRenderScaleCore() const
{
	return GetValue<double>(BitmapCacheRenderScaleProperty);
}

#pragma endregion

void UIElement::OnRenderSizeChanged(const System::Size& newSize)
{

}

#pragma region OnChangedCore

void UIElement::OnDesiredSizeChangedCore(const System::Size& size)
{
	SetValue(DesiredSizeProperty, size);
}

void UIElement::OnRenderSizeChangedCore(const System::Size& size)
{
	SetValue(RenderSizeProperty, size);
	OnRenderSizeChanged(size);
}
#pragma endregion

#pragma region OnChanged

bool UIElement::OnBitmapCacheRenderScaleChanged(DependencyObject* const dependencyObject, const PropertyChangedEventArgs& changedArgs)
{
	try
	{
		auto visualptr = dynamic_cast<Visual*>(dependencyObject);
		if (visualptr)
		{
			visualptr->push_BitmapCachedInvalid();
		}
	}
	catch (...) {}
	return false;
}

bool UIElement::OnFocusableChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}
bool UIElement::OnIsFocusedChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}
bool UIElement::OnIsHitTestVisibleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}
bool UIElement::OnRenderTransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}
bool UIElement::OnRenderTransformOriginChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}
bool UIElement::OnIsVisibleChanged(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)
{
	return true;
}
bool UIElement::OnVisibilityChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}


#pragma endregion

#pragma region Coerce

bool UIElement::CoerceIsHitTestVisible(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)
{
	/*need*/
	return true;
}

bool UIElement::CoerceRenderTransformOrigin(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)
{
	if (newVal)
	{
		try
		{
			System::Point* val = static_cast<System::Point*>(newVal.get());
			if (val)
			{
				return ((!isnan(val->X) && !isinf(val->X))
					&& (!isnan(val->Y) && !isinf(val->Y)));
			}
		}
		catch (...)
		{

		}
	}
	return false;
}


bool UIElement::CoerceVisibility(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)
{
	if (newVal)
	{
		try
		{
			System::UI::Windows::Visibility* val = static_cast<System::UI::Windows::Visibility*>(newVal.get());
			if (val)
			{
				return *val == Visibility::Visible || *val == Visibility::Collapsed || *val == Visibility::Hidden;
			}
		}
		catch (...)
		{

		}
	}
	return false;
}

bool UIElement::CoerceMargin(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)
{
	if (newVal)
	{
		try
		{
			Thickness* val = static_cast<Thickness*>(newVal.get());
			if (val)
			{
				if (std::isnan(val->Left) || std::isnan(val->Right) || std::isnan(val->Top) || std::isnan(val->Bottom)) return false;
				if (std::isinf(val->Left) || std::isinf(val->Right) || std::isinf(val->Top) || std::isinf(val->Bottom)) return false;
				return true;
			}
		}
		catch (...)
		{

		}
	}
	return false;
}

bool UIElement::CoerceBitmapCacheRenderScale(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)
{
	if (newVal)
	{
		try
		{
			double* val = static_cast<double*>(newVal.get());
			if (val)
			{
				if (*val < 0) newVal = make_shared<double>(0.0);
				return true;
			}
		}
		catch (...)
		{

		}
	}
	return false;
}


#pragma endregion

#pragma region CoreEvents

void UIElement::OnPreviewMouseMoveCore(DependencyObjectPtr sender, MouseEventArgs& e)
{
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnPreviewMouseMove(e);
}
void UIElement::OnMouseMoveCore(DependencyObjectPtr sender, MouseEventArgs& e)
{
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnMouseMove(e);
}

void UIElement::OnPreviewMouseDownCore(DependencyObjectPtr sender, MouseButtonEventArgs& e)
{

}
void UIElement::OnMouseDownCore(DependencyObjectPtr sender, MouseButtonEventArgs& e)
{

}

void UIElement::OnPreviewMouseUpCore(DependencyObjectPtr sender, MouseButtonEventArgs& e)
{

}
void UIElement::OnMouseUpCore(DependencyObjectPtr sender, MouseButtonEventArgs& e)
{

}

void UIElement::OnPreviewMouseWheelCore(DependencyObjectPtr sender, MouseWheelEventArgs& e)
{
	/*check command*/
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnPreviewMouseWheel(e);
}
void UIElement::OnMouseWheelCore(DependencyObjectPtr sender, MouseWheelEventArgs& e)
{
	/*check command*/
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnMouseWheel(e);
}

void UIElement::OnMouseEnterCore(DependencyObjectPtr sender, MouseEventArgs& e)
{
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnMouseEnter(e);
}
void UIElement::OnMouseLeaveCore(DependencyObjectPtr sender, MouseEventArgs& e)
{
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnMouseLeave(e);
}

void UIElement::OnGotMouseCaptureCore(DependencyObjectPtr sender, MouseEventArgs& e)
{
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnGotMouseCapture(e);
}
void UIElement::OnLostMouseCaptureCore(DependencyObjectPtr sender, MouseEventArgs& e)
{
	UIElementPtr uie = std::dynamic_pointer_cast<UIElement>(sender);
	if (uie) uie->OnLostMouseCapture(e);
}

#pragma endregion


#pragma region OnEvents
void UIElement::OnPreviewMouseMove(MouseEventArgs& args)
{

}

void UIElement::OnMouseMove(MouseEventArgs& e)
{

}

void UIElement::OnPreviewMouseDown(MouseButtonEventArgs& e)
{

}
void UIElement::OnMouseDown(MouseButtonEventArgs& e)
{

}

void UIElement::OnPreviewMouseUp(MouseButtonEventArgs& e)
{

}
void UIElement::OnMouseUp(MouseButtonEventArgs& e)
{

}

void UIElement::OnPreviewMouseWheel(MouseWheelEventArgs& e)
{

}
void UIElement::OnMouseWheel(MouseWheelEventArgs& e)
{

}

void UIElement::OnMouseEnter(MouseEventArgs& e)
{

}
void UIElement::OnMouseLeave(MouseEventArgs& e)
{

}

void UIElement::OnGotMouseCapture(MouseEventArgs& e)
{

}
void UIElement::OnLostMouseCapture(MouseEventArgs& e)
{

}
#pragma endregion