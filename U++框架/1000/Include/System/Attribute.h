#pragma once
#include "Any.h"
#include "MemberInfo.h"
#include "TypeConverter.h"
#include "InertiaString.h"
using namespace System;

namespace System
{
	ENUM_OPERATORS(AttributePolicy)
	/// <summary>
	/// 标签政策枚举
	/// </summary>
	enum class AttributePolicy
	{
		/// <summary>
		/// 所有
		/// </summary>
		All,
		/// <summary>
		/// 程序集
		/// </summary>
		Assembly,
		/// <summary>
		/// 命名空间
		/// </summary>
		NameSpace,

		/// <summary>
		/// 类
		/// </summary>
		Class,
		/// <summary>
		/// 结构体
		/// </summary>
		Struct,
		/// <summary>
		/// 枚举
		/// </summary>
		Enum,

		/// <summary>
		/// 成员变量
		/// </summary>
		Field,
		/// <summary>
		/// 属性
		/// </summary>
		Property,
		/// <summary>
		/// 方法
		/// </summary>
		Method,
		/// <summary>
		/// 构造函数
		/// </summary>
		Constructor,
				
	};

	class ClassInfo;
	class EnumInfo;


	ENUM_OPERATORSEX(AttributeFlags, UInt64)
	/// <summary>
	/// 标签标识枚举
	/// </summary>
	enum class AttributeFlags:UInt64
	{
		/// <summary>
		/// 类型转换器
		/// </summary>
		TypeConverter,
		/// <summary>
		/// 自定义
		/// </summary>
		Custom = 512,
		/// <summary>
		/// 用户自定义
		/// </summary>
		User = 8196,
	};

	class Attribute_impl;

	/// <summary>
	/// 标签类
	/// </summary>
	class RUNTIME_API Attribute :public MemberInfo
	{
	private:
		Attribute_impl* impl;
	public:
		/// <summary>
		/// 获取标签标识
		/// </summary>
		/// <returns></returns>
		AttributeFlags GetFlag() const;

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetName() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 标签标识
		/// </summary>
		__declspec(property(get = GetFlag)) AttributeFlags Flag;
	protected:
		friend class ClassInfo;
		friend class EnumInfo;
		Attribute(const wchar_t* name, AttributeFlags flag, AttributePolicy policy, bool allowMultiple, bool inherited);

	public:
		virtual ~Attribute();
	};


	class RUNTIME_API TypeConverterAttribute final: public Attribute
	{
	private:
		std::shared_ptr<TypeConverter> _converter;
		InertiaString _name;
	public:
		template<class T>
		TypeConverterAttribute(const wchar_t* name, std::shared_ptr<T> converter) :Attribute(name, AttributeFlags::TypeConverter, AttributePolicy::Class | AttributePolicy::Struct, true, true), _name(name), _converter(std::dynamic_pointer_cast<TypeConverter>(converter))
		{
			static_assert(std::is_base_of<TypeConverter, T>::value, "T should be based on TypeConverter");
		}

		~TypeConverterAttribute();

		/// <summary>
		/// 判断是否可从指定类型转换到当前类型
		/// </summary>
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		bool CanConvertFrom(_type type) const;

		/// <summary>
		/// 判断是否可从指定类型转换到目标类型
		/// </summary>
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		bool CanConvertTo(_type type) const;

		/// <summary>
		/// 从指定对象转换到当前类型
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="info">区域性信息</param>
		/// <returns></returns>
		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;

		/// <summary>
		/// 从指定对象转换到目标类型
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="type">指定类型</param>
		/// <param name="info">区域性信息</param>
		/// <returns></returns>
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		/// <summary>
		/// 从指定字符串转换到当前类型
		/// </summary>
		/// <param name="str">指定字符串</param>
		/// <param name="info">区域性信息</param>
		/// <returns></returns>
		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;

		/// <summary>
		/// 从指定对象转换到字符串类型
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="info">区域性信息</param>
		/// <returns></returns>
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}