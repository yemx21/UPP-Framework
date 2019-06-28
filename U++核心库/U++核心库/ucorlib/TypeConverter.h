#pragma once

#include "Any.h"
namespace System
{
	/// <summary>
	/// 类型转换器
	/// </summary>
	class RUNTIME_API TypeConverter : _noncopyable
	{
	protected:
		virtual ~TypeConverter();
	public:
		/// <summary>
		/// 判断是否支持从指定类型的转换
		/// </summary>
		/// <param name="type">指定类型</param>
		virtual bool CanConvertFrom(_type type) const = 0;

		/// <summary>
		/// 判断是否支持转换到指定类型
		/// </summary>
		/// <param name="type">指定类型</param>
		virtual bool CanConvertTo(_type type) const = 0;

		/// <summary>
		/// 从目标对象转换成指定对象
		/// </summary>
		/// <param name="obj">目标对象</param>
		/// <param name="loc">区域性信息</param>
		/// <returns>指定对象</returns>
		virtual Any ConvertFrom(const Any& obj, const CultureInfo& info) const = 0;

		/// <summary>
		/// 将目标对象转换成指定对象
		/// </summary>
		/// <param name="obj">目标对象</param>
		/// <param name="type">指定类型</param>
		/// <param name="loc">区域性信息</param>
		/// <returns>指定对象</returns>
		virtual Any ConvertTo(const Any& obj, _type type, const CultureInfo& info) const = 0;

		/// <summary>
		/// 从指定字符串解析成指定对象
		/// </summary>
		/// <param name="str">指定字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		virtual Any ConvertFromString(const std::wstring& str, const CultureInfo& info) const = 0;

		/// <summary>
		/// 将指定对象转换成字符串
		/// </summary>
		/// <param name="obj">指定对象</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		virtual std::wstring ConvertToString(const Any& obj, const CultureInfo& info) const = 0;
	};
}