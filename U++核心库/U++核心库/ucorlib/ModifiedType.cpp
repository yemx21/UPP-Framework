#include "ModifiedType_impl.h"


using namespace System;


std::hash<std::wstring> ModifiedType_impl::hash_fn;

std::mutex& ModifiedType_impl::getlocker()
{
	static std::mutex locker;
	return locker;
}

std::unordered_map<size_t, ModifiedType*>&  ModifiedType_impl::getcaches()
{
	static std::unordered_map<size_t, ModifiedType*> caches;
	return caches;
}

const ModifiedType* ModifiedType_impl::Create(_type hold, Modifiers modifier)
{
	if (!hold) return nullptr;
	if (modifier<Modifiers::None || modifier>Modifiers::ConstVolatile) return nullptr;
	std::wstring wstr = std::wstring();
	switch (modifier)
	{
	case Modifiers::None:
	default:
		break;
	case Modifiers::Const:
		wstr.append(L"const ");
		break;
	case Modifiers::Volatile:
		wstr.append(L"volatile ");
		break;
	case Modifiers::ConstVolatile:
		wstr.append(L"const volatile ");
		break;
	}
	wstr.append(hold->Name);

	size_t hash = hash_fn(wstr);
	std::lock_guard<std::mutex> lock(getlocker());
	std::unordered_map<size_t, ModifiedType*>& caches = getcaches();
	auto iter = caches.find(hash);
	if (iter != caches.end())
		return iter->second;
	else
	{
		ModifiedType* mtype = new ModifiedType(hold, modifier);
		mtype->_hashCode = hash;
		mtype->name = wstr;
		caches.insert(std::make_pair(hash, mtype));
		return mtype;
	}
}

void ModifiedType_impl::Recycle()
{
	std::lock_guard<std::mutex> lock(getlocker());
	std::unordered_map<size_t, ModifiedType*>& caches = getcaches();
	std::unordered_map<size_t, ModifiedType*>::iterator iter;
	for (iter = caches.begin(); iter != caches.end();)
	{
		bool remove = false;
		ModifiedType* pt = iter->second;
		if (!OSHelper::IsValidAddress(pt, sizeof(ModifiedType), false))
		{
			remove = true;
		}
		else
		{
			if (!OSHelper::IsValidAddress(pt->Hold, sizeof(Type), false))
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


const ModifiedType* ModifiedType::Create(_type hold, Modifiers modifier) 
{
	return ModifiedType_impl::Create(hold, modifier);
}

const wchar_t* ModifiedType::GetName() const
{
	return name->c_str();
}

ModifiedType::ModifiedType(_type hold, Modifiers modifier) :Type(TypeTag::Modified) ,_Hold(hold), _Modifier(modifier)
{
	
}

ModifiedType::~ModifiedType()
{

}

_type ModifiedType::GetHold() const
{
	return _Hold;
}

Modifiers ModifiedType::GetModifier() const
{
	return _Modifier;
}