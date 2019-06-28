#include "ITranslatable.h"
#include "BuiltInAssembly.h"
using namespace System;


SimpleTranslationReference::SimpleTranslationReference(const InertiaString& source, bool isEnabled) :_source(source), _isEnabled(isEnabled)
{

}

InertiaString SimpleTranslationReference::GetSource() const
{
	return _source;
}

bool SimpleTranslationReference::GetEnabled() const
{
	return _isEnabled;
}

void SimpleTranslationReference::SetEnabled(bool isEnabled)
{
	_isEnabled = isEnabled;
}

SimpleTranslationReference1::SimpleTranslationReference1(const InertiaString& source, const InertiaString& translationContext, bool isEnabled, bool isDependent) :_source(source), _translationContext(translationContext), _isEnabled(isEnabled), _isDependent(isDependent)
{

}

InertiaString SimpleTranslationReference1::GetSource() const
{
	return _source;
}

bool SimpleTranslationReference1::GetEnabled() const
{
	return _isEnabled;
}

void SimpleTranslationReference1::SetEnabled(bool isEnabled)
{
	_isEnabled = isEnabled;
}

InertiaString SimpleTranslationReference1::GetTranslationContext() const
{
	return _translationContext;
}

bool SimpleTranslationReference1::GetIsDependent() const
{
	return _isDependent;
}

__uregister_class(ITranslatable, ClassFlags::Normal)
{
	type.AddMethod(L"GetSource", MethodFlags::Virtual, MethodTypeAdapter<0, ITranslatable, InertiaString>::Create(&ITranslatable::GetSource));
	type.AddMethod(L"GetEnabled", MethodFlags::Virtual, MethodTypeAdapter<0, ITranslatable, bool>::Create(&ITranslatable::GetEnabled));
	type.AddMethod(L"SetEnabled", MethodFlags::Virtual, MethodTypeAdapter<1, ITranslatable>::Create(&ITranslatable::SetEnabled));

	type.AddProperty(L"Source", PropertyFlags::Public, PropertyTypeOf(&ITranslatable::GetSource));
	type.AddProperty(L"Enabled", PropertyFlags::Public, PropertyTypeOf(&ITranslatable::GetEnabled, &ITranslatable::SetEnabled));

	BuiltInAssemblyFactory::Regisiter(&type);
}

__uregister_class(ITranslatable1, ClassFlags::Normal)
{
	type.AddBaseClass<ITranslatable>();

	type.AddMethod(L"GetSource", MethodFlags::Virtual, MethodTypeAdapter<0, ITranslatable1, InertiaString>::Create(&ITranslatable1::GetSource));
	type.AddMethod(L"GetEnabled", MethodFlags::Virtual, MethodTypeAdapter<0, ITranslatable1, bool>::Create(&ITranslatable1::GetEnabled));
	type.AddMethod(L"SetEnabled", MethodFlags::Virtual, MethodTypeAdapter<1, ITranslatable1>::Create(&ITranslatable1::SetEnabled));
	type.AddMethod(L"GetIsDependent", MethodFlags::Virtual, MethodTypeAdapter<0, ITranslatable1, bool>::Create(&ITranslatable1::GetIsDependent));
	type.AddMethod(L"GetTranslationContext", MethodFlags::Virtual, MethodTypeAdapter<0, ITranslatable1, InertiaString>::Create(&ITranslatable1::GetTranslationContext));

	type.AddProperty(L"Source", PropertyFlags::Public, PropertyTypeOf(&ITranslatable1::GetSource));
	type.AddProperty(L"Enabled", PropertyFlags::Public, PropertyTypeOf(&ITranslatable1::GetEnabled, &ITranslatable1::SetEnabled));
	type.AddProperty(L"IsDependent", PropertyFlags::Public, PropertyTypeOf(&ITranslatable1::GetIsDependent));
	type.AddProperty(L"TranslationContext", PropertyFlags::Public, PropertyTypeOf(&ITranslatable1::GetTranslationContext));

	BuiltInAssemblyFactory::Regisiter(&type);
}

__inline static Object* Factory_SimpleTranslationReference_0(const System::InertiaString& source, bool isEnabled) { return new SimpleTranslationReference(source, isEnabled); }

__uregister_class(SimpleTranslationReference, ClassFlags::Normal)
{
	type.AddBaseClass<ITranslatable>();

	type.AddConstructor(L"SimpleTranslationReference", ConstructorFlags::Default, ConstructorTypeAdapter<2, SimpleTranslationReference>::Create(&Factory_SimpleTranslationReference_0));

	type.AddMethod(L"GetSource", MethodFlags::Virtual, MethodTypeAdapter<0, SimpleTranslationReference, InertiaString>::Create(&SimpleTranslationReference::GetSource));
	type.AddMethod(L"GetEnabled", MethodFlags::Virtual, MethodTypeAdapter<0, SimpleTranslationReference, bool>::Create(&SimpleTranslationReference::GetEnabled));
	type.AddMethod(L"SetEnabled", MethodFlags::Virtual, MethodTypeAdapter<1, SimpleTranslationReference>::Create(&SimpleTranslationReference::SetEnabled));

	type.AddProperty(L"Source", PropertyFlags::Public, PropertyTypeOf(&SimpleTranslationReference::GetSource));
	type.AddProperty(L"Enabled", PropertyFlags::Public, PropertyTypeOf(&SimpleTranslationReference::GetEnabled, &SimpleTranslationReference::SetEnabled));

	BuiltInAssemblyFactory::Regisiter(&type);
}


__inline static Object* Factory_SimpleTranslationReference1_0(const InertiaString& source, const InertiaString& translationContext, bool isEnabled, bool isDependent) { return new SimpleTranslationReference1(source, translationContext, isEnabled, isDependent); }

__uregister_class(SimpleTranslationReference1, ClassFlags::Normal)
{
	type.AddBaseClass<ITranslatable1>();

	type.AddConstructor(L"SimpleTranslationReference1", ConstructorFlags::Default, ConstructorTypeAdapter<4, SimpleTranslationReference1>::Create(&Factory_SimpleTranslationReference1_0));

	type.AddMethod(L"GetSource", MethodFlags::Virtual, MethodTypeAdapter<0, SimpleTranslationReference1, InertiaString>::Create(&SimpleTranslationReference1::GetSource));
	type.AddMethod(L"GetEnabled", MethodFlags::Virtual, MethodTypeAdapter<0, SimpleTranslationReference1, bool>::Create(&SimpleTranslationReference1::GetEnabled));
	type.AddMethod(L"SetEnabled", MethodFlags::Virtual, MethodTypeAdapter<1, SimpleTranslationReference1>::Create(&SimpleTranslationReference1::SetEnabled));
	type.AddMethod(L"GetIsDependent", MethodFlags::Virtual, MethodTypeAdapter<0, SimpleTranslationReference1, bool>::Create(&SimpleTranslationReference1::GetIsDependent));
	type.AddMethod(L"GetTranslationContext", MethodFlags::Virtual, MethodTypeAdapter<0, SimpleTranslationReference1, InertiaString>::Create(&SimpleTranslationReference1::GetTranslationContext));

	type.AddProperty(L"Source", PropertyFlags::Public, PropertyTypeOf(&SimpleTranslationReference1::GetSource));
	type.AddProperty(L"Enabled", PropertyFlags::Public, PropertyTypeOf(&SimpleTranslationReference1::GetEnabled, &SimpleTranslationReference1::SetEnabled));
	type.AddProperty(L"IsDependent", PropertyFlags::Public, PropertyTypeOf(&SimpleTranslationReference1::GetIsDependent));
	type.AddProperty(L"TranslationContext", PropertyFlags::Public, PropertyTypeOf(&SimpleTranslationReference1::GetTranslationContext));

	BuiltInAssemblyFactory::Regisiter(&type);
}



