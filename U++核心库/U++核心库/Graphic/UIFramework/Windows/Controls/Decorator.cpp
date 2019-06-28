#include "Decorator.h"
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;


System::Size Decorator::MeasureOverride(const System::Size& availableSize)
{
	UIElementPtr child = Child;
	if (child)
	{
		child->Measure(availableSize);
		return (child->DesiredSize);
	}
	return System::Size{0.0,0.0};
}

System::Size Decorator::ArrangeOverride(const System::Size& finalSize)
{
	UIElementPtr child = Child;
	if (child)
	{
		child->Arrange(Rect{ finalSize});
	}
	return finalSize;
}

Decorator::Decorator()
{

}

Decorator::~Decorator()
{

}

UIElementPtr Decorator::GetChildCore() const
{
	return _child;
}

void Decorator::SetChildCore(UIElementPtr child)
{
	if (_child!=child)
	{
		RemoveVisualChild(_child);
		//RemoveLogicalChild(_child);

		_child = child;

		//AddLogicalChild(value);
		AddVisualChild(child);

		InvalidateMeasure();
	}
}

UIElementPtr Decorator::GetChild() const
{
	return GetChildCore();
}

void Decorator::SetChild(UIElementPtr child)
{
	SetChildCore(child);
}