#include "PtrType_impl.h"
#include "OSHelper.h"
using namespace System;

std::mutex& PtrType_impl::getlocker()
{
	static std::mutex locker;
	return locker;
}
std::unordered_map<size_t, PtrType*>& PtrType_impl::getcaches()
{
	static std::unordered_map<size_t, PtrType*> caches;
	return caches;
}
std::hash<std::wstring> PtrType_impl::hash_fn;

PtrType* PtrType_impl::Create(_type type, UInt dimension)
{
	if (!type) return nullptr;

	_type base = type;
	UInt od = 0;
	if (type->IsPointer)
	{
		try
		{
			const PtrType* pt = dynamic_cast<const PtrType*>(type);
			if (pt)
			{
				base = pt->BasicType;
				od = pt->Dimension;
			}
		}
		catch (...)
		{
			return nullptr;
		}
	}

	if (!base) return nullptr;
	od += dimension;
	if (od > 5) return nullptr;

	std::wstring wstr = std::wstring(base->Name);

	for (UInt i = 0; i < od; i++)
	{
		wstr.push_back(L'*');
	}

	size_t hash = hash_fn(wstr);

	std::lock_guard<std::mutex> lock(getlocker());
	std::unordered_map<size_t, PtrType*>& caches = getcaches();
	auto iter = caches.find(hash);
	if (iter != caches.end())
		return iter->second;
	else
	{
		PtrType* ptype = new PtrType(type, od);
		ptype->_hashCode = hash;
		ptype->name = wstr;
		caches.insert(std::make_pair(hash, ptype));
		return ptype;
	}
}

void PtrType_impl::Recycle()
{
	std::lock_guard<std::mutex> lock(getlocker());
	std::unordered_map<size_t, PtrType*>& caches = getcaches();
	std::unordered_map<size_t, PtrType*>::iterator iter;
	for (iter = caches.begin(); iter != caches.end();)
	{
		bool remove = false;
		PtrType* pt = iter->second;
		if (!OSHelper::IsValidAddress(pt, sizeof(PtrType), false))
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
