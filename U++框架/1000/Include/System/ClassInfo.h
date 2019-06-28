#pragma once 
#include "Type.h"
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
	
	ENUM_FLAGS(ClassFlags)
	/// <summary>
	/// 类标识枚举
	/// </summary>
	enum class ClassFlags
	{
		/// <summary>
		/// 普通
		/// </summary>
		Normal = 0x0000,
		/// <summary>
		/// 抽象
		/// </summary>
		Abstract = 0x0001,
		/// <summary>
		/// 内部
		/// </summary>
		Internal = 0x0002,
	};

	/// <summary>
	/// 类信息错误枚举
	/// </summary>
	enum class ClassInfoError
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
		/// 默认构造函数已存在
		/// </summary>
		DefaultConstructorExisted,
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
		InvalidArgument,
	};

	struct ClassInfo_impl;

	struct ClassInfo_internal;

	class BaseClassEnumerator_impl;

	/// <summary>
	/// 基类枚举器类
	/// </summary>
	class RUNTIME_API BaseClassEnumerator final: _noncopyable
	{
	private:
		friend class ClassInfo;
		BaseClassEnumerator_impl* impl;
		BaseClassEnumerator();
	public:
		~BaseClassEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		///<return></return>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项名称
		/// </summary>
		///<return></return>
		const wchar_t* GetCurrentName() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		///<return></return>
		const ClassInfo* GetCurrent() const;

		/// <summary>
		/// 获取当前子项所属命名空间
		/// </summary>
		///<return></return>
		const NameSpace* GetCurrentNameSpace() const;

		/// <summary>
		/// 获取当前子项所属程序集
		/// </summary>
		///<return></return>
		const Assembly* GetCurrentAssembly() const;

		/// <summary>
		/// 获取下一个子项
		/// </summary>
		///<return></return>
		void Next();

		/// <summary>
		/// 获取上一个子项
		/// </summary>
		///<return></return>
		void Previous();

		/// <summary>
		/// 判断是否迭代至末尾
		/// </summary>
		///<return></return>
		bool GetIsEnd() const;

		/// <summary>
		/// 当前子项名称
		/// </summary>
		__declspec(property(get = GetCurrentName)) const wchar_t* CurrentName;

		/// <summary>
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const ClassInfo* Current;

		/// <summary>
		/// 当前子项所属命名空间
		/// </summary>
		__declspec(property(get = GetCurrentNameSpace)) const NameSpace* CurrentNameSpace;

		/// <summary>
		/// 当前子项所属程序集
		/// </summary>
		__declspec(property(get = GetCurrentAssembly)) const Assembly* CurrentAssembly;

		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;

		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	/// <summary>
	/// 类信息系类
	/// </summary>
	class RUNTIME_API ClassInfo final: public Type
	{
	private:
		friend class System::Factory;
		friend class Object;
		friend struct ClassInfo_internal;

		ClassInfo_impl* impl;
		ClassInfo_internal* internal;

		ClassInfoError AddBaseClass(_type type);
		ClassInfoError AddDefaultConstructor_impl(const wchar_t* name, ConstructorFlags flags, ConstructorType* type);
	public:
		typedef void(*DefineMetaFunc)(ClassInfo& type);
	protected:
		/// <summary>
		/// 创建<see cref="ClassInfo"/>实例.
		/// </summary>
		/// <param name="name">类名称</param>
		/// <param name="size">类大小</param>
		/// <param name="flags">类标识</param>
		ClassInfo(const wchar_t* name, unsigned int size, ClassFlags flags);
	public:

		/// <summary>
		/// 创建<see cref="ClassInfo"/>实例
		/// </summary>
		/// <param name="pointer">类信息指针</param>
		/// <param name="func">类信息定义函数</param>
		/// <param name="name">类名称</param>
		/// <param name="size">类大小</param>
		/// <param name="flags">类标识</param>
		/// <remarks>内部宏专用</remarks>
		static void Create(Type const*& pointer, DefineMetaFunc func, const wchar_t* name, unsigned int size, ClassFlags flags);

		/// <summary>
		/// 创建<see cref="ClassInfo"/>实例
		/// </summary>
		/// <param name="pointer">类信息指针</param>
		/// <param name="name">类名称</param>
		/// <param name="size">类大小</param>
		/// <param name="flags">类标识</param>
		/// <remarks>内部宏专用</remarks>
		template<class T>
		static void Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags)
		{
			Create(pointer, &T::DefineMeta, name, size, flags);
		}

		~ClassInfo();
	public:

		/// <summary>
		/// 获取基类枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<BaseClassEnumerator> GetBaseClassEnumerator() const;

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
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetName() const;		

		/// <summary>
		/// 获取类大小
		/// </summary>
		/// <returns></returns>
		unsigned int GetSize() const;

		/// <summary>
		/// 获取类标识
		/// </summary>
		/// <returns></returns>
		ClassFlags GetFlags() const;

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
		/// 获取基类数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfBaseClasses()const;

		/// <summary>
		/// 判断是否继承于指定类型
		/// </summary>
		/// <param name="info">指定类型</param>
		/// <returns></returns>
		bool IsBaseOf(_type info)const;

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
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 默认构造函数
		/// </summary>
		__declspec(property(get = GetDefaultConstructor)) const ConstructorInfo* DefaultConstructor;

		/// <summary>
		/// 基类数
		/// </summary>
		__declspec(property(get = GetNumberOfBaseClasses)) unsigned int NumberOfBaseClasses;

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
		///增加构造函数
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">构造函数类型</param>
		/// <param name="defaultParams">默认参数</param>
		/// <returns></returns>
		ClassInfoError AddConstructor(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams = nullptr);

		template<class __C>
		/// <summary>
		///增加默认构造函数
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">构造函数类型</param>
		/// <returns></returns>
		ClassInfoError AddDefaultConstructor(const wchar_t* name, ConstructorFlags flags, Object* (*func)())
		{
			return AddDefaultConstructor_impl(name, flags, ConstructorTypeAdapter<0, __C>::Create(func));
		}

		/// <summary>
		///增加方法
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">方法类型</param>
		/// <param name="defaultParams">默认参数</param>
		/// <returns></returns>
		ClassInfoError AddMethod(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams = nullptr);

		/// <summary>
		///增加属性
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">属性类型</param>
		/// <returns></returns>
		ClassInfoError AddProperty(const wchar_t* name, PropertyFlags flags, PropertyType* type); 

		/// <summary>
		///增加成员变量
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">成员变量类型</param>
		/// <returns></returns>
		ClassInfoError AddField(const wchar_t* name, FieldFlags flags, FieldType* type);

		/// <summary>
		///增加运算符
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="flags">标识</param>
		/// <param name="type">运算符类型</param>
		/// <returns></returns>
		ClassInfoError AddOperator(const wchar_t* name, OperatorType* type);

		/// <summary>
		///增加标签
		/// </summary>
		/// <param name="name">名称</param>
		/// <returns></returns>
		ClassInfoError AddAttribute(Attribute* attribute);

		/// <summary>
		/// 增加基类
		/// </summary>
		/// <returns></returns>
		template<typename T>
		ClassInfoError AddBaseClass()
		{
			_type ty = TypeFactory::Create<T>();
			if (ty) return AddBaseClass(ty);
			return ClassInfoError::InvalidArgument;
		}

		/// <summary>
		/// 定义模板参数类型
		/// </summary>
		/// <param name="index">模板参数索引</param>
		/// <param name="argumentType">参数类型</param>
		/// <returns></returns>
		bool DefineTemplateArgument(size_t index, _type argumentType);

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
		bool InvokeMethod(const wchar_t* name, MethodFlags flags, Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow=true)const;

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
		bool InvokeOperator(const wchar_t* name, Operators op, bool defaultDeclaration, Any* result, nullptr_t obj, const RealVariadicAny& params, bool nothrow = true) const;
	};
}