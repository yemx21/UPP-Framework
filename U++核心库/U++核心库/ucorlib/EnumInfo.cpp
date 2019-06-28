#include "EnumInfo.h"
#include "Reflection.h"
#include <string>
#include <map>
#include <set>
#include <assert.h>
#include "NameSpace.h"
#include "Reflection.h"
#include <string>
#include <vector>
#include <set>
#include <exception>
#include <memory>
#include "Enumerators_impl.h"

using namespace System;

namespace System
{
	struct EnumInfo_impl_box
	{
		StandardInterger val;
		uhash code;
		template<class T>
		EnumInfo_impl_box(T v, uhash hashCode) :val(v), code(hashCode) {}
		EnumInfo_impl_box(const EnumInfo_impl_box& b) { val = b.val; code = b.code; }
	};

	struct EnumInfo_impl
	{
		InertiaString name;
		_type valT;
		bool valSigned;
		std::map<InertiaString, EnumInfo_impl_box> vals;
	};

	struct EnumInfo_internal
	{
		ConstructorInfo* dci;
		std::vector<Attribute*>* ats;
		std::set<MethodInfo*>* mi;
		std::set<ConstructorInfo*>* ci;
		std::set<PropertyInfo*>* pi;
		std::set<OperatorInfo*>* oi;
		std::set<FieldInfo*>* fi;
	};

	class EnumEnumerator_impl
	{
	protected:
		friend class EnumInfo;
		friend class EnumEnumerator;
		typedef std::map<InertiaString, EnumInfo_impl_box>* base;
		typedef std::map<InertiaString, EnumInfo_impl_box>::const_iterator base_iterator;
		typedef std::map<InertiaString, EnumInfo_impl_box>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		std::mutex lock;
		EnumEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		size_t Count() const
		{
			return iterOwner->size();
		}

		std::map<InertiaString, EnumInfo_impl_box>::const_reference Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == iterOwner->end()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer++;
		}

		void Previous()
		{
			if (citer == iterOwner->begin()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class EnumReverseEnumerator_impl
	{
	protected:
		friend class EnumInfo;
		friend class EnumReverseEnumerator;
		typedef std::map<InertiaString, EnumInfo_impl_box>* base;
		typedef std::map<InertiaString, EnumInfo_impl_box>::const_reverse_iterator base_iterator;
		typedef std::map<InertiaString, EnumInfo_impl_box>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		std::mutex lock;
		EnumReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		std::map<InertiaString, EnumInfo_impl_box>::const_reference Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == iterOwner->rend()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer++;
		}

		void Previous()
		{
			if (citer == iterOwner->rbegin()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class cn32798csd
	{
	private:
		const wchar_t* _name;
		MethodFlags _flags;
		MethodType* _type;
	public:
		explicit cn32798csd(const wchar_t* name, MethodFlags flags, MethodType* type) :_name(name), _flags(flags), _type(type)
		{
		}
		bool operator() (MethodInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				if (wcscmp(_name, iname) == 0)
				if (_flags == info->GetFlags())
					return  _type == info->GetType();
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class cw12ey3ca
	{
	private:
		const wchar_t* _name;
		ConstructorFlags _flags;
	public:
		explicit cw12ey3ca(const wchar_t* name, ConstructorFlags flags) :_name(name), _flags(flags)
		{
		}

		bool operator() (ConstructorInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				if (wcscmp(_name, iname) == 0)
					return Enums::Contains(info->GetFlags(), _flags);
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class nfy3w32f
	{
	private:
		const wchar_t* _name;
		MethodFlags _flags;
	public:
		explicit nfy3w32f(const wchar_t* name, MethodFlags flags) :_name(name), _flags(flags)
		{
		}

		bool operator() (MethodInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				if (wcscmp(_name, iname) == 0)
					return Enums::Contains(info->GetFlags(), _flags);
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class pdneydfaefw
	{
	private:
		const wchar_t* _name;
		PropertyFlags _flags;
	public:
		explicit pdneydfaefw(const wchar_t* name, PropertyFlags flags) :_name(name), _flags(flags)
		{
		}

		bool operator() (PropertyInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				if (wcscmp(_name, iname) == 0)
					return Enums::Contains(info->GetFlags(), _flags);
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class vnyadewf
	{
	private:
		const wchar_t* _name;
		FieldFlags _flags;
	public:
		explicit vnyadewf(const wchar_t* name, FieldFlags flags) :_name(name), _flags(flags)
		{
		}

		bool operator() (FieldInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				if (wcscmp(_name, iname) == 0)
					return Enums::Contains(info->GetFlags(), _flags);
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class fnndfuiasfak
	{
	private:
		const wchar_t* _name;
		Operators _op;
	public:
		explicit fnndfuiasfak(const wchar_t* name, Operators op) :_name(name), _op(op)
		{
		}

		bool operator() (OperatorInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				if (wcscmp(_name, iname) == 0)
					return info->Operator == _op;
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class jfsacasf
	{
	private:
		const wchar_t* _name;
		Operators _op;
		const EnumInfo* _info;
	public:
		explicit  jfsacasf(const wchar_t* name, Operators op, const EnumInfo* info) :_name(name), _op(op), _info(info)
		{
		}

		bool operator() (OperatorInfo* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (iname && _name)
				{
					if (wcscmp(_name, info->GetName()) == 0)
					{
						if (info->Operator == _op)
						{
							auto vv = info->Type;
							if (vv)
							{
								auto pts = vv->ParameterTypes;
								if (pts)
								{
									return false;
									//auto ft = pts->at(1);
									//if (ft)
									//{
									//	return ft->IsType(_info);
									//}
								}
							}
						}
					}
				}
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class ld3uda
	{
	private:
		Operators _op;
	public:
		explicit ld3uda(Operators op) : _op(op)
		{
		}

		bool operator() (OperatorInfo* info)
		{
			try
			{
				if (!info) return false;
				return info->Operator == _op;
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class ncedvsbu
	{
	private:
		Operators _op;
		const EnumInfo* _info;
	public:
		explicit  ncedvsbu(Operators op, const EnumInfo* info) : _op(op), _info(info)
		{
		}

		bool operator() (OperatorInfo* info)
		{
			try
			{
				if (!info) return false;
				if (info->Operator == _op)
				{
					auto vv = info->Type;
					if (vv)
					{
						auto pts = vv->ParameterTypes;
						if (pts)
						{
							return false;
							//auto ft = pts->at(1);
							//if (ft)
							//{
							//	return ft->IsType(_info);
							//}
						}
					}
				}
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};

	class xvxtwsads
	{
	private:
		const wchar_t* _name;
		AttributeFlags _flag;
	public:
		explicit xvxtwsads(const wchar_t* name, AttributeFlags flag) : _name(name), _flag(flag)
		{
		}

		bool operator() (Attribute* info)
		{
			if (!info) return false;
			auto iname = info->Name;
			if (_name && iname)
			if (wcscmp(_name, iname) == 0)
				return info->Flag == _flag;
			return false;
		}
	};

	class ldbwydwe
	{
	private:
		AttributeFlags _flag;
	public:
		explicit ldbwydwe(AttributeFlags flag) : _flag(flag)
		{
		}

		bool operator() (Attribute* info)
		{
			if (!info) return false;
			return info->Flag == _flag;
		}
	};

	class zdaferw
	{
	private:
		const wchar_t* _name;
	public:
		explicit zdaferw(const wchar_t* name) : _name(name)
		{
		}

		bool operator() (Attribute* info)
		{
			if (!info) return false;
			auto iname = info->Name;
			if (_name && iname)
				return wcscmp(_name, iname) == 0;
			return false;
		}
	};
}

EnumEnumerator::EnumEnumerator() :impl(nullptr)
{

}

EnumEnumerator::~EnumEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

std::wstring EnumEnumerator::GetCurrentName() const
{
	if (impl) return impl->Current().first;
	return std::wstring();
}

StandardInterger EnumEnumerator::GetCurrentValue() const
{
	if (impl) return impl->Current().second.val;
	return 0;
}

uhash EnumEnumerator::GetCurrentHashCode() const
{
	if (impl) return impl->Current().second.code;
	return 0u;
}

size_t EnumEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void EnumEnumerator::Next()
{
	if (impl) impl->Next();
}

void EnumEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool EnumEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


EnumReverseEnumerator::EnumReverseEnumerator() :impl(nullptr)
{

}

EnumReverseEnumerator::~EnumReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

std::wstring EnumReverseEnumerator::GetCurrentName() const
{
	if (impl) return impl->Current().first;
	return std::wstring();
}

StandardInterger EnumReverseEnumerator::GetCurrentValue() const
{
	if (impl) return impl->Current().second.val;
	return 0;
}

uhash EnumReverseEnumerator::GetCurrentHashCode() const
{
	if (impl) return impl->Current().second.code;
	return 0u;
}

size_t EnumReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void EnumReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void EnumReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool EnumReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


void EnumInfo::Create(Type const*& pointer, DefineMetaFunc func, const wchar_t* name, _type valueType)
{
	if (pointer != nullptr)return;
	EnumInfo* type = new EnumInfo(name, valueType);
	pointer = type;
	assert(func != nullptr);
	func(*type);
}

EnumInfo::EnumInfo(const wchar_t* name, _type valueType) : Type(TypeTag::Enum)
{
	this->space = nullptr;
	impl = new EnumInfo_impl();
	impl->name = name;
	impl->valT = valueType;
	impl->valSigned = ValueType->IsSigned;

	internal = new EnumInfo_internal();
	internal->dci = nullptr;
	internal->ats = new std::vector<Attribute*>();
	internal->fi = new std::set<FieldInfo*>();
	internal->ci = new std::set<ConstructorInfo*>();
	internal->mi = new std::set<MethodInfo*>();
	internal->oi = new std::set<OperatorInfo*>();
	internal->pi = new std::set<PropertyInfo*>();

}

class map_value_finder
{
public:
	map_value_finder(const StandardInterger& val, bool issigned) :value(val), isSigned(issigned){}
	bool operator ()(const std::map<InertiaString, EnumInfo_impl_box>::value_type &pair)
	{
		return isSigned ? value.Compare<Int64>(pair.second.val) : value.Compare<UInt64>(pair.second.val);
	}
private:
	StandardInterger  value;
	bool isSigned;
};

class map_hashcode_finder
{
public:
	map_hashcode_finder(uhash val) : code(val) {}
	bool operator ()(const std::map<InertiaString, EnumInfo_impl_box>::value_type &pair)
	{
		return pair.second.code == code;
	}
private:
	uhash code;
};

_type EnumInfo::GetValueType() const
{
	if (impl) return impl->valT;
	return false;
}

bool EnumInfo::TryGetValueName(const StandardInterger& value, std::wstring& result) const
{
	if (impl)
	{
		auto val = find_if(impl->vals.begin(), impl->vals.end(), map_value_finder(value, impl->valSigned));
		if (val != impl->vals.end())
		{
			result = val->first;
			return true;
		}
	}
	return false;
}

bool EnumInfo::TryGetValueName(_type enumType, const StandardInterger& value, std::wstring& result)
{
	if (!enumType->IsEnum) return false;
	try
	{
		const EnumInfo* ei = dynamic_cast<const EnumInfo*>(enumType);
		if (ei)
		{
			return ei->TryGetValueName(value, result);
		}
		return false;
	}
	catch (...)
	{
		return false;
	}
}

bool EnumInfo::TryParse(uhash hashCode, StandardInterger& result) const
{
	if (impl)
	{
		auto val = find_if(impl->vals.begin(), impl->vals.end(), map_hashcode_finder(hashCode));
		if (val != impl->vals.end())
		{
			result = val->second.val;
			return true;
		}
	}
	return false;
}

bool EnumInfo::TryGetHashCode(const StandardInterger& value, uhash& result) const
{
	if (impl)
	{
		auto val = find_if(impl->vals.begin(), impl->vals.end(), map_value_finder(value, impl->valSigned));
		if (val != impl->vals.end())
		{
			result = val->second.code;
			return true;
		}
	}
	return false;
}

bool EnumInfo::TryGetHashCode(_type enumType, const StandardInterger& value, uhash& result)
{
	if (!enumType->IsEnum) return false;
	try
	{
		const EnumInfo* ei = dynamic_cast<const EnumInfo*>(enumType);
		if (ei)
		{
			return ei->TryGetHashCode(value, result);
		}
		return false;
	}
	catch (...)
	{
		return false;
	}
}

EnumInfoError EnumInfo::InternalAddValue(const wchar_t* valueName, const StandardInterger& value, uhash hashCode)
{
	if (impl)
	{
		auto iter = impl->vals.find(InertiaString(valueName));
		if (iter != impl->vals.end())
		{
			return EnumInfoError::AlreadyExisted;
		}
		else
		{
			impl->vals.insert(std::pair<InertiaString, EnumInfo_impl_box>(InertiaString(valueName), EnumInfo_impl_box(value, hashCode)));
			return EnumInfoError::OK;
		}
	}
	return EnumInfoError::Unknown;
}

size_t EnumInfo::GetCount() const
{
	if (impl) return impl->vals.size(); else return 0;
}

const wchar_t* EnumInfo::GetName() const
{
	if (impl) return impl->name.get().c_str(); else return nullptr;
}


 bool EnumInfo::TryParse(const wchar_t* valueName, StandardInterger& result) const
{
	if (impl)
	{
		auto val= impl->vals.find(InertiaString(valueName));
		if (val != impl->vals.end())
		{
			result = val->second.val;
			return true;
		}
	}
	return false;
}

std::unique_ptr<EnumEnumerator> EnumInfo::GetEnumEnumerator() const
{
	EnumEnumerator* miter = new EnumEnumerator();
	miter->impl = new EnumEnumerator_impl(&impl->vals);
	return std::unique_ptr<EnumEnumerator>(miter);
}

std::unique_ptr<EnumReverseEnumerator> EnumInfo::GetEnumReverseEnumerator() const
{
	EnumReverseEnumerator* miter = new EnumReverseEnumerator();
	miter->impl = new EnumReverseEnumerator_impl(&impl->vals);
	return std::unique_ptr<EnumReverseEnumerator>(miter);
}

std::unique_ptr<FieldEnumerator> EnumInfo::GetFieldEnumerator() const
{
	FieldEnumerator* miter = new FieldEnumerator();
	miter->impl = new FieldEnumerator_impl(internal->fi);
	return std::unique_ptr<FieldEnumerator>(miter);
}

std::unique_ptr<FieldReverseEnumerator> System::EnumInfo::GetFieldReverseEnumerator() const
{
	FieldReverseEnumerator* miter = new FieldReverseEnumerator();
	miter->impl = new FieldReverseEnumerator_impl(internal->fi);
	return std::unique_ptr<FieldReverseEnumerator>(miter);
}

std::unique_ptr<MethodEnumerator> EnumInfo::GetMethodEnumerator() const
{
	MethodEnumerator* miter = new MethodEnumerator();
	miter->impl = new MethodEnumerator_impl(internal->mi);
	return std::unique_ptr<MethodEnumerator>(miter);
}

std::unique_ptr<MethodReverseEnumerator> EnumInfo::GetMethodReverseEnumerator() const
{
	MethodReverseEnumerator* miter = new MethodReverseEnumerator();
	miter->impl = new MethodReverseEnumerator_impl(internal->mi);
	return std::unique_ptr<MethodReverseEnumerator>(miter);
}

std::unique_ptr<PropertyEnumerator> EnumInfo::GetPropertyEnumerator() const
{
	PropertyEnumerator* miter = new PropertyEnumerator();
	miter->impl = new PropertyEnumerator_impl(internal->pi);
	return std::unique_ptr<PropertyEnumerator>(miter);
}

std::unique_ptr<PropertyReverseEnumerator> EnumInfo::GetPropertyReverseEnumerator() const
{
	PropertyReverseEnumerator* miter = new PropertyReverseEnumerator();
	miter->impl = new PropertyReverseEnumerator_impl(internal->pi);
	return std::unique_ptr<PropertyReverseEnumerator>(miter);
}

std::unique_ptr<ConstructorEnumerator> EnumInfo::GetConstructorEnumerator() const
{
	ConstructorEnumerator* miter = new ConstructorEnumerator();
	miter->impl = new ConstructorEnumerator_impl(internal->ci);
	return std::unique_ptr<ConstructorEnumerator>(miter);
}

std::unique_ptr<ConstructorReverseEnumerator> EnumInfo::GetConstructorReverseEnumerator() const
{
	ConstructorReverseEnumerator* miter = new ConstructorReverseEnumerator();
	miter->impl = new ConstructorReverseEnumerator_impl(internal->ci);
	return std::unique_ptr<ConstructorReverseEnumerator>(miter);
}

std::unique_ptr<OperatorEnumerator> EnumInfo::GetOperatorEnumerator() const
{
	OperatorEnumerator* miter = new OperatorEnumerator();
	miter->impl = new OperatorEnumerator_impl(internal->oi);
	return std::unique_ptr<OperatorEnumerator>(miter);
}

std::unique_ptr<OperatorReverseEnumerator> EnumInfo::GetOperatorReverseEnumerator() const
{
	OperatorReverseEnumerator* miter = new OperatorReverseEnumerator();
	miter->impl = new OperatorReverseEnumerator_impl(internal->oi);
	return std::unique_ptr<OperatorReverseEnumerator>(miter);
}

std::unique_ptr<AttributeEnumerator> EnumInfo::GetAttributeEnumerator() const
{
	AttributeEnumerator* miter = new AttributeEnumerator();
	miter->impl = new AttributeEnumerator_impl(internal->ats);
	return std::unique_ptr<AttributeEnumerator>(miter);
}

std::unique_ptr<AttributeReverseEnumerator> EnumInfo::GetAttributeReverseEnumerator() const
{
	AttributeReverseEnumerator* miter = new AttributeReverseEnumerator();
	miter->impl = new AttributeReverseEnumerator_impl(internal->ats);
	return std::unique_ptr<AttributeReverseEnumerator>(miter);
}

EnumInfoError EnumInfo::AddMethod(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams)
{
	try
	{
		if (!type) return EnumInfoError::EmptyArgument;
		MethodInfo* info = new MethodInfo(name, flags, type, defaultParams);
		if (internal)
		{
			auto res = internal->mi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return EnumInfoError::AlreadyExisted;
			}
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;
}

EnumInfoError EnumInfo::AddDefaultConstructor(const wchar_t* name, ConstructorFlags flags, ConstructorType* type)
{
	try
	{
		if (!type) return EnumInfoError::EmptyArgument;
		ConstructorInfo* info = new ConstructorInfo(name, flags, type, nullptr);
		if (internal)
		{
			if (internal->dci) return EnumInfoError::DefaultConstructorExisted;
			auto res = internal->ci->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return EnumInfoError::AlreadyExisted;
			}
			internal->dci = info;
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;
}

EnumInfoError EnumInfo::AddConstructor(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams)
{
	try
	{
		if (!type) return EnumInfoError::EmptyArgument;
		ConstructorInfo* info = new ConstructorInfo(name, flags, type, defaultParams);
		if (internal)
		{
			auto res = internal->ci->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return EnumInfoError::AlreadyExisted;
			}
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;
}

EnumInfoError EnumInfo::AddProperty(const wchar_t* name, PropertyFlags flags, PropertyType* type)
{
	try
	{
		if (!type) return EnumInfoError::EmptyArgument;
		PropertyInfo* info = new PropertyInfo(name, flags, type);
		if (internal)
		{
			auto res = internal->pi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return EnumInfoError::AlreadyExisted;
			}
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;
}

EnumInfoError EnumInfo::AddOperator(const wchar_t* name, OperatorType* type)
{
	try
	{
		if (!type) return EnumInfoError::EmptyArgument;
		OperatorInfo* info = new OperatorInfo(name, type);
		if (internal)
		{
			auto res = internal->oi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return EnumInfoError::AlreadyExisted;
			}
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;

}

EnumInfoError EnumInfo::AddAttribute(Attribute* attribute)
{
	try
	{
		if (!attribute) return EnumInfoError::EmptyArgument;
		if (internal)
		{
			internal->ats->push_back(attribute);
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;

}

EnumInfoError EnumInfo::AddField(const wchar_t* name, FieldFlags flags, FieldType* type)
{
	try
	{
		FieldInfo* info = new FieldInfo(name, flags, type);
		if (!info) return EnumInfoError::Unknown;
		if (internal)
		{
			auto res = internal->fi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return EnumInfoError::AlreadyExisted;
			}
			return EnumInfoError::OK;
		}
	}
	catch (...)
	{
		return EnumInfoError::Unknown;
	}
	return EnumInfoError::OK;

}

EnumInfo::~EnumInfo()
{
	if (impl){ delete impl; impl = nullptr; }
	if (internal)
	{
		if (internal->ats)
		{
			for (auto iter = internal->ats->begin(); iter != internal->ats->end(); ++iter)
			{
				auto item = *iter;
				if (item) { delete item; item = nullptr; }
			}
		}

		if (internal->fi)
		{
			for (auto iter = internal->fi->begin(); iter != internal->fi->end(); ++iter)
			{
				auto item = *iter;
				if (item) { delete item; item = nullptr; }
			}
		}
		if (internal->ci)
		{
			for (auto iter = internal->ci->begin(); iter != internal->ci->end(); ++iter)
			{
				auto item = *iter;
				if (item) { delete item; item = nullptr; }
			}
		}
		if (internal->pi)
		{
			for (auto iter = internal->pi->begin(); iter != internal->pi->end(); ++iter)
			{
				auto item = *iter;
				if (item) { delete item; item = nullptr; }
			}
		}
		if (internal->oi)
		{
			for (auto iter = internal->oi->begin(); iter != internal->oi->end(); ++iter)
			{
				auto item = *iter;
				if (item) { delete item; item = nullptr; }
			}
		}
		if (internal->mi)
		{
			for (auto iter = internal->mi->begin(); iter != internal->mi->end(); ++iter)
			{
				auto item = *iter;
				if (item) { delete item; item = nullptr; }
			}
		}
	}
}

const ConstructorInfo* EnumInfo::GetDefaultConstructor() const
{
	return internal != nullptr ? internal->dci : nullptr;
}

const PropertyInfo* EnumInfo::FindFirstProperty(const wchar_t* name, PropertyFlags flags) const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const ConstructorInfo* EnumInfo::FindFirstConstructor(const wchar_t* name, ConstructorFlags flags)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->ci->begin(), internal->ci->end(), cw12ey3ca(name, flags));
			if (res != internal->ci->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const MethodInfo* EnumInfo::FindFirstMethod(const wchar_t* name, MethodFlags flags)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->mi->begin(), internal->mi->end(), nfy3w32f(name, flags));
			if (res != internal->mi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const OperatorInfo* EnumInfo::FindFirstOperator(const wchar_t* name, Operators op, bool defaultDeclaration) const
{
	try
	{
		if (internal)
		{
			if (defaultDeclaration)
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), jfsacasf(name, op, this));
				if (res != internal->oi->end())
				{
					return *res;
				}
				return nullptr;
			}
			else
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), fnndfuiasfak(name, op));
				if (res != internal->oi->end())
				{
					return *res;
				}
				return nullptr;
			}
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const OperatorInfo* EnumInfo::FindFirstOperator(Operators op, bool defaultDeclaration) const
{
	try
	{
		if (internal)
		{
			if (defaultDeclaration)
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), ncedvsbu(op, this));
				if (res != internal->oi->end())
				{
					return *res;
				}
				return nullptr;
			}
			else
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), ld3uda(op));
				if (res != internal->oi->end())
				{
					return *res;
				}
				return nullptr;
			}
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}


const Attribute* EnumInfo::FindFirstAttribute(const wchar_t* name) const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->ats->begin(), internal->ats->end(), zdaferw(name));
			if (res != internal->ats->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const Attribute* EnumInfo::FindFirstAttribute(const wchar_t* name, AttributeFlags flag) const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->ats->begin(), internal->ats->end(), xvxtwsads(name, flag));
			if (res != internal->ats->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const Attribute* EnumInfo::FindFirstAttribute(AttributeFlags flag) const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->ats->begin(), internal->ats->end(), ldbwydwe(flag));
			if (res != internal->ats->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const FieldInfo* EnumInfo::FindFirstField(const wchar_t* name, FieldFlags flags) const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->fi->begin(), internal->fi->end(), vnyadewf(name, flags));
			if (res != internal->fi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

unsigned int EnumInfo::GetNumberOfMethods() const
{
	if (internal)
	if (internal->mi)
		return internal->mi->size();
	return 0;
}

unsigned int EnumInfo::GetNumberOfProperties() const
{
	if (internal)
	if (internal->mi)
		return internal->pi->size();
	return 0;
}

unsigned int EnumInfo::GetNumberOfFields() const
{
	if (internal)
	if (internal->mi)
		return internal->fi->size();
	return 0;
}

unsigned int EnumInfo::GetNumberOfOperators() const
{
	if (internal)
	if (internal->mi)
		return internal->oi->size();
	return 0;
}

unsigned int EnumInfo::GetNumberOfAttributes() const
{
	if (internal)
	if (internal->mi)
		return internal->ats->size();
	return 0;
}

unsigned int EnumInfo::GetNumberOfConstructors() const
{
	if (internal)
	if (internal->mi)
		return internal->ci->size();
	return 0;
}

const ConstructorInfo* EnumInfo::FindFirstConstructor(ConstructorPredicate predicate) const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->ci->begin(), internal->ci->end(), predicate);
			if (res != internal->ci->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const MethodInfo* EnumInfo::FindFirstMethod(MethodPredicate predicate)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->mi->begin(), internal->mi->end(), predicate);
			if (res != internal->mi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const PropertyInfo* EnumInfo::FindFirstProperty(PropertyPredicate predicate)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), predicate);
			if (res != internal->pi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const OperatorInfo* EnumInfo::FindFirstOperator(OperatorPredicate predicate)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->oi->begin(), internal->oi->end(), predicate);
			if (res != internal->oi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const Attribute* EnumInfo::FindFirstAttribute(AttributePredicate predicate)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->ats->begin(), internal->ats->end(), predicate);
			if (res != internal->ats->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

const FieldInfo* EnumInfo::FindFirstField(FieldPredicate predicate)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->fi->begin(), internal->fi->end(), predicate);
			if (res != internal->fi->end())
			{
				return *res;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
	catch (...)
	{
		return nullptr;
	}
}

std::unique_ptr<ConstructorQueryEnumerator> EnumInfo::FindConstructors(ConstructorPredicate predicate)const
{
	if (!predicate) return nullptr;
	ConstructorQueryEnumerator* miter = new ConstructorQueryEnumerator();
	miter->impl = new ConstructorQueryEnumerator_impl(internal->ci, predicate);
	return std::unique_ptr<ConstructorQueryEnumerator>(miter);
}

std::unique_ptr<MethodQueryEnumerator> EnumInfo::FindMethods(MethodPredicate predicate)const
{
	if (!predicate) return nullptr;
	MethodQueryEnumerator* miter = new MethodQueryEnumerator();
	miter->impl = new MethodQueryEnumerator_impl(internal->mi, predicate);
	return std::unique_ptr<MethodQueryEnumerator>(miter);
}

std::unique_ptr<PropertyQueryEnumerator> EnumInfo::FindProperties(PropertyPredicate predicate)const
{
	if (!predicate) return nullptr;
	PropertyQueryEnumerator* miter = new PropertyQueryEnumerator();
	miter->impl = new PropertyQueryEnumerator_impl(internal->pi, predicate);
	return std::unique_ptr<PropertyQueryEnumerator>(miter);
}

std::unique_ptr<OperatorQueryEnumerator> EnumInfo::FindOperators(OperatorPredicate predicate)const
{
	if (!predicate) return nullptr;
	OperatorQueryEnumerator* miter = new OperatorQueryEnumerator();
	miter->impl = new OperatorQueryEnumerator_impl(internal->oi, predicate);
	return std::unique_ptr<OperatorQueryEnumerator>(miter);
}

std::unique_ptr<AttributeQueryEnumerator> EnumInfo::FindAttributes(AttributePredicate predicate)const
{
	if (!predicate) return nullptr;
	AttributeQueryEnumerator* miter = new AttributeQueryEnumerator();
	miter->impl = new AttributeQueryEnumerator_impl(internal->ats, predicate);
	return std::unique_ptr<AttributeQueryEnumerator>(miter);
}

std::unique_ptr<FieldQueryEnumerator> EnumInfo::FindFields(FieldPredicate predicate)const
{
	if (!predicate) return nullptr;
	FieldQueryEnumerator* miter = new FieldQueryEnumerator();
	miter->impl = new FieldQueryEnumerator_impl(internal->fi, predicate);
	return std::unique_ptr<FieldQueryEnumerator>(miter);
}

bool EnumInfo::InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->mi->begin(), internal->mi->end(), nfy3w32f(name, flags));
			if (res != internal->mi->end())
			{
				const MethodInfo* mi = *res;
				if (mi)
					return mi->Invoke(result, obj, params, nothrow);
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->mi->begin(), internal->mi->end(), nfy3w32f(name, flags));
			if (res != internal->mi->end())
			{
				const MethodInfo* mi = *res;
				if (mi)
					return mi->Invoke(result, obj, params, nothrow);
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, Object* obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				const PropertyInfo* mi = *res;
				if (mi)
				{
					*result = mi->Get(obj, true);
					return true;
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, const Object* obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				const PropertyInfo* mi = *res;
				if (mi)
				{
					*result = mi->Get(obj, true);
					return true;
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, nullptr_t obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				const PropertyInfo* mi = *res;
				if (mi)
				{
					*result = mi->Get(obj, true);
					return true;
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, Object* obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				const PropertyInfo* mi = *res;
				if (mi)
				{
					return mi->Set(obj, value, nothrow);
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, const Object* obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				const PropertyInfo* mi = *res;
				if (mi)
				{
					return mi->Set(obj, value, nothrow);
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, nullptr_t obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->pi->begin(), internal->pi->end(), pdneydfaefw(name, flags));
			if (res != internal->pi->end())
			{
				const PropertyInfo* mi = *res;
				if (mi)
				{
					return mi->Set(obj, value, nothrow);
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::GetField(const wchar_t* name, FieldFlags flags, Any* result, nullptr_t obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->fi->begin(), internal->fi->end(), vnyadewf(name, flags));
			if (res != internal->fi->end())
			{
				const FieldInfo* mi = *res;
				if (mi)
				{
					*result = mi->Get(obj, true);
					return true;
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::SetField(const wchar_t* name, FieldFlags flags, const Any& value, Object* obj, bool nothrow)const
{
	try
	{
		if (internal)
		{
			auto res = std::find_if(internal->fi->begin(), internal->fi->end(), vnyadewf(name, flags));
			if (res != internal->fi->end())
			{
				const FieldInfo* mi = *res;
				if (mi)
				{
					return mi->Set(obj, value, nothrow);
				}
				else
					return false;
			}
			return false;
		}
		else
			return  false;
	}
	catch (...)
	{
		if (nothrow)
			return false;
		else
			throw;
	}
}

bool EnumInfo::InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, Object* obj, const RealVariadicAny& params, bool nothrow) const
{
	try
	{
		if (internal)
		{
			if (defaultDeclaration)
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), jfsacasf(name, op, this));
				if (res != internal->oi->end())
				{
					const OperatorInfo* oi = *res;
					if (oi)
					{
						return oi->Invoke(result, obj, params, nothrow);
					}
					else
						return false;
				}
				return false;
			}
			else
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), fnndfuiasfak(name, op));
				if (res != internal->oi->end())
				{
					const OperatorInfo* oi = *res;
					if (oi)
					{
						return oi->Invoke(result, obj, params, nothrow);
					}
					else
						return false;
				}
				return  false;
			}
		}
		else
			return  false;
	}
	catch (...)
	{
		return  false;
	}
	return  false;
}

bool EnumInfo::InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow) const
{
	try
	{
		if (internal)
		{
			if (defaultDeclaration)
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), jfsacasf(name, op, this));
				if (res != internal->oi->end())
				{
					const OperatorInfo* oi = *res;
					if (oi)
					{
						return oi->Invoke(result, obj, params, nothrow);
					}
					else
						return false;
				}
				return  false;
			}
			else
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), fnndfuiasfak(name, op));
				if (res != internal->oi->end())
				{
					const OperatorInfo* oi = *res;
					if (oi)
					{
						return oi->Invoke(result, obj, params, nothrow);
					}
					else
						return false;
				}
				return false;
			}
		}
		else
			return  false;
	}
	catch (...)
	{
		return  false;
	}
	return false;
}

bool EnumInfo::InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, nullptr_t obj, const RealVariadicAny& params, bool nothrow) const
{
	try
	{
		if (internal)
		{
			if (defaultDeclaration)
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), jfsacasf(name, op, this));
				if (res != internal->oi->end())
				{
					const OperatorInfo* oi = *res;
					if (oi)
					{
						return oi->Invoke(result, obj, params, nothrow);
					}
					else
						return false;
				}
				return false;
			}
			else
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(), fnndfuiasfak(name, op));
				if (res != internal->oi->end())
				{
					const OperatorInfo* oi = *res;
					if (oi)
					{
						return oi->Invoke(result, obj, params, nothrow);
					}
					else
						return false;
				}
				return false;
			}
		}
		else
			return false;
	}
	catch (...)
	{
		return false;
	}
	return false;
}