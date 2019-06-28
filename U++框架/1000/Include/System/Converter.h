#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include "Config.h"

namespace System
{
	/// <summary>
	/// 转换器类
	/// </summary>
	 class RUNTIME_API Converter
	{
	public:		
		/// <summary>
		/// 转换字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="wresult">宽字节字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		static bool Convert(const char* str, std::wstring& wresult, const std::locale& loc) ;

		/// <summary>
		/// 转换字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="aresult">字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		static bool Convert(const wchar_t* str, std::string& aresult, const std::locale& loc) ;

		/// <summary>
		/// 转换字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="wresult">宽字节字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		static bool Convert(std::string const& str, std::wstring& wresult, const std::locale& loc) ;

		/// <summary>
		/// 转换字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="aresult">字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		static bool Convert(std::wstring const& str, std::string& aresult, const std::locale& loc) ;

		/// <summary>
		/// Cs the plus new.
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="loc_t">区域性信息</param>
		/// <returns>宽字节字符串</returns>
		static wchar_t* CPlusNew(const char* str, const _locale_t & loc_t) ;

		/// <summary>
		/// 转换分配字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="loc_t">区域性信息</param>
		/// <returns>字符串</returns>
		static char* CPlusNew(const wchar_t* str, const _locale_t & loc_t) ;

		/// <summary>
		/// 转换字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="loc_t">区域性信息</param>
		/// <returns>宽字节字符串</returns>
		static std::wstring Convert(const char* str, const std::locale& loc, const _locale_t& loc_t) ;

		/// <summary>
		/// 转换字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="loc_t">区域性信息</param>
		/// <returns>字符串</returns>
		static std::string Convert(const wchar_t* str, const std::locale& loc, const _locale_t& loc_t) ;

		/// <summary>
		/// 转换分配字符串
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="loc_t">区域性信息</param>
		/// <returns>宽字节字符串</returns>
		static wchar_t* ConvertNew(const char* str, const std::locale& loc, const _locale_t& loc_t) ;

		/// <summary>
		/// 转换分配字符串
		/// </summary>
		/// <param name="str">宽字节字符串</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="loc_t">区域性信息</param>
		/// <returns>字符串</returns>
		static char* ConvertNew(const wchar_t* str, const std::locale& loc, const _locale_t& loc_t) ;

		/// <summary>
		///转换UCS编码字符串转换到UTF8编码
		/// </summary>
		/// <param name="src">宽字节字符串</param>
		/// <returns>字符串</returns>
		static std::string StringToUTF8(const std::wstring& src) ;

		/// <summary>
		/// 转换UTF8编码字符串转换到UCS编码
		/// </summary>
		/// <param name="src">字符串</param>
		/// <returns>宽字节字符串</returns>
		static std::wstring UTF8ToString(const std::string& src) ;

		/// <summary>
		/// 转换UCS编码字符串转换到UTF16编码
		/// </summary>
		/// <param name="src">宽字节字符串</param>
		/// <returns>字符串</returns>
		static std::string StringToUTF16(const std::wstring& src) ;

		/// <summary>
		/// 转换UTF16编码字符串转换到UCS编码
		/// </summary>
		/// <param name="src">字符串</param>
		/// <returns>宽字节字符串</returns>
		static std::wstring UTF16ToString(const std::string& src) ;

	};
}