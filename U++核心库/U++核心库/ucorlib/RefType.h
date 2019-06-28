#pragma once 
#include "Type.h"
#include "InertiaString.h"
namespace System
{
	/// <summary>
	/// 引用种类
	/// </summary>
	enum class RefKinds
	{
		/// <summary>
		/// 左值引用
		/// </summary>
		Left,
		/// <summary>
		/// 右值引用
		/// </summary>
		Right
	};

	class RefType_impl;

	/// <summary>
	/// 引用类型
	/// </summary>
	class RUNTIME_API RefType final: public Type
	{
	protected:
		friend class TypeFactory;
		/// <summary>
		/// 创建<see cref="RefType"/>实例
		/// </summary>
		/// <param name="type">指定基类型</param>
		/// <param name="kind">引用种类</param>
		/// <returns></returns>
		static const RefType* Create(_type type, RefKinds kind);
	public:
			
		virtual ~RefType();

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual const wchar_t* GetName() const;

		/// <summary>
		/// 获取引用种类
		/// </summary>
		/// <returns></returns>
		RefKinds GetKind() const;

		/// <summary>
		/// 获取基类型
		/// </summary>
		/// <returns></returns>
		_type GetBasicType() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 引用种类
		/// </summary>
		__declspec(property(get = GetKind)) RefKinds Kind;

		/// <summary>
		/// 基类型
		/// </summary>
		__declspec(property(get = GetBasicType)) _type BasicType;

	private:
		friend class Type;
		friend class RefType_impl; 
		_type refType;
		InertiaString name;
		RefKinds refKind;
		RefType(_type type, RefKinds kind);
	};
}