#pragma once
#include "Any.h"
#include "VariadicAny.h"
#include <map>
#include <memory>

namespace System
{
	struct ConditionalVariadicAnyVisitor;

	class DefaultVariadicAny
	{
	private:
		typedef Any              argument;
		typedef std::map<unsigned int, argument*> container;
		container::size_type len;
		friend struct ConditionalVariadicAnyVisitor;
	public:
		typedef container::size_type    size_type;

		explicit DefaultVariadicAny() : params(), len(0) {}

		explicit DefaultVariadicAny(_nodefault_t nodefault) : params(), len(1) {}

		explicit DefaultVariadicAny(VariadicAny& anys)
		{
			len = 0;
			for (size_t i = 0; i < anys.Size; i++)
			{
				auto iter = anys.At(i);
				if (iter.IsNoDefault)
					len++;
				else if (!iter.IsMissing)
				{
					params.insert(std::pair<unsigned int, argument*>(len, new argument(std::move(iter))));
					len++;
				}
			}
		}

		explicit DefaultVariadicAny(const VariadicAny& anys)
		{
			len = 0;
			for (size_t i = 0; i < anys.Size; i++)
			{
				auto iter = anys.At(i);
				if (iter.IsNoDefault)
					len++;
				else if (!iter.IsMissing)
				{
					params.insert(std::pair<unsigned int, argument*>(len, new argument(std::move(iter))));
					len++;
				}
			}
		}

		~DefaultVariadicAny()
		{
			params.clear();
		}

		explicit DefaultVariadicAny(nullptr_t null)
		{	
			params.insert(std::pair<unsigned int, argument*>(0, new argument(nullptr)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(T& value, bool copy = true)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, copy))); 
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const T& value, bool copy = true)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, copy)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const volatile T& value, bool copy = true)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, copy)));
			len++;
		} 

		template<typename T>
		explicit DefaultVariadicAny(const T&& value, bool noconst, bool copy = true)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, noconst, copy)));
			len++;
		} 

		template<typename T>
		explicit DefaultVariadicAny(const volatile T&& value, bool noconst, bool copy = true)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, noconst, copy)));
			len++;
		}

#pragma region Compile Time Value
		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(T(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const T(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(volatile T(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const volatile T(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(T(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const T(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(volatile T(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const volatile T(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit DefaultVariadicAny(T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit DefaultVariadicAny(const T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit DefaultVariadicAny(volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4, size_t SIZE5>
		explicit DefaultVariadicAny(const volatile T(&value)[SIZE1][SIZE2][SIZE3][SIZE4][SIZE5], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

#pragma endregion
#pragma region Compile Time Pointer
		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(T*(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const T*(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(volatile T*(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const volatile T*(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(T*(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const T*(&value)[SIZE1][SIZE2], bool placeholder) 
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(volatile T*(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const volatile T*(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(const T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3, size_t SIZE4>
		explicit DefaultVariadicAny(const volatile T*(&value)[SIZE1][SIZE2][SIZE3][SIZE4], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

#pragma endregion
#pragma region Compile Time Pointer2
		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(T**(&value)[SIZE], bool placeholder) 
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const T**(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(volatile T**(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const volatile T**(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(T**(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const T**(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(volatile T**(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const volatile T**(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(const T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2, size_t SIZE3>
		explicit DefaultVariadicAny(const volatile T**(&value)[SIZE1][SIZE2][SIZE3], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

#pragma endregion
#pragma region Compile Time Pointer3
		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(T***(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const T***(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(volatile T***(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const volatile T***(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(T***(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const T***(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(volatile T***(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE1, size_t SIZE2>
		explicit DefaultVariadicAny(const volatile T***(&value)[SIZE1][SIZE2], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

#pragma endregion
#pragma region Compile Time Pointer4
		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(T****(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const T****(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(volatile T****(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

		template<typename T, size_t SIZE>
		explicit DefaultVariadicAny(const volatile T****(&value)[SIZE], bool placeholder)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, placeholder)));
			len++;
		}

#pragma endregion

#pragma region Runtime 
		template<typename T>
		explicit DefaultVariadicAny(T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, arraySize)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, arraySize)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, arraySize)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const volatile T* value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t arraySize = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, arraySize)));
			len++;
		}

#pragma endregion
#pragma region Runtime 1
		template<typename T>
		explicit DefaultVariadicAny(T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr, size_t array1Size = 0, size_t array2Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const volatile T** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size)));
			len++;
		}

#pragma endregion
#pragma region Runtime 2
		template<typename T>
		explicit DefaultVariadicAny(T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const volatile T*** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr2, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size)));
			len++;
		}

#pragma endregion
#pragma region Runtime 3
		template<typename T>
		explicit DefaultVariadicAny(T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const volatile T**** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr3, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size)));
			len++;
		}

#pragma endregion
#pragma region Runtime 4
		template<typename T>
		explicit DefaultVariadicAny(T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)));
			len++;
		}

		template<typename T>
		explicit DefaultVariadicAny(const volatile T***** value, AnyDynamicPtrFlags ptrFlag = AnyDynamicPtrFlags::Ptr4, size_t array1Size = 0, size_t array2Size = 0, size_t array3Size = 0, size_t array4Size = 0, size_t array5Size = 0)
		{
			params.insert(std::pair<unsigned int, argument*>(0, new argument(value, ptrFlag, array1Size, array2Size, array3Size, array4Size, array5Size)));
			len++;
		}

#pragma endregion


		DefaultVariadicAny(DefaultVariadicAny&& any)
		{
			params = container(any.params);
			len = any.len; 
			any.params.clear();
		}

		DefaultVariadicAny(const DefaultVariadicAny& any)
		{
			params = container(any.params);
			len = any.len;
		}

		DefaultVariadicAny& operator=(const DefaultVariadicAny& any)
		{
			params = container(any.params);
			len = any.len;
			return *this;
		}

		void Add(_nodefault_t optional)
		{
			len++;
		}	


		void Add(_missing_t missing)
		{

		}

		void Add(const Any& value)
		{
			if (value.IsNoDefault)
			{
				len++;
			}
			else if (!value.IsMissing)
			{
				params.insert(std::pair<unsigned int, argument*>(len, new argument(value)));
				len++;
			}
		}
		
		DefaultVariadicAny& operator,(_nodefault_t optional)
		{
			len++;
			return *this;
		}

		DefaultVariadicAny& operator,(_missing_t missing)
		{
			return *this;
		}

		DefaultVariadicAny& operator,(const Any& value)
		{
			if (!value.IsMissing)
			{
				params.insert(std::pair<unsigned int, argument*>(len, new argument(value)));
				len++;
			}
			return *this;
		}

		size_t GetDimensionSize(size_type index, UInt dimension) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						argument* item = iter->second;
						if (item)
							return item->DimensionSize[dimension];
					}
				}
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension0Size(size_type index) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						argument* item = iter->second;
						if (item)
							return item->DimensionSize[0];
					}
				}
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension1Size(size_type index) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						argument* item = iter->second;
						if (item)
							return item->DimensionSize[1];
					}
				}
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension2Size(size_type index) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						argument* item = iter->second;
						if (item)
							return item->DimensionSize[2];
					}
				}
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension3Size(size_type index) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						argument* item = iter->second;
						if (item)
							return item->DimensionSize[3];
					}
				}
				else
					return 0;
			}
			return 0;
		}

		size_t GetDimension4Size(size_type index) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						argument* item = iter->second;
						if (item)
							return item->DimensionSize[4];
					}
				}
				else
					return 0;
			}
			return 0;
		}

		const argument* At(size_type index) const
		{
			if (len > 0)
			{
				if (index < len)
				{
					auto iter = params.find((unsigned int)index);
					if (iter != params.end())
					{
						return iter->second;
					}
				}
			}
			return nullptr;
		}

		size_type GetSize() const
		{
			return len;
		}

		const argument* operator [] (size_type index) const
		{
			return At(index);
		}

		static __forceinline DefaultVariadicAny Empty()
		{
			return DefaultVariadicAny();
		}

		__declspec(property (get = GetSize)) size_t Size;

		__declspec(property (get = At)) const Any* Items[];

		__declspec(property (get = GetDimension0Size)) size_t Dimension0Size[];

		__declspec(property (get = GetDimension1Size)) size_t Dimension1Size[];

		__declspec(property (get = GetDimension2Size)) size_t Dimension2Size[];

		__declspec(property (get = GetDimension3Size)) size_t Dimension3Size[];

		__declspec(property (get = GetDimension4Size)) size_t Dimension4Size[];



	private:
		container params;
	};

	typedef std::shared_ptr<DefaultVariadicAny> SharedDefaultVariadicAny;

	template class __declspec(dllexport) std::shared_ptr<DefaultVariadicAny>;
}