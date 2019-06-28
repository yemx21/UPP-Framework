#include "OperatorInfo.h"
#include "OperatorType.h"
#include <string>
#include "Object.h"
#include <memory>


using namespace System;


namespace System
{
	class OperatorInfo_impl
	{
	protected:
		friend class OperatorInfo;
		InertiaString str;
		OperatorType* type;
	public:
		OperatorInfo_impl(const wchar_t* name, OperatorType* type) : str(name)
		{
			this->type = type;
		}

		~OperatorInfo_impl()
		{
			if (type)
			{
				type->destroy();
				type = nullptr;
			}
		}
	};
}

bool OperatorInfo::Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(result, obj, parameters, nothrow);
}

bool OperatorInfo::Invoke(Any* result, const Object* obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(result, obj, parameters, nothrow);
}

bool OperatorInfo::Invoke(Any* result, nullptr_t obj, const RealVariadicAny& parameters, bool nothrow) const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->Invoke(result, obj, parameters, nothrow);
}


OperatorInfo::OperatorInfo(const wchar_t* name, OperatorType* type)
{
	impl = new OperatorInfo_impl(name, type);
}

OperatorInfo::~OperatorInfo()
{
	if (impl){ delete impl; impl = nullptr; }
}

bool OperatorInfo::operator ==(const OperatorInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str == info.impl->str)
		{
			if (info.impl->type == impl->type)
			{
				return info.GetDeclaringClass() == GetDeclaringClass();
			}
		}
	}
	return false;
}

bool OperatorInfo::operator !=(const OperatorInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str == info.impl->str)
		{
			if (info.impl->type == impl->type)
			{
				return info.GetDeclaringClass() != GetDeclaringClass();
			}
		}
	}
	return true;
}

const wchar_t* OperatorInfo::GetName() const
{
	return impl != nullptr ? impl->str->c_str() : nullptr;
}

const OperatorType* OperatorInfo::GetType() const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type;
}

Operators OperatorInfo::GetOperator() const
{
	if (!impl) return Operators::Unknown;
	if (!impl->type) return Operators::Unknown;
	return impl->type->op;
}

_type OperatorInfo::GetDeclaringClass() const
{
	if (!impl) return nullptr;
	if (!impl->type) return nullptr;
	return impl->type->ClassType;
}