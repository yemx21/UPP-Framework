#include "FieldType.h"
#include <string>

using namespace System;



FieldType::FieldType() : System::Type(TypeTag::Field)
{

}

const wchar_t* FieldType::GetName() const
{
	if (!description)
	{
		std::wstring result;
		if (fieldType == nullptr)
		{
			switch (kind)
			{
			case FieldKinds::Read:
				result += L"readonly ";
				result += fieldType->Name;
				break;
			case FieldKinds::Write:
				result += L"writeonly ";
				result += fieldType->Name;
				break;
			case FieldKinds::ReadWrite:
				result += fieldType->Name;
				break;			
			case FieldKinds::StaticRead:
				result += L"static readonly ";
				result += fieldType->Name;
				break;
			case FieldKinds::StaticWrite:
				result += L"static writeonly ";
				result += fieldType->Name;
				break;
			case FieldKinds::StaticReadWrite:
				result += L"static ";
				result += fieldType->Name;
				break;
			}
		}
		try
		{
			FieldType* mt = const_cast<FieldType*>(this);
			mt->description = result;
		}
		catch (...)
		{
			return nullptr;
		}
	}
	return (*description).c_str();

}

FieldPolicy FieldType::GetPolicy() const
{
	return policy;
}

_type FieldType::GetClassType() const
{
	return ownerClass;
}

_type FieldType::GetFieldType() const
{
	return fieldType;
}

bool FieldType::operator ==(const FieldType& type) const
{
	if (type.ownerClass == ownerClass)
	{
		if (type.fieldType == fieldType)
		{
			return type.policy == policy;
		}
	}
	return false;
}

bool FieldType::operator !=(const FieldType& type) const
{
	if (type.ownerClass == ownerClass)
	{
		if (type.fieldType == fieldType)
		{
			return type.policy != policy;
		}
	}
	return true;
}