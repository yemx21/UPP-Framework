#pragma once
#include "Optional.h"
#include "Type.h"
namespace System
{
	class OptionalType_impl;

	/// <summary>
	/// 可空类型
	/// </summary>
	class RUNTIME_API OptionalType final: public Type
	{
	protected:
		friend class TypeFactory;

		/// <summary>
		/// 创建<see cref="OptionalType"/>实例
		/// </summary>
		/// <param name="type">类型</param>
		/// <returns></returns>
		static const OptionalType* Create(_type type);
	public:

		virtual ~OptionalType();

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
		/// 获取类型
		/// </summary>
		/// <returns></returns>
		_type GetType() const;

		/// <summary>
		/// 类型
		/// </summary>
		__declspec(property(get = GetType)) _type Type;

	private:
		friend class Type;
		friend class OptionalType_impl;
		OptionalType_impl* impl;
		OptionalType(_type type);

	};

}