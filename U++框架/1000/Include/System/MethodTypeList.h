#pragma once
#include "MethodType.h"
#include "ConditionalAnyVisitor.h"

namespace System
{
	template<class __C, class __RT>
	struct MethodTypeAdapter<-1, __C, __RT>
	{
		class VariadicAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(OptionalVariadicAny);
			funcptr func;
			VariadicAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		class ConstVariadicAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(OptionalVariadicAny) const;
			funcptr func;
			ConstVariadicAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

		};

		class VolatileVariadicAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(OptionalVariadicAny) volatile;
			funcptr func;
			VolatileVariadicAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

		};

		class ConstVolatileVariadicAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(OptionalVariadicAny) const volatile;
			funcptr func;
			ConstVolatileVariadicAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (((cobj)->*func)(params));
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

		};

		class StaticVariadicAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(const OptionalVariadicAny&);
			funcptr func;
			StaticVariadicAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = nullptr;
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (*func)(params);
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (*func)(params);
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (*func)(params);
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const
			{
				try
				{
					if (!result) return false;
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						*result = (*func)(params);
					}
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		inline MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&))
		{
			return new VariadicAdapter<__C, __RT>(func);
		}

		inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&) const)
		{
			return new ConstVariadicAdapter(func);
		}

		inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&) volatile)
		{
			return new VolatileVariadicAdapter(func);
		}

		inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&) const volatile)
		{
			return new ConstVolatileVariadicAdapter(func);
		}

		inline static MethodType* Create(__RT(*func)(const OptionalVariadicAny&))
		{
			return new StaticVariadicAdapter(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&))
			{
				return new VariadicAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&) const)
			{
				return new ConstVariadicAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&) volatile)
			{
				return new VolatileVariadicAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			inline static MethodType* Create(__RT(__C::*func)(const OptionalVariadicAny&) const volatile)
			{
				return new ConstVolatileVariadicAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			inline static MethodType* Create(__RT(*func)(const OptionalVariadicAny&))
			{
				return new StaticVariadicAdapter(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<0, __C, __RT>
	{
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)();
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)() const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)() volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)() const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)();
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = NULL;
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
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

		inline static MethodType* Create(__RT(__C::*func)())
		{
			return new Adapter(func);
		}

		inline static MethodType* Create(__RT(__C::*func)() const)
		{
			return new ConstAdapter(func);
		}

		inline static MethodType* Create(__RT(__C::*func)() volatile)
		{
			return new VolatileAdapter(func);
		}

		inline static MethodType* Create(__RT(__C::*func)() const volatile)
		{
			return new ConstVolatileAdapter(func);
		}

		inline static MethodType* Create(__RT(*func)())
		{
			return new StaticAdapter(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			inline static MethodType* Create(__RT(__C::*func)())
			{
				return new Adapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			inline static MethodType* Create(__RT(__C::*func)() const)
			{
				return new ConstAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			inline static MethodType* Create(__RT(__C::*func)() volatile)
			{
				return new VolatileAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			inline static MethodType* Create(__RT(__C::*func)() const volatile)
			{
				return new ConstVolatileAdapter(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			inline static MethodType* Create(__RT(*func)())
			{
				return new StaticAdapter(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<1, __C, __RT>
	{
		template<typename __P1>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		inline static MethodType* Create(__RT(__C::*func)(__P1))
		{
			return new Adapter<__P1>(func);
		}

		template<typename __P1>
		inline static MethodType* Create(__RT(__C::*func)(__P1) const)
		{
			return new ConstAdapter<__P1>(func);
		}

		template<typename __P1>
		inline static MethodType* Create(__RT(__C::*func)(__P1) volatile)
		{
			return new VolatileAdapter<__P1>(func);
		}

		template<typename __P1>
		inline static MethodType* Create(__RT(__C::*func)(__P1) const volatile)
		{
			return new ConstVolatileAdapter<__P1>(func);
		}

		template<typename __P1>
		inline static MethodType* Create(__RT(*func)(__P1))
		{
			return new StaticAdapter<__P1>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1>
			inline static MethodType* Create(__RT(__C::*func)(__P1))
			{
				return new Adapter<__P1>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1>
			inline static MethodType* Create(__RT(__C::*func)(__P1) const)
			{
				return new ConstAdapter<__P1>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1>
			inline static MethodType* Create(__RT(__C::*func)(__P1) volatile)
			{
				return new VolatileAdapter<__P1>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1>
			inline static MethodType* Create(__RT(__C::*func)(__P1) const volatile)
			{
				return new ConstVolatileAdapter<__P1>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1>
			inline static MethodType* Create(__RT(*func)(__P1))
			{
				return new StaticAdapter<__P1>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<2, __C, __RT>
	{
		template<typename __P1, typename __P2>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2))
		{
			return new Adapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2) const)
		{
			return new ConstAdapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2) volatile)
		{
			return new VolatileAdapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2>(func);
		}

		template<typename __P1, typename __P2>
		inline static MethodType* Create(__RT(*func)(__P1, __P2))
		{
			return new StaticAdapter<__P1, __P2>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2))
			{
				return new Adapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2) const)
			{
				return new ConstAdapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2) volatile)
			{
				return new VolatileAdapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2>
			inline static MethodType* Create(__RT(*func)(__P1, __P2))
			{
				return new StaticAdapter<__P1, __P2>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<3, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3))
		{
			return new Adapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3) const)
		{
			return new ConstAdapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3>(func);
		}

		template<typename __P1, typename __P2, typename __P3>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3))
		{
			return new StaticAdapter<__P1, __P2, __P3>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3))
			{
				return new Adapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3) const)
			{
				return new ConstAdapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3))
			{
				return new StaticAdapter<__P1, __P2, __P3>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<4, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4))
		{
			return new Adapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4))
			{
				return new Adapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<5, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<6, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<7, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__RT>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<8, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<9, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
			}
		};
	};

	template<class __C, class __RT>
	struct MethodTypeAdapter<10, __C, __RT>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class Adapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Normal;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class ConstAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const;
			funcptr func;
			ConstAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Const;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class VolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) volatile;
			funcptr func;
			VolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Volatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class ConstVolatileAdapter : public MethodType
		{
		public:
			typedef __RT(__C::*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const volatile;
			funcptr func;
			ConstVolatileAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::ConstVolatile;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					__C* cobj = dynamic_cast<__C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const __C* cobj = dynamic_cast<const __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					volatile __C* cobj = dynamic_cast<volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					if (!obj) return false;
					const volatile __C* cobj = dynamic_cast<const volatile __C*>(obj);
					if (!cobj) return false;
					*result = (((cobj)->*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9)));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class StaticAdapter : public MethodType
		{
		public:
			typedef __RT(*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10);
			funcptr func;
			StaticAdapter(funcptr f)
			{
				func = f;
				methodClass = TypeFactory::Create<__C>();
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
				kind = MethodKinds::Static;
			}

			bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}

			bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (!result) return false;
					*result = (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9));
					return true;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const)
		{
			return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) volatile)
		{
			return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const volatile)
		{
			return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
		{
			return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}

		template<MethodKinds kind>
		struct Detail;

		template<>
		struct Detail<MethodKinds::Normal>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
			{
				return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Const>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const)
			{
				return new ConstAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Volatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) volatile)
			{
				return new VolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::ConstVolatile>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static MethodType* Create(__RT(__C::*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10) const volatile)
			{
				return new ConstVolatileAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};

		template<>
		struct Detail<MethodKinds::Static>
		{
			template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
			inline static MethodType* Create(__RT(*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
			{
				return new StaticAdapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
			}
		};
	};


}