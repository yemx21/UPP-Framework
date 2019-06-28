#include "Type.h"
#include "Assembly.h"
#include "NameSpace.h"
#include "ArrayType.h"
#include "ClassInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"
#include "PtrType.h"
#include "MethodType.h"
#include "Any.h"
#include "BuiltInAssembly.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;

#pragma init_seg( ".CRT$XCC" )

std::unordered_map<std::type_index, std::unordered_set<std::type_index>> AnyConversion::regs = std::unordered_map<std::type_index, std::unordered_set<std::type_index> >();

const wchar_t* TypeTag_Name_unknown = L"???"; 
const wchar_t* TypeTag_Name__uobject = L"object";
const wchar_t* TypeTag_Name_any = L"any";
const wchar_t* TypeTag_Name_variadicAny = L"variadic any";
const wchar_t* TypeTag_Name_defaultVariadicAny = L"default variadic any";
const wchar_t* TypeTag_Name_realVariadicAny = L"real variadic any";
const wchar_t* TypeTag_Name_optionalVariadicAny = L"optional variadic any";
const wchar_t* TypeTag_Name_safeVoidPtr = L"safe_void_ptr";
const wchar_t* TypeTag_Name_inertiaString = L"inertia string";
const wchar_t* TypeTag_Name_void = L"void";
const wchar_t* TypeTag_Namebool = L"bool";
const wchar_t* TypeTag_Name_char = L"char";
const wchar_t* TypeTag_Name_uchar = L"unsigned char";
const wchar_t* TypeTag_Name_wchar = L"wide char";
const wchar_t* TypeTag_Name_short = L"short";
const wchar_t* TypeTag_Name_UShort = L"unsigned short";
const wchar_t* TypeTag_Name_int = L"int";
const wchar_t* TypeTag_Name_UInt = L"unsigned int";
const wchar_t* TypeTag_Name_Int24 = L"Int24";
const wchar_t* TypeTag_Name_UInt24 = L"unsigned Int24";
const wchar_t* TypeTag_Name_long = L"long";
const wchar_t* TypeTag_Name_ULong = L"unsigned long";
const wchar_t* TypeTag_Name_longlong = L"long long";
const wchar_t* TypeTag_Name_ULonglong = L"unsigned long long";
const wchar_t* TypeTag_Name_Int128 = L"Int128";
const wchar_t* TypeTag_Name_UInt128 = L"unsigned Int128";
const wchar_t* TypeTag_Name_float = L"float";
const wchar_t* TypeTag_Name_double = L"double";
const wchar_t* TypeTag_Name_longdouble = L"long double";

#pragma data_seg("GLOBAL_APP")      // 声明共享数据段，并命名该数据段
volatile UInt64 TYPE_COUNT = 0U;     // 必须在定义的同时进行初始化!!!!
#pragma data_seg()
#pragma comment(linker,"/section:GLOBAL_APP,rws")

namespace System
{
	class Type_impl
	{
	public:
		static void* locker;
	public:
		static Type* Create(TypeTag tag)
		{
			Type* type = new Type(tag);
			type->_hashCode = std::hash<std::wstring>{}(type->GetName());
			return type;
		}
		static void EnsureMutex()
		{
			if (!Type_impl::locker)	Type_impl::locker = Microsoft::winapi_CreateMutex(NULL, false, L"Global\\TYPE_LOCKER");
		}
	};

	void* Type_impl::locker = nullptr;
}

Type* const Type::unknownType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Unknown));

Type* const Type::objectType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Object));


Type* const Type::boolType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Bool));
Type* const Type::anyType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Any));
Type* const Type::variadicAnyType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Any));
Type* const Type::defaultVariadicAnyType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::DefaultVariadicAny));
Type* const Type::realVariadicAnyType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::RealVariadicAny));
Type* const Type::optionalVariadicAnyType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::OptionalVariadicAny));
Type* const Type::safeVoidPtrType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::SafeVoidPtr));
Type* const Type::inertiaStringType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::InertiaString));

Type* const Type::voidType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Void));
Type* const Type::charType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Char));
Type* const Type::ucharType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::UChar));
Type* const Type::wcharType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::WChar));
Type* const Type::shortType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Short));
Type* const Type::UShortType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::UShort));
Type* const Type::intType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Int));
Type* const Type::UIntType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::UInt));
Type* const Type::Int24Type = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Int24));
Type* const Type::UInt24Type = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::UInt24));
Type* const Type::longType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Long));
Type* const Type::ULongType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::ULong));
Type* const Type::longlongType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::LongLong));
Type* const Type::ULonglongType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::ULongLong));
Type* const Type::Int128Type = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Int128));
Type* const Type::UInt128Type = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::UInt128));
Type* const Type::floatType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Float));
Type* const Type::doubleType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::Double));
Type* const Type::longdoubleType = BuiltInAssemblyFactory::Regisiter(Type_impl::Create(TypeTag::LongDouble));


_type STDTypes::stringType = nullptr;
_type STDTypes::wstringType = nullptr;

_type BUILTINTypes::cultureInfoType = nullptr;

#pragma region STDTYPES
static void STDTypes_Define_string(ClassInfo& type);

template<> 
_type TypeHelper<std::string>::GetType() 
{ 
	ClassInfo::Create(STDTypes::stringType, &STDTypes_Define_string, L"string", sizeof(std::string), ClassFlags::Normal);
	return STDTypes::stringType;
}

static struct XAUL_PREMAIN_STD_string
{
	XAUL_PREMAIN_STD_string()
	{
		ClassInfo::Create(STDTypes::stringType, &STDTypes_Define_string, L"string", sizeof(std::string), ClassFlags::Normal);
	}
}XAUL_PREMAIN_ENTRY_STD_string;

void STDTypes_Define_string(ClassInfo& type)
{

}

static void STDTypes_Define_wstring(ClassInfo& type);

template<>
_type TypeHelper<std::wstring>::GetType()
{
	ClassInfo::Create(STDTypes::wstringType, &STDTypes_Define_wstring, L"std::wstring", sizeof(std::wstring), ClassFlags::Normal);
	return STDTypes::wstringType;
}

static struct XAUL_PREMAIN_STD_wstring
{
	XAUL_PREMAIN_STD_wstring()
	{
		ClassInfo::Create(STDTypes::wstringType, &STDTypes_Define_wstring, L"std::wstring", sizeof(std::wstring), ClassFlags::Normal);
	}
}XAUL_PREMAIN_ENTRY_STD_wstring;

void STDTypes_Define_wstring(ClassInfo& type)
{

}
#pragma endregion

#pragma region BUILTINTYPES
static void BUILTINTypes_Define_cultureInfo(ClassInfo& type);

template<>
_type TypeHelper<System::CultureInfo>::GetType()
{
	ClassInfo::Create(BUILTINTypes::cultureInfoType, &BUILTINTypes_Define_cultureInfo, L"System::CultureInfo", sizeof(System::CultureInfo), ClassFlags::Normal);
	return BUILTINTypes::cultureInfoType;
}

static struct XAUL_PREMAIN_BUILTIN_cultureInfo
{
	XAUL_PREMAIN_BUILTIN_cultureInfo()
	{
		ClassInfo::Create(BUILTINTypes::cultureInfoType, &BUILTINTypes_Define_cultureInfo, L"System::CultureInfo", sizeof(System::CultureInfo), ClassFlags::Normal);
	}
}XAUL_PREMAIN_ENTRY_BUILTIN_cultureInfo;

void BUILTINTypes_Define_cultureInfo(ClassInfo& type)
{

}

#pragma endregion

Type::Type(TypeTag tag) : _tag(tag), space(NULL)
{
	Type_impl::EnsureMutex();
	auto dwWaitResult = Microsoft::winapi_WaitForSingleObject(Type_impl::locker, API_INFINITE);

	if (dwWaitResult == API_WAIT_OBJECT_0 || dwWaitResult == API_WAIT_ABANDONED)
	{
		if (dwWaitResult == API_WAIT_ABANDONED)
		{
			
		}
		TYPE_COUNT++;
		Microsoft::winapi_ReleaseMutex(Type_impl::locker);
	}
}

Type::~Type()
{
	Type_impl::EnsureMutex();
	auto dwWaitResult = Microsoft::winapi_WaitForSingleObject(Type_impl::locker, API_INFINITE);

	if (dwWaitResult == API_WAIT_OBJECT_0 || dwWaitResult == API_WAIT_ABANDONED)
	{
		if (dwWaitResult == API_WAIT_ABANDONED)
		{
			
		}
		TYPE_COUNT--;
		Microsoft::winapi_ReleaseMutex(Type_impl::locker);
	}
}

UInt64 Type::GetCount()
{
	UInt64 count = 0U;
	Type_impl::EnsureMutex();
	auto dwWaitResult = Microsoft::winapi_WaitForSingleObject(Type_impl::locker, API_INFINITE);

	if (dwWaitResult == API_WAIT_OBJECT_0 || dwWaitResult == API_WAIT_ABANDONED)
	{
		if (dwWaitResult == API_WAIT_ABANDONED)
		{
			
		}
		count = TYPE_COUNT;
		Microsoft::winapi_ReleaseMutex(Type_impl::locker);
	}
	return count;
}

const wchar_t* Type::GetName() const
{
	switch (_tag) 
	{
	case TypeTag::Unknown:
		return TypeTag_Name_unknown;	
	case TypeTag::Object:
		return TypeTag_Name__uobject;
	case TypeTag::Any:
		return TypeTag_Name_any;
	case TypeTag::VariadicAny:
		return TypeTag_Name_variadicAny;
	case TypeTag::DefaultVariadicAny:
		return TypeTag_Name_defaultVariadicAny;
	case TypeTag::RealVariadicAny:
		return TypeTag_Name_realVariadicAny;
	case TypeTag::OptionalVariadicAny:
		return TypeTag_Name_optionalVariadicAny;
	case TypeTag::SafeVoidPtr:
		return TypeTag_Name_safeVoidPtr;
	case TypeTag::InertiaString:
		return TypeTag_Name_inertiaString;
	case TypeTag::Void:
		return TypeTag_Name_void;
	case TypeTag::Bool:
		return TypeTag_Namebool;
	case TypeTag::Char:
		return TypeTag_Name_char;
	case TypeTag::UChar:
		return TypeTag_Name_uchar;
	case TypeTag::WChar:
		return TypeTag_Name_wchar;
	case TypeTag::Int:
		return TypeTag_Name_int;
	case TypeTag::UInt:
		return TypeTag_Name_UInt;	
	case TypeTag::Short:
		return TypeTag_Name_short;
	case TypeTag::UShort:
		return TypeTag_Name_UShort;
	case TypeTag::Int24:
		return TypeTag_Name_Int24;
	case TypeTag::UInt24:
		return TypeTag_Name_UInt24;
	case TypeTag::Long:
		return TypeTag_Name_long;
	case TypeTag::ULong:
		return TypeTag_Name_ULong;
	case TypeTag::LongLong:
		return TypeTag_Name_longlong;
	case TypeTag::ULongLong:
		return TypeTag_Name_ULonglong;
	case TypeTag::Int128:
		return TypeTag_Name_Int128;
	case TypeTag::UInt128:
		return TypeTag_Name_UInt128;
	case TypeTag::Float:
		return TypeTag_Name_float;
	case TypeTag::Double:
		return TypeTag_Name_double;
	case TypeTag::LongDouble:
		return TypeTag_Name_longdouble;
	default:
		return 0;
	}
}

size_t Type::GetHashCode() const
{
	if (_hashCode) return *_hashCode; else return 0;
}

const System::NameSpace* Type::GetNameSpace() const
{
	return space;
}

const System::Assembly* Type::GetAssembly() const
{
	if (space) return space->Assembly; 
	return nullptr;
}

TypeDetails Type::GetDetail() const
{
	Modifiers modi = Modifiers::None;
	Declarators decl = Declarators::Value;
	UInt8 dm = 0;
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			modi = mt->GetModifier();
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Ptr)
				{
					const PtrType* pt = dynamic_cast<const PtrType*>(ht);
					if (pt)
					{
						decl = Declarators::Pointer;
						dm = pt->Dimension;
					}
				}
				else if (tt == TypeTag::Ref)
				{
					const RefType* rt = dynamic_cast<const RefType*>(ht);
					if (rt)
					{
						decl = rt->Kind == RefKinds::Left ? Declarators::LValueReference : Declarators::RValueReference;
					}
				}
			}
		}
	}

	switch (decl)
	{
		case Declarators::Value:
		{
			switch (modi)
			{
			case Modifiers::None:
				return TypeDetails::Value;
			case Modifiers::Const:
				return TypeDetails::Const_Value;
			case Modifiers::Volatile:
				return TypeDetails::Volatile_Value;
			case Modifiers::ConstVolatile:
				return TypeDetails::ConstVolatile_Value;
			}
		}
			break;
		case Declarators::Pointer:
		{
			if (dm < 1) return TypeDetails::Invalid;
			switch (modi)
			{
			case Modifiers::None:
				return (TypeDetails)((UShort)TypeDetails::Pointer + dm - 1);
			case Modifiers::Const:
				return (TypeDetails)((UShort)TypeDetails::Const_Pointer + dm - 1);
			case Modifiers::Volatile:
				return (TypeDetails)((UShort)TypeDetails::Volatile_Pointer + dm - 1);
			case Modifiers::ConstVolatile:
				return (TypeDetails)((UShort)TypeDetails::ConstVolatile_Pointer + dm - 1);
			}
		}
			break;
		case Declarators::LValueReference:
		{
			switch (modi)
			{
			case Modifiers::None:
				return TypeDetails::LValueReference;
			case Modifiers::Const:
				return TypeDetails::Const_LValueReference;
			case Modifiers::Volatile:
				return TypeDetails::Volatile_LValueReference;
			case Modifiers::ConstVolatile:
				return TypeDetails::ConstVolatile_LValueReference;
			}
		}
			break;
		case Declarators::RValueReference:
		{
			switch (modi)
			{
			case Modifiers::None:
				return TypeDetails::RValueReference;
			case Modifiers::Const:
				return TypeDetails::Const_RValueReference;
			case Modifiers::Volatile:
				return TypeDetails::Volatile_RValueReference;
			case Modifiers::ConstVolatile:
				return TypeDetails::ConstVolatile_RValueReference;
			}
		}
			break;
	}
	return TypeDetails::Invalid;
}

bool Type::GetIsRawString(bool& iswiden, bool ignoreModifier) const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return ht->GetIsRawString(iswiden, ignoreModifier);
				}
				else if (tt == TypeTag::Ptr)
				{
					const PtrType* pt = dynamic_cast<const PtrType*>(ht);
					if (pt)
					{
						iswiden = pt->_tag == TypeTag::WChar;
						if (iswiden) return true;
						return pt->_tag == TypeTag::Char;
					}
				}
			}
		}
	}
	else
	{
		if (_tag == TypeTag::Ptr)
		{
			const PtrType* pt = dynamic_cast<const PtrType*>(this);
			if (pt)
			{
				iswiden = pt->_tag == TypeTag::WChar;
				if (iswiden) return true;
				return pt->_tag == TypeTag::Char;
			}
		}
	}
	return false;
}

bool Type::GetIsSTDString(bool& iswiden, bool ignoreModifier) const
{
	if (_tag == TypeTag::Modified)
	{
		if (ignoreModifier)
		{
			const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
			if (mt)
			{
				_type ht = mt->Hold;
				if (ht)
				{
					return ht->GetIsSTDString(iswiden, ignoreModifier);
				}
			}
		}
	}
	else
	{
		iswiden = this == STDTypes::wstringType;
		if (iswiden) return true;
		return this == STDTypes::stringType;
	}
	return false;
}

bool Type::GetIsRawOrSTDString(bool& iswiden, bool& israw, bool ignoreModifier) const
{
	if (_tag == TypeTag::Modified)
	{
		if (ignoreModifier)
		{
			const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
			if (mt)
			{
				_type ht = mt->Hold;
				if (ht)
				{
					TypeTag tt = ht->_tag;
					if (tt == TypeTag::Modified)
					{
						return ht->GetIsRawOrSTDString(iswiden, ignoreModifier);
					}
					else if (tt == TypeTag::Ptr)
					{
						const PtrType* pt = dynamic_cast<const PtrType*>(ht);
						if (pt)
						{
							israw = true;
							iswiden = pt->_tag == TypeTag::WChar;
							if (iswiden) return true;
							return pt->_tag == TypeTag::Char;
						}
					}
					else
					{
						israw = false;
						iswiden = this == STDTypes::wstringType;
						if (iswiden) return true;
						return this == STDTypes::stringType;
					}
				}
			}
		}
		else
		{
			if (_tag == TypeTag::Ptr)
			{
				const PtrType* pt = dynamic_cast<const PtrType*>(this);
				if (pt)
				{
					israw = true;
					iswiden = pt->_tag == TypeTag::WChar;
					if (iswiden) return true;
					return pt->_tag == TypeTag::Char;
				}
			}
			else
			{
				israw = false;
				iswiden = this == STDTypes::wstringType;
				if (iswiden) return true;
				return this == STDTypes::stringType;
			}
		}
	}
	return false;
}

bool Type::DirectGetIsRawString(bool ignoreModifier) const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return ht->DirectGetIsRawString(ignoreModifier);
				}
				else if (tt == TypeTag::Ptr)
				{
					const PtrType* pt = dynamic_cast<const PtrType*>(ht);
					if (pt)
					{
						return pt->_tag == TypeTag::WChar || pt->_tag == TypeTag::Char;
					}
				}
			}
		}
	}
	else
	{
		if (_tag == TypeTag::Ptr)
		{
			const PtrType* pt = dynamic_cast<const PtrType*>(this);
			if (pt)
			{
				return pt->_tag == TypeTag::WChar || pt->_tag == TypeTag::Char;
			}
		}
	}
	return false;
}

bool Type::DirectGetIsSTDString(bool ignoreModifier) const
{
	if (_tag == TypeTag::Modified)
	{
		if (ignoreModifier)
		{
			const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
			if (mt)
			{
				_type ht = mt->Hold;
				if (ht)
				{
					return ht->DirectGetIsSTDString(ignoreModifier);
				}
			}
		}
	}
	else
	{
		return this == STDTypes::wstringType || this == STDTypes::stringType;
	}
	return false;
}

bool Type::DirectGetIsRawOrSTDString(bool ignoreModifier) const
{
	if (_tag == TypeTag::Modified)
	{
		if (ignoreModifier)
		{
			const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
			if (mt)
			{
				_type ht = mt->Hold;
				if (ht)
				{
					TypeTag tt = ht->_tag;
					if (tt == TypeTag::Modified)
					{
						return ht->DirectGetIsRawOrSTDString(ignoreModifier);
					}
					else if (tt == TypeTag::Ptr)
					{
						const PtrType* pt = dynamic_cast<const PtrType*>(ht);
						if (pt)
						{
							return pt->_tag == TypeTag::WChar || pt->_tag == TypeTag::Char;
						}
					}
					else
					{
						return this == STDTypes::wstringType || this == STDTypes::stringType;
					}
				}
			}
		}
		else
		{
			if (_tag == TypeTag::Ptr)
			{
				const PtrType* pt = dynamic_cast<const PtrType*>(this);
				if (pt)
				{
					return pt->_tag == TypeTag::WChar || pt->_tag == TypeTag::Char;
				}
			}
			else
			{
				return this == STDTypes::wstringType || this == STDTypes::stringType;
			}
		}
	}
	return false;
}

TypeTag Type::GetTag() const
{
	return _tag;
}

bool Type::GetIsObject() const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return ht->GetIsObject();
				}
				else
					return tt ==TypeTag::Struct || tt ==TypeTag::Enum;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Struct || _tag == TypeTag::Enum;
}

bool Type::GetIsBuiltin()const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return ht->GetIsBuiltin();
				}
				else
					return tt <= TypeTag::Bool;
			}
		}
		else
			return false;
	}
	return _tag <= TypeTag::Bool;
}

bool Type::GetIsScalar()const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsScalar();
				}
				else
					return tt > TypeTag::Void && tt <= TypeTag::Bool;
			}
		}
		else
			return false;
	}
	return _tag > TypeTag::Void && _tag <= TypeTag::Bool;
}

bool Type::GetIsArray()const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsArray();
				}
				else
					return tt == TypeTag::Array;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Array;
}

bool Type::GetIsPointer()const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsPointer();
				}
				else
					return tt == TypeTag::Ptr;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Ptr;
}

bool Type::GetIsClass()const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsClass();
				}
				else
					return tt == TypeTag::Struct;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Struct;
}

bool Type::IsBaseOf(_type info, bool ignoreModifier)const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->IsBaseOf(info, ignoreModifier);
				}
				else if (tt == TypeTag::Struct)
				{
					const ClassInfo* ci = dynamic_cast<const ClassInfo*>(ht);
					if (ci)
					{
						return ci->IsBaseOf(info);
					}
				}	
			}
		}
		else
			return false;
	}
	else
	{
		if (_tag == TypeTag::Struct)
		{
			const ClassInfo* ci = dynamic_cast<const ClassInfo*>(this);
			if (ci)
			{
				return ci->IsBaseOf(info);
			}
		}
	}
	return false;
}

bool Type::IsBaseOfOrSameAs(_type info, bool ignoreModifier) const
{
	if (IsType(info, ignoreModifier)) return true;
	return IsBaseOf(info, ignoreModifier);
}

bool Type::GetIsConstructor() const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsConstructor();
				}
				else
					return tt == TypeTag::Constructor;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Constructor;
}

bool Type::GetIsEnum()const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsEnum();
				}
				else
					return tt == TypeTag::Enum;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Enum;
}

bool System::Type::GetIsSigned() const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsSigned();
				}
				else
					return tt == TypeTag::Char || tt == TypeTag::Short || tt == TypeTag::Int || tt == TypeTag::Int24 || tt == TypeTag::Long || tt == TypeTag::LongLong || tt == TypeTag::Int128;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Char || _tag == TypeTag::Short || _tag == TypeTag::Int || _tag == TypeTag::Int24 || _tag == TypeTag::Long || _tag == TypeTag::LongLong || _tag == TypeTag::Int128;
}

void Type::destroy()
{
	if (!IsBuiltin)
	{
		delete this;
	}
}

bool Type::IsType(_type type, bool ignoreModifier) const
{
	if (!type) return false;
	if (type->_tag == _tag)
	{
		switch (_tag)
		{
			case TypeTag::Array:
			{
				const ArrayType* at = dynamic_cast< const ArrayType*>(type);
				const ArrayType* this_at = dynamic_cast< const ArrayType*>(this);
				return at==this_at;
			}
			case TypeTag::Constructor:
			case TypeTag::Enum:
			case TypeTag::Method:
			case TypeTag::Struct:
			case TypeTag::Unknown:
				return type == this;
			case TypeTag::Void:
				return false;
			case TypeTag::Ptr:
			{
				const PtrType* at = dynamic_cast< const PtrType*>(type);
				const PtrType* this_at = dynamic_cast< const PtrType*>(this);
				if (at && this_at) return this_at->BasicType->IsType(at->BasicType);
				return false;
			}			
			case TypeTag::Ref:
			{
				const RefType* at = dynamic_cast< const RefType*>(type);
				const RefType* this_at = dynamic_cast< const RefType*>(this);
				if (at && this_at) return this_at->BasicType->IsType(at->BasicType);
				return false;
			}
			case TypeTag::Bool:
			case TypeTag::Char:
			case TypeTag::Double:
			case TypeTag::Float:
			case TypeTag::Int:
			case TypeTag::Long:
			case TypeTag::LongDouble:
			case TypeTag::LongLong:
			case TypeTag::Short:
			case TypeTag::UChar:
			case TypeTag::UInt:
			case TypeTag::ULong:
			case TypeTag::ULongLong:
			case TypeTag::UShort:
				return true;
			default:
				return false;
		}
	}
	return false;
}

bool Type::GetIsReference() const
{
	if (_tag == TypeTag::Modified)
	{
		const ModifiedType* mt = dynamic_cast<const ModifiedType*>(this);
		if (mt)
		{
			_type ht = mt->Hold;
			if (ht)
			{
				TypeTag tt = ht->_tag;
				if (tt == TypeTag::Modified)
				{
					return mt->GetIsReference();
				}
				else
					return tt == TypeTag::Ref;
			}
		}
		else
			return false;
	}
	return _tag == TypeTag::Ref;
}

bool Type::IsConvertible(_type type)const
{
	if (!type) return false;
	if (type->_tag == _tag)
	{
		switch (_tag)
		{
		case TypeTag::Array:
		case TypeTag::Constructor:
		case TypeTag::Enum:
		case TypeTag::Method:
		case TypeTag::Unknown:
		case TypeTag::Void:
			return false;
		case TypeTag::Struct:
		{
			const ClassInfo* at = dynamic_cast< const ClassInfo*>(type);
			const ClassInfo* this_at = dynamic_cast< const ClassInfo*>(this);
			if (at && this_at)
			{
				if (at == this_at) return true;
				return this_at->IsBaseOf(at);
			}
			return false;
		}
		case TypeTag::Ptr:
		{
			const PtrType* at = dynamic_cast< const PtrType*>(type);
			const PtrType* this_at = dynamic_cast< const PtrType*>(this);
			if (at && this_at) return this_at->BasicType->IsConvertible(at->BasicType);
			return false;
		}
		case TypeTag::Bool:
		case TypeTag::Char:
		case TypeTag::Double:
		case TypeTag::Float:
		case TypeTag::Int:
		case TypeTag::Long:
		case TypeTag::LongDouble:
		case TypeTag::LongLong:
		case TypeTag::Short:
		case TypeTag::UChar:
		case TypeTag::UInt:
		case TypeTag::ULong:
		case TypeTag::ULongLong:
		case TypeTag::UShort:
			return true;
		default:
			return false;
		}
	}
	else
		return type->IsScalar && IsScalar;
	return false;		
}