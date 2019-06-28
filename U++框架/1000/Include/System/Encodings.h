#pragma once
#include "Config.h"
#include "CultureInfo.h"
namespace System
{
	/// <summary>
	/// 编码帮助类
	/// </summary>
	class RUNTIME_API EncodingHelper
	{
	public:
		/// <summary>
		/// 获取最大字符数
		/// </summary>
		/// <param name="byteCount">字节数</param>
		/// <param name="encoding">编码</param>
		/// <returns>字符数</returns>
		static ULong GetMaxCharCount(unsigned int byteCount, Encodings encoding);

		/// <summary>
		/// UTF8转换到UCS
		/// </summary>
		/// <param name="bytes">字节</param>
		/// <param name="byteCount">字节数</param>
		/// <param name="chars">字符</param>
		/// <param name="charCount">字符数</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns>ULong.</returns>
		static ULong Utf8GetUCS(char* bytes, ULong byteCount, wchar_t* chars, ULong charCount, const CultureInfo& loc, bool nothrow = true);

		/// <summary>
		/// UTF16小端转换到UCS
		/// </summary>
		/// <param name="bytes">字节</param>
		/// <param name="byteCount">字节数</param>
		/// <param name="chars">字符</param>
		/// <param name="charCount">字符数</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns>字符数</returns>
		static ULong Utf16LEGetUCS(char* bytes, ULong byteCount, wchar_t* chars, ULong charCount, const CultureInfo& loc, bool nothrow = true);

		/// <summary>
		/// UTF16大端转换到UCS
		/// </summary>
		/// <param name="bytes">字节</param>
		/// <param name="byteCount">字节数</param>
		/// <param name="chars">字符</param>
		/// <param name="charCount">字符数</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns>字符数</returns>
		static ULong Utf16BEGetUCS(char* bytes, ULong byteCount, wchar_t* chars, ULong charCount, const CultureInfo& loc, bool nothrow = true);

		/// <summary>
		/// UTF32小端转换到UCS
		/// </summary>
		/// <param name="bytes">字节</param>
		/// <param name="byteCount">字节数</param>
		/// <param name="chars">字符</param>
		/// <param name="charCount">字符数</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns>字符数</returns>
		static ULong Utf32LEGetUCS(char* bytes, ULong byteCount, wchar_t* chars, ULong charCount, const CultureInfo& loc, bool nothrow = true);

		/// <summary>
		/// UTF32大端转换到UCS
		/// </summary>
		/// <param name="bytes">字节</param>
		/// <param name="byteCount">字节数</param>
		/// <param name="chars">字符</param>
		/// <param name="charCount">字符数</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns>字符数</returns>
		static ULong Utf32BEGetUCS(char* bytes, ULong byteCount, wchar_t* chars, ULong charCount, const CultureInfo& loc, bool nothrow = true);

		/// <summary>
		/// UCS字节转换
		/// </summary>
		/// <param name="bytes">字节</param>
		/// <param name="byteSize">字节大小</param>
		/// <param name="byteIndex">字节开始索引</param>
		/// <param name="byteCount">字节数</param>
		/// <param name="chars">The chars.</param>
		/// <param name="charSize">字符大小</param>
		/// <param name="charIndex">字符开始索引</param>
		/// <param name="encoding">编码</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns>字符数</returns>
		static ULong GetUCS(char* bytes, ULong byteSize, ULong byteIndex, unsigned int byteCount, wchar_t* chars, ULong charSize, ULong charIndex, Encodings encoding, const CultureInfo& loc, bool nothrow = true);

	};
}