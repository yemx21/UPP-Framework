#include "ClassInfo.h"
#include "NameSpace.h"
#include "Reflection.h"
#include <string>
#include <vector>
#include <set>
#include <exception>
#include <memory>
#include <assert.h>
#include "Enumerators_impl.h"

using namespace System;


namespace System
{
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
		const ClassInfo* _info;
	public:
		explicit  jfsacasf(const wchar_t* name, Operators op, const ClassInfo* info) :_name(name), _op(op), _info(info)
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
		const ClassInfo* _info;
	public:
		explicit  ncedvsbu(Operators op, const ClassInfo* info) : _op(op), _info(info)
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

	struct ClassInfo_impl
	{
		bool isDirty;
		InertiaString name;
		InertiaString oname;
		unsigned int size;
		ClassFlags flag;
	};

	struct ClassInfo_internal : ZeroStruct<ClassInfo_internal>
	{
		ConstructorInfo* dci;
		std::vector<Attribute*>* ats;
		std::set<MethodInfo*>* mi;
		std::set<ConstructorInfo*>* ci;
		std::set<PropertyInfo*>* pi; 
		std::set<OperatorInfo*>* oi;
		std::set<FieldInfo*>* fi;
		std::vector<_type>* tc;
		std::set<const ClassInfo*>* bcls;

		static void LoopCountBaseClass(std::set<const ClassInfo*>* all, const ClassInfo* cls)
		{
			if (!cls || !all) return;
			for (auto& b : *cls->internal->bcls)
			{
				if (all->find(b) != all->end())
				{
					all->insert(b);
					LoopCountBaseClass(all, b);
				}
			}
		}

		~ClassInfo_internal()
		{
			if (ats)
			{
				for (auto& i : *ats)
				{
					Attribute* t = i;
					SAFE_DELETE(t);
				}
				ats->clear();
				delete ats;
				ats = nullptr;
			}
			if (mi)
			{
				for (auto& i : *mi)
				{
					MethodInfo* t = i;
					SAFE_DELETE(t);
				}
				mi->clear();
				delete mi;
				mi = nullptr;
			}
			if (ci)
			{
				for (auto& i : *ci)
				{
					ConstructorInfo* t = i;
					SAFE_DELETE(t);
				}
				ci->clear();
				delete ci;
				ci = nullptr;
			}
			if (pi)
			{
				for (auto& i : *pi)
				{
					PropertyInfo* t = i;
					SAFE_DELETE(t);
				}
				pi->clear();
				delete pi;
				pi = nullptr;
			}
			if (oi)
			{
				for (auto& i : *oi)
				{
					OperatorInfo* t = i;
					SAFE_DELETE(t);
				}
				oi->clear();
				delete oi;
				oi = nullptr;
			}
			if (fi)
			{
				for (auto& i : *fi)
				{
					FieldInfo* t = i;
					SAFE_DELETE(t);
				}
				fi->clear();
				delete fi;
				fi = nullptr;
			}
			if (tc)
			{
				tc->clear();
				delete tc;
				tc = nullptr;
			}
			if (bcls)
			{
				bcls->clear();
				delete bcls;
				bcls = nullptr;
			}			
			dci = nullptr;
		}
	};

	class BaseClassEnumerator_impl
	{
	protected:
		friend class ClassInfo;
		friend class BaseClassEnumerator;
		typedef std::set<const ClassInfo*> base;
		typedef std::set<const ClassInfo*>::const_iterator base_iterator;
		typedef std::set<const ClassInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		std::mutex lock;
		BaseClassEnumerator_impl(const ClassInfo_internal* cls)
		{
			for (auto& b : *cls->bcls)
			{
				if (iterOwner.find(b) != iterOwner.end()) continue;
				iterOwner.insert(b);
				ClassInfo_internal::LoopCountBaseClass(&iterOwner, b);
			}
			citer = iterOwner.begin();
		}

		unsigned int Count() const
		{
			return iterOwner.size();
		}

		const wchar_t* GetCurrentName() const
		{
			const ClassInfo* m = *citer;
			return m != nullptr ? m->Name : nullptr;
		}

		const ClassInfo* GetCurrent() const
		{
			return *citer;
		}

		const NameSpace* GetCurrentNameSpace() const
		{
			const ClassInfo* m = *citer;
			return m != nullptr ? m->NameSpace : nullptr;
		}

		const Assembly* GetCurrentAssembly() const
		{
			const ClassInfo* m = *citer;
			return m != nullptr ? m->Assembly : nullptr;
		}

		void Next()
		{
			if (citer == iterOwner.end()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer++;
		}

		void Previous()
		{
			if (citer == iterOwner.begin()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner.end();
		}
	};

	BaseClassEnumerator::BaseClassEnumerator() :impl(nullptr)
	{

	}

	BaseClassEnumerator::~BaseClassEnumerator()
	{
		if (impl){ delete impl; impl = nullptr; }
	}

	const wchar_t* BaseClassEnumerator::GetCurrentName() const
	{
		if (impl) return impl->GetCurrentName();
		return nullptr;
	}

	const ClassInfo* BaseClassEnumerator::GetCurrent() const
	{
		if (impl) return impl->GetCurrent();
		return nullptr;
	}

	const NameSpace* BaseClassEnumerator::GetCurrentNameSpace() const
	{
		if (impl) return impl->GetCurrentNameSpace();
		return nullptr;
	}

	const Assembly* BaseClassEnumerator::GetCurrentAssembly() const
	{
		if (impl) return impl->GetCurrentAssembly();
		return nullptr;
	}

	unsigned int BaseClassEnumerator::GetCount() const
	{
		return impl != nullptr ? impl->Count() : 0;
	}

	void BaseClassEnumerator::Next()
	{
		if (impl) impl->Next();
	}

	void BaseClassEnumerator::Previous()
	{
		if (impl) impl->Previous();
	}

	bool BaseClassEnumerator::GetIsEnd() const
	{
		if (impl) return impl->IsEnd();
		return true;
	}

}


void ClassInfo::Create(Type const*& pointer, DefineMetaFunc func, const wchar_t* name, unsigned int size, ClassFlags flags)
{
	if (pointer != nullptr)return;
	ClassInfo* type = new ClassInfo(name, size, flags);
	pointer = type;
	assert(func != nullptr);
	func(*type);
}


ClassInfo::ClassInfo(const wchar_t* name, unsigned int size, ClassFlags flags) : Type(TypeTag::Struct)
{
	this->space = nullptr;
	impl = new ClassInfo_impl();
	impl->oname = name;
	impl->name = name;
	impl->size = size;
	impl->flag = flags;
	impl->isDirty = false;

	internal = new ClassInfo_internal();
	internal->dci = nullptr;
	internal->ats = new std::vector<Attribute*>();
	internal->fi = new std::set<FieldInfo*>();
	internal->ci = new std::set<ConstructorInfo*>();
	internal->mi = new std::set<MethodInfo*>();
	internal->oi = new std::set<OperatorInfo*>();
	internal->pi = new std::set<PropertyInfo*>();
	internal->tc = new std::vector<_type>();
	internal->bcls = new std::set<const ClassInfo*>();
}

ClassInfo::~ClassInfo()
{
	SAFE_DELETE(impl);
	SAFE_DELETE(internal);
}


std::unique_ptr<FieldEnumerator> ClassInfo::GetFieldEnumerator() const
{
	FieldEnumerator* miter = new FieldEnumerator();
	miter->impl = new FieldEnumerator_impl(internal->fi);
	return std::unique_ptr<FieldEnumerator>(miter);
}

std::unique_ptr<BaseClassEnumerator> ClassInfo::GetBaseClassEnumerator() const
{
	BaseClassEnumerator* miter = new BaseClassEnumerator();
	miter->impl = new BaseClassEnumerator_impl(internal);
	return std::unique_ptr<BaseClassEnumerator>(miter);
}

std::unique_ptr<MethodEnumerator> ClassInfo::GetMethodEnumerator() const
{
	MethodEnumerator* miter = new MethodEnumerator();
	miter->impl = new MethodEnumerator_impl(internal->mi);
	return std::unique_ptr<MethodEnumerator>(miter);
}

std::unique_ptr<MethodReverseEnumerator> ClassInfo::GetMethodReverseEnumerator() const
{
	MethodReverseEnumerator* miter = new MethodReverseEnumerator();
	miter->impl = new MethodReverseEnumerator_impl(internal->mi);
	return std::unique_ptr<MethodReverseEnumerator>(miter);
}

std::unique_ptr<PropertyEnumerator> ClassInfo::GetPropertyEnumerator() const
{
	PropertyEnumerator* miter = new PropertyEnumerator();
	miter->impl = new PropertyEnumerator_impl(internal->pi);
	return std::unique_ptr<PropertyEnumerator>(miter);
}

std::unique_ptr<PropertyReverseEnumerator> ClassInfo::GetPropertyReverseEnumerator() const
{
	PropertyReverseEnumerator* miter = new PropertyReverseEnumerator();
	miter->impl = new PropertyReverseEnumerator_impl(internal->pi);
	return std::unique_ptr<PropertyReverseEnumerator>(miter);
}

std::unique_ptr<ConstructorEnumerator> ClassInfo::GetConstructorEnumerator() const
{
	ConstructorEnumerator* miter = new ConstructorEnumerator();
	miter->impl = new ConstructorEnumerator_impl(internal->ci);
	return std::unique_ptr<ConstructorEnumerator>(miter);
}

std::unique_ptr<ConstructorReverseEnumerator> ClassInfo::GetConstructorReverseEnumerator() const
{
	ConstructorReverseEnumerator* miter = new ConstructorReverseEnumerator();
	miter->impl = new ConstructorReverseEnumerator_impl(internal->ci);
	return std::unique_ptr<ConstructorReverseEnumerator>(miter);
}

std::unique_ptr<OperatorEnumerator> ClassInfo::GetOperatorEnumerator() const
{
	OperatorEnumerator* miter = new OperatorEnumerator();
	miter->impl = new OperatorEnumerator_impl(internal->oi);
	return std::unique_ptr<OperatorEnumerator>(miter);
}

std::unique_ptr<OperatorReverseEnumerator> ClassInfo::GetOperatorReverseEnumerator() const
{
	OperatorReverseEnumerator* miter = new OperatorReverseEnumerator();
	miter->impl = new OperatorReverseEnumerator_impl(internal->oi);
	return std::unique_ptr<OperatorReverseEnumerator>(miter);
}

std::unique_ptr<AttributeEnumerator> ClassInfo::GetAttributeEnumerator() const
{
	AttributeEnumerator* miter = new AttributeEnumerator();
	miter->impl = new AttributeEnumerator_impl(internal->ats);
	return std::unique_ptr<AttributeEnumerator>(miter);
}

std::unique_ptr<AttributeReverseEnumerator> ClassInfo::GetAttributeReverseEnumerator() const
{
	AttributeReverseEnumerator* miter = new AttributeReverseEnumerator();
	miter->impl = new AttributeReverseEnumerator_impl(internal->ats);
	return std::unique_ptr<AttributeReverseEnumerator>(miter);
}

ClassInfoError ClassInfo::AddMethod(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams)
{
	try
	{
		if (!type) return ClassInfoError::EmptyArgument;
		MethodInfo* info = new MethodInfo(name, flags, type, defaultParams);
		if (internal)
		{
			auto res = internal->mi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return ClassInfoError::AlreadyExisted;
			}	
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;
}

ClassInfoError ClassInfo::AddDefaultConstructor_impl(const wchar_t* name, ConstructorFlags flags, ConstructorType* type)
{
	try
	{
		if (!type) return ClassInfoError::EmptyArgument;
		if (type->NumberOfParameters != 0) return ClassInfoError::InvalidArgument;
		ConstructorInfo* info = new ConstructorInfo(name, flags, type, nullptr);
		if (internal)
		{
			if (internal->dci) return ClassInfoError::DefaultConstructorExisted;
			auto res = internal->ci->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return ClassInfoError::AlreadyExisted;
			}
			internal->dci = info;
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;
}


ClassInfoError ClassInfo::AddConstructor(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams)
{
	try
	{
		if (!type) return ClassInfoError::EmptyArgument;
		ConstructorInfo* info = new ConstructorInfo(name, flags, type, defaultParams);	
		if (internal)
		{
			auto res = internal->ci->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return ClassInfoError::AlreadyExisted;
			}
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;
}

ClassInfoError ClassInfo::AddProperty(const wchar_t* name, PropertyFlags flags, PropertyType* type)
{
	try
	{
		if (!type) return ClassInfoError::EmptyArgument;
		PropertyInfo* info = new PropertyInfo(name, flags, type);
		if (internal)
		{
			auto res = internal->pi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return ClassInfoError::AlreadyExisted;
			}
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;
}

ClassInfoError ClassInfo::AddOperator(const wchar_t* name, OperatorType* type)
{
	try
	{
		if (!type) return ClassInfoError::EmptyArgument;
		OperatorInfo* info = new OperatorInfo(name, type);
		if (internal)
		{
			auto res = internal->oi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return ClassInfoError::AlreadyExisted;
			}
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;

}

ClassInfoError ClassInfo::AddAttribute(Attribute* attribute)
{
	try
	{
		if (!attribute) return ClassInfoError::EmptyArgument;
		if (internal)
		{
			internal->ats->push_back(attribute);
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;

}

ClassInfoError ClassInfo::AddBaseClass(_type type)
{
	try
	{
		if (!type) return ClassInfoError::EmptyArgument;
		if (type->Tag != TypeTag::Struct) return ClassInfoError::InvalidArgument;
		const ClassInfo* cls = dynamic_cast<const ClassInfo*>(type);
		if (!cls) return ClassInfoError::InvalidArgument;
		if (internal)
		{
			if (cls == this) return ClassInfoError::AlreadyExisted;
			for (auto& b : *internal->bcls)
			{
				if (b->NumberOfBaseClasses > 0)
				{
					if (b->IsBaseOf(cls)) return ClassInfoError::AlreadyExisted;
				}
				else
				{
					if (b == cls) return ClassInfoError::AlreadyExisted;
				}
			}
			auto res = internal->bcls->insert(cls);
			if (!res.second)
			{
				return ClassInfoError::AlreadyExisted;
			}
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;
}

ClassInfoError ClassInfo::AddField(const wchar_t* name, FieldFlags flags, FieldType* type)
{
	try
	{
		FieldInfo* info = new FieldInfo(name, flags, type);
		if (!info) return ClassInfoError::Unknown;
		if (internal)
		{
			auto res = internal->fi->insert(info);
			if (!res.second)
			{
				delete info;
				info = nullptr;
				return ClassInfoError::AlreadyExisted;
			}
			return ClassInfoError::OK;
		}
	}
	catch (...)
	{
		return ClassInfoError::Unknown;
	}
	return ClassInfoError::OK;

}

const wchar_t* ClassInfo::GetName() const
{
	if (internal &&impl)
	{
		size_t tcc = internal->tc->size();
		if (tcc == 0) return impl->oname->c_str();
		if (impl->isDirty)
		{		
			if (tcc == 0)
			{
				impl->name = impl->oname;
				return impl->name->c_str();
			}
			std::wstring nname = impl->oname;
			nname.push_back('<');
			tcc--;
			for (size_t icc = 0; icc <= tcc; icc++)
			{
				auto& c = internal->tc->at(icc);
				if (c) nname += c->Name;
				if (icc < tcc) nname.push_back(',');
			}
			nname.push_back('>');
			impl->name = nname;
			impl->isDirty = false;
			return impl->name->c_str();
		}
		else
		{
			return impl->name->c_str();
		}
	}

	return nullptr;
}

ClassFlags ClassInfo::GetFlags() const
{
	if (impl) return impl->flag;
	return ClassFlags::Normal;
}

const ConstructorInfo* ClassInfo::GetDefaultConstructor() const
{
	return internal != nullptr ? internal->dci : nullptr;
}

const PropertyInfo* ClassInfo::FindFirstProperty(const wchar_t* name, PropertyFlags flags) const
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


const ConstructorInfo* ClassInfo::FindFirstConstructor(const wchar_t* name, ConstructorFlags flags)const
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

const MethodInfo* ClassInfo::FindFirstMethod(const wchar_t* name, MethodFlags flags)const
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

const OperatorInfo* ClassInfo::FindFirstOperator(const wchar_t* name, Operators op, bool defaultDeclaration) const
{
	try
	{
		if (internal)
		{
			if (defaultDeclaration)
			{
				auto res = std::find_if(internal->oi->begin(), internal->oi->end(),  jfsacasf(name, op, this));
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

const OperatorInfo* ClassInfo::FindFirstOperator(Operators op, bool defaultDeclaration) const
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


const Attribute* ClassInfo::FindFirstAttribute(const wchar_t* name) const
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

const Attribute* ClassInfo::FindFirstAttribute(const wchar_t* name, AttributeFlags flag) const
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

const Attribute* ClassInfo::FindFirstAttribute(AttributeFlags flag) const
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

const FieldInfo* ClassInfo::FindFirstField(const wchar_t* name, FieldFlags flags) const
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

unsigned int ClassInfo::GetNumberOfMethods() const
{
	if (internal)
		if (internal->mi)
			return internal->mi->size();
	return 0;
}

unsigned int ClassInfo::GetNumberOfProperties() const
{
	if (internal)
	if (internal->mi)
		return internal->pi->size();
	return 0;
}

unsigned int ClassInfo::GetNumberOfFields() const
{
	if (internal)
	if (internal->mi)
		return internal->fi->size();
	return 0;
}

unsigned int ClassInfo::GetNumberOfOperators() const
{
	if (internal)
	if (internal->mi)
		return internal->oi->size();
	return 0;
}

unsigned int ClassInfo::GetNumberOfBaseClasses()const
{
	if (internal)
	if (internal->bcls)
	{
		std::set<const ClassInfo*> all;
		for (auto& b : *internal->bcls)
		{
			if (all.find(b) != all.end()) continue;
			all.insert(b);
			ClassInfo_internal::LoopCountBaseClass(&all, b);
		}
		return all.size();
	}
	return 0;
}

unsigned int ClassInfo::GetNumberOfAttributes() const
{
	if (internal)
	if (internal->mi)
		return internal->ats->size();
	return 0;
}

unsigned int ClassInfo::GetNumberOfConstructors() const
{
	if (internal)
	if (internal->mi)
		return internal->ci->size();
	return 0;
}

bool ClassInfo::IsBaseOf(_type info)const
{
	try
	{
		const ClassInfo* cls = dynamic_cast<const ClassInfo*>(info);
		if (cls)
		{
			if (internal)
			{
				if (internal->bcls)
				{
					for (auto& b : *internal->bcls)
					{
						if (b->NumberOfBaseClasses > 0)
						{
							if (b == info) return true;
							if (b->IsBaseOf(info)) return true;
						}
						else
						{
							if (b == info) return true;
						}
					}
				}
			}
		}
	}
	catch (...){}
	return false;
}

unsigned int ClassInfo::GetSize() const
{
	if (impl) return impl->size;
	return 0;
}

const ConstructorInfo* ClassInfo::FindFirstConstructor(ConstructorPredicate predicate) const
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

const MethodInfo* ClassInfo::FindFirstMethod(MethodPredicate predicate)const
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

const PropertyInfo* ClassInfo::FindFirstProperty(PropertyPredicate predicate)const
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

const OperatorInfo* ClassInfo::FindFirstOperator(OperatorPredicate predicate)const
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

const Attribute* ClassInfo::FindFirstAttribute(AttributePredicate predicate)const
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

const FieldInfo* ClassInfo::FindFirstField(FieldPredicate predicate)const
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

std::unique_ptr<ConstructorQueryEnumerator> ClassInfo::FindConstructors(ConstructorPredicate predicate)const
{
	if (!predicate) return nullptr;
	ConstructorQueryEnumerator* miter = new ConstructorQueryEnumerator();
	miter->impl = new ConstructorQueryEnumerator_impl(internal->ci, predicate);
	return std::unique_ptr<ConstructorQueryEnumerator>(miter);
}

std::unique_ptr<MethodQueryEnumerator> ClassInfo::FindMethods(MethodPredicate predicate)const
{
	if (!predicate) return nullptr;
	MethodQueryEnumerator* miter = new MethodQueryEnumerator();
	miter->impl = new MethodQueryEnumerator_impl(internal->mi, predicate);
	return std::unique_ptr<MethodQueryEnumerator>(miter);
}

std::unique_ptr<PropertyQueryEnumerator> ClassInfo::FindProperties(PropertyPredicate predicate)const
{
	if (!predicate) return nullptr;
	PropertyQueryEnumerator* miter = new PropertyQueryEnumerator();
	miter->impl = new PropertyQueryEnumerator_impl(internal->pi, predicate);
	return std::unique_ptr<PropertyQueryEnumerator>(miter);
}

std::unique_ptr<OperatorQueryEnumerator> ClassInfo::FindOperators(OperatorPredicate predicate)const
{
	if (!predicate) return nullptr;
	OperatorQueryEnumerator* miter = new OperatorQueryEnumerator();
	miter->impl = new OperatorQueryEnumerator_impl(internal->oi, predicate);
	return std::unique_ptr<OperatorQueryEnumerator>(miter);
}

std::unique_ptr<AttributeQueryEnumerator> ClassInfo::FindAttributes(AttributePredicate predicate)const
{
	if (!predicate) return nullptr;
	AttributeQueryEnumerator* miter = new AttributeQueryEnumerator();
	miter->impl = new AttributeQueryEnumerator_impl(internal->ats, predicate);
	return std::unique_ptr<AttributeQueryEnumerator>(miter);
}

std::unique_ptr<FieldQueryEnumerator> ClassInfo::FindFields(FieldPredicate predicate)const
{
	if (!predicate) return nullptr;
	FieldQueryEnumerator* miter = new FieldQueryEnumerator();
	miter->impl = new FieldQueryEnumerator_impl(internal->fi, predicate);
	return std::unique_ptr<FieldQueryEnumerator>(miter);
}


bool ClassInfo::InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow)const
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

bool ClassInfo::InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow)const
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

bool ClassInfo::DefineTemplateArgument(size_t index, _type argumentType)
{
	try
	{
		if (internal && impl)
		{
			if (internal->tc->size() < index + 1)
			{
				internal->tc->resize(index + 1, nullptr);
			}
			internal->tc->at(index) = argumentType;
			impl->isDirty = true;
			return true;
		}
		else
			return  false;
	}
	catch (...)
	{
		return false;
	}
}


bool ClassInfo::InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, nullptr_t obj, const OptionalVariadicAny& params, bool nothrow)const
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

bool ClassInfo::GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, Object* obj, bool nothrow)const
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

bool ClassInfo::GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, const Object* obj, bool nothrow)const
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

bool ClassInfo::GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, nullptr_t obj, bool nothrow)const
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

bool ClassInfo::SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, Object* obj, bool nothrow)const
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

bool ClassInfo::SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, const Object* obj, bool nothrow)const
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

bool ClassInfo::SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, nullptr_t obj, bool nothrow)const
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

bool ClassInfo::GetField(const wchar_t* name, FieldFlags flags, Any* result, nullptr_t obj, bool nothrow)const
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
					*result= mi->Get(obj, true);
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

bool ClassInfo::SetField(const wchar_t* name, FieldFlags flags, const Any& value, Object* obj, bool nothrow)const
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

bool ClassInfo::InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, Object* obj, const RealVariadicAny& params, bool nothrow) const
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

bool ClassInfo::InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow) const
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

bool ClassInfo::InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, nullptr_t obj, const RealVariadicAny& params, bool nothrow) const
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