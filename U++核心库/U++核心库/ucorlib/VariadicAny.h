#pragma once
#include "Any.h"
namespace System
{
	class VariadicAny
	{
	private:
		typedef Any              argument;
		typedef std::vector< argument > container;

	public:
		typedef container::size_type    size_type;
		~VariadicAny()
		{

		}
		explicit VariadicAny() : args() {}

		explicit VariadicAny(const Any& val) : args(1, val) {}

		explicit VariadicAny(nullptr_t null) : args(1, null) {}

		explicit VariadicAny(_missing_t missing) : args(1, missing) {}

		explicit VariadicAny(_nodefault_t nodefault) : args(1, nodefault) {}

		template<typename T>
		explicit VariadicAny(T& value, bool copy = true) : args(1, argument(value, copy)) {}

		template<typename T>
		explicit VariadicAny(const T& value, bool copy = true) : args(1, argument(value, copy)) {}

		template<typename T>
		explicit VariadicAny(const volatile T& value, bool copy = true) : args(1, argument(value, copy)) {}

		template<typename T>
		explicit VariadicAny(const T&& value, bool noconst, bool copy = true) : args(1, argument(value, noconst, copy)) {}

		template<typename T>
		explicit VariadicAny(const volatile T&& value, bool noconst, bool copy = true) : args(1, argument(value, noconst, copy)) {}

#pragma region Compile Time Value
		template<typename T, size_t SIZE>
		explicit VariadicAny(T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(volatile T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const volatile T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(volatile T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const volatile T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit VariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit VariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit VariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit VariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer
		template<typename T, size_t SIZE>
		explicit VariadicAny(T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(volatile T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const volatile T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit VariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer2
		template<typename T, size_t SIZE>
		explicit VariadicAny(T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(volatile T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const volatile T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(const T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit VariadicAny(const volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

#pragma endregion
#pragma region Compile Time Pointer3
		template<typename T, size_t SIZE>
		explicit VariadicAny(T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(volatile T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const volatile T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit VariadicAny(const volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer4
		template<typename T, size_t SIZE>
		explicit VariadicAny(T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(volatile T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit VariadicAny(const volatile T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion

#pragma region Runtime 
		template<typename T>
		explicit VariadicAny(T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit VariadicAny(const T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit VariadicAny(volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit VariadicAny(const volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}
#pragma endregion
#pragma region Runtime 1
		template<typename T>
		explicit VariadicAny(T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit VariadicAny(const T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit VariadicAny(volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit VariadicAny(const volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}
#pragma endregion
#pragma region Runtime 2
		template<typename T>
		explicit VariadicAny(T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit VariadicAny(const T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit VariadicAny(volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit VariadicAny(const volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}
#pragma endregion
#pragma region Runtime 3
		template<typename T>
		explicit VariadicAny(T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit VariadicAny(const T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit VariadicAny(volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit VariadicAny(const volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}
#pragma endregion
#pragma region Runtime 4
		template<typename T>
		explicit VariadicAny(T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit VariadicAny(const T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit VariadicAny(volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit VariadicAny(const volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}
#pragma endregion

		VariadicAny(const VariadicAny& any)
		{
			args = container(any.args);
		}

		VariadicAny& operator=(const VariadicAny& any)
		{
			args = container(any.args);
			return *this;
		}

		VariadicAny(VariadicAny&& any)
		{
			args = container(any.args);
		}

		void Copy(const VariadicAny& any)
		{
			args = container(any.args);
		}


		VariadicAny& operator,(const Any& value)
		{
			args.push_back(argument(value)); return *this;
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

		static __forceinline VariadicAny Empty()
		{
			return VariadicAny();
		}

		__declspec(property (get = GetSize)) size_t Size;

		__declspec(property (get = GetDimension0Size)) size_t Dimension0Size[];

		__declspec(property (get = GetDimension1Size)) size_t Dimension1Size[];

		__declspec(property (get = GetDimension2Size)) size_t Dimension2Size[];

		__declspec(property (get = GetDimension3Size)) size_t Dimension3Size[];

		__declspec(property (get = GetDimension4Size)) size_t Dimension4Size[];


	private:
		container args;
	};
}