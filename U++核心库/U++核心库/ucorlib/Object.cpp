#include "Object.h"
#include "ClassInfo.h"
#include "TypeList.h"
#include "Reflection.h"
using namespace System;

_type Object::Meta = nullptr;

Object::~Object()
{

}


std::wstring Object::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return std::wstring();
}


_type Object::GetType() const
{
	return Meta;
}


bool Object::Is(_type type) const
{
	_type thisType = GetType();
	return thisType->IsType(type);
}



bool Object::Equals(const Object* ref) const
{
	return ref == this;
}

uhash Object::GetHashCode() const
{
	return static_cast<uhash>((intptr_t)this);
}