#pragma once 
#include "Type.h"
#include "DefaultVariadicAny.h"
#include "OptionalVariadicAny.h"
#include <memory>

namespace System
{		
	class Object;

	class EnumInfo;
	class ClassInfo;
	class MethodType;
	class MethodInfo_impl;

	ENUM_FLAGS(MethodFlags)
	/// <summary>
	/// 方法标识
	/// </summary>
	enum class MethodFlags
	{
		/// <summary>
		/// 静态
		/// </summary>
		Static = 0x0001,

		/// <summary>
		/// 常量
		/// </summary>
		Const = 0x0002,

		/// <summary>
		/// 公有
		/// </summary>
		Public = 0x0004,

		/// <summary>
		/// 保护
		/// </summary>
		Protected = 0x0010,

		/// <summary>
		/// 私有
		/// </summary>
		Private = 0x0020,

		/// <summary>
		/// 虚拟
		/// </summary>
		Virtual = 0x0040,

		/// <summary>
		/// 默认
		/// </summary>
		Default = Public
	};

	/// <summary>
	/// 方法信息类
	/// </summary>
	class RUNTIME_API MethodInfo final: public MemberInfo
	{
	private:
		friend class MethodInfo_impl;
		MethodInfo_impl* impl;
		friend class ClassInfo;
		friend class EnumInfo;
		MethodInfo(const wchar_t* name, MethodFlags flags, MethodType* type, SharedDefaultVariadicAny defaultParams = nullptr);

	public:

		/// <summary>
		/// 获取名称
		/// </summary>
		///<return></return>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取方法类型
		/// </summary>
		///<return></return>
		const MethodType* GetType() const;

		/// <summary>
		/// 获取方法标识
		/// </summary>
		///<return></return>
		const MethodFlags GetFlags() const;

		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Invoke(Any* result, Object* obj, const OptionalVariadicAny& params, bool nothrow = true) const;

		/// <summary>
		/// 调用指定对象的当前方法
		/// </summary>
		/// <param name="result">结果</param>
		/// <param name="obj">指定对象</param>
		/// <param name="parameters">参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool Invoke(Any* result, const Object* obj, const OptionalVariadicAny& parameters, bool nothrow = true) const;

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
		/// 获取声明类型
		/// </summary>
		///<return></return>
		_type GetDeclaringClass() const;

		/// <summary>
		/// 名称
		/// </summary>
		__declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		/// 方法类型
		/// </summary>
		__declspec(property(get = GetType)) const MethodType* Type;

		/// <summary>
		/// 方法标识
		/// </summary>
		__declspec(property(get = GetFlags)) MethodFlags Flags;

		/// <summary>
		/// 声明类型
		/// </summary>
		__declspec(property(get = GetDeclaringClass)) const System::Type* DeclaringClass;

		~MethodInfo();

		bool operator ==(const MethodInfo& info) const;
		bool operator !=(const MethodInfo& info) const;

	};

}