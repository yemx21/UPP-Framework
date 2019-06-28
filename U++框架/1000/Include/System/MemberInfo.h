#pragma once
#include "Config.h"
#include <cstdint>

namespace System
{
	class Type;
	class Assembly;
	class NameSpace;

	/// <summary>
	/// 成员信息类
	/// </summary>
	class MemberInfo
	{
	public:

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual const wchar_t* GetName() const = 0;
	};
}