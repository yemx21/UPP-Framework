#pragma once
#include "FileReader.h"
#include "FileStream.h"
#include "Encodings.h"
#include "CultureInfo.h"
#include <codecvt>
#include <mbstring.h>

using namespace System;

namespace System
{
	namespace IO
	{
		/// <summary>
		/// 文件流读取
		/// </summary>
		class RUNTIME_API FileStreamReader : public FileReader
		{
		private:		
			static const int DefaultFileStreamBufferSize = 4096;
			static const int MinBufferSize = 128;
			FileStream* base;

			char* byteBuffer;

			ULong byteBufferSize;

			wchar_t* charBuffer;

			int charPos;

			int charLen;

			int byteLen;

			int bytePos;

			int _maxCharsPerBuffer;

			bool _checkPreamble;

			bool _isBlocked;

			bool _closable;

			Encodings _encoding;

			CultureInfo* _loc;

			int ReadBuffer();

		public:
			~FileStreamReader();

			/// <summary>
			/// 创建 <see cref="FileStreamReader"/> 实例
			/// </summary>
			/// <remarks>最小文件缓存大小为128，且建议使用默认的4096，若自定义缓存大小请选用128的倍数</remarks>
			/// <param name="path">文件位置</param>
			/// <param name="encoding">编码</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <param name="detectEncodingFromByteOrderMarks">自动检查编码.</param>
			/// <param name="loc">区域性信息</param>
			/// <param name="bufferSize">文件缓存大小</param>	
			FileStreamReader(const wchar_t* path, Encodings encoding, bool nothrow = true, bool detectEncodingFromByteOrderMarks = true, const CultureInfo& loc = CultureInfo::Current(), int bufferSize = DefaultFileStreamBufferSize);
			
			/// <summary>
			/// 返回下一个可用的字符
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			int Peek(bool nothrow = true) override;

			/// <summary>
			/// 判断已经读取到结尾
			/// </summary>
			bool IsEndOfStream() override;

			/// <summary>
			/// 读取下一个可用的字符
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			int Read(bool nothrow = true) override;

			/// <summary>
			/// 读取字符
			/// </summary>
			/// <param name="userBuffer">字符</param>
			/// <param name="userOffset">字符索引</param>
			/// <param name="desiredChars">期望字符数</param>
			/// <param name="readToUserBuffer">是否成功读取</param>
			/// <returns>字符数</returns>
			ULong ReadBuffer(wchar_t* userBuffer, int userOffset, int desiredChars, bool& readToUserBuffer);

			/// <summary>
			/// 读取字符
			/// </summary>
			/// <param name="buffer">字符</param>
			/// <param name="bufferCount">字符数</param>
			/// <param name="index">字符开始索引</param>
			/// <param name="count">读取数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>实际读取字符数</returns>
			ULong Read(wchar_t* buffer, ULong bufferCount, ULong index, ULong count, bool nothrow = true)override;

			/// <summary>
			/// 读取字符块
			/// </summary>
			/// <param name="buffer">字符</param>
			/// <param name="bufferBytes">字符大小</param>
			/// <param name="index">字符开始索引</param>
			/// <param name="count">读取数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>实际读取字符数</returns>
			ULong ReadBlock(wchar_t* buffer, ULong bufferCount, ULong index, ULong count, bool nothrow = true) override;

			/// <summary>
			/// 读取下一行
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			std::wstring ReadLine(bool nothrow = true) override;

			/// <summary>
			/// 读取到结尾
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			std::wstring ReadToEnd(bool nothrow = true) override;

		};

	}
}