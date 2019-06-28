#pragma once
//#include <Windows.h>
#include "Type.h"
#include "OSHelper.h"
#include "InertiaString.h"
// #include "CultureInfo.h"

namespace System
{
	class Any;
	class VariadicAny;
	class DefaultVariadicAny;
	class OptionalVariadicAny;
	class RealVariadicAny;

	template<typename __T>
	class TypeHelper
	{
	public:
		static _type GetType();
	};

	template<>
	_inline _type TypeHelper<void>::GetType() { return Type::voidType; }

	template<>
	_inline _type TypeHelper<bool>::GetType() { return Type::boolType; }

	template<>
	_inline _type TypeHelper<char>::GetType() { return Type::charType; }

	template<>
	_inline _type TypeHelper<unsigned char>::GetType() { return Type::ucharType; }

	template<>
	_inline _type TypeHelper<wchar_t>::GetType() { return Type::wcharType; }

	template<>
	_inline _type TypeHelper<short>::GetType() { return Type::shortType; }

	template<>
	_inline _type TypeHelper<unsigned short>::GetType() { return Type::UShortType; }

	template<>
	_inline _type TypeHelper<int>::GetType() { return Type::intType; }

	template<>
	_inline _type TypeHelper<unsigned int>::GetType() { return Type::UIntType; }

	template<>
	_inline _type TypeHelper<Int24_t>::GetType() { return Type::Int24Type; }

	template<>
	_inline _type TypeHelper<UInt24_t>::GetType() { return Type::UInt24Type; }

	template<>
	_inline _type TypeHelper<long>::GetType() { return Type::longType; }

	template<>
	_inline _type TypeHelper<unsigned long>::GetType() { return Type::ULongType; }

	template<>
	_inline _type TypeHelper<long long>::GetType() { return Type::longlongType; }

	template<>
	_inline _type TypeHelper<unsigned long long>::GetType() { return Type::ULonglongType; }

	template<>
	_inline _type TypeHelper<Int128_t>::GetType() { return Type::Int128Type; }

	template<>
	_inline _type TypeHelper<UInt128_t>::GetType() { return Type::UInt128Type; }

	template<>
	_inline _type TypeHelper<float>::GetType() { return Type::floatType; }

	template<>
	_inline _type TypeHelper<double>::GetType() { return Type::doubleType; }

	template<>
	_inline _type TypeHelper<long double>::GetType() { return Type::longdoubleType; }

	template<>
	_inline _type TypeHelper<System::Object>::GetType() { return Type::objectType; }

	template<>
	_inline _type TypeHelper<System::Any>::GetType() { return Type::anyType; }

	template<>
	_inline _type TypeHelper<System::VariadicAny>::GetType() { return Type::variadicAnyType; }

	template<>
	_inline _type TypeHelper<System::DefaultVariadicAny>::GetType() { return Type::defaultVariadicAnyType; }

	template<>
	_inline _type TypeHelper<System::RealVariadicAny>::GetType() { return Type::realVariadicAnyType; }

	template<>
	_inline _type TypeHelper<System::OptionalVariadicAny>::GetType() { return Type::optionalVariadicAnyType; }

	template<>
	_inline _type TypeHelper<System::safe_void_ptr>::GetType() { return Type::safeVoidPtrType; }

	template<>
	_inline _type TypeHelper<System::InertiaString>::GetType() { return Type::inertiaStringType; }

	template<>
	RUNTIME_API _type TypeHelper<std::string>::GetType();

	template<>
	RUNTIME_API _type TypeHelper<std::wstring>::GetType();

	class CultureInfo;

	template<>
	RUNTIME_API _type TypeHelper<System::CultureInfo>::GetType();

	template<typename T>
	struct is_std_type
	{
		static const bool value = std::is_same<T, std::string >::value || std::is_same<T, std::wstring>::value;
	};

	template<typename T>
	struct is_builtin_type
	{
		static const bool value = std::is_same<T, System::CultureInfo>::value;
	};

	template<typename T>
	struct is_TypeHelper_supported
	{
		static const bool value = std::is_same<T, void>::value || std::is_same<T, bool>::value || std::is_same<T, char>::value || std::is_same<T, unsigned char>::value || std::is_same<T, wchar_t>::value ||
			std::is_same<T, short>::value || std::is_same<T, unsigned short>::value || std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, Int24>::value || std::is_same<T, UInt24>::value ||
			std::is_same<T, long>::value || std::is_same<T, unsigned long>::value || std::is_same<T, long long>::value || std::is_same<T, unsigned long long>::value || std::is_same<T, Int128>::value || std::is_same<T, UInt128>::value ||
			std::is_same<T, float>::value || std::is_same<T, double>::value || std::is_same<T, long double>::value || std::is_same<T, Any>::value || std::is_same<T, VariadicAny>::value || std::is_same<T, DefaultVariadicAny>::value ||
			std::is_same<T, RealVariadicAny>::value || std::is_same<T, OptionalVariadicAny>::value || std::is_same<T, safe_void_ptr>::value || std::is_same<T, InertiaString>::value || std::is_base_of<Object, T>::value || is_std_type<T>::value || is_builtin_type<T>::value;
	};

}
