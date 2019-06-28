#pragma once
#include "CultureInfo.h"

namespace System
{
	
	/// <summary>
	/// 格式化接口类
	/// </summary>
	class RUNTIME_API IFormattable
	{
	protected:
		/// <summary>
		/// 自定义转换字符串
		/// </summary>
		/// <param name="fmt">格式</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		virtual std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const = 0;

		constexpr IFormattable() {}

	public:
		/// <summary>
		/// 转换为字符串
		/// </summary>
		/// <param name="fmt">格式</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		std::wstring ToString(const wchar_t* fmt, const CultureInfo& loc) const
		{
			return CustomToString(fmt, loc);
		}

		/// <summary>
		/// 转换为字符串
		/// </summary>
		/// <param name="fmt">格式</param>
		/// <returns></returns>
		std::wstring ToString(const wchar_t* fmt) const
		{
			return CustomToString(fmt, CultureInfo::Current());
		}

		/// <summary>
		/// 转换为字符串
		/// </summary>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		std::wstring ToString(const CultureInfo& loc) const
		{
			return ToString(nullptr, loc);
		};

		/// <summary>
		/// 转换为字符串
		/// </summary>
		/// <returns></returns>
		std::wstring ToString() const
		{
			return ToString(nullptr);
		};
	};
}