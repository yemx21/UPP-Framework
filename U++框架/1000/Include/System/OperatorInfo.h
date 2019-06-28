#pragma once 
#include "Type.h"
#include "Operators.h"
#include "RealVariadicAny.h"
#include <memory>

namespace System
{
	class Object;
	class ClassInfo;
	class EnumInfo;
	class OperatorType;
	class OperatorInfo_impl;

	/// <summary>
	/// 运算符信息类
	/// </summary>
	class RUNTIME_API OperatorInfo final
	{
	private:
		friend class ClassInfo; 
		friend class EnumInfo;
		friend class OperatorInfo_impl;
		OperatorInfo_impl* impl;
		friend class ClassInfo;
		OperatorInfo(const wchar_t* name, OperatorType* type);

	public:

		/// <summary>
		/// 获取名称
		/// </summary>
		///<return></return>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取运算符种类
		/// </summary>
		///<return></return>
		Operators GetOperator() const;

		/// <summary>
		/// 获取运算符类型
		/// </summary>
		///<return></return>
		const OperatorType* GetType() const;

		/// <summary>
		/// 获取声明类型
		/// </summary>
		///<return></return>
		_type GetDeclaringClass() const;

		/// <summary>
		/// 调用指定对象的当前运算符
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool  Invoke(Any* result, Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const;
		
		/// <summary>
		/// 调用指定对象的当前运算符
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool  Invoke(Any* result, const Object* obj, const RealVariadicAny& parameters, bool nothrow = true) const;

		/// <summary>
		/// 调用指定对象的当前运算符
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Invoke(Any* result, nullptr_t obj, const RealVariadicAny& parameters, bool nothrow) const;

		/// <summary>
		/// 声明类型
		/// </summary>
		_declspec(property(get = GetDeclaringClass)) _type DeclaringClass;

		/// <summary>
		/// 运算符类型
		/// </summary>
		_declspec(property(get = GetType)) const OperatorType* Type;

		/// <summary>
		/// 运算符种类
		/// </summary>
		_declspec(property(get = GetOperator)) Operators Operator;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		~OperatorInfo();

		bool operator ==(const OperatorInfo& info) const;
		bool operator !=(const OperatorInfo& info) const;
	};
}