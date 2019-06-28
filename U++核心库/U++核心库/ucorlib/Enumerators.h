#pragma once
#include "Config.h"
namespace System
{
	class ClassInfo;
	class EnumInfo;

	class MethodInfo;
	class MethodEnumerator_impl;
	class MethodReverseEnumerator_impl;
	class MethodQueryEnumerator_impl;

	/// <summary>
	/// 方法枚举器类
	/// </summary>
	class RUNTIME_API MethodEnumerator: _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class MethodEnumerator_impl;
		MethodEnumerator_impl* impl;
		MethodEnumerator();
	public:			
		~MethodEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>
		const MethodInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const MethodInfo* Current;
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
	/// 方法枚举器类（倒序）
	/// </summary>
	class RUNTIME_API MethodReverseEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class  MethodReverseEnumerator_impl;
		MethodReverseEnumerator_impl* impl;
		MethodReverseEnumerator();
	public:
		~MethodReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const MethodInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const MethodInfo* Current;
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
	/// 方法枚举器类（自定义）
	/// </summary>
	class RUNTIME_API MethodQueryEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class MethodQueryEnumerator_impl;
		MethodQueryEnumerator_impl* impl;
		MethodQueryEnumerator();
	public:
		~MethodQueryEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const MethodInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const MethodInfo* Current;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	class OperatorInfo;
	class OperatorEnumerator_impl;
	class OperatorReverseEnumerator_impl;
	class OperatorQueryEnumerator_impl;

	/// <summary>
	/// 运算符枚举器类
	/// </summary>
	class RUNTIME_API OperatorEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class OperatorEnumerator_impl;
		OperatorEnumerator_impl* impl;
		OperatorEnumerator();
	public:
		~OperatorEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const OperatorInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const OperatorInfo* Current;
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
	/// 运算符枚举器类（倒序）
	/// </summary>
	class RUNTIME_API OperatorReverseEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class  OperatorReverseEnumerator_impl;
		OperatorReverseEnumerator_impl* impl;
		OperatorReverseEnumerator();
	public:
		~OperatorReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const OperatorInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const OperatorInfo* Current;
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
	/// 运算符枚举器类（自定义）
	/// </summary>
	class RUNTIME_API OperatorQueryEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class OperatorQueryEnumerator_impl;
		OperatorQueryEnumerator_impl* impl;
		OperatorQueryEnumerator();
	public:
		~OperatorQueryEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const OperatorInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const OperatorInfo* Current;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	class PropertyInfo;
	class PropertyEnumerator_impl;
	class PropertyReverseEnumerator_impl;
	class PropertyQueryEnumerator_impl;

	/// <summary>
	/// 属性枚举器类
	/// </summary>
	class RUNTIME_API PropertyEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class PropertyEnumerator_impl;
		PropertyEnumerator_impl* impl;
		PropertyEnumerator();
	public:
		~PropertyEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const PropertyInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const PropertyInfo* Current;
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
	/// 属性枚举器类（倒序）
	/// </summary>
	class RUNTIME_API PropertyReverseEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class  PropertyReverseEnumerator_impl;
		PropertyReverseEnumerator_impl* impl;
		PropertyReverseEnumerator();
	public:
		~PropertyReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const PropertyInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const PropertyInfo* Current;
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
	/// 属性枚举器类（自定义）
	/// </summary>
	class RUNTIME_API PropertyQueryEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class PropertyQueryEnumerator_impl;
		PropertyQueryEnumerator_impl* impl;
		PropertyQueryEnumerator();
	public:
		~PropertyQueryEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const PropertyInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const PropertyInfo* Current;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	class ConstructorInfo;
	class ConstructorEnumerator_impl;
	class ConstructorReverseEnumerator_impl;
	class ConstructorQueryEnumerator_impl;

	/// <summary>
	/// 构造函数枚举器类
	/// </summary>
	class RUNTIME_API ConstructorEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class ConstructorEnumerator_impl;
		ConstructorEnumerator_impl* impl;
		ConstructorEnumerator();
	public:
		~ConstructorEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const ConstructorInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const ConstructorInfo* Current;
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
	/// 构造函数枚举器类（倒序）
	/// </summary>
	class RUNTIME_API ConstructorReverseEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class  ConstructorReverseEnumerator_impl;
		ConstructorReverseEnumerator_impl* impl;
		ConstructorReverseEnumerator();
	public:
		~ConstructorReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const ConstructorInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const ConstructorInfo* Current;
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
	/// 构造函数枚举器类（自定义）
	/// </summary>
	class RUNTIME_API ConstructorQueryEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class ConstructorQueryEnumerator_impl;
		ConstructorQueryEnumerator_impl* impl;
		ConstructorQueryEnumerator();
	public:
		~ConstructorQueryEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const ConstructorInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const ConstructorInfo* Current;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	class Attribute;
	class AttributeEnumerator_impl;
	class AttributeReverseEnumerator_impl;
	class AttributeQueryEnumerator_impl;

	/// <summary>
	/// 标签枚举器类
	/// </summary>
	class RUNTIME_API AttributeEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class AttributeEnumerator_impl;
		AttributeEnumerator_impl* impl;
		AttributeEnumerator();
	public:
		~AttributeEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const Attribute* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const Attribute* Current;
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
	/// 标签枚举器类（倒序）
	/// </summary>
	class RUNTIME_API AttributeReverseEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class  AttributeReverseEnumerator_impl;
		AttributeReverseEnumerator_impl* impl;
		AttributeReverseEnumerator();
	public:
		~AttributeReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const Attribute* GetCurrent() const;


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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const Attribute* Current;
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
	/// 标签枚举器类（自定义）
	/// </summary>
	class RUNTIME_API AttributeQueryEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class AttributeQueryEnumerator_impl;
		AttributeQueryEnumerator_impl* impl;
		AttributeQueryEnumerator();
	public:
		~AttributeQueryEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const Attribute* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const Attribute* Current;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	class Field;
	class FieldEnumerator_impl;
	class FieldReverseEnumerator_impl;
	class FieldQueryEnumerator_impl;

	/// <summary>
	/// 成员变量枚举器类
	/// </summary>
	class RUNTIME_API FieldEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class FieldEnumerator_impl;
		FieldEnumerator_impl* impl;
		FieldEnumerator();
	public:
		~FieldEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const FieldInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const FieldInfo* Current;
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
	/// 成员变量枚举器类（倒序）
	/// </summary>
	class RUNTIME_API FieldReverseEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class  FieldReverseEnumerator_impl;
		FieldReverseEnumerator_impl* impl;
		FieldReverseEnumerator();
	public:
		~FieldReverseEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const FieldInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const FieldInfo* Current;
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
	/// 成员变量枚举器类（自定义）
	/// </summary>
	class RUNTIME_API FieldQueryEnumerator : _noncopyable
	{
	private:
		friend class ClassInfo;
		friend class EnumInfo;
		friend class FieldQueryEnumerator_impl;
		FieldQueryEnumerator_impl* impl;
		FieldQueryEnumerator();
	public:
		~FieldQueryEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>

		const FieldInfo* GetCurrent() const;

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
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const FieldInfo* Current;
		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;
		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

}