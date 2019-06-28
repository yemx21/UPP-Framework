#include "ConstructorType.h"
#include <string>

using namespace System;



ConstructorType::ConstructorType() : Type(TypeTag::Constructor)
{

}

ConstructorType::~ConstructorType()
{
	if (paramTypes)
	{
		delete paramTypes;
		paramTypes = nullptr;
	}
}

const wchar_t* ConstructorType::GetName() const
{
	if (!description)
	{
		std::wstring result;
		result += returnType != nullptr ? returnType->Name : L"???";
		if (paramTypes == nullptr) 
			result += L"()";
		else
		{
			result += '(';
			int index = 0;
			int count = paramTypes->size();
			for (auto iter : *paramTypes)
			{
				result += iter->Name;
				if (index < count - 1) result += ',';
				index++;
			}
			result += ')';
		}
		try
		{
			ConstructorType* mt = const_cast<ConstructorType*>(this);
			mt->description = result;
		}
		catch (...)
		{
			return nullptr;
		}
	}
	return (*description).c_str();
}

_type ConstructorType::GetClassType() const
{
	return constructorClass;
}

_type ConstructorType::GetReturnType() const
{
	return returnType;
}

const TypeCollection* ConstructorType::GetParameterTypes() const
{
	return paramTypes;
}

unsigned int ConstructorType::GetNumberOfParameters() const
{
	return  paramTypes == NULL ? 0 : paramTypes->size();
}

bool ConstructorType::operator ==(const ConstructorType& type) const
{
	if (type.constructorClass == constructorClass)
	{
		if (type.returnType == returnType)
		{
			if (type.paramTypes && paramTypes)
			{
				if (paramTypes->size() == type.paramTypes->size())
				{
					for (size_t i = 0; i < paramTypes->size(); i++)
					{
						if ((*type.paramTypes)[i] != (*type.paramTypes)[i]) return false;
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool ConstructorType::operator !=(const ConstructorType& type) const
{
	if (type.constructorClass == constructorClass)
	{
		if (type.returnType == returnType)
		{
			if (type.paramTypes && paramTypes)
			{
				if (paramTypes->size() == type.paramTypes->size())
				{
					for (size_t i = 0; i < paramTypes->size(); i++)
					{
						if ((*type.paramTypes)[i] != (*type.paramTypes)[i]) return true;
					}
				}
				return false;
			}
		}
	}
	return true;
}