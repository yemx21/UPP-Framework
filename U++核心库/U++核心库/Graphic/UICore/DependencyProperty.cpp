#include "DependencyProperty.h"

using namespace System::UI;


PropertyChangedEventArgs::PropertyChangedEventArgs(const SharedVoid& oldValue, const SharedVoid& newValue) : oldValue(oldValue), newValue(newValue)
{

}

const SharedVoid& PropertyChangedEventArgs::OldValue() const
{
	return oldValue;
}

const SharedVoid& PropertyChangedEventArgs::NewValue() const
{
	return newValue;
}




FrameworkPropertyMetadata::FrameworkPropertyMetadata(SharedVoid defaultValue,
FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback) :
defaultValue(defaultValue),
changeCallback(changeCallback),
propertyEqualityCallback(propertyEqualityCallback),
propertyPreCoerceCallback(propertyPreCoerceCallback),
frameworkPropertyMetadataOptionsFlags(frameworkPropertyMetadataOptionsFlags),
isDependantObjectType(false)
{

}

SharedVoid FrameworkPropertyMetadata::DefaultValue() const { return defaultValue; }
PropertyChangedCallback FrameworkPropertyMetadata::ChangeCallback() const { return changeCallback; }
PropertyEqualityCallback FrameworkPropertyMetadata::EqualityCallback() const { return propertyEqualityCallback; }
PropertyPreCoerceCallback FrameworkPropertyMetadata::PreCoerceCallback() const { return propertyPreCoerceCallback; }
FrameworkPropertyMetadataOptions FrameworkPropertyMetadata::FrameworkPropertyMetadataOptionsFlags() const { return frameworkPropertyMetadataOptionsFlags; }

bool FrameworkPropertyMetadata::IsDepenantObjectType() const
{
	return isDependantObjectType;
}

bool FrameworkPropertyMetadata::Inherits() const { return Enums::Contains(frameworkPropertyMetadataOptionsFlags, FrameworkPropertyMetadataOptions::Inherits); }

IDependencyProperty::IDependencyProperty(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata, bool readOnly):name(name), isofOwnerType(ofOwnerTypeCallback), propertyMetadata(propertyMetadata), isReadOnly(readOnly)
{

}

bool IDependencyProperty::GetIsReadOnly() const
{
	return isReadOnly;
}

const std::wstring& IDependencyProperty::GetName() const
{
	return name;
}

FrameworkPropertyMetadataPtr IDependencyProperty::GetPropertyMetadata() const
{
	return propertyMetadata;
}

DependencyProperty::DependencyProperty(fun__ustatic_ctor_impl ustatic_ptr):impl(nullptr), fun_ustaticdp(ustatic_ptr)
{

}

void DependencyProperty::Register(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata)
{
	impl = std::make_shared<IDependencyProperty>(name, ofOwnerTypeCallback, propertyMetadata, false);
}

void DependencyProperty::RegisterReadOnly(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata)
{
	impl = std::make_shared<IDependencyProperty>(name, ofOwnerTypeCallback, propertyMetadata, true);
}

std::shared_ptr<IDependencyProperty> DependencyProperty::operator ->()
{
	if (fun_ustaticdp) fun_ustaticdp();
	return impl;
}