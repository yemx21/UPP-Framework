#pragma once
#include "TimeSpan.h"


namespace System
{
	class DateTimeParse;

	class DateTimeFormat
	{
	protected:
		friend class DateTimeParse;
		enum class MonthNameStyles
		{
			Regular,
			Genitive,
			LeapYear,
		};
		friend class DateTime;
		friend class TimeSpanFormat;

		static const std::wstring RoundtripFormat;

		static const std::wstring RoundtripDateTimeUnfixed;

		static const std::wstring RFC1123Pattern;

		static const std::wstring SortableDateTimePattern;

		static const std::wstring UniversalSortableDateTimePattern;

		static int ParseNextChar(std::wstring format, size_t pos);

		static int ParseQuoteString(std::wstring format, size_t pos, std::wstring* result);

		static int ParseRepeatPattern(std::wstring format, size_t pos, wchar_t patternChar);

		static void FormatDigits(std::wstring* outputBuffer, int value, int len, bool overrideLengthLimit = false);

		static void HebrewFormatDigits(std::wstring* outputBuffer, int digits);

		static std::wstring FormatHebrewMonthName(const DateTime& time, int month, int repeatCount, const CultureInfo& loc);

		static std::wstring GetRealFormat(std::wstring format, const CultureInfo& loc);

		static std::wstring GetRealFormat(std::wstring format);

		static std::wstring ExpandPredefinedFormat(std::wstring format, DateTime& dateTime, const CultureInfo& loc, TimeSpan& offset, bool& useInvariant);

		static std::wstring FormatDayOfWeek(int dayOfWeek, int repeat, const CultureInfo& loc);

		static std::wstring FormatMonth(int month, int repeat, const CultureInfo& loc);

		static bool IsUseGenitiveForm(std::wstring format, int index, int tokenLen, wchar_t patternToMatch);

		static std::wstring FormatCustomized(const DateTime& dateTime, std::wstring format, const CultureInfo& loc, const TimeSpan& offset);

		static void FormatCustomizedRoundripTimeZone(const DateTime& dateTime, const TimeSpan& offset, std::wstring* result);

		static void FormatCustomizedTimeZone(const DateTime& dateTime, const TimeSpan& offset, std::wstring format, int tokenLen, bool timeOnly, std::wstring* result);

		static std::wstring Format(const DateTime& dateTime, std::wstring format, const CultureInfo& loc, const TimeSpan& offset);
	};
}