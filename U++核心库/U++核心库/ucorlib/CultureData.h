#pragma once
#include "Common.h"
#include "CalendarData.h"
#include <unordered_set>
#include "StringHelper.h"

namespace System
{
	class CultureInfo_impl;

	ENUM_OPERATORS(FoundDatePattern)
	enum class FoundDatePattern
	{
		None = 0,
		FoundYearPatternFlag = 1,
		FoundMonthPatternFlag = 2,
		FoundDayPatternFlag = 4,
		FoundYMDPatternFlag = 7
	};

	class CultureData: _noncopyable
	{
	public:
#pragma region »õ±Ò
		char frac_digits;
		char p_cs_precedes;
		char p_sep_by_space;
		char n_cs_precedes;
		char n_sep_by_space;
		char p_sign_posn;
		char n_sign_posn;
		wchar_t *_W_decimal_point;
		wchar_t *_W_thousands_sep;
		wchar_t *_W_currency_symbol;
		wchar_t *_W_mon_decimal_point;
		wchar_t *_W_mon_thousands_sep;
		wchar_t *_W_positive_sign;
		wchar_t *_W_negative_sign;
#pragma endregion

		std::wstring FullTimeSpanPositivePattern;
		std::wstring FullTimeSpanNegativePattern;

		wstringvector YearMonthPattern;
		wstringvector LongDatePattern;
		wstringvector ShortDatePattern;
		wstringvector LongTimePattern;
		wstringvector ShortTimePattern;
		std::wstring MonthDayPattern;

		std::wstring langName;
		std::wstring culName;

		std::vector<CalendarData*> cals;
		CultureInfo_impl* impl;

		std::wstring amDesignator;
		std::wstring pmDesignator;
		std::wstring dateSeparator;
		std::wstring timeSeparator;

		std::wstring FullLongDateShortTimePattern;
		std::wstring 	FullLongDateLongTimePattern;
		std::wstring 	GeneralShortDateShortTimePattern;
		std::wstring 	GeneralShortDateLongTimePattern;
		std::wstring DateTimeOffsetPattern;

		bool _firstDayofWeekIsMonday;
		bool _hasForceTwoDigitYears;
		bool _useGenitiveMonth;

		bool HasSpacesInMonthNames;
		bool HasSpacesInDayNames;

		int formatFlags;

		static const std::unordered_set<std::wstring> DateWords;

		static std::wstring ReescapeWin32wstring(const std::wstring& str)
		{
			int i;
			if (str.empty())
			{
				return std::wstring();
			}
			std::wstring wstringBuilder;
			bool flag = false;
			for (i = 0; i < (int)str.length(); i++)
			{
			Label0:
				if (str[i] != '\'')
				{
					if (str[i] != '\\')
					{
						goto Label1;
					}
					if (wstringBuilder.empty())
					{
						wstringBuilder = str.substr(0, i);
						wstringBuilder.reserve(str.length() * 2);
					}
					wstringBuilder.append(L"\\\\");
					goto Label0;
				}
				else if (!flag)
				{
					flag = true;
				}
				else
				{
					if (i + 1 <  (int)str.length() && str[i + 1] == '\'')
					{
						goto Label2;
					}
					flag = false;
				}
			Label1:
				if (!wstringBuilder.empty())
				{
					wstringBuilder.push_back(str[i]);
				}
			}
			if (wstringBuilder.empty())
			{
				return str;
			}
			return wstringBuilder;
		Label2:
			if (wstringBuilder.empty())
			{
				wstringBuilder = str.substr(0, i);
				wstringBuilder.reserve(str.length() * 2);
			}
			wstringBuilder.append(L"\\'");
			i++;
			goto Label0;
		}

		static wstringvector ReescapeWin32wstrings(wstringvector& array)
		{
			if (array.size())
			{
				for (int i = 0; i < (int)array.size(); i++)
				{
					array[i] = ReescapeWin32wstring(array[i]);
				}
			}
			return array;
		}

		const char GetMoneyConst(CultureInfoMoneyConsts moneyconst) const;

		const wchar_t* GetPattern(CultureInfoPatterns pattern, unsigned int index=0) const;

		unsigned int GetPatternCount(CultureInfoPatterns pattern) const;

		unsigned int GetConstsNameCount(CultureInfoConstNames name) const;

		const wchar_t* GetConstsName(CultureInfoConstNames name, unsigned int index=0) const;

		bool GetBooleanFlags(CultureInfoBooleanFlags flag) const;

protected:
		static CultureData* InVariant;

		CultureData();
		CultureData(CultureInfo_impl* loc);
		CultureData(CultureInfo_impl* loc, int id);
public:
		~CultureData();

		static CultureData* Create(CultureInfo_impl* loc);

		static CultureData* Create(CultureInfo_impl* loc, int id);

		static wstringvector GetMergedPatterns(const wstringvector& patterns, const std::wstring& defaultPattern)
		{
			wstringvector strArrays;
			if (defaultPattern == patterns[0])
			{
				return patterns;
			}
			int num = 0;
			while (num <  (int)patterns.size() && !(defaultPattern == patterns[num]))
			{
				num++;
			}
			if (num >= (int)patterns.size())
			{
				strArrays = wstringvector(patterns.size() + 1);

				std::copy(patterns.begin(), patterns.end(), strArrays.begin() + 1);

			}
			else
			{
				strArrays = patterns;
				strArrays[num] = strArrays[0];
			}
			strArrays[0] = defaultPattern;
			return strArrays;
		}
		static wstringvector GetCombinedPatterns(const wstringvector& patterns1, const wstringvector& patterns2, const std::wstring& connectString)
		{
			wstringvector strArrays = wstringvector(patterns1.size() * patterns2.size());
			int num = 0;
			for (int i = 0; i < (int)patterns1.size(); i++)
			{
				for (int j = 0; j < (int)patterns2.size(); j++)
				{
					int num1 = num;
					num = num1 + 1;
					strArrays[num1] = patterns1[i] + connectString + patterns2[j];
				}
			}
			return strArrays;
		}

		wstringvector GetAllDateTimePatterns(wchar_t format)
		{
			wstringvector strArrays;
			wstringvector strArrays1;
			wstringvector allShortDatePatterns;
			wchar_t chr = format;
			if (chr > 'U')
			{
				if (chr != 'Y')
				{
					switch (chr)
					{
						case 'd':
						{
							return ShortDatePattern;
						}
						case 'e':
						{
							throw std::exception("Format_BadFormatSpecifier");
						}
						case 'f':
						{
							return GetCombinedPatterns(LongDatePattern, ShortTimePattern, L" ");
						}
						case 'g':
						{
							return GetCombinedPatterns(ShortDatePattern, ShortTimePattern, L" ");
						}
						default:
						{
						switch (chr)
						{
						case 'm':
						{
							return wstringvector{ MonthDayPattern };
						}
						case 'n':
						case 'p':
						case 'q':
						case 'v':
						case 'w':
						case 'x':
						{
							throw std::exception("Format_BadFormatSpecifier");
						}
						case 'o':
						{
							return wstringvector{ L"yyyy'-'MM'-'dd'T'HH':'mm':'ss.fffffffK" };
						}
						case 'r':
						{
							return wstringvector{ L"ddd, dd MMM yyyy HH':'mm':'ss 'GMT'" };
						}
						case 's':
						{
							return wstringvector{ L"yyyy'-'MM'-'dd'T'HH':'mm':'ss" };
						}
						case 't':
						{
							return ShortTimePattern;
						}
						case 'u':
						{
							return wstringvector{ L"yyyy'-'MM'-'dd HH':'mm':'ss'Z'" };
						}
						case 'y':
						{
							break;
						}
						default:
						{
							throw std::exception("Format_BadFormatSpecifier");
						}
					}
					break;
					}
				}
				}
				allShortDatePatterns = YearMonthPattern;
			}
			else
			{
				switch (chr)
				{
					case 'D':
					{
						allShortDatePatterns = LongDatePattern;
					break;
					}
					case 'E':
					{
						throw std::exception("Format_BadFormatSpecifier");
					}
					case 'F':
					{
						Label5:
						allShortDatePatterns = GetCombinedPatterns(LongDatePattern, LongTimePattern, L" ");
						break;
					}
					case 'G':
					{
						allShortDatePatterns = GetCombinedPatterns(ShortDatePattern, LongTimePattern, L" ");
						break;
					}
					default:
					{
						switch (chr)
						{
							case 'M':
							{
								return wstringvector{ MonthDayPattern };
							}
							case 'N':
							case 'P':
							case 'Q':
							case 'S':
							{
								throw std::exception("Format_BadFormatSpecifier");
							}
							case 'O':
							{
								return wstringvector{ L"yyyy'-'MM'-'dd'T'HH':'mm':'ss.fffffffK" };
							}
							case 'R':
							{
								return  wstringvector{ L"ddd, dd MMM yyyy HH':'mm':'ss 'GMT'" };
							}
							case 'T':
							{
								allShortDatePatterns = LongTimePattern;
							break;
							}
							case 'U':
							{
								goto Label5;
							}
							default:
							{
								throw std::exception("Format_BadFormatSpecifier");
							}
						}
						break;
					}
				}
			}
			return allShortDatePatterns;
		}

		FoundDatePattern m_ymdFlags;

		static int ScanRepeatChar(const std::wstring& pattern, wchar_t ch, int index, int& count)
		{
			count = 1;
			while (true)
			{
				int num = index + 1;
				index = num;
				if (num >= (int)pattern.length() || pattern[index] != ch)
				{
					break;
				}
				count = count + 1;
			}
			return index;
		}

		static int SkipWhiteSpacesAndNonLetter(const std::wstring& pattern, int currentIndex)
		{
			while (currentIndex <  (int)pattern.length())
			{
				wchar_t chr = pattern[currentIndex];
				if (chr == '\\')
				{
					currentIndex++;
					if (currentIndex >= (int)pattern.length())
					{
						break;
					}
					chr = pattern[currentIndex];
					if (chr == '\'')
					{
						continue;
					}
				}
				if (iswalpha(chr) || chr == '\'' || chr == '.')
				{
					break;
				}
				currentIndex++;
			}
			return currentIndex;
		}

		void AddDateWordOrPostfix(const std::wstring& formatPostfix, const std::wstring& str, std::unordered_set<std::wstring>& dateWords)
		{
			std::wstring str1;
			if (str.length() > 0)
			{
				if (StringHelper::Compare(str, std::wstring(L"."))==0) 
				{
					AddIgnorableSymbols(L".", dateWords);
					return;
				}
				if (DateWords.find(str) != DateWords.end()) return;

				if (StringHelper::Compare(formatPostfix, std::wstring(L"MMMM"))!=0)
				{
					dateWords.insert(str);
					if (str[str.length() - 1] == '.')
					{
						std::wstring str2 = str.substr(0, str.length() - 1);
						dateWords.insert(str2);
					}
				}
				else
				{
					std::wstring str3;
					str3.push_back(L'\xE000');
					str3 += str;

					dateWords.insert(str3);
				}
			}
		}

		int AddDateWords(const std::wstring& pattern, int index, std::wstring formatPostfix, std::unordered_set<std::wstring>& dateWords)
		{
			int num = SkipWhiteSpacesAndNonLetter(pattern, index);
			if (num != index && !formatPostfix.empty())
			{
				formatPostfix = L"";
			}
			index = num;
			std::wstring stringBuilder;
			while (index <  (int)pattern.length())
			{
				wchar_t chr = pattern[index];
				if (chr == '\'')
				{
					AddDateWordOrPostfix(formatPostfix, stringBuilder, dateWords);
					index++;
					break;
				}
				else if (chr == '\\')
				{
					index++;
					if (index >= (int)pattern.length())
					{
						continue;
					}
					stringBuilder.push_back(pattern[index]);
					index++;
				}
				else if (!iswspace(chr))
				{
					stringBuilder.push_back(chr);
					index++;
				}
				else
				{
					AddDateWordOrPostfix(formatPostfix, stringBuilder, dateWords);
					if (!formatPostfix.empty())
					{
						formatPostfix = L"";
					}
					stringBuilder.clear();
					index++;
				}
			}
			return index;
		}

		void AddIgnorableSymbols(const std::wstring& text, std::unordered_set<std::wstring>& dateWords)
		{
			std::wstring str;
			str.push_back(L'\xE001');
			str += text;

			dateWords.insert(str);
		}

		void ScanDateWord(const std::wstring& pattern, std::unordered_set<std::wstring>& dateWords)
		{
			int num;
			m_ymdFlags = FoundDatePattern::None;
			int num1 = 0;
			while (num1 <  (int)pattern.length())
			{
				wchar_t chr = pattern[num1];
				wchar_t chr1 = chr;
				if (chr1 <= 'M')
				{
					if (chr1 == '\'')
					{
						num1 = AddDateWords(pattern, num1 + 1, std::wstring(), dateWords);
						continue;
					}
					else if (chr1 == '.')
					{
						if (m_ymdFlags == FoundDatePattern::FoundYMDPatternFlag)
						{
							AddIgnorableSymbols(L".", dateWords);
							m_ymdFlags = FoundDatePattern::None;
						}
						num1++;
						continue;
					}
					else if (chr1 == 'M')
					{
						num1 = ScanRepeatChar(pattern, 'M', num1, num);
						if (num >= 4 && num1 <  (int)pattern.length() && pattern[num1] == '\'')
						{
							num1 = AddDateWords(pattern, num1 + 1, L"MMMM", dateWords);
						}
						m_ymdFlags = m_ymdFlags | FoundDatePattern::FoundMonthPatternFlag;
						continue;
					}
				}
				else if (chr1 == '\\')
				{
					num1 = num1 + 2;
					continue;
				}
				else if (chr1 == 'd')
				{
					num1 = ScanRepeatChar(pattern, 'd', num1, num);
					if (num > 2)
					{
						continue;
					}
					m_ymdFlags = m_ymdFlags | FoundDatePattern::FoundDayPatternFlag;
					continue;
				}
				else if (chr1 == 'y')
				{
					num1 = ScanRepeatChar(pattern, 'y', num1, num);
					m_ymdFlags = m_ymdFlags | FoundDatePattern::FoundYearPatternFlag;
					continue;
				}
				if (m_ymdFlags == FoundDatePattern::FoundYMDPatternFlag && ! iswspace(chr))
				{
					m_ymdFlags = FoundDatePattern::None;
				}
				num1++;
			}
		}

		wstringvector GetDateWords()
		{
			int i;
			std::unordered_set<std::wstring> item;
			wstringvector res;
			wstringvector allDateTimePatterns = GetAllDateTimePatterns('D');
			for (i = 0; i < (int)allDateTimePatterns.size(); i++)
			{
				ScanDateWord(allDateTimePatterns[i], item);
			}
			allDateTimePatterns = GetAllDateTimePatterns('d');
			for (i = 0; i < (int)allDateTimePatterns.size(); i++)
			{
				ScanDateWord(allDateTimePatterns[i], item);
			}
			allDateTimePatterns = GetAllDateTimePatterns('y');
			for (i = 0; i < (int)allDateTimePatterns.size(); i++)
			{
				ScanDateWord(allDateTimePatterns[i], item);
			}
			ScanDateWord(MonthDayPattern, item);
			allDateTimePatterns = GetAllDateTimePatterns('T');
			for (i = 0; i < (int)allDateTimePatterns.size(); i++)
			{
				ScanDateWord(allDateTimePatterns[i], item);
			}
			allDateTimePatterns = GetAllDateTimePatterns('t');
			for (i = 0; i < (int)allDateTimePatterns.size(); i++)
			{
				ScanDateWord(allDateTimePatterns[i], item);
			}
			
			res.resize(item.size());

			std::copy(item.begin(), item.end(), res.begin());

			return res;
		}
	};
}