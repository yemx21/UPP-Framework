#include "Utils.h"

using namespace uppcl;
using namespace uppcl::utils;


clang::FileID uppcl::utils::CreateFileIDForMemBuffer(clang::Preprocessor &PP, std::unique_ptr<llvm::MemoryBuffer> Buf, clang::SourceLocation Loc)
{
#if CLANG_VERSION_MAJOR != 3 || CLANG_VERSION_MINOR > 4
	return PP.getSourceManager().createFileID(std::unique_ptr<llvm::MemoryBuffer>(Buf.get()), clang::SrcMgr::C_User, 0, 0, Loc);
#else
	return PP.getSourceManager().createFileIDForMemBuffer(Buf, clang::SrcMgr::C_User, 0, 0, Loc);
#endif
}

enum
{
	ISVALID_FILENAME_ERROR = -1,
	INVALID_FILENAME_CLOCK = -2,
	INVALID_FILENAME_AUX = -3,
	INVALID_FILENAME_CON = -4,
	INVALID_FILENAME_NUL = -5,
	INVALID_FILENAME_PRN = -6,
	INVALID_FILENAME_COM1 = -7,
	INVALID_FILENAME_COM2 = -8,
	INVALID_FILENAME_COM3 = -9,
	INVALID_FILENAME_COM4 = -10,
	INVALID_FILENAME_COM5 = -11,
	INVALID_FILENAME_COM6 = -12,
	INVALID_FILENAME_COM7 = -13,
	INVALID_FILENAME_COM8 = -14,
	INVALID_FILENAME_COM9 = -15,
	INVALID_FILENAME_LPT1 = -16,
	INVALID_FILENAME_LPT2 = -17,
	INVALID_FILENAME_LPT3 = -18,
	INVALID_FILENAME_LPT4 = -19,
	INVALID_FILENAME_LPT5 = -20,
	INVALID_FILENAME_LPT6 = -21,
	INVALID_FILENAME_LPT7 = -22,
	INVALID_FILENAME_LPT8 = -23,
	INVALID_FILENAME_LPT9 = -24
};

int uppcl::utils::IsValidFileName(const char *pFileName)
{
	if (!pFileName || !*pFileName)
		return ISVALID_FILENAME_ERROR;

	int nonDot = -1;	// position of the first non dot in the file name
	int dot = -1;		// position of the first dot in the file name
	int len = 0;		// length of the file name

	// If a non-dot character has been encountered

	for (; len < 256 && pFileName[len]; len++)
	{
		if (pFileName[len] == '.')
		{
			if (dot < 0)
				dot = len;
			continue;
		}
		else if (nonDot < 0)
			nonDot = len;

		// The upper characters can be passed with a single check and
		// since only the backslash and bar are above the ampersand
		// it saves memory to do the check this way with little performance
		// cost.
		if (pFileName[len] >= '@')
		{
			if (pFileName[len] == '\\' || pFileName[len] == '|')
				return pFileName[len];

			continue;
		}

		static bool isCharValid[32] =
		{
			//  ' '   !     "      #     $     %     &     '     (     )     *      +     ,      -     .      / 
			true, true, false, true, true, true, true, true, true, true, false, true, true, true, true, false,
			//  0     1     2      3     4     5     6     7     8     9     :      ;     <      =     >      ?
			true, true, true, true, true, true, true, true, true, true, false, true, false, true, false, false
			//  0     1     2      3     4     5     6     7     8     9     :      ;     <      =     >      ?
		};

		// This is faster, at the expense of memory, than checking each
		// invalid character individually. However, either method is much
		// faster than using strchr().
		if (pFileName[len] >= 32)
		{
			if (isCharValid[pFileName[len] - 32])
				continue;
		}
		return pFileName[len];
	}

	if (len == 256)
		return ISVALID_FILENAME_ERROR;

	// if nonDot is still -1, no non-dots were encountered, return a dot (period)
	if (nonDot < 0)
		return '.';

	// if the first character is a dot, the filename is okay
	if (dot == 0)
		return 0;

	// if the file name has a dot, we only need to check up to the first dot
	if (dot > 0)
		len = dot;

	// Since the device names aren't numerous, this method of checking is the
	// fastest. Note that each character is checked with both cases.
	if (len == 3)
	{
		if (pFileName[0] == 'a' || pFileName[0] == 'A')
		{
			if ((pFileName[1] == 'u' || pFileName[1] == 'U') &&
				(pFileName[2] == 'x' || pFileName[2] == 'X'))
				return INVALID_FILENAME_AUX;
		}
		else if (pFileName[0] == 'c' || pFileName[0] == 'C')
		{
			if ((pFileName[1] == 'o' || pFileName[1] == 'O') &&
				(pFileName[2] == 'n' || pFileName[2] == 'N'))
				return INVALID_FILENAME_CON;
		}
		else if (pFileName[0] == 'n' || pFileName[0] == 'N')
		{
			if ((pFileName[1] == 'u' || pFileName[1] == 'U') &&
				(pFileName[2] == 'l' || pFileName[2] == 'L'))
				return INVALID_FILENAME_NUL;
		}
		else if (pFileName[0] == 'p' || pFileName[0] == 'P')
		{
			if ((pFileName[1] == 'r' || pFileName[1] == 'R') &&
				(pFileName[2] == 'n' || pFileName[2] == 'N'))
				return INVALID_FILENAME_PRN;
		}
	}
	else if (len == 4)
	{
		if (pFileName[0] == 'c' || pFileName[0] == 'C')
		{
			if ((pFileName[1] == 'o' || pFileName[1] == 'O') &&
				(pFileName[2] == 'm' || pFileName[2] == 'M') &&
				(pFileName[3] >= '1' || pFileName[3] <= '9'))
				return INVALID_FILENAME_COM1 - (pFileName[3] - '1');
		}
		else if (pFileName[0] == 'l' || pFileName[0] == 'L')
		{
			if ((pFileName[1] == 'p' || pFileName[1] == 'P') &&
				(pFileName[2] == 't' || pFileName[2] == 'T') &&
				(pFileName[3] >= '1' || pFileName[3] <= '9'))
				return INVALID_FILENAME_LPT1 - (pFileName[3] - '1');
		}
	}
	else if (len == 6)
	{
		if ((pFileName[0] == 'c' || pFileName[0] == 'C') &&
			(pFileName[1] == 'l' || pFileName[1] == 'L') &&
			(pFileName[2] == 'o' || pFileName[2] == 'O') &&
			(pFileName[3] == 'c' || pFileName[3] == 'C') &&
			(pFileName[4] == 'k' || pFileName[4] == 'K') &&
			(pFileName[5] == '$' || pFileName[5] == '$'))
			return INVALID_FILENAME_CLOCK;
	}

	return 0;
}

#include <Windows.h>
#include <list>
#include <io.h>
#include <direct.h>

bool uppcl::utils::CreateFolder(const std::string& directory)
{
	static const std::string separators("\\/");

	// If the specified directory name doesn't exist, do our thing
	DWORD fileAttributes = ::GetFileAttributesA(directory.c_str());
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

		// Recursively do it all again for the parent directory, if any
		std::size_t slashIndex = directory.find_last_of(separators);
		if (slashIndex != std::wstring::npos) 
		{
			uppcl::utils::CreateFolder(directory.substr(0, slashIndex));
		}

		// Create the last directory on the path (the recursive calls will have taken
		// care of the parent directories by now)
		BOOL result = ::CreateDirectoryA(directory.c_str(), nullptr);
		if (result == FALSE) {
			return false;
		}
	}
	return true;
}


std::string& uppcl::utils::ReplaceAllDistinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return  str;
}

std::string uppcl::utils::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

std::string uppcl::utils::UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}

bool uppcl::utils::TryParseInt32(const std::string& input, int& result, int base)
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
