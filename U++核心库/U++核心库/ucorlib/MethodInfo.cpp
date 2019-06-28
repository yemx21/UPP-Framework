#include "MethodInfo.h"
#include "MethodType.h"
#include <string>
#include "Object.h"
#include <memory>
using namespace System;



namespace System
{
	class MethodInfo_impl
	{
	protected:
		friend class MethodInfo;
		InertiaString str;
		MethodFlags flag;
		MethodType* type;
	public:
		MethodInfo_impl(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams) : str(name), flag(flags)
		{
			this->type = type;
			if (this->type)	this->type->defaultParams = defaultParams;
		}

		~MethodInfo_impl()
		{
			if (type)
			{
				type->destroy();
				type = nullptr;
			}
		}
	};
}

bool MethodInfo::Invoke(Any* result, Object* obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(result, obj, parameters, nothrow);
}

bool MethodInfo::Invoke(Any* result, const Object* obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(result, obj, parameters, nothrow);
}

bool MethodInfo::Invoke(Any* result, nullptr_t obj, const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(result, obj, parameters, nothrow);
}

_type MethodInfo::GetDeclaringClass() const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->GetClassType();
}

MethodInfo::MethodInfo(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams)
{
	impl = new MethodInfo_impl(name, flags, type, defaultParams);

}

MethodInfo::~MethodInfo()
{
	if (impl){ delete impl; impl = nullptr; }
}

bool MethodInfo::operator ==(const MethodInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str == info.impl->str)
		{
			if (info.impl->type == impl->type)
			{
				if (info.impl->flag == impl->flag)
				{
					return info.GetDeclaringClass() == GetDeclaringClass();
				}
			}
		}
	}
	return false;
}

bool MethodInfo::operator !=(const MethodInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str == info.impl->str)
		{
			if (info.impl->type == impl->type)
			{
				if (info.impl->flag == impl->flag)
				{
					return info.GetDeclaringClass() != GetDeclaringClass();
				}
			}
		}
	}
	return true;
}

const wchar_t* MethodInfo::GetName() const
{
	return impl != nullptr ? impl->str->c_str() : nullptr;
}

const MethodType* MethodInfo::GetType() const
{
	return impl != nullptr ? impl->type : nullptr;
}

const MethodFlags MethodInfo::GetFlags() const
{
	return impl != nullptr ? impl->flag : MethodFlags::Public;
}
