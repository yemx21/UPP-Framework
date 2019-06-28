#include "RefType_impl.h"
#include "OSHelper.h"

using namespace System;

std::mutex& RefType_impl::getlocker()
{
	static std::mutex locker;
	return locker;
}
std::unordered_map<size_t, RefType*>& RefType_impl::getcaches()
{
	static std::unordered_map<size_t, RefType*> caches;
	return caches;
}

std::hash<std::wstring> RefType_impl::hash_fn;

RefType* RefType_impl::Create(_type type, RefKinds pos)
{
	if (!type) return nullptr;
	std::wstring wstr = std::wstring(type->Name) + (pos == RefKinds::Left ? std::wstring(L"&") : std::wstring(L"&&"));
	size_t hash = hash_fn(wstr);
	std::lock_guard<std::mutex> lock(getlocker());
	std::unordered_map<size_t, RefType*>& caches = getcaches();
	auto iter = caches.find(hash);
	if (iter != caches.end())
		return iter->second;
	else
	{
		RefType* rtype = new RefType(type, pos);
		rtype->_hashCode = hash;
		rtype->name = InertiaString(wstr);
		caches.insert(std::make_pair(hash, rtype));
		return rtype;
	}
}


void RefType_impl::Recycle()
{
	std::lock_guard<std::mutex> lock(getlocker());
	std::unordered_map<size_t, RefType*>& caches = getcaches();
	std::unordered_map<size_t, RefType*>::iterator iter;
	for (iter = caches.begin(); iter != caches.end();)
	{
		bool remove = false;
		RefType* pt = iter->second;
		if (!OSHelper::IsValidAddress(pt, sizeof(RefType), false))
		{
			remove = true;
		}
		else
		{
			if (!OSHelper::IsValidAddress(pt->BasicType, sizeof(Type), false))
				remove = true;
		}
		if (remove)
		{
			iter = caches.erase(iter);			
			pt->destroy();
			pt = nullptr;
		}
		else
			iter++;
	}
}