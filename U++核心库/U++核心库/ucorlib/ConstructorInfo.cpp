#include "ConstructorInfo.h"
#include "ConstructorType.h"
#include <string>
#include "Object.h"
#include <memory>

 
using namespace System;


namespace System
{
	class ConstructorInfo_impl
	{
	protected:
		friend class ConstructorInfo;
		InertiaString str;
		ConstructorFlags flag;
		ConstructorType* type;
	public:
		ConstructorInfo_impl(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams) : str(name), flag(flags)
		{
			this->type = type; 
			if (this->type)	this->type->defaultParams = defaultParams;
		}

		~ConstructorInfo_impl()
		{
			if (type)
			{
				type->destroy();
				type = nullptr;
			}
		}
	};
}

Object* ConstructorInfo::Invoke(bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(OptionalVariadicAny(_missing), nothrow);
}


Object* ConstructorInfo::Invoke(const OptionalVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(parameters, nothrow);
}

_type ConstructorInfo::GetDeclaringClass() const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->GetClassType();
}

bool ConstructorInfo::GetHasParameters() const
{
	if (!impl) return false;
	if (!impl->type) return false;
	return impl->type->ParameterTypes != nullptr;
}

ConstructorInfo::ConstructorInfo(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams)
{
	impl = new ConstructorInfo_impl(name, flags, type, defaultParams);
}

ConstructorInfo::~ConstructorInfo()
{
	if (impl){ delete impl; impl = nullptr; }
}

bool ConstructorInfo::operator ==(const ConstructorInfo& info) const
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

bool ConstructorInfo::operator !=(const ConstructorInfo& info) const
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


const wchar_t* ConstructorInfo::GetName() const
{
	return impl != nullptr ? impl->str->c_str() : nullptr;
}

const ConstructorType* ConstructorInfo::GetType() const
{
	return impl != nullptr ? impl->type : nullptr;
}

const ConstructorFlags ConstructorInfo::GetFlags() const
{
	return impl != nullptr ? impl->flag : ConstructorFlags::Public;
}
