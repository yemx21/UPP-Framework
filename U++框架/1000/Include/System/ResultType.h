#pragma once
#include "Result.h"
#include "Type.h"
namespace System
{
	class ResultType_impl;

	/// <summary>
	/// 结果类型
	/// </summary>
	class RUNTIME_API ResultType final: public Type
	{
	protected:
		friend class TypeFactory;

		/// <summary>
		/// 创建<see cref="ResultType"/>实例
		/// </summary>
		/// <param name="type1">类型1</param>
		/// <param name="type2">类型2</param>
		/// <param name="type3">类型3</param>
		/// <param name="stype">状态类型</param>
		/// <param name="rtype">结果类型种类</param>
		/// <returns></returns>
		static const ResultType* Create(_type type1, _type type2, _type type3, _type stype, ResultTypes rtype);
	public:

		virtual ~ResultType();

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual const wchar_t* GetName() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 获取类型1
		/// </summary>
		/// <returns></returns>
		_type GetType1() const;

		/// <summary>
		/// 类型1
		/// </summary>
		__declspec(property(get = GetType1)) _type Type1;
		
		/// <summary>
		/// 获取类型2
		/// </summary>
		/// <returns></returns>
		_type GetType2() const;

		/// <summary>
		/// 类型2
		/// </summary>
		__declspec(property(get = GetType2)) _type Type2;	

		/// <summary>
		/// 获取类型3
		/// </summary>
		/// <returns></returns>
		_type GetType3() const;

		/// <summary>
		/// 类型3
		/// </summary>
		__declspec(property(get = GetType3)) _type Type3;

		/// <summary>
		/// 获取状态类型
		/// </summary>
		/// <returns></returns>
		_type GetStateType() const;

		/// <summary>
		/// 状态类型
		/// </summary>
		__declspec(property(get = GetStateType)) _type StateType;

		/// <summary>
		/// 获取结果类型种类
		/// </summary>
		/// <returns></returns>
		ResultTypes GetResultType() const;

		/// <summary>
		/// 结果类型种类
		/// </summary>
		__declspec(property(get = GetResultType)) ResultTypes RTypes;
	private:
		friend class Type;
		friend class ResultType_impl;
		ResultType_impl* impl;
		ResultType(_type type1, _type type2, _type type3, _type stype, ResultTypes rtype);

	};

}