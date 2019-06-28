#pragma once
#include "Common.h"
#include "Version.h"
//#include <objbase.h>
#include "Type.h"
#include "Assembly.h"
namespace System
{

	class NameSpace;
	class BuiltInAssemblyFactory;
	class NameSpaceData;


	class NameSpace_impl
	{
	protected:
		typedef std::vector<NameSpace*>::const_iterator base_iterator;
		friend class Assembly;
		friend class NameSpace;
		friend class NameSpaceData;
		friend class NameSpaceEnumerator_impl;
		friend class BuiltInAssemblyFactory;
		const Assembly* _Assembly;
		const NameSpace* _Parent;
		InertiaString Name;
		std::vector<_type> Registry;
		std::vector<NameSpace*> Children;

		NameSpace_impl(const wchar_t* name) : Name(name), _Assembly(nullptr), _Parent(nullptr)
		{
		}

		~NameSpace_impl()
		{
			for (auto& i : Registry)
			{
				SAFE_DELETE(i);
			}
			Registry.clear();
			for (auto& c : Children)
			{
				SAFE_DELETE(c);
			}
			Children.clear();
		}

		static Type* InternalRegisiter(NameSpace* space, Type* type);


		bool operator ==(const NameSpace_impl& space) const
		{
			if (_Assembly && space._Assembly)
			{
				if (_Assembly->operator!=(*space._Assembly)) return false;
			}
			else
				return false;
			return Name == space.Name;
		}

		bool operator !=(const NameSpace_impl& space) const
		{
			if (_Assembly && space._Assembly)
			{
				if (_Assembly->operator==(*space._Assembly)) return false;
			}
			else
				return true;
			return Name != space.Name;
		}

	};
}