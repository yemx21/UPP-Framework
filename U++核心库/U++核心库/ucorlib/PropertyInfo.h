#pragma once
#include "Type.h"
#include "Any.h"
namespace System
{
	class Object;
	class PropertyInfo_impl;
	class ClassInfo;
	class EnumInfo;
	class PropertyType;

	ENUM_FLAGS(PropertyFlags)
	/// <summary>
	/// 属性标识
	/// </summary>
	enum class PropertyFlags
	{
		/// <summary>
		/// 静态
		/// </summary>
		Static = 0x0001,
		/// <summary>
		/// 公有
		/// </summary>
		Public = 0x002,
		/// <summary>
		/// 保护
		/// </summary>
		Protected = 0x004,
		/// <summary>
		/// 私有
		/// </summary>
		Private = 0x0010,
		/// <summary>
		/// 默认
		/// </summary>
		Default = Public
	};

	/// <summary>
	/// 属性政策
	/// </summary>
	enum class PropertyPolicy
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

	/// <summary>
	/// 属性信息类
	/// </summary>
	 class RUNTIME_API PropertyInfo final: public MemberInfo
	{
	private:
		friend class PropertyInfo_impl;
		PropertyInfo_impl* impl;
		friend class ClassInfo;
		friend class EnumInfo;
		PropertyInfo(const wchar_t* name, PropertyFlags flags, PropertyType* type);

	public:

		~PropertyInfo();

		bool operator ==(const PropertyInfo& info) const;
		bool operator !=(const PropertyInfo& info) const;

		/// <summary>
		/// 获取名称
		/// </summary>
		///<return></return>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取属性类型
		/// </summary>
		///<return></return>
		const PropertyType* GetType() const;

		/// <summary>
		/// 获取属性标识
		/// </summary>
		///<return></return>
		PropertyFlags GetFlags() const;

		/// <summary>
		/// 获取属性政策
		/// </summary>
		///<return></return>
		PropertyPolicy GetPolicy() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 属性标识
		/// </summary>
		_declspec(property(get = GetFlags)) PropertyFlags Flags;

		/// <summary>
		/// 属性类型
		/// </summary>
		_declspec(property(get = GetType)) const PropertyType* Type;

		/// <summary>
		/// 属性政策
		/// </summary>
		_declspec(property(get = GetPolicy)) PropertyPolicy Policy;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		Any Get(Object* obj, bool nothrow = true) const;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">值</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Set(Object* obj, const Any& value, bool nothrow = true) const;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		Any Get(const Object* obj, bool nothrow = true) const;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="value">值</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Set(const Object* obj, const Any& value, bool nothrow = true) const;

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
	};
}