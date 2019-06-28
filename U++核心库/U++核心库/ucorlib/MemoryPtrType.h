#pragma once 
#include "Type.h"
#include "Common.h"
namespace System
{
	class MemoryPtrType_impl;

	/*

	std::shared_ptr<T>:  HoldType=T
	std::weak_ptr<T>:  HoldType=T
	std::unique_ptr<T>:  HoldType=T
	auto_ptr<T>:  HoldType=T
	*/

	/// <summary>
	/// 内存指针类型
	/// </summary>
	class RUNTIME_API MemoryPtrType final: public Type
	{
	protected:
		friend class TypeFactory;

		/// <summary>
		/// 创建<see cref="MemoryPtrType"/>实例.
		/// </summary>
		/// <param name="holdType">子类型</param>
		/// <param name="ptype">内存指针类型</param>
		/// <returns></returns>
		static const MemoryPtrType* MemoryPtrType::Create(_type holdType, MemoryPtrTypes ptype);
	public:
		virtual ~MemoryPtrType();

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual const wchar_t* GetName() const;

		/// <summary>
		/// 获取内存指针类型
		/// </summary>
		/// <returns></returns>
		MemoryPtrTypes GetPtrType() const;

		/// <summary>
		/// 获取子类型
		/// </summary>
		/// <returns></returns>
		_type GetHoldType() const;

		/// <summary>
		/// 子类型
		/// </summary>
		__declspec(property(get = GetHoldType)) _type HoldType;

		/// <summary>
		/// 内存指针类型
		/// </summary>
		__declspec(property(get = GetPtrTypes)) MemoryPtrTypes PtrType;
	private:
		friend class Type;
		friend class MemoryPtrType_impl;

		MemoryPtrType_impl* impl;
		MemoryPtrType(_type holdType, MemoryPtrTypes ptype);
	};
}