#pragma once 
#include "Type.h"
#include "DefaultVariadicAny.h"
#include "OptionalVariadicAny.h"
#include <memory>

namespace System
{
	class Object;

	class ClassInfo;
	class EnumInfo;
	class ConstructorType;
	class ConstructorInfo_impl;


	ENUM_FLAGS(ConstructorFlags)
	/// <summary>
	/// Enum 构造函数标识
	/// </summary>
	enum class ConstructorFlags
	{
		/// <summary>
		/// 公有
		/// </summary>
		Public = 0x0001,
		/// <summary>
		/// 保护
		/// </summary>
		Protected = 0x0002,
		/// <summary>
		/// 私有
		/// </summary>
		Private = 0x0004,
		/// <summary>
		/// 虚拟
		/// </summary>
		Virtual = 0x0010,
		/// <summary>
		/// 默认
		/// </summary>
		Default = Public
	};

	/// <summary>
	/// 构造函数信息
	/// </summary>
	class RUNTIME_API ConstructorInfo final: public MemberInfo
	{
	private:
		friend class ConstructorInfo_impl;
		ConstructorInfo_impl* impl;
		friend class ClassInfo;
		friend class EnumInfo;
		ConstructorInfo(const wchar_t* name, ConstructorFlags flags, ConstructorType* type, SharedDefaultVariadicAny defaultParams = nullptr);

	public:

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取构造类型
		/// </summary>
		/// <returns></returns>
		const ConstructorType* GetType() const;

		/// <summary>
		/// 获取构造函数标识
		/// </summary>
		/// <returns></returns>
		const ConstructorFlags GetFlags() const;

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		Object* Invoke(bool nothrow = true) const;

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		Object* Invoke(const OptionalVariadicAny& parameters, bool nothrow = true) const;

		/// <summary>
		/// 获取构造函数所属类
		/// </summary>
		/// <returns></returns>
		_type GetDeclaringClass() const;

		/// <summary>
		/// 获取构造函数是否含有参数
		/// </summary>
		/// <returns></returns>
		bool GetHasParameters() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;
		/// <summary>
		/// 构造函数类型
		/// </summary>
		__declspec(property(get = GetType)) const ConstructorType* Type;
		/// <summary>
		/// 构造函数标识
		/// </summary>
		__declspec(property(get = GetFlags)) ConstructorFlags Flags;
		/// <summary>
		/// 构造函数所属类
		/// </summary>
		__declspec(property(get = GetDeclaringClass)) const System::Type* DeclaringClass;
		/// <summary>
		/// 获取构造函数是否含有参数
		/// </summary>
		__declspec(property(get = GetHasParameters)) bool HasParameters;

		~ConstructorInfo();

		bool operator ==(const ConstructorInfo& info) const;
		bool operator !=(const ConstructorInfo& info) const;
	};
}