#include "Assembly.h"
#include <algorithm>
#include "Assembly_impl.h"
#include "NameSpace_impl.h"
using namespace System;

namespace System
{

	void NameSpaceSubEnumerator_impl::iterate(std::vector<NameSpace*>* p)
	{
		if (!p) yield_break;

		for (auto iter = p->begin(); iter != p->end(); iter++)
		{
			NameSpace* c = *iter;
			if (c)
			{
				yield_return(c)
				std::vector<NameSpace*>* pp = NameSpaceEnumerator_impl::GetChildren(c);
				yield_nested(iterate(pp))
			}
		}
	}

	NameSpaceEnumerator_impl::~NameSpaceEnumerator_impl()
	{
		SAFE_DELETE(sub);
	}

	NameSpaceEnumerator::NameSpaceEnumerator() :impl(nullptr)
	{

	}

	NameSpaceEnumerator::~NameSpaceEnumerator()
	{
		if (impl){ delete impl; impl = nullptr; }
	}

	const wchar_t* NameSpaceEnumerator::GetCurrentName() const
	{
		if (impl) return impl->GetCurrentName();
		return nullptr;
	}

	const NameSpace* NameSpaceEnumerator::GetCurrent() const
	{
		if (impl) return impl->GetCurrent();
		return nullptr;
	}

	const Assembly* NameSpaceEnumerator::GetCurrentAssembly() const
	{
		if (impl) return impl->GetCurrentAssembly();
		return nullptr;
	}


	unsigned int NameSpaceEnumerator::GetCount() const
	{
		return impl != nullptr ? impl->Count() : 0;
	}

	void NameSpaceEnumerator::Next()
	{
		if (impl) impl->Next();
	}

	bool NameSpaceEnumerator::GetIsEnd() const
	{
		if (impl) return impl->IsEnd();
		return true;
	}

	NameSpaceEnumerator_impl::NameSpaceEnumerator_impl(base owner, bool subNameSpace)
	{
		iterOwner = owner;
		sub = subNameSpace ? new NameSpaceSubEnumerator_impl(owner) : nullptr;
		citer = iterOwner->begin();
	}

	void NameSpaceEnumerator_impl::_Count(unsigned int& sum, NameSpace* space) const
	{
		if (!space) return;
		if (space->internal)
		{
			size_t ch = space->internal->Children.size();
			if (ch != 0)
			{
				sum += ch;
				for (auto iter = space->internal->Children.begin(); iter != space->internal->Children.end(); iter++)
				{
					_Count(sum, *iter);
				}
			}
		}
		return;
	}

	unsigned int NameSpaceEnumerator_impl::Count() const
	{
		if (sub)
		{
			unsigned int sum = 0;
			for (base_iterator iter = iterOwner->begin(); iter != iterOwner->end(); iter++)
			{
				_Count(sum, *iter);
			}
		}
		return iterOwner->size();
	}

	const wchar_t* NameSpaceEnumerator_impl::GetCurrentName() const
	{
		const NameSpace* m =  GetCurrent();
		return m != nullptr ? m->Name : nullptr;
	}

	const NameSpace* NameSpaceEnumerator_impl::GetCurrent() const
	{
		try
		{
			if (!sub)
				return *citer;
			else
			{
				return (const NameSpace*)sub->operator*();
			}
		}
		catch (...)
		{

		}
		return nullptr;
	}

	const Assembly* NameSpaceEnumerator_impl::GetCurrentAssembly() const
	{
		const NameSpace* m = GetCurrent();
		return m != nullptr ? m->Assembly : nullptr;
	}


	void NameSpaceEnumerator_impl::Next()
	{
		if (!sub)
		{
			if (citer == iterOwner->end()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer++;
		}
		else
		{
			std::lock_guard<std::mutex> locker(lock);
			if (!sub->done())
				sub->operator++();
		}

	}

	bool NameSpaceEnumerator_impl::IsEnd() const
	{
		if (!sub)
			return citer == iterOwner->end();
		return sub->done();
	}
}

std::unique_ptr<NameSpaceEnumerator> Assembly::GetNameSpaceEnumerator(bool subNameSpace) const
{
	NameSpaceEnumerator* me = new NameSpaceEnumerator();
	me->impl = new NameSpaceEnumerator_impl(&internal->Registry, subNameSpace);
	return std::unique_ptr<NameSpaceEnumerator>(me);
}

Assembly::Assembly() :internal(nullptr)
{

}

Assembly* Assembly::Create(const wchar_t* name)
{
	return Assembly_impl::Create(name);
}

Assembly* Assembly::Create(const wchar_t* name, const System::Version& ver)
{
	return Assembly_impl::Create(name, ver);
}


Assembly* Assembly::Create(const wchar_t* name, const System::Version& ver, uGUID publicKey)
{
	return Assembly_impl::Create(name, ver, publicKey);
}

Assembly::~Assembly()
{
	delete internal;
	internal = nullptr;
}

const wchar_t* Assembly::GetName() const { return internal->Name.get().c_str(); }
const uGUID Assembly::GetToken() const { return internal->Token; }
const System::Version* Assembly::GetVersion() const { return internal->Version; }

AssemblyError Assembly::AddNameSpace(NameSpace* space)
{
	if (!space)return AssemblyError::EmptyNameSpace;
	if (!space->Name) return AssemblyError::InvalidNameSpace;
	if (space->Assembly != nullptr) return AssemblyError::OwnedByOther;
	auto iter = std::find_if(internal->Registry.begin(), internal->Registry.end(), NameSpaceComparer(space));
	if (iter != internal->Registry.end()) return AssemblyError::Existed;
	space->internal->_Assembly = this;
	internal->Registry.push_back(space);
	return AssemblyError::Success;
}


const NameSpace* Assembly::FindNameSpace(const wchar_t* name) const
{
	auto iter = std::find_if(internal->Registry.begin(), internal->Registry.end(), NameSpaceNameComparer(name));
	if (iter != internal->Registry.end()) return *iter;
	return nullptr;
}


bool Assembly::operator ==(const Assembly& assembly) const
{
	return internal == assembly.internal;
}

bool Assembly::operator !=(const Assembly& assembly) const
{
	return internal != assembly.internal;
}