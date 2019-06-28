#pragma once
#include "Any.h"
#include <map>
namespace System
{
	struct ConditionalVariadicAnyVisitor;

	class OptionalVariadicAny
	{
	private:
		typedef Any              argument;
		typedef std::vector< argument > container;

	public:
		typedef container::size_type    size_type;

		explicit OptionalVariadicAny() :  args() {}

		explicit OptionalVariadicAny(nullptr_t null) :  args(1, null) {}

		explicit OptionalVariadicAny(_missing_t missing) :  args(1, missing) {}

		template<typename T>
		explicit OptionalVariadicAny(T& value, bool copy = true) :  args(1, argument(value, copy)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T& value, bool copy = true) :  args(1, argument(value, copy)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T& value, bool copy = true) :  args(1, argument(value, copy)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T&& value, bool noconst, bool copy = true) :  args(1, argument(value, noconst, copy)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T&& value, bool noconst, bool copy = true) :  args(1, argument(value, noconst, copy)) {}

#pragma region Compile Time Value
		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(volatile T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const volatile T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(volatile T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const volatile T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit OptionalVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit OptionalVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit OptionalVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit OptionalVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer
		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(volatile T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const volatile T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit OptionalVariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer2
		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(volatile T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const volatile T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(const T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit OptionalVariadicAny(const volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

#pragma endregion
#pragma region Compile Time Pointer3
		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(volatile T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const volatile T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit OptionalVariadicAny(const volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer4
		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(volatile T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit OptionalVariadicAny(const volatile T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion

#pragma region Runtime 
		template<typename T>
		explicit OptionalVariadicAny(T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit OptionalVariadicAny(volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}
#pragma endregion
#pragma region Runtime 1
		template<typename T>
		explicit OptionalVariadicAny(T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}
#pragma endregion
#pragma region Runtime 2
		template<typename T>
		explicit OptionalVariadicAny(T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}
#pragma endregion
#pragma region Runtime 3
		template<typename T>
		explicit OptionalVariadicAny(T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}
#pragma endregion
#pragma region Runtime 4
		template<typename T>
		explicit OptionalVariadicAny(T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit OptionalVariadicAny(const volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}
#pragma endregion


		OptionalVariadicAny(const OptionalVariadicAny& any)
		{
			args = any.args;
		}

		OptionalVariadicAny(OptionalVariadicAny&& any)
		{
			args = any.args;
		}

		OptionalVariadicAny& operator =(const OptionalVariadicAny& any)
		{
			args = any.args;
			return *this;
		}

		~OptionalVariadicAny()
		{
		}

		size_t Add(Any* value)
		{
			if (!value) return args.size() - 1;
			args.push_back(*value);
			return args.size() - 1;
		}

		size_t Add(const Any& value)
		{
			args.push_back(value);
			return args.size() - 1;
		}

		bool Remove(size_t index)
		{
			if (index > 0 && index < args.size())
			{
				args.erase(args.begin() + index);
				return true;
			}
			return false;
		}

		OptionalVariadicAny& operator,(const Any& value)
		{
			args.push_back(value); return *this;
		}

		size_t GetDimensionSize(size_type index, UInt dimension) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index].DimensionSize[dimension];
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension0Size(size_type index) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index].DimensionSize[0];
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension1Size(size_type index) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index].DimensionSize[1];
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension2Size(size_type index) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index].DimensionSize[2];
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension3Size(size_type index) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index].DimensionSize[3];
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension4Size(size_type index) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index].DimensionSize[4];
				else
					return 0;
			}
			return 0;
		}

		const Any& At(size_type index) const
		{
			if (args.size() > 0)
			{
				if (index < args.size())
					return args[index];
				else
					throw std::out_of_range("index");
			}
			throw std::exception("empty");
		}

		size_type GetSize() const
		{
			return args.size();
		}

		const Any& operator [] (size_type index) const
		{
			return At(index);
		}

		static __forceinline OptionalVariadicAny Empty()
		{
			return OptionalVariadicAny();
		}

		__declspec(property (get = GetSize)) size_t Size;

		__declspec(property (get = GetDimension0Size)) size_t Dimension0Size[];

		__declspec(property (get = GetDimension1Size)) size_t Dimension1Size[];

		__declspec(property (get = GetDimension2Size)) size_t Dimension2Size[];

		__declspec(property (get = GetDimension3Size)) size_t Dimension3Size[];

		__declspec(property (get = GetDimension4Size)) size_t Dimension4Size[];

	private:
		friend struct ConditionalVariadicAnyVisitor;
		mutable container args;
		bool Set(const Any& value, size_t index) const
		{
			if (index > 0 && index < args.size())
			{
				args[index] = value;
				return true;
			}
			return false;
		}
	};
}