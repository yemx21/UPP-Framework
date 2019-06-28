#include "Canvas.h"
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;

const DependencyPropertyPtr Canvas::LeftProperty = make_dependencyproperty<Canvas>();
const DependencyPropertyPtr Canvas::TopProperty = make_dependencyproperty<Canvas>();
const DependencyPropertyPtr Canvas::RightProperty = make_dependencyproperty<Canvas>();
const DependencyPropertyPtr Canvas::BottomProperty = make_dependencyproperty<Canvas>();


__usctor(Canvas)
{
	Canvas::LeftProperty.get()->Register<Canvas>(L"Left", FrameworkPropertyMetadata::Create<double>(std::numeric_limits<double>::quiet_NaN(), FrameworkPropertyMetadataOptions::AffectsMeasure, &LocationChanged));
	Canvas::TopProperty.get()->Register<Canvas>(L"Top", FrameworkPropertyMetadata::Create<double>(std::numeric_limits<double>::quiet_NaN(), FrameworkPropertyMetadataOptions::AffectsMeasure, &LocationChanged));
	Canvas::RightProperty.get()->Register<Canvas>(L"Right", FrameworkPropertyMetadata::Create<double>(std::numeric_limits<double>::quiet_NaN(), FrameworkPropertyMetadataOptions::AffectsMeasure, &LocationChanged));
	Canvas::BottomProperty.get()->Register<Canvas>(L"Bottom", FrameworkPropertyMetadata::Create<double>(std::numeric_limits<double>::quiet_NaN(), FrameworkPropertyMetadataOptions::AffectsMeasure, &LocationChanged));
	return true;
}

Canvas::Canvas(void)
{
}


Canvas::~Canvas(void)
{
}

System::Size Canvas::MeasureOverride(const System::Size& availableSize)
{
	auto childConstraint = System::Size::Infitite();

	for (int i = 0; i < GetVisualChildCount(); i++)
	{
		auto child = GetVisualChild(i);
		child->Measure(childConstraint);
	}

	return System::Size();
}

System::Size Canvas::ArrangeOverride(const System::Size& finalSize)
{
	for (int i = 0; i < GetChildCount(); i++)
	{
		auto child = GetChild(i);

		double xPosition = 0;
		double yPosition = 0;

		double left = GetLeft(child);

		auto childDesiredSize = child->DesiredSize;

		if (!FloatingHelper::IsNaNOrInfinity(left))
			xPosition = left;
		else
		{
			double right = GetRight(child);

			if (!FloatingHelper::IsNaNOrInfinity(right))
				xPosition = finalSize.Width - childDesiredSize.Width - right;
		}

		double top = GetTop(child);

		if (!FloatingHelper::IsNaNOrInfinity(top))
			yPosition = top;
		else
		{
			double bottom = GetBottom(child);

			if (!FloatingHelper::IsNaNOrInfinity(bottom))
				yPosition = finalSize.Height - childDesiredSize.Height - bottom;
		}

		child->Arrange(Rect(xPosition, yPosition, childDesiredSize.Width + xPosition, childDesiredSize.Height + yPosition));
	}

	return finalSize;
}

bool Canvas::LocationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	return true;
}

void Canvas::SetTop(const VisualPtr& element, double top)
{
	element->SetValue(TopProperty, top);
}

void Canvas::SetLeft(const VisualPtr& element, double top)
{
	element->SetValue(LeftProperty, top);
}

void Canvas::SetBottom(const  VisualPtr& element, double top)
{
	element->SetValue(BottomProperty, top);
}

void Canvas::SetRight(const VisualPtr& element, double top)
{
	element->SetValue(RightProperty, top);
}

double Canvas::GetTop(const VisualPtr& element)
{
	return element->GetValue<double>(TopProperty);
}

double Canvas::GetLeft(const VisualPtr& element)
{
	return element->GetValue<double>(LeftProperty);
}

double Canvas::GetBottom(const VisualPtr& element)
{
	return element->GetValue<double>(BottomProperty);
}

double Canvas::GetRight(const VisualPtr& element)
{
	return element->GetValue<double>(RightProperty);
}
