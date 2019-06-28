#include "Panel.h"
using namespace System::UI::Graphic; 
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;

Panel::Panel(void)
{
}

Panel::~Panel(void)
{
}

bool Panel::HitTest(const Point&)
{
	return false;
}

int Panel::GetChildCount() const
{
	return GetVisualChildCount();
}

const UIElementPtr Panel::GetChild(int index) const
{
	return std::dynamic_pointer_cast<UIElement>(GetVisualChild(index));
}

void Panel::RemoveChild(const UIElementPtr& child)
{
	Visual::RemoveVisualChild(child);
	InvalidateMeasure();
}

void Panel::AddChild(const UIElementPtr& child)
{
	Visual::AddVisualChild(child);
	InvalidateMeasure();
}

System::Size Panel::MeasureOverride(const System::Size& availableSize)
{
	auto available = availableSize;
	auto constraint = available;

	auto size = System::Size(0, 0);

	for (int i = 0; i < GetChildCount(); i++)
	{
		auto child = GetChild(i);

		child->Measure(available);
		auto desiredSize = child->DesiredSize;

		size.Width = max(desiredSize.Width, size.Width);
		size.Height = max(desiredSize.Height, size.Height);
	}

	return size;
}

System::Size Panel::ArrangeOverride(const System::Size& finalSize)
{
	auto finalRect = Rect(0, 0, finalSize.Width, finalSize.Height);

	for (int i = 0; i < GetChildCount(); i++)
	{
		auto element = GetChild(i);

		element->Arrange(finalRect);
	}

	return finalSize;
}
