#include "Any.h"
#include "Converter.h"
using namespace System;

namespace System
{
	typedef std::type_info TypeDescriptor;

	struct PMD
	{
		ptrdiff_t mdisp; //vftable offset
		ptrdiff_t pdisp; //vbtable offset
		ptrdiff_t vdisp; //vftable offset (for virtual base class)
	};

	struct _s_RTTICompleteObjectLocator;
	struct _s_RTTIClassHierarchyDescriptor;
	struct _s_RTTIBaseClassDescriptor;

	struct _s_RTTICompleteObjectLocator
	{
		UInt32 signature;
		UInt32 offset; //vftable offset to this
		UInt32 cdOffset;
		TypeDescriptor *pTypeDescriptor;
		_s_RTTIClassHierarchyDescriptor *pClassHierarchyDescriptor;
	};
	struct _s_RTTIClassHierarchyDescriptor
	{
		UInt32 signature;
		UInt32 attributes; //bit 0 multiple inheritance, bit 1 virtual inheritance
		size_t numBaseClasses; //at least 1 (all base classes, including itself)
		_s_RTTIBaseClassDescriptor **pBaseClassArray;
	};
	struct _s_RTTIBaseClassDescriptor
	{
		TypeDescriptor *pTypeDescriptor;
		size_t numBaseClasses; //direct base classes
		PMD pmd; //Len=0xC
		UInt32 attributes;
		_s_RTTIClassHierarchyDescriptor *pClassHierarchyDescriptor; //of this base class
	};

	template<typename R> //only ptrdiff_t and size_t allowed
	R ReadData(size_t ptr)
	{
		return *((const R *)ptr);
	}
	template<typename R>
	R ReadData(const void *ptr)
	{
		return ReadData<R>((size_t)ptr);
	}

	_s_RTTICompleteObjectLocator *GetCompleteObjectLocator(const void *ptr) //not have vbtable
	{
		ptrdiff_t offset = 0;
		if (ReadData<ptrdiff_t>(ReadData<size_t>(ptr)) == 0) offset = ReadData<ptrdiff_t>(ReadData<size_t>(ptr)+4);
		return (_s_RTTICompleteObjectLocator *)(ReadData<size_t>(ReadData<size_t>((size_t)ptr + offset) - 4));
	}

	_inline bool Reviewer_Check_impl_loop(_s_RTTIBaseClassDescriptor * des, TypeDescriptor* pinfo, const std::type_info* info)
	{
		if (des)
		{
			if (des->pTypeDescriptor == info) 
				return true;
			_s_RTTIClassHierarchyDescriptor * chd = des->pClassHierarchyDescriptor;
			if (chd)
			{
				if (chd->numBaseClasses)
				{
					_s_RTTIBaseClassDescriptor ** pArrays = chd->pBaseClassArray;
					for (size_t i = 0; i < chd->numBaseClasses; i++)
					{
						_s_RTTIBaseClassDescriptor *  cd = pArrays[i];
						if (cd)
						{
							if (cd->pTypeDescriptor != pinfo)
							{
								if (Reviewer_Check_impl_loop(cd, cd->pTypeDescriptor, info)) return true;
							}
						}
					}
				}
			}
		}
		return false;
	}
}

bool _RTDynamicCastReviewer::Check_impl(const void* rawptr, const std::type_info& ptr_info, const std::type_info& info)
{
	_s_RTTICompleteObjectLocator *pCompleteObjectLocator;
	pCompleteObjectLocator = GetCompleteObjectLocator(rawptr);
	if (pCompleteObjectLocator)
	{
		if (pCompleteObjectLocator->pTypeDescriptor == &info) 
			return true;
		else if(pCompleteObjectLocator->pTypeDescriptor == &ptr_info)
		{
			_s_RTTIClassHierarchyDescriptor * pClassHierarchyDescriptor = pCompleteObjectLocator->pClassHierarchyDescriptor;
			if (pClassHierarchyDescriptor)
			{
				if (pClassHierarchyDescriptor->numBaseClasses)
				{
					_s_RTTIBaseClassDescriptor ** pArrays = pClassHierarchyDescriptor->pBaseClassArray;
					for (size_t i = 0; i < pClassHierarchyDescriptor->numBaseClasses; i++)
					{
						_s_RTTIBaseClassDescriptor *  cd = pArrays[i];
						if (cd)
						{
							if (cd->pTypeDescriptor != &ptr_info)
							{
								if (Reviewer_Check_impl_loop(cd, cd->pTypeDescriptor, &info)) return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

Any::Any(const Any& other)
{
	storage = other.storage ? 
		other.storage->clone() : 0;
	flag = other.flag;
}

Any::Any(Any&& other)
{
	storage = other.storage;
	other.storage = nullptr;
	flag = other.flag;
}

Any & Any::swap(Any & rhs)
{
	std::swap(flag, rhs.flag);
	std::swap(storage, rhs.storage);
	return *this;
}

Any & Any::operator=(const Any& rhs)
{
	Any(rhs).swap(*this);
	return *this;
}

Any & Any::operator=(Any&& rhs)
{
	rhs.swap(*this);
	Any().swap(rhs);
	return *this;
}

bool Any::GetIsNull() const
{
	return flag == AnyFlags::Null;
}

bool Any::GetIsMissing() const
{
	return flag == AnyFlags::Missing;
}

bool Any::GetIsNoDefault() const
{
	return flag == AnyFlags::NoDefault;
}

bool Any::GetIsValueable() const
{
	return flag != AnyFlags::Missing && flag != AnyFlags::NoDefault && flag != AnyFlags::Null && (flag == AnyFlags::UnFlagged ? storage != nullptr : true);
}

_type Any::GetType() const
{
	if (storage) return storage->getType(); else return nullptr;
}

const std::type_info & Any::GetSTDRTTI() const
{
	if (storage) return storage->type(); else return typeid(_missing_t);
}

Object* Any::AllocateObject() const
{
	if (!storage) return nullptr;
	if (storage->storageType() != AnyStorageTypes::Value) return nullptr;
	return storage->allocate();
}

Object* Any::ObjectGetter_impl() const
{
	if (!storage) return nullptr;
	if (storage->storageType() < AnyStorageTypes::StaticValuePtr) return nullptr;
	return storage->allocate();
}

Object** Any::ObjectGetter_impl1() const
{
	if (!storage) return nullptr;
	if (storage->storageType() < AnyStorageTypes::StaticValuePtr) return nullptr;
	return storage->allocate1();
}

Object*** Any::ObjectGetter_impl2() const
{
	if (!storage) return nullptr;
	if (storage->storageType() < AnyStorageTypes::StaticValuePtr) return nullptr;
	return storage->allocate2();
}

Object**** Any::ObjectGetter_impl3() const
{
	if (!storage) return nullptr;
	if (storage->storageType() < AnyStorageTypes::StaticValuePtr) return nullptr;
	return storage->allocate3();
}

Object***** Any::ObjectGetter_impl4() const
{
	if (!storage) return nullptr;
	if (storage->storageType() < AnyStorageTypes::StaticValuePtr) return nullptr;
	return storage->allocate4();
}

UInt8 Any::GetDimension() const
{
	return storage ? storage->dimension() : 0U;
}

size_t Any::GetDimensionSize(UInt8 index) const
{
	return storage ? storage->dimension_num(index) : 0U;
}

bool Any::TryGetString(std::wstring& str, const CultureInfo& loc) const
{
	_type type = Type;
	if (!type) return false;
	try
	{
		bool iswiden = false;
		bool israw = false;
		if (type->GetIsRawOrSTDString(iswiden, israw))
		{
			TypeDetails td = type->GetDetail();
			Any tmp = Any();
			if (israw)
			{
				if (!iswiden)
				{
					const char* ctmp = as<const char*>();
					if (ctmp)
					{
						str = Converter::Convert(ctmp, loc.CLocale, loc.CPlusLocale);
						return true;
					}
				}
				else
				{
					const wchar_t* ctmp = as<const wchar_t*>();
					if (ctmp)
					{
						str = std::wstring(ctmp);
						return true;
					}
				}
			}
			else
			{
				if (iswiden)
				{
					str = as<std::wstring>();
					return true;
				}
				else
				{
					std::string stmp = as<std::string>();
					return Converter::Convert(stmp, str, loc.CLocale);
				}
			}
		}
	}
	catch (...){}
	return false;
}

Any::Any(nullptr_t null) : storage(nullptr), flag(AnyFlags::Null)
{

}

Any::Any(_missing_t optional) : storage(nullptr), flag(AnyFlags::Missing)
{

}

Any::Any(_nodefault_t nodefault) : storage(nullptr), flag(AnyFlags::NoDefault)
{

}

Any::Any() : storage(nullptr), flag(AnyFlags::UnFlagged)
{
}

Any::~Any()
{
	SAFE_DELETE(storage);
}


#pragma region Number

Any::Any(bool& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<bool>(value);
	else
		storage = new ValuePointerStorage<bool>(&value, ValuePointerKinds::Left);
}

Any::Any(const bool& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<bool>(value);
	else
		storage = new ValuePointerStorage<bool>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile bool& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<bool>(value);
	else
		storage = new ValuePointerStorage<bool>(&value, ValuePointerKinds::Left);
}

Any::Any(Int8& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int8>(value);
	else
		storage = new ValuePointerStorage<Int8>(&value, ValuePointerKinds::Left);
}

Any::Any(const Int8& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int8>(value);
	else
		storage = new ValuePointerStorage<Int8>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Int8& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int8>(value);
	else
		storage = new ValuePointerStorage<Int8>(&value, ValuePointerKinds::Left);
}

Any::Any(UInt8& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt8>(value);
	else
		storage = new ValuePointerStorage<UInt8>(&value, ValuePointerKinds::Left);
}

Any::Any(const UInt8& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt8>(value);
	else
		storage = new ValuePointerStorage<UInt8>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile UInt8& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt8>(value);
	else
		storage = new ValuePointerStorage<UInt8>(&value, ValuePointerKinds::Left);
}

Any::Any(UInt16& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt16>(value);
	else
		storage = new ValuePointerStorage<UInt16>(&value, ValuePointerKinds::Left);
}

Any::Any(const UInt16& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt16>(value);
	else
		storage = new ValuePointerStorage<UInt16>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile UInt16& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt16>(value);
	else
		storage = new ValuePointerStorage<UInt16>(&value, ValuePointerKinds::Left);
}

Any::Any(UInt24& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt24>(value);
	else
		storage = new ValuePointerStorage<UInt24>(&value, ValuePointerKinds::Left);
}

Any::Any(const UInt24& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt24>(value);
	else
		storage = new ValuePointerStorage<UInt24>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile UInt24& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt24>(value);
	else
		storage = new ValuePointerStorage<UInt24>(&value, ValuePointerKinds::Left);
}

Any::Any(UInt32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt32>(value);
	else
		storage = new ValuePointerStorage<UInt32>(&value, ValuePointerKinds::Left);
}

Any::Any(const UInt32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt32>(value);
	else
		storage = new ValuePointerStorage<UInt32>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile UInt32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt32>(value);
	else
		storage = new ValuePointerStorage<UInt32>(&value, ValuePointerKinds::Left);
}

Any::Any(ULong& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<ULong>(value);
	else
		storage = new ValuePointerStorage<ULong>(&value, ValuePointerKinds::Left);
}

Any::Any(const ULong& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<ULong>(value);
	else
		storage = new ValuePointerStorage<ULong>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile ULong& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<ULong>(value);
	else
		storage = new ValuePointerStorage<ULong>(&value, ValuePointerKinds::Left);
}

Any::Any(UInt64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt64>(value);
	else
		storage = new ValuePointerStorage<UInt64>(&value, ValuePointerKinds::Left);
}

Any::Any(const UInt64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt64>(value);
	else
		storage = new ValuePointerStorage<UInt64>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile UInt64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt64>(value);
	else
		storage = new ValuePointerStorage<UInt64>(&value, ValuePointerKinds::Left);
}

Any::Any(UInt128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt128>(value);
	else
		storage = new ValuePointerStorage<UInt128>(&value, ValuePointerKinds::Left);
}

Any::Any(const UInt128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt128>(value);
	else
		storage = new ValuePointerStorage<UInt128>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile UInt128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt128>(value);
	else
		storage = new ValuePointerStorage<UInt128>(&value, ValuePointerKinds::Left);
}

Any::Any(Int16& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int16>(value);
	else
		storage = new ValuePointerStorage<Int16>(&value, ValuePointerKinds::Left);
}

Any::Any(const Int16& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int16>(value);
	else
		storage = new ValuePointerStorage<Int16>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Int16& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int16>(value);
	else
		storage = new ValuePointerStorage<Int16>(&value, ValuePointerKinds::Left);
}

Any::Any(Int24& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int24>(value);
	else
		storage = new ValuePointerStorage<Int24>(&value, ValuePointerKinds::Left);
}

Any::Any(const Int24& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int24>(value);
	else
		storage = new ValuePointerStorage<Int24>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Int24& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int24>(value);
	else
		storage = new ValuePointerStorage<Int24>(&value, ValuePointerKinds::Left);
}

Any::Any(Int32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int32>(value);
	else
		storage = new ValuePointerStorage<Int32>(&value, ValuePointerKinds::Left);
}

Any::Any(const Int32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int32>(value);
	else
		storage = new ValuePointerStorage<Int32>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Int32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int32>(value);
	else
		storage = new ValuePointerStorage<Int32>(&value, ValuePointerKinds::Left);
}

Any::Any(long& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<long>(value);
	else
		storage = new ValuePointerStorage<long>(&value, ValuePointerKinds::Left);
}

Any::Any(const long& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<long>(value);
	else
		storage = new ValuePointerStorage<long>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile long& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<long>(value);
	else
		storage = new ValuePointerStorage<long>(&value, ValuePointerKinds::Left);
}

Any::Any(Int64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int64>(value);
	else
		storage = new ValuePointerStorage<Int64>(&value, ValuePointerKinds::Left);
}

Any::Any(const Int64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int64>(value);
	else
		storage = new ValuePointerStorage<Int64>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Int64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int64>(value);
	else
		storage = new ValuePointerStorage<Int64>(&value, ValuePointerKinds::Left);
}

Any::Any(Int128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int128>(value);
	else
		storage = new ValuePointerStorage<Int128>(&value, ValuePointerKinds::Left);
}

Any::Any(const Int128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int128>(value);
	else
		storage = new ValuePointerStorage<Int128>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Int128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int128>(value);
	else
		storage = new ValuePointerStorage<Int128>(&value, ValuePointerKinds::Left);
}

Any::Any(Float32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float32>(value);
	else
		storage = new ValuePointerStorage<Float32>(&value, ValuePointerKinds::Left);
}

Any::Any(const Float32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float32>(value);
	else
		storage = new ValuePointerStorage<Float32>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Float32& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float32>(value);
	else
		storage = new ValuePointerStorage<Float32>(&value, ValuePointerKinds::Left);
}

Any::Any(Float64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float64>(value);
	else
		storage = new ValuePointerStorage<Float64>(&value, ValuePointerKinds::Left);
}

Any::Any(const Float64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float64>(value);
	else
		storage = new ValuePointerStorage<Float64>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Float64& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float64>(value);
	else
		storage = new ValuePointerStorage<Float64>(&value, ValuePointerKinds::Left);
}

Any::Any(Float128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float128>(value);
	else
		storage = new ValuePointerStorage<Float128>(&value, ValuePointerKinds::Left);
}

Any::Any(const Float128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float128>(value);
	else
		storage = new ValuePointerStorage<Float128>(&value, ValuePointerKinds::Left);
}

Any::Any(const volatile Float128& value, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float128>(value);
	else
		storage = new ValuePointerStorage<Float128>(&value, ValuePointerKinds::Left);
}

Any::Any(const bool&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<bool>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<bool>(const_cast<bool*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<bool>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile bool&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<bool>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<bool>(const_cast<bool*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<bool>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Int8&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int8>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Int8>(const_cast<Int8*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Int8>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Int8&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int8>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Int8>(const_cast<Int8*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Int8>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Int16&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int16>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Int16>(const_cast<Int16*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Int16>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Int16&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int16>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Int16>(const_cast<Int16*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Int16>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Int24&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int24>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Int24>(const_cast<Int24*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Int24>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Int24&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int24>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Int24>(const_cast<Int24*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Int24>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Int32&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int32>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Int32>(const_cast<Int32*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Int32>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Int32&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int32>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Int32>(const_cast<Int32*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Int32>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Int64&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int64>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Int64>(const_cast<Int64*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Int64>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Int64&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int64>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Int64>(const_cast<Int64*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Int64>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Int128&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int64>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Int128>(const_cast<Int128*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Int128>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Int128&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Int128>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Int128>(const_cast<Int128*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Int128>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const UInt8&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt8>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<UInt8>(const_cast<UInt8*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<UInt8>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile UInt8&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt8>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<UInt8>(const_cast<UInt8*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<UInt8>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const UInt16&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt16>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<UInt16>(const_cast<UInt16*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<UInt16>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile UInt16&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt16>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<UInt16>(const_cast<UInt16*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<UInt16>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const UInt24&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt24>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<UInt24>(const_cast<UInt24*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<UInt24>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile UInt24&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt24>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<UInt24>(const_cast<UInt24*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<UInt24>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const UInt32&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt32>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<UInt32>(const_cast<UInt32*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<UInt32>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile UInt32&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt32>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<UInt32>(const_cast<UInt32*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<UInt32>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const UInt64&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt64>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<UInt64>(const_cast<UInt64*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<UInt64>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile UInt64&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt64>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<UInt64>(const_cast<UInt64*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<UInt64>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const UInt128&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt128>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<UInt128>(const_cast<UInt128*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<UInt128>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile UInt128&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<UInt128>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<UInt128>(const_cast<UInt128*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<UInt128>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Float32&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float32>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Float32>(const_cast<Float32*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Float32>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Float32&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float32>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Float32>(const_cast<Float32*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Float32>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Float64&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float64>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Float64>(const_cast<Float64*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Float64>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Float64&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float64>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Float64>(const_cast<Float64*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Float64>(&value, ValuePointerKinds::Right);
	}
}

Any::Any(const Float128&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float128>(value);
	else
	{
		if (noconst)
		{
			storage = new ValuePointerStorage<Float128>(const_cast<Float128*>(&value), ValuePointerKinds::Right);
		}
		else
		{
			storage = new ValuePointerStorage<Float128>(&value, ValuePointerKinds::Right);
		}
	}
}

Any::Any(const volatile Float128&& value, bool noconst, bool copy) : flag(AnyFlags::UnFlagged)
{
	if (copy)
		storage = new NumberStorage<Float128>(value);
	else
	{
		if (noconst)
			storage = new ValuePointerStorage<Float128>(const_cast<Float128*>(&value), ValuePointerKinds::Right);
		else
			storage = new ValuePointerStorage<Float128>(&value, ValuePointerKinds::Right);
	}
}
#pragma endregion


#pragma region Void Cast
void* Any::VoidCast(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::ValuePtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			void* res = nullptr;
			storage->super(res);
			return res;
		}
			break;
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, 0U) > 0U)
			{
				if (dem == 1U && !val)
				{
					return tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const void* Any::VoidCast_Const(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::ValuePtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
				break;
			case Modifiers::Const:
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			void* res = nullptr;
			storage->super(res);
			return res;
		}
			break;
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
				break;
			case Modifiers::Const:
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, 0U) > 0U)
			{
				if (dem == 1U && !val)
				{
					return (const void*)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

volatile void* Any::VoidCast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::ValuePtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			void* res = nullptr;
			storage->super(res);
			return res;
		}
			break;
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, 0U) > 0U)
			{
				if (dem == 1U && !val)
				{
					return (volatile void*)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const volatile void* Any::VoidCast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::ValuePtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				break;
			default:
				goto ERR;
				break;
			}
			void* res = nullptr;
			storage->super(res);
			return res;
		}
			break;
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, 0U) > 0U)
			{
				if (dem == 1U && !val)
				{
					return (const volatile void*)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}
#pragma endregion

#pragma region Void2 Cast
void** Any::Void2Cast(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
			{
				if (dem == 2U && !val)
				{
					return (void**)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const void** Any::Void2Cast_Const(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
				break;
			case Modifiers::Const:
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
			{
				if (dem == 2U && !val)
				{
					return (const void**)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

volatile void** Any::Void2Cast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
			{
				if (dem == 2U && !val)
				{
					return (volatile void**)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const volatile void** Any::Void2Cast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
			{
				if (dem == 2U && !val)
				{
					return (const volatile void**)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}
#pragma endregion

#pragma region Void3 Cast
void*** Any::Void3Cast(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
			{
				if (dem == 3U && !val)
				{
					return (void***)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const void*** Any::Void3Cast_Const(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
				break;
			case Modifiers::Const:
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
			{
				if (dem == 3U && !val)
				{
					return (const void***)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

volatile void*** Any::Void3Cast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
			{
				if (dem == 3U && !val)
				{
					return (volatile void***)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const volatile void*** Any::Void3Cast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
			{
				if (dem == 3U && !val)
				{
					return (const volatile void***)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}
#pragma endregion

#pragma region Void4 Cast
void**** Any::Void4Cast(ModifierPolicy policy, size_t index1, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 4U && !val)
				{
					return (void****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const void**** Any::Void4Cast_Const(ModifierPolicy policy, size_t index1, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
				break;
			case Modifiers::Const:
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 4U && !val)
				{
					return (const void****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

volatile void**** Any::Void4Cast_Volatile(ModifierPolicy policy, size_t index1, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 4U && !val)
				{
					return (volatile void****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const volatile void**** Any::Void4Cast_ConstVolatile(ModifierPolicy policy, size_t index1, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 4U && !val)
				{
					return (const volatile void****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}
#pragma endregion

#pragma region Void5 Cast
void***** Any::Void5Cast(ModifierPolicy policy, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 5U && !val)
				{
					return (void*****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const void***** Any::Void5Cast_Const(ModifierPolicy policy, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
				break;
			case Modifiers::Const:
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 5U && !val)
				{
					return (const void*****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

volatile void***** Any::Void5Cast_Volatile(ModifierPolicy policy, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				break;
			case Modifiers::ConstVolatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 5U && !val)
				{
					return (volatile void*****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}

const volatile void***** Any::Void5Cast_ConstVolatile(ModifierPolicy policy, bool nothrow) const
{
	if (storage)
	{
		AnyStorageTypes ast = storage->storageType();
		switch (ast)
		{
		case AnyStorageTypes::StaticPtr:
		case AnyStorageTypes::DynamicPtr:
		{
			switch (storage->modifier)
			{
			case Modifiers::None:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Const:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
				break;
			case Modifiers::Volatile:
				if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
				break;
			case Modifiers::ConstVolatile:
				break;
			default:
				goto ERR;
				break;
			}
			bool val = false;
			size_t nindex = 0U;
			void* tmp = nullptr;
			UInt8 dem = 0U;
			if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
			{
				if (dem == 5U && !val)
				{
					return (const volatile void*****)tmp;
				}
			}
		}
			break;
		}
	}
ERR:
	throw bad_any_cast("empty", AnyErrors::General);
	return nullptr;
}
#pragma endregion
