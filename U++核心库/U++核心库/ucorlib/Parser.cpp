#include "Parser.h"
#include "StringHelper.h"
using namespace System;

bool Parser::TryParseBool(const std::wstring& input, bool& result)
{
	std::wstring ci = input;
	StringHelper::Trim(ci);
	size_t csize = ci.size();
	if (csize <1) return false;
	if (csize == 1)
	{
		wchar_t fC = ci[0];
		if (fC == 'T' || fC == 't' || fC =='1')
		{
			result = true;
			return true;
		}
		else if (fC == 'F' || fC == 'f' || fC == '0')
		{
			result = false;
			return true;
		}
		else
			return false;
	}
	if (csize == 4)
	{
		if (ci == L"True" || ci == L"true")
		{
			result = true;
			return true;
		}
	}
	if (csize == 5)
	{
		if (ci == L"False" || ci == L"false")
		{
			result = true;
			return true;
		}
	}
	return false;
}

bool Parser::TryParseInt8(const std::wstring& input, Int8& result, int base)
{
	try
	{
		result = (Int8)stoi(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseUInt8(const std::wstring& input, UInt8& result, int base)
{
	try
	{
		result = (UInt8)stoi(input, 0, base); 
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseInt16(const std::wstring& input, Int16& result, int base)
{
	try
	{
		result = (Int16)stoi(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}

}

bool Parser::TryParseUInt16(const std::wstring& input, UInt16& result, int base)
{
	try
	{
		result = (UInt16)stoi(input, 0, base); 
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseInt24(const std::wstring& input, Int24& result, int base)
{
	try
	{
		result = stoi(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseUInt24(const std::wstring& input, UInt24& result, int base)
{
	try
	{
		result = (UInt32)stoul(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseInt32(const std::wstring& input, Int32& result, int base)
{
	try
	{
		result = stoi(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseUInt32(const std::wstring& input, UInt32& result, int base)
{
	try
	{
		result = (UInt32)stoul(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseLong(const std::wstring& input, long& result, int base)
{
	try
	{
		result = stol(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseULong(const std::wstring& input, ULong& result, int base)
{
	try
	{
		result = stoul(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseInt64(const std::wstring& input, Int64& result, int base)
{
	try
	{
		result = stoll(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseUInt64(const std::wstring& input, UInt64& result, int base)
{
	try
	{
		result = stoull(input, 0, base);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseInt128(const std::wstring& input, Int128& result, int base)
{
	try
	{
		if (base == 10)
		{
			result.low = 0;
			result.high = 0; 
			bool neg = false;
			for(auto& ch: input)
			{
				Byte b;
				if (!neg)
				{
					if (ch == L'-')
					{
						neg = true;
						continue;
					}
				}
				if ((ch >= L'0') && (ch <= L'9'))
				{
					b = (Byte)(ch - L'0');
				}
				else
					return false;

				result = Int128(10) * result;
				result += b;
			}
			if (neg)
			{
				result = -result;
			}
		}
		else if (base == 16)
		{
			size_t len = input.length();
			if (len> 32) return false;
			bool hi = false;
			result.low = 0;
			result.high = 0;
			int pos = 0;
			for (int i = len - 1; i >= 0; i--)
			{
				wchar_t ch = input[i];
				ULong b;
				if ((ch >= L'0') && (ch <= L'9'))
				{
					b = (ULong)(ch - L'0');
				}
				else if ((ch >= L'A') && (ch <= L'F'))
				{
					b = (ULong)(ch - L'A' + 10);
				}
				else if ((ch >= L'a') && (ch <= L'f'))
				{
					b = (ULong)(ch - L'a' + 10);
				}
				else
				{
					result.low = 0;
					result.high = 0;
					return false;
				}

				if (hi)
				{
					result.high |= b << pos;
					pos += 4;
				}
				else
				{
					result.low |= b << pos;
					pos += 4;
					if (pos == 64)
					{
						pos = 0;
						hi = true;
					}
				}
			}
		}
		else
			return false;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseUInt128(const std::wstring& input, UInt128& result, int base)
{
	try
	{
		if (base == 10)
		{
			result.low = 0;
			result.high = 0;
			for (auto& ch : input)
			{
				Byte b;
				if ((ch >= L'0') && (ch <= L'9'))
				{
					b = (Byte)(ch - L'0');
				}
				else
					return false;

				result = UInt128(10U) * result;
				result += (UInt)b;
			}
		}
		else if (base == 16)
		{
			size_t len = input.length();
			if (len> 32) return false; 
			bool hi = false;
			result.low = 0;
			result.high = 0;
			int pos = 0;
			for (int i = len - 1; i >= 0; i--)
			{
				wchar_t ch = input[i];
				ULong b;
				if ((ch >= L'0') && (ch <= L'9'))
				{
					b = (ULong)(ch - L'0');
				}
				else if ((ch >= L'A') && (ch <= L'F'))
				{
					b = (ULong)(ch - L'A' + 10);
				}
				else if ((ch >= L'a') && (ch <= L'f'))
				{
					b = (ULong)(ch - L'a' + 10);
				}
				else
				{
					result.low = 0;
					result.high = 0;
					return false;
				}

				if (hi)
				{
					result.high |= b << pos;
					pos += 4;
				}
				else
				{
					result.low |= b << pos;
					pos += 4;
					if (pos == 64)
					{
						pos = 0;
						hi = true;
					}
				}
			}
		}
		else
			return false;
		return true;
	}
	catch (...)
	{
		return false;
	}
}


bool Parser::TryParseFloat32(const std::wstring& input, Float32& result)
{
	try
	{
		result = stof(input, 0);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseFloat64(const std::wstring& input, Float64& result)
{
	try
	{
		result = stod(input, 0);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Parser::TryParseFloat128(const std::wstring& input, Float128& result)
{
	try
	{
		result = stold(input, 0);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
