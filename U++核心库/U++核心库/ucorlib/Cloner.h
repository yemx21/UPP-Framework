#pragma once
#include <string>
#include "Config.h"

namespace System
{
	/// <summary>
	/// 克隆类
	/// </summary>
	 class RUNTIME_API Cloner
	{
	public:
		/// <summary>
		/// 克隆字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <returns></returns>
		static wchar_t* Clone(const wchar_t* str);

		/// <summary>
		/// 克隆宽字节字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <returns></returns>
		static char* Clone(const char* str);

		/// <summary>
		/// 克隆宽字节字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <returns></returns>
		static wchar_t* Clone(const std::wstring& str);

		/// <summary>
		/// 克隆指定字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <returns></returns>
		static char* Clone(const std::string& str);

		/// <summary>
		/// 克隆指定宽字节字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="memSize">字节大小</param>
		/// <returns></returns>
		static wchar_t* Clone(const wchar_t* str, UInt32& memSize);

		/// <summary>
		/// 克隆指定字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="memSize">字节大小</param>
		/// <returns></returns>
		static char* Clone(const char* str, UInt32& memSize);

		/// <summary>
		/// 克隆指定宽字节字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="memSize">字节大小</param>
		/// <returns></returns>
		static wchar_t* Clone(const std::wstring& str, UInt32& memSize);

		/// <summary>
		/// 克隆指定字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="memSize">字节大小</param>
		/// <returns></returns>
		static char* Clone(const std::string& str, UInt32& memSize);

	};
}

