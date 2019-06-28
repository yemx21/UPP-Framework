#pragma once
#include "Any.h"
namespace System
{
	class RealVariadicAny
	{
	private:
		typedef Any   argument;
		typedef std::vector< argument > container;

	public:
		typedef container::size_type    size_type;
		~RealVariadicAny()
		{

		}
		explicit RealVariadicAny() : args() {}

		explicit RealVariadicAny(const Any& val)
		{
			if (!val.IsMissing && !val.IsNoDefault)
				args.push_back(argument(val));
		}

		explicit RealVariadicAny(nullptr_t null) : args(1, null) {}

		explicit RealVariadicAny(_missing_t missing) : args() {}

		explicit RealVariadicAny(_nodefault_t nodefault) : args() {}

		template<typename T>
		explicit RealVariadicAny(T& value, bool copy = true) : args(1, argument(value, copy)) {}

		template<typename T>
		explicit RealVariadicAny(const T& value, bool copy = true) : args(1, argument(value, copy)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T& value, bool copy = true) : args(1, argument(value, copy)) {}

		template<typename T>
		explicit RealVariadicAny(const T&& value, bool noconst, bool copy = true) : args(1, argument(value, noconst, copy)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T&& value, bool noconst, bool copy = true) : args(1, argument(value, noconst, copy)) {}

#pragma region Compile Time Value
		template<typename T, size_t SIZE>
		explicit RealVariadicAny(T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(volatile T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const volatile T(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(volatile T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const volatile T(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit RealVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit RealVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit RealVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit RealVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer
		template<typename T, size_t SIZE>
		explicit RealVariadicAny(T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(volatile T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const volatile T*(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const volatile T*(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit RealVariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer2
		template<typename T, size_t SIZE>
		explicit RealVariadicAny(T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(volatile T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const volatile T**(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const volatile T**(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(const T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit RealVariadicAny(const volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder) : args(1, argument(value, placeholder)) {}

#pragma endregion
#pragma region Compile Time Pointer3
		template<typename T, size_t SIZE>
		explicit RealVariadicAny(T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(volatile T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const volatile T***(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit RealVariadicAny(const volatile T***(&value)[SIZE1][SIZE2], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion
#pragma region Compile Time Pointer4
		template<typename T, size_t SIZE>
		explicit RealVariadicAny(T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(volatile T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}

		template<typename T, size_t SIZE>
		explicit RealVariadicAny(const volatile T****(&value)[SIZE], bool placeholder) : args(1, argument(value, placeholder)) {}
#pragma endregion

#pragma region Runtime 
		template<typename T>
		explicit RealVariadicAny(T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit RealVariadicAny(const T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit RealVariadicAny(volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0) : args(1, argument(value, ptrFlag, arraySize)) {}
#pragma endregion
#pragma region Runtime 1
		template<typename T>
		explicit RealVariadicAny(T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit RealVariadicAny(const T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit RealVariadicAny(volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size)) {}
#pragma endregion
#pragma region Runtime 2
		template<typename T>
		explicit RealVariadicAny(T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit RealVariadicAny(const T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit RealVariadicAny(volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size)) {}
#pragma endregion
#pragma region Runtime 3
		template<typename T>
		explicit RealVariadicAny(T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit RealVariadicAny(const T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit RealVariadicAny(volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)) {}
#pragma endregion
#pragma region Runtime 4
		template<typename T>
		explicit RealVariadicAny(T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit RealVariadicAny(const T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit RealVariadicAny(volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}

		template<typename T>
		explicit RealVariadicAny(const volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0) : args(1, argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)) {}
#pragma endregion


		RealVariadicAny(const RealVariadicAny& any)
		{
			args = container(any.args);
		}

		RealVariadicAny(RealVariadicAny&& any)
		{
			args = container(any.args);
		}

		RealVariadicAny& operator=(const RealVariadicAny& any)
		{
			args = container(any.args);
			return *this;
		}

		void Copy(const RealVariadicAny& any)
		{
			args = container(any.args);
		}


		RealVariadicAny& operator,(const Any& value)
		{
			if (value.IsMissing || value.IsNoDefault) return *this;
			args.push_back(argument(value)); 
			return *this;
		}

		RealVariadicAny& operator,(_missing_t value)
		{
			return *this;
		}

		RealVariadicAny& operator,(_nodefault_t value)
		{
			return *this;
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

		static __forceinline RealVariadicAny Empty()
		{
			return RealVariadicAny();
		}

		__declspec(property (get = GetSize)) size_type Size;

		__declspec(property (get = GetDimension0Size)) size_t Dimension0Size[];

		__declspec(property (get = GetDimension1Size)) size_t Dimension1Size[];

		__declspec(property (get = GetDimension2Size)) size_t Dimension2Size[];

		__declspec(property (get = GetDimension3Size)) size_t Dimension3Size[];

		__declspec(property (get = GetDimension4Size)) size_t Dimension4Size[];

	private:
		container args;
	};
}