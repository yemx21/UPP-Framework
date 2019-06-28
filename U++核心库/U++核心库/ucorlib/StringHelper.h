#pragma once
#include <string>
#include <vector>
#include <algorithm>   
#include <functional>
#include <locale>
#include "CultureInfo.h"

namespace System
{

	/// <summary>
	/// 字符串帮助类
	/// </summary>
	 class StringHelper
	{
	public:

		/// <summary>
		/// 分割字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="delim">分隔符</param>
		/// <param name="results">结果.</param>
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
			if (index - last>0)
			{
				results.push_back(input.substr(last, index - last));
			}
		}

		/// <summary>
		/// 分割字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="delim">分隔符</param>
		/// <param name="results">结果.</param>
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
			if (index - last>0)
			{
				results.push_back(input.substr(last, index - last));
			}
		}

		template <class T>
		static std::basic_string<T>& LTrim(std::basic_string<T>& input);

		/// <summary>
		/// 左修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char>& LTrim(std::basic_string<char>& input)
		{
			return input.erase(0, input.find_first_not_of(" \t\n\r"));
		}

		/// <summary>
		/// 左修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t>& LTrim(std::basic_string<wchar_t>& input)
		{
			return input.erase(0, input.find_first_not_of(L" \t\n\r"));
		}

		template <class T>
		static std::basic_string<T> LTrim(const std::basic_string<T>& input);

		/// <summary>
		/// 左修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char> LTrim(const std::basic_string<char>& input)
		{
			std::basic_string<char>::size_type startpos = input.find_first_not_of(" \t\n\r");
			return (startpos == std::basic_string<char>::npos) ? input : input.substr(startpos);
		}

		/// <summary>
		/// 左修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
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
		/// 右修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char>& RTrim(std::basic_string<char>& input)
		{
			return input.erase(input.find_last_not_of(" \t\n\r") + 1);
		}

		/// <summary>
		/// 右修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t>& RTrim(std::basic_string<wchar_t>& input)
		{
			return input.erase(input.find_last_not_of(L" \t\n\r") + 1);
		}

		template <class T>
		static std::basic_string<T> RTrim(const std::basic_string<T>& input);

		/// <summary>
		/// 右修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<char> RTrim(const std::basic_string<char>& input)
		{
			std::basic_string<char>::size_type endpos = input.find_last_not_of(" \t\n\r");
			return (endpos == std::basic_string<char>::npos) ? input : input.substr(0, endpos + 1);

		}

		/// <summary>
		/// 右修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <>
		__inline static std::basic_string<wchar_t> RTrim(const std::basic_string<wchar_t>& input)
		{
			std::basic_string<wchar_t>::size_type endpos = input.find_last_not_of(L" \t\n\r");
			return (endpos == std::basic_string<wchar_t>::npos) ? input : input.substr(0, endpos + 1);

		}


		/// <summary>
		/// 修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T>& Trim(std::basic_string<T>& input)
		{
			return LTrim(RTrim(input));
		}

		/// <summary>
		/// 修剪字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T> Trim(const std::basic_string<T>& input)
		{
			return LTrim(RTrim(input));
		}





		/// <summary>
		/// 替换第一个匹配字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="strSrc">匹配值</param>
		/// <param name="strDest">替换值</param>
		/// <returns></returns>
		template<class T>
		__inline static std::basic_string<T> & Replace(std::basic_string<T>& input, const std::basic_string<T> &strSrc, const std::basic_string<T> &strDest)
		{
			while (true)
			{
				auto pos = input.find(strSrc);
				if (pos != std::basic_string<T>::npos)
					input.replace(pos, strSrc.size(), strDest);
				else
					break;
			}
			return input;
		}

		/// <summary>
		/// 替换所有匹配字符串 
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="strSrc">匹配值</param>
		/// <param name="strDest">替换值</param>
		/// <returns></returns>
		template<class T>
		__inline static std::basic_string<T> & ReplaceAll(std::basic_string<T>& input, const std::basic_string<T> &strSrc, const std::basic_string<T> &strDest)
		{
			for (typename std::basic_string<T>::size_type pos(0); pos != std::basic_string<T>::npos; pos += strDest.size())
			{
				if ((pos = input.find(strSrc, pos)) != std::basic_string<T>::npos)
					input.replace(pos, strSrc.size(), strDest);
				else   
					break;
			}
			return input;
		}

		/// <summary>
		/// 删除第一个匹配字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="charactor">匹配值</param>
		/// <returns></returns>
		template<typename T>
		__inline static std::basic_string<T>& Erase(std::basic_string<T>& input, const T &charactor)
		{
			input.erase(std::remove_if(input.begin(), input.end(), std::bind2nd(std::equal_to<T>(), charactor)), input.end());
			return input;
		}

		/// <summary>
		/// 首段字符串匹配
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <returns>是否匹配</returns>
		template<typename T>
		__inline static bool StartsWith(const std::basic_string<T>& input, const std::basic_string<T>& test)
		{
			return input.find(test) == 0;
		}

		template<typename T>
		__inline static bool StartsWith(const std::basic_string<T>& input, const std::basic_string<T>& test, StringComparison comparsion);

		/// <summary>
		/// 首段字符串匹配
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <param name="comparsion">字符串比较枚举</param>
		/// <returns>是否匹配</returns>
		template<>
		__inline static bool StartsWith<char>(const std::basic_string<char>& input, const std::basic_string<char>& test, StringComparison comparsion)
		{
			switch (comparsion)
			{
				case StringComparison::CurrentCulture:
				{
					CultureInfo loc = CultureInfo::Current();
					if (input.size() > test.size()) return _strncoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
					return false;
				}
					break;
				case StringComparison::CurrentCultureIgnoreCase:
				{
					CultureInfo loc = CultureInfo::Current();
					if (input.size() > test.size()) return _strnicoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
					return false;
				}
					break;
				case StringComparison::InvariantCulture:
				{
					if (input.size() > test.size()) return _strncoll(input.c_str(), test.c_str(), test.size()) == 0;
					return false;
				}
					break;
				case StringComparison::InvariantCultureIgnoreCase:
				{
					if (input.size() > test.size()) return _strnicoll(input.c_str(), test.c_str(), test.size()) == 0;
					return false;
				}
					break;
			}
			return false;
		}

		/// <summary>
		/// 首段字符串匹配
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <param name="comparsion">字符串比较枚举</param>
		/// <returns>是否匹配</returns>
		template<>
		__inline static bool StartsWith<wchar_t>(const std::basic_string<wchar_t>& input, const std::basic_string<wchar_t>& test, StringComparison comparsion)
		{
			switch (comparsion)
			{
			case StringComparison::CurrentCulture:
			{
				CultureInfo loc = CultureInfo::Current();
				if (input.size() > test.size()) return _wcsncoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale)==0;
				return false;									
			}
				break;
			case StringComparison::CurrentCultureIgnoreCase:
			{
				CultureInfo loc = CultureInfo::Current();
				if (input.size() > test.size()) return _wcsnicoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
				return false;
			}
				break;
			case StringComparison::InvariantCulture:
			{
				if (input.size() > test.size()) return _wcsncoll(input.c_str(), test.c_str(), test.size()) == 0;
				return false;
			}
				break;
			case StringComparison::InvariantCultureIgnoreCase:
			{
				if (input.size() > test.size()) return _wcsnicoll(input.c_str(), test.c_str(), test.size()) == 0;
				return false;
			}
				break;
			}
			return false;
		}

		/// <summary>
		/// 尾端字符串匹配
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <returns>是否匹配</returns>
		template<typename T>
		__inline static bool EndsWith(const std::basic_string<T>& input, const std::basic_string<T>& test)
		{
			size_t i = input.rfind(test);
			return (i != std::basic_string<T>::npos) && (i == (input.length() - test.length()));
		}

		template<typename T>
		__inline static bool EndsWith(const std::basic_string<T>& input, const std::basic_string<T>& test, StringComparison comparsion);
		
		/// <summary>
		/// 尾端字符串匹配
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <param name="comparsion">字符串比较枚举</param>
		/// <returns>是否匹配</returns>
		template<>
		__inline static bool EndsWith<char>(const std::basic_string<char>& input, const std::basic_string<char>& test, StringComparison comparsion)
		{
			switch (comparsion)
			{
			case StringComparison::CurrentCulture:
			{
				CultureInfo loc = CultureInfo::Current();
				if (input.size() > test.size()) return _strncoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
				return false;
			}
				break;
			case StringComparison::CurrentCultureIgnoreCase:
			{
				CultureInfo loc = CultureInfo::Current();
				if (input.size() > test.size()) return _strnicoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
				return false;
			}
				break;
			case StringComparison::InvariantCulture:
			{
				if (input.size() > test.size()) return _strncoll(input.c_str(), test.c_str(), test.size()) == 0;
				return false;
			}
				break;
			case StringComparison::InvariantCultureIgnoreCase:
			{
				if (input.size() > test.size()) return _strnicoll(input.c_str(), test.c_str(), test.size()) == 0;
				return false;
			}
				break;
			}
			return false;
		}

		/// <summary>
		/// 尾端字符串匹配
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <param name="comparsion">字符串比较枚举</param>
		/// <returns>是否匹配</returns>
		template<>
		__inline static bool EndsWith<wchar_t>(const std::basic_string<wchar_t>& input, const std::basic_string<wchar_t>& test, StringComparison comparsion)
		{
			switch (comparsion)
			{
				case StringComparison::CurrentCulture:
				{
					CultureInfo loc = CultureInfo::Current();
					if (input.size() > test.size()) return _wcsncoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
					return false;
				}
					break;
				case StringComparison::CurrentCultureIgnoreCase:
				{
					CultureInfo loc = CultureInfo::Current();
					if (input.size() > test.size()) return _wcsnicoll_l(input.c_str(), test.c_str(), test.size(), loc.CPlusLocale) == 0;
					return false;
				}
					break;
				case StringComparison::InvariantCulture:
				{
					if (input.size() > test.size()) return _wcsncoll(input.c_str(), test.c_str(), test.size()) == 0;
					return false;
				}
					break;
				case StringComparison::InvariantCultureIgnoreCase:
				{
					if (input.size() > test.size()) return _wcsnicoll(input.c_str(), test.c_str(), test.size()) == 0;
					return false;
				}
					break;
			}
			return false;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, const std::basic_string<T>& test);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <returns>比较结果</returns>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, const std::basic_string<char>& test)
		{
			if (!input.empty())
			{
				if (!test.empty())
					return strcoll(input.c_str(), test.c_str());
				else
					return 1;
			}
			else
			{
				if (test.empty()) return 0;
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <returns>比较结果</returns>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, const std::basic_string<wchar_t>& test)
		{
			if (!input.empty())
			{
				if (!test.empty())
					return wcscoll(input.c_str(), test.c_str()) == 0; 
				else
					return 1;
			}
			else
			{
				if (test.empty()) return 0;
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, const std::basic_string<T>& test, StringComparison comparison);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <param name="comparison">字符串比较枚举</param>
		/// <returns>比较结果</returns>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, const std::basic_string<char>& test, StringComparison comparison)
		{
			switch (comparison)
			{
				case StringComparison::CurrentCulture:
				{
					CultureInfo loc = CultureInfo::Current();
					if (!input.empty())
					{
						if (!test.empty())
							return _strcoll_l(input.c_str(), test.c_str(), loc.CPlusLocale);
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
				break;
				case StringComparison::CurrentCultureIgnoreCase:
				{
					CultureInfo loc = CultureInfo::Current();
					if (!input.empty())
					{
						if (!test.empty())
							return _stricoll_l(input.c_str(), test.c_str(), loc.CPlusLocale);						
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
				break;
				case StringComparison::InvariantCulture:
				{
					if (!input.empty())
					{
						if (!test.empty())
							return strcoll(input.c_str(), test.c_str());						
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
				break;
				case StringComparison::InvariantCultureIgnoreCase:
				{
					if (!input.empty())
					{
						if (!test.empty())
							return _stricoll(input.c_str(), test.c_str());						
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
				break;
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		/// <param name="input">输入值</param>
		/// <param name="test">匹配值</param>
		/// <param name="comparison">字符串比较枚举</param>
		/// <returns>比较结果</returns>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, const std::basic_string<wchar_t>& test, StringComparison comparison)
		{
			switch (comparison)
			{
				case StringComparison::CurrentCulture:
				{
					CultureInfo loc = CultureInfo::Current();
					if (!input.empty())
					{
						if (!test.empty())
							return _wcscoll_l(input.c_str(), test.c_str(), loc.CPlusLocale);
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
				break;			
				case StringComparison::CurrentCultureIgnoreCase:
				{
					CultureInfo loc = CultureInfo::Current();
					if (!input.empty())
					{
						if (!test.empty())
							return _wcsicoll_l(input.c_str(), test.c_str(), loc.CPlusLocale);
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
					break;
				case StringComparison::InvariantCulture:
				{
					if (!input.empty())
					{
						if (!test.empty())
							return wcscoll(input.c_str(), test.c_str());
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
					break;
				case StringComparison::InvariantCultureIgnoreCase:
				{
					if (!input.empty())
					{
						if (!test.empty())
							return _wcsicoll(input.c_str(), test.c_str());
						else
							return 1;
					}
					else
					{
						if (test.empty()) return 0;
					}
					return -1;
				}
					break;
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount, const std::basic_string<T>& test, typename std::basic_string<T>::size_type testStart, typename std::basic_string<T>::size_type testCount, StringComparison comparison);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="comparison">字符串比较枚举</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, typename std::basic_string<char>::size_type inputStart, typename std::basic_string<char>::size_type inputCount, const std::basic_string<char>& test, typename std::basic_string<char>::size_type testStart, typename std::basic_string<char>::size_type testCount, StringComparison comparison)
		{
			switch (comparison)
			{
			case StringComparison::CurrentCulture:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _strcoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::CurrentCultureIgnoreCase:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _stricoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCulture:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return strcoll(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCultureIgnoreCase:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return _stricoll(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="comparison">字符串比较枚举</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, std::basic_string<wchar_t>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<wchar_t>& test, std::basic_string<wchar_t>::size_type testStart, std::basic_string<wchar_t>::size_type testCount, StringComparison comparison)
		{
			switch (comparison)
			{
			case StringComparison::CurrentCulture:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _wcscoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::CurrentCultureIgnoreCase:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _wcsicoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCulture:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return wcscoll(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCultureIgnoreCase:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return _wcsicoll(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, typename std::basic_string<T>::size_type inputStart, const std::basic_string<T>& test, typename std::basic_string<T>::size_type testStart, typename std::basic_string<T>::size_type testCount, StringComparison comparison);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="comparison">字符串比较枚举</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, typename std::basic_string<char>::size_type inputStart, const std::basic_string<char>& test, typename std::basic_string<char>::size_type testStart, typename std::basic_string<char>::size_type testCount, StringComparison comparison)
		{
			switch (comparison)
			{
			case StringComparison::CurrentCulture:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _strcoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::CurrentCultureIgnoreCase:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _stricoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCulture:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return strcoll(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCultureIgnoreCase:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return _stricoll(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="comparison">字符串比较枚举</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, std::basic_string<wchar_t>::size_type inputStart, const std::basic_string<wchar_t>& test, std::basic_string<wchar_t>::size_type testStart, std::basic_string<wchar_t>::size_type testCount, StringComparison comparison)
		{
			switch (comparison)
			{
			case StringComparison::CurrentCulture:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _wcscoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::CurrentCultureIgnoreCase:
			{
				CultureInfo loc = CultureInfo::Current();
				if (!input.empty())
				{
					if (!test.empty())
						return _wcsicoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCulture:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return wcscoll(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			case StringComparison::InvariantCultureIgnoreCase:
			{
				if (!input.empty())
				{
					if (!test.empty())
						return _wcsicoll(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str());
					else
						return 1;
				}
				else
				{
					if (test.empty()) return 0;
				}
				return -1;
			}
				break;
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, typename std::basic_string<T>::size_type inputStart, const std::basic_string<T>& test, typename std::basic_string<T>::size_type testStart, typename std::basic_string<T>::size_type testCount, const CultureInfo& loc, bool ignoreCase = true);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, typename std::basic_string<char>::size_type inputStart, const std::basic_string<char>& test, typename std::basic_string<char>::size_type testStart, typename std::basic_string<char>::size_type testCount, const CultureInfo& loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _strcoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
					else
					{
						return _stricoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
				}
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, std::basic_string<wchar_t>::size_type inputStart, const std::basic_string<wchar_t>& test, std::basic_string<wchar_t>::size_type testStart, std::basic_string<wchar_t>::size_type testCount, const CultureInfo& loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _wcscoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
					else
					{
						return _wcsicoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
				}
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, typename std::basic_string<T>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<T>& test, typename std::basic_string<T>::size_type testStart, typename std::basic_string<T>::size_type testCount, CultureInfo& loc, bool ignoreCase = true);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息 (C++语言）</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, typename std::basic_string<char>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<char>& test, typename std::basic_string<char>::size_type testStart, typename std::basic_string<char>::size_type testCount, CultureInfo& loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _strcoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
					else
					{
						return _stricoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
				}
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息 (C++语言）</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, std::basic_string<wchar_t>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<wchar_t>& test, std::basic_string<wchar_t>::size_type testStart, std::basic_string<wchar_t>::size_type testCount, CultureInfo& loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _wcscoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
					else
					{
						return _wcsicoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc.CPlusLocale);
					}
				}
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, typename std::basic_string<T>::size_type inputStart, const std::basic_string<T>& test, typename std::basic_string<T>::size_type testStart, typename std::basic_string<T>::size_type testCount, _locale_t loc, bool ignoreCase = true);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息 (C++语言）</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, typename std::basic_string<char>::size_type inputStart, const std::basic_string<char>& test, typename std::basic_string<char>::size_type testStart, typename std::basic_string<char>::size_type testCount, _locale_t loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _strcoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
					else
					{
						return _stricoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
				}
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息 (C++语言）</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, std::basic_string<wchar_t>::size_type inputStart, const std::basic_string<wchar_t>& test, std::basic_string<wchar_t>::size_type testStart, std::basic_string<wchar_t>::size_type testCount, _locale_t loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _wcscoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
					else
					{
						return _wcsicoll_l(input.substr(inputStart, testCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
				}
			}
			return -1;
		}

		template<typename T>
		__inline static int Compare(const std::basic_string<T>& input, typename std::basic_string<T>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<T>& test, typename std::basic_string<T>::size_type testStart, typename std::basic_string<T>::size_type testCount, _locale_t loc, bool ignoreCase = true);

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息 (C++语言）</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<char>& input, typename std::basic_string<char>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<char>& test, typename std::basic_string<char>::size_type testStart, typename std::basic_string<char>::size_type testCount, _locale_t loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _strcoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
					else
					{
						return _stricoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
				}
			}
			return -1;
		}

		/// <summary>
		/// 比较字符串
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <param name="test">匹配值</param>
		/// <param name="testStart">匹配值起始位置</param>
		/// <param name="testCount">匹配值从起始位置后的取值长度</param>
		/// <param name="loc">区域性信息 (C++语言）</param>
		/// <param name="ignoreCase">指定是否忽略大小写</param>
		/// <returns>比较结果</returns>
		/// <remarks>返回值: 0表示相同; 1表示大于; -1表示小于</remarks>
		template<>
		__inline static int Compare(const std::basic_string<wchar_t>& input, std::basic_string<wchar_t>::size_type inputStart, std::basic_string<wchar_t>::size_type inputCount, const std::basic_string<wchar_t>& test, std::basic_string<wchar_t>::size_type testStart, std::basic_string<wchar_t>::size_type testCount, _locale_t loc, bool ignoreCase)
		{
			if (!input.empty())
			{
				if (!test.empty())
				{
					if (!ignoreCase)
					{
						return _wcscoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
					else
					{
						return _wcsicoll_l(input.substr(inputStart, inputCount).c_str(), test.substr(testStart, testCount).c_str(), loc);
					}
				}
			}
			return -1;
		}



		template<typename T>
		struct Internal_ToLower
		{
			Internal_ToLower(const std::locale& Loc) : m_Loc(&Loc) {}

			T operator ()(T Ch) const
			{
				return std::tolower<T>(Ch, *m_Loc);
			}
		private:
			const std::locale* m_Loc;
		};

		template<typename T>
		struct Internal_ToUpper
		{
			Internal_ToUpper(const std::locale& Loc) : m_Loc(&Loc) {}

			T operator ()(T Ch) const
			{
				return std::toupper<T>(Ch, *m_Loc);
			}
		private:
			const std::locale* m_Loc;
		};

		template<typename T>
		struct Internal_ToLowerNoLoc
		{
			Internal_ToLowerNoLoc() {}

			T operator ()(T Ch) const
			{
				return ::tolower(Ch);
			}
		};

		template<typename T>
		struct Internal_ToUpperNoLoc
		{
			Internal_ToUpperNoLoc() {}

			T operator ()(T Ch) const
			{
				return ::toupper(Ch);
			}
		};



		/// <summary>
		/// 转换为大写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T>& ToUpper(std::basic_string<T>& input)
		{
			for (auto & c : input) c = std::toupper(c);
			return input;
		}

		/// <summary>
		/// 转换为大写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T>& ToUpper(std::basic_string<T>& input, const CultureInfo& loc)
		{
			const std::locale _loc = loc.CLocale;
			for (auto & c : input) c = std::toupper(c, _loc);
			return input;
		}

		/// <summary>
		/// 转换为大写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T> ToUpper(const std::basic_string<T>& input)
		{
			std::basic_string<T> result;
			std::transform(input.begin(), input.end(), std::back_inserter(result), Internal_ToUpperNoLoc<T>());
			return result;
		}

		/// <summary>
		/// 转换为大写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T> ToUpper(const std::basic_string<T>& input, const CultureInfo& loc)
		{
			const std::locale _loc = loc.CLocale;
			std::basic_string<T> result;
			std::transform(input.begin(), input.end(), std::back_inserter(result), Internal_ToUpper<T>(_loc));
			return result;
		}

		/// <summary>
		/// 转换为小写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T>& ToLower(std::basic_string<T>& input)
		{
			for (auto & c : input) c = std::tolower(c, CultureInfo::Current().CLocale);
			return input;
		}

		/// <summary>
		/// 转换为小写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T>& ToLower(std::basic_string<T>& input, const CultureInfo& loc)
		{
			const std::locale _loc = loc.CLocale;
			for (auto & c : input) c = std::tolower(c, _loc);
			return input;
		}

		/// <summary>
		/// 转换为小写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T> ToLower(const std::basic_string<T>& input)
		{
			std::basic_string<T> result;
			std::transform(input.begin(), input.end(), std::back_inserter(result), Internal_ToLowerNoLoc<T>());
			return result;
		}

		/// <summary>
		/// 转换为小写
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		template <class T>
		__inline static std::basic_string<T> ToLower(const std::basic_string<T>& input, const CultureInfo& loc)
		{
			const std::locale _loc = loc.CLocale;
			std::basic_string<T> result;
			std::transform(input.begin(), input.end(), std::back_inserter(result), Internal_ToLower<T>(_loc));
			return result;
		}

		/// <summary>
		/// 查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexOf(const std::basic_string<T>& input, T match)
		{
			return input.find_first_of(match);
		}

		/// <summary>
		/// 查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_first_of(match, inputStart);
		}

		/// <summary>
		/// 查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_first_of(match, inputStart, inputCount);
		}

		/// <summary>
		/// 查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexNotOf(const std::basic_string<T>& input, T match)
		{
			return input.find_first_not_of(match);
		}

		/// <summary>
		/// 查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexNotOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_first_not_of(match, inputStart);
		}

		/// <summary>
		/// 查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexNotOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_first_not_of(match, inputStart, inputCount);
		}


		/// <summary>
		/// 从末尾查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexOf(const std::basic_string<T>& input, T match)
		{
			return input.find_last_of(match);
		}

		/// <summary>
		/// 从末尾查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_last_of(match, inputStart);
		}

		/// <summary>
		/// 从末尾查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_last_of(match, inputStart, inputCount);
		}


		/// <summary>
		/// 从末尾查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexNotOf(const std::basic_string<T>& input, T match)
		{
			return input.find_last_not_of(match);
		}

		/// <summary>
		/// 从末尾查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexNotOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_last_not_of(match, inputStart);
		}

		/// <summary>
		/// 从末尾查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexNotOf(const std::basic_string<T>& input, T match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_last_not_of(match, inputStart, inputCount);
		}








		/// <summary>
		/// 查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match)
		{
			return input.find_first_of(match);
		}

		/// <summary>
		/// 查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_first_of(match, inputStart);
		}

		/// <summary>
		/// 查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_first_of(match, inputStart, inputCount);
		}

		/// <summary>
		/// 查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexNotOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match)
		{
			return input.find_first_not_of(match);
		}

		/// <summary>
		/// 查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexNotOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_first_not_of(match, inputStart);
		}

		/// <summary>
		/// 查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type IndexNotOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_first_not_of(match, inputStart, inputCount);
		}


		/// <summary>
		/// 从末尾查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match)
		{
			return input.find_last_of(match);
		}

		/// <summary>
		/// 从末尾查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_last_of(match, inputStart);
		}

		/// <summary>
		/// 从末尾查找第一个匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_last_of(match, inputStart, inputCount);
		}


		/// <summary>
		/// 从末尾查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexNotOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match)
		{
			return input.find_last_not_of(match);
		}

		/// <summary>
		/// 从末尾查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexNotOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart)
		{
			return input.find_last_not_of(match, inputStart);
		}

		/// <summary>
		/// 从末尾查找第一个不匹配字符位置索引
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="match">匹配字符</param>
		/// <param name="inputStart">输入值起始位置</param>
		/// <param name="inputCount">输入值从起始位置后的取值长度</param>
		/// <returns></returns>
		template<class T>
		__inline static typename std::basic_string<T>::size_type LastIndexNotOfAny(const std::basic_string<T>& input, const std::basic_string<T>& match, typename std::basic_string<T>::size_type inputStart, typename std::basic_string<T>::size_type inputCount)
		{
			return input.find_last_not_of(match, inputStart, inputCount);
		}
	};
}