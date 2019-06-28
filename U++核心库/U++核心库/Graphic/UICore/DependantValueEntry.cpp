#include "DependantValueEntry.h"
#include "DependencyObject.h"
using namespace System::UI;

DependantValueEntry::DependantValueEntry(const DependencyPropertyPtr& dependencyProperty, DependencyObject* const dependencyObject) : isAnimated(FALSE), dependencyProperty(dependencyProperty), dependencyObject(dependencyObject)
{

}

DependantValueEntry::~DependantValueEntry()
{
	UnregisterDependants();
}

DependencyPropertyPtr DependantValueEntry::DependencyProperty() const { return dependencyProperty; }

void DependantValueEntry::BaseValue(const SharedVoid val)
{
	baseValue = val;
	RegisterDependantCalculatedValue();
}

SharedVoid DependantValueEntry::AnimatedValue() const { return animatedValue; }

void DependantValueEntry::AnimatedValue(const SharedVoid& val)
{
	animatedValue = val;
	RegisterDependantCalculatedValue();
}

SharedVoid DependantValueEntry::InheritatedValue() const { return inheritatedValue; }

void DependantValueEntry::InheritatedValue(const SharedVoid& val)
{
	inheritatedValue = val;
	RegisterDependantCalculatedValue();
}

bool DependantValueEntry::HasCalculatedValue()  const
{
	return (IsAnimated() && animatedValue) || baseValue || inheritatedValue;
}

bool DependantValueEntry::HasBaseValue() const
{
	return baseValue != nullptr;
}

bool DependantValueEntry::HasInheritedValue() const
{
	return inheritatedValue != nullptr;
}

bool DependantValueEntry::HasAnimatedValue() const
{
	return animatedValue != nullptr;
}

SharedVoid DependantValueEntry::CalculatedValue()  const
{
	SharedVoid returnValue;

	if (IsAnimated() && animatedValue)
	{
		returnValue = animatedValue;
	}
	else if (baseValue)
	{
		returnValue = baseValue;
	}
	else if (inheritatedValue)
	{
		returnValue = inheritatedValue;
	}

	return returnValue;
}

bool DependantValueEntry::IsAnimated() const { return isAnimated; }

void DependantValueEntry::RegisterDependantCalculatedValue()
{
	bool isDependantType = dependencyProperty->PropertyMetadata->IsDepenantObjectType();

	if (!isDependantType)
	{
		return;
	}

	UnregisterDependants();
	auto calcedValue = CalculatedValue();

	if (calcedValue)
	{
		auto item = static_cast<DependantObject*>(calcedValue.get());
		item->RegisterInvalidatable(reinterpret_cast<IInvalidatable*>(dependencyObject));
	}
}

void DependantValueEntry::UnregisterDependants()
{
	if (dependencyProperty->PropertyMetadata->IsDepenantObjectType())
	{
		if (animatedValue)
		{
			auto item = static_cast<DependantObject*>(animatedValue.get());
			item->UnregisterInvalidatable(reinterpret_cast<IInvalidatable*>(dependencyObject));
		}

		if (inheritatedValue)
		{
			auto item = reinterpret_cast<DependantObject*>(inheritatedValue.get());
			item->UnregisterInvalidatable(reinterpret_cast<IInvalidatable*>(dependencyObject));
		}

		if (baseValue)
		{
			auto item = static_cast<DependantObject*>(baseValue.get());
			item->UnregisterInvalidatable(reinterpret_cast<IInvalidatable*>(dependencyObject));
		}
	}
}