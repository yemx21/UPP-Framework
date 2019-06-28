#pragma once
#include "DefaultVariadicAny.h"
#include "OptionalVariadicAny.h"
namespace System
{
	template< typename T, bool isvoid = std::is_void<typename deduce<T>::type>::value, Modifiers modifier = DetectModifier<T>::modifier, Declarators declarator = DetectDeclarator<T>::declarator, size_t demesion = DetectDeclarator<T>::dimension>
	struct ConditionalAnyVisitor
	{
	};
#pragma region ConditionalAnyVisitor Void
	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 1U>
	{
	public:
		static void* Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->VoidCast(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.VoidCast(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 1U>
	{
	public:
		static const void* Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->VoidCast_Const(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.VoidCast_Const(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 1U>
	{
	public:
		static volatile void* Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->VoidCast_Volatile(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.VoidCast_Volatile(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 1U>
	{
	public:
		static const volatile void* Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Void2
	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 2U>
	{
	public:
		static void** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void2Cast(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void2Cast(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 2U>
	{
	public:
		static const void** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void2Cast_Const(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void2Cast_Const(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 2U>
	{
	public:
		static volatile void** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void2Cast_Volatile(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void2Cast_Volatile(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 2U>
	{
	public:
		static const volatile void** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void2Cast_ConstVolatile(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void2Cast_ConstVolatile(policy, index1, index2, index3);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Void3
	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 3U>
	{
	public:
		static void*** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void3Cast(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void3Cast(policy, index1, index2);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 3U>
	{
	public:
		static const void*** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void3Cast_Const(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void3Cast_Const(policy, index1, index2);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 3U>
	{
	public:
		static volatile void*** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void3Cast_Volatile(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void3Cast_Volatile(policy, index1, index2);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 3U>
	{
	public:
		static const volatile void*** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void3Cast_ConstVolatile(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void3Cast_ConstVolatile(policy, index1, index2);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Void4
	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 4U>
	{
	public:
		static void**** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void4Cast(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void4Cast(policy, index1);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 4U>
	{
	public:
		static const void**** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void4Cast_Const(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void4Cast_Const(policy, index1);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 4U>
	{
	public:
		static volatile void**** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void4Cast_Volatile(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void4Cast_Volatile(policy, index1);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 4U>
	{
	public:
		static const volatile void**** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void4Cast_ConstVolatile(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void4Cast_ConstVolatile(policy, index1);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Void5
	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 5U>
	{
	public:
		static void***** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void5Cast(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void5Cast(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 5U>
	{
	public:
		static const void***** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void5Cast_Const(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void5Cast_Const(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 5U>
	{
	public:
		static volatile void***** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void5Cast_Volatile(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void5Cast_Volatile(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 5U>
	{
	public:
		static const volatile void***** Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Void5Cast_ConstVolatile(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Void5Cast_ConstVolatile(policy);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Value
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::Value, 0U>
	{
		typedef typename deduce<T>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->ValueCast<base>(policy, index1, index2, index3, index4, index5);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else 
				return oany.ValueCast<base>(policy, index1, index2, index3, index4, index5);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::Value, 0U>
	{
		typedef typename std::add_const<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::Value, 0U>
	{
		typedef typename std::add_volatile<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Value, 0U>
	{
		typedef typename std::add_cv<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Pointer
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->PtrCast<base>(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.PtrCast<base>(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename std::add_const<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->PtrCast_Const<base>(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.PtrCast_Const<base>(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename std::add_cv<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Pointer2
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr2Cast<base>(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr2Cast<base>(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr2Cast_Const<base>(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr2Cast_Const<base>(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Pointer3
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr3Cast<base>(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr3Cast<base>(policy, index1, index2);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr3Cast_Const<base>(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr3Cast_Const<base>(policy, index1, index2);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr3Cast_Volatile<base>(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr3Cast_Volatile<base>(policy, index1, index2);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Pointer4
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr4Cast<base>(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr4Cast<base>(policy, index1);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr4Cast_Const<base>(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr4Cast_Const<base>(policy, index1);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr4Cast_Volatile<base>(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr4Cast_Volatile<base>(policy, index1);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr4Cast_ConstVolatile<base>(policy, index1);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr4Cast_ConstVolatile<base>(policy, index1);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor Pointer5
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr5Cast<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr5Cast<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr5Cast_Const<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr5Cast_Const<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr5Cast_Volatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr5Cast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->Ptr5Cast_ConstVolatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.Ptr5Cast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion


#pragma region ConditionalAnyVisitor LValueReference

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefCast<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefCast<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefCast_Const<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefCast_Const<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefCast_Volatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefCast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefCast_ConstVolatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefCast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor RValueReference
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefCast<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefCast<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefCast_Const<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefCast_Const<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefCast_Volatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefCast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefCast_ConstVolatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefCast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor LPointerReference

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::LValueReference, 1U>
	{
		typedef typename std::add_lvalue_reference<typename deduce<T>::type>::type base;
		typedef typename std::add_lvalue_reference<typename std::add_pointer<typename deduce<T>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefPtrCast<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefPtrCast<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::LValueReference, 1U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
		typedef typename std::add_lvalue_reference<typename std::add_const<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefPtrCast_Const<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefPtrCast_Const<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::LValueReference, 1U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
		typedef typename std::add_lvalue_reference<typename std::add_volatile<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefPtrCast_Volatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefPtrCast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::LValueReference, 1U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
		typedef typename std::add_lvalue_reference<typename std::add_cv<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->LRefPtrCast_ConstVolatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.LRefPtrCast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion

#pragma region ConditionalAnyVisitor RPointerReference
	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::None, Declarators::RValueReference, 1U>
	{
		typedef typename std::add_rvalue_reference<typename deduce<T>::type>::type base;
		typedef typename std::add_rvalue_reference<typename std::add_pointer<typename deduce<T>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefPtrCast<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefPtrCast<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Const, Declarators::RValueReference, 1U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
		typedef typename std::add_rvalue_reference<typename std::add_const<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefPtrCast_Const<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefPtrCast_Const<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::Volatile, Declarators::RValueReference, 1U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
		typedef typename std::add_rvalue_reference<typename std::add_volatile<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefPtrCast_Volatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefPtrCast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct ConditionalAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::RValueReference, 1U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
		typedef typename std::add_rvalue_reference<typename std::add_cv<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type result;
	public:
		static result Get(const OptionalVariadicAny& optionalAnys, SharedDefaultVariadicAny defaultAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			if (oany.IsMissing)
			{
				if (defaultAnys)
				{
					auto dany = defaultAnys->At(index);
					if (dany)
					{
						return dany->RRefPtrCast_ConstVolatile<base>(policy);
					}
					else
						throw std::exception("no default");
				}
				else
					throw std::exception("no default");
			}
			else
				return oany.RRefPtrCast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion


	struct ConditionalVariadicAnyVisitor
	{
	public:
		static bool Get(const OptionalVariadicAny* optionalAnys, SharedDefaultVariadicAny defaultAnys)
		{
			if (!optionalAnys) return false;
			if (defaultAnys)
			{
				size_t len = optionalAnys->Size;
				if (len == 0) return true;
				if (len >= defaultAnys->len)
				{
					for (size_t i = 0; i < defaultAnys->len; i++)
					{
						const Any& iter = optionalAnys->args.at(i);
						//if(iter.IsMissing) optionalAnys->Set(defaultAnys->params.at((unsigned int)i),i);
					}	
					return true;
				}
			}
			return true;
		}

	};
}