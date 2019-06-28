#pragma once
#include "NameSpace.h"
#include "Type.h"
#include <algorithm>
namespace System
{
	class TypeComparer
	{
	private:
		const wchar_t* name;
	public:
		explicit TypeComparer(_type cls)
		{
			name = cls != NULL ? cls->Name : 0;
		}
		bool operator() (_type cls)
		{
			return name == nullptr || cls == nullptr ? false : wcscmp(cls->Name, name) == 0;
		}
	};

	class TypeCaches_impl
	{
	protected:
		friend class TypeCaches;
		std::vector<_type> Registry;

		TypeCaches_impl()
		{
		}

		~TypeCaches_impl()
		{
			Registry.clear();
		}

		NameSpaceError Insert(_type type)
		{
			if (!type)return NameSpaceError::EmptyType;
			if (!type->Name) return NameSpaceError::InvalidType;
			if (type->IsBuiltin)return NameSpaceError::InvalidType;
			auto iter = std::find_if(Registry.begin(), Registry.end(), TypeComparer(type));
			if (iter != Registry.end()) return NameSpaceError::Existed;
			Registry.push_back(type);
			return NameSpaceError::Success;
		}

		void Remove(_type type)
		{
			if (!type) return;
			auto iter = std::find_if(Registry.begin(), Registry.end(), TypeComparer(type));
			if (iter != Registry.end()) Registry.erase(iter);
		}

		size_t GetItemSize() const
		{
			return Registry.size();
		}

		_type GetItem(size_t index) const
		{
			if (index >= 0 && index < Registry.size())
			{
				return Registry.at(index);
			}
			return nullptr;
		}
	};
}