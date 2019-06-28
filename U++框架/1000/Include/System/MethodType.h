#pragma once 

#include "Anys.h"
namespace System
{
	
	class Object;
	class ClassInfo;
	class EnumInfo;
	class MethodInfo_impl;
	
	/// <summary>
	/// 方法种类
	/// </summary>
	enum class MethodKinds
	{
		/// <summary>
		/// 普通
		/// </summary>
		Normal,
		/// <summary>
		/// 常量
		/// </summary>
		Const,
		/// <summary>
		/// 可修改
		/// </summary>
		Volatile,
		/// <summary>
		/// 常量可修改
		/// </summary>
		ConstVolatile,
		/// <summary>
		/// 静态
		/// </summary>
		Static,
	};

	/// <summary>
	/// 方法类型
	/// </summary>
	class RUNTIME_API MethodType : public Type
	{
	private:
		Optional<std::wstring> description;
	public:
		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const;

		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const;


		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual bool Invoke(Any* result, volatile Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const;


		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual bool Invoke(Any* result, const volatile Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const;


		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Invoke(Any* result, nullptr_t obj, const OptionalVariadicAny& parameters, bool nothrow = true) const;

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
		/// 获取返回值类型
		/// </summary>
		///<return></return>
		_type GetReturnType() const;

		/// <summary>
		/// 获取参数类型集合
		/// </summary>
		///<return></return>
		const TypeCollection* GetParameterTypes() const;

		/// <summary>
		/// 获取方法种类
		/// </summary>
		///<return></return>
		MethodKinds GetKind() const;

		/// <summary>
		/// 获取参数数
		/// </summary>
		///<return></return>
		unsigned int GetNumberOfParameters() const;

		/// <summary>
		/// 方法种类
		/// </summary>
		_declspec(property(get = GetKind)) MethodKinds Kind;

		/// <summary>
		/// 声明类型
		/// </summary>
		_declspec(property(get = GetClassType)) _type ClassType;

		/// <summary>
		/// 返回值类型
		/// </summary>
		_declspec(property(get = GetReturnType)) _type ReturnType;

		/// <summary>
		/// 参数类型集合
		/// </summary>
		_declspec(property(get = GetParameterTypes)) const TypeCollection* ParameterTypes;

		/// <summary>
		/// 参数数
		/// </summary>
		_declspec(property(get = GetNumberOfParameters)) unsigned int NumberOfParameters;

		virtual ~MethodType();

		bool operator ==(const MethodType& type) const;
		bool operator !=(const MethodType& type) const;
	protected:
		friend class MethodInfo; 
		friend class MethodInfo_impl;
		friend class ClassInfo;
		friend class EnumInfo;
		MethodType();
		_type  returnType;
		TypeCollection* paramTypes;
		SharedDefaultVariadicAny defaultParams;
		_type methodClass;
		MethodKinds kind;
	};

	template<int N, class...>
	class MethodTypeAdapter;

}