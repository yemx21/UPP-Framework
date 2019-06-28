#pragma once 
#include "OptionalVariadicAny.h"
#include "DefaultVariadicAny.h"
namespace System
{
	class Object;
	class ClassInfo; 
	class EnumInfo;
	class ConstructorInfo;
	class ConstructorInfo_impl;

	/// <summary>
	/// 构造函数类型
	/// </summary>
	class RUNTIME_API ConstructorType : public Type
	{
	private:
		Optional<std::wstring> description;
	public:
		/// <summary>
		/// 调用当前构造函数
		/// </summary>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		virtual Object* Invoke(const OptionalVariadicAny& parameters, bool nothrow = true) const = 0;

		/// <summary>
		/// 获取类类型
		/// </summary>
		/// <returns></returns>
		_type GetClassType() const;

		/// <summary>
		/// 获取返回值类型
		/// </summary>
		/// <returns></returns>
		_type GetReturnType() const;

		/// <summary>
		/// 获取函数类型集合
		/// </summary>
		/// <returns></returns>
		const TypeCollection* GetParameterTypes() const;

		/// <summary>
		/// 获取参数数
		/// </summary>
		/// <returns></returns>
		unsigned int GetNumberOfParameters() const;

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetName() const;

		/// <summary>
		/// 类类型
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

		virtual ~ConstructorType();

		bool operator ==(const ConstructorType& type) const;
		bool operator !=(const ConstructorType& type) const;
	protected:
		friend class ConstructorInfo;
		friend class ConstructorInfo_impl;
		friend class ClassInfo;
		friend class EnumInfo;
		ConstructorType();
		_type  returnType;
		TypeCollection* paramTypes;
		SharedDefaultVariadicAny defaultParams;
		_type constructorClass;
		bool       isStatic;
	};

	template<int N, class __C>
	class ConstructorTypeAdapter;

}