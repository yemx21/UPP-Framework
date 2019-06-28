#include "DateTime.h"
#include "DateTimeFormat.h"
#include "Calendar.h"
#include "CultureInfo_impl.h"
#include "CalendarData.h"
#include "TimeZoneInfo.h"
#include "TimeZoneInfo_impl.h"
using namespace System;

const std::wstring DateTimeFormat::RoundtripFormat = L"yyyy'-'MM'-'dd'T'HH':'mm':'ss.fffffffK";
const std::wstring DateTimeFormat::RoundtripDateTimeUnfixed = L"yyyy'-'MM'-'ddTHH':'mm':'ss zzz";
const std::wstring DateTimeFormat::RFC1123Pattern = L"ddd, dd MMM yyyy HH':'mm':'ss 'GMT'";
const std::wstring DateTimeFormat::SortableDateTimePattern = L"yyyy'-'MM'-'dd'T'HH':'mm':'ss";
const std::wstring DateTimeFormat::UniversalSortableDateTimePattern = L"yyyy'-'MM'-'dd HH':'mm':'ss'Z'";

static std::wstring HebrewToString(int Number)
{
	wchar_t cTens = '\0';
	wchar_t cUnits;          
	int Hundreds, Tens;          
	std::wstring szHebrew = std::wstring();

	if (Number > 5000) 
	{
		Number -= 5000;
	}

	Hundreds = Number / 100;

	if (Hundreds > 0) {
		Number -= Hundreds * 100;
		// \u05e7 = 100
		// \u05e8 = 200
		// \u05e9 = 300
		// \u05ea = 400
		// If the number is greater than 400, use the multiples of 400.
		for (int i = 0; i < (Hundreds / 4); i++) {
			szHebrew.push_back((wchar_t)L'\x05ea');
		}

		int remains = Hundreds % 4;
		if (remains > 0) {
			szHebrew.push_back((wchar_t)((int)L'\x05e6' + remains));
		}
	}

	Tens = Number / 10;
	Number %= 10;

	switch (Tens) {
	case (0) :
		cTens = '\0';
		break;
	case (1) :
		cTens = L'\x05d9';          // Hebrew Letter Yod
		break;
	case (2) :
		cTens = L'\x05db';          // Hebrew Letter Kaf
		break;
	case (3) :
		cTens = L'\x05dc';          // Hebrew Letter ----d
		break;
	case (4) :
		cTens = L'\x05de';          // Hebrew Letter Mem
		break;
	case (5) :
		cTens = L'\x05e0';          // Hebrew Letter Nun
		break;
	case (6) :
		cTens = L'\x05e1';          // Hebrew Letter Samekh
		break;
	case (7) :
		cTens = L'\x05e2';          // Hebrew Letter Ayin
		break;
	case (8) :
		cTens = L'\x05e4';          // Hebrew Letter Pe
		break;
	case (9) :
		cTens = L'\x05e6';          // Hebrew Letter Tsadi
		break;
	}

	cUnits = (wchar_t)(Number > 0 ? ((int)L'\x05d0' + Number - 1) : 0);

	if ((cUnits == L'\x05d4') &&            // Hebrew Letter He  (5)
		(cTens == L'\x05d9')) {              // Hebrew Letter Yod (10)
		cUnits = L'\x05d5';                 // Hebrew Letter Vav (6)
		cTens = L'\x05d8';                 // Hebrew Letter Tet (9)
	}

	if ((cUnits == L'\x05d5') &&            // Hebrew Letter Vav (6)
		(cTens == L'\x05d9')) {               // Hebrew Letter Yod (10)
		cUnits = L'\x05d6';                 // Hebrew Letter Zayin (7)
		cTens = L'\x05d8';                 // Hebrew Letter Tet (9)
	}

	if (cTens != '\0') {
		szHebrew.push_back(cTens);
	}

	if (cUnits != '\0') {
		szHebrew.push_back(cUnits);
	}

	if (szHebrew.length() > 1) {
		szHebrew.insert(szHebrew.length() - 1, 1, '"');
	}
	else {
		szHebrew.push_back('\'');
	}

	return (szHebrew);
}

int DateTimeFormat::ParseNextChar(std::wstring format, size_t pos)
{
	if ((size_t)pos >= format.size() - 1)
	{
		return -1;
	}
	return format[pos + 1];
}

int DateTimeFormat::ParseQuoteString(std::wstring format, size_t po, std::wstring* result)
{
	if (!result) return -1;
	int formatLen = (int)format.length();
	int pos = (int)po;
	int beginPos = pos;
	wchar_t quoteChar = format[pos++];

	bool foundQuote = false;
	while (pos < formatLen)
	{
		wchar_t ch = format[pos++];
		if (ch == quoteChar)
		{
			foundQuote = true;
			break;
		}
		else if (ch == '\\')
		{
			if (pos < formatLen)
			{
				result->push_back(format[pos++]);
			}
			else
			{
				throw std::exception("Format_InvalidString");
			}
		}
		else
		{
			result->push_back(ch);
		}
	}

	if (!foundQuote)
	{
		throw std::exception("Format_BadQuote");
	}


	return (pos - beginPos);
}

int DateTimeFormat::ParseRepeatPattern(std::wstring format, size_t pos, wchar_t patternChar)
{
	auto length = format.size();
	auto num = pos + 1;
	while (num < length && format[num] == patternChar)
	{
		num++;
	}
	return (int)(num - pos);
}

void DateTimeFormat::FormatDigits(std::wstring* outputBuffer, int value, int len, bool overrideLengthLimit)
{
	if (!outputBuffer) return;

	if (!overrideLengthLimit && len > 2)
		len = 2;
	wchar_t chPtr1[16];
	memset(chPtr1, 0, sizeof(wchar_t) * 16);
	wchar_t* chPtr2 = chPtr1 + 16;
	int num = value;
	do
	{
		*--chPtr2 = (wchar_t)(num % 10 + 48);
		num /= 10;
	} while (num != 0 && chPtr2 > chPtr1);
	int valueCount;
	for (valueCount = (int)(chPtr1 + 16 - chPtr2); valueCount < len && chPtr2 > chPtr1; ++valueCount)
		*--chPtr2 = '0';

	outputBuffer->append(chPtr2, valueCount);
}

void DateTimeFormat::HebrewFormatDigits(std::wstring* outputBuffer, int digits)
{
	if (!outputBuffer) return;
	outputBuffer->append(HebrewToString(digits));
}

std::wstring DateTimeFormat::GetRealFormat(std::wstring format, const CultureInfo& loc)
{
	std::wstring realFormat = std::wstring();
	switch (format[0])
	{
	case 'd':       // Short Date
		realFormat = loc.GetPattern(CultureInfoPatterns::ShortDatePattern); 
		break;
	case 'D':       // Long Date
		realFormat = loc.GetPattern(CultureInfoPatterns::LongDatePattern);
		break;
	case 'f':       // Full (long date + short time)
		realFormat = loc.GetPattern(CultureInfoPatterns::FullLongDateShortTimePattern);
		break;
	case 'F':       // Full (long date + long time)
		realFormat = loc.GetPattern(CultureInfoPatterns::FullLongDateLongTimePattern);
		break;
	case 'g':       // General (short date + short time)
		realFormat = loc.GetPattern(CultureInfoPatterns::GeneralShortDateShortTimePattern);
		break;
	case 'G':       // General (short date + long time)
		realFormat = loc.GetPattern(CultureInfoPatterns::GeneralShortDateLongTimePattern);
		break;
	case 'm':
	case 'M':       // Month/Day Date
		realFormat = loc.GetPattern(CultureInfoPatterns::MonthDayPattern);
		break;
	case 'o':
	case 'O':
		realFormat = RoundtripFormat;
		break;
	case 'r':
	case 'R':       // RFC 1123 Standard                    
		realFormat = RFC1123Pattern;
		break;
	case 's':       // Sortable without Time Zone Info
		realFormat = SortableDateTimePattern;
		break;
	case 't':       // Short Time
		realFormat = loc.GetPattern(CultureInfoPatterns::ShortTimePattern);
		break;
	case 'T':       // Long Time
		realFormat = loc.GetPattern(CultureInfoPatterns::LongTimePattern);
		break;
	case 'u':       // Universal with Sortable format
		realFormat = UniversalSortableDateTimePattern;
		break;
	case 'U':       // Universal with Full (long date + long time) format
		realFormat = loc.GetPattern(CultureInfoPatterns::FullLongDateLongTimePattern);
		break;
	case 'y':
	case 'Y':       // Year/Month Date
		realFormat = loc.GetPattern(CultureInfoPatterns::YearMonthPattern);
		break;
	}
	return (realFormat);
}

std::wstring DateTimeFormat::GetRealFormat(std::wstring format)
{
	std::wstring realFormat = std::wstring();
	switch (format[0])
	{
	case 'd':       // Short Date
		realFormat = L"MM/dd/yyyy";
		break;
	case 'D':       // Long Date
		realFormat = L"dddd, dd MMMM yyyy";
		break;
	case 'f':       // Full (long date + short time)
		realFormat = L"dddd, dd MMMM yyyy HH:mm";
		break;
	case 'F':       // Full (long date + long time)
		realFormat = L"dddd, dd MMMM yyyy HH:mm::ss";
		break;
	case 'g':       // General (short date + short time)
		realFormat = L"MM/dd/yyyy HH:mm";
		break;
	case 'G':       // General (short date + long time)
		realFormat = L"MM/dd/yyyy HH:mm::ss";
		break;
	case 'm':
	case 'M':       // Month/Day Date
		realFormat = L"MMMM dd";
		break;
	case 'o':
	case 'O':
		realFormat = RoundtripFormat;
		break;
	case 'r':
	case 'R':       // RFC 1123 Standard                    
		realFormat = RFC1123Pattern;
		break;
	case 's':       // Sortable without Time Zone Info
		realFormat = SortableDateTimePattern;
		break;
	case 't':       // Short Time
		realFormat = L"HH:mm";
		break;
	case 'T':       // Long Time
		realFormat = L"HH:mm::ss";
		break;
	case 'u':       // Universal with Sortable format
		realFormat = UniversalSortableDateTimePattern;
		break;
	case 'U':       // Universal with Full (long date + long time) format
		realFormat = L"MM/dd/yyyy  HH:mm::ss";
		break;
	case 'y':
	case 'Y':       // Year/Month Date
		realFormat = L"yyyy MMMM";
		break;
	}
	return (realFormat);
}

std::wstring DateTimeFormat::ExpandPredefinedFormat(std::wstring format, DateTime& dateTime, const CultureInfo& loc, TimeSpan& offset, bool& useInvariant)
{
	useInvariant = false;
	bool greogrian = false;
	switch (format[0]) 
	{
	case 'o':
	case 'O':       // Round trip format
		useInvariant = true;
		break;
	case 'r':
	case 'R':       // RFC 1123 Standard                    
		if (offset.span != 0LL) {
			// Convert to UTC invariants mean this will be in range
			dateTime = DateTime(dateTime.Ticks - offset.span);
		}
		useInvariant = true;
		break;
	case 's':       // Sortable without Time Zone Info                
		useInvariant = true;
		break;
	case 'u':       // Universal time in sortable format.
		if (offset.span != 0LL) {
			// Convert to UTC invariants mean this will be in range
			dateTime = DateTime(dateTime.Ticks - offset.span);
		}
		
		useInvariant = true;
		break;
	case 'U':       // Universal time in culture dependent format.
		if (offset.span != 0LL) {
			// This format is not supported by DateTimeOffset
			throw std::exception("Format_InvalidString");
		}
		// Universal time is always in Greogrian calendar.
		//
		// Change the Calendar to be Gregorian Calendar.
		//
		greogrian = true;
		dateTime = dateTime.ToUniversalTime();
		break;
	}

	if (useInvariant)
	{
		format = GetRealFormat(format, CultureInfo::Invariant());
		return format;
	}
	if (greogrian)
	{
		format = GetRealFormat(format); 
		return format;
	}
	format = GetRealFormat(format, loc);
	return format;
}

std::wstring DateTimeFormat::FormatDayOfWeek(int dayOfWeek, int repeat, const CultureInfo& loc)
{
	if (repeat == 3)
		return loc.GetConstsName((CultureInfoConstNames)(((std::underlying_type<CultureInfoConstNames>::type)dayOfWeek + 7)));
	else
		return loc.GetConstsName((CultureInfoConstNames)((std::underlying_type<CultureInfoConstNames>::type)dayOfWeek));
}

std::wstring DateTimeFormat::FormatMonth(int month, int repeat, const CultureInfo& loc)
{
	if (repeat == 3)
		return loc.GetConstsName((CultureInfoConstNames)((std::underlying_type<CultureInfoConstNames>::type)month + 26));
	else
		return loc.GetConstsName((CultureInfoConstNames)((std::underlying_type<CultureInfoConstNames>::type)month + 14));
}

std::wstring DateTimeFormat::FormatHebrewMonthName(const DateTime& time, int month, int repeatCount, const CultureInfo& loc)
{
	const Calendar* cal = loc.Calendar;
	if (cal)
		if (cal->IsLeapYear(cal->GetYear(time)) )
		{
			return loc.GetConstsName((CultureInfoConstNames)(61 + month));
		}

	if (month >= 7) {
		month++;
	}
	if (repeatCount == 3) {
		return loc.GetConstsName((CultureInfoConstNames)(25 + month));
	}
	return loc.GetConstsName((CultureInfoConstNames)(13 + month));
}

bool DateTimeFormat::IsUseGenitiveForm(std::wstring format, int index, int tokenLen, wchar_t patternToMatch)
{
	int num1 = 0;
	int index1 = index - 1;
	while (index1 >= 0 && (int)format[index1] != (int)patternToMatch)
		--index1;
	if (index1 >= 0)
	{
		while (--index1 >= 0 && (int)format[index1] == (int)patternToMatch)
			++num1;
		if (num1 <= 1)
			return true;
	}
	size_t index2 = index + tokenLen;
	while (index2 < format.size() && (int)format[index2] != (int)patternToMatch)
		++index2;
	if (index2 < format.size())
	{
		int num2 = 0;
		while (++index2 < format.size() && (int)format[index2] == (int)patternToMatch)
			++num2;
		if (num2 <= 1)
			return true;
	}
	return false;
}

std::wstring DateTimeFormat::FormatCustomized(const DateTime& dateTime, std::wstring format, const CultureInfo& loc, const TimeSpan& offset)
{
	std::wstring stringBuilder = std::wstring();
	stringBuilder.reserve(16);

	const Calendar* cal = loc.Calendar;

	bool isHebrewCalendar = cal != nullptr ? cal->GetID() == 8 : false;
	bool timeOnly = true;

	int i = 0;
	int tokenLen, hour12;

	while (i < (int)format.length())
	{
		wchar_t ch = format[i];
		int nextChar;
		switch (ch)
		{
		case 'g':
			tokenLen = ParseRepeatPattern(format, i, ch);
			stringBuilder.append(loc.GetConstsName(CultureInfoConstNames::Era, cal != nullptr ? cal->GetEra(dateTime) - 1 : 0));
			break;
		case 'h':
			tokenLen = ParseRepeatPattern(format, i, ch);
			hour12 = dateTime.Hour % 12;
			if (hour12 == 0)
			{
				hour12 = 12;
			}
			FormatDigits(&stringBuilder, hour12, tokenLen);
			break;
		case 'H':
			tokenLen = ParseRepeatPattern(format, i, ch);
			FormatDigits(&stringBuilder, dateTime.Hour, tokenLen);
			break;
		case 'm':
			tokenLen = ParseRepeatPattern(format, i, ch);
			FormatDigits(&stringBuilder, dateTime.Minute, tokenLen);
			break;
		case 's':
			tokenLen = ParseRepeatPattern(format, i, ch);
			FormatDigits(&stringBuilder, dateTime.Second, tokenLen);
			break;
		case 'f':
		case 'F':
			tokenLen = ParseRepeatPattern(format, i, ch);
			if (tokenLen <= 7)
			{
				long long fraction = (dateTime.Ticks % TimeSpan::SECONDS);
				fraction = fraction / (long long)pow(10, 7 - tokenLen);
				if (ch == 'f')
				{
					FormatDigits(&stringBuilder, (int)fraction, tokenLen);
				}
				else
				{
					int effectiveDigits = tokenLen;
					while (effectiveDigits > 0) {
						if (fraction % 10 == 0) {
							fraction = fraction / 10;
							effectiveDigits--;
						}
						else {
							break;
						}
					}
					if (effectiveDigits > 0)
					{
						FormatDigits(&stringBuilder, (int)fraction, effectiveDigits);
					}
					else
					{
						if (stringBuilder.length() > 0 && stringBuilder[stringBuilder.length() - 1] == '.')
						{
							stringBuilder.pop_back();
						}
					}
				}
			}
			else
			{
				throw std::exception("Format_InvalidString", 0);
			}
			break;
		case 't':
			tokenLen = ParseRepeatPattern(format, i, ch);
			if (tokenLen == 1)
			{
				if (dateTime.Hour < 12)
				{
					auto am = loc.GetConstsName(CultureInfoConstNames::AMDesignator, 0);
					if (am)
					{
						if (wcsnlen_s(am, 2)>1)
							stringBuilder.push_back(am[0]);
					}
				}
				else
				{
					auto pm = loc.GetConstsName(CultureInfoConstNames::PMDesignator, 0);
					if (pm)
					{
						if (wcsnlen_s(pm, 2) > 1)
							stringBuilder.push_back(pm[0]);
					}
				}
			}
			else
			{
				if (dateTime.Hour < 12)
				{
					auto am = loc.GetConstsName(CultureInfoConstNames::AMDesignator, 0);
					if (am)
					{
						stringBuilder.append(am);
					}
				}
				else
				{
					auto pm = loc.GetConstsName(CultureInfoConstNames::PMDesignator, 0);
					if (pm)
					{
						stringBuilder.append(pm);
					}
				}
			}
			break;
		case 'd':
			tokenLen = ParseRepeatPattern(format, i, ch);
			if (tokenLen <= 2)
			{
				if (cal)
				{
					int day = cal->GetDayOfMonth(dateTime);
					if (isHebrewCalendar) {
						// For Hebrew calendar, we need to convert numbers to Hebrew text for yyyy, MM, and dd values.
						HebrewFormatDigits(&stringBuilder, day);
					}
					else {
						FormatDigits(&stringBuilder, day, tokenLen);
					}
				}
			}
			else
			{
				if (cal)
				{
					int dayOfWeek = (int)cal->GetDayOfWeek(dateTime);
					stringBuilder.append(FormatDayOfWeek(dayOfWeek, tokenLen, loc));
				}
			}
			timeOnly = false;
			break;
		case 'M':
			// 
			// tokenLen == 1 : Month as digits with no leading zero.
			// tokenLen == 2 : Month as digits with leading zero for single-digit months.
			// tokenLen == 3 : Month as a three-letter abbreviation.
			// tokenLen >= 4 : Month as its full name.
			//
			tokenLen = ParseRepeatPattern(format, i, ch);
			if (cal)
			{
				int month = cal->GetMonth(dateTime);
				if (tokenLen <= 2)
				{
					if (isHebrewCalendar) {
						// For Hebrew calendar, we need to convert numbers to Hebrew text for yyyy, MM, and dd values.
						HebrewFormatDigits(&stringBuilder, month);
					}
					else {
						FormatDigits(&stringBuilder, month, tokenLen);
					}
				}
				else {
					if (isHebrewCalendar) {
						stringBuilder.append(FormatHebrewMonthName(dateTime, month, tokenLen, loc));
					}
					else
					{
						if (((loc.GetInternalValue(0) & (int)DateTimeFormatFlags::UseGenitiveMonth) != 0) && tokenLen >= 4)
						{
							if (!IsUseGenitiveForm(format, i, tokenLen, 'd'))
								loc.GetConstsName((CultureInfoConstNames)(13 + month));
							else
								loc.GetConstsName((CultureInfoConstNames)(37 + month));
						}
						else {
							stringBuilder.append(FormatMonth(month, tokenLen, loc));
						}
					}
				}
			}
			timeOnly = false;
			break;
		case 'y':
			// Notes about OS behavior:
			// y: Always print (year % 100). No leading zero.
			// yy: Always print (year % 100) with leading zero.
			// yyy/yyyy/yyyyy/... : Print year value.  No leading zero.
			if (cal)
			{
				int year = cal->GetYear(dateTime);
				tokenLen = ParseRepeatPattern(format, i, ch);
				if (loc.GetInternalValue(1) == 1)
				{
					FormatDigits(&stringBuilder, year, tokenLen <= 2 ? tokenLen : 2);
				}
				else if (cal->GetID() == 8)
				{
					HebrewFormatDigits(&stringBuilder, year);
				}
				else {
					if (tokenLen <= 2)
					{
						FormatDigits(&stringBuilder, year % 100, tokenLen);
					}
					else
					{
						FormatDigits(&stringBuilder, year, tokenLen);
					}
				}
			}
			timeOnly = false;
			break;
		case 'z':
			tokenLen = ParseRepeatPattern(format, i, ch);
			FormatCustomizedTimeZone(dateTime, offset, format, tokenLen, timeOnly, &stringBuilder);
			break;
		case 'K':
			tokenLen = 1;
			FormatCustomizedRoundripTimeZone(dateTime, offset, &stringBuilder);
			break;
		case ':':
			stringBuilder.append(loc.GetConstsName(CultureInfoConstNames::TimeSeparator));
			tokenLen = 1;
			break;
		case '/':
			stringBuilder.append(loc.GetConstsName(CultureInfoConstNames::DateSeparator));
			tokenLen = 1;
			break;
		case '\'':
		case '\"':
		{
			std::wstring enquotedString = std::wstring();
			tokenLen = ParseQuoteString(format, i, &enquotedString);
			stringBuilder.append(enquotedString);
		}
			break;
		case '%':
			// Optional format character.
			// For example, format string "%d" will print day of month 
			// without leading zero.  Most of the cases, "%" can be ignored.
			nextChar = ParseNextChar(format, i);
			// nextChar will be -1 if we already reach the end of the format string.
			// Besides, we will not allow "%%" appear in the pattern.
			if (nextChar >= 0 && nextChar != (int)'%') 
			{
				std::wstring tmp = std::wstring();
				tmp.push_back((wchar_t)nextChar);
				stringBuilder.append(FormatCustomized(dateTime, tmp, loc, offset));
				tokenLen = 2;
			}
			else
			{
				//
				// This means that '%' is at the end of the format string or
				// "%%" appears in the format string.
				//
				throw std::exception("Format_InvalidString");
			}
			break;
		case '\\':
			// Escaped character.  Can be used to insert character into the format string.
			// For exmple, "\d" will insert the character 'd' into the string.
			//
			// NOTENOTE : we can remove this format character if we enforce the enforced quote 
			// character rule.
			// That is, we ask everyone to use single quote or double quote to insert characters,
			// then we can remove this character.
			//
			nextChar = ParseNextChar(format, i);
			if (nextChar >= 0)
			{
				stringBuilder.push_back(((wchar_t)nextChar));
				tokenLen = 2;
			}
			else
			{
				//
				// This means that '\' is at the end of the formatting string.
				//
				throw std::exception("Format_InvalidString");
			}
			break;
		default:
			// NOTENOTE : we can remove this rule if we enforce the enforced quote
			// character rule.
			// That is, if we ask everyone to use single quote or double quote to insert characters,
			// then we can remove this default block.
			stringBuilder.push_back(ch);
			tokenLen = 1;
			break;
		}
		i += tokenLen;
	}
	return stringBuilder;
}

void DateTimeFormat::FormatCustomizedRoundripTimeZone(const DateTime& dateTime, const TimeSpan& offset, std::wstring* result)
{
	if (!result) return;
	TimeSpan off = offset;
	if (dateTime.Kind == DateTimeKind::Utc)
	{
		result->push_back('Z');
		return;
	}
	else if (dateTime.Kind == DateTimeKind::Local)
	{
		off = TimeZoneInfo_impl::GetLocalUtcOffset(dateTime);
		if (off.span < 0LL)
		{
			result->push_back('-');
			off = off.GetNegate();
		}
		else
		{
			result->push_back('+');
		}
		FormatDigits(result, off.Hours, 2);
		result->push_back(':');
		FormatDigits(result, off.Minutes, 2);
	}
}
void DateTimeFormat::FormatCustomizedTimeZone(const DateTime& dateTime, const TimeSpan& offset, std::wstring format, int tokenLen, bool timeOnly, std::wstring* result)
{
	TimeSpan off=offset;

	if (off.span == 0)
	{
		if (timeOnly && dateTime.Ticks < TimeSpan::DAYS)
			off = TimeZoneInfo_impl::GetLocalUtcOffset(DateTime::Now());
		else if (dateTime.Kind == DateTimeKind::Utc)
		{
			DateTime dt = DateTime::SpecifyKind(dateTime, DateTimeKind::Local);
			off = TimeZoneInfo_impl::GetLocalUtcOffset(dt);
		}
		else
			off = TimeZoneInfo_impl::GetLocalUtcOffset(dateTime);
	}

	if (offset.span >= 0LL)
	{
		result->push_back('+');
	}
	else
	{
		result->push_back('-');
		off = off.GetNegate();
	}

	if (tokenLen <= 1)
	{
		FormatDigits(result, off.Hours, 1);
	}
	else
	{
		FormatDigits(result, off.Hours, 2);
		if (tokenLen < 3) return;
		FormatDigits(result, off.Minutes, 2);
	}
}

std::wstring DateTimeFormat::Format(const DateTime& dateTime, std::wstring format, const CultureInfo& loc, const TimeSpan& offset)
{
	bool useinvariant = false;
	DateTime dt = dateTime;
	TimeSpan off = offset;
	if (format.empty() || format.size() == 0)
	{
		bool timeOnlySpecialCase = false;

		const Calendar* cal = loc.Calendar;
		if (cal)
		{
			switch (cal->GetID())
			{
			case 3:
			case 4:
			case 6:
			case 8:
			case 13:
			case 23:
				timeOnlySpecialCase = true;
				useinvariant = true;
				break;
			}
		}
		if (off.span == 0LL) {
			// Default DateTime.ToString case.
			if (timeOnlySpecialCase) {
				format = L"s";
			}
			else {
				format = L"G";
			}
		}
		else {
			// Default DateTimeOffset.ToString case.
			if (timeOnlySpecialCase) {
				format = RoundtripDateTimeUnfixed;
			}
			else {
				format = loc.GetPattern(CultureInfoPatterns::DateTimeOffsetPattern);
			}

		}
	}
	bool useInvariant = false;
	if (format.size() == 1)
		format = ExpandPredefinedFormat(format, dt, loc, off, useInvariant);
	return FormatCustomized(dt, format, useInvariant? CultureInfo::Invariant() : loc, off);
}