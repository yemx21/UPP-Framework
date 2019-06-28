#pragma once
#include "ConstructorType.h"
#include "ConditionalAnyVisitor.h"

namespace System
{
	template<class __C>
	class ConstructorTypeAdapter < -1, __C >
	{
		class VariadicAdapter : public ConstructorType
		{
		public:
			typedef Object*(*funcptr)(OptionalVariadicAny);
			funcptr func;
			VariadicAdapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = NULL;
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					if (ConditionalVariadicAnyVisitor::Get(&params, defaultParams))
					{
						return (*func)(params);
					}
					return false;
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		inline static ConstructorType* Create(Object*(*func)(const OptionalVariadicAny&))
		{
			return new VariadicAdapter(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<0, __C>
	{
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)();
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = NULL;
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)();
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		inline static ConstructorType* Create(Object* (*func)())
		{
			return new Adapter(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<1, __C>
	{
		template<typename __P1>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(1);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1>
		inline static ConstructorType* Create(Object* (*func)(__P1))
		{
			return new Adapter<__P1>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<2, __C>
	{
		template<typename __P1, typename __P2>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(2);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2))
		{
			return new Adapter<__P1, __P2>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<3, __C>
	{
		template<typename __P1, typename __P2, typename __P3>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(3);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3))
		{
			return new Adapter<__P1, __P2, __P3>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<4, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(4);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4))
		{
			return new Adapter<__P1, __P2, __P3, __P4>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<5, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4, __P5);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(5);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4, __P5))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<6, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(6);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4, __P5, __P6))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<7, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(7);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<8, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
				paramTypes = new TypeCollection(8);
				(*paramTypes)[0] = TypeFactory::Create<__P1>();
				(*paramTypes)[1] = TypeFactory::Create<__P2>();
				(*paramTypes)[2] = TypeFactory::Create<__P3>();
				(*paramTypes)[3] = TypeFactory::Create<__P4>();
				(*paramTypes)[4] = TypeFactory::Create<__P5>();
				(*paramTypes)[5] = TypeFactory::Create<__P6>();
				(*paramTypes)[6] = TypeFactory::Create<__P7>();
				(*paramTypes)[7] = TypeFactory::Create<__P8>();
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<9, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
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
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9>(func);
		}
	};

	template<class __C>
	struct ConstructorTypeAdapter<10, __C>
	{
		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		class Adapter : public ConstructorType
		{
		public:
			typedef Object* (*funcptr)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10);
			funcptr func;
			Adapter(funcptr f)
			{
				func = f;
				constructorClass = TypeFactory::Create<__C>();
				returnType = TypeFactory::Create<__C*>();
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
			}

			Object* Invoke(const OptionalVariadicAny& params, bool nothrow = true) const override
			{
				try
				{
					return (*func)(ConditionalAnyVisitor<__P1>::Get(params, defaultParams, 0), ConditionalAnyVisitor<__P2>::Get(params, defaultParams, 1), ConditionalAnyVisitor<__P3>::Get(params, defaultParams, 2), ConditionalAnyVisitor<__P4>::Get(params, defaultParams, 3), ConditionalAnyVisitor<__P5>::Get(params, defaultParams, 4), ConditionalAnyVisitor<__P6>::Get(params, defaultParams, 5), ConditionalAnyVisitor<__P7>::Get(params, defaultParams, 6), ConditionalAnyVisitor<__P8>::Get(params, defaultParams, 7), ConditionalAnyVisitor<__P9>::Get(params, defaultParams, 8), ConditionalAnyVisitor<__P10>::Get(params, defaultParams, 9));
				}
				catch (...)
				{
					if (nothrow) return false; else throw;
				}
			}
		};

		template<typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9, typename __P10>
		inline static ConstructorType* Create(Object* (*func)(__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10))
		{
			return new Adapter<__P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9, __P10>(func);
		}
	};

}