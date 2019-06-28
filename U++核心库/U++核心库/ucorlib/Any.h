#pragma once
#include <typeinfo>
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <exception>
#include <algorithm> 
#include <vector>
#include "Common.h"
#include "Object.h"
#include "Variant.h"
#include "Type.h"
#include "ArrayType.h"
#include "PtrType.h"
#include "RefType.h"
#include "MemoryPtrType.h"
#include "OptionalType.h"
#include "ResultType.h"
#include "OSHelper.h"
#include "TypeList.h"

namespace System
{
#pragma region Detect
	template<bool Const, bool Volatile>
	struct DetectModifier_impl
	{
	};

	template<>
	struct DetectModifier_impl<false, false>
	{
	public:
		static const Modifiers modifier = Modifiers::None;
	};

	template<>
	struct DetectModifier_impl<true, false>
	{
	public:
		static const Modifiers modifier = Modifiers::Const;
	};

	template<>
	struct DetectModifier_impl<false, true>
	{
	public:
		static const Modifiers modifier = Modifiers::Volatile;
	};

	template<>
	struct DetectModifier_impl<true, true>
	{
	public:
		static const Modifiers modifier = Modifiers::ConstVolatile;
	};

	template<typename T>
	struct DetectModifier
	{
		typedef typename std::remove_reference<typename std::remove_pointer<typename std::remove_pointer<T>::type>::type>::type base;
	public:
		static const bool isConst = std::is_const<base>::value;
		static const bool isVolatile = std::is_volatile<base>::value;

		static const Modifiers modifier = DetectModifier_impl<isConst, isVolatile>::modifier;
	};

	template<bool ptr, bool lref, bool rref>
	struct DetectDeclarator_impl
	{
	};

	template<>
	struct DetectDeclarator_impl<false, false, false>
	{
	public:
		static const Declarators declarator = Declarators::Value;
	};

	template<>
	struct DetectDeclarator_impl<true, false, false>
	{
	public:
		static const Declarators declarator = Declarators::Pointer;
	};

	template<>
	struct DetectDeclarator_impl<false, true, false>
	{
	public:
		static const Declarators declarator = Declarators::LValueReference;
	};

	template<>
	struct DetectDeclarator_impl<false, false, true>
	{
	public:
		static const Declarators declarator = Declarators::RValueReference;
	};

	template<class _Ty>
	struct pointer_rank
		: std::integral_constant<size_t, 0>
	{
	};


	template<class _Ty>
	struct pointer_rank<_Ty*>
		: std::integral_constant<size_t, pointer_rank<_Ty>::value + 1>
	{
	};

	template<bool _Test, size_t _N1, size_t _N2>
	struct _rank_condition
	{
		static const size_t value = _N2;
	};

	template<size_t _N1, size_t _N2>
	struct _rank_condition<true, _N1, _N2>
	{
		static const size_t value = _N1;
	};

	template<typename T>
	struct DetectDeclarator
	{
		static const bool isPointer = std::is_pointer<T>::value;
		static const bool isArray = std::is_array<T>::value;
		static const bool isLValueReference = std::is_lvalue_reference<T>::value;
		static const bool isRValueReference = std::is_rvalue_reference<T>::value;

		static const Declarators declarator = DetectDeclarator_impl<isPointer || isArray, isLValueReference, isRValueReference>::declarator;

		typedef typename std::remove_reference<T>::type rrT;

		static const size_t dimension = _rank_condition<std::is_pointer<rrT>::value, pointer_rank<rrT>::value, std::rank<rrT>::value>::value;
	};

#pragma endregion

#pragma region Any_Support

	class RUNTIME_API _RTDynamicCastReviewer
	{
	private:
		static bool Check_impl(const void* rawptr, const std::type_info& ptr_info, const std::type_info& info);
	public:
		template <typename T>
		static bool Check(const void* rawptr, const std::type_info& ptr_info)
		{
			return Check_impl(rawptr, ptr_info, typeid(deduce<T>::type));
		}
	};

#define i_ANY_CONCATNAME(a, b) a ## b
#define ANY_CONCATNAME(a, b) i_ANY_CONCATNAME(a, b)

#define ANY_REGISTER(REG,...)\
static struct REG{\
	REG() { __VA_ARGS__ } \
}ANY_CONCATNAME(U_ANY_REGISTER_ENTRY, __COUNTER__);

#define ANY_REGISTER_BEGIN(REG)\
static struct REG{\
	REG() {

	struct RUNTIME_API AnyConversion
	{
	private:
		static std::unordered_map<std::type_index, std::unordered_set<std::type_index>> regs;
	public:
		template<typename FROM, typename TO>
		static bool Register()
		{
			std::unordered_map<std::type_index, std::unordered_set<std::type_index>>::iterator iter = regs.find(std::type_index(typeid(FROM)));
			if (iter == regs.end())
			{
				auto rs = regs.insert(make_pair(std::type_index(typeid(FROM)), std::unordered_set<std::type_index>()));
				if (rs.second)
				{
					rs.first->second.insert(std::type_index(typeid(TO)));
				}
				else
					return false;
			}
			else
			{
				iter->second.insert(std::type_index(typeid(TO)));
			}
			return true;
		}

		template<typename T>
		static bool IsSupport(const std::type_info& info)
		{
			auto iter = regs.find(std::type_index(info));
			if (iter != regs.end())
			{
				return iter->second.find(std::type_index(typeid(T))) != iter->second.end();
			}
			return false;
		}
	};

#define any_support(FROM, TO)\
	ANY_REGISTER(ANY_CONCATNAME(ANY_PREMAIN_CLASS, __COUNTER__), AnyConversion::Register<FROM, TO>();)

#define any_support_begin\
	ANY_REGISTER_BEGIN(ANY_CONCATNAME(ANY_PREMAIN_CLASS, __COUNTER__))

#define any_support_add(FROM, TO) AnyConversion::Register<FROM, TO>();

#define any_support_end\
		}\
	}ANY_CONCATNAME(U_ANY_REGISTER_ENTRY, __COUNTER__);

#pragma endregion

	class ModifiedType_impl;
	class TypeFactory;

	class RUNTIME_API ModifiedType final:public Type
	{
	private:
		friend class ModifiedType_impl;
		_type _Hold;
		Modifiers _Modifier;
		InertiaString name;
		ModifiedType(_type hold, Modifiers modifier);
	protected:
		friend class TypeFactory;
		static const ModifiedType* Create(_type hold, Modifiers modifier);
	public:
		virtual const wchar_t* GetName() const;

		~ModifiedType();

		_type GetHold() const;
		Modifiers GetModifier() const;

		__declspec(property(get = GetHold)) _type Hold;
		__declspec(property(get = GetModifier)) Modifiers Modifier;
	};

#pragma region TypeFactory
	class RUNTIME_API TypeFactory final: public _noncopyable
	{
	public:
		template<class T, bool SUPPORTED = is_TypeHelper_supported<T>::value>
		struct Resolve;

		template<class T>
		struct Resolve<T, false>
		{
			static_assert(!is_template<T>::value || System::is_container<T>::value || System::is_memory_ptr<T>::value, "T is not defined, its defination is essential. Use Object as its base type and bring __uregister_template(T) inside its class declaration");
			static _type Get()
			{
				return nullptr;
			}
		};

		template<class T>
		struct Resolve<T, true>
		{
			static _type Get()
			{
				return TypeHelper<T>::GetType();
			}
		};

		template<Modifiers modifier, Declarators declarator, size_t dimension>
		__inline static _type PartialCreate(_type type)
		{
			return nullptr;
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::Value, 0U>(_type type)
		{
			return type;
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::Value, 0U>(_type type)
		{
			return ModifiedType::Create(type, Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::Value, 0U>(_type type)
		{
			return ModifiedType::Create(type, Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::Value, 0U>(_type type)
		{
			return ModifiedType::Create(type, Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::Pointer, 1U>(_type type)
		{
			return PtrType::Create(type, 1);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::Pointer, 1U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 1), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::Pointer, 1U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 1), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::Pointer, 1U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 1), Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::Pointer, 2U>(_type type)
		{
			return PtrType::Create(type, 2);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::Pointer, 2U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 2), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::Pointer, 2U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 2), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::Pointer, 2U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 2), Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::Pointer, 3U>(_type type)
		{
			return PtrType::Create(type, 3);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::Pointer, 3U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 3), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::Pointer, 3U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 3), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::Pointer, 3U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 3), Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::Pointer, 4U>(_type type)
		{
			return PtrType::Create(type, 4);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::Pointer, 4U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 4), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::Pointer, 4U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 4), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::Pointer, 4U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 4), Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::Pointer, 5U>(_type type)
		{
			return PtrType::Create(type, 5);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::Pointer, 5U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 5), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::Pointer, 5U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 5), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::Pointer, 5U>(_type type)
		{
			return ModifiedType::Create(PtrType::Create(type, 5), Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::LValueReference, 0U>(_type type)
		{
			return RefType::Create(type, RefKinds::Left);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::LValueReference, 0U>(_type type)
		{
			return ModifiedType::Create(RefType::Create(type, RefKinds::Left), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::LValueReference, 0U>(_type type)
		{
			return ModifiedType::Create(RefType::Create(type, RefKinds::Left), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::LValueReference, 0U>(_type type)
		{
			return ModifiedType::Create(RefType::Create(type, RefKinds::Left), Modifiers::ConstVolatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::None, Declarators::RValueReference, 0U>(_type type)
		{
			return RefType::Create(type, RefKinds::Right);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Const, Declarators::RValueReference, 0U>(_type type)
		{
			return ModifiedType::Create(RefType::Create(type, RefKinds::Right), Modifiers::Const);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::Volatile, Declarators::RValueReference, 0U>(_type type)
		{
			return ModifiedType::Create(RefType::Create(type, RefKinds::Right), Modifiers::Volatile);
		}

		template<>
		__inline static _type PartialCreate<Modifiers::ConstVolatile, Declarators::RValueReference, 0U>(_type type)
		{
			return ModifiedType::Create(RefType::Create(type, RefKinds::Right), Modifiers::ConstVolatile);
		}


		template<class T, ContainerTypes CT>
		struct PartialContainer;

		template<class T>
		struct PartialContainer<T, ContainerTypes::Unknown>
		{
			static _type Create()
			{
				return TypeHelper<T>::GetType();
			}
		};

		template<class T, size_t RANK = std::rank<T>::value>
		struct PartialArray
		{
			static _type Create()
			{
				return nullptr;
			}
		};

		template<class T>
		struct PartialArray<T, 1>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename deduce<T>::type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::Array, 1, std::extent<T>::value, 0, 0, 0, 0);
			}
		};

		template<class T>
		struct PartialArray<T, 2>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename deduce<T>::type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::Array, 2, std::extent<T>::value, std::extent<T, 1>::value, 0, 0, 0);
			}
		};

		template<class T>
		struct PartialArray<T, 3>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename deduce<T>::type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::Array, 3, std::extent<T>::value, std::extent<T, 1>::value, std::extent<T, 2>::value, 0, 0);
			}
		};

		template<class T>
		struct PartialArray<T, 4>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename deduce<T>::type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::Array, 4, std::extent<T>::value, std::extent<T, 1>::value, std::extent<T, 2>::value, std::extent<T, 3>::value, 0);
			}
		};

		template<class T>
		struct PartialArray<T, 5>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename deduce<T>::type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::Array, 5, std::extent<T>::value, std::extent<T, 1>::value, std::extent<T, 2>::value, std::extent<T, 3>::value, std::extent<T, 4>::value);
			}
		};
		template<class T>
		struct PartialContainer<T, ContainerTypes::Array>
		{
			static _type Create()
			{
				return PartialArray<T>::Create();
			}
		};

		template<class T>
		struct PartialContainer<T, ContainerTypes::STL_array>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename T::value_type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::STL_array, T::_EEN_SIZE);
			}
		};

		template<class T>
		struct PartialContainer<T, ContainerTypes::STL_valarray>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename T::value_type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::STL_valarray);
			}
		};

		template<class T>
		struct PartialContainer<T, ContainerTypes::STL_vector>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename T::value_type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::STL_vector);
			}
		};

		template<class T>
		struct PartialContainer<T, ContainerTypes::ObservableCollection>
		{
			static _type Create()
			{
				return ArrayType::Create(nullptr, nullptr, nullptr, nullptr, nullptr, Resolve<typename T::value_type>::Get(), nullptr, nullptr, nullptr, ContainerTypes::ObservableCollection);
			}
		};

		template<class T, MemoryPtrTypes CT>
		struct PartialMemoryPtr;

		template<class T>
		struct PartialMemoryPtr<T, MemoryPtrTypes::Unknown>
		{
			static _type Create()
			{
				return TypeHelper<T>::GetType();
			}
		};

		template<class T>
		struct PartialMemoryPtr<T, MemoryPtrTypes::SharedPtr>
		{
			static _type Create()
			{
				return MemoryPtrType::Create(Resolve<typename T::element_type>::Get(), MemoryPtrTypes::SharedPtr);
			}
		};

		template<class T>
		struct PartialMemoryPtr<T, MemoryPtrTypes::WeakPtr>
		{
			static _type Create()
			{
				return MemoryPtrType::Create(Resolve<typename T::element_type>::Get(), MemoryPtrTypes::WeakPtr);
			}
		};

		template<class T>
		struct PartialMemoryPtr<T, MemoryPtrTypes::UniquePtr>
		{
			static _type Create()
			{
				return MemoryPtrType::Create(Resolve<typename T::element_type>::Get(), MemoryPtrTypes::UniquePtr);
			}
		};

#if !_HAS_CXX17	
		template<class T>
		struct PartialMemoryPtr<T, MemoryPtrTypes::AutoPtr>
		{
			static _type Create()
			{
				return MemoryPtrType::Create(Resolve<typename T::element_type>::Get(), MemoryPtrTypes::AutoPtr);
			}
		};
#endif
		template<class T>
		struct PartialOptional
		{
			static _type Create()
			{
				return OptionalType::Create(Resolve<typename T::type>::Get());
			}
		};

		template<class T, ResultTypes CT>
		struct PartialResult;

		template<class T>
		struct PartialResult<T, ResultTypes::Unknown>
		{
			static _type Create()
			{
				return TypeHelper<T>::GetType();
			}
		};

		template<class T>
		struct PartialResult<T, ResultTypes::One>
		{
			static _type Create()
			{
				return ResultType::Create(Resolve<typename T::type1>::Get(), nullptr, nullptr, nullptr, ResultTypes::One);
			}
		};

		template<class T>
		struct PartialResult<T, ResultTypes::Two>
		{
			static _type Create()
			{
				return ResultType::Create(Resolve<typename T::type1>::Get(), nullptr, nullptr, Resolve<typename T::stateType>::Get(), ResultTypes::Two);
			}
		};

		template<class T>
		struct PartialResult<T, ResultTypes::Three>
		{
			static _type Create()
			{
				return ResultType::Create(Resolve<typename T::type1>::Get(), Resolve<typename T::type2>::Get(), nullptr, Resolve<typename T::stateType>::Get(), ResultTypes::Three);
			}
		};

		template<class T>
		struct PartialResult<T, ResultTypes::Four>
		{
			static _type Create()
			{
				return ResultType::Create(Resolve<typename T::type1>::Get(), Resolve<typename T::type2>::Get(), Resolve<typename T::type3>::Get(), Resolve<typename T::stateType>::Get(), ResultTypes::Four);
			}
		};

		template<class T, bool isContainer = System::is_container<typename deduce<T>::type>::value, bool isMemoryPtr = System::is_memory_ptr<typename deduce<T>::type>::value, bool is_Result = System::is_Result_based<typename deduce<T>::type>::value, bool is_Optional = System::is_Optional_based<typename deduce<T>::type>::value>
		struct Internal;

		template<class T>
		struct Internal<T, false, false, false, false>
		{
			static _type Create()
			{
				return PartialCreate<DetectModifier<T>::modifier, DetectDeclarator<T>::declarator, DetectDeclarator<T>::dimension>(Resolve<typename deduce<T>::type>::Get());
			}
		};


		template<class T>
		struct Internal<T, true, false, false, false>
		{
			static _type Create()
			{
				return PartialCreate<DetectModifier<T>::modifier, DetectDeclarator<T>::declarator, DetectDeclarator<T>::dimension>(PartialContainer<typename deduce<T>::type, DetectSTLContainer<typename deduce<T>::type>::value>::Create());
			}
		};

		template<class T>
		struct Internal<T, false, true, false, false>
		{
			static _type Create()
			{
				return PartialCreate<DetectModifier<T>::modifier, DetectDeclarator<T>::declarator, DetectDeclarator<T>::dimension>(PartialMemoryPtr<typename deduce<T>::type, DetectMemoryPtr<typename deduce<T>::type>::value>::Create());
			}
		};

		template<class T>
		struct Internal<T, false, false, true, false>
		{
			static _type Create()
			{
				return PartialCreate<DetectModifier<T>::modifier, DetectDeclarator<T>::declarator, DetectDeclarator<T>::dimension>(PartialResult<typename deduce<T>::type, DetectResult<typename deduce<T>::type>::value>::Create());
			}
		};

		template<class T>
		struct Internal<T, false, false, false, true>
		{
			static _type Create()
			{
				return PartialCreate<DetectModifier<T>::modifier, DetectDeclarator<T>::declarator, DetectDeclarator<T>::dimension>(PartialOptional<typename deduce<T>::type>::Create());
			}
		};

	public:

		template<class T>
		static _type Create()
		{
			return Internal<T>::Create();
		}

	};

#define _typeof(OBJECT)  System::TypeFactory::Create<OBJECT>()
#pragma endregion

	enum class AnyErrors
	{
		None,
		General,
		ModifierMissMatch,
		TemporaryAddress,
		NonConvertible
	};

	enum class AnyDynamicPtrFlags
	{
		Value,
		Ptr,
		Ptr2,
		Ptr3,
		Ptr4,
	};

	ENUM_FLAGSEX(ModifierPolicy, UShort)
	enum class ModifierPolicy:UShort
	{
		IgnoreNone2Const=0x01,
		IgnoreNone2Volatile=0x02,
		IgnoreNone2ConstVolatile=0x04,

		IgnoreNone = IgnoreNone2Const | IgnoreNone2Volatile | IgnoreNone2ConstVolatile,

		IgnoreConst2None=0x08,
		IgnoreConst2Volatile=0x10,
		IgnoreConst2ConstVolatile=0x20,

		IgnoreConst = IgnoreConst2None | IgnoreConst2Volatile | IgnoreConst2ConstVolatile,

		IgnoreVolatile2None = 0x40,
		IgnoreVolatile2Const = 0x80,
		IgnoreVolatile2ConstVolatile = 0x100,

		IgnoreVolatile = IgnoreVolatile2None | IgnoreVolatile2Const | IgnoreVolatile2ConstVolatile,

		IgnoreConstVolatile2None = 0x200,
		IgnoreConstVolatile2Const = 0x400,
		IgnoreConstVolatile2Volatile = 0x800,

		IgnoreConstVolatile = IgnoreConstVolatile2None | IgnoreConstVolatile2Const | IgnoreConstVolatile2Volatile,

		IgnoreAll = IgnoreNone | IgnoreConst | IgnoreVolatile | IgnoreConstVolatile,

		General = IgnoreNone | IgnoreConst,

		Same = 0x4000,
	};

#define CORRECTPTRFLAGS(flag, def_flag)  if(flag<AnyDynamicPtrFlags::Value || flag>AnyDynamicPtrFlags::Ptr) flag=def_flag;

#define CORRECTPTRFLAGS2(flag, def_flag)  if(flag<AnyDynamicPtrFlags::Value || flag>AnyDynamicPtrFlags::Ptr2) flag=def_flag;

#define CORRECTPTRFLAGS3(flag, def_flag)  if(flag<AnyDynamicPtrFlags::Value || flag>AnyDynamicPtrFlags::Ptr3) flag=def_flag;

#define CORRECTPTRFLAGS4(flag, def_flag)  if(flag<AnyDynamicPtrFlags::Value || flag>AnyDynamicPtrFlags::Ptr4) flag=def_flag;

#if _MSC_VER < 1900
	class bad_any_cast : public std::bad_cast
	{
		AnyErrors error;
	public:
		bad_any_cast(const char* message, AnyErrors e = AnyErrors::General) : bad_cast(message)
		{
		}

		AnyErrors err() const
		{
			return error;
		}
	};
#else
	class bad_any_cast : public std::exception
	{
		AnyErrors error;
	public:
		bad_any_cast(const char* message, AnyErrors e = AnyErrors::General) :	std::exception(message, 1)
		{
		}

		AnyErrors err() const
		{
			return error;
		}
	};
#endif

	class RUNTIME_API  Any
	{
	private:

		template <typename T>
		struct notany
		{
			static const T value = typename std::enable_if<!std::is_same<T, Any>::value || std::is_base_of<T, Any>::value, T>::type;
		};

		enum class AnyFlags: ::UInt8
		{
			UnFlagged = 0,
			Null = 1,
			Missing = 2,
			NoDefault = 3,
		};

#pragma region Storage

		template<size_t... N>
		struct is_all_positive;

		template<size_t N1>
		struct is_all_positive<N1>
		{
			static const bool value = N1 > 0U;
		};

		template<size_t N1, size_t N2>
		struct is_all_positive<N1, N2>
		{
			static const bool value = N1 > 0U && N2 > 0U;
		};

		template<size_t N1, size_t N2, size_t N3>
		struct is_all_positive<N1, N2, N3>
		{
			static const bool value = N1 > 0U && N2 > 0U && N3 > 0U;
		};

		template<size_t N1, size_t N2, size_t N3, size_t N4>
		struct is_all_positive<N1, N2, N3, N4>
		{
			static const bool value = N1 > 0U && N2 > 0U && N3 > 0U && N4 > 0U;
		};

		template<size_t N1, size_t N2, size_t N3, size_t N4, size_t N5>
		struct is_all_positive<N1, N2, N3, N4, N5>
		{
			static const bool value = N1 > 0U && N2 > 0U && N3 > 0U && N4 > 0U && N5 > 0U;
		};

		enum class AnyStorageTypes : ::UInt8
		{
			Number,
			Value,
			StaticValuePtr,
			StaticPtr,
			StaticPtr2,
			StaticPtr3,
			StaticPtr4,
			DynamicPtr,
			ValuePtr,
		};

		class AnyStorage
		{
		public:
			Modifiers modifier;
			AnyStorage() :modifier(Modifiers::None) {}
			AnyStorage(Modifiers mod) :modifier(mod) {}
			virtual ~AnyStorage() {}
			virtual AnyStorage * clone() const
			{
				return nullptr;
			}
			virtual const std::type_info & type() const = 0;
			virtual const std::type_info & basetype() const = 0;
			virtual AnyStorageTypes storageType() const = 0;
			virtual const System::Type* getType() const = 0;
			virtual const System::Type* getBaseType() const = 0;	
			virtual size_t super(void*& addressStore) const = 0;
			virtual ::UInt8 dimension() const = 0;
			virtual size_t dimension_num(::UInt8 index) const = 0;
			virtual bool polymorphic() const = 0;

			virtual Object* allocate() const
			{
				return nullptr;
			}

			virtual Object** allocate1() const
			{
				return nullptr;
			}

			virtual Object*** allocate2() const
			{
				return nullptr;
			}

			virtual Object**** allocate3() const
			{
				return nullptr;
			}

			virtual Object***** allocate4() const
			{
				return nullptr;
			}

			virtual bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const
			{
				index = 0U;
				return false;
			}

			virtual size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				index = 0U;
				dem = 0U;
				addressStore = nullptr;
				return 0U;
			}

			virtual bool superpolymorphic(void*& addressStore) const
			{
				return false;
			}
		};

#pragma region NumberStorage
		template<typename T>
		class NumberStorage: public AnyStorage
		{
			typedef typename std::remove_reference<typename deduce<T>::type>::type U;
			typedef VariantHelper<bool, Int8, ::UInt8, Int16, UInt16, Int24, UInt24, Int32, UInt32, long, ULong, Int64, UInt64, Int128, UInt128, Float32, Float64, Float128> Helper_t;
		public:
			NumberStorage() :typeIndex(typeid(void)) {}

			NumberStorage(T& value) : typeIndex(std::type_index(typeid(T))), AnyStorage(Modifiers::None)
			{
				new(data)U(value);

			}

			NumberStorage(const T& value) : typeIndex(std::type_index(typeid(T))), AnyStorage(Modifiers::Const)
			{
				new(data)U(value);
			}

			NumberStorage(volatile T& value) : typeIndex(std::type_index(typeid(T))), AnyStorage(Modifiers::Volatile)
			{
				new(data)U(value);
			}

			NumberStorage(const volatile T& value) : typeIndex(std::type_index(typeid(T))), AnyStorage(Modifiers::ConstVolatile)
			{
				new(data)U(value);
			}

			NumberStorage(T&& value, Modifiers mod) : typeIndex(std::type_index(typeid(void))), AnyStorage(mod)
			{
				new(data)U(value);
				typeIndex = std::type_index(typeid(T));
			}

			~NumberStorage()
			{
				Helper_t::Destroy(typeIndex, &data);
			}

			const std::type_info & type() const override
			{
				return typeid(U);
			}

			const std::type_info & basetype() const override
			{
				return typeid(U);
			}

			AnyStorage* clone() const override
			{
				NumberStorage* vs = new NumberStorage();
				memcpy(vs->data, data, sizeof(data));
				vs->typeIndex = typeIndex;
				vs->modifier = modifier;
				return vs;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::Number;
			}

			const System::Type* getType() const override
			{
				return TypeHelper<typename deduce<T>::type>::GetType();
			}

			const System::Type* getBaseType() const override
			{
				return TypeHelper<typename deduce<T>::type>::GetType();
			}

			Object* allocate() const  override
			{
				return nullptr;
			}

			bool polymorphic() const override
			{
				return false;
			}

			template<typename T, bool C>
			struct ValueGetter;

			template<typename T>
			struct ValueGetter<T, false>
			{
				template<typename NT>
				static T Get(const char(*mem)[MaxType<bool, Int8, ::UInt8, Int16, UInt16, Int24, UInt24, Int32, UInt32, long, ULong, Int64, UInt64, Int128, UInt128, Float32, Float64, Float128>::value])
				{
					throw bad_any_cast("can not convert", AnyErrors::NonConvertible);
				}
			};

			template<typename T>
			struct ValueGetter<T, true>
			{
				template<typename NT>
				static T Get(const char(*mem)[MaxType<bool, Int8, ::UInt8, Int16, UInt16, Int24, UInt24, Int32, UInt32, long, ULong, Int64, UInt64, Int128, UInt128, Float32, Float64, Float128>::value])
				{
					return number_cast<NT, T>::Cast(*(NT*)mem);
				}
			};

			template<typename T, bool C>
			struct TempGetter;

			template<typename T>
			struct TempGetter<T, false>
			{
				template<typename NT>
				static T Get(NT value)
				{
					throw bad_any_cast("can not convert", AnyErrors::NonConvertible);
				}
			};

			template<typename T>
			struct TempGetter<T, true>
			{
				template<typename NT>
				static T Get(NT value)
				{
					return (T)value;
				}
			};

			template<typename T>
			T Get() const
			{
				if (std::type_index(typeid(bool)) == typeIndex)
				{
					if (std::is_convertible<bool, T>::value)
					{
						return ValueGetter<T, std::is_convertible<bool, T>::value>::Get<bool>(&data);
					}
				}
				else if (std::type_index(typeid(Int8)) == typeIndex)
				{
					if (std::is_convertible<Int8, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Int8, T>::value>::Get<Int8>(&data);
					}
				}
				else if (std::type_index(typeid(::UInt8)) == typeIndex)
				{
					if (std::is_convertible<::UInt8, T>::value)
					{
						return ValueGetter<T, std::is_convertible<::UInt8, T>::value>::Get<::UInt8>(&data);
					}
				}
				else if (std::type_index(typeid(Int16)) == typeIndex)
				{
					if (std::is_convertible<Int16, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Int16, T>::value>::Get<Int16>(&data);
					}
				}
				else if (std::type_index(typeid(UInt16)) == typeIndex)
				{
					if (std::is_convertible<UInt16, T>::value)
					{
						return ValueGetter<T, std::is_convertible<UInt16, T>::value>::Get<UInt16>(&data);
					}
				}
				else if (std::type_index(typeid(Int24)) == typeIndex)
				{
					if (std::is_convertible<Int24, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Int24, T>::value>::Get<Int24>(&data);
					}
					else if (std::is_convertible<int, T>::value)
					{
						int tmp = number_cast<Int24, int>::Cast(*(Int24*)&data);
						return TempGetter<T, std::is_convertible<int, T>::value>::Get<int>(tmp);
					}
				}
				else if (std::type_index(typeid(UInt24)) == typeIndex)
				{
					if (std::is_convertible<UInt24, T>::value)
					{
						return ValueGetter<T, std::is_convertible<UInt24, T>::value>::Get<UInt24>(&data);
					}

					else if (std::is_convertible<int, T>::value)
					{
						int tmp = number_cast<UInt24, int>::Cast(*(UInt24*)&data);
						return TempGetter<T, std::is_convertible<int, T>::value>::Get<int>(tmp);
					}
				}
				else if (std::type_index(typeid(Int32)) == typeIndex)
				{
					if (std::is_convertible<Int32, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Int32, T>::value>::Get<Int32>(&data);
					}
				}
				else if (std::type_index(typeid(UInt32)) == typeIndex)
				{
					if (std::is_convertible<UInt32, T>::value)
					{
						return ValueGetter<T, std::is_convertible<UInt32, T>::value>::Get<UInt32>(&data);
					}
				}
				else if (std::type_index(typeid(long)) == typeIndex)
				{
					if (std::is_convertible<long, T>::value)
					{
						return ValueGetter<T, std::is_convertible<long, T>::value>::Get<long>(&data);
					}
				}
				else if (std::type_index(typeid(ULong)) == typeIndex)
				{
					if (std::is_convertible<ULong, T>::value)
					{
						return ValueGetter<T, std::is_convertible<ULong, T>::value>::Get<ULong>(&data);
					}
				}
				else if (std::type_index(typeid(Int64)) == typeIndex)
				{
					if (std::is_convertible<Int64, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Int64, T>::value>::Get<Int64>(&data);
					}
				}
				else if (std::type_index(typeid(UInt64)) == typeIndex)
				{
					if (std::is_convertible<UInt64, T>::value)
					{
						return ValueGetter<T, std::is_convertible<UInt64, T>::value>::Get<UInt64>(&data);
					}
				}
				else if (std::type_index(typeid(Int128)) == typeIndex)
				{
					if (std::is_convertible<Int128, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Int128, T>::value>::Get<Int128>(&data);
					}
					else if (std::is_convertible<Float128, T>::value)
					{
						Float128 tmp = number_cast<Int128, Float128>::Cast(*(Int128*)&data);
						return TempGetter<T, std::is_convertible<Float128, T>::value>::Get<Float128>(tmp);
					}
				}
				else if (std::type_index(typeid(UInt128)) == typeIndex)
				{
					if (std::is_convertible<UInt128, T>::value)
					{
						return ValueGetter<T, std::is_convertible<UInt128, T>::value>::Get<UInt128>(&data);
					}
					else if (std::is_convertible<Float128, T>::value)
					{
						Float128 tmp = number_cast<UInt128, Float128>::Cast(*(UInt128*)&data);
						return TempGetter<T, std::is_convertible<Float128, T>::value>::Get<Float128>(tmp);
					}
				}
				else if (std::type_index(typeid(Float32)) == typeIndex)
				{
					if (std::is_convertible<Float32, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Float32, T>::value>::Get<Float32>(&data);
					}
				}
				else if (std::type_index(typeid(Float64)) == typeIndex)
				{
					if (std::is_convertible<Float64, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Float64, T>::value>::Get<Float64>(&data);
					}
				}
				else if (std::type_index(typeid(Float128)) == typeIndex)
				{
					if (std::is_convertible<Float128, T>::value)
					{
						return ValueGetter<T, std::is_convertible<Float128, T>::value>::Get<Float128>(&data);
					}
				}
				throw bad_any_cast("can not convert", AnyErrors::NonConvertible);
			}

			size_t super(void*& addressStore) const override
			{
				addressStore = nullptr;
				return 0;
			}

			::UInt8 dimension() const override
			{
				return 0U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return 0U;
			}

		public:
			char data[MaxType<bool, Int8, ::UInt8, Int16, UInt16, Int24, UInt24, Int32, UInt32, long, ULong, Int64, UInt64, Int128, UInt128, Float32, Float64, Float128>::value];
			std::type_index typeIndex;
		private:
			NumberStorage & operator=(const NumberStorage &);

		};
#pragma endregion

		template<typename T>
		class ValueStorage : public AnyStorage
		{
		private:
			template<typename Ty, bool isObject>
			struct InternalAllocate;

			template<typename Ty>
			struct InternalAllocate<Ty, false>
			{
				static Object* Allocate(const T& val) { return nullptr; }
			};

			template<typename Ty>
			struct InternalAllocate<Ty, true>
			{
				static Object* Allocate(const T& val) { return (Object*)new T(val); }
			};

		public:
			ValueStorage(T& value) :val(value), AnyStorage(Modifiers::None)
			{
			}

			ValueStorage(const T& value) :val(value), AnyStorage(Modifiers::Const)
			{
			}

			ValueStorage(volatile T& value) :val(const_cast<T&>(value)), AnyStorage(Modifiers::Volatile)
			{
			}

			ValueStorage(const volatile T& value) :val(const_cast<T&>(value)), AnyStorage(Modifiers::ConstVolatile)
			{
			}

			ValueStorage(T&& value, Modifiers mod) : val(static_cast<T&&>(value)), AnyStorage(mod)
			{
			}

			const std::type_info & basetype() const override
			{
				return typeid(T);
			}

			const std::type_info & type() const override
			{
				switch (modifier)
				{
				case Modifiers::None:
				default:
					return typeid(T);
				case Modifiers::Const:
					return typeid(const T);
				case Modifiers::Volatile:
					return typeid(volatile T);
				case Modifiers::ConstVolatile:
					return typeid(const volatile T);
				}
				return typeid(_missing_t);
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<T>::value;
			}

			AnyStorage * clone() const override
			{
				ValueStorage* vs = new ValueStorage(val);
				vs->modifier = modifier;
				return vs;
			}

			AnyStorageTypes storageType() const  override
			{
				return AnyStorageTypes::Value;
			}

			const System::Type* getType() const  override;

			const System::Type* getBaseType() const override;

			Object* allocate() const override
			{
				return InternalAllocate<T, std::is_base_of<Object, T>::value>::Allocate(val);
			}

			size_t super(void*& addressStore) const override
			{
				addressStore = const_cast<void*>((const void*)&val);
				return sizeof(T);
			}

			::UInt8 dimension() const override
			{
				return 0U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return 0U;
			}

			~ValueStorage()
			{

			}

		public:
			T val;
		private:
			ValueStorage & operator=(const ValueStorage &);
		};

		class IPointerStorage
		{
		public:
			mutable union PointerMem
			{
				void* p;
				const void* cp;
				volatile void* vp;
				const volatile void* cvp;
			} mem;

			IPointerStorage() = default;

			virtual ~IPointerStorage() = default;

		};
#pragma region StaticValuePointerStorage

		template<typename T, size_t ...N>
		class StaticValuePointerStorage;

		template<typename T, size_t N1>
		class StaticValuePointerStorage<T, N1> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticValuePointerStorage & operator=(const StaticValuePointerStorage &);
		public:
			StaticValuePointerStorage(base ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticValuePointerStorage(const base ptr[N1]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticValuePointerStorage(volatile base ptr[N1]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticValuePointerStorage(const volatile base ptr[N1]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*);
				else if (modifier == Modifiers::Const)
					return typeid(const base*);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticValuePointerStorage* ps = new StaticValuePointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticValuePtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.cp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = (base*)mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1>::value)
					{
						addressStore = tmp;
						return true;
					}
				}
				return false;
			}

			~StaticValuePointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 1U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return index == 0 ? N1 : 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const override
			{
				if (index1 < N1)
				{
					index = index1;
					return true;
				}
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				if (mem.p)
				{
					value = true;
					index = index1 < N1 ? index1 : 0U;
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1>::value)
					{
						dem = 1U;
						addressStore =&tmp[index];
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
				}
				value = false;
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2>
		class StaticValuePointerStorage<T, N1, N2> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticValuePointerStorage & operator=(const StaticValuePointerStorage &);
		public:
			StaticValuePointerStorage(base ptr[N1][N2]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticValuePointerStorage(const base ptr[N1][N2]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticValuePointerStorage(volatile base ptr[N1][N2]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticValuePointerStorage(const volatile base ptr[N1][N2]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base**);
				else if (modifier == Modifiers::Const)
					return typeid(const base**);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base**);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base**);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticValuePointerStorage* ps = new StaticValuePointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticValuePtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.cp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						addressStore = tmp;
						return true;
					}
				}
				return false;
			}

			~StaticValuePointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 2U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const override
			{
				if (index1 < N1 && index2 < N2)
				{
					index = index1*N2 + index2;
					return true;
				}
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				if (mem.p)
				{
					value = true;
					if (index1 < N1 && index2 < N2)
					{
						index = index1*N2 + index2;
					}
					else
						index = 0U;
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						dem = 1U;
						addressStore = &tmp[index];
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
				}
				value = false;
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3>
		class StaticValuePointerStorage<T, N1, N2, N3> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticValuePointerStorage & operator=(const StaticValuePointerStorage &);
		public:

			StaticValuePointerStorage(base ptr[N1][N2][N3]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticValuePointerStorage(const base ptr[N1][N2][N3]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticValuePointerStorage(volatile base ptr[N1][N2][N3]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticValuePointerStorage(const volatile base ptr[N1][N2][N3]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base***);
				else if (modifier == Modifiers::Const)
					return typeid(const base***);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base***);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base***);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticValuePointerStorage* ps = new StaticValuePointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticValuePtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.cp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						addressStore = tmp;
						return true;
					}
				}
				return false;
			}

			~StaticValuePointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 3U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const override
			{
				if (index1 < N1 && index2 < N2 && index3<N3)
				{
					index = index1*N2 + index2*N3 + index3;
					return true;
				}
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				if (mem.p)
				{
					value = true;
					if (index1 < N1 && index2 < N2 && index3 < N3)
					{
						index = index1*N2 + index2*N3 + index3;
					}
					else
						index = 0U;
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2, N3>::value)
					{
						dem = 1U;
						addressStore = &tmp[index];
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
				}
				value = false;
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3, size_t N4>
		class StaticValuePointerStorage<T, N1, N2, N3, N4> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticValuePointerStorage & operator=(const StaticValuePointerStorage &);
		public:
			
			StaticValuePointerStorage(base ptr[N1][N2][N3][N4]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticValuePointerStorage(const base ptr[N1][N2][N3][N4]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticValuePointerStorage(volatile base ptr[N1][N2][N3][N4]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticValuePointerStorage(const volatile base ptr[N1][N2][N3][N4]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base****);
				else if (modifier == Modifiers::Const)
					return typeid(const base****);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base****);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base****);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticValuePointerStorage* ps = new StaticValuePointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticValuePtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.cp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						addressStore = tmp;
						return true;
					}
				}
				return false;
			}

			~StaticValuePointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 4U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				case 3:
					return N4;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const override
			{
				if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4)
				{
					index = index1*N2 + index2*N3 + index3*N4+ index4;
					return true;
				}
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				if (mem.p)
				{
					value = true;
					if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4)
					{
						index = index1*N2 + index2*N3 + index3*N4 + index4;
					}
					else
						index = 0U;
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2, N3, N4>::value)
					{
						dem = 1U;
						addressStore = &tmp[index];
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
				}
				value = false;
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3, size_t N4, size_t N5>
		class StaticValuePointerStorage<T, N1, N2, N3, N4, N5> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticValuePointerStorage & operator=(const StaticValuePointerStorage &);
		public:
			
			StaticValuePointerStorage(base ptr[N1][N2][N3][N4][N5]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticValuePointerStorage(const base ptr[N1][N2][N3][N4][N5]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticValuePointerStorage(volatile base ptr[N1][N2][N3][N4][N5]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticValuePointerStorage(const volatile base ptr[N1][N2][N3][N4][N5]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*****);
				else if (modifier == Modifiers::Const)
					return typeid(const base*****);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*****);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*****);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticValuePointerStorage* ps = new StaticValuePointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticValuePtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.cp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						addressStore = tmp;
						return true;
					}
				}
				return false;
			}

			~StaticValuePointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 5U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				case 3:
					return N4;
				case 4:
					return N5;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const override
			{
				if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4 && index5< N5)
				{
					index = index1*N2 + index2*N3 + index3*N4 + index4*N5 + index5;
					return true;
				}
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				if (mem.p)
				{
					value = true;
					if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4 && index5 < N5)
					{
						index = index1*N2 + index2*N3 + index3*N4 + index4*N5 + index5;
					}
					else
						index = 0U;
					base* tmp = (base*)mem.p;
					if (is_all_positive<N1, N2, N3, N4, N5>::value)
					{
						dem = 1U;
						addressStore = &tmp[index];
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
				}
				value = false;
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

#pragma endregion

#pragma region StaticPointerStorage

		template<typename T, size_t ...N>
		class StaticPointerStorage;

		template<typename T, size_t N1>
		class StaticPointerStorage<T, N1> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointerStorage & operator=(const StaticPointerStorage &);

			StaticPointerStorage() : AnyStorage(Modifiers::None) {}

		public:
			StaticPointerStorage(base* ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointerStorage(const base* ptr[N1]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointerStorage(volatile base* ptr[N1]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointerStorage(const volatile base* ptr[N1]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*[N1]);
				else if (modifier == Modifiers::Const)
					return typeid(const base*[N1]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*[N1]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*[N1]);
				return typeid(_missing_t);
			}

		    AnyStorage * clone() const override
			{
				StaticPointerStorage* ps = new StaticPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object** allocate1() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object**)mem.p;
				case Modifiers::Const:
					return (Object**)const_cast<base**>((const base**)mem.cp);
				case Modifiers::Volatile:
					return (Object**)const_cast<base**>((volatile base**)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object**)const_cast<base**>((const volatile base**)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (OSHelper::IsValidAddress<base, 2, 1>(tmp, false))
						{
							addressStore = tmp[0];
							return true;
						}
					}
				}
				return false;
			}

			~StaticPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 1U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return index == 0 ? N1 : 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1>::value)  return false;
				if (index1 < N1)
				{
					index = index1;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1>::value)
					{
						dem = 1U;
						index = index1 < N1 ? index1 : 0U;
						addressStore = tmp[index1];
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2>
		class StaticPointerStorage<T, N1, N2> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointerStorage & operator=(const StaticPointerStorage &);			
			StaticPointerStorage() : AnyStorage(Modifiers::None) {}
		public:
			StaticPointerStorage(base* ptr[N1][N2]) : AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointerStorage(const base* ptr[N1][N2]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointerStorage(volatile base* ptr[N1][N2]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointerStorage(const volatile base* ptr[N1][N2]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*[N1][N2]);
				else if (modifier == Modifiers::Const)
					return typeid(const base*[N1][N2]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*[N1][N2]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*[N1][N2]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointerStorage* ps = new StaticPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object** allocate1() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object**)mem.p;
				case Modifiers::Const:
					return (Object**)const_cast<base**>((const base**)mem.cp);
				case Modifiers::Volatile:
					return (Object**)const_cast<base**>((volatile base**)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object**)const_cast<base**>((const volatile base**)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						if (OSHelper::IsValidAddress<base, 2, 1>(tmp, false))
						{
							addressStore = tmp[0];
							return true;
						}
					}
				}
				return false;
			}

			~StaticPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 2U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2>::value)  return false;
				if (index1 < N1 && index2 < N2)
				{
					index = index1*N2 + index2;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						if (index1 < N1 && index2 < N2)
						{
							dem = 1U;
							index = index1*N2 + index2;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3>
		class StaticPointerStorage<T, N1, N2, N3> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointerStorage & operator=(const StaticPointerStorage &);
			StaticPointerStorage() : AnyStorage(Modifiers::None) {}
		public:
			StaticPointerStorage(base* ptr[N1][N2][N3]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointerStorage(const base* ptr[N1][N2][N3]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointerStorage(volatile base* ptr[N1][N2][N3]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointerStorage(const volatile base* ptr[N1][N2][N3]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2, N3>::value)
					{
						addressStore = tmp[0];
					}
					return true;
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*[N1][N2][N3]);
				else if (modifier == Modifiers::Const)
					return typeid(const base*[N1][N2][N3]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*[N1][N2][N3]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*[N1][N2][N3]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointerStorage* ps = new StaticPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object** allocate1() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object**)mem.p;
				case Modifiers::Const:
					return (Object**)const_cast<base**>((const base**)mem.cp);
				case Modifiers::Volatile:
					return (Object**)const_cast<base**>((volatile base**)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object**)const_cast<base**>((const volatile base**)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~StaticPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 3U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2, N3>::value)  return false;
				if (index1 < N1 && index2 < N2 && index3 < N3)
				{
					index = index1*N2 + index2*N3 + index3;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2, N3>::value)
					{
						if (index1 < N1 && index2 < N2 && index3 < N3)
						{
							dem = 1U;
							index = index1*N2 + index2*N3 + index3;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3, size_t N4>
		class StaticPointerStorage<T, N1, N2, N3, N4> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointerStorage & operator=(const StaticPointerStorage &);
			StaticPointerStorage() : AnyStorage(Modifiers::None) {}
		public:

			StaticPointerStorage(base* ptr[N1][N2][N3][N4]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointerStorage(const base* ptr[N1][N2][N3][N4]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointerStorage(volatile base* ptr[N1][N2][N3][N4]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointerStorage(const volatile base* ptr[N1][N2][N3][N4]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2, N3, N4>::value)
					{
						if (OSHelper::IsValidAddress<base, 2, 1>(tmp, false))
						{
							addressStore = tmp[0];
							return true;
						}
					}
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*[N1][N2][N3][N4]);
				else if (modifier == Modifiers::Const)
					return typeid(const base*[N1][N2][N3][N4]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*[N1][N2][N3][N4]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*[N1][N2][N3][N4]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointerStorage* ps = new StaticPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object** allocate1() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object**)mem.p;
				case Modifiers::Const:
					return (Object**)const_cast<base**>((const base**)mem.cp);
				case Modifiers::Volatile:
					return (Object**)const_cast<base**>((volatile base**)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object**)const_cast<base**>((const volatile base**)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~StaticPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 4U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				case 3:
					return N4;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2, N3, N4>::value)  return false;
				if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4)
				{
					index = index1*N2 + index2*N3 + index3*N4 + index4;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2, N3, N4>::value)
					{
						if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4)
						{
							dem = 1U;
							index = index1*N2 + index2*N3 + index3*N4 + index4;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3, size_t N4, size_t N5>
		class StaticPointerStorage<T, N1, N2, N3, N4, N5> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointerStorage & operator=(const StaticPointerStorage &);
			StaticPointerStorage() : AnyStorage(Modifiers::None) {}
		public:
			StaticPointerStorage(base* ptr[N1][N2][N3][N4][N5]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointerStorage(const base* ptr[N1][N2][N3][N4][N5]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointerStorage(volatile base* ptr[N1][N2][N3][N4][N5]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointerStorage(const volatile base* ptr[N1][N2][N3][N4][N5]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1,N2,N3,N4,N5>::value)
					{
						if (OSHelper::IsValidAddress<base, 2, 1>(tmp, false))
						{
							addressStore = tmp[0];
							return true;
						}
					}
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*[N1][N2][N3][N4][N5]);
				else if (modifier == Modifiers::Const)
					return typeid(const base*[N1][N2][N3][N4][N5]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*[N1][N2][N3][N4][N5]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*[N1][N2][N3][N4][N5]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointerStorage* ps = new StaticPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object** allocate1() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object**)mem.p;
				case Modifiers::Const:
					return (Object**)const_cast<base**>((const base**)mem.cp);
				case Modifiers::Volatile:
					return (Object**)const_cast<base**>((volatile base**)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object**)const_cast<base**>((const volatile base**)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~StaticPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 5U;
			}
			 
			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				case 3:
					return N4;
				case 4:
					return N5;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2, N3, N4, N5>::value)  return false;
				if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4 && index5< N5)
				{
					index = index1*N2 + index2*N3 + index3*N4 + index4*N5 + index5;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (is_all_positive<N1, N2, N3, N4, N5>::value)
					{
						if (index1 < N1 && index2 < N2 && index3 < N3 && index4 < N4 && index5< N5)
						{
							dem = 1U;
							index = index1*N2 + index2*N3 + index3*N4 + index4*N5 + index5;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

#pragma endregion

#pragma region StaticPointer2Storage

		template<typename T, size_t ...N>
		class StaticPointer2Storage;

		template<typename T, size_t N1>
		class StaticPointer2Storage<T, N1> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointer2Storage & operator=(const StaticPointer2Storage &);
		public:
			StaticPointer2Storage(base** ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointer2Storage(const base** ptr[N1]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointer2Storage(volatile base** ptr[N1]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointer2Storage(const volatile base** ptr[N1]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base**[N1]);
				else if (modifier == Modifiers::Const)
					return typeid(const base**[N1]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base**[N1]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base**[N1]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointer2Storage* ps = new StaticPointer2Storage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr2;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object*** allocate2() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object***)mem.p;
				case Modifiers::Const:
					return (Object***)const_cast<base***>((const base***)mem.cp);
				case Modifiers::Volatile:
					return (Object***)const_cast<base***>((volatile base***)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object***)const_cast<base***>((const volatile base***)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (OSHelper::IsValidAddress<base, 3, 1>(tmp, false))
						{
							if (OSHelper::IsValidAddress<base, 3, 1>(tmp, false))
							{
								addressStore = tmp[0][0];
								return true;
							}
						}
					}	
				}
				return false;
			}

			~StaticPointer2Storage()
			{
			}

			::UInt8 dimension() const override
			{
				return 1U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return index == 0 ? N1 : 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1>::value)  return false;
				if (index1 < N1)
				{
					index = index1;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (index1 < N1)
						{
							dem = 2U;
							index = index1;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2>
		class StaticPointer2Storage<T, N1, N2> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointer2Storage & operator=(const StaticPointer2Storage &);
		public:
			StaticPointer2Storage(base** ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointer2Storage(const base** ptr[N1][N2]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointer2Storage(volatile base** ptr[N1][N2]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointer2Storage(const volatile base** ptr[N1][N2]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base**[N1][N2]);
				else if (modifier == Modifiers::Const)
					return typeid(const base**[N1][N2]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base**[N1][N2]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base**[N1][N2]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointer2Storage* ps = new StaticPointer2Storage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr2;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object*** allocate2() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object***)mem.p;
				case Modifiers::Const:
					return (Object***)const_cast<base***>((const base***)mem.cp);
				case Modifiers::Volatile:
					return (Object***)const_cast<base***>((volatile base***)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object***)const_cast<base***>((const volatile base***)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (is_all_positive<N1,N2>::value)
					{
						if (OSHelper::IsValidAddress<base, 3, 1>(tmp, false))
						{
							if (OSHelper::IsValidAddress<base, 3, 1>(tmp, false))
							{
								addressStore = tmp[0][0];
								return true;
							}
						}
					}
				}
				return false;
			}

			~StaticPointer2Storage()
			{
			}

			::UInt8 dimension() const override
			{
				return 2U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2>::value)  return false;
				if (index1 < N1 && index2 < N2)
				{
					index = index1* N2 + index2;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						if (index1 < N1 && index2 < N2)
						{
							dem = 2U;
							index = index1* N2 + index2;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2, size_t N3>
		class StaticPointer2Storage<T, N1, N2, N3> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointer2Storage & operator=(const StaticPointer2Storage &);
		public:
			StaticPointer2Storage(base** ptr[N1][N2][N3]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointer2Storage(const base** ptr[N1][N2][N3]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointer2Storage(volatile base** ptr[N1][N2][N3]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointer2Storage(const volatile base** ptr[N1][N2][N3]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base**[N1][N2][N3]);
				else if (modifier == Modifiers::Const)
					return typeid(const base**[N1][N2][N3]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base**[N1][N2][N3]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base**[N1][N2][N3]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointer2Storage* ps = new StaticPointer2Storage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr2;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object*** allocate2() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object***)mem.p;
				case Modifiers::Const:
					return (Object***)const_cast<base***>((const base***)mem.cp);
				case Modifiers::Volatile:
					return (Object***)const_cast<base***>((volatile base***)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object***)const_cast<base***>((const volatile base***)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (is_all_positive<N1, N2, N3>::value)
					{
						if (OSHelper::IsValidAddress<base, 3, 1>(tmp, false))
						{
							addressStore = tmp[0][0];
							return true;
						}
					}
				}
				return false;
			}

			~StaticPointer2Storage()
			{
			}

			::UInt8 dimension() const override
			{
				return 3U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				case 2:
					return N3;
				}
				return 0U;
			}


			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2, N3>::value)  return false;
				if (index1 < N1 && index2 < N2 && index3 < N3)
				{
					index = index1*N2 + index2*N3 + index3;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (is_all_positive<N1, N2, N3>::value)
					{
						if (index1 < N1 && index2 < N2 && index3 < N3)
						{
							dem = 2U;
							index = index1*N2 + index2*N3 + index3;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		
#pragma endregion

#pragma region StaticPointer3Storage

		template<typename T, size_t ...N>
		class StaticPointer3Storage;

		template<typename T, size_t N1>
		class StaticPointer3Storage<T, N1> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointer3Storage & operator=(const StaticPointer3Storage &);
		public:
			StaticPointer3Storage(base*** ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointer3Storage(const base*** ptr[N1]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointer3Storage(volatile base*** ptr[N1]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointer3Storage(const volatile base*** ptr[N1]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base***[N1]);
				else if (modifier == Modifiers::Const)
					return typeid(const base***[N1]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base***[N1]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base***[N1]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointer3Storage* ps = new StaticPointer3Storage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr3;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object**** allocate3() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object****)mem.p;
				case Modifiers::Const:
					return (Object****)const_cast<base****>((const base****)mem.cp);
				case Modifiers::Volatile:
					return (Object****)const_cast<base****>((volatile base****)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object****)const_cast<base****>((const volatile base****)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base**** tmp = (base****)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (OSHelper::IsValidAddress<base, 4, 1>(tmp, false))
						{
							addressStore = tmp[0][0][0];
							return true;
						}
					}
				}
				return false;
			}

			~StaticPointer3Storage()
			{
			}

			::UInt8 dimension() const override
			{
				return 1U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return index == 0 ? N1 : 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1>::value)  return false;
				if (index1 < N1)
				{
					index = index1;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base**** tmp = (base****)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (index1 < N1)
						{
							dem = 3U;
							index = index1;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T, size_t N1, size_t N2>
		class StaticPointer3Storage<T, N1, N2> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointer3Storage & operator=(const StaticPointer3Storage &);
		public:
			StaticPointer3Storage(base*** ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointer3Storage(const base*** ptr[N1][N2]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointer3Storage(volatile base*** ptr[N1][N2]) : AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointer3Storage(const volatile base*** ptr[N1][N2]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base***[N1][N2]);
				else if (modifier == Modifiers::Const)
					return typeid(const base***[N1][N2]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base***[N1][N2]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base***[N1][N2]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointer3Storage* ps = new StaticPointer3Storage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr3;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object**** allocate3() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object****)mem.p;
				case Modifiers::Const:
					return (Object****)const_cast<base****>((const base****)mem.cp);
				case Modifiers::Volatile:
					return (Object****)const_cast<base****>((volatile base****)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object****)const_cast<base****>((const volatile base****)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base**** tmp = (base****)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						if (OSHelper::IsValidAddress<base, 4, 1>(tmp, false))
						{
							addressStore = tmp[0][0][0];
							return true;
						}
					}
				}
				return false;
			}

			~StaticPointer3Storage()
			{
			}

			::UInt8 dimension() const override
			{
				return 2U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return N1;
				case 1:
					return N2;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1, N2>::value)  return false;
				if (index1 < N1 && index2 < N2)
				{
					index = index1*N2 + index2;
					return true;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base**** tmp = (base****)mem.p;
					if (is_all_positive<N1, N2>::value)
					{
						if (index1 < N1 && index2 < N2)
						{
							dem = 3U;
							index = index1*N2 + index2;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

#pragma endregion

#pragma region StaticPointer4Storage

		template<typename T, size_t ...N>
		class StaticPointer4Storage;

		template<typename T, size_t N1>
		class StaticPointer4Storage<T, N1> : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			StaticPointer4Storage & operator=(const StaticPointer4Storage &);
		public:
			StaticPointer4Storage(base**** ptr[N1]): AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			StaticPointer4Storage(const base**** ptr[N1]) : AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			StaticPointer4Storage(volatile base**** ptr[N1]): AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			StaticPointer4Storage(const volatile base**** ptr[N1]) : AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base****[N1]);
				else if (modifier == Modifiers::Const)
					return typeid(const base****[N1]);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base****[N1]);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base****[N1]);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				StaticPointer3Storage* ps = new StaticPointer3Storage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::StaticPtr4;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object***** allocate4() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*****)mem.p;
				case Modifiers::Const:
					return (Object*****)const_cast<base*****>((const base*****)mem.cp);
				case Modifiers::Volatile:
					return (Object*****)const_cast<base*****>((volatile base*****)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object*****)const_cast<base*****>((const volatile base*****)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base***** tmp = (base*****)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (OSHelper::IsValidAddress<base, 5, 1>(tmp, false))
						{
							addressStore = tmp[0][0][0][0];
							return true;
						}
					}				
				}
				return false;
			}

			~StaticPointer4Storage()
			{
			}

			::UInt8 dimension() const override
			{
				return 1U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return index == 0 ? N1 : 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				if (!is_all_positive<N1>::value)  return false;
				if (index1 < N1)
				{
					index = index1;
					return true;
				}
				index = 0U
				return  false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				value = false;
				if (mem.p)
				{
					base***** tmp = (base*****)mem.p;
					if (is_all_positive<N1>::value)
					{
						if (index1 < N1)
						{
							dem = 4U;
							index= index1;
							addressStore = tmp[index];
							return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
						}
					}
				}
				dem = 0U;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};
#pragma endregion

#pragma region DynamicPointerStorage
		template<typename T, size_t ...N>
		class DynamicPointerStorage;

		template<typename T>
		class DynamicPointerStorage < T, 0U > : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			DynamicPointerStorage & operator=(const DynamicPointerStorage &);
			size_t _n;
			DynamicPointerStorage() :_n(0), AnyStorage(Modifiers::None) {}
		public:
			DynamicPointerStorage(base* ptr, size_t n) : _n(n), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base* ptr, size_t n) : _n(n), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base* ptr, size_t n) : _n(n), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base* ptr, size_t n) : _n(n), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*);
				else if (modifier == Modifiers::Const)
					return typeid(const base*);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				DynamicPointerStorage* ps = new DynamicPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::DynamicPtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.cp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				addressStore =mem.p;
				return true;
			}

			~DynamicPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 1U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return index == 0 ? _n : 0U;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				dem = 1U;
				value = false;
				index = 0U;
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}
		};

		template<typename T>
		class DynamicPointerStorage < T, 1U > : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			DynamicPointerStorage & operator=(const DynamicPointerStorage &);
			size_t _n1;
			size_t _n2;
			::UInt8 _d;
			DynamicPointerStorage() :_n1(0), _n2(0), _d(0), AnyStorage(Modifiers::None) {}
		public:
#pragma region value_array2
			DynamicPointerStorage(base** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _d(2U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _d(2U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _d(2U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _d(2U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr_array
			DynamicPointerStorage(base** ptr, size_t n) : _n1(n), _n2(0), _d(1U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base** ptr, size_t n) : _n1(n), _n2(0), _d(1U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base** ptr, size_t n) : _n1(n), _n2(0), _d(1U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base** ptr, size_t n) : _n1(n), _n2(0), _d(1U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base** tmp = (base**)mem.p;
					if (OSHelper::IsValidAddress<base, 2, 1>(tmp, false))
					{
						addressStore = tmp[0];
						return true;
					}
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base**);
				else if (modifier == Modifiers::Const)
					return typeid(const base**);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base**);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base**);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				DynamicPointerStorage* ps = new DynamicPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const
			{
				return AnyStorageTypes::DynamicPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object** allocate1() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object**)mem.p;
				case Modifiers::Const:
					return (Object**)const_cast<base**>((const base**)mem.cp);
				case Modifiers::Volatile:
					return (Object**)const_cast<base**>((volatile base**)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object**)const_cast<base**>((const volatile base**)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~DynamicPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return _d;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return _n1;
				case 1: 
					return _n2;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				switch (_d)
				{
				case 1U:
					if (index1 < _n1)
					{
						index = index1;
						return true;
					}
					break;
				case 2U:
					if (index1 < _n1 && index2< _n2)
					{
						index = index1*_n2 + index2;
						return true;
					}
					break;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				switch (_d)
				{
				case 1U:
				{
					index = index1 < _n1 ? index1 : 0U;
					dem = 1U;
					base** tmp = (base**)mem.p;
					addressStore = tmp[index];
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 2U:
					if (index1 < _n1 && index2< _n2)
					{
						dem = 1U;
						value = true;
						addressStore = mem.p;
						index = index1*_n2 + index2;
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
					break;
				}
				dem = 0U;
				value = false;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T>
		class DynamicPointerStorage < T, 2U > : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			DynamicPointerStorage & operator=(const DynamicPointerStorage &);
			size_t _n1;
			size_t _n2; 
			size_t _n3;
			::UInt8 _d;
			DynamicPointerStorage() :_n1(0), _n2(0), _n3(0), _d(0), AnyStorage(Modifiers::None) {}
		public:
#pragma region value_array3
			DynamicPointerStorage(base*** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _d(3U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base*** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _d(3U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base*** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _d(3U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base*** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _d(3U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr2_array
			DynamicPointerStorage(base*** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _d(1U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base*** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _d(1U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base*** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _d(1U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base*** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _d(1U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr_array2
			DynamicPointerStorage(base*** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _d(2U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base*** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _d(2U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base*** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _d(2U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base*** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _d(2U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base*** tmp = (base***)mem.p;
					if (OSHelper::IsValidAddress<base, 3, 1>(tmp, false))
					{
						addressStore = tmp[0][0];
						return true;
					}
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base***);
				else if (modifier == Modifiers::Const)
					return typeid(const base***);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base***);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base***);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				DynamicPointerStorage* ps = new DynamicPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const
			{
				return AnyStorageTypes::DynamicPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object*** allocate2() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object***)mem.p;
				case Modifiers::Const:
					return (Object***)const_cast<base***>((const base***)mem.cp);
				case Modifiers::Volatile:
					return (Object***)const_cast<base***>((volatile base***)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object***)const_cast<base***>((const volatile base***)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~DynamicPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return _d;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return _n1;
				case 1:
					return _n2;
				case 2:
					return _n3;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				switch (_d)
				{
				case 1U:
					if (index1 < _n1)
					{
						index = index1;
						return true;
					}
					break;
				case 2U:
					if (index1 < _n1 && index2< _n2)
					{
						index = index1*_n2 + index2;
						return true;
					}
					break;
				case 3U:
					if (index1 < _n1 && index2< _n2 && index3< _n3)
					{
						index = index1*_n2 + index2*_n3 + index3;
						return true;
					}
					break;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				switch (_d)
				{
				case 1U:
				{
					dem = 2U;
					index = index1 < _n1 ? index1 : 0U;
					base*** tmp = (base***)mem.p;
					addressStore = tmp[index];
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 2U:
				{
					dem = 1U;
					base*** tmp = (base***)mem.p;
					if (index1 < _n1 && index2 < _n2)
					{
						index = index1*_n2 + index2;
						addressStore = tmp[index1][index2];
					}
					else
					{
						index = 0U;
						addressStore = tmp[0][0];
					}
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 3U:
					if (index1 < _n1 && index2< _n2 && index3< _n3)
					{
						dem = 1U;
						value = true;
						addressStore = mem.p;
						index = index1*_n2 + index2* _n3 + index3;
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
					break;
				}
				dem = 0U;
				value = false;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T>
		class DynamicPointerStorage < T, 3U > : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			DynamicPointerStorage & operator=(const DynamicPointerStorage &);
			size_t _n1;
			size_t _n2;
			size_t _n3;
			size_t _n4;
			::UInt8 _d;
			DynamicPointerStorage() :_n1(0), _n2(0), _n3(0), _n4(0), _d(0), AnyStorage(Modifiers::None) {}
		public:
#pragma region value_array4
			DynamicPointerStorage(base**** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _d(4U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base**** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _d(4U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base**** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _d(4U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base**** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _d(4U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr_array3
			DynamicPointerStorage(base**** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _d(3U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base**** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _d(3U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base**** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _d(3U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base**** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _d(3U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr2_array2
			DynamicPointerStorage(base**** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _d(2U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base**** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _d(2U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base**** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _d(2U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base**** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _d(2U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr3_array
			DynamicPointerStorage(base**** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _d(1U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base**** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _d(1U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base**** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _d(1U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base**** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _d(1U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base**** tmp = (base****)mem.p;
					if (OSHelper::IsValidAddress<base, 4, 1>(tmp, false))
					{
						addressStore = tmp[0][0][0];
						return true;
					}
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base****);
				else if (modifier == Modifiers::Const)
					return typeid(const base****);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base****);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base****);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				DynamicPointerStorage* ps = new DynamicPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const
			{
				return AnyStorageTypes::DynamicPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object**** allocate3() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object****)mem.p;
				case Modifiers::Const:
					return (Object****)const_cast<base****>((const base****)mem.cp);
				case Modifiers::Volatile:
					return (Object****)const_cast<base****>((volatile base****)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object****)const_cast<base****>((const volatile base****)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~DynamicPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return _d;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return _n1;
				case 1:
					return _n2;
				case 2:
					return _n3;
				case 3:
					return _n4;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				switch (_d)
				{
				case 1U:
					if (index1 < _n1)
					{
						index = index1;
						return true;
					}
					break;
				case 2U:
					if (index1 < _n1 && index2< _n2)
					{
						index = index1*_n2 + index2;
						return true;
					}
					break;
				case 3U:
					if (index1 < _n1 && index2< _n2 && index3< _n3)
					{
						index = index1*_n2 + index2*_n3 + index3;
						return true;
					}
					break;
				case 4U:
					if (index1 < _n1 && index2< _n2 && index3< _n3 && index4< _n4)
					{
						index = index1*_n2 + index2*_n3 + index3*_n4 + index4;
						return true;
					}
					break;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				switch (_d)
				{
				case 1U:
				{
					dem = 3U;
					index = index1 < _n1 ? index1 : 0U;
					base**** tmp = (base****)mem.p;
					addressStore = tmp[index];
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 2U:
				{
					dem = 2U;
					base**** tmp = (base****)mem.p;
					if (index1 < _n1 && index2 < _n2)
					{
						index = index1*_n2 + index2;
						addressStore = tmp[index1][index2];
					}
					else
					{
						index = 0U;
						addressStore = tmp[0][0];
					}
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 3U:
				{
					dem = 1U;
					base**** tmp = (base****)mem.p;
					if (index1 < _n1 && index2 < _n2 && index3 < _n3)
					{
						index = index1*_n2 + index2*_n3 + index3;
						addressStore = tmp[index1][index2][index3];
					}
					else
					{
						index = 0U;
						addressStore = tmp[0][0][0];
					}
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 4U:
					if (index1 < _n1 && index2< _n2 && index3< _n3 && index4< _n4)
					{
						dem = 1U;
						value = true;
						addressStore = mem.p;
						index = index1*_n2 + index2* _n3 + index3* _n4 + index4;
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
					break;
				}
				dem = 0U;
				value = false;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

		template<typename T>
		class DynamicPointerStorage < T, 4U > : public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			DynamicPointerStorage & operator=(const DynamicPointerStorage &);
			size_t _n1;
			size_t _n2;
			size_t _n3;
			size_t _n4;
			size_t _n5;
			::UInt8 _d;
			DynamicPointerStorage() :_n1(0), _n2(0), _n3(0), _n4(0), _n5(0), _d(0), AnyStorage(Modifiers::None) {}
		public:
#pragma region value_array5
			DynamicPointerStorage(base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(n5), _d(5U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(n5), _d(5U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(n5), _d(5U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(n5), _d(5U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr_array4
			DynamicPointerStorage(base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(0), _d(4U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(0), _d(4U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(0), _d(4U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base***** ptr, size_t n1, size_t n2, size_t n3, size_t n4) : _n1(n1), _n2(n2), _n3(n3), _n4(n4), _n5(0), _d(4U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr2_array3
			DynamicPointerStorage(base***** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _n5(0), _d(3U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base***** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _n5(0), _d(3U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base***** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _n5(0), _d(3U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base***** ptr, size_t n1, size_t n2, size_t n3) : _n1(n1), _n2(n2), _n3(n3), _n4(0), _n5(0), _d(3U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr3_array2
			DynamicPointerStorage(base***** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _n5(0), _d(2U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base***** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _n5(0), _d(2U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base***** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _n5(0), _d(2U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base***** ptr, size_t n1, size_t n2) : _n1(n1), _n2(n2), _n3(0), _n4(0), _n5(0), _d(2U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

#pragma region ptr4_array
			DynamicPointerStorage(base***** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _n5(0), _d(1U), AnyStorage(Modifiers::None)
			{
				mem.p = ptr;
			}

			DynamicPointerStorage(const base***** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _n5(0), _d(1U), AnyStorage(Modifiers::Const)
			{
				mem.cp = ptr;
			}

			DynamicPointerStorage(volatile base***** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _n5(0), _d(1U), AnyStorage(Modifiers::Volatile)
			{
				mem.vp = ptr;
			}

			DynamicPointerStorage(const volatile base***** ptr, size_t n) : _n1(n), _n2(0), _n3(0), _n4(0), _n5(0), _d(1U), AnyStorage(Modifiers::ConstVolatile)
			{
				mem.cvp = ptr;
			}
#pragma endregion

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				if (mem.p)
				{
					base***** tmp = (base*****)mem.p;
					if (OSHelper::IsValidAddress<base, 5, 1>(tmp, false))
					{
						addressStore = tmp[0][0][0][0];
						return true;
					}
				}
				return false;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				if (modifier == Modifiers::None)
					return typeid(base*****);
				else if (modifier == Modifiers::Const)
					return typeid(const base*****);
				else if (modifier == Modifiers::Volatile)
					return typeid(volatile base*****);
				else if (modifier == Modifiers::ConstVolatile)
					return typeid(const volatile base*****);
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				DynamicPointerStorage* ps = new DynamicPointerStorage();
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const
			{
				return AnyStorageTypes::DynamicPtr;
			}

			Object* allocate() const override
			{
				return nullptr;
			}

			Object***** allocate4() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*****)mem.p;
				case Modifiers::Const:
					return (Object*****)const_cast<base*****>((const base*****)mem.cp);
				case Modifiers::Volatile:
					return (Object*****)const_cast<base*****>((volatile base*****)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object*****)const_cast<base*****>((const volatile base*****)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore = mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~DynamicPointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return _d;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				switch (index)
				{
				case 0:
					return _n1;
				case 1:
					return _n2;
				case 2:
					return _n3;
				case 3:
					return _n4;
				case 4:
					return _n5;
				}
				return 0U;
			}

			bool index(size_t& index, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5)  const override
			{
				switch (_d)
				{
				case 1U:
					if (index1 < _n1)
					{
						index = index1;
						return true;
					}
					break;
				case 2U:
					if (index1 < _n1 && index2< _n2)
					{
						index = index1*_n2 + index2;
						return true;
					}
					break;
				case 3U:
					if (index1 < _n1 && index2< _n2 && index3< _n3)
					{
						index = index1*_n2 + index2*_n3 + index3;
						return true;
					}
					break;
				case 4U:
					if (index1 < _n1 && index2< _n2 && index3< _n3 && index4< _n4)
					{
						index = index1*_n2 + index2*_n3 + index3*_n4 + index4;
						return true;
					}
					break;
				case 5U:
					if (index1 < _n1 && index2< _n2 && index3< _n3 && index4< _n4 && index5< _n5)
					{
						index = index1*_n2 + index2*_n3 + index3*_n4 + index4*_n5 + index5;
						return true;
					}
					break;
				}
				index = 0U;
				return false;
			}

			size_t superpointer(bool& value, size_t& index, ::UInt8& dem, void*& addressStore, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
			{
				switch (_d)
				{
				case 1U:
				{
					dem = 4U;
					index = index1 < _n1 ? index1 : 0U;
					base***** tmp = (base*****)mem.p;
					addressStore = tmp[index];
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 2U:
				{
					dem = 3U;
					base***** tmp = (base*****)mem.p;
					if (index1 < _n1 && index2 < _n2)
					{
						index = index1*_n2 + index2;
						addressStore = tmp[index1][index2];
					}
					else
					{
						index = 0U;
						addressStore = tmp[0][0];
					}
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 3U:
				{
					dem = 2U;
					base***** tmp = (base*****)mem.p;
					if (index1 < _n1 && index2 < _n2 && index3 < _n3)
					{
						index = index1*_n2 + index2*_n3 + index3;
						addressStore = tmp[index1][index2][index3];
					}
					else
					{
						index = 0U;
						addressStore = tmp[0][0][0];
					}
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 4U:
				{
					dem = 1U;
					base***** tmp = (base*****)mem.p;
					if (index1 < _n1 && index2 < _n2 && index3 < _n3 && index4 < _n4)
					{
						index = index1*_n2 + index2*_n3 + index3* _n4 + index4;
						addressStore = tmp[index1][index2][index3][index4];
					}
					else
					{
						index = 0U;
						addressStore = tmp[0][0][0][0];
					}
					return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
				}
					break;
				case 5U:
					if (index1 < _n1 && index2< _n2 && index3< _n3 && index4< _n4 && index5< _n5)
					{
						dem = 1U;
						value = true;
						addressStore = mem.p;
						index = index1*_n2 + index2* _n3 + index3* _n4 + index4* _n5 + index5;
						return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
					}
					break;
				}
				dem = 0U;
				value = false;
				index = 0U;
				addressStore = nullptr;
				return 0U;
			}
		};

#pragma endregion

#pragma region ValuePointerStorage

		enum class ValuePointerKinds
		{
			None,
			Left,
			Right
		};

		template<typename T>
		class ValuePointerStorage: public AnyStorage, public IPointerStorage
		{
			typedef typename remove_md<T>::type base;
			static const bool is_based__uobject = std::is_base_of<Object, base>::value;
		private:
			ValuePointerKinds Kind;
			ValuePointerStorage & operator=(const ValuePointerStorage &);
		public:
			ValuePointerStorage() :AnyStorage(Modifiers::None), Kind(ValuePointerKinds::None) {}

			ValuePointerStorage(base* ptr, ValuePointerKinds kind) : AnyStorage(Modifiers::None), Kind(kind)
			{
				mem.p = ptr;
			}

			ValuePointerStorage(const base* ptr, ValuePointerKinds kind) : AnyStorage(Modifiers::Const), Kind(kind)
			{
				mem.cp = ptr;
			}

			ValuePointerStorage(volatile base* ptr, ValuePointerKinds kind) : AnyStorage(Modifiers::Volatile), Kind(kind)
			{
				mem.vp = ptr;
			}

			ValuePointerStorage(const volatile base* ptr, ValuePointerKinds kind) : AnyStorage(Modifiers::ConstVolatile), Kind(kind)
			{
				mem.cvp = ptr;
			}

			bool polymorphic() const override
			{
				return std::is_polymorphic<base>::value;
			}

			bool superpolymorphic(void*& addressStore) const override
			{
				if (!std::is_polymorphic<base>::value) return false;
				addressStore = mem.p;
				return true;
			}

			const std::type_info & basetype() const override
			{
				return typeid(base);
			}

			const std::type_info & type() const override
			{
				switch (Kind)
				{
				case ValuePointerKinds::None:
				{
					if (modifier == Modifiers::None)
						return typeid(base*);
					else if (modifier == Modifiers::Const)
						return typeid(const base*);
					else if (modifier == Modifiers::Volatile)
						return typeid(volatile base*);
					else if (modifier == Modifiers::ConstVolatile)
						return typeid(const volatile base*);
				}
					break;
				case ValuePointerKinds::Left:
				{
					if (modifier == Modifiers::None)
						return typeid(std::conditional<std::is_same<base, void>::value, void*, base>::type&);
					else if (modifier == Modifiers::Const)
						return typeid(const std::conditional<std::is_same<base, void>::value, void*, base>::type&);
					else if (modifier == Modifiers::Volatile)
						return typeid(volatile std::conditional<std::is_same<base, void>::value, void*, base>::type&);
					else if (modifier == Modifiers::ConstVolatile)
						return typeid(const volatile std::conditional<std::is_same<base, void>::value, void*, base>::type&);
				}
					break;
				case ValuePointerKinds::Right:
				{
					if (modifier == Modifiers::None)
						return typeid(std::conditional<std::is_same<base, void>::value, void*, base>::type&&);
					else if (modifier == Modifiers::Const)
						return typeid(const std::conditional<std::is_same<base, void>::value, void*, base>::type&&);
					else if (modifier == Modifiers::Volatile)
						return typeid(volatile std::conditional<std::is_same<base, void>::value, void*, base>::type&&);
					else if (modifier == Modifiers::ConstVolatile)
						return typeid(const volatile std::conditional<std::is_same<base, void>::value, void*, base>::type&&);
				}
					break;
				}
				return typeid(_missing_t);
			}

			AnyStorage * clone() const override
			{
				ValuePointerStorage* ps = new ValuePointerStorage();
				ps->Kind = Kind;
				ps->modifier = modifier;
				if (modifier == Modifiers::None)
					ps->mem.p = mem.p;
				else if (modifier == Modifiers::Const)
					ps->mem.cp = mem.cp;
				else if (modifier == Modifiers::Volatile)
					ps->mem.vp = mem.vp;
				else if (modifier == Modifiers::ConstVolatile)
					ps->mem.cvp = mem.cvp;
				return ps;
			}

			AnyStorageTypes storageType() const override
			{
				return AnyStorageTypes::ValuePtr;
			}

			Object* allocate() const override
			{
				if (!is_based__uobject) return nullptr;
				switch (modifier)
				{
				case Modifiers::None:
					return (Object*)mem.p;
				case Modifiers::Const:
					return (Object*)const_cast<base*>((const base*)mem.cp);
				case Modifiers::Volatile:
					return (Object*)const_cast<base*>((volatile base*)mem.vp);
				case Modifiers::ConstVolatile:
					return (Object*)const_cast<base*>((const volatile base*)mem.cvp);
				default:
					return nullptr;
				}
				return nullptr;
			}

			const System::Type* getType() const override;

			const System::Type* getBaseType() const override;

			size_t super(void*& addressStore) const override
			{
				addressStore =mem.p;
				return sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type);
			}

			~ValuePointerStorage()
			{
			}

			::UInt8 dimension() const override
			{
				return 0U;
			}

			size_t dimension_num(::UInt8 index) const override
			{
				return 0U;
			}
		};

#pragma endregion
#pragma endregion

		AnyStorage * storage;

		AnyFlags flag;

		Object* ObjectGetter_impl() const;

		Object** ObjectGetter_impl1() const;

		Object*** ObjectGetter_impl2() const;

		Object**** ObjectGetter_impl3() const;

		Object***** ObjectGetter_impl4() const;

	public:

		Any(nullptr_t null);

		Any(_missing_t optional);

		Any(_nodefault_t nodefault);

		Any();

		~Any();

#pragma region Number

		Any(bool& value, bool copy = true);

		Any(const bool& value, bool copy = true);

		Any(const volatile bool& value, bool copy = true);

		Any(Int8& value, bool copy = true);

		Any(const Int8& value, bool copy = true);

		Any(const volatile Int8& value, bool copy = true);

		Any(::UInt8& value, bool copy = true);

		Any(const ::UInt8& value, bool copy = true);

		Any(const volatile ::UInt8& value, bool copy = true);

		Any(UInt16& value, bool copy = true);

		Any(const UInt16& value, bool copy = true);

		Any(const volatile UInt16& value, bool copy = true);

		Any(UInt24& value, bool copy = true);

		Any(const UInt24& value, bool copy = true);

		Any(const volatile UInt24& value, bool copy = true);

		Any(UInt32& value, bool copy = true);

		Any(const UInt32& value, bool copy = true);

		Any(const volatile UInt32& value, bool copy = true);

		Any(ULong& value, bool copy = true);

		Any(const ULong& value, bool copy = true);

		Any(const volatile ULong& value, bool copy = true);

		Any(UInt64& value, bool copy = true);

		Any(const UInt64& value, bool copy = true);

		Any(const volatile UInt64& value, bool copy = true);

		Any(UInt128& value, bool copy = true);

		Any(const UInt128& value, bool copy = true);

		Any(const volatile UInt128& value, bool copy = true);

		Any(Int16& value, bool copy = true);

		Any(const Int16& value, bool copy = true);

		Any(const volatile Int16& value, bool copy = true);

		Any(Int24& value, bool copy = true);

		Any(const Int24& value, bool copy = true);

		Any(const volatile Int24& value, bool copy = true);

		Any(Int32& value, bool copy = true);

		Any(const Int32& value, bool copy = true);

		Any(const volatile Int32& value, bool copy = true);

		Any(long& value, bool copy = true);

		Any(const long& value, bool copy = true);

		Any(const volatile long& value, bool copy = true);

		Any(Int64& value, bool copy = true);

		Any(const Int64& value, bool copy = true);

		Any(const volatile Int64& value, bool copy = true);

		Any(Int128& value, bool copy = true);

		Any(const Int128& value, bool copy = true);

		Any(const volatile Int128& value, bool copy = true);

		Any(Float32& value, bool copy = true);

		Any(const Float32& value, bool copy = true);

		Any(const volatile Float32& value, bool copy = true);

		Any(Float64& value, bool copy = true);

		Any(const Float64& value, bool copy = true);

		Any(const volatile Float64& value, bool copy = true);

		Any(Float128& value, bool copy = true);

		Any(const Float128& value, bool copy = true);

		Any(const volatile Float128& value, bool copy = true);

		Any(const bool&& value, bool noconst, bool copy = true);

		Any(const volatile bool&& value, bool noconst, bool copy = true);

		Any(const Int8&& value, bool noconst, bool copy = true);

		Any(const volatile Int8&& value, bool noconst, bool copy = true);

		Any(const Int16&& value, bool noconst, bool copy = true);

		Any(const volatile Int16&& value, bool noconst, bool copy = true);

		Any(const Int24&& value, bool noconst, bool copy = true);

		Any(const volatile Int24&& value, bool noconst, bool copy = true);

		Any(const Int32&& value, bool noconst, bool copy = true);

		Any(const volatile Int32&& value, bool noconst, bool copy = true);

		Any(const Int64&& value, bool noconst, bool copy = true);

		Any(const volatile Int64&& value, bool noconst, bool copy = true);

		Any(const Int128&& value, bool noconst, bool copy = true);

		Any(const volatile Int128&& value, bool noconst, bool copy = true);

		Any(const ::UInt8&& value, bool noconst, bool copy = true);

		Any(const volatile ::UInt8&& value, bool noconst, bool copy = true);

		Any(const UInt16&& value, bool noconst, bool copy = true);

		Any(const volatile UInt16&& value, bool noconst, bool copy = true);

		Any(const UInt24&& value, bool noconst, bool copy = true);

		Any(const volatile UInt24&& value, bool noconst, bool copy = true);

		Any(const UInt32&& value, bool noconst, bool copy = true);

		Any(const volatile UInt32&& value, bool noconst, bool copy = true);

		Any(const UInt64&& value, bool noconst, bool copy = true);

		Any(const volatile UInt64&& value, bool noconst, bool copy = true);

		Any(const UInt128&& value, bool noconst, bool copy = true);

		Any(const volatile UInt128&& value, bool noconst, bool copy = true);

		Any(const Float32&& value, bool noconst, bool copy = true);

		Any(const volatile Float32&& value, bool noconst, bool copy = true);

		Any(const Float64&& value, bool noconst, bool copy = true);

		Any(const volatile Float64&& value, bool noconst, bool copy = true);

		Any(const Float128&& value, bool noconst, bool copy = true);

		Any(const volatile Float128&& value, bool noconst, bool copy = true);
#pragma endregion

		template<typename T>
		Any(typename notany<T>::value& value, bool copy = true) :  flag(AnyFlags::UnFlagged)
		{
			if (copy)
				storage = new ValueStorage<typename deduce<T>::type>(value);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(&value, ValuePointerKinds::Left);
		}

		template<typename T>
		Any(const T& value, bool copy = true) :  flag(AnyFlags::UnFlagged)
		{
			if (copy)
				storage = new ValueStorage<typename deduce<T>::type>(value);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(&value, ValuePointerKinds::Left);
		}

		template<typename T>
		Any(const volatile T& value, bool copy = true) :  flag(AnyFlags::UnFlagged)
		{
			if (copy)
				storage = new ValueStorage<typename deduce<T>::type>(value);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(&value, ValuePointerKinds::Left);
		}

		template<typename T>
		Any(const T&& value, bool noconst, bool copy = true) :  flag(AnyFlags::UnFlagged)
		{
			if (copy)
				storage = new ValueStorage<typename deduce<T>::type>(value);
			else
			{
				if (noconst)
				{
					storage = new ValuePointerStorage<typename deduce<T>::type>(const_cast<T*>(&value), ValuePointerKinds::Right);
				}
				else
				{
					storage = new ValuePointerStorage<typename deduce<T>::type>(&value, ValuePointerKinds::Right);
				}
			}
		}

		template<typename T>
		Any(const volatile T&& value, bool noconst, bool copy = true) :  flag(AnyFlags::UnFlagged)
		{
			if (copy)
				storage = new ValueStorage<typename deduce<T>::type>(value);
			else
			{
				if (noconst)
					storage = new ValuePointerStorage<typename deduce<T>::type>(const_cast<T*>(&value), ValuePointerKinds::Right);
				else
					storage = new ValuePointerStorage<typename deduce<T>::type>(&value, ValuePointerKinds::Right);
			}
		}

#pragma region Compile Time Value
		template<typename T, size_t SIZE>
		Any(T(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const T(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(volatile T(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const volatile T(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(T(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const T(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(volatile T(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const volatile T(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(const T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(const volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		Any(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4, SIZE5>((typename deduce<T>::type*****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		Any(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4, SIZE5>((typename deduce<T>::type*****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		Any(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4, SIZE5>((typename deduce<T>::type*****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		Any(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticValuePointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4, SIZE5>((typename deduce<T>::type*****)value);
		}
#pragma endregion
#pragma region Compile Time Pointer
		template<typename T, size_t SIZE>
		Any(T*(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const T*(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(volatile T*(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const volatile T*(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(T*(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const T*(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(const T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(const volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(const T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		Any(const volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointerStorage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3, SIZE4>((typename deduce<T>::type****)value);
		}
#pragma endregion
#pragma region Compile Time Pointer2
		template<typename T, size_t SIZE>
		Any(T**(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const T**(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(volatile T**(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const volatile T**(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(T**(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const T**(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(const T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		Any(const volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer2Storage<typename deduce<T>::type, SIZE1, SIZE2, SIZE3>((typename deduce<T>::type***)value);
		}

#pragma endregion
#pragma region Compile Time Pointer3
		template<typename T, size_t SIZE>
		Any(T***(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const T***(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(volatile T***(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const volatile T***(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(T***(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const T***(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		Any(const volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer3Storage<typename deduce<T>::type, SIZE1, SIZE2>((typename deduce<T>::type**)value);
		}
#pragma endregion
#pragma region Compile Time Pointer4
		template<typename T, size_t SIZE>
		Any(T****(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer4Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const T****(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer4Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(volatile T****(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer4Storage<typename deduce<T>::type, SIZE>(value);
		}

		template<typename T, size_t SIZE>
		Any(const volatile T****(&value)[SIZE], bool placeholder) : flag(AnyFlags::UnFlagged)
		{
			storage = new StaticPointer4Storage<typename deduce<T>::type, SIZE>(value);
		}
#pragma endregion

#pragma region Runtime 
		template<typename T>
		Any(T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			if (ptrFlag == AnyDynamicPtrFlags::Ptr)
				storage = new DynamicPointerStorage<typename deduce<T>::type, 0U>(value, arraySize);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(value, ValuePointerKinds::None);
		}

		template<typename T>
		Any(const T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			if (ptrFlag == AnyDynamicPtrFlags::Ptr)
				storage = new DynamicPointerStorage<typename deduce<T>::type, 0U>(value, arraySize);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(value, ValuePointerKinds::None);
		}

		template<typename T>
		Any(volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			if (ptrFlag == AnyDynamicPtrFlags::Ptr)
				storage = new DynamicPointerStorage<typename deduce<T>::type, 0U>(value, arraySize);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(value, ValuePointerKinds::None);
		}

		template<typename T>
		Any(const volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			if (ptrFlag == AnyDynamicPtrFlags::Ptr)
				storage = new DynamicPointerStorage<typename deduce<T>::type, 0U>(value, arraySize);
			else
				storage = new ValuePointerStorage<typename deduce<T>::type>(value, ValuePointerKinds::None);
		}
#pragma endregion
#pragma region Runtime 1
		template<typename T>
		Any(T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size);
					break;				
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS(ptrFlag, AnyDynamicPtrFlags::Ptr)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 1U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}
#pragma endregion
#pragma region Runtime 2
		template<typename T>
		Any(T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS2(ptrFlag, AnyDynamicPtrFlags::Ptr2)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS2(ptrFlag, AnyDynamicPtrFlags::Ptr2)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS2(ptrFlag, AnyDynamicPtrFlags::Ptr2)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS2(ptrFlag, AnyDynamicPtrFlags::Ptr2)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 2U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}
#pragma endregion
#pragma region Runtime 3
		template<typename T>
		Any(T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS3(ptrFlag, AnyDynamicPtrFlags::Ptr3)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS3(ptrFlag, AnyDynamicPtrFlags::Ptr3)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS3(ptrFlag, AnyDynamicPtrFlags::Ptr3)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS3(ptrFlag, AnyDynamicPtrFlags::Ptr3)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 3U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}
#pragma endregion
#pragma region Runtime 4
		template<typename T>
		Any(T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS4(ptrFlag, AnyDynamicPtrFlags::Ptr4)

			switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size, array5Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr4:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS4(ptrFlag, AnyDynamicPtrFlags::Ptr4)

				switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size, array5Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr4:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS4(ptrFlag, AnyDynamicPtrFlags::Ptr4)

				switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size, array5Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr4:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}

		template<typename T>
		Any(const volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : flag(AnyFlags::UnFlagged)
		{
			CORRECTPTRFLAGS4(ptrFlag, AnyDynamicPtrFlags::Ptr4)

				switch (ptrFlag)
			{
				case AnyDynamicPtrFlags::Value:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size, array5Size);
					break;
				case AnyDynamicPtrFlags::Ptr:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size, array4Size);
					break;
				case AnyDynamicPtrFlags::Ptr2:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size, array3Size);
					break;
				case AnyDynamicPtrFlags::Ptr3:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size, array2Size);
					break;
				case AnyDynamicPtrFlags::Ptr4:
					storage = new DynamicPointerStorage<typename deduce<T>::type, 4U>(value, array1Size);
					break;
				default:
					storage = nullptr;
					break;
			}
		}
#pragma endregion

		Any(const Any& other);

		Any(Any&& other);

		Any & swap(Any & rhs);

		Any & operator=(const Any& rhs);

		Any & operator=(Any&& rhs);

		static __forceinline Any Empty()
		{
			return Any();
		}

		_type GetType() const;

		const std::type_info & GetSTDRTTI() const;

		::UInt8 GetDimension() const;

		size_t GetDimensionSize(::UInt8 index) const;

		bool GetIsNull() const;

		bool GetIsMissing() const;

		bool GetIsNoDefault() const;

		bool GetIsValueable() const;

		Object* AllocateObject() const;

		template<size_t N>
		typename add_pointer_ex<Object, N>::type ToObject() const;

		template<>
		typename add_pointer_ex<Object, 1>::type ToObject<1>() const
		{
			return ObjectGetter_impl();
		}

		template<>
		typename add_pointer_ex<Object, 2>::type ToObject<2>() const
		{
			return ObjectGetter_impl1();
		}

		template<>
		typename add_pointer_ex<Object, 3>::type ToObject<3>() const
		{
			return ObjectGetter_impl2();
		}

		template<>
		typename add_pointer_ex<Object, 4>::type ToObject<4>() const
		{
			return ObjectGetter_impl3();
		}

		template<>
		typename add_pointer_ex<Object, 5>::type ToObject<5>() const
		{
			return ObjectGetter_impl4();
		}

		__declspec(property (get = GetType)) _type Type;
		__declspec(property (get = GetSTDRTTI)) const std::type_info& STDRTTI;
		__declspec(property (get = GetDimension)) ::UInt8 Dimension;
		__declspec(property (get = GetDimensionSize)) size_t DimensionSize[];
		__declspec(property (get = GetIsNull)) bool IsNull;
		__declspec(property (get = GetIsMissing)) bool IsMissing;
		__declspec(property (get = GetIsNoDefault)) bool IsNoDefault;
		__declspec(property (get = GetIsValueable)) bool IsValueable;


#pragma region Value Cast
		template<class T>
		typename deduce<T>::type ValueCast(ModifierPolicy policy=ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0) const;

		template<class T>
		typename std::add_const<typename deduce<T>::type>::type ValueCast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0) const;

		template<class T>
		typename std::add_volatile<typename deduce<T>::type>::type ValueCast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0) const;

		template<class T>
		typename std::add_cv<typename deduce<T>::type>::type ValueCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0) const;
#pragma endregion

#pragma region Ptr Cast
		template<class T>
		typename std::add_pointer<typename deduce<T>::type>::type PtrCast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow=true) const;

		template<class T>
		typename std::add_pointer<typename std::add_const<typename deduce<T>::type>::type>::type PtrCast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;

		template<class T>
		typename std::add_pointer<typename std::add_volatile<typename deduce<T>::type>::type>::type PtrCast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;

		template<class T>
		typename std::add_pointer<typename std::add_cv<typename deduce<T>::type>::type>::type PtrCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Ptr2 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 2U>::type Ptr2Cast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 2U>::type Ptr2Cast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 2U>::type Ptr2Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 2U>::type Ptr2Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Ptr3 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 3U>::type Ptr3Cast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 3U>::type Ptr3Cast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 3U>::type Ptr3Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 3U>::type Ptr3Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Ptr4 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 4U>::type Ptr4Cast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 4U>::type Ptr4Cast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 4U>::type Ptr4Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 4U>::type Ptr4Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Ptr5 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 5U>::type Ptr5Cast(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 5U>::type Ptr5Cast_Const(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 5U>::type Ptr5Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 5U>::type Ptr5Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;
#pragma endregion

#pragma region LReference Cast
		template<class T>
		typename std::add_lvalue_reference<typename deduce<T>::type>::type LRefCast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type LRefCast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type LRefCast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type LRefCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region RReference Cast
		template<class T>
		typename std::add_rvalue_reference<typename deduce<T>::type>::type RRefCast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type RRefCast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type RRefCast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type RRefCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region LReference Ptr Cast
		template<class T>
		typename std::add_lvalue_reference<typename std::add_pointer<typename deduce<T>::type>::type>::type LRefPtrCast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type LRefPtrCast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type LRefPtrCast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type LRefPtrCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region RReference Ptr Cast
		template<class T>
		typename std::add_rvalue_reference<typename std::add_pointer<typename deduce<T>::type>::type>::type RRefPtrCast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type RRefPtrCast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type RRefPtrCast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type RRefPtrCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region LReference Ptr2 Cast
		template<class T>
		typename std::add_lvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type LRefPtr2Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type>::type LRefPtr2Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type>::type LRefPtr2Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type>::type LRefPtr2Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region RReference Ptr2 Cast
		template<class T>
		typename std::add_rvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type RRefPtr2Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type>::type RRefPtr2Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type>::type RRefPtr2Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 2U>::type>::type>::type RRefPtr2Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region LReference Ptr3 Cast
		template<class T>
		typename std::add_lvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type LRefPtr3Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type>::type LRefPtr3Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type>::type LRefPtr3Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type>::type LRefPtr3Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region RReference Ptr3 Cast
		template<class T>
		typename std::add_rvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type RRefPtr3Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type>::type RRefPtr3Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type>::type RRefPtr3Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 3U>::type>::type>::type RRefPtr3Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region LReference Ptr4 Cast
		template<class T>
		typename std::add_lvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type LRefPtr4Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type>::type LRefPtr4Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type>::type LRefPtr4Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type>::type LRefPtr4Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region RReference Ptr4 Cast
		template<class T>
		typename std::add_rvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type RRefPtr4Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type>::type RRefPtr4Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type>::type RRefPtr4Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 4U>::type>::type>::type RRefPtr4Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region LReference Ptr5 Cast
		template<class T>
		typename std::add_lvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type LRefPtr5Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type>::type LRefPtr5Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type>::type LRefPtr5Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type>::type LRefPtr5Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region RReference Ptr5 Cast
		template<class T>
		typename std::add_rvalue_reference<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type RRefPtr5Cast(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type>::type RRefPtr5Cast_Const(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type>::type RRefPtr5Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General) const;

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename add_pointer_ex<typename deduce<T>::type, 5U>::type>::type>::type RRefPtr5Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General) const;
#pragma endregion

#pragma region Void Cast
		void* VoidCast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;

		const void* VoidCast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;

		volatile void* VoidCast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;

		const volatile void* VoidCast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Void2 Cast
		void** Void2Cast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;

		const void** Void2Cast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;

		volatile void** Void2Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;

		const volatile void** Void2Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Void3 Cast
		void*** Void3Cast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;

		const void*** Void3Cast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;

		volatile void*** Void3Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;

		const volatile void*** Void3Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Void4 Cast
		void**** Void4Cast(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;

		const void**** Void4Cast_Const(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;

		volatile void**** Void4Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;

		const volatile void**** Void4Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, bool nothrow = true) const;
#pragma endregion

#pragma region Void5 Cast
		void***** Void5Cast(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;

		const void***** Void5Cast_Const(ModifierPolicy policy = ModifierPolicy::General,  bool nothrow = true) const;

		volatile void***** Void5Cast_Volatile(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;

		const volatile void***** Void5Cast_ConstVolatile(ModifierPolicy policy = ModifierPolicy::General, bool nothrow = true) const;
#pragma endregion

		template<class U>
	    operator U() const;

		template<class U>
		U as() const;

		bool TryGetString(std::wstring& str, const CultureInfo& loc) const;

	};

#pragma region ValueStorage
	template<typename T>
	const System::Type* Any::ValueStorage<T>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(T);
		case Modifiers::Const:
			return _typeof(const T);
		case Modifiers::Volatile:
			return _typeof(volatile T);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile T);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::ValueStorage<T>::getBaseType() const
	{
		return _typeof(T);
	}

	template<typename T, size_t N1>
	const System::Type* Any::StaticValuePointerStorage<T, N1>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base[N1]);
		case Modifiers::Const:
			return _typeof(const base[N1]);
		case Modifiers::Volatile:
			return _typeof(volatile base[N1]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base[N1]);
		}
		return nullptr;
	}

	template<typename T, size_t N1>
	const System::Type* Any::StaticValuePointerStorage<T, N1>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base[N1][N2]);
		case Modifiers::Const:
			return _typeof(const base[N1][N2]);
		case Modifiers::Volatile:
			return _typeof(volatile base[N1][N2]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base[N1][N2]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2, size_t N3>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2, N3>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base);
		case Modifiers::Const:
			return _typeof(const base[N1][N2][N3]);
		case Modifiers::Volatile:
			return _typeof(volatile base[N1][N2][N3]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base[N1][N2][N3]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2, size_t N3>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2, N3>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2, size_t N3, size_t N4>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2, N3, N4>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base[N1][N2][N3][N4]);
		case Modifiers::Const:
			return _typeof(const base[N1][N2][N3][N4]);
		case Modifiers::Volatile:
			return _typeof(volatile base[N1][N2][N3][N4]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base[N1][N2][N3][N4]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2, size_t N3, size_t N4>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2, N3, N4>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2, size_t N3, size_t N4, size_t N5>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2, N3, N4, N5>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base[N1][N2][N3][N4][N5]);
		case Modifiers::Const:
			return _typeof(const base[N1][N2][N3][N4][N5]);
		case Modifiers::Volatile:
			return _typeof(volatile base[N1][N2][N3][N4][N5]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base[N1][N2][N3][N4][N5]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2, size_t N3, size_t N4, size_t N5>
	const System::Type* Any::StaticValuePointerStorage<T, N1, N2, N3, N4, N5>::getBaseType() const
	{
		return _typeof(base);
	}

#pragma endregion

#pragma region StaticPointerStorage

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointerStorage<T, N1>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base*[N1]);
		case Modifiers::Const:
			return _typeof(const base*[N1]);
		case Modifiers::Volatile:
			return _typeof(volatile base*[N1]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*[N1]);
		}
		return nullptr;
	}

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointerStorage<T, N1>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticPointerStorage<T, N1, N2>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base*[N1][N2]);
		case Modifiers::Const:
			return _typeof(const base*[N1][N2]);
		case Modifiers::Volatile:
			return _typeof(volatile base*[N1][N2]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*[N1][N2]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticPointerStorage<T, N1, N2>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2, size_t N3>
	const System::Type* Any::StaticPointerStorage<T, N1, N2, N3>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base);
		case Modifiers::Const:
			return _typeof(const base*[N1][N2][N3]);
		case Modifiers::Volatile:
			return _typeof(volatile base*[N1][N2][N3]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*[N1][N2][N3]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2, size_t N3>
	const System::Type* Any::StaticPointerStorage<T, N1, N2, N3>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2, size_t N3, size_t N4>
	const System::Type* Any::StaticPointerStorage<T, N1, N2, N3, N4>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base*[N1][N2][N3][N4]);
		case Modifiers::Const:
			return _typeof(const base*[N1][N2][N3][N4]);
		case Modifiers::Volatile:
			return _typeof(volatile base*[N1][N2][N3][N4]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*[N1][N2][N3][N4]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2, size_t N3, size_t N4>
	const System::Type* Any::StaticPointerStorage<T, N1, N2, N3, N4>::getBaseType() const
	{
		return _typeof(base);
	}
#pragma endregion

#pragma region StaticPointer2Storage

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointer2Storage<T, N1>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base**[N1]);
		case Modifiers::Const:
			return _typeof(const base**[N1]);
		case Modifiers::Volatile:
			return _typeof(volatile base**[N1]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base**[N1]);
		}
		return nullptr;
	}

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointer2Storage<T, N1>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticPointer2Storage<T, N1, N2>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base**[N1][N2]);
		case Modifiers::Const:
			return _typeof(const base**[N1][N2]);
		case Modifiers::Volatile:
			return _typeof(volatile base**[N1][N2]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base**[N1][N2]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticPointer2Storage<T, N1, N2>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2, size_t N3>
	const System::Type* Any::StaticPointer2Storage<T, N1, N2, N3>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base);
		case Modifiers::Const:
			return _typeof(const base**[N1][N2][N3]);
		case Modifiers::Volatile:
			return _typeof(volatile base**[N1][N2][N3]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base**[N1][N2][N3]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2, size_t N3>
	const System::Type* Any::StaticPointer2Storage<T, N1, N2, N3>::getBaseType() const
	{
		return _typeof(base);
	}

#pragma endregion

#pragma region StaticPointer3Storage

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointer3Storage<T, N1>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base***[N1]);
		case Modifiers::Const:
			return _typeof(const base***[N1]);
		case Modifiers::Volatile:
			return _typeof(volatile base***[N1]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base***[N1]);
		}
		return nullptr;
	}

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointer3Storage<T, N1>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticPointer3Storage<T, N1, N2>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base***[N1][N2]);
		case Modifiers::Const:
			return _typeof(const base***[N1][N2]);
		case Modifiers::Volatile:
			return _typeof(volatile base***[N1][N2]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base***[N1][N2]);
		}
		return nullptr;
	}

	template<typename T, size_t N1, size_t N2>
	const System::Type* Any::StaticPointer3Storage<T, N1, N2>::getBaseType() const
	{
		return _typeof(base);
	}

#pragma endregion

#pragma region StaticPointer4Storage

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointer4Storage<T, N1>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base****[N1]);
		case Modifiers::Const:
			return _typeof(const base****[N1]);
		case Modifiers::Volatile:
			return _typeof(volatile base****[N1]);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base****[N1]);
		}
		return nullptr;
	}

	template<typename T, size_t N1>
	const System::Type* Any::StaticPointer4Storage<T, N1>::getBaseType() const
	{
		return _typeof(base);
	}

#pragma endregion

#pragma region DynamicPointerStorage
	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 0U>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base*);
		case Modifiers::Const:
			return _typeof(const base*);
		case Modifiers::Volatile:
			return _typeof(volatile base*);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 0U>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 1U>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base**);
		case Modifiers::Const:
			return _typeof(const base**);
		case Modifiers::Volatile:
			return _typeof(volatile base**);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base**);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 1U>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 2U>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base***);
		case Modifiers::Const:
			return _typeof(const base***);
		case Modifiers::Volatile:
			return _typeof(volatile base***);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base***);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 2U>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 3U>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base****);
		case Modifiers::Const:
			return _typeof(const base****);
		case Modifiers::Volatile:
			return _typeof(volatile base****);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base****);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 3U>::getBaseType() const
	{
		return _typeof(base);
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 4U>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base*****);
		case Modifiers::Const:
			return _typeof(const base*****);
		case Modifiers::Volatile:
			return _typeof(volatile base*****);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*****);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::DynamicPointerStorage<T, 4U>::getBaseType() const
	{
		return _typeof(base);
	}
#pragma endregion

#pragma region ValuePointerStorage
	template<typename T>
	const System::Type* Any::ValuePointerStorage<T>::getType() const
	{
		switch (modifier)
		{
		case Modifiers::None:
		default:
			return _typeof(base*);
		case Modifiers::Const:
			return _typeof(const base*);
		case Modifiers::Volatile:
			return _typeof(volatile base*);
		case Modifiers::ConstVolatile:
			return _typeof(const volatile base*);
		}
		return nullptr;
	}

	template<typename T>
	const System::Type* Any::ValuePointerStorage<T>::getBaseType() const
	{
		return _typeof(base);
	}
#pragma endregion

#pragma region Value Cast
	template<class T>
	typename deduce<T>::type Any::ValueCast(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
	{
		typedef typename deduce<T>::type result;
		if (storage)
		{
			AnyStorageTypes ast = storage->storageType();
			switch (ast)
			{
				case  AnyStorageTypes::Number:
				{
					NumberStorage<result>* ns = (NumberStorage<result>*)(storage);
					return ns->Get<result>();
				}
					break;
				case AnyStorageTypes::Value:
				{
					if (typeid(result) == storage->type())
					{
						ValueStorage<result>* vs = (ValueStorage<result>*)(storage);
						return vs->val;
					}
					if (std::is_base_of<Object, result>::value)
					{
						_type ty = storage->getType();
						if (ty)
						{
							if (ty->IsConvertible(_typeof(result)))
							{
								void* mem = nullptr;
								size_t size = storage->super(mem);
								if (size > 0 && mem)
								{
									if (OSHelper::IsValidAddress(mem, sizeof(result), false))
									{
										result* res = (result*)mem;
										return *res;
									}
								}
							}
						}
					}
					if (storage->polymorphic() && std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								if (_RTDynamicCastReviewer::Check<result>(mem, storage->basetype()))
								{
									result* res = (result*)mem;
									return *res;
								}
							}
						}
					}
					if (AnyConversion::IsSupport<result>(storage->basetype()))
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								result* res = (result*)mem;
								return *res;
							}
						}
					}
				}
					break;
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					result* pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(result));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<result>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
									pp = (result*)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									pp = (result*)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return *pp;
					}
				}
					break;
				case AnyStorageTypes::StaticValuePtr:
				{
					bool test = typeid(result) == storage->type();
					result* pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(result));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<result>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
									pp = (result*)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									pp = (result*)mem;
								}
							}
						}
						if (!pp) goto ERR;
						size_t nindex = 0U;
						if (storage->index(nindex, index1, index2, index3, index4, index5))
						{
							switch (storage->modifier)
							{
							case Modifiers::None:
								break;
							case Modifiers::Const:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
								break;
							case Modifiers::Volatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
								break;
							case Modifiers::ConstVolatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
								break;
							default:
								goto ERR;
								break;
							}
							return pp[nindex];
						}
					}
				}
					break;
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->basetype();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(result));
					}
					if (!test)
					{
						if (std::is_polymorphic<result>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, index5) > 0U)
						{
							switch (storage->modifier)
							{
							case Modifiers::None:
								break;
							case Modifiers::Const:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
								break;
							case Modifiers::Volatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
								break;
							case Modifiers::ConstVolatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
								break;
							default:
								goto ERR;
								break;
							}
							if (tmp)
							{
								result* res = (result*)tmp;
								return val? res[nindex] : *res;
							}	
						}
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

	template<class T>
	typename std::add_const<typename deduce<T>::type>::type Any::ValueCast_Const(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
	{
		typedef typename deduce<T>::type base;
		typedef typename std::add_const<typename deduce<T>::type>::type result;
		if (storage)
		{
			AnyStorageTypes ast = storage->storageType();
			switch (ast)
			{
			case  AnyStorageTypes::Number:
			{
				NumberStorage<base>* ns = (NumberStorage<base>*)(storage);
				return ns->Get<result>();
			}
				break;
			case AnyStorageTypes::Value:
			{
				if (typeid(result) == storage->type())
				{
					ValueStorage<base>* vs = (ValueStorage<base>*)(storage);
					return vs->val;
				}
				if (std::is_base_of<Object, result>::value)
				{
					_type ty = storage->getType();
					if (ty)
					{
						if (ty->IsConvertible(_typeof(base)))
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									result* res = (result*)mem;
									return *res;
								}
							}
						}
					}
				}
			}
				break;
			case AnyStorageTypes::StaticValuePtr:
			{
				bool test = typeid(result) == storage->type();
				result* pp = nullptr;
				if (!test)
				{
					_type ty = storage->getBaseType();
					if (ty) test = ty->IsConvertible(_typeof(result));
				}
				if (!test)
				{
					if (storage->polymorphic() && std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
								pp = (result*)mem;
							}
						}
					}
					if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
				}
				if (test)
				{
					if (!pp)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								pp = (result*)mem;
							}
						}
					}
					if (!pp) goto ERR;
					size_t nindex = 0U;
					if (storage->index(nindex, index1, index2, index3, index4, index5))
					{
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp[nindex];
					}
				}
			}
				break; 
			case AnyStorageTypes::StaticPtr:
			case AnyStorageTypes::DynamicPtr:
			{
				bool test = typeid(result) == storage->basetype();
				if (!test)
				{
					_type ty = storage->getBaseType();
					if (ty) test = ty->IsConvertible(_typeof(result));
				}
				if (!test)
				{
					if (std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						if (storage->superpolymorphic(mem))
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
							}
						}
					}
					if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
				}
				if (test)
				{
					bool val = false;
					size_t nindex = 0U;
					void* tmp = nullptr;
					::UInt8 dem = 0U;
					if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, index5) > 0U)
					{
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						if (tmp)
						{
							result* res = (result*)tmp;
							return val ? res[nindex] : *res;
						}
					}
				}
			}
				break;
			}
		}
	ERR:
		throw bad_any_cast("empty", AnyErrors::General);

	}

	template<class T>
	typename std::add_volatile<typename deduce<T>::type>::type Any::ValueCast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
	{
		typedef typename deduce<T>::type base;
		typedef typename std::add_volatile<typename deduce<T>::type>::type result;
		if (storage)
		{
			AnyStorageTypes ast = storage->storageType();
			switch (ast)
			{
			case  AnyStorageTypes::Number:
			{
				NumberStorage<base>* ns = (NumberStorage<base>*)(storage);
				return ns->Get<result>();
			}
				break;
			case AnyStorageTypes::Value:
			{
				if (typeid(result) == storage->type())
				{
					ValueStorage<base>* vs = (ValueStorage<base>*)(storage);
					return vs->val;
				}
				if (std::is_base_of<Object, result>::value)
				{
					_type ty = storage->getType();
					if (ty)
					{
						if (ty->IsConvertible(_typeof(base)))
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									result* res = (result*)mem;
									return *res;
								}
							}
						}
					}
				}
			}
				break;
			case AnyStorageTypes::StaticValuePtr:
			{
				bool test = typeid(result) == storage->type();
				result* pp = nullptr;
				if (!test)
				{
					_type ty = storage->getBaseType();
					if (ty) test = ty->IsConvertible(_typeof(result));
				}
				if (!test)
				{
					if (storage->polymorphic() && std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
								pp = (result*)mem;
							}
						}
					}
					if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
				}
				if (test)
				{
					if (!pp)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								pp = (result*)mem;
							}
						}
					}
					if (!pp) goto ERR;
					size_t nindex = 0U;
					if (storage->index(nindex, index1, index2, index3, index4, index5))
					{
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp[nindex];
					}
				}
			}
				break;
			case AnyStorageTypes::StaticPtr:
			case AnyStorageTypes::DynamicPtr:
			{
				bool test = typeid(result) == storage->basetype();
				if (!test)
				{
					_type ty = storage->getBaseType();
					if (ty) test = ty->IsConvertible(_typeof(result));
				}
				if (!test)
				{
					if (std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						if (storage->superpolymorphic(mem))
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
							}
						}
					}
					if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
				}
				if (test)
				{
					bool val = false;
					size_t nindex = 0U;
					void* tmp = nullptr;
					if (storage->superpointer(val, nindex, tmp, index1, index2, index3, index4, index5) > 0U)
					{
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						if (tmp)
						{
							result* res = (result*)tmp;
							return val ? res[nindex] : *res;
						}
					}
				}
			}
				break;
			}
		}
	ERR:
		throw bad_any_cast("empty", AnyErrors::General);
	}

	template<class T>
	typename std::add_cv<typename deduce<T>::type>::type Any::ValueCast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, size_t index5) const
	{
		typedef typename deduce<T>::type base;
		typedef typename std::add_cv<typename deduce<T>::type>::type result;
		if (storage)
		{
			AnyStorageTypes ast = storage->storageType();
			switch (ast)
			{
			case  AnyStorageTypes::Number:
			{
				NumberStorage<base>* ns = (NumberStorage<base>*)(storage);
				return ns->Get<result>();
			}
				break;
			case AnyStorageTypes::Value:
			{
				if (typeid(result) == storage->type())
				{
					ValueStorage<base>* vs = (ValueStorage<base>*)(storage);
					return vs->val;
				}
				if (std::is_base_of<Object, result>::value)
				{
					_type ty = storage->getType();
					if (ty)
					{
						if (ty->IsConvertible(_typeof(base)))
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(result), false))
								{
									result* res = (result*)mem;
									return *res;
								}
							}
						}
					}
				}
			}
				break;
			case AnyStorageTypes::StaticValuePtr:
			{
				bool test = typeid(result) == storage->type();
				result* pp = nullptr;
				if (!test)
				{
					_type ty = storage->getBaseType();
					if (ty) test = ty->IsConvertible(_typeof(result));
				}
				if (!test)
				{
					if (storage->polymorphic() && std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
								pp = (result*)mem;
							}
						}
					}
					if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
				}
				if (test)
				{
					if (!pp)
					{
						void* mem = nullptr;
						size_t size = storage->super(mem);
						if (size > 0 && mem)
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								pp = (result*)mem;
							}
						}
					}
					if (!pp) goto ERR;
					size_t nindex = 0U;
					if (storage->index(nindex, index1, index2, index3, index4, index5))
					{
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						return pp[nindex];
					}
				}
			}
				break;
			case AnyStorageTypes::StaticPtr:
			case AnyStorageTypes::DynamicPtr:
			{
				bool test = typeid(result) == storage->basetype();
				if (!test)
				{
					_type ty = storage->getBaseType();
					if (ty) test = ty->IsConvertible(_typeof(result));
				}
				if (!test)
				{
					if (std::is_polymorphic<result>::value)
					{
						void* mem = nullptr;
						if (storage->superpolymorphic(mem))
						{
							if (OSHelper::IsValidAddress(mem, sizeof(result), false))
							{
								test = _RTDynamicCastReviewer::Check<result>(mem, storage->basetype());
							}
						}
					}
					if (!test) test = AnyConversion::IsSupport<result>(storage->basetype());
				}
				if (test)
				{
					bool val = false;
					size_t nindex = 0U;
					void* tmp = nullptr;
					if (storage->superpointer(val, nindex, tmp, index1, index2, index3, index4, index5) > 0U)
					{
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						if (tmp)
						{
							result* res = (result*)tmp;
							return val ? res[nindex] : *res;
						}
					}
				}
			}
				break;
			}
		}
	ERR:
		throw bad_any_cast("empty", AnyErrors::General);
	}
#pragma endregion

#pragma region Ptr Cast
		template<class T>
		typename std::add_pointer<typename deduce<T>::type>::type Any::PtrCast(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					result pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (result)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (result)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
					break;
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, 0U) > 0U)
						{
							switch (storage->modifier)
							{
							case Modifiers::None:
								break;
							case Modifiers::Const:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
								break;
							case Modifiers::Volatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
								break;
							case Modifiers::ConstVolatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
								break;
							default:
								goto ERR;
								break;
							}
							if (tmp && !val)
							{
								return (result)tmp;
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if(!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename std::add_pointer<typename std::add_const<typename deduce<T>::type>::type>::type Any::PtrCast_Const(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename std::add_const<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					result pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (result)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (result)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
					break;
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, index4, 0U) > 0U)
						{
							switch (storage->modifier)
							{
							case Modifiers::None:
								break;
							case Modifiers::Const:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
								break;
							case Modifiers::Volatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
								break;
							case Modifiers::ConstVolatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
								break;
							default:
								goto ERR;
								break;
							}
							if (tmp && !val)
							{
								return (result)tmp;
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;

		}

		template<class T>
		typename std::add_pointer<typename std::add_volatile<typename deduce<T>::type>::type>::type Any::PtrCast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename std::add_volatile<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					result pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (result)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (result)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
					break;
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						if (storage->superpointer(val, nindex, tmp, index1, index2, index3, index4, 0U) > 0U)
						{
							switch (storage->modifier)
							{
							case Modifiers::None:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
								break;
							case Modifiers::Const:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
								break;
							case Modifiers::Volatile:
								break;
							case Modifiers::ConstVolatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
								break;
							default:
								goto ERR;
								break;
							}
							if (tmp && !val)
							{
								return (result)tmp;
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;

		}

		template<class T>
		typename std::add_pointer<typename std::add_cv<typename deduce<T>::type>::type>::type Any::PtrCast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, size_t index4, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename std::add_cv<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					result pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (result)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (result)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
					break;
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						if (storage->superpointer(val, nindex, tmp, index1, index2, index3, index4, 0U) > 0U)
						{
							switch (storage->modifier)
							{
							case Modifiers::None:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
								break;
							case Modifiers::Const:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
								break;
							case Modifiers::Volatile:
								if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
								break;
							case Modifiers::ConstVolatile:
								break;
							default:
								goto ERR;
								break;
							}
							if (tmp && !val)
							{
								return (result)tmp;
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;

		}
#pragma endregion

#pragma region Ptr2 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 2U>::type Any::Ptr2Cast(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename deduce<T>::type2U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
						{
							if (dem == 2U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 2U>::type Any::Ptr2Cast_Const(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 2U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
						{
							if (dem == 2U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
									break;
								case Modifiers::Const:
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;

		}

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 2U>::type Any::Ptr2Cast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 2U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
						{
							if (dem == 2U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 2U>::type Any::Ptr2Cast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, size_t index3, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 2U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, index3, 0U, 0U) > 0U)
						{
							if (dem == 2U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}
#pragma endregion

#pragma region Ptr3 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 3U>::type Any::Ptr3Cast(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename deduce<T>::type, 3U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
						{
							if (dem == 3U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 3U>::type Any::Ptr3Cast_Const(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 3U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
						{
							if (dem == 3U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
									break;
								case Modifiers::Const:
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 3U>::type Any::Ptr3Cast_Volatile(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 3U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
						{
							if (dem == 3U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 3U>::type Any::Ptr3Cast_ConstVolatile(ModifierPolicy policy, size_t index1, size_t index2, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 3U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, index2, 0U, 0U, 0U) > 0U)
						{
							if (dem == 3U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}
#pragma endregion

#pragma region Ptr4 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 4U>::type Any::Ptr4Cast(ModifierPolicy policy, size_t index1, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename deduce<T>::type, 4U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 4U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 4U>::type Any::Ptr4Cast_Const(ModifierPolicy policy, size_t index1, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 4U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 4U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
									break;
								case Modifiers::Const:
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 4U>::type Any::Ptr4Cast_Volatile(ModifierPolicy policy, size_t index1, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 4U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 4U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 4U>::type Any::Ptr4Cast_ConstVolatile(ModifierPolicy policy, size_t index1, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 4U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, index1, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 4U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}
#pragma endregion

#pragma region Ptr5 Cast
		template<class T>
		typename add_pointer_ex<typename deduce<T>::type, 5U>::type Any::Ptr5Cast(ModifierPolicy policy, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename deduce<T>::type, 5U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 5U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 5U>::type Any::Ptr5Cast_Const(ModifierPolicy policy, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 5U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 5U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
									break;
								case Modifiers::Const:
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 5U>::type Any::Ptr5Cast_Volatile(ModifierPolicy policy, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 5U>::typ result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 5U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									break;
								case Modifiers::ConstVolatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}

		template<class T>
		typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 5U>::type Any::Ptr5Cast_ConstVolatile(ModifierPolicy policy, bool nothrow) const
		{
			typedef typename deduce<T>::type base;
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 5U>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::StaticPtr:
				case AnyStorageTypes::DynamicPtr:
				{
					bool test = typeid(result) == storage->type();
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							if (storage->superpolymorphic(mem))
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						bool val = false;
						size_t nindex = 0U;
						void* tmp = nullptr;
						::UInt8 dem = 0U;
						if (storage->superpointer(val, nindex, dem, tmp, 0U, 0U, 0U, 0U, 0U) > 0U)
						{
							if (dem == 5U)
							{
								switch (storage->modifier)
								{
								case Modifiers::None:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Const:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
									break;
								case Modifiers::Volatile:
									if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
									break;
								case Modifiers::ConstVolatile:
									break;
								default:
									goto ERR;
									break;
								}
								if (tmp && !val)
								{
									return (result)tmp;
								}
							}
						}
					}
				}
					break;
				}
			}
		ERR:
			if (!nothrow) throw bad_any_cast("empty", AnyErrors::General);
			return nullptr;
		}
#pragma endregion

#pragma region LReference Cast
		template<class T>
		typename std::add_lvalue_reference<typename deduce<T>::type>::type Any::LRefCast(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename deduce<T>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return *pp;
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type Any::LRefCast_Const(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return *pp;
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type Any::LRefCast_Volatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return *pp;
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type Any::LRefCast_ConstVolatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						return *pp;
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}
#pragma endregion

#pragma region RReference Cast
		template<class T>
		typename std::add_rvalue_reference<typename deduce<T>::type>::type Any::RRefCast(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename deduce<T>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(*pp);
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);

		}

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type Any::RRefCast_Const(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(*pp);
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type Any::RRefCast_Volatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(*pp);
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type Any::RRefCast_ConstVolatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(*pp);
					}
				}
					break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}
#pragma endregion

#pragma region LReference Ptr Cast
		template<class T>
		typename std::add_lvalue_reference<typename std::add_pointer<typename deduce<T>::type>::type>::type Any::LRefPtrCast(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename deduce<T>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_lvalue_reference<typename std::add_const<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type Any::LRefPtrCast_Const(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_lvalue_reference<typename std::add_volatile<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type Any::LRefPtrCast_Volatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_lvalue_reference<typename std::add_cv<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type Any::LRefPtrCast_ConstVolatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						return pp;
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}
#pragma endregion

#pragma region RReference Ptr Cast
		template<class T>
		typename std::add_rvalue_reference<typename std::add_pointer<typename deduce<T>::type>::type>::type Any::RRefPtrCast(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename deduce<T>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2None)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2None)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2None)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(pp);
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);

		}

		template<class T>
		typename std::add_rvalue_reference<typename std::add_const<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type Any::RRefPtrCast_Const(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Const)) goto ERR;
							break;
						case Modifiers::Const:
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2Const)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Const)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(pp);
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_rvalue_reference<typename std::add_volatile<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type Any::RRefPtrCast_Volatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2Volatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2Volatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							break;
						case Modifiers::ConstVolatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConstVolatile2Volatile)) goto ERR;
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(pp);
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}

		template<class T>
		typename std::add_rvalue_reference<typename std::add_cv<typename std::add_pointer<typename deduce<T>::type>::type>::type>::type Any::RRefPtrCast_ConstVolatile(ModifierPolicy policy) const
		{
			typedef typename deduce<T>::type base;
			typedef typename std::add_pointer<typename deduce<T>::type>::type pbase;
			typedef typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type result;
			if (storage)
			{
				AnyStorageTypes ast = storage->storageType();
				switch (ast)
				{
				case AnyStorageTypes::ValuePtr:
				{
					bool test = typeid(result) == storage->type();
					pbase pp = nullptr;
					if (!test)
					{
						_type ty = storage->getBaseType();
						if (ty) test = ty->IsConvertible(_typeof(base));
					}
					if (!test)
					{
						if (storage->polymorphic() && std::is_polymorphic<base>::value)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									test = _RTDynamicCastReviewer::Check<base>(mem, storage->basetype());
									pp = (pbase)mem;
								}
							}
						}
						if (!test) test = AnyConversion::IsSupport<base>(storage->basetype());
					}
					if (test)
					{
						if (!pp)
						{
							void* mem = nullptr;
							size_t size = storage->super(mem);
							if (size > 0 && mem)
							{
								if (OSHelper::IsValidAddress(mem, sizeof(typename std::conditional<std::is_same<void, base>::value, void*, base>::type), false))
								{
									pp = (pbase)mem;
								}
							}
						}
						if (!pp) goto ERR;
						switch (storage->modifier)
						{
						case Modifiers::None:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreNone2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Const:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreConst2ConstVolatile)) goto ERR;
							break;
						case Modifiers::Volatile:
							if (!Enums::Contains(policy, ModifierPolicy::IgnoreVolatile2ConstVolatile)) goto ERR;
							break;
						case Modifiers::ConstVolatile:
							break;
						default:
							goto ERR;
							break;
						}
						return std::move(pp);
					}
				}
				break;
				}
			}
		ERR:
			throw bad_any_cast("empty", AnyErrors::General);
		}
#pragma endregion

#pragma region AnyVisitor

		template< typename T, bool isvoid=std::is_void<typename deduce<T>::type>::value, Modifiers modifier = DetectModifier<T>::modifier, Declarators declarator = DetectDeclarator<T>::declarator, size_t demesion = DetectDeclarator<T>::dimension>
		struct AnyVisitor
		{
		};

#pragma region AnyVisitor Void
		template< typename T>
		struct AnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 1U>
		{
		public:
			static void* Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.VoidCast(policy, index1, index2, index3, index4);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 1U>
		{
		public:
			static const void* Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.VoidCast_Const(policy, index1, index2, index3, index4);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 1U>
		{
		public:
			static volatile void* Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.VoidCast_Volatile(policy, index1, index2, index3, index4);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 1U>
		{
		public:
			static const volatile void* Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.VoidCast_ConstVolatile(policy, index1, index2, index3, index4);
			}
		};
#pragma endregion
		
#pragma region AnyVisitor Void2
		template< typename T>
		struct AnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 2U>
		{
		public:
			static void** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Void2Cast(policy, index1, index2, index3);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 2U>
		{
		public:
			static const void** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Void2Cast_Const(policy, index1, index2, index3);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 2U>
		{
		public:
			static volatile void** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Void2Cast_Volatile(policy, index1, index2, index3);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 2U>
		{
		public:
			static const volatile void** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Void2Cast_ConstVolatile(policy, index1, index2, index3);
			}
		};
#pragma endregion

#pragma region AnyVisitor Void3
		template< typename T>
		struct AnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 3U>
		{
		public:
			static void*** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Void3Cast(policy, index1, index2);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 3U>
		{
		public:
			static const void*** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Void3Cast_Const(policy, index1, index2);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 3U>
		{
		public:
			static volatile void*** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Void3Cast_Volatile(policy, index1, index2);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 3U>
		{
		public:
			static const volatile void*** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Void3Cast_ConstVolatile(policy, index1, index2);
			}
		};
#pragma endregion

#pragma region AnyVisitor Void4
		template< typename T>
		struct AnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 4U>
		{
		public:
			static void**** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Void4Cast(policy, index1);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 4U>
		{
		public:
			static const void**** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Void4Cast_Const(policy, index1);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 4U>
		{
		public:
			static volatile void**** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Void4Cast_Volatile(policy, index1);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 4U>
		{
		public:
			static const volatile void**** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Void4Cast_ConstVolatile(policy, index1);
			}
		};
#pragma endregion

#pragma region AnyVisitor Void5
		template< typename T>
		struct AnyVisitor<T, true, Modifiers::None, Declarators::Pointer, 5U>
		{
		public:
			static void***** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Void5Cast(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Const, Declarators::Pointer, 5U>
		{
		public:
			static const void***** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Void5Cast_Const(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::Volatile, Declarators::Pointer, 5U>
		{
		public:
			static volatile void***** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Void5Cast_Volatile(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, true, Modifiers::ConstVolatile, Declarators::Pointer, 5U>
		{
		public:
			static const volatile void***** Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Void5Cast_ConstVolatile(policy);
			}
		};
#pragma endregion

#pragma region AnyVisitor Value
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::Value, 0U>
		{
			typedef typename deduce<T>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
			{
				return any.ValueCast<base>(policy, index1, index2, index3, index4, index5);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::Value, 0U>
		{
			typedef typename std::add_const<typename deduce<T>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
			{
				return any.ValueCast_Const<base>(policy, index1, index2, index3, index4, index5);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::Value, 0U>
		{
			typedef typename std::add_volatile<typename deduce<T>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
			{
				return any.ValueCast_Volatile<base>(policy, index1, index2, index3, index4, index5);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Value, 0U>
		{
			typedef typename std::add_cv<typename deduce<T>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0, size_t index5 = 0)
			{
				return any.ValueCast_ConstVolatile<base>(policy, index1, index2, index3, index4, index5);
			}
		};
#pragma endregion

#pragma region AnyVisitor Pointer
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 1U>
		{
			typedef typename std::add_pointer<typename deduce<T>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.PtrCast<base>(policy, index1, index2, index3, index4);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 1U>
		{
			typedef typename std::add_pointer<typename std::add_const<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.PtrCast_Const<base>(policy, index1, index2, index3, index4);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 1U>
		{
			typedef typename std::add_pointer<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.PtrCast_Volatile<base>(policy, index1, index2, index3, index4);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 1U>
		{
			typedef typename std::add_pointer<typename std::add_cv<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0, size_t index4 = 0)
			{
				return any.PtrCast_ConstVolatile<base>(policy, index1, index2, index3, index4);
			}
		};
#pragma endregion

#pragma region AnyVisitor Pointer2
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 2U>
		{
			typedef typename add_pointer_ex<typename deduce<T>::type, 2U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Ptr2Cast<base>(policy, index1, index2, index3);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 2U>
		{
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 2U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Ptr2Cast_Const<base>(policy, index1, index2, index3);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 2U>
		{
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 2U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Ptr2Cast_Volatile<base>(policy, index1, index2, index3);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 2U>
		{
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 2U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0, size_t index3 = 0)
			{
				return any.Ptr2Cast_ConstVolatile<base>(policy, index1, index2, index3);
			}
		};
#pragma endregion

#pragma region AnyVisitor Pointer3
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 3U>
		{
			typedef typename add_pointer_ex<typename deduce<T>::type, 3U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Ptr3Cast<base>(policy, index1, index2);
			}
		};

		template< typename T>
		struct AnyVisitor<T,false, Modifiers::Const, Declarators::Pointer, 3U>
		{
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 3U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Ptr3Cast_Const<base>(policy, index1, index2);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 3U>
		{
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 3U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Ptr3Cast_Volatile<base>(policy, index1, index2);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 3U>
		{
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 3U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0, size_t index2 = 0)
			{
				return any.Ptr3Cast_ConstVolatile<base>(policy, index1, index2);
			}
		};
#pragma endregion

#pragma region AnyVisitor Pointer4
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 4U>
		{
			typedef typename add_pointer_ex<typename deduce<T>::type, 4U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Ptr4Cast<base>(policy, index1);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 4U>
		{
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 4U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Ptr4Cast_Const<base>(policy, index1);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 4U>
		{
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 4U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Ptr4Cast_Volatile<base>(policy, index1);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 4U>
		{
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 4U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General, size_t index1 = 0)
			{
				return any.Ptr4Cast_ConstVolatile<base>(policy, index1);
			}
		};
#pragma endregion

#pragma region AnyVisitor Pointer5
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::Pointer, 5U>
		{
			typedef typename add_pointer_ex<typename deduce<T>::type, 5U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Ptr5Cast<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::Pointer, 5U>
		{
			typedef typename add_pointer_ex<typename std::add_const<typename deduce<T>::type>::type, 5U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Ptr5Cast_Const<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::Pointer, 5U>
		{
			typedef typename add_pointer_ex<typename std::add_volatile<typename deduce<T>::type>::type, 5U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Ptr5Cast_Volatile<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::Pointer, 5U>
		{
			typedef typename add_pointer_ex<typename std::add_cv<typename deduce<T>::type>::type, 5U>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.Ptr5Cast_ConstVolatile<base>(policy);
			}
		};
#pragma endregion

#pragma region AnyVisitor LValueReference
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::LValueReference, 0U>
		{
			typedef typename std::add_lvalue_reference<typename deduce<T>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.LRefCast<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::LValueReference, 0U>
		{
			typedef typename std::add_lvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.LRefCast_Const<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::LValueReference, 0U>
		{
			typedef typename std::add_lvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.LRefCast_Volatile<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::LValueReference, 0U>
		{
			typedef typename std::add_lvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.LRefCast_ConstVolatile<base>(policy);
			}
		};
#pragma endregion

#pragma region AnyVisitor RValueReference
		template< typename T>
		struct AnyVisitor<T, false, Modifiers::None, Declarators::RValueReference, 0U>
		{
			typedef typename std::add_rvalue_reference<typename deduce<T>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.RRefCast<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Const, Declarators::RValueReference, 0U>
		{
			typedef typename std::add_rvalue_reference<typename std::add_const<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.RRefCast_Const<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::Volatile, Declarators::RValueReference, 0U>
		{
			typedef typename std::add_rvalue_reference<typename std::add_volatile<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.RRefCast_Volatile<base>(policy);
			}
		};

		template< typename T>
		struct AnyVisitor<T, false, Modifiers::ConstVolatile, Declarators::RValueReference, 0U>
		{
			typedef typename std::add_rvalue_reference<typename std::add_cv<typename deduce<T>::type>::type>::type base;
		public:
			static base Get(const Any& any, ModifierPolicy policy = ModifierPolicy::General)
			{
				return any.RRefCast_ConstVolatile<base>(policy);
			}
		};
#pragma endregion

#pragma endregion


		template<class U>
		Any::operator U() const
		{
			return AnyVisitor<U>::Get(*this);
		}

		template<class U>
		U Any::as() const
		{
			return AnyVisitor<U>::Get(*this);
		}

}

