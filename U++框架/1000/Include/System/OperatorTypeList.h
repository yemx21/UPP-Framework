#pragma once
#include "OperatorType.h"
#include "VariadicAnyVisitor.h"

namespace System
{
	template<Operators __OP, class __C, class __RT>
	class OperatorTypeAdapter<-1, __OP, __C, __RT>
	{
		class VariadicOperatorType : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(const RealVariadicAny& anys);
			funcptr func;
			VariadicOperatorType(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				op = __OP;
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		class ConstVariadicOperatorType : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(const RealVariadicAny& anys) const;
			funcptr func;
			ConstVariadicOperatorType(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				op = __OP;
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

		};

		class VolatileVariadicOperatorType : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(const RealVariadicAny& anys) volatile;
			funcptr func;
			VolatileVariadicOperatorType(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				op = __OP;
				kind = OperatorKinds::CVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

		};

		class ConstVolatileVariadicOperatorType : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(const RealVariadicAny& anys) const volatile;
			funcptr func;
			ConstVolatileVariadicOperatorType(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				op = __OP;
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const  __C* cobj = dynamic_cast<const  __C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = ((cobj)->*func)(parameters);
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

		};

		inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&))
		{
			return new VariadicOperatorType<__OP, __C, __RT>(oper);
		}

		inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&) const)
		{
			return new ConstVariadicOperatorType<__OP, __C, __RT>(oper);
		}

		inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&) volatile)
		{
			return new VolatileVariadicOperatorType<__OP, __C, __RT>(oper);
		}

		inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&) const volatile)
		{
			return new ConstVolatileVariadicOperatorType<__OP, __C, __RT>(oper);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&))
			{
				return new VariadicOperatorType<__OP, __C, __RT>(oper);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&) const)
			{
				return new ConstVariadicOperatorType<__OP, __C, __RT>(oper);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&) volatile)
			{
				return new VolatileVariadicOperatorType<__OP, __C, __RT>(oper);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			inline static OperatorType* Create(__RT(__C::*oper)(const RealVariadicAny&) const volatile)
			{
				return new ConstVolatileVariadicOperatorType<__OP, __C, __RT>(oper);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<0, __OP, __C, __RT>
	{
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)();
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)() const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)() volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)() const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)());
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)();
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)();
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)();
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)();
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)();
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		inline static OperatorType* Create(__RT(__C::*func)())
		{
			return new Adapter(func);
		}

		inline static OperatorType* Create(__RT(__C::*func)() const)
		{
			return new ConstAdapter(func);
		}

		inline static OperatorType* Create(__RT(__C::*func)() volatile)
		{
			return new VolatileAdapter(func);
		}

		inline static OperatorType* Create(__RT(__C::*func)() const volatile)
		{
			return new ConstVolatileAdapter(func);
		}

		inline static OperatorType* Create(__RT(*func)())
		{
			return new StaticAdapter(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			inline static OperatorType* Create(__RT(__C::*func)())
			{
				return new Adapter(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			inline static OperatorType* Create(__RT(__C::*func)() const)
			{
				return new ConstAdapter(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			inline static OperatorType* Create(__RT(__C::*func)() volatile)
			{
				return new VolatileAdapter(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			inline static OperatorType* Create(__RT(__C::*func)() const volatile)
			{
				return new ConstVolatileAdapter(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			inline static OperatorType* Create(__RT(*func)())
			{
				return new StaticAdapter(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<1, __OP, __C, __RT>
	{
		template<typename __P1>
		class Adapter : public OperatorType
		{
			static_assert(!is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class ConstAdapter : public OperatorType
		{
			static_assert(!is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class VolatileAdapter : public OperatorType
		{
			static_assert(!is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(!is_operatorSupportedWithNoArg<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		inline static OperatorType* Create(__RT(__C::*func)(__P1))
		{
			return new Adapter<__P1>(func);
		}

		template<typename __P1>
		inline static OperatorType* Create(__RT(__C::*func)(__P1) const)
		{
			return new ConstAdapter<__P1>(func);
		}

		template<typename __P1>
		inline static OperatorType* Create(__RT(__C::*func)(__P1) volatile)
		{
			return new VolatileAdapter<__P1>(func);
		}

		template<typename __P1>
		inline static OperatorType* Create(__RT(__C::*func)(__P1) const volatile)
		{
			return new ConstVolatileAdapter<__P1>(func);
		}

		template<typename __P1>
		inline static OperatorType* Create(__RT(*func)(__P1))
		{
			return new StaticAdapter<__P1>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1>
			inline static OperatorType* Create(__RT(__C::*func)(__P1))
			{
				return new Adapter<__P1>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1>
			inline static OperatorType* Create(__RT(__C::*func)(__P1) const)
			{
				return new ConstAdapter<__P1>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1>
			inline static OperatorType* Create(__RT(__C::*func)(__P1) volatile)
			{
				return new VolatileAdapter<__P1>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1>
			inline static OperatorType* Create(__RT(__C::*func)(__P1) const volatile)
			{
				return new ConstVolatileAdapter<__P1>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1>
			inline static OperatorType* Create(__RT(*func)(__P1))
			{
				return new StaticAdapter<__P1>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<2, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2))
		{
			return new Adapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2) const)
		{
			return new ConstAdapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2) volatile)
		{
			return new VolatileAdapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2))
		{
			return new StaticAdapter<__P1, __P2>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2))
			{
				return new Adapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2) const)
			{
				return new ConstAdapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2) volatile)
			{
				return new VolatileAdapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2))
			{
				return new StaticAdapter<__P1, __P2>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<3, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3))
		{
			return new Adapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3) const)
		{
			return new ConstAdapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3))
		{
			return new StaticAdapter<__P1, __P2, __P3>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3))
			{
				return new Adapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3) const)
			{
				return new ConstAdapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3))
			{
				return new StaticAdapter<__P1, __P2, __P3>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<4, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4))
		{
			return new Adapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4))
			{
				return new Adapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<5, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<6, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<7, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<8, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(8);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(8);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(8);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(8);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(8);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<9, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(9);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(9);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(9);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(9);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(9);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};
	};

	template<Operators __OP, class __C, class __RT>
	struct OperatorTypeAdapter<10, __OP, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class Adapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(10);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				(*paramTypes)[9] = TypeFactory::Create<__P10>();
				kind = OperatorKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class ConstAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(10);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				(*paramTypes)[9] = TypeFactory::Create<__P10>();
				kind = OperatorKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class VolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(10);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				(*paramTypes)[9] = TypeFactory::Create<__P10>();
				kind = OperatorKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class ConstVolatileAdapter : public OperatorType
		{
			static_assert(is_operatorSupportedWithMoreThanOneArgs<__OP>::value, "unsupported Operator");
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(10);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				(*paramTypes)[9] = TypeFactory::Create<__P10>();
				kind = OperatorKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class StaticAdapter : public OperatorType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				operatorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(10);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
				(*paramTypes)[8] = TypeFactory::Create<__P9>();
				(*paramTypes)[9] = TypeFactory::Create<__P10>();
				kind = OperatorKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const RealVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(VariadicAnyVisitor<__P1>::Get(params, 0), VariadicAnyVisitor<__P2>::Get(params, 1), VariadicAnyVisitor<__P3>::Get(params, 2), VariadicAnyVisitor<__P4>::Get(params, 3), VariadicAnyVisitor<__P5>::Get(params, 4), VariadicAnyVisitor<__P6>::Get(params, 5), VariadicAnyVisitor<__P7>::Get(params, 6), VariadicAnyVisitor<__P8>::Get(params, 7), VariadicAnyVisitor<__P9>::Get(params, 8), VariadicAnyVisitor<__P10>::Get(params, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<OperatorKinds kind>
		struct Detail;

		template<>
		struct Detail<OperatorKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static OperatorType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<OperatorKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static OperatorType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};
	};


}