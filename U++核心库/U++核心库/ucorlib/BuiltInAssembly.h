#pragma once

#include "Assembly.h"
#include "NameSpace.h"
namespace System
{
	class Type;

	class Factory;

	class BuiltInAssemblyFactory
	{
	private:
		friend class Factory;
		static const Assembly* System;
		BuiltInAssemblyFactory();
	public:
		static BuiltInAssemblyFactory& GetBuiltIn();
		static Type* Regisiter(Type* type);
	};
}
