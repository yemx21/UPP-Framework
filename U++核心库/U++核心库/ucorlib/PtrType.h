#pragma once 
#include "Type.h"
#include "InertiaString.h"
namespace System
{
	class PtrType_impl;

	/// <summary>
	/// 指针类型
	/// </summary>
	class RUNTIME_API PtrType final: public Type
	{
	protected:
		friend class TypeFactory;

		/// <summary>
		/// 创建<see cref="PtrType"/>实例
		/// </summary>
		/// <param name="type">指定基类型</param>
		/// <param name="dimension">维数</param>
		/// <returns></returns>
		static const PtrType* Create(_type type, UInt dimension);
	public:

		virtual ~PtrType();

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual const wchar_t* GetName() const;

		/// <summary>
		/// 获取基类型
		/// </summary>
		/// <returns></returns>
		_type GetBasicType() const;

		/// <summary>
		/// 获取维数
		/// </summary>
		/// <returns></returns>
		UInt8 GetDimension() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 基类型
		/// </summary>
		__declspec(property(get = GetBasicType)) _type BasicType;

		/// <summary>
		/// 维数
		/// </summary>
		__declspec(property(get = GetDimension)) UInt8 Dimension;

	private:
		friend class Type; 
		friend class PtrType_impl;
		_type ptrType;
		UInt rank;
		InertiaString name;
		PtrType(_type type, UInt rank);

	};
}