#include "FileReader.h"

using namespace System::IO;

int FileReader::Peek(bool nothrow)
{
	return -1;
}

bool FileReader::IsEndOfStream()
{
	return true;
}

int FileReader::Read(bool nothrow)
{
	return -1;
}

ULong FileReader::Read(wchar_t* buffer, ULong bufferCount, ULong index, ULong count, bool nothrow)
{
	if (buffer == nullptr)
	{
		if (!nothrow) throw std::exception("empty buffer");
		return 0UL;
	}
	if (bufferCount - index < count)
	{
		if (!nothrow) throw std::exception("invalid index and count");
		return 0UL;
	}
	ULong num = 0;
	do
	{
		int num1 = Read();
		if (num1 == -1) break;
		ULong num2 = num;
		num = num2 + 1;
		buffer[index + num2] = (wchar_t)num1;
	} while (num < count);
	return num;
}

ULong FileReader::ReadBlock(wchar_t* buffer, ULong bufferBytes, ULong index, ULong count, bool nothrow)
{
	ULong num;
	ULong num1 = 0;
	do
	{
		ULong num2 = Read(buffer, bufferBytes, index + num1, count - num1);
		num = num2;
		num1 = num1 + num2;
	} while (num > 0 && num1 < count);
	return num1;
}

std::wstring FileReader::ReadLine(bool nothrow)
{
	int num;
	std::wstring stringBuilder = std::wstring();
	while (true)
	{
		num = Read(nothrow);
		if (num == -1)
		{
			return stringBuilder;
		}
		if (num == 13 || num == 10)
		{
			break;
		}
		stringBuilder.push_back((wchar_t)num);
	}
	if (num == 13 && Peek() == 10)
	{
		Read(nothrow);
	}
	return stringBuilder;
}

std::wstring FileReader::ReadToEnd(bool nothrow)
{
	wchar_t chrArray[2048];
	std::wstring stringBuilder = std::wstring();
	stringBuilder.reserve(2048);
	while (true)
	{
		int num = Read((wchar_t*)chrArray, 2048, 0, 2048);
		if (num == 0) break;
		stringBuilder.append(chrArray, num);
	}
	return stringBuilder;
}