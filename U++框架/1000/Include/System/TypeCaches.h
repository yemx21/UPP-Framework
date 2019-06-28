#pragma once

#include "NameSpace.h"

//#include <objbase.h>
#include <algorithm>

namespace System
{
	class TypeCaches_impl;

	/// <summary>
	/// 类型集合（缓存）
	/// </summary>
	class RUNTIME_API TypeCaches
	{
	protected:
		friend class TypeCaches_impl;
		TypeCaches_impl* impl;
	public:

		/// <summary>
		/// 创建<see cref="TypeCaches"/>实例
		/// </summary>
		TypeCaches();
		~TypeCaches();

		/// <summary>
		/// 插入指定类型
		/// </summary>
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		NameSpaceError Insert(_type type);

		/// <summary>
		/// 移除指定类型
		/// </summary>
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		void Remove(_type type);

		/// <summary>
		/// 获取子项
		/// </summary>
		/// <param name="index">索引</param>
		/// <returns></returns>
		_type GetItem(size_t index) const;

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		size_t GetItemSize() const;

		/// <summary>
		/// 子项数
		/// </summary>
		_declspec(property(get = GetItemSize)) size_t ItemSize;
		/// <summary>
		/// 子项
		/// </summary>
		_declspec(property(get = GetItem)) _type Items[];
	};
}