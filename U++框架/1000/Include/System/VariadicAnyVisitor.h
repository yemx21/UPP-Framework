#pragma once
#include "OptionalVariadicAny.h"
#include "VariadicAny.h"
#include "DefaultVariadicAny.h"
#include "RealVariadicAny.h"
namespace System
{
	template< typename T, bool isvoid = std::is_void<typename deduce<T>::type>::value, Modifiers modifier = DetectModifier<T>::modifier, Declarators declarator = DetectDeclarator<T>::declarator, size_t demesion = DetectDeclarator<T>::dimension>
	struct VariadicAnyVisitor
	{
	};
#pragma region VariadicAnyVisitor Void
	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 1U>
	{
	public:
		static void* Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.VoidCast(policy, index1, index2, index3, index4);
		}

		static void* Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast(policy, index1, index2, index3, index4);
		}

		static void* Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast(policy, index1, index2, index3, index4);
		}

		static void* Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->VoidCast(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 1U>
	{
	public:
		static const void* Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.VoidCast_Const(policy, index1, index2, index3, index4);
		}

		static const void* Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast_Const(policy, index1, index2, index3, index4);
		}

		static const void* Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast_Const(policy, index1, index2, index3, index4);
		}

		static const void* Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->VoidCast_Const(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 1U>
	{
	public:
		static volatile void* Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.VoidCast_Volatile(policy, index1, index2, index3, index4);
		}

		static volatile void* Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast_Volatile(policy, index1, index2, index3, index4);
		}

		static volatile void* Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast_Volatile(policy, index1, index2, index3, index4);
		}

		static volatile void* Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->VoidCast_Volatile(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 1U>
	{
	public:
		static const volatile void* Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
		}

		static const volatile void* Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
		}

		static const volatile void* Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
		}

		static const volatile void* Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Void2
	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 2U>
	{
	public:
		static void** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void2Cast(policy, index1, index2, index3);
		}

		static void** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast(policy, index1, index2, index3);
		}

		static void** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast(policy, index1, index2, index3);
		}

		static void** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void2Cast(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 2U>
	{
	public:
		static const void** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void2Cast_Const(policy, index1, index2, index3);
		}

		static const void** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast_Const(policy, index1, index2, index3);
		}

		static const void** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast_Const(policy, index1, index2, index3);
		}

		static const void** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void2Cast_Const(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 2U>
	{
	public:
		static volatile void** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void2Cast_Volatile(policy, index1, index2, index3);
		}

		static volatile void** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast_Volatile(policy, index1, index2, index3);
		}

		static volatile void** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast_Volatile(policy, index1, index2, index3);
		}

		static volatile void** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void2Cast_Volatile(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 2U>
	{
	public:
		static const volatile void** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void2Cast_ConstVolatile(policy, index1, index2, index3);
		}

		static const volatile void** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast_ConstVolatile(policy, index1, index2, index3);
		}

		static const volatile void** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void2Cast_ConstVolatile(policy, index1, index2, index3);
		}

		static const volatile void** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void2Cast_ConstVolatile(policy, index1, index2, index3);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Void3
	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 3U>
	{
	public:
		static void*** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void3Cast(policy, index1, index2);
		}

		static void*** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast(policy, index1, index2);
		}

		static void*** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast(policy, index1, index2);
		}

		static void*** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void3Cast(policy, index1, index2);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 3U>
	{
	public:
		static const void*** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void3Cast_Const(policy, index1, index2);
		}

		static const void*** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast_Const(policy, index1, index2);
		}

		static const void*** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast_Const(policy, index1, index2);
		}

		static const void*** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void3Cast_Const(policy, index1, index2);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 3U>
	{
	public:
		static volatile void*** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void3Cast_Volatile(policy, index1, index2);
		}

		static volatile void*** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast_Volatile(policy, index1, index2);
		}

		static volatile void*** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast_Volatile(policy, index1, index2);
		}

		static volatile void*** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void3Cast_Volatile(policy, index1, index2);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 3U>
	{
	public:
		static const volatile void*** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void3Cast_ConstVolatile(policy, index1, index2);
		}

		static const volatile void*** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast_ConstVolatile(policy, index1, index2);
		}

		static const volatile void*** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void3Cast_ConstVolatile(policy, index1, index2);
		}

		static const volatile void*** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void3Cast_ConstVolatile(policy, index1, index2);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Void4
	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 4U>
	{
	public:
		static void**** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void4Cast(policy, index1);
		}

		static void**** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast(policy, index1);
		}

		static void**** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast(policy, index1);
		}

		static void**** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void4Cast(policy, index1);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 4U>
	{
	public:
		static const void**** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void4Cast_Const(policy, index1);
		}

		static const void**** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast_Const(policy, index1);
		}

		static const void**** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast_Const(policy, index1);
		}

		static const void**** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void4Cast_Const(policy, index1);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 4U>
	{
	public:
		static volatile void**** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void4Cast_Volatile(policy, index1);
		}

		static volatile void**** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast_Volatile(policy, index1);
		}

		static volatile void**** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast_Volatile(policy, index1);
		}

		static volatile void**** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void4Cast_Volatile(policy, index1);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 4U>
	{
	public:
		static const volatile void**** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void4Cast_ConstVolatile(policy, index1);
		}

		static const volatile void**** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast_ConstVolatile(policy, index1);
		}

		static const volatile void**** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Void4Cast_ConstVolatile(policy, index1);
		}

		static const volatile void**** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void4Cast_ConstVolatile(policy, index1);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Void5
	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 5U>
	{
	public:
		static void***** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void5Cast(policy);
		}

		static void***** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast(policy);
		}

		static void***** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast(policy);
		}

		static void***** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void5Cast(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 5U>
	{
	public:
		static const void***** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void5Cast_Const(policy);
		}

		static const void***** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast_Const(policy);
		}

		static const void***** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast_Const(policy);
		}

		static const void***** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void5Cast_Const(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 5U>
	{
	public:
		static volatile void***** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void5Cast_Volatile(policy);
		}

		static volatile void***** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast_Volatile(policy);
		}

		static volatile void***** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast_Volatile(policy);
		}

		static volatile void***** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void5Cast_Volatile(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 5U>
	{
	public:
		static const volatile void***** Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Void5Cast_ConstVolatile(policy);
		}

		static const volatile void***** Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast_ConstVolatile(policy);
		}

		static const volatile void***** Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Void5Cast_ConstVolatile(policy);
		}

		static const volatile void***** Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Void5Cast_ConstVolatile(policy);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Value
	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::Value, 0U>
	{
		typedef typename deduce<T>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.ValueCast<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->ValueCast<base>(policy, index1, index2, index3, index4, index5);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::Value, 0U>
	{
		typedef typename std::add_const<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::Value, 0U>
	{
		typedef typename std::add_volatile<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Value, 0U>
	{
		typedef typename std::add_cv<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any& any = anys.At(index);
			return any.ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Pointer
	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.PtrCast<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->PtrCast<base>(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename std::add_const<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.PtrCast_Const<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast_Const<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast_Const<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->PtrCast_Const<base>(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 1U>
	{
		typedef typename std::add_pointer<typename std::add_cv<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Pointer2
	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr2Cast<base>(policy, index1, index2, index3);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast<base>(policy, index1, index2, index3);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast<base>(policy, index1, index2, index3);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr2Cast<base>(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr2Cast_Const<base>(policy, index1, index2, index3);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast_Const<base>(policy, index1, index2, index3);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast_Const<base>(policy, index1, index2, index3);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr2Cast_Const<base>(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 2U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 2U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Pointer3
	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr3Cast<base>(policy, index1, index2);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast<base>(policy, index1, index2);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast<base>(policy, index1, index2);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr3Cast<base>(policy, index1, index2);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr3Cast_Const<base>(policy, index1, index2);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast_Const<base>(policy, index1, index2);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast_Const<base>(policy, index1, index2);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr3Cast_Const<base>(policy, index1, index2);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr3Cast_Volatile<base>(policy, index1, index2);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast_Volatile<base>(policy, index1, index2);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast_Volatile<base>(policy, index1, index2);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr3Cast_Volatile<base>(policy, index1, index2);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 3U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 3U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Pointer4
	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr4Cast<base>(policy, index1);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast<base>(policy, index1);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast<base>(policy, index1);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr4Cast<base>(policy, index1);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr4Cast_Const<base>(policy, index1);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast_Const<base>(policy, index1);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast_Const<base>(policy, index1);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr4Cast_Const<base>(policy, index1);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr4Cast_Volatile<base>(policy, index1);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast_Volatile<base>(policy, index1);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast_Volatile<base>(policy, index1);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr4Cast_Volatile<base>(policy, index1);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 4U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 4U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr4Cast_ConstVolatile<base>(policy, index1);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast_ConstVolatile<base>(policy, index1);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr4Cast_ConstVolatile<base>(policy, index1);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr4Cast_ConstVolatile<base>(policy, index1);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor Pointer5
	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename deduce<T>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr5Cast<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr5Cast<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr5Cast_Const<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast_Const<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast_Const<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr5Cast_Const<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr5Cast_Volatile<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast_Volatile<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast_Volatile<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr5Cast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 5U>
	{
		typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 5U>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.Ptr5Cast_ConstVolatile<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast_ConstVolatile<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = anys.At(index);
			return oany.Ptr5Cast_ConstVolatile<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->Ptr5Cast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor LValueReference

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.LRefCast<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->LRefCast<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.LRefCast_Const<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast_Const<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast_Const<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->LRefCast_Const<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.LRefCast_Volatile<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast_Volatile<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast_Volatile<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->LRefCast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::LValueReference, 0U>
	{
		typedef typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.LRefCast_ConstVolatile<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast_ConstVolatile<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.LRefCast_ConstVolatile<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->LRefCast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion

#pragma region VariadicAnyVisitor RValueReference

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::None, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename deduce<T>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.RRefCast<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->RRefCast<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Const, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.RRefCast_Const<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast_Const<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast_Const<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->RRefCast_Const<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::Volatile, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.RRefCast_Volatile<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast_Volatile<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast_Volatile<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->RRefCast_Volatile<base>(policy);
		}
	};

	template< typename T>
	struct VariadicAnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::RValueReference, 0U>
	{
		typedef typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
	public:
		static base Get(const OptionalVariadicAny& optionalAnys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& oany = optionalAnys.At(index);
			return oany.RRefCast_ConstVolatile<base>(policy);
		}

		static base Get(const VariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast_ConstVolatile<base>(policy);
		}

		static base Get(const RealVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any& any = anys.At(index);
			return any.RRefCast_ConstVolatile<base>(policy);
		}

		static base Get(const DefaultVariadicAny& anys, size_t index, ModifierPolicy policy = ModifierPolicy::General)
		{
			const Any* any = anys.At(index);
			if (!any) throw std::exception("empty item");
			return any->RRefCast_ConstVolatile<base>(policy);
		}
	};
#pragma endregion
}