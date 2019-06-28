#include "FieldInfo.h"
#include "FieldType.h"
#include "Object.h"
#include <memory>
#include "InertiaString.h"
using namespace System;


namespace System
{
	class FieldInfo_impl
	{
	protected:
		friend class FieldInfo;
		InertiaString str;
		FieldFlags flag;
		FieldType* type;
	public:
		FieldInfo_impl(const wchar_t* name, FieldFlags flags, FieldType* type) : str(name), flag(flags)
		{
			this->type = type;
		}
		~FieldInfo_impl()
		{
			if (type)
			{
				delete type;
				type = nullptr;
			}
		}
	};
}

FieldInfo::FieldInfo(const wchar_t* name, FieldFlags flags, FieldType* type)
{
	impl = new FieldInfo_impl(name, flags, type);
}

bool FieldInfo::Set(Object* obj, const Any& value, bool nothrow) const
{
	if (!impl) return false;
	if (!impl->type) return  false;
	if (impl->type->Set(obj, value, nothrow)) return  false;
	return  false;
}

Any FieldInfo::Get(const Object* obj, bool nothrow) const
{
	if (!impl) return _missing;
	if (!impl->type) return _missing;
	try
	{
		return impl->type->Get(obj, nothrow);
	}
	catch (...)
	{
		if (nothrow) return _missing; else throw;
	}
}

FieldInfo::~FieldInfo()
{
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

const wchar_t* FieldInfo::GetName() const
{
	return impl != nullptr ? impl->str->c_str() : nullptr;
}

FieldPolicy FieldInfo::GetPolicy() const
{
	if (impl)
	if (impl->type)
		return impl->type->Policy;
	return FieldPolicy::All;
}

const FieldType* FieldInfo::GetType()const
{
	return impl != nullptr ? impl->type : nullptr;
}

FieldFlags FieldInfo::GetFlags() const
{
	return impl != nullptr ? impl->flag : FieldFlags::Default;
}

bool FieldInfo::operator ==(const FieldInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str == info.impl->str)
		{
			if (info.impl->type == impl->type)
			{
				return info.impl->flag == impl->flag;
			}
		}
	}
	return false;
}

bool FieldInfo::operator !=(const FieldInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str == info.impl->str)
		{
			if (info.impl->type == impl->type)
			{
				return info.impl->flag != impl->flag;
			}
		}
	}
	return true;
}