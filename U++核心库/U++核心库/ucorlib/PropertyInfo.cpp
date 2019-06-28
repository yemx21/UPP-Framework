#include "PropertyInfo.h"
#include "PropertyType.h"
#include "Object.h"
#include "InertiaString.h"
using namespace System;


namespace System
{
	class PropertyInfo_impl
	{
	protected:
		friend class PropertyInfo;
		InertiaString str;
		PropertyFlags flag;
		PropertyType* type;
	public:
		PropertyInfo_impl(const wchar_t* name, PropertyFlags flags, PropertyType* type) : str(name), flag(flags)
		{
			this->type = type;
		}
		~PropertyInfo_impl()
		{
			if (type)
			{
				delete type;
				type = nullptr;
			}
		}
	};
}

PropertyInfo::PropertyInfo(const wchar_t* name, PropertyFlags flags, PropertyType* type)
{
	impl = new PropertyInfo_impl(name, flags, type);
}

PropertyInfo::~PropertyInfo()
{
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

bool PropertyInfo::operator ==(const PropertyInfo& info) const
{
	if (info.impl && impl)
	{
		if (impl->str==info.impl->str)
		{
			if (info.impl->type ==impl->type)
			{
				return info.impl->flag == impl->flag;
			}
		}
	}
	return false;
}

bool PropertyInfo::operator !=(const PropertyInfo& info) const
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

const wchar_t* PropertyInfo::GetName() const
{
	return impl != nullptr ? impl->str->c_str() : nullptr;
}

const PropertyType* PropertyInfo::GetType() const
{
	return impl != nullptr ? impl->type : nullptr;
}

PropertyFlags PropertyInfo::GetFlags() const
{
	return impl != nullptr ? impl->flag : PropertyFlags::Public;
}

PropertyPolicy PropertyInfo::GetPolicy() const
{
	if (impl)
		if (impl->type)
			return impl->type->Policy;
	return PropertyPolicy::All;
}

Any PropertyInfo::Get(Object* obj, bool nothrow) const
{
	if (impl)
		if (impl->type)
			return impl->type->Get(obj, nothrow);
	return _missing;
}

bool PropertyInfo::Set(Object* obj, const Any& value, bool nothrow) const
{
	if (impl)
	if (impl->type)
		return impl->type->Set(obj, value, nothrow);
	return false;
}

Any PropertyInfo::Get(const Object* obj, bool nothrow) const
{
	if (impl)
	if (impl->type)
		return impl->type->Get(obj, nothrow);
	return _missing;
}

bool PropertyInfo::Set(const Object* obj, const Any& value, bool nothrow) const
{
	if (impl)
	if (impl->type)
		return impl->type->Set(obj, value, nothrow);
	return false;
}

Any PropertyInfo::Get(nullptr_t obj, bool nothrow) const
{
	if (impl)
		if (impl->type)
			return impl->type->Get(obj, nothrow);
	return _missing;
}

bool PropertyInfo::Set(nullptr_t obj, const Any& value, bool nothrow) const
{
	if (impl)
		if (impl->type)
			return impl->type->Set(obj, value, nothrow);
	return false;
}

Any PropertyType::Get(nullptr_t obj, bool nothrow) const
{
	return Get((Object*)nullptr, nothrow);
}

bool PropertyType::Set(nullptr_t obj, const Any& value, bool nothrow) const
{
	return Set((Object*)nullptr, value, nothrow);
}

const wchar_t* PropertyType::GetName() const
{
	if (!description)
	{
		std::wstring result;
		switch (kind)
		{
			case PropertyKinds::Read:
				result += L"Get=";
				result += readType->Name;
				result += L"()";
			break;
			case PropertyKinds::Write:
				result += L"Set=void(";
				result += writeType->Name;
				result += ')';
			break;		
			case PropertyKinds::ReadWrite:
				result += L"Get=";
				result += readType->Name;
				result += L"(),";
				result += L"Set=void(";
				result += writeType->Name;
				result += ')';
				break;		
			case PropertyKinds::ConstRead:
				result += L"Get=";
				result += readType->Name;
				result += L"() const";
				break;			
			case PropertyKinds::ConstRead_Write:
				result += L"Get=";
				result += readType->Name;
				result += L"() const,";
				result += L"Set=void(";
				result += writeType->Name;
				result += ')';
				break;
			case PropertyKinds::ConstWrite:
				result += L"Set=void(";
				result += writeType->Name;
				result += L") const";
				break;			
			case PropertyKinds::ConstWrite_Read:				
				result += L"Get=";
				result += readType->Name;
				result += L"(),";
				result += L"Set=void(";
				result += writeType->Name;
				result += L") const";
				break;
			case PropertyKinds::ConstReadWrite:
				result += L"Get=";
				result += readType->Name;
				result += L"() const,";
				result += L"Set=void(";
				result += writeType->Name;
				result += L") const";
				break;			
			case PropertyKinds::StaticRead:
				result += L"Get=static ";
				result += readType->Name;
				result += L"()";
				break;
			case PropertyKinds::StaticWrite:
				result += L"Set=static void(";
				result += writeType->Name;
				result += ')';
				break;
			case PropertyKinds::StaticReadWrite:
				result += L"Get=static ";
				result += readType->Name;
				result += L"(),";
				result += L"Set=static void(";
				result += writeType->Name;
				result += ')';
				break;
		}
		try
		{
			PropertyType* mt = const_cast<PropertyType*>(this);
			mt->description = result;
		}
		catch (...)
		{
			return nullptr;
		}
	}
	return (*description).c_str();

}

PropertyType::PropertyType() :Type(TypeTag::Property)
{

}

PropertyKinds PropertyType::GetKind() const
{
	return kind;
}

PropertyPolicy PropertyType::GetPolicy() const
{
	return policy;
}

_type PropertyType::GetClassType() const
{
	return ownerClass;
}


_type PropertyType::GetReadType() const
{
	return readType;
}

PropertyType::~PropertyType()
{
	
}

_type PropertyType::GetWriteType() const
{
	return writeType;
}

bool PropertyType::operator ==(const PropertyType& type) const
{
	if (type.ownerClass == ownerClass)
	{
		if (type.readType == readType)
		{
			if (type.writeType == writeType)
			{
				return type.policy == policy;
			}
		}
	}
	return false;

}

bool PropertyType::operator !=(const PropertyType& type) const
{
	if (type.ownerClass == ownerClass)
	{
		if (type.readType == readType)
		{
			if (type.writeType == writeType)
			{
				return type.policy != policy;
			}
		}
	}
	return true;
}