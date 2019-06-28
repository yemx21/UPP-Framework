#pragma once
#include "Type.h"
#include "Any.h"
#include "MethodInfo.h"
#include "ConstructorInfo.h"
#include "FieldInfo.h"
#include "PropertyInfo.h"
#include "OperatorInfo.h"
#include "Attribute.h"
#include <memory>
#include "Predicates.h"
#include "Enumerators.h"
namespace System
{
	class Factory;
	/// <summary>
	/// 枚举信息枚举
	/// </summary>
	enum class EnumInfoError
	{
		/// <summary>
		/// 成功
		/// </summary>
		OK = 0,
		/// <summary>
		/// 无错误
		/// </summary>
		None = 0,

		/// <summary>
		/// 已存在
		/// </summary>
		AlreadyExisted,
		/// <summary>
		/// 未知错误
		/// </summary>
		Unknown,
		/// <summary>
		/// 空参数
		/// </summary>
		EmptyArgument,
		/// <summary>
		/// 无效参数
		/// </summary>
		InvalidType,
		/// <summary>
		/// 默认构造函数已存在
		/// </summary>
		DefaultConstructorExisted,
	};

	struct EnumInfo_impl;
	struct EnumInfo_internal;
	class EnumReverseEnumerator_impl;
	class EnumEnumerator_impl;

	/// <summary>
	/// 枚举值枚举器类
	/// </summary>
	class RUNTIME_API EnumEnumerator:_noncopyable
	{
	private:
		friend class EnumInfo;
		friend class EnumEnumerator_impl;
		EnumEnumerator_impl* impl;
		EnumEnumerator();
	public:
		~EnumEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		size_t GetCount() const;

		/// <summary>
		/// 获取当前子项名称
		/// </summary>
		/// <returns></returns>
		std::wstring GetCurrentName() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>
		StandardInterger GetCurrentValue() const;

		/// <summary>
		/// 获取当前子项哈希值
		/// </summary>
		/// <returns></returns>
		uhash GetCurrentHashCode() const;

		/// <summary>
		/// 获取下一个子项
		/// </summary>
		/// <returns></returns>
		void Next();

		/// <summary>
		/// 获取上一个子项
		/// </summary>
		/// <returns></returns>
		void Previous();

		/// <summary>
		/// 获取是否迭代至末尾
		/// </summary>
		/// <returns></returns>
		bool GetIsEnd() const;

		/// <summary>
		/// 当前子项名称
		/// </summary>
		__declspec(property(get = GetCurrentName)) std::wstring CurrentName;
		/// <summary>
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrentValue)) StandardInterger CurrentValue;
		/// <summary>
		/// 当前子项哈希值
		/// </summary>
		__declspec(property(get = GetCurrentHashCode)) uhash CurrentHashCode;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) size_t Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	/// <summary>
	/// 枚举值枚举器类（倒序）
	/// </summary>
	class RUNTIME_API EnumReverseEnumerator :_noncopyable
	{
	private:
		friend class EnumInfo;
		friend class EnumReverseEnumerator_impl;
		EnumReverseEnumerator_impl* impl;
		EnumReverseEnumerator();
	public:
		~EnumReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		size_t GetCount() const;

		/// <summary>
		/// 获取当前子项名称
		/// </summary>
		/// <returns></returns>
		std::wstring GetCurrentName() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>
		StandardInterger GetCurrentValue() const;

		/// <summary>
		/// 获取当前子项哈希值
		/// </summary>
		/// <returns></returns>
		uhash GetCurrentHashCode() const;

		/// <summary>
		/// 获取下一个子项
		/// </summary>
		/// <returns></returns>
		void Next();

		/// <summary>
		/// 获取上一个子项
		/// </summary>
		/// <returns></returns>
		void Previous();

		/// <summary>
		/// 获取是否迭代至末尾
		/// </summary>
		/// <returns></returns>
		bool GetIsEnd() const;

		/// <summary>
		/// 当前子项名称
		/// </summary>
		__declspec(property(get = GetCurrentName)) std::wstring CurrentName;
		/// <summary>
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrentValue)) StandardInterger CurrentValue;
		/// <summary>
		/// 当前子项哈希值
		/// </summary>
		__declspec(property(get = GetCurrentHashCode)) uhash CurrentHashCode;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) size_t Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	/// <summary>
	/// 枚举信息类
	/// </summary>
	class RUNTIME_API EnumInfo final: public Type
	{
	private:
		friend class System::Factory;
		friend class Object;
		EnumInfo_impl* impl; 
		EnumInfo_internal* internal;
		EnumInfoError InternalAddValue(const wchar_t* valueName, const StandardInterger& value, uhash hashCode);
	public:
		typedef void(*DefineMetaFunc)(EnumInfo& type);
	protected:
		friend struct EnumInfo_internal;
		/// <summary>
		/// 创建<see cref="EnumInfo"/>实例.
		/// </summary>
		/// <param name="name">枚举名称</param>
		/// <param name="value">值类型</param>
		EnumInfo(const wchar_t* name, _type valueType);

	public:
		/// <summary>
		/// 创建<see cref="EnumInfo"/>实例
		/// </summary>
		/// <param name="pointer">枚举信息指针</param>
		/// <param name="func">枚举信息定义函数</param>
		/// <param name="name">枚举名称</param>
		/// <param name="valueType">枚举强类型</param>
		/// <remarks>内部宏专用</remarks>
		static void Create(Type const*& pointer, DefineMetaFunc func, const wchar_t* name, _type valueType);

		/// <summary>
		/// 创建<see cref="EnumInfo"/>实例
		/// </summary>
		/// <param name="pointer">枚举信息指针</param>
		/// <param name="name">枚举名称</param>
		/// <param name="valueType">枚举强类型</param>
		/// <remarks>内部宏专用</remarks>
		template<class T, class BASE>
		static typename std::enable_if< std::is_integral<BASE>::value, void>::type Create(Type const*& pointer, const wchar_t* name)
		{
			Create(pointer, &T::DefineMeta, name, TypeFactory::Create<BASE>());
		}

		~EnumInfo();

	public:
		template<class T>	
		typename std::enable_if<std::is_enum<T>::value, EnumInfoError>::type AddValue(const wchar_t* valueName, T value, uhash hashCode)
		{
			return InternalAddValue(valueName, static_cast<std::underlying_type<T>::type>(value), hashCode);
		};

		/// <summary>
		/// 尝试解析指定字符串
		/// </summary>
		/// <param name="valueName">指定字符串</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		bool TryParse(const wchar_t* valueName, StandardInterger& result) const;

		/// <summary>
		/// 尝试从指定对象获取字符串表达
		/// </summary>
		/// <param name="value">指定对象</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		bool TryGetValueName(const StandardInterger& value, std::wstring& result) const;

		/// <summary>
		/// 尝试从指定枚举类型的对象获取字符串表达
		/// </summary>
		/// <param name="enumType">指定枚举类型</param>
		/// <param name="value">指定对象</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryGetValueName(_type enumType, const StandardInterger& value, std::wstring& result);

		/// <summary>
		/// 尝试解析指定字符串
		/// </summary>
		/// <param name="hashCode">哈希值</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		bool TryParse(uhash hashCode, StandardInterger& result) const;

		/// <summary>
		/// 尝试从指定对象获取字符串表达
		/// </summary>
		/// <param name="value">指定对象</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		bool TryGetHashCode(const StandardInterger& value, uhash& result) const;

		/// <summary>
		/// 尝试从指定枚举类型的对象获取哈希值表达
		/// </summary>
		/// <param name="enumType">指定枚举类型</param>
		/// <param name="value">指定对象</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryGetHashCode(_type enumType, const StandardInterger& value, uhash& result);

		/// <summary>
		/// 获取枚举值枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<EnumEnumerator> GetEnumEnumerator() const;

		/// <summary>
		/// 获取枚举值枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<EnumReverseEnumerator> GetEnumReverseEnumerator() const;

		/// <summary>
		/// 获取方法枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<MethodEnumerator> GetMethodEnumerator() const;

		/// <summary>
		/// 获取方法枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<MethodReverseEnumerator> GetMethodReverseEnumerator() const;

		/// <summary>
		/// 获取构造函数枚举器
		/// </summary>
		/// <returns></returns>	
		std::unique_ptr<ConstructorEnumerator> GetConstructorEnumerator() const;

		/// <summary>
		/// 获取构造函数枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<ConstructorReverseEnumerator> GetConstructorReverseEnumerator() const;

		/// <summary>
		/// 获取属性枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<PropertyEnumerator> GetPropertyEnumerator() const;

		/// <summary>
		/// 获取属性枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<PropertyReverseEnumerator> GetPropertyReverseEnumerator() const;

		/// <summary>
		/// 获取运算符枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<OperatorEnumerator> GetOperatorEnumerator() const;

		/// <summary>
		/// 获取运算符枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<OperatorReverseEnumerator> GetOperatorReverseEnumerator() const;

		/// <summary>
		/// 获取标签枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<AttributeEnumerator> GetAttributeEnumerator() const;

		/// <summary>
		/// 获取标签枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<AttributeReverseEnumerator> GetAttributeReverseEnumerator() const;

		/// <summary>
		/// 获取成员变量枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<FieldEnumerator> GetFieldEnumerator() const;

		/// <summary>
		/// 获取成员变量枚举器（倒序）
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<FieldReverseEnumerator> GetFieldReverseEnumerator() const;


		/// <summary>
		/// 获取成员变量数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfFields()const;

		/// <summary>
		/// 获取方法数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfMethods()const;

		/// <summary>
		/// 获取构造函数变量数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfConstructors()const;

		/// <summary>
		/// 获取属性数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfProperties()const;

		/// <summary>
		/// 获取运算符数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfOperators()const;

		/// <summary>
		/// 获取标签数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfAttributes()const;

		/// <summary>
		/// 获取枚举值数
		/// </summary>
		/// <returns></returns>
		size_t GetCount() const;

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetName() const;

		/// <summary>
		/// 查找第一个匹配的构造函数
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">构造函数标识</param>
		/// <returns></returns>
		const ConstructorInfo* FindFirstConstructor(const wchar_t* name, ConstructorFlags flags = ConstructorFlags::Public)const;

		/// <summary>
		/// 查找第一个匹配的方法
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">方法标识</param>
		/// <returns></returns>
		const MethodInfo* FindFirstMethod(const wchar_t* name, MethodFlags flags = MethodFlags::Public)const;

		/// <summary>
		/// 查找第一个匹配的属性
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <returns></returns>
		const PropertyInfo* FindFirstProperty(const wchar_t* name, PropertyFlags flags = PropertyFlags::Public)const;

		/// <summary>
		/// 查找第一个匹配的运算符
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="op">运算符类型</param>
		/// <param name="defaultDeclaration">默认声明</param>
		/// <returns></returns>
		const OperatorInfo* FindFirstOperator(const wchar_t* name, Operators op, bool defaultDeclaration = true)const;

		/// <summary>
		/// 查找第一个匹配的运算符
		/// </summary>
		/// <param name="op">运算符类型</param>
		/// <param name="defaultDeclaration">默认声明</param>
		/// <returns></returns>
		const OperatorInfo* FindFirstOperator(Operators op, bool defaultDeclaration = true) const;

		/// <summary>
		/// 查找第一个匹配的标签
		/// </summary>
		/// <param name="name">名称</param>
		/// <returns></returns>
		const Attribute* FindFirstAttribute(const wchar_t* name) const;

		/// <summary>
		/// 查找第一个匹配的标签
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标签标识</param>
		/// <returns></returns>
		const Attribute* FindFirstAttribute(const wchar_t* name, AttributeFlags flag) const;

		/// <summary>
		/// 查找第一个匹配的构造函数
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标签标识</param>
		/// <returns></returns>
		const Attribute* FindFirstAttribute(AttributeFlags flag) const;

		/// <summary>
		/// 查找第一个匹配的成员变量
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">成员变量标识</param>
		/// <returns></returns>
		const FieldInfo* FindFirstField(const wchar_t* name, FieldFlags flags) const;

		/// <summary>
		/// 获取默认构造函数
		/// </summary>
		/// <returns></returns>
		const ConstructorInfo* GetDefaultConstructor() const;

		/// <summary>
		/// 获取第一个匹配的构造函数
		/// </summary>
		/// <param name="predicate">构造函数匹配器</param>
		/// <returns></returns>
		const ConstructorInfo* FindFirstConstructor(ConstructorPredicate predicate)const;

		/// <summary>
		/// 获取第一个匹配的方法
		/// </summary>
		/// <param name="predicate">方法匹配器</param>
		/// <returns></returns>
		const MethodInfo* FindFirstMethod(MethodPredicate predicate)const;

		/// <summary>
		/// 获取第一个匹配的属性
		/// </summary>
		/// <param name="predicate">属性匹配器</param>
		/// <returns></returns>
		const PropertyInfo* FindFirstProperty(PropertyPredicate predicate)const;

		/// <summary>
		/// 获取第一个匹配的运算符
		/// </summary>
		/// <param name="predicate">运算符匹配器</param>
		/// <returns></returns>
		const OperatorInfo* FindFirstOperator(OperatorPredicate predicate)const;

		/// <summary>
		/// 获取第一个匹配的标签
		/// </summary>
		/// <param name="predicate">标签匹配器</param>
		/// <returns></returns>
		const Attribute* FindFirstAttribute(AttributePredicate predicate)const;

		/// <summary>
		/// 获取第一个匹配的成员变量
		/// </summary>
		/// <param name="predicate">成员变量匹配器</param>
		/// <returns></returns>
		const FieldInfo* FindFirstField(FieldPredicate predicate)const;

		/// <summary>
		/// 获取自定义构造函数匹配器枚举器
		/// </summary>
		/// <param name="predicate">构造函数匹配器</param>
		/// <returns></returns>
		std::unique_ptr<ConstructorQueryEnumerator> FindConstructors(ConstructorPredicate predicate)const;

		/// <summary>
		/// 获取自定义方法匹配器枚举器
		/// </summary>
		/// <param name="predicate">方法匹配器</param>
		/// <returns></returns>
		std::unique_ptr<MethodQueryEnumerator> FindMethods(MethodPredicate predicate)const;

		/// <summary>
		/// 获取自定义属性匹配器枚举器
		/// </summary>
		/// <param name="predicate">属性匹配器</param>
		/// <returns></returns>
		std::unique_ptr<PropertyQueryEnumerator> FindProperties(PropertyPredicate predicate)const;

		/// <summary>
		/// 获取自定义运算符匹配器枚举器
		/// </summary>
		/// <param name="predicate">运算符匹配器</param>
		/// <returns></returns>
		std::unique_ptr<OperatorQueryEnumerator> FindOperators(OperatorPredicate predicate)const;

		/// <summary>
		/// 获取自定义标签匹配器枚举器
		/// </summary>
		/// <param name="predicate">标签匹配器</param>
		/// <returns></returns>
		std::unique_ptr<AttributeQueryEnumerator> FindAttributes(AttributePredicate predicate)const;

		/// <summary>
		/// 获取自定义成员变量匹配器枚举器
		/// </summary>
		/// <param name="predicate">成员变量匹配器</param>
		/// <returns></returns>
		std::unique_ptr<FieldQueryEnumerator> FindFields(FieldPredicate predicate)const;

		/// <summary>
		/// 获取枚举强类型
		/// </summary>
		/// <returns></returns>
		_type GetValueType() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 默认构造函数
		/// </summary>
		__declspec(property(get = GetDefaultConstructor)) const ConstructorInfo* DefaultConstructor;

		/// <summary>
		/// 方法数
		/// </summary>
		__declspec(property(get = GetNumberOfMethods)) unsigned int NumberOfMethods;

		/// <summary>
		/// 成员变量数
		/// </summary>
		__declspec(property(get = GetNumberOfFields)) unsigned int NumberOfFields;

		/// <summary>
		/// 构造函数数
		/// </summary>
		__declspec(property(get = GetNumberOfConstructors)) unsigned int NumberOfConstructors;

		/// <summary>
		/// 属性数
		/// </summary>
		__declspec(property(get = GetNumberOfProperties)) unsigned int NumberOfProperties;

		/// <summary>
		/// 运算符数
		/// </summary>
		__declspec(property(get = GetNumberOfOperators)) unsigned int NumberOfOperators;

		/// <summary>
		/// 标签数
		/// </summary>
		__declspec(property(get = GetNumberOfAttributes)) unsigned int NumberOfAttributes;

		/// <summary>
		/// 枚举值数
		/// </summary>
		__declspec(property(get = GetCount)) size_t Count;

		/// <summary>
		/// 枚举强类型
		/// </summary>
		__declspec(property(get = GetValueType)) _type ValueType;

		/// <summary>
		///增加构造函数
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">构造函数类型</param>
		/// <param name="defaultParams">默认参数</param>
		/// <returns></returns>
		EnumInfoError AddConstructor(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams = nullptr);

		/// <summary>
		///增加默认构造函数
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">构造函数类型</param>
		/// <returns></returns>
		EnumInfoError AddDefaultConstructor(const wchar_t* name, ConstructorFlags flags, ConstructorType* type);

		/// <summary>
		///增加方法
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">方法类型</param>
		/// <param name="defaultParams">默认参数</param>
		/// <returns></returns>
		EnumInfoError AddMethod(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams = nullptr);

		/// <summary>
		///增加属性
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">属性类型</param>
		/// <returns></returns>
		EnumInfoError AddProperty(const wchar_t* name, PropertyFlags flags, PropertyType* type);

		/// <summary>
		///增加成员变量
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">成员变量类型</param>
		/// <returns></returns>
		EnumInfoError AddField(const wchar_t* name, FieldFlags flags, FieldType* type);

		/// <summary>
		///增加运算符
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">运算符类型</param>
		/// <returns></returns>
		EnumInfoError AddOperator(const wchar_t* name, OperatorType* type);

		/// <summary>
		///增加标签
		/// </summary>
		/// <param name="name">名称</param>
		/// <returns></returns>
		EnumInfoError AddAttribute(Attribute* attribute);

		/// <summary>
		/// 调用方法
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">方法标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="params">参数</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true)const;

		/// <summary>
		/// 调用方法
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">方法标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="params">参数</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, const Object* obj, const OptionalVariadicAny& params, bool nothrow = true)const;

		/// <summary>
		/// 调用方法
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">方法标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="params">参数</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, nullptr_t obj, const OptionalVariadicAny& params, bool nothrow = true)const;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, Object* obj, bool nothrow = true)const;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, const Object* obj, bool nothrow = true)const;

		/// <summary>
		/// 获取属性值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool GetProperty(const wchar_t* name, PropertyFlags flags, Any* result, nullptr_t obj, bool nothrow = true)const;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <param name="value">值</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, Object* obj, bool nothrow = true)const;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <param name="value">值</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, const Object* obj, bool nothrow = true)const;

		/// <summary>
		/// 设置属性值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">属性标识</param>
		/// <param name="value">值</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool SetProperty(const wchar_t* name, PropertyFlags flags, const Any& value, nullptr_t obj, bool nothrow = true)const;


		/// <summary>
		/// 获取成员变量值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">成员变量标识</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool GetField(const wchar_t* name, FieldFlags flags, Any* result, nullptr_t obj, bool nothrow = true)const;

		/// <summary>
		/// 设置成员变量值
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">成员变量标识</param>
		/// <param name="value">值</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool SetField(const wchar_t* name, FieldFlags flags, const Any& value, Object* obj, bool nothrow = true)const;

		/// <summary>
		/// 调用运算符
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="op">运算符类型</param>
		/// <param name="defaultDeclaration">默认声明</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="params">参数</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, Object* obj, const RealVariadicAny& params, bool nothrow = true) const;

		/// <summary>
		/// 调用运算符
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="op">运算符类型</param>
		/// <param name="defaultDeclaration">默认声明</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="params">参数</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, const Object* obj, const RealVariadicAny& params, bool nothrow = true) const;

		/// <summary>
		/// 调用运算符
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="op">运算符类型</param>
		/// <param name="defaultDeclaration">默认声明</param>
		/// <param name="result">结果</param>
		/// <param name="obj">当前类型的指定对象</param>
		/// <param name="params">参数</param>
		/// <param name="nothrow">无错误报告</param>
		/// <returns></returns>
		bool InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, nullptr_t obj, const RealVariadicAny& params, bool nothrow = true) const;	};
}
