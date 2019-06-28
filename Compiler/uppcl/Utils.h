#pragma once
#include "Config.h"
#include <clang/Lex/Preprocessor.h>

namespace uppcl
{
	namespace utils
	{
		clang::FileID CreateFileIDForMemBuffer(clang::Preprocessor &PP, std::unique_ptr<llvm::MemoryBuffer> Buf, clang::SourceLocation Loc);


		/// <summary>
		/// ·Ö¸î×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <param name="delim">·Ö¸ô·û</param>
		/// <param name="results">½á¹û.</param>
		template<class T>
		__inline static void Split(const std::basic_string<T>& input, T delim, std::vector<std::basic_string<T>>& results)
		{
			size_t last = 0;
			size_t index = input.find_first_of(delim, last);
			while (index != std::basic_string<T>::npos)
			{
				results.push_back(input.substr(last, index - last));
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last > 0)
			{
				results.push_back(input.substr(last, index - last));
			}
		}

		/// <summary>
		/// ·Ö¸î×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <param name="delim">·Ö¸ô·û</param>
		/// <param name="results">½á¹û.</param>
		template<class T>
		__inline static void Split(const std::basic_string<T>& input, const std::basic_string<T>& delim, std::vector<std::basic_string<T>>& results)
		{
			size_t last = 0;
			size_t index = input.find_first_of(delim, last);
			while (index != std::basic_string<T>::npos)
			{
				results.push_back(input.substr(last, index - last));
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last > 0)
			{
				results.push_back(input.substr(last, index - last));
			}
		}

		/// <summary>
		/// ·Ö¸î×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <param name="delim">·Ö¸ô·û</param>
		/// <param name="results">½á¹û.</param>
		template<class T>
		__inline static void Split(const std::basic_string<T>& input, const std::basic_string<T>& delim, std::queue<std::basic_string<T>>& results)
		{
			size_t last = 0;
			size_t index = input.find_first_of(delim, last);
			while (index != std::basic_string<T>::npos)
			{
				results.push(input.substr(last, index - last));
				last = index + 1;
				index = input.find_first_of(delim, last);
			}
			if (index - last > 0)
			{
				results.push(input.substr(last, index - last));
			}
		}

		template <class T>
		static std::basic_string<T>& LTrim(std::basic_string<T>& input);

		/// <summary>
		/// ×óĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char>& LTrim(std::basic_string<char>& input)
		{
			return input.erase(0, input.find_first_not_of(" \t\n\r"));
		}

		/// <summary>
		/// ×óĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t>& LTrim(std::basic_string<wchar_t>& input)
		{
			return input.erase(0, input.find_first_not_of(L" \t\n\r"));
		}

		template <class T>
		static std::basic_string<T> LTrim(const std::basic_string<T>& input);

		/// <summary>
		/// ×óĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char> LTrim(const std::basic_string<char>& input)
		{
			std::basic_string<char>::size_type startpos = input.find_first_not_of(" \t\n\r");
			return (startpos == std::basic_string<char>::npos) ? input : input.substr(startpos);
		}

		/// <summary>
		/// ×óĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t> LTrim(const std::basic_string<wchar_t>& input)
		{
			std::basic_string<wchar_t>::size_type startpos = input.find_first_not_of(L" \t\n\r");
			return (startpos == std::basic_string<wchar_t>::npos) ? input : input.substr(startpos);
		}


		template <class T>
		static std::basic_string<T>& RTrim(std::basic_string<T>& input);

		/// <summary>
		/// ÓÒĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char>& RTrim(std::basic_string<char>& input)
		{
			return input.erase(input.find_last_not_of(" \t\n\r") + 1);
		}

		/// <summary>
		/// ÓÒĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t>& RTrim(std::basic_string<wchar_t>& input)
		{
			return input.erase(input.find_last_not_of(L" \t\n\r") + 1);
		}

		template <class T>
		static std::basic_string<T> RTrim(const std::basic_string<T>& input);

		/// <summary>
		/// ÓÒĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char> RTrim(const std::basic_string<char>& input)
		{
			std::basic_string<char>::size_type endpos = input.find_last_not_of(" \t\n\r");
			return (endpos == std::basic_string<char>::npos) ? input : input.substr(0, endpos + 1);

		}

		/// <summary>
		/// ÓÒĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t> RTrim(const std::basic_string<wchar_t>& input)
		{
			std::basic_string<wchar_t>::size_type endpos = input.find_last_not_of(L" \t\n\r");
			return (endpos == std::basic_string<wchar_t>::npos) ? input : input.substr(0, endpos + 1);

		}


		/// <summary>
		/// ĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T>& Trim(std::basic_string<T>& input)
		{
			return LTrim(RTrim(input));
		}

		/// <summary>
		/// ĞŞ¼ô×Ö·û´®
		/// </summary>
		/// <param name="input">ÊäÈëÖµ</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T> Trim(const std::basic_string<T>& input)
		{
			return LTrim(RTrim(input));
		}


		//template<class T>
		//std::basic_string<T>& ReplaceAllDistinct(std::basic_string<T>& str, const std::basic_string<T>& old_value, const std::basic_string<T>& new_value)
		//{
		//	for (std::basic_string<T>::size_type pos(0); pos != std::basic_string<T>::npos; pos += new_value.length()) 
		//	{
		//		if ((pos = str.find(old_value, pos)) != std::basic_string<T>::npos)
		//			str.replace(pos, old_value.length(), new_value);
		//		else   
		//			break;
		//	}
		//	return  str;
		//}


		std::string& ReplaceAllDistinct(std::string& str, const std::string& old_value, const std::string& new_value);

		int IsValidFileName(const char *pFileName);

		bool CreateFolder(const std::string& folderName);

		std::string string_To_UTF8(const std::string & str);

		std::string UTF8_To_string(const std::string & str);

		bool TryParseInt32(const std::string& input, int& result, int base = 10);
	};
}