#pragma once
#include "Config.h"
#include <string>


namespace System
{
	namespace IO
	{
		/// <summary>
		/// 文件读取类
		/// </summary>
		 class RUNTIME_API FileReader
		{
		public:
			/// <summary>
			/// 返回下一个可用的字符
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			virtual int Peek(bool nothrow = true);

			/// <summary>
			/// 读取下一个可用的字符
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			virtual int Read(bool nothrow = true);

			/// <summary>
			/// 读取字符
			/// </summary>
			/// <param name="buffer">字符</param>
			/// <param name="bufferCount">字符数</param>
			/// <param name="index">字符开始索引</param>
			/// <param name="count">读取数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>实际读取字符数</returns>
			virtual ULong Read(wchar_t* buffer, ULong bufferCount, ULong index, ULong count, bool nothrow = true);

			/// <summary>
			/// 读取字符块
			/// </summary>
			/// <param name="buffer">字符</param>
			/// <param name="bufferBytes">字符大小</param>
			/// <param name="index">字符开始索引</param>
			/// <param name="count">读取数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>实际读取字符数</returns>
			virtual ULong ReadBlock(wchar_t* buffer, ULong bufferBytes, ULong index, ULong count, bool nothrow = true);

			/// <summary>
			/// 读取下一行
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			virtual std::wstring ReadLine(bool nothrow = true);

			/// <summary>
			/// 读取到结尾
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>字符</returns>
			virtual std::wstring ReadToEnd(bool nothrow = true);

			/// <summary>
			/// 判断已经读取到结尾
			/// </summary>
			virtual bool IsEndOfStream();

		};
	}
}