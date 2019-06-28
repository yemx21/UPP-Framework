#include "CalendarData.h"
#include "CultureData.h"
#include "CultureInfo_impl.h"
#include "WindowsEnvironment.h"
using namespace System;
using namespace System::OS;

std::vector<CalendarData*> CalendarData::globals;
CriticalSection CalendarData::cs;

CalendarData::CalendarData()
{

}

CalendarData& CalendarData::GetInvariant()
{
	static CalendarData calendarDatum = CalendarData();
	calendarDatum.sNativeName = L"Gregorian Calendar";
	calendarDatum.iTwoDigitYearMax = 2029;
	calendarDatum.iCurrentEra = 1;
	calendarDatum.saShortDates = wstringvector{ L"MM/dd/yyyy", L"yyyy-MM-dd" };
	calendarDatum.saLongDates = wstringvector{ L"dddd, dd MMMM yyyy" };
	calendarDatum.saYearMonths = wstringvector{ L"yyyy MMMM" };
	calendarDatum.sMonthDay = L"MMMM dd";
	calendarDatum.saEraNames = wstringvector{ L"A.D." };
	calendarDatum.saAbbrevEraNames = wstringvector{ L"AD" };
	calendarDatum.saAbbrevEnglishEraNames = wstringvector{ L"AD" };
	calendarDatum.saDayNames = wstringvector { L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };
	calendarDatum.saAbbrevDayNames = wstringvector { L"Sun", L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat" };
	calendarDatum.saSuperShortDayNames = wstringvector { L"Su", L"Mo", L"Tu", L"We", L"Th", L"Fr", L"Sa" };
	calendarDatum.saMonthNames = wstringvector { L"January", L"February", L"March", L"April", L"May", L"June", L"July", L"August", L"September", L"October", L"November", L"December", L"" };
	calendarDatum.saAbbrevMonthNames = wstringvector{ L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec", L"" };
	calendarDatum.saShortTimes = wstringvector{ L"HH:mm", L"hh:mm tt", L"H:mm", L"h:mm tt"};
	calendarDatum.saLongTimes = wstringvector{ L"HH:mm::ss" };
	calendarDatum.saMonthGenitiveNames = calendarDatum.saMonthNames;
	calendarDatum.saAbbrevMonthGenitiveNames = calendarDatum.saAbbrevMonthNames;
	calendarDatum.saLeapYearMonthNames = calendarDatum.saMonthNames;
	calendarDatum.bUseUserOverrides = false;
	return calendarDatum;
}

__inline bool GetCalendarInfoString(const wchar_t* name, int calendar, API_UINT32 cal, std::wstring& result)
{
	int len = Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal, NULL, 0, NULL);
	if (len <= 0) return false;
	result.resize(len);
	if (Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal, &result.at(0), len, NULL) == 0)
	{
		result.clear();
		return false;
	}
	if (result[len-1] == '\0')
	{
		result.pop_back();
	}
	return true;
}

__inline bool GetCalendarInfoInteger(const wchar_t* name, int calendar, API_UINT32 cal, int& result)
{
	API_UINT32 val;
	if (Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal | API_CAL_RETURN_NUMBER, NULL, 0, &val) == 2)
	{
		result = val;
		return true;
	}
	return false;
}

__inline bool GetCalendarInfoString(const wchar_t* name, int calendar, API_UINT32 cal, wstringvector& result)
{
	int len = Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal, NULL, 0, NULL);
	if (len <= 0) return false;
	result.resize(1);
	result[0].resize(len);
	if (Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal, &result[0].at(0), len, NULL) == 0)
	{
		result.clear();
		return false;
	}
	if (result[0][len - 1] == '\0')
	{
		result[0].pop_back();
	}
	return true;
}

__inline bool GetCalendarInfoString(const wchar_t* name, int calendar, API_UINT32 cal, wstringvector& result, wstringvector::size_type index)
{
	int len = Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal, NULL, 0, NULL);
	if (len <= 0) return false;
	result[index].resize(len);
	if (Microsoft::winapi_GetCalendarInfo(name, calendar, NULL, cal, &result[index].at(0), len, NULL) == 0)
	{
		result.clear();
		return false;
	}
	if (result[index][len - 1] == '\0')
	{
		result[index].pop_back();
	}
	return true;
}

API_BOOL API_STDCALL Custom_EnumShortTimeFormatsProc(wchar_t* lpTimeFormatString, API_INT64 lParam)
{
	if (!lpTimeFormatString) return API_FALSE;
	if (!lParam) return API_FALSE;
	CalendarData* data = (CalendarData*)lParam;

	if (!OSHelper::IsValidAddress(data, sizeof(Calendar))) return API_FALSE;

	data->saShortTimes.push_back(lpTimeFormatString);

	return API_TRUE;
}

API_BOOL API_STDCALL Custom_EnumLongTimeFormatsProc(wchar_t* lpTimeFormatString, API_INT64 lParam)
{
	if (!lpTimeFormatString) return API_FALSE;
	if (!lParam) return API_FALSE;
	CalendarData* data = (CalendarData*)lParam;

	if (!OSHelper::IsValidAddress(data, sizeof(Calendar))) return API_FALSE;

	data->saLongTimes.push_back(lpTimeFormatString);

	return API_TRUE;
}

bool CalendarData::GetCalendarData(CalendarData* data, const std::string& localeName, int cal)
{
	std::wstring na;
	if (!Converter::Convert(localeName, na, std::locale())) return false;
	const wchar_t* s = na.c_str();

	data->sNativeName.clear();
	data->iTwoDigitYearMax = 2029;



	if (!GetCalendarInfoString(s, cal, API_CAL_SCALNAME, data->sNativeName)) return false;

	if (!GetCalendarInfoInteger(s, cal, API_CAL_ITWODIGITYEARMAX, data->iTwoDigitYearMax)) return false;
	
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTDATE, data->saShortDates)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SLONGDATE, data->saLongDates)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SYEARMONTH, data->saYearMonths)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHDAY, data->sMonthDay)) return false;
	
	if (!GetCalendarInfoString(s, cal, API_CAL_SERASTRING, data->saEraNames)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVERASTRING, data->saAbbrevEraNames)) return false;

	data->saDayNames.resize(7);
	data->saAbbrevDayNames.resize(7);
	data->saSuperShortDayNames.resize(7);
	data->saMonthNames.resize(13);
	data->saAbbrevMonthNames.resize(13);
	data->saMonthGenitiveNames.resize(13);
	data->saAbbrevMonthGenitiveNames.resize(13);
	
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME1, data->saDayNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME2, data->saDayNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME3, data->saDayNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME4, data->saDayNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME5, data->saDayNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME6, data->saDayNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SDAYNAME7, data->saDayNames, 6)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME1, data->saAbbrevDayNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME2, data->saAbbrevDayNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME3, data->saAbbrevDayNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME4, data->saAbbrevDayNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME5, data->saAbbrevDayNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME6, data->saAbbrevDayNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVDAYNAME7, data->saAbbrevDayNames, 6)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME1, data->saSuperShortDayNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME2, data->saSuperShortDayNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME3, data->saSuperShortDayNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME4, data->saSuperShortDayNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME5, data->saSuperShortDayNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME6, data->saSuperShortDayNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SSHORTESTDAYNAME7, data->saSuperShortDayNames, 6)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME1, data->saMonthNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME2, data->saMonthNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME3, data->saMonthNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME4, data->saMonthNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME5, data->saMonthNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME6, data->saMonthNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME7, data->saMonthNames, 6)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME8, data->saMonthNames, 7)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME9, data->saMonthNames, 8)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME10, data->saMonthNames, 9)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME11, data->saMonthNames, 10)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME12, data->saMonthNames, 11)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME13, data->saMonthNames, 12)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME1, data->saAbbrevMonthNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME2, data->saAbbrevMonthNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME3, data->saAbbrevMonthNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME4, data->saAbbrevMonthNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME5, data->saAbbrevMonthNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME6, data->saAbbrevMonthNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME7, data->saAbbrevMonthNames, 6)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME8, data->saAbbrevMonthNames, 7)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME9, data->saAbbrevMonthNames, 8)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME10, data->saAbbrevMonthNames, 9)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME11, data->saAbbrevMonthNames, 10)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME12, data->saAbbrevMonthNames, 11)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME13, data->saAbbrevMonthNames, 12)) return false;

	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME1 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME2 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME3 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME4 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME5 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME6 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME7 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 6)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME8 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 7)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME9 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 8)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME10 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 9)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME11 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 10)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME12 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 11)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SMONTHNAME13 | API_CAL_RETURN_GENITIVE_NAMES, data->saMonthGenitiveNames, 12)) return false;


	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME1 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 0)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME2 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 1)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME3 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 2)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME4 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 3)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME5 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 4)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME6 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 5)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME7 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 6)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME8 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 7)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME9 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 8)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME10 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 9)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME11 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 10)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME12 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 11)) return false;
	if (!GetCalendarInfoString(s, cal, API_CAL_SABBREVMONTHNAME13 | API_CAL_RETURN_GENITIVE_NAMES, data->saAbbrevMonthGenitiveNames, 12)) return false;

	Microsoft::winapi_EnumTimeFormats(Custom_EnumShortTimeFormatsProc, s, API_TIME_NOSECONDS, data);
	Microsoft::winapi_EnumTimeFormats(Custom_EnumLongTimeFormatsProc, s, 0, data);

	return true;
}
CalendarData::CalendarData(const std::string& localeName, int calendarId, bool bUseUser)
{
	Name = localeName;

	CalId = calendarId;

	CalendarData& invariant = GetInvariant();

	bUseUserOverrides = bUseUser;
	if (!CalendarData::GetCalendarData(this, localeName, calendarId))
	{
		if (sNativeName.empty())
		{
			sNativeName = L"";
		}
		if (saShortDates.size() == 0)
		{
			saShortDates = invariant.saShortDates;
		}
		if (saShortTimes.size() == 0)
		{
			saShortTimes = invariant.saShortTimes;
		}
		if (saYearMonths.size() == 0)
		{
			saYearMonths = invariant.saYearMonths;
		}
		if (saLongDates.size() == 0)
		{
			saLongDates = invariant.saLongDates;
		}
		if (saLongTimes.size() == 0)
		{
			saLongTimes = invariant.saLongDates;
		}
		if (sMonthDay.empty())
		{
			sMonthDay = invariant.sMonthDay;
		}
		if (saEraNames.size() == 0)
		{
			saEraNames = invariant.saEraNames;
		}
		if (saAbbrevEraNames.size() == 0)
		{
			saAbbrevEraNames = invariant.saAbbrevEraNames;
		}
		if (saAbbrevEnglishEraNames.size() == 0)
		{
			saAbbrevEnglishEraNames = invariant.saAbbrevEnglishEraNames;
		}
		if (saDayNames.size() == 0)
		{
			saDayNames = invariant.saDayNames;
		}
		if (saAbbrevDayNames.size() == 0)
		{
			saAbbrevDayNames = invariant.saAbbrevDayNames;
		}
		if (saSuperShortDayNames.size() == 0)
		{
			saSuperShortDayNames = invariant.saSuperShortDayNames;
		}
		if (saMonthNames.size() == 0)
		{
			saMonthNames = invariant.saMonthNames;
		}
		if (saAbbrevMonthNames.size() == 0)
		{
			saAbbrevMonthNames = invariant.saAbbrevMonthNames;
		}
	}
	saShortDates = CultureData::ReescapeWin32wstrings(saShortDates);
	saLongDates = CultureData::ReescapeWin32wstrings(saLongDates);
	saYearMonths = CultureData::ReescapeWin32wstrings(saYearMonths);
	sMonthDay = CultureData::ReescapeWin32wstring(sMonthDay);
	if ((UShort)calendarId == 4)
	{
		if (!CultureInfo_impl::IsTaiwanSku())
		{
			sNativeName = L"";
		}
		else
		{
			sNativeName = L"中華民國曆";
		}
	}
	if (saMonthGenitiveNames.size()==0 || saMonthGenitiveNames[0].empty())
	{
		saMonthGenitiveNames = saMonthNames;
	}
	if (saAbbrevMonthGenitiveNames.size() == 0 || saAbbrevMonthGenitiveNames[0].empty())
	{
		saAbbrevMonthGenitiveNames = saAbbrevMonthNames;
	}
	if (saLeapYearMonthNames.size() == 0 || saLeapYearMonthNames[0].empty())
	{
		saLeapYearMonthNames = saMonthNames;
	}
	InitializeEraNames(localeName, calendarId);
	InitializeAbbreviatedEraNames(localeName, calendarId);
	if (calendarId != 3)
	{
		saAbbrevEnglishEraNames = wstringvector{ L"" };
	}
	else
	{
		//saAbbrevEnglishEraNames = JapaneseCalendar.EnglishEraNames();
	}
	iCurrentEra = (int)saEraNames.size();
}

namespace System
{
	class cnb38qcfds
	{
	private:
		std::string _name;
		int _id;
	public:
		explicit cnb38qcfds(const std::string& name, int calendarId) :_name(name), _id(calendarId)
		{
		}
		bool operator() (CalendarData* info)
		{
			try
			{
				if (!info) return false;
				auto iname = info->Name;
				if (StringHelper::Compare(_name, info->Name, StringComparison::InvariantCulture) == 0)
					return  _id == info->CalId;
				return false;
			}
			catch (...)
			{
				return false;
			}
		}
	};
}


CalendarData* CalendarData::Create(const std::string& localeName, int calendarId, bool bUseUser)
{
	if (bUseUser) return new CalendarData(localeName, calendarId, bUseUser);

	std::lock_guard<CriticalSection> css(cs);
	auto iter = std::find_if(globals.begin(), globals.end(), cnb38qcfds(localeName, calendarId));
	if (iter != globals.end()) return *iter;

	CalendarData* data = new CalendarData(localeName, calendarId, bUseUser);

	globals.push_back(data);

	return data;
}

std::string CalendarData::CalendarIdToCultureName(int calendarId)
{
	switch (calendarId)
	{
	case 2:
	{
		return "fa-IR";
	}
	case 3:
	{
		return "ja-JP";
	}
	case 4:
	{
		return "zh-TW";
	}
	case 5:
	{
		return "ko-KR";
	}
	case 6:
	case 10:
	case 23:
	{
		return "ar-SA";
	}
	case 7:
	{
		return "th-TH";
	}
	case 8:
	{
		return "he-IL";
	}
	case 9:
	{
		return "ar-DZ";
	}
	case 11:
	case 12:
	{
		return "ar-IQ";
	}
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	{
		return "en-US";
	}
	default:
	{
		return "en-US";
	}
	}
}

int CalendarData::FindUnescapedCharacter(const std::wstring& s, wchar_t charToFind)
{
	bool flag = false;
	int length = s.length();
	for (int i = 0; i < length; i++)
	{
		wchar_t chr = s[i];
		wchar_t chr1 = chr;
		if (chr1 == '\'')
		{
			flag = !flag;
		}
		else if (chr1 == '\\')
		{
			i++;
		}
		else if (!flag && charToFind == chr)
		{
			return i;
		}
	}
	return -1;
}

void CalendarData::FixupWin7MonthDaySemicolonBug()
{
	int num = FindUnescapedCharacter(sMonthDay, ';');
	if (num > 0)
	{
		sMonthDay = sMonthDay.substr(0, num);
	}
}

CalendarData* CalendarData::GetCalendarData(int calendarId)
{
	return nullptr;
	//return CultureInfo(CalendarIdToCultureName(calendarId).c_str()).GetCalendar(calendarId);
}

void CalendarData::InitializeAbbreviatedEraNames(const std::string& localeName, int calendarId)
{
	wstringvector strArrays;
	wstringvector strArrays1;
	wstringvector strArrays2;
	CalendarId calendarId1 = (CalendarId)((UShort)calendarId);
	switch (calendarId1)
	{
	case CalendarId::GREGORIAN:
	{
		if (saAbbrevEraNames.size() != 0 && saAbbrevEraNames.size() != 0 && !saAbbrevEraNames[0].empty())
		{
			break;
		}
		saAbbrevEraNames = wstringvector{ L"AD" };
		return;
	}
	case CalendarId::GREGORIAN_US:
	{
		strArrays = wstringvector{ L"AD" };
		saAbbrevEraNames = strArrays;
		return;
	}
	case CalendarId::JAPAN:
	{
		//saAbbrevEraNames = JapaneseCalendar.AbbrevEraNames();
		return;
	}
	case CalendarId::TAIWAN:
	{
		saAbbrevEraNames = wstringvector(1);
		if (saEraNames[0].length() != 4)
		{
			saAbbrevEraNames[0] = saEraNames[0];
			return;
		}
		saAbbrevEraNames[0] = saEraNames[0].substr(2, 2);
		return;
	}
	case CalendarId::KOREA:
	{
	Label3:
		saAbbrevEraNames = saEraNames;
		break;
	}
	case CalendarId::HIJRI:
	{
		if (localeName == "dv-MV")
		{
			strArrays1 = wstringvector{ L"ހ." };
			saAbbrevEraNames = strArrays1;
			return;
		}
		strArrays2 = wstringvector{ L"هـ" };
		saAbbrevEraNames = strArrays2;
		return;
	}
	default:
	{
		switch (calendarId1)
		{
		case CalendarId::JULIAN:
		{
			strArrays = wstringvector{ L"AD" };
			saAbbrevEraNames = strArrays;
			return;
		}
		case CalendarId::JAPANESELUNISOLAR:
		{
			//saAbbrevEraNames = JapaneseCalendar.AbbrevEraNames();
			return;
		}
		default:
		{
			if (calendarId1 == CalendarId::UMALQURA)
			{
				if (localeName == "dv-MV")
				{
					strArrays1 = wstringvector{ L"ހ." };
					saAbbrevEraNames = strArrays1;
					return;
				}
				strArrays2 = wstringvector{ L"هـ" };
				saAbbrevEraNames = strArrays2;
				return;
			}
			goto Label3;
		}
		}
		break;
	}
	}
}

void CalendarData::InitializeEraNames(const std::string& localeName, int calendarId)
{
	switch ((UShort)calendarId)
	{
	case 1:
	{
		if (saEraNames.size() != 0 && saEraNames.size() != 0 && !saEraNames[0].empty())
		{
			break;
		}
		saEraNames = wstringvector{ L"A.D." };
		return;
	}
	case 2:
	case 13:
	{
		saEraNames = wstringvector{ L"A.D." };
		return;
	}
	case 3:
	case 14:
	{
		//saEraNames = JapaneseCalendar.EraNames();
		return;
	}
	case 4:
	{
		if (CultureInfo_impl::IsTaiwanSku())
		{
			saEraNames = wstringvector{ L"中華民國" };
			return;
		}
		saEraNames = wstringvector{ L"" };
		return;
	}
	case 5:
	{
		saEraNames = wstringvector{ L"단기" };
		return;
	}
	case 6:
	case 23:
	{
		if (localeName == "dv-MV")
		{
			saEraNames = wstringvector{ L"ހިޖްރީ" };
			return;
		}
		saEraNames = wstringvector{ L"بعد الهجرة" };
		return;
	}
	case 7:
	{
		saEraNames = wstringvector{ L"พ.ศ." };
		return;
	}
	case 8:
	{
		saEraNames = wstringvector{ L"C.E." };
		return;
	}
	case 9:
	{
		saEraNames = wstringvector{ L"ap. J.-C." };
		return;
	}
	case 10:
	case 11:
	case 12:
	{
		saEraNames = wstringvector{ L"م" };
		return;
	}
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	default:
	{
		saEraNames = GetInvariant().saEraNames;
		break;
	}
	}
}

int CalendarData::GetTwoDigitYearMax(int calID)
{
	API_UINT32 val;
	Microsoft::winapi_GetCalendarInfo(NULL, calID, NULL, API_CAL_RETURN_NUMBER, NULL, NULL, &val);
	return (int)val;
}