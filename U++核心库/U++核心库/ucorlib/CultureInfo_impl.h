#pragma once
#include <locale>
#include <locale.h>
#include "Config.h"
#include <string>
#include "Converter.h"
#include "CultureData.h"
#include "StringHelper.h"
#include "CultureInfo.h"
#include <codecvt>
#include "Cloner.h"
#include <xhash>
#include "Calendars.h"


template<class Facet>
struct deletable_facet : Facet
{
	template<class ...Args>
	deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};

struct NumGroup : public std::numpunct<wchar_t>
{
protected:
	std::string grouping;
	wchar_t thousands_sep;

	virtual wchar_t do_thousands_sep() const;
	virtual std::string do_grouping() const;
public:
	NumGroup(const std::locale& loc);
};

namespace System
{
	class EncodingHelper;

	ENUM_OPERATORS(DateTokenType)
	enum class DateTokenType
	{
		// The valid token should start from 1.

		// Regular tokens. The range is from 0x00 ~ 0xff.
		NumberToken = 1,    // The number.  E.g. "12"
		YearNumberToken = 2,    // The number which is considered as year number, which has 3 or more digits.  E.g. "2003"
		Am = 3,    // AM timemark. E.g. "AM"
		Pm = 4,    // PM timemark. E.g. "PM"
		MonthToken = 5,    // A word (or words) that represents a month name.  E.g. "[....]"
		EndOfString = 6,    // End of string
		DayOfWeekToken = 7,    // A word (or words) that represents a day of week name.  E.g. "Monday" or "Mon"
		TimeZoneToken = 8,    // A word that represents a timezone name. E.g. "GMT"
		EraToken = 9,    // A word that represents a era name. E.g. "A.D."
		DateWordToken = 10,   // A word that can appear in a DateTime string, but serves no parsing semantics.  E.g. "de" in Spanish culture.
		UnknownToken = 11,   // An unknown word, which signals an error in parsing.
		HebrewNumber = 12,   // A number that is composed of Hebrew text.  Hebrew calendar uses Hebrew digits for year values, month values, and day values.
		JapaneseEraToken = 13,   // Era name for JapaneseCalendar
		TEraToken = 14,   // Era name for TaiwanCalendar
		IgnorableSymbol = 15,   // A separator like "," that is equivalent to whitespace


		// Separator tokens.
		SEP_Unk = 0x100,         // Unknown separator.
		SEP_End = 0x200,    // The end of the parsing string.
		SEP_Space = 0x300,    // Whitespace (including comma).
		SEP_Am = 0x400,    // AM timemark. E.g. "AM"
		SEP_Pm = 0x500,    // PM timemark. E.g. "PM"
		SEP_Date = 0x600,    // date separator. E.g. "/"
		SEP_Time = 0x700,    // time separator. E.g. ":"
		SEP_YearSuff = 0x800,    // Chinese/Japanese/Korean year suffix.
		SEP_MonthSuff = 0x900,    // Chinese/Japanese/Korean month suffix.
		SEP_DaySuff = 0xa00,    // Chinese/Japanese/Korean day suffix.
		SEP_HourSuff = 0xb00,   // Chinese/Japanese/Korean hour suffix.
		SEP_MinuteSuff = 0xc00,   // Chinese/Japanese/Korean minute suffix.
		SEP_SecondSuff = 0xd00,   // Chinese/Japanese/Korean second suffix.
		SEP_LocalTimeMark = 0xe00,   // 'T', used in ISO 8601 format.
		SEP_DateOrOffset = 0xf00,   // '-' which could be a date separator or start of a time zone offset

		RegularTokenMask = 0x00ff,
		SeparatorTokenMask = 0xff00,
	};

	class DateTokenHashValue
	{
	public:
		std::wstring tokenString;
		DateTokenType tokenType;
		int tokenValue;

		DateTokenHashValue(const std::wstring& tokenStr, DateTokenType tokenType, int tokenValue);
		DateTokenHashValue(const DateTokenHashValue& v);
	};

	ENUM_OPERATORS(DateTimeFormatFlags)
	enum class DateTimeFormatFlags
	{
		None = 0,
		UseGenitiveMonth = 1,
		UseLeapYearMonth = 2,
		UseSpacesInMonthNames = 4,
		UseHebrewRule = 8,
		UseSpacesInDayNames = 16,
		UseDigitPrefixInTokens = 32
	};

	class CultureData;

	class CultureInfo_impl
	{
	public:
		static std::hash<std::string> _hashFun;

		static bool s_haveIsTaiwanSku;
		static bool s_isTaiwanSku;

		std::wstring _name;
		std::locale _locale;
		std::locale _locale_group;
		_locale_t _localeplus;
		unsigned long _lcid;
		bool isInvariant;

		std::vector<DateTokenHashValue*> _dateTokens;

		CultureData* _data;

		Calendar* _defaultCal;

		mutable std::vector<CultureInfo_impl*> _caches;

		int _calId;

		CalendarData* _defaultCalData;

		struct CultureInfo_impl_Compare0
		{
			int cal;

			explicit CultureInfo_impl_Compare0(int id) :cal(id)
			{

			}

			bool operator()(CultureInfo_impl* impl) const
			{ 
				return impl ? impl->_calId == cal : false;
			}
		};

		struct CultureInfo_impl_Compare1
		{
			Calendar* cal;

			explicit CultureInfo_impl_Compare1(Calendar* c) :cal(c)
			{

			}

			bool operator()(CultureInfo_impl* impl) const
			{
				return impl && cal ? impl->_defaultCal == cal : false;
			}
		};

		void InsertAtCurrentHashNode(std::vector<DateTokenHashValue*>& hashTable, const std::wstring& str, wchar_t ch, DateTokenType tokenType, int tokenValue, int pos, int hashcode, int hashProbe);

		void InsertHash(std::vector<DateTokenHashValue*>& hashTable, const std::wstring& str, DateTokenType tokenType, int tokenValue);

		void InsertHash(std::vector<DateTokenHashValue*>& hashTable, const wchar_t* str, DateTokenType tokenType, int tokenValue);

		void AddMonthNames(std::vector<DateTokenHashValue*>& temp, const std::wstring& monthPostfix);

		void CreateTokenHashTable();

		static bool IsTaiwanSku();

		static Calendar* CreateCalendarInstance(int calType);

		CultureInfo_impl(const char* locName);

		CultureInfo_impl(const char* locName, bool noCalInit);

		~CultureInfo_impl();

		CultureInfo_impl* CreateByCalendarId(int id) const;

		CultureInfo_impl* CreateByCalendarId(Calendar* ca) const;

		bool operator ==(const CultureInfo_impl& info) const;
		bool operator !=(const CultureInfo_impl& info) const;


		static bool YearMonthAdjustment(const CultureInfo& loc, int& year, int& month, bool parsedMonthName);

		static CultureInfo_impl* GetImpl(const CultureInfo& loc);
	};
}