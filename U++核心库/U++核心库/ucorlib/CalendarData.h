#pragma once
#include "Calendar.h"
#include "CriticalSection.h"
namespace System
{
	ENUM_OPERATORSEX(CalendarId,UShort)
	enum class CalendarId : UShort
	{
		GREGORIAN = 1,
		GREGORIAN_US = 2,
		JAPAN = 3,
		TAIWAN = 4,
		KOREA = 5,
		HIJRI = 6,
		THAI = 7,
		HEBREW = 8,
		GREGORIAN_ME_FRENCH = 9,
		GREGORIAN_ARABIC = 10,
		GREGORIAN_XLIT_ENGLISH = 11,
		GREGORIAN_XLIT_FRENCH = 12,
		JULIAN = 13,
		JAPANESELUNISOLAR = 14,
		CHINESELUNISOLAR = 15,
		SAKA = 16,
		LUNAR_ETO_CHN = 17,
		LUNAR_ETO_KOR = 18,
		LUNAR_ETO_ROKUYOU = 19,
		KOREANLUNISOLAR = 20,
		TAIWANLUNISOLAR = 21,
		PERSIAN = 22,
		LAST_CALENDAR = 23,
		UMALQURA = 23
	};

	class CalendarData
	{
	public:
		static const int MAX_CALENDARS = 23;

		static std::vector<CalendarData*> globals;

		std::string Name;

		int CalId;

		static CriticalSection cs;

		std::wstring sNativeName;

		wstringvector saShortDates;

		wstringvector saYearMonths;

		wstringvector saLongDates;

		wstringvector saShortTimes;

		wstringvector saLongTimes;

		std::wstring sMonthDay;

		wstringvector saEraNames;

		wstringvector saAbbrevEraNames;

		wstringvector saAbbrevEnglishEraNames;

		wstringvector saDayNames;

		wstringvector saAbbrevDayNames;

		wstringvector saSuperShortDayNames;

		wstringvector saMonthNames;

		wstringvector saAbbrevMonthNames;

		wstringvector saMonthGenitiveNames;

		wstringvector saAbbrevMonthGenitiveNames;

		wstringvector saLeapYearMonthNames;

		int iTwoDigitYearMax = 2029;

		int iCurrentEra;

		bool bUseUserOverrides;

		static CalendarData& GetInvariant();

		CalendarData();

		CalendarData(const std::string& localeName, int calendarId, bool bUseUserOverrides);

		static CalendarData* Create(const std::string& localeName, int calendarId, bool bUseUserOverrides);

		static std::string CalendarIdToCultureName(int calendarId);

		static int FindUnescapedCharacter(const std::wstring& s, wchar_t charToFind);

		void FixupWin7MonthDaySemicolonBug();

		static CalendarData* GetCalendarData(int calendarId);

		void InitializeAbbreviatedEraNames(const std::string& localeName, int calendarId);

		void InitializeEraNames(const std::string& localeName, int calendarId);

		static bool GetCalendarData(CalendarData* data, const std::string& localeName, int calendar);

		static int GetTwoDigitYearMax(int calID);
	};
}