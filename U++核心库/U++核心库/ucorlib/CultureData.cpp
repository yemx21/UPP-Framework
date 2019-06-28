#include "CultureData.h"
#include "CultureInfo_impl.h"
#include "CalendarData.h"
#if _MSC_VER>1800
#include <corecrt.h>
#endif

#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;


const std::unordered_set<std::wstring> CultureData::DateWords = { std::wstring(L"/"), std::wstring(L"-"), std::wstring(L"."), std::wstring(L"年"), std::wstring(L"月"), std::wstring(L"日"), std::wstring(L"년"), std::wstring(L"월"), std::wstring(L"일"), std::wstring(L"시"), std::wstring(L"분"), std::wstring(L"초"), std::wstring(L"時"), std::wstring(L"时"), std::wstring(L"分"), std::wstring(L"秒") };
CultureData* CultureData::InVariant = new CultureData();

API_BOOL API_STDCALL Custom_EnumCalendarInfoProc(wchar_t* lpCalendarInfoString, API_UINT32 Calendar, wchar_t* lpReserved, API_INT64 lParam)
{
	if (!lpCalendarInfoString) return API_FALSE;
	if (!lParam) return API_FALSE;
	CultureData* data = (CultureData*)lParam;

	CalendarData* cal = CalendarData::Create(data->impl!=nullptr ?  data->impl->_locale.name() : "ivl", Calendar, false);
	
	if (cal)
	{
		data->cals.push_back(cal);
	}

	return API_TRUE;
}

API_BOOL API_STDCALL Count_EnumCalendarInfoProc(wchar_t* lpCalendarInfoString, API_UINT32 Calendar, wchar_t* lpReserved, API_INT64 lParam)
{
	if (!lpCalendarInfoString) return API_FALSE;
	if (!lParam) return API_FALSE;

	std::vector<unsigned long>* count = (std::vector<unsigned  long>*)lParam;

	count->push_back(Calendar);

	return API_TRUE;
}

__inline bool _getinfo(API_UINT32 lcid, API_UINT32 ctype, std::wstring& res)
{
	int len = Microsoft::winapi_GetLocaleInfo(lcid, ctype, NULL, 0);
	if (len <= 0) return false;
	res.resize(len);
	Microsoft::winapi_GetLocaleInfo(lcid, ctype, &res.at(0), len);
	return true;
}

bool ArrayElementsBeginWithDigit(const wstringvector& arr)
{
	for (int i = 0; i<(int)arr.size(); i++)
	{
		// it is faster to check for space character manually instead of calling IndexOf
		// so we don't have to go to native code side.
		if ((int)arr[i].length() > 0 && arr[i][0] >= '0' && arr[i][0] <= '9')
		{
			int index = 1;
			while (index < (int)arr[i].length() && arr[i][index] >= '0' && arr[i][index] <= '9')
			{
				// Skip other digits.
				index++;
			}
			if (index == (int)arr[i].length())
			{
				return (false);
			}

			if (index == (int)arr[i].length() - 1)
			{
				// Skip known CJK month suffix.
				// CJK uses month name like "1\x6708", since \x6708 is a known month suffix,
				// we don't need the UseDigitPrefixInTokens since it is slower.

				switch (arr[i][index])
				{
				case L'\x6708': // CJKMonthSuff
				case L'\xc6d4': // KoreanMonthSuff
					return (false);
				}
			}

			if (index == arr[i].length() - 4)
			{
				// Skip known CJK month suffix.
				// Starting with Windows 8, the CJK months for some cultures looks like: "1' \x6708'" 
				// instead of just "1\x6708"
				if (arr[i][index] == '\'' && arr[i][index + 1] == ' ' &&
					arr[i][index + 2] == L'\x6708' && arr[i][index + 3] == '\'')
				{
					return (false);
				}

			}
			return (true);
		}
	}

	return false;
}

bool ArrayElementsHaveSpace(const wstringvector& arr)
{
	for (int i = 0; i < (int)arr.size(); i++)
	{
		for (int j = 0; j < (int)arr[i].size(); j++)
		{
			if (iswspace(arr[i][j]))
			{
				return true;
			}
		}
	}
	return false;
}

bool EqualStringArrays(const wstringvector& array1, const wstringvector& array2)
{
	if (&array1 == &array2)
	{
		return true;
	}
	if (array1.size() != array2.size())
	{
		return false;
	}
	for (int i = 0; i < (int)array1.size(); i++)
	{
		if (StringHelper::Compare(array1[i], array2[i],StringComparison::InvariantCulture)!=0)
		{
			return false;
		}
	}
	return true;
}

DateTimeFormatFlags GetFormatFlagGenitiveMonth(const wstringvector& monthNames, const wstringvector& genitveMonthNames, const wstringvector& abbrevMonthNames, const wstringvector& genetiveAbbrevMonthNames)
{
	if (EqualStringArrays(monthNames, genitveMonthNames) && EqualStringArrays(abbrevMonthNames, genetiveAbbrevMonthNames))
	{
		return DateTimeFormatFlags::None;
	}
	return DateTimeFormatFlags::UseGenitiveMonth;
}

DateTimeFormatFlags GetFormatFlagUseHebrewCalendar(int calID)
{
	if (calID != 8)
	{
		return DateTimeFormatFlags::None;
	}
	return DateTimeFormatFlags::UseLeapYearMonth | DateTimeFormatFlags::UseHebrewRule;
}

DateTimeFormatFlags GetFormatFlagUseSpaceInDayNames(const wstringvector& dayNames, const wstringvector& abbrevDayNames)
{
	if (!ArrayElementsHaveSpace(dayNames) && !ArrayElementsHaveSpace(abbrevDayNames))
	{
		return DateTimeFormatFlags::None;
	}
	return DateTimeFormatFlags::UseSpacesInDayNames;
}

DateTimeFormatFlags GetFormatFlagUseSpaceInMonthNames(const wstringvector& monthNames, const wstringvector& genitveMonthNames, const wstringvector& abbrevMonthNames, const wstringvector& genetiveAbbrevMonthNames)
{
	return DateTimeFormatFlags::None | (ArrayElementsBeginWithDigit(monthNames) || ArrayElementsBeginWithDigit(genitveMonthNames) || ArrayElementsBeginWithDigit(abbrevMonthNames) || ArrayElementsBeginWithDigit(genetiveAbbrevMonthNames) ? DateTimeFormatFlags::UseDigitPrefixInTokens : DateTimeFormatFlags::None) | (ArrayElementsHaveSpace(monthNames) || ArrayElementsHaveSpace(genitveMonthNames) || ArrayElementsHaveSpace(abbrevMonthNames) || ArrayElementsHaveSpace(genetiveAbbrevMonthNames) ? DateTimeFormatFlags::UseSpacesInMonthNames : DateTimeFormatFlags::None);
}

__inline int CultureData_ObtainFlag_Field(char f)
{
	switch (f)
	{
	case std::moneypunct<char>::sign:
		return 1;
	case std::moneypunct<char>::symbol:
		return 2;
	case std::moneypunct<char>::value:
		return 4;
	case std::moneypunct<char>::space:
		return 8;
	case std::moneypunct<char>::none:
		return 16;
	}
	return 0;
}

__inline void CultureData_Obtain_CS_SEP_SIGN(char field[4], bool bsymbol, bool bsign, char& cs, char& sp, char& sign)
{
	int cdf = CultureData_ObtainFlag_Field(field[0]) * 16777216 + CultureData_ObtainFlag_Field(field[1]) * 4096 + CultureData_ObtainFlag_Field(field[2]) * 64 + CultureData_ObtainFlag_Field(field[3]);

	switch (cdf)
	{
	case 16793744:
		cs = 0;
		sp = 0;
		sign = bsign ? 1 : 0;
		break;
	case 67117136:
		cs = 0;
		sp = 0;
		sign = bsign ? 2 : 4;
		break;
	case 67113104:
		cs = 0;
		sp = 0;
		sign = 3;
		break;	
	case 16794114:
		cs = 0;
		sp = 1;
		sign = bsign ? 1 : 0;
		break;
	case 67141761:
		cs = 0;
		sp = 1;
		sign = bsign ? 2 : 4;
		break;
	case 67141698:
		cs = 0;
		sp = 1;
		sign = 3;
		break;
	case 16810242:
		cs = 0;
		sp = 2;
		sign = bsign ? 0 : 1;
		break;
	case 67117569:
		cs = 0;
		sp = 2;
		sign = bsign ? 2 : 4;
		break;
	case 67113474:
		cs = 0;
		sp = 2;
		sign = 3;
		break;
	case 16785680:
		cs = 1;
		sp = 0;
		sign = bsign ? 0 : bsymbol? 1 : 3;
		break;
	case 33570896:
		cs = 1;
		sp = 0;
		sign = 2;
		break;
	case 33558800:
		cs = 1;
		sp = 0;
		sign = 4;
		break;
	case 16785924:
		cs = 1;
		sp = 1;
		sign = bsign ? 0 : bsymbol ? 1 : 3;
		break;
	case 33587457:
		cs = 1;
		sp = 1;
		sign = 2;
		break;
	case 33559044:
		cs = 1;
		sp = 1;
		sign = 4;
		break;
	case 16810116:
		cs = 1;
		sp = 2;
		sign = bsign ? 0 : bsymbol ? 1 : 3;
		break;
	case 33571329:
		cs = 1;
		sp = 2;
		sign = 2;
		break;
	case 33587268:
		cs = 1;
		sp = 2;
		sign = 4;
		break;
	}

}

CultureData::CultureData(CultureInfo_impl* loc)
{
	impl = loc;

#pragma region 货币信息	
	const std::moneypunct<char>& mp = std::use_facet<std::moneypunct<char>>(loc->_locale);
	const std::moneypunct<wchar_t>& wmp = std::use_facet<std::moneypunct<wchar_t> >(loc->_locale);
	frac_digits = mp.frac_digits();

	CultureData_Obtain_CS_SEP_SIGN(mp.pos_format().field, !mp.curr_symbol().empty(), !mp.positive_sign().empty(), p_cs_precedes, p_sep_by_space, p_sign_posn);
	CultureData_Obtain_CS_SEP_SIGN(mp.neg_format().field, !mp.curr_symbol().empty(), !mp.negative_sign().empty(), n_cs_precedes, n_sep_by_space, n_sign_posn);

	_W_decimal_point = new wchar_t[1]{ wmp.decimal_point() };
	_W_thousands_sep = new wchar_t[1]{ wmp.thousands_sep() };
	_W_currency_symbol = Cloner::Clone(wmp.curr_symbol());
	_W_mon_decimal_point = new wchar_t[1]{ wmp.decimal_point() };
	_W_mon_thousands_sep = new wchar_t[1]{ wmp.thousands_sep() };
	_W_positive_sign = Cloner::Clone(wmp.positive_sign());
	_W_negative_sign = Cloner::Clone(wmp.negative_sign());
#pragma endregion

	FullTimeSpanPositivePattern = L"d':'h':'mm':'ss'";
	FullTimeSpanPositivePattern += _W_decimal_point;
	FullTimeSpanPositivePattern += L"'FFFFFFF";
	
	FullTimeSpanNegativePattern = L'-' + FullTimeSpanPositivePattern;

	if (!loc->_name.empty() && loc->_name.size() != 0)
	{
		Microsoft::winapi_EnumCalendarInfo(Custom_EnumCalendarInfoProc, loc->_name.c_str(), API_ENUM_ALL_CALENDARS, NULL, API_CAL_SCALNAME, this);

		if (cals.size() > 0)
		{
			auto& cal = cals[0];
			YearMonthPattern = cal->saYearMonths;
			LongDatePattern = cal->saLongDates;
			ShortDatePattern = cal->saShortDates;
			LongTimePattern = cal->saYearMonths;
			ShortTimePattern = cal->saShortTimes;
			LongTimePattern = cal->saLongTimes;
			MonthDayPattern = cal->sMonthDay;


			if (!_getinfo(loc->_lcid, API_LOCALE_SAM, amDesignator)) amDesignator = L"AM";
			if (!_getinfo(loc->_lcid, API_LOCALE_SPM, pmDesignator)) pmDesignator = L"PM";
			if (!_getinfo(loc->_lcid, API_LOCALE_SDATE, dateSeparator)) dateSeparator = L"/";
			if (!_getinfo(loc->_lcid, API_LOCALE_STIME, timeSeparator)) timeSeparator = L":";


			_getinfo(loc->_lcid, API_LOCALE_SISO639LANGNAME, langName);

			std::wstring sName;
			_getinfo(loc->_lcid, API_LOCALE_SNAME, sName);

			if (StringHelper::Compare(sName, std::wstring(L"zh-CHS")) == 0 || StringHelper::Compare(sName, std::wstring(L"zh-CHT")) == 0)
				culName = sName;
			else
				culName = loc->_name;

			std::wstring fdwm;
			_getinfo(loc->_lcid, API_LOCALE_IFIRSTDAYOFWEEK, fdwm);
			_firstDayofWeekIsMonday = fdwm.empty() ? true : _wcsicmp(fdwm.c_str(), L"0") == 0;

			bool ugm = false;
			int i = 0;
			for (; i < 12; i++)
			{
				ugm = wcscmp(cal->saMonthNames[i].c_str(), cal->saMonthGenitiveNames[i].c_str()) != 0;
				if (ugm) break;
			}
			if (ugm)
				_useGenitiveMonth = true;
			else
			{
				for (i = 0; i < 12; i++)
				{
					ugm = wcscmp(cal->saAbbrevMonthNames[i].c_str(), cal->saAbbrevMonthGenitiveNames[i].c_str()) != 0;
					if (ugm) break;
				}
				_useGenitiveMonth = ugm;
			}

		}
		else
		{
			auto& cal = CalendarData::GetInvariant();

			YearMonthPattern = wstringvector{ L"yyyy MMMM" };
			LongDatePattern = wstringvector{ L"dddd, dd MMMM yyyy" };
			ShortDatePattern = wstringvector{ L"MM/dd/yyyy", L"yyyy-MM-dd" };
			LongTimePattern = wstringvector{ L"HH:mm:ss" };
			ShortTimePattern = wstringvector{ L"HH:mm", L"hh:mm tt", L"H:mm", L"h:mm tt" };
			MonthDayPattern = L"MMMM dd";


			amDesignator = L"AM";
			pmDesignator = L"PM";
			dateSeparator = L"/";
			timeSeparator = L":";

			_firstDayofWeekIsMonday = true;
			_useGenitiveMonth = false;
		}
	}
	else
	{
		cals.push_back(&CalendarData::GetInvariant());
		auto& cal = CalendarData::GetInvariant();

		YearMonthPattern = cal.saYearMonths;
		LongDatePattern = cal.saLongDates;
		ShortDatePattern = cal.saShortDates;
		LongTimePattern = cal.saYearMonths;
		ShortTimePattern = cal.saShortTimes;
		LongTimePattern = cal.saLongTimes;

		amDesignator = L"AM";
		pmDesignator = L"PM";
		dateSeparator = L"/";
		timeSeparator = L":";

		_firstDayofWeekIsMonday = true;
		_useGenitiveMonth = false;
	}
	formatFlags = 0;

	if (cals.size() > 0)
	{
		auto& cal = cals[0];
		if (cal)
		{
			
			formatFlags = ((int)formatFlags | (int)GetFormatFlagGenitiveMonth(cal->saMonthNames, cal->saMonthGenitiveNames, cal->saAbbrevMonthNames, cal->saAbbrevMonthGenitiveNames));
			formatFlags = ((int)formatFlags | (int)GetFormatFlagUseSpaceInMonthNames(cal->saMonthNames, cal->saMonthGenitiveNames, cal->saAbbrevMonthNames, cal->saAbbrevMonthGenitiveNames));
			formatFlags = ((int)formatFlags | (int)GetFormatFlagUseSpaceInDayNames(cal->saDayNames, cal->saAbbrevDayNames));
			formatFlags = ((int)formatFlags | (int)GetFormatFlagUseHebrewCalendar(cal->CalId));
		}
	}

	if (ShortDatePattern.size() > 0) DateTimeOffsetPattern = ShortDatePattern[0];
	DateTimeOffsetPattern += L" ";
	if (LongDatePattern.size() > 0) DateTimeOffsetPattern += LongDatePattern[0];

	bool foundZ = false;
	bool inQuote = false;
	wchar_t quote = '\'';

	if (LongDatePattern.size() > 0)
	{
		for (int i = 0; !foundZ && i < (int)LongTimePattern[0].size(); i++) {
			switch (LongTimePattern[0][i]) {
			case 'z':
				/* if we aren't in a quote, we've found a z */
				foundZ = !inQuote;
				/* we'll fall out of the loop now because the test includes !foundZ */
				break;
			case '\'':
			case '\"':
				if (inQuote && (quote == LongTimePattern[0][i])) {
					/* we were in a quote and found a matching exit quote, so we are outside a quote now */
					inQuote = false;
				}
				else if (!inQuote) {
					quote = LongTimePattern[0][i];
					inQuote = true;
				}
				else {
					/* we were in a quote and saw the other type of quote character, so we are still in a quote */
				}
				break;
			case '%':
			case '\\':
				i++; /* skip next character that is escaped by this backslash */
				break;
			default:
				break;
			}
		}
	}

	if (!foundZ) 
	{
		DateTimeOffsetPattern = DateTimeOffsetPattern + L" zzz";
	}

	if (LongDatePattern.size()>0) FullLongDateShortTimePattern = LongDatePattern[0];
	FullLongDateShortTimePattern += L" ";
	if (ShortTimePattern.size()>0) FullLongDateShortTimePattern.append(ShortTimePattern[0]);

	if (LongDatePattern.size()>0) FullLongDateLongTimePattern = LongDatePattern[0];
	FullLongDateLongTimePattern += L" ";
	if (LongTimePattern.size()>0) FullLongDateLongTimePattern += LongTimePattern[0];

	if (ShortDatePattern.size()>0) GeneralShortDateShortTimePattern = ShortDatePattern[0];
	GeneralShortDateShortTimePattern += L" ";
	if (ShortTimePattern.size()>0) GeneralShortDateShortTimePattern += ShortTimePattern[0];

	if (ShortDatePattern.size()>0) GeneralShortDateLongTimePattern = ShortDatePattern[0];
	GeneralShortDateLongTimePattern += L" ";
	if (LongTimePattern.size()>0) GeneralShortDateLongTimePattern += LongTimePattern[0];

	HasSpacesInMonthNames = ((DateTimeFormatFlags)formatFlags & DateTimeFormatFlags::UseSpacesInMonthNames) != DateTimeFormatFlags::None;
	HasSpacesInDayNames = ((DateTimeFormatFlags)formatFlags & DateTimeFormatFlags::UseSpacesInDayNames) != DateTimeFormatFlags::None;

}

CultureData::CultureData(CultureInfo_impl* loc, int id)
{
	impl = loc;

#pragma region 货币信息	
	const std::moneypunct<char>& mp = std::use_facet<std::moneypunct<char>>(loc->_locale);
	const std::moneypunct<wchar_t>& wmp = std::use_facet<std::moneypunct<wchar_t> >(loc->_locale);
	frac_digits = mp.frac_digits();

	CultureData_Obtain_CS_SEP_SIGN(mp.pos_format().field, !mp.curr_symbol().empty(), !mp.positive_sign().empty(), p_cs_precedes, p_sep_by_space, p_sign_posn);
	CultureData_Obtain_CS_SEP_SIGN(mp.neg_format().field, !mp.curr_symbol().empty(), !mp.negative_sign().empty(), n_cs_precedes, n_sep_by_space, n_sign_posn);

	_W_decimal_point = new wchar_t[1]{ wmp.decimal_point() };
	_W_thousands_sep = new wchar_t[1]{ wmp.thousands_sep() };
	_W_currency_symbol = Cloner::Clone(wmp.curr_symbol());
	_W_mon_decimal_point = new wchar_t[1]{ wmp.decimal_point() };
	_W_mon_thousands_sep = new wchar_t[1]{ wmp.thousands_sep() };
	_W_positive_sign = Cloner::Clone(wmp.positive_sign());
	_W_negative_sign = Cloner::Clone(wmp.negative_sign());
#pragma endregion

	FullTimeSpanPositivePattern = L"d':'h':'mm':'ss'";
	FullTimeSpanPositivePattern += _W_decimal_point;
	FullTimeSpanPositivePattern += L"'FFFFFFF";

	FullTimeSpanNegativePattern = L'-' + FullTimeSpanPositivePattern;

	if (!loc->_name.empty() && loc->_name.size() != 0)
	{
		Microsoft::winapi_EnumCalendarInfo(Custom_EnumCalendarInfoProc, loc->_name.c_str(), API_ENUM_ALL_CALENDARS, NULL, API_CAL_SCALNAME, this);

		CalendarData* cal = nullptr;
		for (auto& ca : cals)
		{
			if (ca->CalId == id)
			{
				cal = ca;
			}
		}
		if (cal)
		{
			YearMonthPattern = cal->saYearMonths;
			LongDatePattern = cal->saLongDates;
			ShortDatePattern = cal->saShortDates;
			LongTimePattern = cal->saYearMonths;
			ShortTimePattern = cal->saShortTimes;
			LongTimePattern = cal->saLongTimes;
			MonthDayPattern = cal->sMonthDay;


			if (!_getinfo(loc->_lcid, API_LOCALE_SAM, amDesignator)) amDesignator = L"AM";
			if (!_getinfo(loc->_lcid, API_LOCALE_SPM, pmDesignator)) pmDesignator = L"PM";
			if (!_getinfo(loc->_lcid, API_LOCALE_SDATE, dateSeparator)) dateSeparator = L"/";
			if (!_getinfo(loc->_lcid, API_LOCALE_STIME, timeSeparator)) timeSeparator = L":";


			_getinfo(loc->_lcid, API_LOCALE_SISO639LANGNAME, langName);

			std::wstring sName;
			_getinfo(loc->_lcid, API_LOCALE_SNAME, sName);

			if (StringHelper::Compare(sName, std::wstring(L"zh-CHS")) == 0 || StringHelper::Compare(sName, std::wstring(L"zh-CHT")) == 0)
				culName = sName;
			else
				culName = loc->_name;

			std::wstring fdwm;
			_getinfo(loc->_lcid, API_LOCALE_IFIRSTDAYOFWEEK, fdwm);
			_firstDayofWeekIsMonday = fdwm.empty() ? true : _wcsicmp(fdwm.c_str(), L"0") == 0;

			bool ugm = false;
			int i = 0;
			for (; i < 12; i++)
			{
				ugm = wcscmp(cal->saMonthNames[i].c_str(), cal->saMonthGenitiveNames[i].c_str()) != 0;
				if (ugm) break;
			}
			if (ugm)
				_useGenitiveMonth = true;
			else
			{
				for (i = 0; i < 12; i++)
				{
					ugm = wcscmp(cal->saAbbrevMonthNames[i].c_str(), cal->saAbbrevMonthGenitiveNames[i].c_str()) != 0;
					if (ugm) break;
				}
				_useGenitiveMonth = ugm;
			}

		}
		else
		{
			auto& cal = CalendarData::GetInvariant();

			YearMonthPattern = wstringvector{ L"yyyy MMMM" };
			LongDatePattern = wstringvector{ L"dddd, dd MMMM yyyy" };
			ShortDatePattern = wstringvector{ L"MM/dd/yyyy", L"yyyy-MM-dd" };
			LongTimePattern = wstringvector{ L"HH:mm:ss" };
			ShortTimePattern = wstringvector{ L"HH:mm", L"hh:mm tt", L"H:mm", L"h:mm tt" };
			MonthDayPattern = L"MMMM dd";


			amDesignator = L"AM";
			pmDesignator = L"PM";
			dateSeparator = L"/";
			timeSeparator = L":";

			_firstDayofWeekIsMonday = true;
			_useGenitiveMonth = false;
		}
	}
	else
	{
		cals.push_back(&CalendarData::GetInvariant());
		auto& cal = CalendarData::GetInvariant();

		YearMonthPattern = cal.saYearMonths;
		LongDatePattern = cal.saLongDates;
		ShortDatePattern = cal.saShortDates;
		LongTimePattern = cal.saYearMonths;
		ShortTimePattern = cal.saShortTimes;
		LongTimePattern = cal.saLongTimes;

		amDesignator = L"AM";
		pmDesignator = L"PM";
		dateSeparator = L"/";
		timeSeparator = L":";

		_firstDayofWeekIsMonday = true;
		_useGenitiveMonth = false;
	}
	formatFlags = 0;
	if (cals.size() > 0)
	{
		auto& cal = cals[0];
		if (cal)
		{
			formatFlags = ((int)formatFlags | (int)GetFormatFlagGenitiveMonth(cal->saMonthNames, cal->saMonthGenitiveNames, cal->saAbbrevMonthNames, cal->saAbbrevMonthGenitiveNames));
			formatFlags = ((int)formatFlags | (int)GetFormatFlagUseSpaceInMonthNames(cal->saMonthNames, cal->saMonthGenitiveNames, cal->saAbbrevMonthNames, cal->saAbbrevMonthGenitiveNames));
			formatFlags = ((int)formatFlags | (int)GetFormatFlagUseSpaceInDayNames(cal->saDayNames, cal->saAbbrevDayNames));
			formatFlags = ((int)formatFlags | (int)GetFormatFlagUseHebrewCalendar(cal->CalId));
		}
	}

	if (ShortDatePattern.size() > 0) DateTimeOffsetPattern = ShortDatePattern[0];
	DateTimeOffsetPattern += L" ";
	if (LongDatePattern.size() > 0) DateTimeOffsetPattern += LongDatePattern[0];

	bool foundZ = false;
	bool inQuote = false;
	wchar_t quote = '\'';

	if (LongDatePattern.size() > 0)
	{
		for (int i = 0; !foundZ && i < (int)LongTimePattern[0].size(); i++) {
			switch (LongTimePattern[0][i]) {
			case 'z':
				/* if we aren't in a quote, we've found a z */
				foundZ = !inQuote;
				/* we'll fall out of the loop now because the test includes !foundZ */
				break;
			case '\'':
			case '\"':
				if (inQuote && (quote == LongTimePattern[0][i])) {
					/* we were in a quote and found a matching exit quote, so we are outside a quote now */
					inQuote = false;
				}
				else if (!inQuote) {
					quote = LongTimePattern[0][i];
					inQuote = true;
				}
				else {
					/* we were in a quote and saw the other type of quote character, so we are still in a quote */
				}
				break;
			case '%':
			case '\\':
				i++; /* skip next character that is escaped by this backslash */
				break;
			default:
				break;
			}
		}
	}

	if (!foundZ)
	{
		DateTimeOffsetPattern = DateTimeOffsetPattern + L" zzz";
	}

	if (LongDatePattern.size()>0) FullLongDateShortTimePattern = LongDatePattern[0];
	FullLongDateShortTimePattern += L" ";
	if (ShortTimePattern.size()>0) FullLongDateShortTimePattern += ShortTimePattern[0];

	if (LongDatePattern.size()>0) FullLongDateLongTimePattern = LongDatePattern[0];
	FullLongDateLongTimePattern += L" ";
	if (LongTimePattern.size()>0) FullLongDateLongTimePattern += LongTimePattern[0];

	if (ShortDatePattern.size()>0) GeneralShortDateShortTimePattern = ShortDatePattern[0];
	GeneralShortDateShortTimePattern += L" ";
	if (ShortTimePattern.size()>0) GeneralShortDateShortTimePattern += ShortTimePattern[0];

	if (ShortDatePattern.size()>0) GeneralShortDateLongTimePattern = ShortDatePattern[0];
	GeneralShortDateLongTimePattern += L" ";
	if (LongTimePattern.size()>0) GeneralShortDateLongTimePattern += LongTimePattern[0];

	HasSpacesInMonthNames = ((DateTimeFormatFlags)formatFlags & DateTimeFormatFlags::UseSpacesInMonthNames) != DateTimeFormatFlags::None;
	HasSpacesInDayNames = ((DateTimeFormatFlags)formatFlags & DateTimeFormatFlags::UseSpacesInDayNames) != DateTimeFormatFlags::None;

}

CultureData::CultureData()
{
	impl = nullptr;

#pragma region 货币信息	
	std::locale ivloc=std::locale("ivl");
	const std::moneypunct<char>& mp = std::use_facet<std::moneypunct<char>>(ivloc);
	const std::moneypunct<wchar_t>& wmp = std::use_facet<std::moneypunct<wchar_t> >(ivloc);
	frac_digits = mp.frac_digits();

	CultureData_Obtain_CS_SEP_SIGN(mp.pos_format().field, !mp.curr_symbol().empty(), !mp.positive_sign().empty(), p_cs_precedes, p_sep_by_space, p_sign_posn);
	CultureData_Obtain_CS_SEP_SIGN(mp.neg_format().field, !mp.curr_symbol().empty(), !mp.negative_sign().empty(), n_cs_precedes, n_sep_by_space, n_sign_posn);

	_W_decimal_point = new wchar_t[1]{ wmp.decimal_point() };
	_W_thousands_sep = new wchar_t[1]{ wmp.thousands_sep() };
	_W_currency_symbol = Cloner::Clone(wmp.curr_symbol());
	_W_mon_decimal_point = new wchar_t[1]{ wmp.decimal_point() };
	_W_mon_thousands_sep = new wchar_t[1]{ wmp.thousands_sep() };
	_W_positive_sign = Cloner::Clone(wmp.positive_sign());
	_W_negative_sign = Cloner::Clone(wmp.negative_sign());
#pragma endregion

	FullTimeSpanPositivePattern = L"d':'h':'mm':'ss'";
	FullTimeSpanPositivePattern += _W_decimal_point;
	FullTimeSpanPositivePattern += L"'FFFFFFF";

	FullTimeSpanNegativePattern = L'-' + FullTimeSpanPositivePattern;

	cals.push_back(&CalendarData::GetInvariant());
	auto& cal = CalendarData::GetInvariant();

	YearMonthPattern = wstringvector{ L"yyyy MMMM" };
	LongDatePattern = wstringvector{ L"dddd, dd MMMM yyyy" };
	ShortDatePattern = wstringvector{ L"MM/dd/yyyy", L"yyyy-MM-dd" };
	LongTimePattern = wstringvector{ L"HH:mm:ss" };
	ShortTimePattern = wstringvector{ L"HH:mm", L"hh:mm tt", L"H:mm", L"h:mm tt" };
	MonthDayPattern = L"MMMM dd";

	amDesignator = L"AM";
	pmDesignator = L"PM";
	dateSeparator = L"/";
	timeSeparator = L":";

	_firstDayofWeekIsMonday = true;
	_useGenitiveMonth = false;

	_hasForceTwoDigitYears = false;

	formatFlags = 0;
	formatFlags = ((int)formatFlags | (int)GetFormatFlagGenitiveMonth(cal.saMonthNames, cal.saMonthGenitiveNames, cal.saAbbrevMonthNames, cal.saAbbrevMonthGenitiveNames));
	formatFlags = ((int)formatFlags | (int)GetFormatFlagUseSpaceInMonthNames(cal.saMonthNames, cal.saMonthGenitiveNames, cal.saAbbrevMonthNames, cal.saAbbrevMonthGenitiveNames));
	formatFlags = ((int)formatFlags | (int)GetFormatFlagUseSpaceInDayNames(cal.saDayNames, cal.saAbbrevDayNames));
	formatFlags = ((int)formatFlags | (int)GetFormatFlagUseHebrewCalendar(cal.CalId));


	if (ShortDatePattern.size() > 0) DateTimeOffsetPattern = ShortDatePattern[0];
	DateTimeOffsetPattern += L" ";
	if (LongDatePattern.size() > 0) DateTimeOffsetPattern += LongDatePattern[0];

	bool foundZ = false;
	bool inQuote = false;
	wchar_t quote = '\'';

	if (LongDatePattern.size() > 0)
	{
		for (int i = 0; !foundZ && i < (int)LongTimePattern[0].size(); i++) {
			switch (LongTimePattern[0][i]) {
			case 'z':
				/* if we aren't in a quote, we've found a z */
				foundZ = !inQuote;
				/* we'll fall out of the loop now because the test includes !foundZ */
				break;
			case '\'':
			case '\"':
				if (inQuote && (quote == LongTimePattern[0][i])) {
					/* we were in a quote and found a matching exit quote, so we are outside a quote now */
					inQuote = false;
				}
				else if (!inQuote) {
					quote = LongTimePattern[0][i];
					inQuote = true;
				}
				else {
					/* we were in a quote and saw the other type of quote character, so we are still in a quote */
				}
				break;
			case '%':
			case '\\':
				i++; /* skip next character that is escaped by this backslash */
				break;
			default:
				break;
			}
		}
	}

	if (!foundZ)
	{
		DateTimeOffsetPattern = DateTimeOffsetPattern + L" zzz";
	}

	if (LongDatePattern.size()>0) FullLongDateShortTimePattern = LongDatePattern[0];
	FullLongDateShortTimePattern += L" ";
	if (ShortTimePattern.size()>0) FullLongDateShortTimePattern += ShortTimePattern[0];

	if (LongDatePattern.size()>0) FullLongDateLongTimePattern = LongDatePattern[0];
	FullLongDateLongTimePattern += L" ";
	if (LongTimePattern.size()>0) FullLongDateLongTimePattern += LongTimePattern[0];

	if (ShortDatePattern.size()>0) GeneralShortDateShortTimePattern = ShortDatePattern[0];
	GeneralShortDateShortTimePattern += L" ";
	if (ShortTimePattern.size()>0) GeneralShortDateShortTimePattern += ShortTimePattern[0];

	if (ShortDatePattern.size()>0) GeneralShortDateLongTimePattern = ShortDatePattern[0];
	GeneralShortDateLongTimePattern += L" ";
	if (LongTimePattern.size()>0) GeneralShortDateLongTimePattern += LongTimePattern[0];

	HasSpacesInMonthNames = ((DateTimeFormatFlags)formatFlags & DateTimeFormatFlags::UseSpacesInMonthNames) != DateTimeFormatFlags::None;
	HasSpacesInDayNames = ((DateTimeFormatFlags)formatFlags & DateTimeFormatFlags::UseSpacesInDayNames) != DateTimeFormatFlags::None;
}

CultureData* CultureData::Create(CultureInfo_impl* loc)
{
	return  loc == nullptr ? InVariant : new CultureData(loc);
}

CultureData* CultureData::Create(CultureInfo_impl* loc, int id)
{
	std::vector<unsigned long> count;
	Microsoft::winapi_EnumCalendarInfo(Count_EnumCalendarInfoProc, loc->_name.c_str(), API_ENUM_ALL_CALENDARS, NULL, API_CAL_SCALNAME, &count);

	if (std::find(count.begin(), count.end(), (unsigned long)id) != count.end())
	{
		return new CultureData(loc, id);
	}
	return Create(loc);
}


const char CultureData::GetMoneyConst(CultureInfoMoneyConsts moneyconst) const
{
	switch (moneyconst)
	{
	case CultureInfoMoneyConsts::Fraction:
		return frac_digits;
	case CultureInfoMoneyConsts::PositiveCurrencySymbolPos:
		return p_cs_precedes;
	case CultureInfoMoneyConsts::NegativeCurrencySymbolPos:
		return n_cs_precedes;
	case CultureInfoMoneyConsts::PositiveSeparatorPos:
		return p_sep_by_space;
	case CultureInfoMoneyConsts::NegativeSeparatorPos:
		return n_sep_by_space;
	case CultureInfoMoneyConsts::PositiveSignSymbolPos:
		return p_sign_posn;
	case CultureInfoMoneyConsts::NegativeSignSymbolPos:
		return n_sign_posn;
	default:
		return 0;
	}
}

unsigned int CultureData::GetPatternCount(CultureInfoPatterns pattern) const
{
	switch (pattern)
	{
	case CultureInfoPatterns::FullTimeSpanPositivePattern:
		return 1U;
	case CultureInfoPatterns::FullTimeSpanNegativePattern:
		return 1U;
	case CultureInfoPatterns::YearMonthPattern:
		return YearMonthPattern.size();
	case CultureInfoPatterns::LongDatePattern:
		return LongDatePattern.size();
	case CultureInfoPatterns::ShortDatePattern:
		return ShortDatePattern.size();
	case CultureInfoPatterns::LongTimePattern:
		return LongTimePattern.size();
	case CultureInfoPatterns::ShortTimePattern:
		return ShortTimePattern.size();
	case CultureInfoPatterns::FullLongDateShortTimePattern:
		return 1U;
	case CultureInfoPatterns::FullLongDateLongTimePattern:
		return 1U;
	case CultureInfoPatterns::GeneralShortDateShortTimePattern:
		return 1U;
	case CultureInfoPatterns::GeneralShortDateLongTimePattern:
		return 1U;
	case CultureInfoPatterns::DateTimeOffsetPattern:
		return 1U;
	default:
		return 0U;
	}
	return 0U;
}

const wchar_t* CultureData::GetPattern(CultureInfoPatterns pattern, unsigned int index) const
{
	switch (pattern)
	{
	case CultureInfoPatterns::FullTimeSpanPositivePattern:
		return FullTimeSpanPositivePattern.c_str();
	case CultureInfoPatterns::FullTimeSpanNegativePattern:
		return FullTimeSpanNegativePattern.c_str();
	case CultureInfoPatterns::YearMonthPattern:
	{
		if (index < YearMonthPattern.size())
			return YearMonthPattern[index].c_str();
		else
			return L"";
	}	
	case CultureInfoPatterns::LongDatePattern:
	{
		if (index < LongDatePattern.size())
			return LongDatePattern[index].c_str();
		else
			return L"";
	}
	case CultureInfoPatterns::ShortDatePattern:
	{
		if (index < ShortDatePattern.size())
			return ShortDatePattern[index].c_str();
		else
			return L"";
	}
	case CultureInfoPatterns::LongTimePattern:
	{
		if (index < LongTimePattern.size())
			return LongTimePattern[index].c_str();
		else
			return L"";
	}
	case CultureInfoPatterns::ShortTimePattern:
	{
		if (index < ShortTimePattern.size())
			return ShortTimePattern[index].c_str();
		else
			return L"";
	}	
	case CultureInfoPatterns::FullLongDateShortTimePattern:
		return FullLongDateShortTimePattern.c_str();
	case CultureInfoPatterns::FullLongDateLongTimePattern:
		return FullLongDateLongTimePattern.c_str();
	case CultureInfoPatterns::GeneralShortDateShortTimePattern:
		return GeneralShortDateShortTimePattern.c_str();
	case CultureInfoPatterns::GeneralShortDateLongTimePattern:
		return GeneralShortDateLongTimePattern.c_str();
	case CultureInfoPatterns::DateTimeOffsetPattern:
		return DateTimeOffsetPattern.c_str();
	default:
		return L"";
	}
	return L"";
}

unsigned int CultureData::GetConstsNameCount(CultureInfoConstNames name) const
{
	switch (name)
	{
	case CultureInfoConstNames::Era:
		return  cals[0]->saEraNames.size();
	case CultureInfoConstNames::Abbreviated_Era:
		return  cals[0]->saAbbrevEraNames.size();
	case CultureInfoConstNames::Abbreviated_EnglishEra:
		return  cals[0]->saAbbrevEnglishEraNames.size();
	case CultureInfoConstNames::Sunday:
	case CultureInfoConstNames::Monday:
	case CultureInfoConstNames::Tuesday:
	case CultureInfoConstNames::Wednesday:
	case CultureInfoConstNames::Thursday:
	case CultureInfoConstNames::Friday:
	case CultureInfoConstNames::Saturday:
	case CultureInfoConstNames::Abbreviated_Sunday:
	case CultureInfoConstNames::Abbreviated_Monday:
	case CultureInfoConstNames::Abbreviated_Tuesday:
	case CultureInfoConstNames::Abbreviated_Wednesday:
	case CultureInfoConstNames::Abbreviated_Thursday:
	case CultureInfoConstNames::Abbreviated_Friday:
	case CultureInfoConstNames::Abbreviated_Saturday:
	case CultureInfoConstNames::January:
	case CultureInfoConstNames::Genitive_Abbreviated_January:
	case CultureInfoConstNames::Genitive_January:
	case CultureInfoConstNames::February:
	case CultureInfoConstNames::Genitive_Abbreviated_February:
	case CultureInfoConstNames::Genitive_February:
	case CultureInfoConstNames::March:
	case CultureInfoConstNames::Genitive_Abbreviated_March:
	case CultureInfoConstNames::Genitive_March:
	case CultureInfoConstNames::April:
	case CultureInfoConstNames::Genitive_Abbreviated_April:
	case CultureInfoConstNames::Genitive_April:
	case CultureInfoConstNames::May:
	case CultureInfoConstNames::Genitive_Abbreviated_May:
	case CultureInfoConstNames::Genitive_May:
	case CultureInfoConstNames::June:
	case CultureInfoConstNames::Genitive_Abbreviated_June:
	case CultureInfoConstNames::Genitive_June:
	case CultureInfoConstNames::July:
	case CultureInfoConstNames::Genitive_Abbreviated_July:
	case CultureInfoConstNames::Genitive_July:
	case CultureInfoConstNames::August:
	case CultureInfoConstNames::Genitive_Abbreviated_August:
	case CultureInfoConstNames::Genitive_August:
	case CultureInfoConstNames::September:
	case CultureInfoConstNames::Genitive_Abbreviated_September:
	case CultureInfoConstNames::Genitive_September:
	case CultureInfoConstNames::October:
	case CultureInfoConstNames::Genitive_Abbreviated_October:
	case CultureInfoConstNames::Genitive_October:
	case CultureInfoConstNames::November:
	case CultureInfoConstNames::Genitive_Abbreviated_November:
	case CultureInfoConstNames::Genitive_November:
	case CultureInfoConstNames::December:
	case CultureInfoConstNames::Genitive_Abbreviated_December:
	case CultureInfoConstNames::Genitive_December:
	case CultureInfoConstNames::Abbreviated_January:
	case CultureInfoConstNames::Abbreviated_February:
	case CultureInfoConstNames::Abbreviated_March:
	case CultureInfoConstNames::Abbreviated_April:
	case CultureInfoConstNames::Abbreviated_May:
	case CultureInfoConstNames::Abbreviated_June:
	case CultureInfoConstNames::Abbreviated_July:
	case CultureInfoConstNames::Abbreviated_August:
	case CultureInfoConstNames::Abbreviated_September:
	case CultureInfoConstNames::Abbreviated_October:
	case CultureInfoConstNames::Abbreviated_November:
	case CultureInfoConstNames::Abbreviated_December:
	case CultureInfoConstNames::AMDesignator:
	case CultureInfoConstNames::PMDesignator:
	case CultureInfoConstNames::DateSeparator:
	case CultureInfoConstNames::TimeSeparator:
	case CultureInfoConstNames::DecimalPoint:
	case CultureInfoConstNames::ThousandsSeparator:
	case CultureInfoConstNames::CurrencySymbol:
	case CultureInfoConstNames::MoneyDecimalPoint:	
	case CultureInfoConstNames::MoneyThousandsSeparator:
	case CultureInfoConstNames::PositiveSignSymbol:
	case CultureInfoConstNames::NegativeSignSymbol:
	default:
		return 1U;
	}
	return 0U;
}

const wchar_t* CultureData::GetConstsName(CultureInfoConstNames name, unsigned int index) const
{
	switch (name)
	{
	case CultureInfoConstNames::Sunday:
		return cals[0]->saDayNames[0].c_str();
	case CultureInfoConstNames::Monday:
		return cals[0]->saDayNames[1].c_str();
	case CultureInfoConstNames::Tuesday:
		return cals[0]->saDayNames[2].c_str();
	case CultureInfoConstNames::Wednesday:
		return cals[0]->saDayNames[3].c_str();
	case CultureInfoConstNames::Thursday:
		return cals[0]->saDayNames[4].c_str();
	case CultureInfoConstNames::Friday:
		return cals[0]->saDayNames[5].c_str();
	case CultureInfoConstNames::Saturday:
		return cals[0]->saDayNames[6].c_str();
	case CultureInfoConstNames::Abbreviated_Sunday:
		return cals[0]->saAbbrevDayNames[0].c_str();
	case CultureInfoConstNames::Abbreviated_Monday:
		return cals[0]->saAbbrevDayNames[1].c_str();
	case CultureInfoConstNames::Abbreviated_Tuesday:
		return cals[0]->saAbbrevDayNames[2].c_str();
	case CultureInfoConstNames::Abbreviated_Wednesday:
		return cals[0]->saAbbrevDayNames[3].c_str();
	case CultureInfoConstNames::Abbreviated_Thursday:
		return cals[0]->saAbbrevDayNames[4].c_str();
	case CultureInfoConstNames::Abbreviated_Friday:
		return cals[0]->saAbbrevDayNames[5].c_str();
	case CultureInfoConstNames::Abbreviated_Saturday:
		return cals[0]->saAbbrevDayNames[6].c_str();
	case CultureInfoConstNames::January:
		return cals[0]->saMonthNames[0].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_January:
		return cals[0]->saAbbrevMonthGenitiveNames[0].c_str(); 
	case CultureInfoConstNames::Genitive_January:
		return cals[0]->saMonthGenitiveNames[0].c_str();
	case CultureInfoConstNames::February:
		return cals[0]->saMonthNames[1].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_February:
		return cals[0]->saAbbrevMonthGenitiveNames[1].c_str();
	case CultureInfoConstNames::Genitive_February:
		return cals[0]->saMonthGenitiveNames[1].c_str();
	case CultureInfoConstNames::March:
		return cals[0]->saMonthNames[2].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_March:
		return cals[0]->saAbbrevMonthGenitiveNames[2].c_str();
	case CultureInfoConstNames::Genitive_March:
		return cals[0]->saMonthGenitiveNames[2].c_str();
	case CultureInfoConstNames::April:
		return cals[0]->saMonthNames[3].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_April:
		return cals[0]->saAbbrevMonthGenitiveNames[3].c_str();
	case CultureInfoConstNames::Genitive_April:
		return cals[0]->saMonthGenitiveNames[3].c_str();
	case CultureInfoConstNames::May:
		return cals[0]->saMonthNames[4].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_May:
		return cals[0]->saAbbrevMonthGenitiveNames[4].c_str();
	case CultureInfoConstNames::Genitive_May:
		return cals[0]->saMonthGenitiveNames[4].c_str();
	case CultureInfoConstNames::June:
		return cals[0]->saMonthNames[5].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_June:
		return cals[0]->saAbbrevMonthGenitiveNames[5].c_str();
	case CultureInfoConstNames::Genitive_June:
		return cals[0]->saMonthGenitiveNames[5].c_str();
	case CultureInfoConstNames::July:
		return cals[0]->saMonthNames[6].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_July:
		return cals[0]->saAbbrevMonthGenitiveNames[6].c_str();
	case CultureInfoConstNames::Genitive_July:
		return cals[0]->saMonthGenitiveNames[6].c_str();
	case CultureInfoConstNames::August:
		return cals[0]->saMonthNames[7].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_August:
		return cals[0]->saAbbrevMonthGenitiveNames[7].c_str();
	case CultureInfoConstNames::Genitive_August:
		return cals[0]->saMonthGenitiveNames[7].c_str();
	case CultureInfoConstNames::September:
		return cals[0]->saMonthNames[8].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_September:
		return cals[0]->saAbbrevMonthGenitiveNames[8].c_str();
	case CultureInfoConstNames::Genitive_September:
		return cals[0]->saMonthGenitiveNames[8].c_str();
	case CultureInfoConstNames::October:
		return cals[0]->saMonthNames[9].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_October:
		return cals[0]->saAbbrevMonthGenitiveNames[9].c_str();
	case CultureInfoConstNames::Genitive_October:
		return cals[0]->saMonthGenitiveNames[9].c_str();
	case CultureInfoConstNames::November:
		return cals[0]->saMonthNames[10].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_November:
		return cals[0]->saAbbrevMonthGenitiveNames[10].c_str();
	case CultureInfoConstNames::Genitive_November:
		return cals[0]->saMonthGenitiveNames[10].c_str();
	case CultureInfoConstNames::December:
		return cals[0]->saMonthNames[11].c_str();
	case CultureInfoConstNames::Genitive_Abbreviated_December:
		return cals[0]->saAbbrevMonthGenitiveNames[11].c_str();
	case CultureInfoConstNames::Genitive_December:
		return cals[0]->saMonthGenitiveNames[11].c_str();
	case CultureInfoConstNames::Abbreviated_January:
		return cals[0]->saAbbrevMonthNames[0].c_str();
	case CultureInfoConstNames::Abbreviated_February:
		return cals[0]->saAbbrevMonthNames[1].c_str();
	case CultureInfoConstNames::Abbreviated_March:
		return cals[0]->saAbbrevMonthNames[2].c_str();
	case CultureInfoConstNames::Abbreviated_April:
		return cals[0]->saAbbrevMonthNames[3].c_str();
	case CultureInfoConstNames::Abbreviated_May:
		return cals[0]->saAbbrevMonthNames[4].c_str();
	case CultureInfoConstNames::Abbreviated_June:
		return cals[0]->saAbbrevMonthNames[5].c_str();
	case CultureInfoConstNames::Abbreviated_July:
		return cals[0]->saAbbrevMonthNames[6].c_str();
	case CultureInfoConstNames::Abbreviated_August:
		return cals[0]->saAbbrevMonthNames[7].c_str();
	case CultureInfoConstNames::Abbreviated_September:
		return cals[0]->saAbbrevMonthNames[8].c_str();
	case CultureInfoConstNames::Abbreviated_October:
		return cals[0]->saAbbrevMonthNames[9].c_str();
	case CultureInfoConstNames::Abbreviated_November:
		return cals[0]->saAbbrevMonthNames[10].c_str();
	case CultureInfoConstNames::Abbreviated_December:
		return cals[0]->saAbbrevMonthNames[11].c_str();
	case CultureInfoConstNames::Era:
		return  index<cals[0]->saEraNames.size() ? cals[0]->saEraNames[index].c_str() : L"";
	case CultureInfoConstNames::Abbreviated_Era:
		return  index<cals[0]->saAbbrevEraNames.size() ? cals[0]->saAbbrevEraNames[index].c_str() : L"";
	case CultureInfoConstNames::Abbreviated_EnglishEra:
		return  index<cals[0]->saAbbrevEnglishEraNames.size() ? cals[0]->saAbbrevEnglishEraNames[index].c_str() : L"";
	case CultureInfoConstNames::AMDesignator:
		return amDesignator.c_str();
	case CultureInfoConstNames::PMDesignator:
		return pmDesignator.c_str();
	case CultureInfoConstNames::DateSeparator:
		return dateSeparator.c_str();
	case CultureInfoConstNames::TimeSeparator:
		return timeSeparator.c_str();
	case CultureInfoConstNames::DecimalPoint:
		return _W_decimal_point ? _W_decimal_point : L"";
	case CultureInfoConstNames::ThousandsSeparator:
		return _W_thousands_sep ? _W_thousands_sep : L"";
	case CultureInfoConstNames::CurrencySymbol:
		return _W_currency_symbol ? _W_currency_symbol : L"";
	case CultureInfoConstNames::MoneyDecimalPoint:
		return _W_mon_decimal_point ? _W_mon_decimal_point : L"";
	case CultureInfoConstNames::MoneyThousandsSeparator:
		return _W_mon_thousands_sep ? _W_mon_thousands_sep : L"";
	case CultureInfoConstNames::PositiveSignSymbol:
		return _W_positive_sign ? _W_positive_sign : L"";
	case CultureInfoConstNames::NegativeSignSymbol:
		return _W_negative_sign ? _W_negative_sign : L"";
	default:
		return false;
	}
}

bool CultureData::GetBooleanFlags(CultureInfoBooleanFlags flag) const
{
	switch (flag)
	{
	case CultureInfoBooleanFlags::FirstDayofWeekIsMonday:
		return _firstDayofWeekIsMonday;
	case CultureInfoBooleanFlags::HasForceTwoDigitYears:
		return _hasForceTwoDigitYears;
	case CultureInfoBooleanFlags::UseGenitiveMonth:
		return _useGenitiveMonth;
	case CultureInfoBooleanFlags::Has13Months:
		return cals[0]->saMonthNames.size() == 13;
	default:
		return true;
	}
}

CultureData::~CultureData()
{

	SAFE_DELETE(_W_decimal_point);
	SAFE_DELETE(_W_thousands_sep);
	SAFE_DELETE(_W_currency_symbol);
	SAFE_DELETE(_W_mon_decimal_point);
	SAFE_DELETE(_W_mon_thousands_sep);
	SAFE_DELETE(_W_positive_sign);
	SAFE_DELETE(_W_negative_sign);

}

bool CultureInfo_impl::operator == (const CultureInfo_impl& info) const
{
	return info._name == _name;
}

bool CultureInfo_impl::operator != (const CultureInfo_impl& info) const
{
	return info._name != _name;
}