#pragma once
#include "Config.h"
#include <string>
#include <vector>


namespace System
{
	/// <summary>
	/// 解析器
	/// </summary>
	 class RUNTIME_API Parser
	{
	public:
		/// <summary>
		/// 尝试解析布尔型值
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseBool(const std::wstring& input, bool& result) ;
		/// <summary>
		/// 尝试解析8位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseInt8(const std::wstring& input, Int8& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号8位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseUInt8(const std::wstring& input, UInt8& result, int base = 10) ;
		/// <summary>
		/// 尝试解析8位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseInt16(const std::wstring& input, Int16& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号16位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseUInt16(const std::wstring& input, UInt16& result, int base = 10) ;
		/// <summary>
		/// 尝试解析24位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseInt24(const std::wstring& input, Int24& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号24位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseUInt24(const std::wstring& input, UInt24& result, int base = 10) ;
		/// <summary>
		/// 尝试解析32位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseInt32(const std::wstring& input, Int32& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号32位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseUInt32(const std::wstring& input, UInt32& result, int base = 10) ;
		/// <summary>
		/// 尝试解析32位长整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseLong(const std::wstring& input, long& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号32位长整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseULong(const std::wstring& input, ULong& result, int base = 10) ;
		/// <summary>
		/// 尝试解析64位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseInt64(const std::wstring& input, Int64& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号64位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseUInt64(const std::wstring& input, UInt64& result, int base = 10) ;
		/// <summary>
		/// 尝试解析128位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseInt128(const std::wstring& input, Int128& result, int base = 10) ;
		/// <summary>
		/// 尝试解析无符号128位整数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="base">进制</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseUInt128(const std::wstring& input, UInt128& result, int base = 10) ;

		/// <summary>
		/// 尝试解析32位浮点数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseFloat32(const std::wstring& input, Float32& result) ;
		/// <summary>
		/// 尝试解析64位浮点数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseFloat64(const std::wstring& input, Float64& result) ;
		/// <summary>
		/// 尝试解析128位浮点数
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <returns>是否解析成功</returns>
		static bool TryParseFloat128(const std::wstring& input, Float128& result) ;



		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<class T>
		static void Parse(const std::wstring& input, std::vector<T>& result, const std::wstring& delim);

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<bool>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				bool val = false;
				if (TryParseBool(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				bool val = false;
				if (TryParseBool(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int8>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int8 val;
				if (TryParseInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int8 val;
				if (TryParseInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt8>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt8 val;
				if (TryParseUInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt8 val;
				if (TryParseUInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int16>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int16 val;
				if (TryParseInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int16 val;
				if (TryParseInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt16>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt16 val;
				if (TryParseUInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt16 val;
				if (TryParseUInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int24>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int24 val;
				if (TryParseInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int24 val;
				if (TryParseInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt24>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt24 val;
				if (TryParseUInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt24 val;
				if (TryParseUInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int32>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int32 val;
				if (TryParseInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int32 val;
				if (TryParseInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt32>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt32 val;
				if (TryParseUInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt32 val;
				if (TryParseUInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<long>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				long val;
				if (TryParseLong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				long val;
				if (TryParseLong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<ULong>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				ULong val;
				if (TryParseULong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				ULong val;
				if (TryParseULong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int64>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int64 val;
				if (TryParseInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int64 val;
				if (TryParseInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt64>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt64 val;
				if (TryParseUInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt64 val;
				if (TryParseUInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int128>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int128 val;
				if (TryParseInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int128 val;
				if (TryParseInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt128>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt128 val;
				if (TryParseUInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt128 val;
				if (TryParseUInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Float32>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Float32 val;
				if (TryParseFloat32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Float32 val;
				if (TryParseFloat32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Float64>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Float64 val;
				if (TryParseFloat64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Float64 val;
				if (TryParseFloat64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Float128>& result, const std::wstring& delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Float128 val;
				if (TryParseFloat128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Float128 val;
				if (TryParseFloat128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}


		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<class T>
		static void Parse(const std::wstring& input, std::vector<T>& result, wchar_t delim);

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<bool>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				bool val = false;
				if (TryParseBool(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				bool val = false;
				if (TryParseBool(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int8>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int8 val;
				if (TryParseInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int8 val;
				if (TryParseInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt8>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt8 val;
				if (TryParseUInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt8 val;
				if (TryParseUInt8(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int16>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int16 val;
				if (TryParseInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int16 val;
				if (TryParseInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt16>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt16 val;
				if (TryParseUInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt16 val;
				if (TryParseUInt16(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int24>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int24 val;
				if (TryParseInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int24 val;
				if (TryParseInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt24>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt24 val;
				if (TryParseUInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt24 val;
				if (TryParseUInt24(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int32>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int32 val;
				if (TryParseInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int32 val;
				if (TryParseInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt32>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt32 val;
				if (TryParseUInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt32 val;
				if (TryParseUInt32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<long>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				long val;
				if (TryParseLong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				long val;
				if (TryParseLong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<ULong>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				ULong val;
				if (TryParseULong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				ULong val;
				if (TryParseULong(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int64>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int64 val;
				if (TryParseInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int64 val;
				if (TryParseInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt64>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt64 val;
				if (TryParseUInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt64 val;
				if (TryParseUInt64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Int128>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Int128 val;
				if (TryParseInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Int128 val;
				if (TryParseInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<UInt128>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				UInt128 val;
				if (TryParseUInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				UInt128 val;
				if (TryParseUInt128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Float32>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Float32 val;
				if (TryParseFloat32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Float32 val;
				if (TryParseFloat32(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Float64>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Float64 val;
				if (TryParseFloat64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Float64 val;
				if (TryParseFloat64(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

		/// <summary>
		/// 解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="result">结果</param>
		/// <param name="delim">分隔符</param>
		template<>
		static void Parse(const std::wstring& input, std::vector<Float128>& result, wchar_t delim)
		{
			size_t last = std::wstring::npos;
			size_t index = input.find_first_of(delim, last);
			while (index != std::wstring::npos)
			{
				Float128 val;
				if (TryParseFloat128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last>0)
			{
				Float128 val;
				if (TryParseFloat128(input.substr(last, index - last), val))
				{
					result.push_back(val);
				}
			}
		}

	};
}