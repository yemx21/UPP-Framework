#pragma once
#include <iostream>
#include <typeindex>
#include <type_traits>
#include "RuntimeTypes.h"


namespace System
{
	template<typename T, typename... Args>
	struct MaxType : std::integral_constant<int,
		(sizeof(T)>MaxType<Args...>::value ? sizeof(T) : MaxType<Args...>::value) >

	{};

	template<typename T>
	struct MaxType<T> : std::integral_constant<int, sizeof(T) >{};

	template < typename T, typename... List >
	struct Contains : std::true_type {};

	template < typename T, typename Head, typename... Rest >
	struct Contains<T, Head, Rest...>
		: std::conditional < std::is_same<T, Head>::value, std::true_type, Contains < T,

		Rest... >> ::type{};

	template < typename T >
	struct Contains<T> : std::false_type{};

	template<typename... Args>
	struct VariantHelper;

	template<typename T, typename... Args>
	struct VariantHelper<T, Args...>
	{
		inline static void Destroy(const std::type_index& id, void * data)
		{
			if (id == std::type_index(typeid(T)))
				((T*)(data))->~T();
			else
				VariantHelper<Args...>::Destroy(id, data);
		}
	};

	template<> struct VariantHelper<>
	{
		inline static void Destroy(const std::type_index& id, void * data) {}
	};

	template<typename... Types>
	class Variant
	{
		typedef VariantHelper<Types...> Helper_t;
	public:
		Variant(void) :m_typeIndex(typeid(void))
		{
		}

		~Variant()
		{
			Helper_t::Destroy(m_typeIndex, &m_data);
		}

		template<typename T>
		bool Is() const
		{
			return (m_typeIndex == std::type_index(typeid(T)));
		}

		template<typename T>
		T& Get() const
		{
			if (!Is<T>())
			{
				throw std::bad_cast();
			}

			return *(T*)(&m_data);
		}

		template <class T,
		class = typename std::enable_if<Contains<typename

			std::remove_reference<T>::type, Types...>::value>::type>
			Variant(T&& value) : m_typeIndex(std::type_index(typeid(void)))
		{
				Helper_t::Destroy(m_typeIndex, &m_data);
				typedef typename std::remove_reference<T>::type U;
				new(m_data)U(std::forward<T>(value));
				m_typeIndex = std::type_index(typeid(T));
			}



		template<typename F>
		void Visit(F&& f) const
		{
			using T = typename std::function_traits<F>::arg<0>::type;
			if (Is<T>())
				f(Get<T>());
		}

		template<typename F, typename... Rest>
		void Visit(F&& f, Rest&&... rest) const
		{
			using T = typename std::function_traits<F>::arg<0>::type;
			if (Is<T>())
				Visit(std::forward<F>(f));
			else
				Visit(std::forward<Rest>(rest)...);
		}

		template<typename T>
		bool Compare(const Variant& var)
		{
			return (*(T*)(&m_data)) == (*(T*)(&var.m_data));
		}
	private:
		char m_data[MaxType<Types...>::value];
		std::type_index m_typeIndex;
	};

	typedef System::Variant<bool, Int8, UInt8, Int16, UInt16, Int24, UInt24, Int32, UInt32, Int64, UInt64, Int128, UInt128, Float32, Float64, Float128> Number;
	typedef System::Variant<Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64> StandardInterger;
	
	template<typename T>
	struct DetectNumber :std::false_type {};

	template<>
	struct DetectNumber<bool> :std::true_type{};
	template<>
	struct DetectNumber<Int8> :std::true_type{};
	template<>
	struct DetectNumber<UInt8> :std::true_type{};
	template<>
	struct DetectNumber<Int16> :std::true_type{};
	template<>
	struct DetectNumber<UInt16> :std::true_type{};
	template<>
	struct DetectNumber<Int24> :std::true_type{};
	template<>
	struct DetectNumber<UInt24> :std::true_type{};
	template<>
	struct DetectNumber<Int32> :std::true_type{};
	template<>
	struct DetectNumber<UInt32> :std::true_type{};
	template<>
	struct DetectNumber<Int64> :std::true_type{};
	template<>
	struct DetectNumber<UInt64> :std::true_type{};
	template<>
	struct DetectNumber<Int128> :std::true_type{};
	template<>
	struct DetectNumber<UInt128> :std::true_type{};
	template<>
	struct DetectNumber<Float32> :std::true_type{};
	template<>
	struct DetectNumber<Float64> :std::true_type{};
	template<>
	struct DetectNumber<Float128> :std::true_type{};

}