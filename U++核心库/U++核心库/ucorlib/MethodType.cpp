#include "MethodType.h"
#include "Formatter.h"
#include <string>

using namespace System;


MethodType::MethodType() : Type(TypeTag::Method), returnType(nullptr), paramTypes(nullptr), defaultParams(nullptr), methodClass(nullptr)
{
	
}

bool MethodType::Invoke(Any* result, Object* obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool MethodType::Invoke(Any* result, const Object* obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool MethodType::Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool MethodType::Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool MethodType::Invoke(Any* result, nullptr_t obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	return Invoke(result, (Object*)nullptr, parameters, nothrow);
}

const wchar_t* MethodType::GetName() const
{
	if (!description)
	{
		std::wstring result = std::wstring();
		if (paramTypes == nullptr)
		{
			switch (kind)
			{
			case MethodKinds::Normal:
				result += returnType != nullptr ? returnType->Name : L"???";
				result += L"()";
				break;
			case MethodKinds::Const:
				result += returnType != nullptr ? returnType->Name : L"???";
				result += L"() const";
				break;
			case MethodKinds::Static:
				result += L"static ";
				result += returnType != nullptr ? returnType->Name : L"???";
				result += L"()";
				break;
			}
		}
		else
		{
			switch (kind)
			{
				case MethodKinds::Normal:
				{
					result += returnType != nullptr ? returnType->Name : L"???";
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
					break;
				case MethodKinds::Const:
				{
					result += returnType != nullptr ? returnType->Name : L"???";
					result += '(';
					int index = 0;
					int count = paramTypes->size();
					for (auto iter : *paramTypes)
					{
						result += iter->Name;
						if (index < count - 1) result += ',';
						index++;
					}
					result += L") const";
					break;
				}
				case MethodKinds::Static:
				{
					result += L"static ";
					result += returnType != nullptr ? returnType->Name : L"???";
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
				break;
			}
		}
		try
		{
			MethodType* mt = const_cast<MethodType*>(this);
			mt->description = result;
		}
		catch (...)
		{
			return nullptr;
		}
	}
	return (*description).c_str();
}

MethodType::~MethodType()
{
	if (paramTypes)
	{
		delete paramTypes;
		paramTypes = nullptr;
	}
}

_type MethodType::GetClassType() const
{
	return methodClass;
}

_type MethodType::GetReturnType() const
{
	return returnType;
}

MethodKinds MethodType::GetKind() const
{
	return kind;
}


const TypeCollection* MethodType::GetParameterTypes() const
{
	return paramTypes;
}

unsigned int MethodType::GetNumberOfParameters() const
{
	return paramTypes == NULL ? 0 : paramTypes->size();
}

bool MethodType::operator ==(const MethodType& type) const
{
	if (type.methodClass == methodClass)
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

bool MethodType::operator !=(const MethodType& type) const
{
	if (type.methodClass == methodClass)
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
