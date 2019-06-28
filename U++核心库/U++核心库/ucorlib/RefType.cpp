#include "RefType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"
#include "RefType_impl.h"

using namespace System;

const RefType* RefType::Create(_type type, RefKinds pos)
{
	return RefType_impl::Create(type, pos);
}

RefType::RefType(_type type, RefKinds kind) : Type(TypeTag::Ref), refType(type), refKind(kind)
{
	
}

RefType::~RefType()
{
}

RefKinds RefType::GetKind() const
{
	return refKind;
}

_type RefType::GetBasicType() const
{
	return refType;
}


const wchar_t* RefType::GetName() const
{
	return name->c_str();
}