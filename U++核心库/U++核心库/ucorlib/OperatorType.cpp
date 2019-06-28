#include "OperatorType.h"
#include <string>

using namespace System;



OperatorType::OperatorType() : Type(TypeTag::Operator)
{

}

const wchar_t* OperatorType::GetName() const
{
	if (!description)
	{
		std::wstring result;
		result += returnType->Name;
		result += L" operator";
		switch (op)
		{
		case Operators::Addition:
			result += '+';
			break;		
		case Operators::AdditionAssignment:
			result += L"+=";
			break;
		case Operators::AddressOf:
			result += '&';
			break;
		case Operators::Assignment:
			result += '=';
			break;		
		case Operators::BitwiseAND:
			result += '&';
			break;	
		case Operators::BitwiseANDAssignment:
			result += L"&=";
			break;		
		case Operators::BitwiseInclusiveOR:
			result += '|';
			break;
		case Operators::BitwiseInclusiveORAssignment:
			result += L"|=";
			break;
		case Operators::Comma:
			result += ',';
			break;
		case Operators::ConversionOperators://
			break;
		case Operators::Decrement:
			result += L"--";
			break;
		case Operators::DivisionAssignment:
			result += L"\\=";
			break;
		case Operators::Division:
			result += '\\';
			break;
		case Operators::Equality:
			result += L"==";
			break;
		case Operators::ExclusiveOR:
			result += '^';
			break;
		case Operators::ExclusiveORAssignment:
			result += L"^=";
			break;
		case Operators::Function://
			break;
		case Operators::GreaterThan:
			result += '>';
			break;
		case Operators::GreaterThanOrEqualTo:
			result += L">=";
			break;
		case Operators::Increment:
			result += L"++";
			break;
		case Operators::Inequality:
			result += L"!=";
			break;
		case Operators::LeftShift:
			result += L"<<";
			break;
		case Operators::LeftShiftAssignment:
			result += L"<<=";
			break;
		case Operators::LessThan:
			result +='<';
			break;
		case Operators::LessThanOrEqualTo:
			result += L"<=";
			break;
		case Operators::LogicalAND:
			result += L"&&";
			break;
		case Operators::LogicalNOT:
			result += L"!";
			break;
		case Operators::LogicalOR:
			result += L"||";
			break;
		case Operators::MemberSelection:
			result += L".";
			break;
		case Operators::Modulus:
			result += L"%";
			break;
		case Operators::ModulusAssignment:
			result += L"%=";
			break;
		case Operators::Multiplication:
			result += '*';
			break;
		case Operators::MultiplicationAssignment:
			result += L"*=";
			break;
		case Operators::OnesComplement:  
			result += L"~";
			break;
		case Operators::PointerDereference: 
			result += L"*";
			break;
		case Operators::PointerToMemberSelection: 
			result += L"->";
			break;
		case Operators::RightShift:
			result += L">>";
			break;
		case Operators::RightShiftAssignment:
			result += L">>=";
			break;
		case Operators::Subscripting:
			result += L"[]";
			break;
		case Operators::Subtraction:
			result += '-';
			break;
		case Operators::SubtractionAssignment:
			result += L"-=";
			break;
		case Operators::UnaryNegation: 
			result += L"-";
			break;
		case Operators::UnaryPlus:
			result += L"+";
			break;
		case Operators::Unknown://
			break;
		}
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
			OperatorType* mt = const_cast<OperatorType*>(this);
			mt->description = result;
		}
		catch (...)
		{
			return nullptr;
		}
	}
	return (*description).c_str();

}

OperatorType::~OperatorType()
{
	if (paramTypes)
	{
		delete paramTypes;
		paramTypes = nullptr;
	}
}

bool OperatorType::Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool OperatorType::Invoke(Any* result, const Object* obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool OperatorType::Invoke(Any* result, volatile Object* obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool OperatorType::Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (nothrow) return false; else throw std::exception("not implemented");
}

bool OperatorType::Invoke(Any* result, nullptr_t obj, const RealVariadicAny& parameters, bool nothrow) const
{
	return Invoke(result, (Object*)nullptr, parameters, nothrow);
}

_type OperatorType::GetClassType() const
{
	return operatorClass;
}

_type OperatorType::GetReturnType() const
{
	return returnType;
}

const TypeCollection* OperatorType::GetParameterTypes() const
{
	return paramTypes;
}

unsigned int OperatorType::GetNumberOfParameters() const
{
	return  paramTypes == NULL ? 0 : paramTypes->size();
}

bool OperatorType::operator ==(const OperatorType& type) const
{
	if (type.operatorClass == operatorClass)
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

bool OperatorType::operator !=(const OperatorType& type) const
{
	if (type.operatorClass == operatorClass)
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