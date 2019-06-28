#include "MemoryPtrType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"

using namespace System;


namespace System
{
	class MemoryPtrType_impl
	{
	private:
		static std::mutex locker;
		static std::unordered_map<size_t, MemoryPtrType*> caches;
		static std::hash<std::wstring> hash_fn;
		static bool CreateName(std::wstring& name, MemoryPtrTypes ctype, _type htype)
		{
			switch (ctype)
			{
			case MemoryPtrTypes::SharedPtr:
			default:
			{
				if (!htype) return false;
				name = L"std::shared_ptr<";
				name.append(htype->Name);
				name.push_back('>');
			}
				break;
			case MemoryPtrTypes::WeakPtr:
			{
				if (!htype) return false;
				name = L"std::weak_ptr<";
				name.append(htype->Name);
				name.push_back('>');
			}
				break;
			case MemoryPtrTypes::UniquePtr:
			{
				if (!htype) return false;
				name = L"std::unique_ptr<";
				name.append(htype->Name);
				name.push_back('>');
			}
				break;
#if !_HAS_CXX17
			case MemoryPtrTypes::AutoPtr:
			{
				if (!htype) return false;
				name = L"std::auto_ptr<";
				name.append(htype->Name);
				name.push_back('>');
			}
				break;
#endif
			case MemoryPtrTypes::Unknown:
				break;
			}
			return true;
		}
	public:
		_type holdType;

		InertiaString name;
		MemoryPtrTypes cttype;

		static MemoryPtrType* Create(_type htype, MemoryPtrTypes ctype)
		{
			std::wstring str;
			if (CreateName(str, ctype, htype))
			{
				size_t hash = hash_fn(str);
				std::lock_guard<std::mutex> lock(locker);
				auto iter = caches.find(hash);
				if (iter != caches.end())
					return iter->second;
				else
				{
					MemoryPtrType* type = new MemoryPtrType(htype, ctype);
					type->_hashCode = hash;
					type->impl->name = InertiaString(str.c_str());
					caches.insert(std::make_pair(hash, type));
					return type;
				}
			}
			return false;
		}

		MemoryPtrType_impl(_type htype, MemoryPtrTypes ctype) : holdType(htype), cttype(ctype)
		{

		};
	};

	std::mutex MemoryPtrType_impl::locker;
	std::unordered_map<size_t, MemoryPtrType*> MemoryPtrType_impl::caches;
	std::hash<std::wstring> MemoryPtrType_impl::hash_fn;
}

const MemoryPtrType* MemoryPtrType::Create(_type holdType, MemoryPtrTypes ptype)
{
	return MemoryPtrType_impl::Create(holdType, ptype);
}

MemoryPtrType::MemoryPtrType(_type holdType, MemoryPtrTypes ptype) :Type(TypeTag::SmartPtr)
{
	impl = new MemoryPtrType_impl(holdType, ptype);
}

MemoryPtrType::~MemoryPtrType()
{
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

MemoryPtrTypes MemoryPtrType::GetPtrType() const
{
	return  impl != nullptr ? impl->cttype : MemoryPtrTypes::Unknown;
}

const wchar_t* MemoryPtrType::GetName() const
{
	return impl != nullptr ? impl->name->c_str() : nullptr;
}

_type MemoryPtrType::GetHoldType() const
{
	return  impl != nullptr ? impl->holdType : nullptr;
}
