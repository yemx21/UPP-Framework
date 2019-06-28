#pragma once
#include "FieldType.h"
#include "ConditionalAnyVisitor.h"


namespace System
{
	template<class __C, class __T>
	class MemberFieldType : public FieldType
	{
	public:
		mutable __T __C::* fieldptr;
		MemberFieldType(__T __C::* field)
		{
			fieldptr = field;
			ownerClass = TypeFactory::Create<__C>();
			fieldType = TypeFactory::Create<__T>();
			policy = FieldPolicy::All;
			kind = FieldKinds::ReadWrite;
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (cobj)->*(fieldptr);
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				(cobj)->*(fieldptr) = AnyVisitor<__T>::Get(value);
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};

	template<class __C, class __T>
	class MemberFieldTypeR : public FieldType
	{
	public:
		mutable __T __C::* fieldptr;
		MemberFieldTypeR(__T __C::* field)
		{
			fieldptr = field;
			ownerClass = TypeFactory::Create<__C>();
			fieldType = TypeFactory::Create<__T>();
			policy = FieldPolicy::ReadOnly;
			kind = FieldKinds::Read;
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (cobj)->*(fieldptr);
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("readonly");
		}
	};

	template<class __C, class __T>
	class MemberFieldTypeW : public FieldType
	{
	public:
		mutable __T __C::* fieldptr;
		MemberFieldTypeW(__T __C::* field)
		{
			fieldptr = field;
			ownerClass = TypeFactory::Create<__C>();
			fieldType = TypeFactory::Create<__T>();
			policy = FieldPolicy::WriteOnly;
			kind = FieldKinds::Write;
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("writeonly");
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				(cobj)->*(fieldptr) = AnyVisitor<__T>::Get(value);
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};

	template<class __C, typename __T>
	inline FieldType* FieldTypeOfAll(__T __C::* field)
	{
		return new MemberFieldType<__C, __T>(field);
	}

	template<class __C, typename __T>
	inline FieldType* FieldTypeOfReadOnly(__T __C::* field)
	{
		return new MemberFieldTypeR<__C, __T>(field);
	}

	template<class __C, typename __T>
	inline FieldType* FieldTypeOfWriteOnly(__T __C::* field)
	{
		return new MemberFieldTypeW<__C, __T>(field);
	}

	template<FieldPolicy Policy, class __C, typename __T>
	inline FieldType* FieldTypeOf(__T __C::* field)
	{
		if (Policy == FieldPolicy::All)
			return FieldTypeOfAll<__C, __T>(field);
		else if (Policy == FieldPolicy::ReadOnly)
			return FieldTypeOfReadOnly<__C, __T>(field);			
		else if (Policy == FieldPolicy::WriteOnly)
			return FieldTypeOfWriteOnly<__C, __T>(field);
	}

	template<class __C, class __T>
	class StaticMemberFieldType : public FieldType
	{
	public:
		mutable __T * fieldptr;
		StaticMemberFieldType(__T * field)
		{
			fieldptr = field;
			ownerClass = TypeFactory::Create<__C>();
			fieldType = TypeFactory::Create<__T>();
			policy = FieldPolicy::All;
			kind = FieldKinds::StaticReadWrite;
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return *(fieldptr);
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				*(fieldptr) = AnyVisitor<__T>::Get(value);
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};

	template<class __C, class __T>
	class StaticMemberFieldTypeR : public FieldType
	{
	public:
		mutable __T * fieldptr;
		StaticMemberFieldTypeR(__T * field)
		{
			fieldptr = field;
			ownerClass = TypeFactory::Create<__C>();
			fieldType = TypeFactory::Create<__T>();
			policy = FieldPolicy::ReadOnly;
			kind = FieldKinds::StaticRead;
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return *(fieldptr);
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("readonly");
		}
	};

	template<class __C, class __T>
	class StaticMemberFieldTypeW : public FieldType
	{
	public:
		mutable __T * fieldptr;
		StaticMemberFieldTypeW(__T * field)
		{
			fieldptr = field;
			ownerClass = TypeFactory::Create<__C>();
			fieldType = TypeFactory::Create<__T>();
			policy = FieldPolicy::WriteOnly;
			kind = FieldKinds::StaticWrite;
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("writeonly");
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				*(fieldptr) = AnyVisitor<__T>::Get(value);
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};

	template<class __C, typename __T>
	inline FieldType* FieldTypeOfAll(__T * field)
	{
		return new StaticMemberFieldType<__C, __T>(field);
	}

	template<class __C, typename __T>
	inline FieldType* FieldTypeOfReadOnly(__T * field)
	{
		return new StaticMemberFieldTypeR<__C, __T>(field);
	}

	template<class __C, typename __T>
	inline FieldType* FieldTypeOfWriteOnly(__T * field)
	{
		return new StaticMemberFieldTypeW<__C, __T>(field);
	}

	template<FieldPolicy Policy, class __C, typename __T>
	inline FieldType* FieldTypeOf(__T * field)
	{
		if (Policy == FieldPolicy::All)
			return FieldTypeOfAll<__C, __T>(field);
		else if (Policy == FieldPolicy::ReadOnly)
			return FieldTypeOfReadOnly<__C, __T>(field);
		else if (Policy == FieldPolicy::WriteOnly)
			return FieldTypeOfWriteOnly<__C, __T>(field);
	}
}