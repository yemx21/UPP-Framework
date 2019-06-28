#pragma once
#include "PropertyType.h"
#include "ConditionalAnyVisitor.h"

namespace System
{
	template<class __C, class __R, class __W >
	class MemberPropertyType_CR_CW : public PropertyType
	{
	public:
		typedef __R(__C::*readfuncptr)() const;
		typedef void (__C::*writefuncptr)(__W) const;
		readfuncptr readfunc;
		writefuncptr writefunc;
		MemberPropertyType_CR_CW(readfuncptr read, writefuncptr write)
		{
			readfunc = read;
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>(); 
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::All;
			kind = PropertyKinds::ConstReadWrite;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
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
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};

	template<class __C, class __R, class __W >
	class MemberPropertyType : public PropertyType
	{
	public:
		typedef __R(__C::*readfuncptr)();
		typedef void (__C::*writefuncptr)(__W);
		readfuncptr readfunc;
		writefuncptr writefunc;
		MemberPropertyType(readfuncptr read, writefuncptr write)
		{
			readfunc = read;
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::All; 
			kind = PropertyKinds::ReadWrite;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
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
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class __C, class __R, class __W >
	class MemberPropertyType_CR : public PropertyType
	{
	public:
		typedef __R(__C::*readfuncptr)() const;
		typedef void (__C::*writefuncptr)(__W);
		readfuncptr readfunc;
		writefuncptr writefunc;
		MemberPropertyType_CR(readfuncptr read, writefuncptr write)
		{
			readfunc = read;
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::All; 
			kind = PropertyKinds::ConstRead_Write;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
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
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class __C, class __R, class __W >
	class MemberPropertyType_CW : public PropertyType
	{
	public:
		typedef __R(__C::*readfuncptr)();
		typedef void (__C::*writefuncptr)(__W) const;
		readfuncptr readfunc;
		writefuncptr writefunc;
		MemberPropertyType_CW(readfuncptr read, writefuncptr write)
		{
			readfunc = read;
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::All;
			kind = PropertyKinds::ConstWrite_Read;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
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
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};


	template<class  __C, class __R>
	class MemberPropertyTypeR : public PropertyType
	{
	public:
		typedef __R(__C::*readfuncptr)();
		readfuncptr readfunc;
		MemberPropertyTypeR(readfuncptr read)
		{
			readfunc = read;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			policy = PropertyPolicy::ReadOnly; 
			kind = PropertyKinds::Read;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
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

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class __C, class __W >
	class MemberPropertyTypeW : public PropertyType
	{
	public:
		typedef void (__C::*writefuncptr)(__W);
		writefuncptr writefunc;
		MemberPropertyTypeW(writefuncptr write)
		{
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::WriteOnly; 
			kind = PropertyKinds::Write;
		}

		Any Get(Object* obj, bool nothrow = true) const override
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
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class  __C, class __R>
	class MemberPropertyTypeCR : public PropertyType
	{
	public:
		typedef __R(__C::*readfuncptr)() const;
		readfuncptr readfunc;
		MemberPropertyTypeCR(readfuncptr read)
		{
			readfunc = read;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			policy = PropertyPolicy::ReadOnly; 
			kind = PropertyKinds::ConstRead;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				__C* cobj = dynamic_cast<__C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
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

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				return (((cobj)->*readfunc)());
			}
			catch (...)
			{
				if (nothrow) return _missing; else throw;
			}
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class __C, class __W >
	class MemberPropertyTypeCW : public PropertyType
	{
	public:
		typedef void (__C::*writefuncptr)(__W) const;
		writefuncptr writefunc;
		MemberPropertyTypeCW(writefuncptr write)
		{
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::WriteOnly;
			kind = PropertyKinds::ConstWrite;
		}

		Any Get(Object* obj, bool nothrow = true) const override
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
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				if (!obj) throw std::exception("empty object");
				const __C* cobj = dynamic_cast<const __C*>(obj);
				if (!cobj) throw std::exception("invalid object");
				((cobj)->*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

	};


	template<class __C, class __R, class __W >
	class StaticMemberPropertyType : public PropertyType
	{
	public:
		typedef __R(*readfuncptr)();
		typedef void (*writefuncptr)(__W);
		readfuncptr readfunc;
		writefuncptr writefunc;
		StaticMemberPropertyType(readfuncptr read, writefuncptr write)
		{
			readfunc = read;
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::All;
			kind = PropertyKinds::StaticReadWrite;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				return (*readfunc)();
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
				(*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class  __C, class __R>
	class StaticMemberPropertyTypeR : public PropertyType
	{
	public:
		typedef __R(*readfuncptr)();
		readfuncptr readfunc;
		StaticMemberPropertyTypeR(readfuncptr read)
		{
			readfunc = read;
			ownerClass = TypeFactory::Create<__C>();
			readType = TypeFactory::Create<__R>();
			policy = PropertyPolicy::ReadOnly; 
			kind = PropertyKinds::StaticRead;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			try
			{
				return (*readfunc)();
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

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};

	template<class __C, class __W >
	class StaticMemberPropertyTypeW : public PropertyType
	{
	public:
		typedef void (*writefuncptr)(__W);
		writefuncptr writefunc;
		StaticMemberPropertyTypeW(writefuncptr write)
		{
			writefunc = write;
			ownerClass = TypeFactory::Create<__C>();
			writeType = TypeFactory::Create<__W>();
			policy = PropertyPolicy::WriteOnly; 
			kind = PropertyKinds::StaticWrite;
		}

		Any Get(Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("writeonly");
		}

		bool Set(Object* obj, const Any& value, bool nothrow = true) const override
		{
			try
			{
				(*writefunc)(AnyVisitor<__W>::Get(value));
				return true;
			}
			catch (...)
			{
				if (nothrow) return false; else throw;
			}
		}

		Any Get(const Object* obj, bool nothrow = true) const override
		{
			if (nothrow) return _missing; else throw std::exception("not implemented");
		}

		bool Set(const Object* obj, const Any& value, bool nothrow = true) const override
		{
			if (nothrow) return false; else throw std::exception("not implemented");
		}

	};



	template<class __C, typename __R, typename __W>
	inline PropertyType* PropertyTypeOf(__R(__C::*readfunc)(), void(__C::*writefunc)(__W))
	{
		static_assert(std::is_same<typename deduce<__R>::type, typename deduce<__W>::type>::value, "read and write function must have the same deduced type");
		if (readfunc && writefunc)
			return new MemberPropertyType<__C, __R, __W>(readfunc, writefunc);
		else if (!readfunc)
			return new MemberPropertyTypeR<__C, __R>(readfunc);
		else if (!writefunc)
			return new MemberPropertyTypeW<__C, __W>(writefunc);
		else
			return nullptr;
	}

	template<class __C, typename __R, typename __W>
	inline PropertyType* PropertyTypeOf(__R(__C::*readfunc)() const, void(__C::*writefunc)(__W))
	{
		static_assert(std::is_same<typename deduce<__R>::type, typename deduce<__W>::type>::value, "read and write function must have the same deduced type");
		if (readfunc && writefunc)
			return new MemberPropertyType_CR<__C, __R, __W>(readfunc, writefunc);
		else if (!readfunc)
			return new MemberPropertyTypeCR<__C, __R>(readfunc);
		else if (!writefunc)
			return new MemberPropertyTypeW<__C, __W>(writefunc);
		else
			return nullptr;
	}

	template<class __C, typename __R, typename __W>
	inline PropertyType* PropertyTypeOf(__R(__C::*readfunc)(), void(__C::*writefunc)(__W) const)
	{
		static_assert(std::is_same<typename deduce<__R>::type, typename deduce<__W>::type>::value, "read and write function must have the same deduced type");
		if (readfunc && writefunc)
			return new MemberPropertyType_CW<__C, __R, __W>(readfunc, writefunc);
		else if (!readfunc)
			return new MemberPropertyTypeR<__C, __R>(readfunc);
		else if (!writefunc)
			return new MemberPropertyTypeCW<__C, __W>(writefunc);
		else
			return nullptr;
	}


	template<class __C, typename __R, typename __W>
	inline PropertyType* PropertyTypeOf(__R(__C::*readfunc)() const, void(__C::*writefunc)(__W) const)
	{
		static_assert(std::is_same<typename deduce<__R>::type, typename deduce<__W>::type>::value, "read and write function must have the same deduced type");
		if (readfunc && writefunc)
			return new MemberPropertyType_CR_CW<__C, __R, __W>(readfunc, writefunc);
		else if (!readfunc)
			return new MemberPropertyTypeCR<__C, __R>(readfunc);
		else if (!writefunc)
			return new MemberPropertyTypeCW<__C, __W>(writefunc);
		else
			return nullptr;
	}

	template<class __C, typename __R>
	inline PropertyType* PropertyTypeOf(__R(__C::*readfunc)())
	{
		return new MemberPropertyTypeR<__C, __R>(readfunc);
	}

	template<class __C, typename __W>
	inline PropertyType* PropertyTypeOf(void(__C::*writefunc)(__W))
	{
		return new MemberPropertyTypeW<__C, __W>(writefunc);
	}

	template<class __C, typename __R>
	inline PropertyType* PropertyTypeOf(__R(__C::*readfunc)() const)
	{
		return new MemberPropertyTypeCR<__C, __R>(readfunc);
	}

	template<class __C, typename __W>
	inline PropertyType* PropertyTypeOf(void(__C::*writefunc)(__W) const)
	{
		return new MemberPropertyTypeCW<__C, __W>(writefunc);
	}


	template<class __C, typename __R, typename __W>
	inline PropertyType* PropertyTypeOf(__R(*readfunc)(), void(*writefunc)(__W))
	{
		static_assert(std::is_same<typename deduce<__R>::type, typename deduce<__W>::type>::value, "read and write function must have the same deduced type");
		if (readfunc && writefunc)
			return new StaticMemberPropertyType<__C, __R, __W>(readfunc, writefunc);
		else if (!readfunc)
			return new StaticMemberPropertyTypeR<__C, __R>(readfunc);
		else if (!writefunc)
			return new StaticMemberPropertyTypeW<__C, __W>(writefunc);
		else
			return nullptr;
	}

	template<class __C, typename __R>
	inline PropertyType* PropertyTypeOf(__R(*readfunc)())
	{
		return new StaticMemberPropertyTypeR<__C, __R>(readfunc);
	}

	template<class __C, typename __W>
	inline PropertyType* PropertyTypeOf(void(*writefunc)(__W))
	{
		return new StaticMemberPropertyTypeW<__C, __W>(writefunc);
	}
}