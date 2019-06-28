#pragma once 
#include "Type.h"
#include "Any.h"
namespace System
{
	class Object;

	ENUM_FLAGS(FieldFlags)
	/// <summary>
	/// 成员变量标识
	/// </summary>
	enum class FieldFlags
	{
		/// <summary>
		/// 实例
		/// </summary>
		Instance = 0x0001,

		/// <summary>
		/// 静态
		/// </summary>
		Static = 0x0002,

		/// <summary>
		/// 常量
		/// </summary>
		Const = 0x0004,

		/// <summary>
		/// 公有
		/// </summary>
		Public = 0x0010,

		/// <summary>
		/// 保护
		/// </summary>
		Protected = 0x0020,

		/// <summary>
		/// 私有
		/// </summary>
		Private = 0x0040,

		/// <summary>
		/// 易变的（线程）
		/// </summary>
		Volatile = 0x0100,

		/// <summary>
		/// 易变的（实例）
		/// </summary>
		Mutable = 0x0200,

		/// <summary>
		/// 默认
		/// </summary>
		Default = Instance
	};

	/// <summary>
	/// 成员变量政策枚举
	/// </summary>
	enum class FieldPolicy
	{
		/// <summary>
		/// 所有
		/// </summary>
		All,
		/// <summary>
		/// 只读
		/// </summary>
		ReadOnly,
		/// <summary>
		/// 只写
		/// </summary>
		WriteOnly
	};

	class FieldType;
	class FieldInfo_impl;

	/// <summary>
	/// 成员变量信息类
	/// </summary>
	class RUNTIME_API FieldInfo final: public MemberInfo
	{
	private:
		friend class FieldInfo_impl;
		FieldInfo_impl* impl;
		friend class ClassInfo;
		friend class EnumInfo;
		FieldInfo(const wchar_t* name, FieldFlags flags, FieldType* type);

	public:
		/// <summary>
		/// 获取名称
		/// </summary>
		///<return></return>
		const wchar_t* GetName() const;

		/// <summary>
		/// 从指定对象获取该成员变量的值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		///<return></return>
		Any Get(const Object* obj, bool nothrow = true) const;

		/// <summary>
		/// 设置指定对象该成员变量的值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">指定值</param>
		/// <param name="nothrow">无错误报告</param>
		///<return></return>
		bool Set(Object* obj, const Any& value, bool nothrow = true) const;

		/// <summary>
		/// 获取成员变量类型
		/// </summary>
		///<return></return>
		const FieldType* GetType()const;

		/// <summary>
		/// 获取成员变量标识
		/// </summary>
		///<return></return>
		FieldFlags GetFlags() const;

		/// <summary>
		/// 获取成员变量政策
		/// </summary>
		///<return></return>
		FieldPolicy GetPolicy() const;

		~FieldInfo();

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 成员变量类型
		/// </summary>
		_declspec(property(get = GetFlags)) FieldFlags Flags;

		/// <summary>
		/// 成员变量标识
		/// </summary>
		_declspec(property(get = GetType)) const FieldType* Type;

		/// <summary>
		/// 成员变量政策
		/// </summary>
		_declspec(property(get = GetPolicy)) FieldPolicy Policy;

		bool operator ==(const FieldInfo& info) const;
		bool operator !=(const FieldInfo& info) const;
	};
}