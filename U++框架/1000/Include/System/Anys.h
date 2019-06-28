#pragma once
#include "Config.h"
#include "Any.h"
#include "VariadicAny.h"
#include "DefaultVariadicAny.h"
#include "ConditionalAnyVisitor.h"
#include "OptionalVariadicAny.h"
#include "RealVariadicAny.h"
#include "VariadicAnyVisitor.h"

namespace System
{
	template<class T>
	struct is_base_of_Any_impl
	{
		static const bool value = false;
	};

	template<>
	struct is_base_of_Any_impl<Any>
	{
		static const bool value = true;
	};

	template<>
	struct is_base_of_Any_impl<VariadicAny>
	{
		static const bool value = true;
	};

	template<>
	struct is_base_of_Any_impl<RealVariadicAny>
	{
		static const bool value = true;
	};

	template<>
	struct is_base_of_Any_impl<DefaultVariadicAny>
	{
		static const bool value = true;
	};

	template<>
	struct is_base_of_Any_impl<OptionalVariadicAny>
	{
		static const bool value = true;
	};


	template<typename T>
	struct is_base_of_Any
	{
		static const bool value = is_base_of_Any_impl<typename deduce<typename remove_ptrbase<T>::type>::type>::value;
	};

}