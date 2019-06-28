#include "PtrType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"
#include "PtrType_impl.h"

using namespace System;


const PtrType* PtrType::Create(_type type, UInt dimension)
{
	return PtrType_impl::Create(type, dimension);
}

PtrType::PtrType(_type type, UInt dimension) : Type(TypeTag::Ptr), ptrType(type), rank(dimension)
{
}

PtrType::~PtrType()
{
}

_type PtrType::GetBasicType() const
{
	return ptrType;
}

UInt8 PtrType::GetDimension() const
{
	return rank;
}

const wchar_t* PtrType::GetName() const
{
	return name->c_str();
}