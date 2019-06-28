#include "OptionalType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"
using namespace System;



namespace System
{
	class OptionalType_impl
	{
	private:
		static std::mutex locker;
		static std::unordered_map<size_t, OptionalType*> caches;
		static std::hash<std::wstring> hash_fn;
		static bool CreateName(std::wstring& name, _type type)
		{	
			if (!type) return false;
			name = L"Optional<";
			name += type->Name;
			name.push_back('>');			
			return true;
		}
	public:
		_type itype;
		InertiaString name;
		static OptionalType* Create(_type type)
		{
			std::wstring str;
			if (CreateName(str, type))
			{
				size_t hash = hash_fn(str);
				std::lock_guard<std::mutex> lock(locker);
				auto iter = caches.find(hash);
				if (iter != caches.end())
					return iter->second;
				else
				{
					OptionalType* otype = new OptionalType(type);
					otype->_hashCode = hash;
					otype->impl->name = InertiaString(str.c_str());
					caches.insert(std::make_pair(hash, otype));
					return otype;
				}
			}
			return false;
		}

		OptionalType_impl(_type type) : itype(type)
		{

		};

		static void Remove(OptionalType* at)
		{
			std::lock_guard<std::mutex> lock(locker);
			OptionalType* att = at;
			auto iter = caches.find(*at->_hashCode);
			if (iter != caches.end())
			{
				caches.erase(iter);
			}
		}
	};

	std::mutex OptionalType_impl::locker;
	std::unordered_map<size_t, OptionalType*> OptionalType_impl::caches;
	std::hash<std::wstring> OptionalType_impl::hash_fn;
}

const OptionalType* OptionalType::Create(_type type)
{
	return OptionalType_impl::Create(type);

}

OptionalType::~OptionalType()
{
	if (impl)
	{
		OptionalType_impl::Remove(this);
		delete impl;
		impl = nullptr;
	}
}

const wchar_t* OptionalType::GetName() const
{
	return impl != nullptr ? impl->name.get().c_str() : nullptr;
}

_type OptionalType::GetType() const
{
	return  impl != nullptr ? impl->itype : nullptr;
}

OptionalType::OptionalType(_type type) : System::Type(TypeTag::Optional)
{
	impl = new OptionalType_impl(type);
}

