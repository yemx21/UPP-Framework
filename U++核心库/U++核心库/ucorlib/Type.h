#pragma once
#include "MemberInfo.h"
#include "Optional.h"
#include "DynamicArray.h"
namespace System
{
	class ClassInfo;
	class FieldInfo;
	class MethodInfo;

	class PtrType;
	class ArrayType;
	class RefType;
	class ModifiedType;
	class TypeFactory;

	/// <summary>
	/// 类型标签
	/// </summary>
	enum class TypeTag
	{
		/// <summary>
		/// 未知
		/// </summary>
		Unknown,

		/// <summary>
		/// 物件
		/// </summary>
		Object,
		/// <summary>
		/// 任何
		/// </summary>
		Any,
		/// <summary>
		/// 变长集合（任何）
		/// </summary>
		VariadicAny,
		/// <summary>
		/// 有实际意义的变长集合（任何）
		/// </summary>
		RealVariadicAny,
		/// <summary>
		/// 可默认变长集合（任何）
		/// </summary>
		DefaultVariadicAny,
		/// <summary>
		/// 可选变长集合（任何）
		/// </summary>
		OptionalVariadicAny,
		/// <summary>
		/// 安全空指针
		/// </summary>
		SafeVoidPtr,
		/// <summary>
		/// 惰性字符串
		/// </summary>
		InertiaString,

		/// <summary>
		/// 空
		/// </summary>
		Void,
		/// <summary>
		/// 字符（窄）
		/// </summary>
		Char,
		/// <summary>
		/// 无符号字符（窄）
		/// </summary>
		UChar,
		/// <summary>
		/// 字符（宽）
		/// </summary>
		WChar,
		/// <summary>
		/// 16位整数
		/// </summary>
		Short,
		/// <summary>
		/// 无符号16位整数
		/// </summary>
		UShort,
		/// <summary>
		/// 整数
		/// </summary>
		Int,
		/// <summary>
		/// 无符号整数
		/// </summary>
		UInt,
		/// <summary>
		/// 24位整数
		/// </summary>
		Int24,
		/// <summary>
		/// 无符号24位整数
		/// </summary>
		UInt24,
		/// <summary>
		/// 32位整数
		/// </summary>
		Long,
		/// <summary>
		/// 无符号32位整数
		/// </summary>
		ULong,
		/// <summary>
		/// 64位整数
		/// </summary>
		LongLong,
		/// <summary>
		/// 无符号64位整数
		/// </summary>
		ULongLong,
		/// <summary>
		/// 128位整数
		/// </summary>
		Int128,
		/// <summary>
		/// 无符号128位整数
		/// </summary>
		UInt128,
		/// <summary>
		/// 32位单精度浮点数
		/// </summary>
		Float,
		/// <summary>
		/// 64位双精度浮点数
		/// </summary>
		Double,
		/// <summary>
		/// 128位浮点数
		/// </summary>
		LongDouble,
		/// <summary>
		/// 布尔型
		/// </summary>
		Bool,

		/// <summary>
		/// 集合
		/// </summary>
		Array,
		/// <summary>
		/// 类
		/// </summary>
		Struct,
		/// <summary>
		/// 指针指针
		/// </summary>
		SmartPtr,
		/// <summary>
		/// 结果
		/// </summary>
		Result,
		/// <summary>
		/// 可选的
		/// </summary>
		Optional,
		/// <summary>
		/// 指针
		/// </summary>
		Ptr,
		/// <summary>
		/// 引用
		/// </summary>
		Ref,
		/// <summary>
		/// 修饰
		/// </summary>
		Modified,


		/// <summary>
		/// 方法
		/// </summary>
		Method,
		/// <summary>
		/// 构造函数
		/// </summary>
		Constructor,
		/// <summary>
		/// 枚举
		/// </summary>
		Enum,
		/// <summary>
		/// 属性
		/// </summary>
		Property,
		/// <summary>
		/// 成员变量
		/// </summary>
		Field,
		/// <summary>
		/// 运算符
		/// </summary>
		Operator,
	};

	enum class TypeDetails: UShort
	{
		Value, 
		Pointer,
		Pointer2,
		Pointer3,
		Pointer4,
		Pointer5,
		LValueReference,
		RValueReference,
		Const_Value,
		Const_Pointer,
		Const_Pointer2,
		Const_Pointer3,
		Const_Pointer4,
		Const_Pointer5,
		Const_LValueReference,
		Const_RValueReference,
		Volatile_Value,
		Volatile_Pointer,
		Volatile_Pointer2,
		Volatile_Pointer3,
		Volatile_Pointer4,
		Volatile_Pointer5,
		Volatile_LValueReference,
		Volatile_RValueReference,
		ConstVolatile_Value,
		ConstVolatile_Pointer,
		ConstVolatile_Pointer2,
		ConstVolatile_Pointer3,
		ConstVolatile_Pointer4,
		ConstVolatile_Pointer5,
		ConstVolatile_LValueReference,
		ConstVolatile_RValueReference,

		Invalid
	};

	class Type_impl;
	class NameSpace_impl;
	class NameSpace;

	/// <summary>
	/// 类型
	/// </summary>
	class RUNTIME_API Type :public MemberInfo
	{
	private:
		TypeTag _tag;
	protected:
		friend class Type_impl;
		friend class ClassInfo;
		friend class FieldInfo;
		friend class MethodInfo;
		friend class NameSpace_impl;
		friend class NameSpace;


		friend class PtrType;
		friend class RefType;
		friend class ArrayType;
		friend class ModifiedType;
		friend class ResultType;

		/// <summary>
		/// 销毁资源
		/// </summary>
		void destroy();

		/// <summary>
		/// 哈希值
		/// </summary>
		Optional<size_t> _hashCode;

		/// <summary>
		/// 所属命名空间
		/// </summary>
		const System::NameSpace* space;

		/// <summary>
		/// 创建<see cref="Type"/>实例
		/// </summary>
		/// <param name="tag">类型标签</param>
		Type(TypeTag tag);
	public:
		virtual ~Type();

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <return></return>
		virtual const wchar_t* GetName() const;

		/// <summary>
		/// 获取类型标签
		/// </summary>
		/// <return></return>
		TypeTag GetTag() const;

		/// <summary>
		/// 获取是否是物件
		/// </summary>
		/// <return></return>
		bool GetIsObject() const;

		/// <summary>
		/// 获取是否内置
		/// </summary>
		/// <return></return>
		bool GetIsBuiltin() const;

		/// <summary>
		/// 获取是否是纯量
		/// </summary>
		/// <return></return>
		bool GetIsScalar() const;

		/// <summary>
		/// 获取是否是集合
		/// </summary>
		/// <return></return>
		bool GetIsArray() const;

		/// <summary>
		/// 获取是否是指针
		/// </summary>
		/// <return></return>
		bool GetIsPointer() const;

		/// <summary>
		/// 获取是否是类
		/// </summary>
		/// <return></return>
		bool GetIsClass() const;

		/// <summary>
		/// 获取是否是构造函数
		/// </summary>
		/// <return></return>
		bool GetIsConstructor() const;

		/// <summary>
		/// 获取是否是枚举
		/// </summary>
		/// <return></return>
		bool GetIsEnum() const;

		/// <summary>
		/// 获取是否有符号
		/// </summary>
		/// <return></return>
		bool GetIsSigned() const;

		/// <summary>
		/// 获取是否是引用类型
		/// </summary>
		/// <return></return>
		bool GetIsReference() const;

		/// <summary>
		/// 判断是否是指定类型
		/// </summary>
		/// <param name="type">指定类型</param>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool IsType(_type type, bool ignoreModifier=true) const;

		/// <summary>
		/// 判断是否继承于指定类型
		/// </summary>
		/// <param name="info">指定类型</param>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <returns></returns>
		bool IsBaseOf(_type info, bool ignoreModifier = true)const;

		/// <summary>
		/// 判断是否继承于指定类型或是指定类型
		/// </summary>
		/// <param name="info">指定类型</param>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <returns></returns>
		bool IsBaseOfOrSameAs(_type info, bool ignoreModifier = true)const;

		/// <summary>
		/// 判断是否可于指定类型转换
		/// </summary>
		/// <param name="type">指定类型</param>
		/// <return></return>
		bool IsConvertible(_type type)const;

		/// <summary>
		/// 获取是否是纯量
		/// </summary>
		/// <return></return>
		size_t GetHashCode() const;

		/// <summary>
		/// 判断是否是字符串
		/// </summary>
		/// <param name="iswiden">是否是宽字符</param>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool GetIsRawString(bool& iswiden, bool ignoreModifier = true) const;

		/// <summary>
		/// 判断是否是标准库字符串
		/// </summary>
		/// <param name="iswiden">是否是宽字符</param>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool GetIsSTDString(bool& iswiden, bool ignoreModifier = true) const;

		/// <summary>
		/// 判断是否是字符串或标准库字符串
		/// </summary>
		/// <param name="iswiden">是否是宽字符</param>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool GetIsRawOrSTDString(bool& iswiden, bool& israw, bool ignoreModifier = true) const;

		/// <summary>
		/// 判断是否是字符串
		/// </summary>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool DirectGetIsRawString(bool ignoreModifier = true) const;

		/// <summary>
		/// 判断是否是标准库字符串
		/// </summary>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool DirectGetIsSTDString(bool ignoreModifier = true) const;

		/// <summary>
		/// 判断是否是字符串或标准库字符串
		/// </summary>
		/// <param name="ignoreModifier">是否忽略修饰符</param>
		/// <return></return>
		bool DirectGetIsRawOrSTDString(bool ignoreModifier = true) const;

		/// <summary>
		/// 获取所属命名空间
		/// </summary>
		/// <return></return>
		const System::NameSpace* GetNameSpace() const;

		/// <summary>
		/// 获取所属程序集
		/// </summary>
		/// <return></return>
		const System::Assembly* GetAssembly() const;

		/// <summary>
		/// 获取已注册的类型总数
		/// </summary>
		/// <return></return>
		static UInt64 GetCount();

		/// <summary>
		/// 获取类型详细描述
		/// </summary>
		/// <return></return>
		TypeDetails GetDetail() const;

		/// <summary>
		/// 所属命名空间
		/// </summary>
		_declspec(property(get = GetNameSpace)) const System::NameSpace* NameSpace;

		/// <summary>
		/// 所属程序集
		/// </summary>
		_declspec(property(get = GetAssembly)) const System::Assembly* Assembly;

		/// <summary>
		/// 类型标签
		/// </summary>
		_declspec(property(get = GetTag)) TypeTag Tag;

		/// <summary>
		/// 名称
		/// </summary>
		_declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 是否内置
		/// </summary>
		_declspec(property(get = GetIsBuiltin)) bool IsBuiltin;

		/// <summary>
		/// 是否有符号
		/// </summary>
		_declspec(property(get = GetIsSigned)) bool IsSigned;

		/// <summary>
		/// 是否是物件
		/// </summary>
		_declspec(property(get = GetIsObject)) bool IsObject;

		/// <summary>
		/// 是否是纯量
		/// </summary>
		_declspec(property(get = GetIsScalar)) bool IsScalar;

		/// <summary>
		/// 是否是集合
		/// </summary>
		_declspec(property(get = GetIsArray)) bool IsArray;

		/// <summary>
		/// 是否是指针
		/// </summary>
		_declspec(property(get = GetIsPointer)) bool IsPointer;

		/// <summary>
		/// 是否是引用
		/// </summary>
		_declspec(property(get = GetIsReference)) bool IsReference;

		/// <summary>
		/// 是否是类
		/// </summary>
		_declspec(property(get = GetIsClass)) bool IsClass;

		/// <summary>
		/// 是否是构造函数
		/// </summary>
		_declspec(property(get = GetIsConstructor)) bool IsConstructor;

		/// <summary>
		/// 是否是枚举
		/// </summary>
		_declspec(property(get = GetIsEnum)) bool IsEnum;

		/// <summary>
		/// 类型详细描述
		/// </summary>
		_declspec(property(get = GetDetail)) TypeDetails Detail;

		/// <summary>
		/// 哈希值
		/// </summary>
		_declspec(property(get = GetHashCode)) size_t HashCode;

		/// <summary>
		/// 未知类型
		/// </summary>
		static Type* const unknownType;

		/// <summary>
		/// 物件类型
		/// </summary>
		static Type* const objectType;

		/// <summary>
		/// 任何类型
		/// </summary>
		static Type* const anyType;

		/// <summary>
		/// 集合类型（任何）
		/// </summary>
		static Type* const variadicAnyType;
		
		/// <summary>
		/// 有实际意义的变长集合（任何）
		/// </summary>
		static Type* const realVariadicAnyType;

		/// <summary>
		/// 可默认的变长集合（任何）
		/// </summary>
		static Type* const defaultVariadicAnyType;

		/// <summary>
		/// 可选的变长集合（任何）
		/// </summary>
		static Type* const optionalVariadicAnyType;

		/// <summary>
		/// 安全空指针类型
		/// </summary>
		static Type* const safeVoidPtrType;

		/// <summary>
		/// 惰性字符串类型
		/// </summary>
		static Type* const inertiaStringType;

		/// <summary>
		/// 布尔类型
		/// </summary>
		static Type* const boolType;

		/// <summary>
		/// 空类型
		/// </summary>
		static Type* const voidType;

		/// <summary>
		/// 字符类型（窄）
		/// </summary>
		static Type* const charType;

		/// <summary>
		/// 无符号字符类型（窄）
		/// </summary>
		static Type* const ucharType;

		/// <summary>
		/// 字符类型（宽）
		/// </summary>
		static Type* const wcharType;

		/// <summary>
		/// 16位整数类型
		/// </summary>
		static Type* const shortType;

		/// <summary>
		/// 无符号16位整数类型
		/// </summary>
		static Type* const UShortType;

		/// <summary>
		/// 整数类型
		/// </summary>
		static Type* const intType;

		/// <summary>
		/// 无符号整数类型
		/// </summary>
		static Type* const UIntType;

		/// <summary>
		/// 24位整数类型
		/// </summary>
		static Type* const Int24Type;

		/// <summary>
		/// 无符号24位整数类型
		/// </summary>
		static Type* const UInt24Type;

		/// <summary>
		/// 32位整数类型
		/// </summary>
		static Type* const longType;

		/// <summary>
		/// 无符号32位整数类型
		/// </summary>
		static Type* const ULongType;

		/// <summary>
		/// 64位整数类型
		/// </summary>
		static Type* const longlongType;

		/// <summary>
		/// 无符号64位整数类型
		/// </summary>
		static Type* const ULonglongType;		

		/// <summary>
		/// 128位整数类型
		/// </summary>
		static Type* const Int128Type;

		/// <summary>
		/// 无符号128位整数类型
		/// </summary>
		static Type* const UInt128Type;

		/// <summary>
		/// 32位单精度浮点数类型
		/// </summary>
		static Type* const floatType;

		/// <summary>
		/// 64位双精度浮点数类型
		/// </summary>
		static Type* const doubleType;

		/// <summary>
		/// 128位浮点数类型
		/// </summary>
		static Type* const longdoubleType;
	};

	/// <summary>
	/// 标准库类型集合
	/// </summary>
	class RUNTIME_API STDTypes
	{
	public:
		/// <summary>
		/// 字符串（窄）
		/// </summary>
		static _type stringType;
		/// <summary>
		/// 字符串（宽）
		/// </summary>
		static _type wstringType;
	};

	/// <summary>
	/// 内置类型集合
	/// </summary>
	class RUNTIME_API BUILTINTypes
	{
	public:
		/// <summary>
		/// 区域性信息类型
		/// </summary>
		static _type cultureInfoType;
	};


	/// <summary>
	/// 类型集合
	/// </summary>
	typedef DynamicArray<_type> TypeCollection;

}