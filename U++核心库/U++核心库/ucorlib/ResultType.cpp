#include "ResultType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"
using namespace System;



namespace System
{
	class ResultType_impl
	{
	private:
		static std::mutex locker;
		static std::unordered_map<size_t, ResultType*> caches;
		static std::hash<std::wstring> hash_fn;
		static bool CreateName(std::wstring& name, _type type1, _type type2, _type type3, _type stype, ResultTypes ctype)
		{
			switch (ctype)
			{
			case ResultTypes::One:
			default:
			{
				if (!type1) return false;
				name = L"Result<";
				name += type1->Name;
				name.push_back('>');
			}
				break;			
			case ResultTypes::Two:
			{
						if (!type1) return false;
						if (!stype) return false;
						name = L"Result<";
						name += type1->Name;
						name.push_back(',');
						name += stype->Name;
						name.push_back('>');
			}
				break;
			case ResultTypes::Three:
			{
				if (!type1) return false;
				if (!type2) return false; 
				if (!stype) return false;
				name = L"Result<";
				name += type1->Name;
				name.push_back(',');
				name += type2->Name; 
				name.push_back(',');
				name += stype->Name;
				name.push_back('>');
			}
				break;
			case ResultTypes::Four:
			{
				if (!type1) return false;
				if (!type2) return false;
				if (!type3) return false;
				if (!stype) return false;
				name = L"Result<";
				name += type1->Name;
				name.push_back(',');
				name += type2->Name;
				name.push_back(',');									   
				name += type3->Name;
				name.push_back(',');
				name += stype->Name;
				name.push_back('>');
			}
				break;
			case ResultTypes::Unknown:
			{
				return false;
			}
				break;
			}
			return true;
		}
	public:
		_type itype1;
		_type itype2;
		_type itype3;
		_type istype;

		InertiaString name;
		ResultTypes cttype;

		static ResultType* Create(_type type1, _type type2, _type type3, _type stype, ResultTypes ctype)
		{
			std::wstring str;
			if (CreateName(str, type1, type2, type3, stype, ctype))
			{
				size_t hash = hash_fn(str);
				std::lock_guard<std::mutex> lock(locker);
				auto iter = caches.find(hash);
				if (iter != caches.end())
					return iter->second;
				else
				{
					ResultType* type = new ResultType(type1, type2, type3, stype, ctype);
					type->_hashCode = hash;
					type->impl->name = InertiaString(str.c_str());
					caches.insert(std::make_pair(hash, type));
					return type;
				}
			}
			return false;
		}

		ResultType_impl(_type type1, _type type2, _type type3, _type stype, ResultTypes rtype) : itype1(type1), itype2(type2), itype3(type3), istype(stype), cttype(rtype)
		{

		};

		static void Remove(ResultType* at)
		{
			std::lock_guard<std::mutex> lock(locker);
			ResultType* att = at;
			auto iter = caches.find(*at->_hashCode);
			if (iter != caches.end())
			{
				caches.erase(iter);
			}
		}
	};

	std::mutex ResultType_impl::locker;
	std::unordered_map<size_t, ResultType*> ResultType_impl::caches;
	std::hash<std::wstring> ResultType_impl::hash_fn;
}

const ResultType* ResultType::Create(_type type1, _type type2, _type type3, _type stype, ResultTypes rtype)
{
	return ResultType_impl::Create(type1, type2, type3, stype, rtype);
}

ResultType::~ResultType()
{
	if (impl)
	{
		ResultType_impl::Remove(this);
		delete impl;
		impl = nullptr;
	}
}

const wchar_t* ResultType::GetName() const
{
	return impl != nullptr ? impl->name.get().c_str() : nullptr;
}

_type ResultType::GetType1() const
{
	return  impl != nullptr ? impl->itype1 : nullptr;
}

_type ResultType::GetType2() const
{
	return  impl != nullptr ? impl->itype2 : nullptr;
}

_type ResultType::GetType3() const
{
	return  impl != nullptr ? impl->itype3 : nullptr;
}

_type ResultType::GetStateType() const
{
	return  impl != nullptr ? impl->istype : nullptr;
}

ResultTypes ResultType::GetResultType() const
{
	return  impl != nullptr ? impl->cttype : ResultTypes::Unknown;
}

ResultType::ResultType(_type type1, _type type2, _type type3, _type stype, ResultTypes rtype) : Type(TypeTag::Result)
{
	impl = new ResultType_impl(type1, type2, type3, stype, rtype);
}

