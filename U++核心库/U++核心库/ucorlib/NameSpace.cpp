#include "NameSpace.h"
#include <algorithm>
#include "NameSpace_impl.h"
#include "Assembly_impl.h"
using namespace System;

namespace System
{
	class TypeComparer
	{
	private:
		const wchar_t* name;
	public:
		explicit TypeComparer(_type cls)
		{
			name = cls!=NULL? cls->Name : 0;
		}
		bool operator() (_type cls)
		{
			return name == nullptr || cls == nullptr ? false : wcscmp(cls->Name, name) == 0;
		}
	};

	class TypeNameComparer
	{
	private:
		const wchar_t* name;
	public:
		explicit TypeNameComparer(const wchar_t* str) :name(str)
		{

		}
		bool operator() (_type cls)
		{
			return name == nullptr || cls == nullptr ? false : wcscmp(cls->Name, name) == 0;
		}
	};

	class TypeEnumerator_impl
	{
	protected:
		friend class NameSpace;
		friend class TypeEnumerator;
		typedef std::vector<_type>* base;
		typedef std::vector<_type>::const_iterator base_iterator;
		typedef std::vector<_type>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		std::mutex lock;
		TypeEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const wchar_t* GetCurrentName() const
		{
			_type m = *citer;
			return m != nullptr ? m->Name : nullptr;
		}

		_type GetCurrent() const
		{
			return *citer;
		}

		const NameSpace* GetCurrentNameSpace() const
		{
			_type m = *citer;
			return m != nullptr ? m->NameSpace : nullptr;
		}

		const Assembly* GetCurrentAssembly() const
		{
			_type m = *citer;
			return m != nullptr ? m->Assembly : nullptr;
		}

		void Next()
		{
			if (citer == iterOwner->end()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer++;
		}

		void Previous()
		{
			if (citer == iterOwner->begin()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	TypeEnumerator::TypeEnumerator() :impl(nullptr)
	{

	}

	TypeEnumerator::~TypeEnumerator()
	{
		if (impl){ delete impl; impl = nullptr; }
	}

	const wchar_t* TypeEnumerator::GetCurrentName() const
	{
		if (impl) return impl->GetCurrentName();
		return nullptr;
	}

	_type TypeEnumerator::GetCurrent() const
	{
		if (impl) return impl->GetCurrent();
		return nullptr;
	}

	const NameSpace* TypeEnumerator::GetCurrentNameSpace() const
	{
		if (impl) return impl->GetCurrentNameSpace();
		return nullptr;
	}

	const Assembly* TypeEnumerator::GetCurrentAssembly() const
	{
		if (impl) return impl->GetCurrentAssembly();
		return nullptr;
	}

	unsigned int TypeEnumerator::GetCount() const
	{
		return impl != nullptr ? impl->Count() : 0;
	}

	void TypeEnumerator::Next()
	{
		if (impl) impl->Next();
	}

	void TypeEnumerator::Previous()
	{
		if (impl) impl->Previous();
	}

	bool TypeEnumerator::GetIsEnd() const
	{
		if (impl) return impl->IsEnd();
		return true;
	}
}

std::unique_ptr<TypeEnumerator> NameSpace::GetTypeEnumerator() const
{
	TypeEnumerator* te = new TypeEnumerator();
	te->impl = new TypeEnumerator_impl(&internal->Registry);
	return std::unique_ptr<TypeEnumerator>(te);
}

NameSpace::NameSpace(const wchar_t* name) 
{
	internal = new NameSpace_impl(name);
}

NameSpace::~NameSpace()
{
	SAFE_DELETE(internal);
}

Type* NameSpace_impl::InternalRegisiter(NameSpace* space, Type* type)
{
	if (!space) return nullptr;
	if (!type) return nullptr;
	if (!type->Name) return nullptr;
	if (type->space != nullptr)
		if (type->space != space) return nullptr;
	auto iter = std::find_if(space->internal->Registry.begin(), space->internal->Registry.end(), TypeComparer(type));
	if (iter != space->internal->Registry.end()) return type;
	space->internal->Registry.push_back(type);
	type->space = space;
	return type;
}

std::unique_ptr<NameSpaceEnumerator> NameSpace::GetNameSpaceEnumerator(bool subNameSpace) const
{
	NameSpaceEnumerator* me = new NameSpaceEnumerator();
	me->impl = new NameSpaceEnumerator_impl(&internal->Children, subNameSpace);
	return std::unique_ptr<NameSpaceEnumerator>(me);
}

NameSpaceError NameSpace::Register(_type type)
{
	if (!type)return NameSpaceError::EmptyType;
	if (!type->Name) return NameSpaceError::InvalidType;
	if (type->IsBuiltin)return NameSpaceError::InvalidType;
	if (type->space != nullptr)
		if (type->space != this) return NameSpaceError::OwnedByOther;
	auto iter = std::find_if(internal->Registry.begin(), internal->Registry.end(), TypeComparer(type));
	if (iter != internal->Registry.end()) return NameSpaceError::Existed;
	internal->Registry.push_back(type);
	try
	{
		Type* ty = const_cast<Type*>(type);
		if (ty)
		{
			ty->space = this;
			return NameSpaceError::Success;
		}
	}
	catch (...)
	{
		return NameSpaceError::Unknown;
	}
	return NameSpaceError::Unknown;
}

NameSpaceError NameSpace::AddNameSpace(NameSpace* space)
{
	if (!space)return NameSpaceError::EmptyNameSpace;
	if (!space->Name) return NameSpaceError::InvalidNameSpace;
	if (space->Assembly != nullptr) return NameSpaceError::OwnedByOther;
	if (space->internal->_Parent != nullptr) 
		if(space->internal->_Parent!=this)
			return NameSpaceError::OwnedByOther;
	auto iter = std::find_if(internal->Children.begin(), internal->Children.end(), NameSpaceComparer(space));
	if (iter != internal->Children.end()) return NameSpaceError::Existed;
	space->internal->_Parent = this;
	space->internal->_Assembly = internal->_Assembly;
	internal->Children.push_back(space);
	return NameSpaceError::Success;
}

NameSpaceError NameSpace::UnRegister(_type type)
{
	if (!type)return NameSpaceError::EmptyType;
	if (!type->Name) return NameSpaceError::InvalidType;
	if (type->IsBuiltin)return NameSpaceError::InvalidType;
	if (type->space != nullptr)
		if (type->space != this) return NameSpaceError::OwnedByOther;
	auto iter = std::find_if(internal->Registry.begin(), internal->Registry.end(), TypeComparer(type));
	if (iter == internal->Registry.end())
	{
		try
		{
			internal->Registry.erase(iter);
			SAFE_DELETE(type);
		}
		catch (...)
		{
			return NameSpaceError::Unknown;
		}
	}
	return NameSpaceError::InvalidType;
}

NameSpaceError NameSpace::UnRegister(const wchar_t* name, _type type)
{
	auto iter = std::find_if(internal->Children.begin(), internal->Children.end(), NameSpaceNameComparer(name));
	if (iter != internal->Children.end())
	{
		NameSpace* ns = *iter;
		if (ns)
		{
			if (!type)return NameSpaceError::EmptyType;
			if (!type->Name) return NameSpaceError::InvalidType;
			if (type->IsBuiltin)return NameSpaceError::InvalidType;
			if (type->space != nullptr)
				if (type->space != ns) return NameSpaceError::OwnedByOther;
			auto iter = std::find_if(ns->internal->Registry.begin(), ns->internal->Registry.end(), TypeComparer(type));
			if (iter == ns->internal->Registry.end())
			{
				try
				{
					ns->internal->Registry.erase(iter);
					SAFE_DELETE(type);
				}
				catch (...)
				{
					return NameSpaceError::Unknown;
				}
			}

		}
	}
	return NameSpaceError::Unknown;
}

_type NameSpace::FindType(const wchar_t* name) const
{
	if (!name) return nullptr;
	auto iter = std::find_if(internal->Registry.begin(), internal->Registry.end(), TypeNameComparer(name));
	if (iter != internal->Registry.end()) return *iter;
	return nullptr;
}

const NameSpace* NameSpace::FindNameSpace(const wchar_t* name) const
{
	auto iter = std::find_if(internal->Children.begin(), internal->Children.end(), NameSpaceNameComparer(name));
	if (iter != internal->Children.end()) return *iter;
	return nullptr;
}

const wchar_t* NameSpace::GetName() const { return internal->Name.get().c_str(); }

const Assembly* NameSpace::GetAssembly() const 
{ 
	const System::Assembly* ass = internal->_Assembly;
	if (ass) return ass;
	const NameSpace* parent = internal->_Parent;
	while (parent != nullptr && ass==nullptr)
	{
		ass = parent->internal->_Assembly;
		parent = parent->internal->_Parent;
	}
	return ass;
}

const NameSpace* NameSpace::GetParent() const
{
	return internal->_Parent;
}

bool NameSpace::operator ==(const NameSpace& space) const
{
	return internal == space.internal;
}

bool NameSpace::operator !=(const NameSpace& space) const
{
	return internal != space.internal;
}