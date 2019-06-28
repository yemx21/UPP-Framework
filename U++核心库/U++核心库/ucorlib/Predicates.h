#pragma once

namespace System
{
	class ConstructorInfo;
	class MethodInfo;
	class PropertyInfo;
	class OperatorInfo;
	class Attribute;
	class FieldInfo;

	/// <summary>
	/// 构造函数匹配回调函数
	/// </summary>
	typedef bool(*ConstructorPredicate)(const ConstructorInfo* info);

	/// <summary>
	/// 方法匹配回调函数
	/// </summary>
	typedef bool(*MethodPredicate)(const MethodInfo* info);

	/// <summary>
	/// 属性匹配回调函数
	/// </summary>
	typedef bool(*PropertyPredicate)(const PropertyInfo* info);

	/// <summary>
	/// 运算符匹配回调函数
	/// </summary>
	typedef bool(*OperatorPredicate)(const OperatorInfo* info);

	/// <summary>
	/// 标签匹配回调函数
	/// </summary>
	typedef bool(*AttributePredicate)(const Attribute* attr);

	/// <summary>
	/// 成员变量匹配回调函数
	/// </summary>
	typedef bool(*FieldPredicate)(const FieldInfo* attr);
}