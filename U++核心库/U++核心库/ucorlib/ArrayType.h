#pragma once 
#include "Type.h"
#include "Common.h"
#include <array>
namespace System
{
	class ArrayType_impl;

	/*
	Array T[N]:  FixedSize=N,0,0,0,0  ItemType=T
	Array T[N1][N2]:  FixedSize=N1,N2,0,0,0  ItemType=T
	Array T[N1][N2][N3]:  FixedSize=N1,N2,N3,0,0  ItemType=T
	Array T[N1][N2][N3][N4]:  FixedSize=N1,N2,N3,N4,0 ItemType=T
	Array T[N1][N2][N3][N4][N5]:  FixedSize=N1,N2,N3,N4,N5 ItemType=T

	STL_array<T, N>:  FixedSize=N, ItemType=T
	STL_valarray<T>:  FixedSize=0, ItemType=T

	STL_vector<T, std::allocator<T>> :   FixedSize=0, ItemType=T, ItemAllocatorType=std::allocator<T>
	STL_deque<T, std::allocator<T>>:   FixedSize=0, ItemType=T, ItemAllocatorType=std::allocator<T>
	STL_forward_list<T, std::allocator<T>>:   FixedSize=0, ItemType=T, ItemAllocatorType=std::allocator<T>
	STL_list<T, std::allocator<T>>:   FixedSize=0, ItemType=T, ItemAllocatorType=std::allocator<T>

	STL_set<Key, std::less<Key>, std::allocator<Key>>:   FixedSize=0, KeyType=Key, KeyCompareType=std::less<Key>, KeyAllocatorType=std::allocator<T>
	STL_multiset<Key, std::less<Key>, std::allocator<Key>>:   FixedSize=0, KeyType=Key, KeyCompareType=std::less<Key>, KeyAllocatorType=std::allocator<T>

	STL_unordered_set<Key, std::hash<Key>, std::equal_to<Key>, std::allocator<Key>> :   FixedSize=0, KeyType=Key, KeyHashType=std::hash<Key>, KeyEqualType=std::equal_to<Key>, KeyAllocatorType=std::allocator<T>
	STL_unordered_multiset<Key, std::hash<Key>, std::equal_to<Key>, std::allocator<Key>> :   FixedSize=0, KeyType=Key, KeyHashType=std::hash<Key>, KeyEqualType=std::equal_to<Key>, KeyAllocatorType=std::allocator<T>
		
	STL_map<Key, T, std::less<Key>, std::allocator<std::pair<const Key, T>>>:   FixedSize=0, KeyType=Key, ItemType=T, KeyCompareType= std::less<Key>, KeyAllocatorType=std::allocator<std::pair<const Key, T>>
	STL_multimap<Key, T, std::less<Key>, std::allocator<std::pair<const Key, T>>>:   FixedSize=0, KeyType=Key, ItemType=T, KeyCompareType= std::less<Key>, KeyAllocatorType=std::allocator<std::pair<const Key, T>>

	STL_unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, std::allocator<std::pair<const Key, T>>>:   FixedSize=0, KeyType=Key, ItemType=T, KeyHashType= std::hash<Key>, KeyCompareType= std::equal_to<Key>, KeyAllocatorType=std::allocator<std::pair<const Key, T>>
	STL_unordered_multimap<Key, T, std::hash<Key>, std::equal_to<Key>, std::allocator<std::pair<const Key, T>>>:   FixedSize=0, KeyType=Key, ItemType=T, KeyHashType= std::hash<Key>, KeyCompareType= std::equal_to<Key>, KeyAllocatorType=std::allocator<std::pair<const Key, T>>
		
	STL_stack<T, std::deque<T>>:   FixedSize=0, ItemType=T, ItemContainerType=std::deque<T>
	STL_queue<T, std::deque<T>>:   FixedSize=0, ItemType=T, ItemContainerType=std::deque<T>
	STL_priority_queue<T, std::vector<T>, std::less<typename Container::value_type>>:   FixedSize=0, ItemType=T, ItemContainerType=std::vector<T>, ItemCompareType=std::less<typename Container::value_type>

	ObservableCollection<T>: FixedSize=0, ItemType=T
	*/

	/// <summary>
	/// 数组类型
	/// </summary>
	class RUNTIME_API ArrayType final: public Type
	{
	protected:
		friend class TypeFactory;
		static const ArrayType* ArrayType::Create(_type keyType, _type keyHashType, _type keyEqualType, _type keyCompareType, _type keyAllocatorType, _type itemType, _type itemAllocatorType, _type itemContainerType, _type itemCompareType, ContainerTypes arrayType, UInt8 dimension = 0, size_t fixedSize1 = 0, size_t fixedSize2 = 0, size_t fixedSize3 = 0, size_t fixedSize4 = 0, size_t fixedSize5 = 0);
	public:
		virtual ~ArrayType();

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual const wchar_t* GetName() const;

		/// <summary>
		/// 获取容器类型
		/// </summary>
		/// <returns></returns>
		ContainerTypes GetContainerTypes() const;

		/// <summary>
		/// 获取是否固定大小
		/// </summary>
		/// <returns></returns>
		bool GetIsFixed() const;

		/// <summary>
		/// 获取指定维数的大小
		/// </summary>
		/// <returns></returns>
		size_t GetFixedSize(UInt8 dimension) const;

		/// <summary>
		/// 获取维数
		/// </summary>
		/// <returns></returns>
		UInt8 GetDimension() const;

		/// <summary>
		/// 获取键值类型
		/// </summary>
		/// <returns></returns>
		_type GetKeyType() const;

		/// <summary>
		/// 获取键值哈希类型
		/// </summary>
		/// <returns></returns>
		_type GetKeyHashType() const;

		/// <summary>
		/// 获取键值相等类型
		/// </summary>
		/// <returns></returns>
		_type GetKeyEqualType() const;

		/// <summary>
		/// 获取键值比较器类型
		/// </summary>
		/// <returns></returns>
		_type GetKeyCompareType() const;

		/// <summary>
		/// 获取键值分配器类型
		/// </summary>
		/// <returns></returns>
		_type GetKeyAllocatorType() const;

		/// <summary>
		/// 获取值类型
		/// </summary>
		/// <returns></returns>
		_type GetItemType() const;

		/// <summary>
		/// 获取值分配器类型
		/// </summary>
		/// <returns></returns>
		_type GetItemAllocatorType() const;

		/// <summary>
		/// 获取值容器类型
		/// </summary>
		/// <returns></returns>
		_type GetItemContainerType() const;

		/// <summary>
		/// 获取值比较器类型
		/// </summary>
		/// <returns></returns>
		_type GetItemCompareType() const;

		/// <summary>
		/// 是否固定大小
		/// </summary>
		__declspec(property(get = GetIsFixed)) bool IsFixed;

		/// <summary>
		/// 维数大小
		/// </summary>
		__declspec(property(get = GetFixedSize)) size_t FixedSize[];

		/// <summary>
		/// 维数
		/// </summary>
		__declspec(property(get = GetDimension)) UInt8 Dimension;

		/// <summary>
		/// 键值类型
		/// </summary>
		/// <returns></returns>
		__declspec(property(get = GetKeyType)) _type KeyType;

		/// <summary>
		/// 键值哈希类型
		/// </summary>
		__declspec(property(get = GetKeyHashType)) _type KeyHashType;

		/// <summary>
		/// 键值相等类型
		/// </summary>
		__declspec(property(get = GetKeyEqualType)) _type KeyEqualType;

		/// <summary>
		/// 键值比较器类型
		/// </summary>
		__declspec(property(get = GetKeyCompareType)) _type KeyCompareType;

		/// <summary>
		/// 键值分配器类型
		/// </summary>
		__declspec(property(get = GetKeyAllocatorType)) _type KeyAllocatorType;

		/// <summary>
		/// 值类型
		/// </summary>
		__declspec(property(get = GetItemType)) _type ItemType;

		/// <summary>
		/// 值容器类型
		/// </summary>
		__declspec(property(get = GetItemContainerTypes)) ContainerTypes ItemContainerType;

		/// <summary>
		/// 值比较器类型
		/// </summary>
		__declspec(property(get = GetItemCompareType)) _type ItemCompareType;
	private:
		friend class Type;
		friend class ArrayType_impl;

		ArrayType_impl* impl;
		ArrayType(_type keyType, _type keyHashType, _type keyEqualType, _type keyCompareType, _type keyAllocatorType, _type itemType, _type itemAllocatorType, _type itemContainerType, _type itemCompareType, ContainerTypes arrayType, UInt8 dimension, size_t fixedSize1, size_t fixedSize2, size_t fixedSize3, size_t fixedSize4, size_t fixedSize5);
	};
}