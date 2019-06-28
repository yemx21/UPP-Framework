#pragma once
#include "FieldInfo.h"
namespace System
{
	/// <summary>
	/// 成员变量种类枚举
	/// </summary>
	enum class FieldKinds
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
	/// 成员变量类型类
	/// </summary>
	class RUNTIME_API FieldType : public Type
	{
	protected:
		friend class FieldInfo_impl;
		friend class ClassInfo;
		friend class EnumInfo;
		_type fieldType;
		_type ownerClass;
		FieldPolicy policy;
		FieldKinds kind;
		FieldType();
	private:
		Optional<std::wstring> description;
	public:
		virtual ~FieldType() {};

		/// <summary>
		/// 获取成员变量政策
		/// </summary>
		///<return></return>
		FieldPolicy GetPolicy() const;

		/// <summary>
		/// 获取名称
		/// </summary>
		///<return></return>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取所属类类型
		/// </summary>
		///<return></return>
		const System::Type* GetClassType() const;

		/// <summary>
		/// 获取成员变量类型
		/// </summary>
		///<return></return>
		const System::Type* GetFieldType() const;

		/// <summary>
		/// 成员变量政策
		/// </summary>
		_declspec(property(get = GetPolicy)) FieldPolicy Policy;

		/// <summary>
		/// 成员所属类类型
		/// </summary>
		_declspec(property(get = GetClassType)) _type ClassType;

		/// <summary>
		/// 成员成员变量类型
		/// </summary>
		_declspec(property(get = GetFieldType)) const System::Type* Type;

		/// <summary>
		/// 从指定对象获取该成员变量的值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		///<return></return>
		virtual Any Get(const Object* obj, bool nothrow = true) const = 0;

		/// <summary>
		/// 设置指定对象该成员变量的值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">指定值</param>
		/// <param name="nothrow">无错误报告</param>
		///<return></return>
		virtual bool Set(Object* obj, const Any& value, bool nothrow = true) const = 0;


		bool operator ==(const FieldType& type) const;
		bool operator !=(const FieldType& type) const;
	};
}