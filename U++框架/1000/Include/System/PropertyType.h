#pragma once

#include "PropertyInfo.h"
namespace System
{
	/// <summary>
	/// Enum PropertyKinds
	/// </summary>
	enum class PropertyKinds
	{
		/// <summary>
		/// 读
		/// </summary>
		Read,
		/// <summary>
		/// 写
		/// </summary>
		Write,
		/// <summary>
		/// 读写
		/// </summary>
		ReadWrite,
		/// <summary>
		/// 读（常数）
		/// </summary>
		ConstRead,
		/// <summary>
		/// 读（常数）写
		/// </summary>
		ConstRead_Write,
		/// <summary>
		/// 写（常数）
		/// </summary>
		ConstWrite,
		/// <summary>
		/// 写（常数）读
		/// </summary>
		ConstWrite_Read,
		/// <summary>
		/// 读写（常数）
		/// </summary>
		ConstReadWrite,
		/// <summary>
		/// 读（静态）
		/// </summary>
		StaticRead,
		/// <summary>
		/// 写（静态）
		/// </summary>
		StaticWrite,
		/// <summary>
		/// 读写（静态）
		/// </summary>
		StaticReadWrite,
	};

	/// <summary>
	/// 属性类型类
	/// </summary>
	class RUNTIME_API PropertyType : public Type
	{
	protected:
		friend class PropertyInfo_impl;
		friend class ClassInfo;
		_type readType;
		_type writeType;
		_type ownerClass;
		PropertyPolicy policy;
		PropertyKinds kind;

		/// <summary>
		/// 创建<see cref="PropertyType"/>实例
		/// </summary>
		PropertyType();
	private:
		Optional<std::wstring> description;
	public:
		virtual ~PropertyType();

		/// <summary>
		/// 获取名称
		/// </summary>
		///<return></return>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取声明类型
		/// </summary>
		///<return></return>
		_type GetClassType() const;

		/// <summary>
		/// 获取类型（读）
		/// </summary>
		///<return></return>
		_type GetReadType() const;

		/// <summary>
		/// 获取类型（写）
		/// </summary>
		///<return></return>
		_type GetWriteType() const;

		/// <summary>
		/// 获取属性政策
		/// </summary>
		///<return></return>
		PropertyPolicy GetPolicy() const;

		/// <summary>
		/// 获取属性种类
		/// </summary>
		///<return></return>
		PropertyKinds GetKind() const;

		/// <summary>
		/// 属性种类
		/// </summary>
		_declspec(property(get = GetKind)) PropertyKinds Kind;

		/// <summary>
		/// 属性政策
		/// </summary>
		_declspec(property(get = GetPolicy)) PropertyPolicy Policy;

		/// <summary>
		/// 声明类型
		/// </summary>
		_declspec(property(get = GetClassType)) _type ClassType;

		/// <summary>
		/// 类型（读）
		/// </summary>
		_declspec(property(get = GetReadType)) _type ReadType;

		/// <summary>
		/// 类型（写）
		/// </summary>
		_declspec(property(get = GetWriteType)) _type WriteType;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual Any Get(Object* obj, bool nothrow = true) const = 0;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">值</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual bool Set(Object* obj, const Any& value, bool nothrow = true) const = 0;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual Any Get(const Object* obj, bool nothrow = true) const = 0;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">值</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual bool Set(const Object* obj, const Any& value, bool nothrow = true) const = 0;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		Any Get(nullptr_t obj, bool nothrow = true) const;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">值</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Set(nullptr_t obj, const Any& value, bool nothrow = true) const;

		bool operator ==(const PropertyType& type) const;
		bool operator !=(const PropertyType& type) const;
	};
}