#include "TimeSpan.h"
#include "DateTime.h"
#include "BuiltInAssembly.h"
#include "DateTimeFormat.h"
#define ENABLE_INTSAFE_SIGNED_FUNCTIONS 1
#include <intsafe.h>

using namespace System;


/// <summary>
/// 刻度（天）
/// </summary>
const Int64 TimeSpan::DAYS = 864000000000LL;
/// <summary>
/// 刻度（小时）
/// </summary>
const Int64 TimeSpan::HOURS = 36000000000LL;
/// <summary>
/// 刻度（分钟）
/// </summary>
const Int64 TimeSpan::MINUTES = 600000000LL;
/// <summary>
/// 刻度（秒）
/// </summary>
const Int64 TimeSpan::SECONDS = 10000000LL;
/// <summary>
/// 刻度（毫秒）
/// </summary>
const Int64 TimeSpan::MILLISECONDS = 10000LL;
/// <summary>
/// 刻度（微秒）
/// </summary>
const Int64 TimeSpan::MICROSECONDS = 10LL;

namespace System
{
	class TimeSpanFormat
	{
	protected:
		friend class TimeSpan;
		enum class Pattern
		{
			None,
			Minimum,
			Full
		};

		static std::wstring Format(const TimeSpan& value, std::wstring format, const CultureInfo& loc);
		static std::wstring FormatCustomized(const TimeSpan&, std::wstring format, const CultureInfo& loc);
		static std::wstring FormatStandard(const TimeSpan&, bool isInvariant, std::wstring format, TimeSpanFormat::Pattern pattern);
		static void IntToString(int v, int digits, std::wstring& result);
	};

	class TimeSpanParse
	{
	protected:
		friend class TimeSpan;
		friend class TimeSpanConverter;

		static TimeSpan Parse(const std::wstring& input, const CultureInfo& loc);

		static TimeSpan ParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles styles);

		static TimeSpan ParseExactMultiple(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, TimeSpanStyles styles);

		static void ValidateStyles(TimeSpanStyles style);

		static bool TryParse(const std::wstring& input, const CultureInfo& loc, TimeSpan& result);

		static bool TryParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles styles, TimeSpan& result);

		static bool TryParseExactMultiple(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, TimeSpanStyles styles, TimeSpan& result);

	};
}

struct FormatLiterals
{
	std::wstring AppCompatLiteral;

	int dd;

	int hh;

	int mm;

	int ss;

	int ff;

	std::wstring literals[6];

	std::wstring DayHourSep() const
	{
		return literals[1];

	}

	std::wstring End() const
	{
		return literals[5];
	}

	std::wstring HourMinuteSep() const
	{
		return literals[2];
	}

	std::wstring MinuteSecondSep() const
	{
		return literals[3];
	}

	std::wstring SecondFractionSep() const
	{
		return literals[4];
	}

	std::wstring Start() const
	{
		return literals[0];
	}
	FormatLiterals(){}

	void Init(const std::wstring& format, bool useInvariantFieldLengths)
	{
		for (int i = 0; i < 6; i++)
		{
			literals[i] = std::wstring();
		}
		dd = 0;
		hh = 0;
		mm = 0;
		ss = 0;
		ff = 0;
		std::wstring sb = std::wstring();
		sb.reserve(16);
		bool flag = false;
		wchar_t ch = '\'';
		size_t index1 = 0;
		for (size_t index2 = 0; index2 < format.size(); ++index2)
		{
			switch (format[index2])
			{
			case 'm':
				if (!flag)
				{
					index1 = 3;
					++mm;
					break;
				}
				else
					break;
			case 's':
				if (!flag)
				{
					index1 = 4;
					++ss;
					break;
				}
				else
					break;
			case '\\':
				if (!flag)
				{
					++index2;
				}
				else
					sb.push_back(format[index2]);
				break;
			case 'd':
				if (!flag)
				{
					index1 = 1;
					++dd;
					break;
				}
				else
					break;
			case 'f':
			case 'F':
				if (!flag)
				{
					index1 = 5;
					++ff;
					break;
				}
				else
					break;
			case 'h':
				if (!flag)
				{
					index1 = 2;
					++hh;
					break;
				}
				else
					break;
			case '"':
			case '\'':
				if (flag && (int)ch == (int)format[index2])
				{
					if (index1 < 0 || index1 > 5)
						return;
					literals[index1] = sb;
					sb.clear();
					flag = false;
					break;
				}
				else if (!flag)
				{
					ch = format[index2];
					flag = true;
					break;
				}
				else
					break;
			default:
				sb.push_back(format[index2]);
				break;
			}
		}
		AppCompatLiteral = MinuteSecondSep() + SecondFractionSep();
		if (useInvariantFieldLengths)
		{
			dd = 2;
			hh = 2;
			mm = 2;
			ss = 2;
			ff = 7;
		}
		else
		{
			if (dd < 1 || dd > 2)
				dd = 2;
			if (hh < 1 || hh > 2)
				hh = 2;
			if (mm < 1 || mm > 2)
				mm = 2;
			if (ss < 1 || ss > 2)
				ss = 2;
			if (ff < 1 || ff > 7)
				ff = 6;
		}
		sb.clear();
	}

	static FormatLiterals InitInvariant(bool isNegative)
	{
		FormatLiterals empty = FormatLiterals();
		empty.literals[0] = (isNegative ? L"-" : L"");
		empty.literals[1] = L".";
		empty.literals[2] = L":";
		empty.literals[3] = L":";
		empty.literals[4] = L".";
		empty.literals[5] = L"";
		empty.AppCompatLiteral = L":.";
		empty.dd = 2;
		empty.hh = 2;
		empty.mm = 2;
		empty.ss = 2;
		empty.ff = 7;
		return empty;
	}
};

const FormatLiterals PositiveInvariantFormatLiterals = FormatLiterals::InitInvariant(false);
const FormatLiterals NegativeInvariantFormatLiterals = FormatLiterals::InitInvariant(true);

/// <summary>
/// 获取总时间值
/// </summary>
/// <returns></returns>
Int64 TimeSpan::GetTicks() const
{
	return span;
}

TimeSpan & TimeSpan::operator=(const TimeSpan & timeSpan)
{
	span = timeSpan.span;
	return *this;
}

TimeSpan TimeSpan::operator + (const TimeSpan& timeSpan) const
{
	LONGLONG res = 0;
	if (Int64Add(span, timeSpan.span, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

TimeSpan TimeSpan::operator - (const TimeSpan& timeSpan) const
{
	LONGLONG res = 0;
	if (Int64Sub(span, timeSpan.span, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__uref TimeSpan TimeSpan::operator - () const
{
	return this->GetNegate();
}

TimeSpan& TimeSpan::operator += (TimeSpan& timeSpan)
{
	if (Int64Add(span, timeSpan.span, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return *this;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

TimeSpan& TimeSpan::operator -= (TimeSpan& timeSpan)
{
	if (Int64Sub(span, timeSpan.span, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return *this;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

bool TimeSpan::operator == (TimeSpan&  timeSpan) const
{
	return span == timeSpan.span;
}

bool TimeSpan::operator == (const TimeSpan&  timeSpan) const
{
	return  span == timeSpan.span;
}

bool TimeSpan::operator != (TimeSpan&  timeSpan) const
{
	return  span != timeSpan.span;
}

bool TimeSpan::operator != (const TimeSpan&  timeSpan) const
{
	return  span != timeSpan.span;
}

bool TimeSpan::operator > (TimeSpan&  timeSpan) const
{
	return  span > timeSpan.span;
}

bool TimeSpan::operator >= (TimeSpan&  timeSpan) const
{
	return  span >= timeSpan.span;
}

bool TimeSpan::operator < (TimeSpan&  timeSpan) const
{
	return  span < timeSpan.span;
}

bool TimeSpan::operator <= (TimeSpan&  timeSpan) const
{
	return  span <= timeSpan.span;
}

void TimeSpanFormat::IntToString(int v, int digits, std::wstring& result)
{
	int n = v == 0 ? 1 : (int)log10(v) + 1;
	n = digits - n;
	for (; n > 0; n--)
	{
		result.push_back(L'0');
	}
	result.append(std::to_wstring(v));
}

std::wstring TimeSpan::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return TimeSpanFormat::Format(*this, fmt == NULL ? std::wstring() : std::wstring(fmt), loc);
}

bool TimeSpan::TryParse(const std::wstring& input, const CultureInfo& loc, TimeSpan& result)
{
	return TimeSpanParse::TryParse(input, loc, result);
}

bool TimeSpan::TryParseExact(const std::wstring& input, const std::wstring& fmt, const CultureInfo& loc, TimeSpanStyles style, TimeSpan& result)
{
	return TimeSpanParse::TryParseExact(input, fmt, loc, style, result);
}

std::wstring TimeSpanFormat::Format(const TimeSpan& value, std::wstring format, const CultureInfo& loc)
{
	if (format.empty())
	{
		format = std::wstring(L"c");
	}
	if (format.size() != 1)
	{
		return FormatCustomized(value, format, loc);
	}
	wchar_t chr = format[0];
	if (chr == 'c' || chr == 't' || chr == 'T')
	{
		return FormatStandard(value, true, format, TimeSpanFormat::Pattern::Minimum);
	}
	if (chr != 'g' && chr != 'G')
	{
		throw std::exception("Format_InvalidString", 1);
	}

	if (value.span >= 0LL)
	{
		format = loc.GetPattern(CultureInfoPatterns::FullTimeSpanPositivePattern);
	}
	else
	{
		format = loc.GetPattern(CultureInfoPatterns::FullTimeSpanNegativePattern);
	}
	TimeSpanFormat::Pattern pattern;
	pattern = (chr != 'g' ? TimeSpanFormat::Pattern::Full : TimeSpanFormat::Pattern::Minimum);
	return FormatStandard(value, false, format, pattern);
}

std::wstring TimeSpanFormat::FormatCustomized(const TimeSpan& value, std::wstring format, const CultureInfo& dtfi)
{
	int num;
	int num1;
	int i;
	int num2 = value.Days;
	long long num3 = value.span % TimeSpan::DAYS;
	if (value.span < 0LL)
	{
		num2 = -num2;
		num3 = -num3;
	}
	int num4 = (int)(num3 / TimeSpan::HOURS % 24LL);
	int num5 = (int)(num3 / TimeSpan::MINUTES % 60LL);
	int num6 = (int)(num3 / TimeSpan::SECONDS % 60LL);
	int num7 = (int)(num3 % TimeSpan::SECONDS);
	long num8 = 0L;
	size_t num9 = 0;
	std::wstring stringBuilder = std::wstring();
	stringBuilder.reserve(16);
	while (true)
	{
		if (num9 >= format.size())
		{
			return stringBuilder;
		}
		wchar_t chr = format[num9];
		wchar_t chr1 = chr;
		if (chr1 <= 'F')
		{
			if (chr1 != '\"')
			{
				switch (chr1)
				{
				case '%':
				{
							num1 = DateTimeFormat::ParseNextChar(format, num9);
							if (num1 < 0 || num1 == 37)
							{
								throw std::exception("Format_InvalidString", 1);
							}
							wchar_t chr2 = (wchar_t)num1;
							std::wstring tmp = std::wstring();
							tmp.push_back(chr2);
							stringBuilder.append(TimeSpanFormat::FormatCustomized(value, tmp, dtfi));
							num = 2;
							goto Label1;
				}
				case '&':
				{
							throw std::exception("Format_InvalidString", 1);
				}
				case '\'':
				{
							 break;
				}
				default:
				{
						   if (chr1 == 'F')
						   {
							   num = DateTimeFormat::ParseRepeatPattern(format, num9, chr);
							   if (num > 7)
							   {
								   throw std::exception("Format_InvalidString", 1);
							   }
							   num8 = (long)num7;
							   num8 = num8 / (long)pow(10, (double)(7 - num));
							   for (i = num; i > 0 && num8 % (long)10 == (long)0; i--)
							   {
								   num8 = num8 / (long)10;
							   }
							   if (i <= 0)
							   {
								   goto Label1;
							   }
							   TimeSpanFormat::IntToString(num8, i, stringBuilder);
							   goto Label1;
						   }
						   else
						   {
							   throw std::exception("Format_InvalidString", 1);
						   }
				}
				}
			}
			std::wstring stringBuilder1 = std::wstring();
			num = DateTimeFormat::ParseQuoteString(format, num9, &stringBuilder1);
			stringBuilder.append(stringBuilder1);
		}
		else if (chr1 <= 'h')
		{
			if (chr1 == '\\')
			{
				num1 = DateTimeFormat::ParseNextChar(format, num9);
				if (num1 < 0)
				{
					throw std::exception("Format_InvalidString", 1);
				}
				stringBuilder.push_back((wchar_t)num1);
				num = 2;
			}
			else
			{
				switch (chr1)
				{
				case 'd':
				{
							num = DateTimeFormat::ParseRepeatPattern(format, num9, chr);
							if (num > 8)
							{
								throw std::exception("Format_InvalidString", 1);
							}
							DateTimeFormat::FormatDigits(&stringBuilder, num2, num, true);
							break;
				}
				case 'e':
				case 'g':
				{
							throw std::exception("Format_InvalidString", 1);
				}
				case 'f':
				{
							num = DateTimeFormat::ParseRepeatPattern(format, num9, chr);
							if (num > 7)
							{
								throw std::exception("Format_InvalidString", 1);
							}
							num8 = (long)num7;
							num8 = num8 / (long)pow(10, (double)(7 - num));
							TimeSpanFormat::IntToString(num8, num, stringBuilder);
							break;
				}
				case 'h':
				{
							num = DateTimeFormat::ParseRepeatPattern(format, num9, chr);
							if (num > 2)
							{
								throw std::exception("Format_InvalidString", 1);
							}
							DateTimeFormat::FormatDigits(&stringBuilder, num4, num);
							break;
				}
				default:
				{
						   throw std::exception("Format_InvalidString", 1);
				}
				}
			}
		}
		else if (chr1 == 'm')
		{
			num = DateTimeFormat::ParseRepeatPattern(format, num9, chr);
			if (num > 2)
			{
				throw std::exception("Format_InvalidString", 1);
			}
			DateTimeFormat::FormatDigits(&stringBuilder, num5, num);
		}
		else if (chr1 == 's')
		{
			num = DateTimeFormat::ParseRepeatPattern(format, num9, chr);
			if (num > 2)
			{
				throw std::exception("Format_InvalidString", 1);
			}
			DateTimeFormat::FormatDigits(&stringBuilder, num6, num);
		}
		else
		{
			break;
		}
	Label1:
		num9 = num9 + num;
	}
	throw std::exception("Format_InvalidString", 1);
}

std::wstring TimeSpanFormat::FormatStandard(const TimeSpan& value, bool isInvariant, std::wstring format, TimeSpanFormat::Pattern pattern)
{
	FormatLiterals formatLiteral;
	int i;
	std::wstring stringBuilder = std::wstring();
	stringBuilder.reserve(16);
	int num = value.Days;
	long long num1 = value.span % TimeSpan::DAYS;
	if (value.span < 0LL)
	{
		num = -num;
		num1 = -num1;
	}
	int num2 = (int)(num1 / TimeSpan::HOURS % 24LL);
	int num3 = (int)(num1 / TimeSpan::MINUTES % 60LL);
	int num4 = (int)(num1 / TimeSpan::SECONDS % 60LL);
	int num5 = (int)(num1 % TimeSpan::SECONDS);
	if (!isInvariant)
	{
		formatLiteral = FormatLiterals();
		formatLiteral.Init(format, pattern == TimeSpanFormat::Pattern::Full);
	}
	else
	{
		formatLiteral = (value.span >= 0LL ? PositiveInvariantFormatLiterals : NegativeInvariantFormatLiterals);
	}
	if (num5 != 0)
	{
		num5 = (int)((long long)num5 / (long long)pow(10, (double)(7 - formatLiteral.ff)));
	}
	stringBuilder.append(formatLiteral.Start());
	if (pattern == TimeSpanFormat::Pattern::Full || num != 0)
	{
		stringBuilder.append(std::to_wstring(num));
		stringBuilder.append(formatLiteral.DayHourSep());
	}
	TimeSpanFormat::IntToString(num2, formatLiteral.hh, stringBuilder);
	stringBuilder.append(formatLiteral.HourMinuteSep());
	TimeSpanFormat::IntToString(num3, formatLiteral.mm, stringBuilder);
	stringBuilder.append(formatLiteral.MinuteSecondSep());
	TimeSpanFormat::IntToString(num4, formatLiteral.ss, stringBuilder);
	if (!isInvariant && pattern == TimeSpanFormat::Pattern::Minimum)
	{
		for (i = formatLiteral.ff; i > 0 && num5 % 10 == 0; i--)
		{
			num5 = num5 / 10;
		}
		if (i > 0)
		{
			stringBuilder.append(formatLiteral.SecondFractionSep());
			TimeSpanFormat::IntToString(num5, i, stringBuilder);
		}
	}
	else if (pattern == TimeSpanFormat::Pattern::Full || num5 != 0)
	{
		stringBuilder.append(formatLiteral.SecondFractionSep());
		TimeSpanFormat::IntToString(num5, formatLiteral.ff, stringBuilder);
	}
	stringBuilder.append(formatLiteral.End());
	return stringBuilder;
}

__inline TimeSpan TimeSpan::GetDuration() const
{
	return TimeSpan(llabs(span));
}

__inline TimeSpan TimeSpan::GetNegate() const
{
	return TimeSpan(-span);
}

__inline int TimeSpan::GetDays() const
{
	return (int)(span / DAYS);
}

__inline int TimeSpan::GetHours() const
{
	return (int)(span / HOURS) % 24;
}

__inline int TimeSpan::GetMinutes() const
{
	return (int)(span / MINUTES) % 60;
}

__inline int TimeSpan::GetSeconds() const
{
	return (int)(span / SECONDS) % 60;
}

__inline int TimeSpan::GetMilliseconds() const
{
	return (int)(span / MILLISECONDS) % 1000;
}

__inline int TimeSpan::GetMicroseconds() const
{
	return (int)(span %MICROSECONDS);
}

__inline double TimeSpan::GetTotalDays() const
{
	return (double)span / DAYS;
}

__inline double TimeSpan::GetTotalHours() const
{
	return (double)span / HOURS;
}

__inline double TimeSpan::GetTotalMinutes() const
{
	return (double)span / MINUTES;
}

__inline double TimeSpan::GetTotalSeconds() const
{
	return (double)span / SECONDS;
}

__inline double TimeSpan::GetTotalMilliseconds() const
{
	return (double)span / MILLISECONDS;
}

__inline double TimeSpan::GetTotalMicroseconds() const
{
	return (double)span / MICROSECONDS;
}

TimeSpan::TimeSpan() :span(0LL){}

TimeSpan::TimeSpan(Int64 nano100seconds) : span(nano100seconds) {}


TimeSpan::TimeSpan(int hours, int minutes, int seconds)
{
	LONGLONG h = 0;
	LONGLONG mi = 0;
	LONGLONG s = 0;
	if (Int64Mult((LONGLONG)hours, HOURS, &h) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)minutes, MINUTES, &mi) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)seconds, SECONDS, &s) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(h, mi, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, s, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}


TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
{
	LONGLONG d = 0;
	LONGLONG h = 0;
	LONGLONG mi = 0;
	LONGLONG s = 0;
	LONGLONG ms = 0;
	if (Int64Mult((LONGLONG)days, DAYS, &d) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)hours, HOURS, &h) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)minutes, MINUTES, &mi) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)seconds, SECONDS, &s) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)milliseconds, MILLISECONDS, &ms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(d, h, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mi, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, s, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, ms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds) 
{
	LONGLONG d = 0;
	LONGLONG h = 0;
	LONGLONG mi = 0;
	LONGLONG s = 0;
	LONGLONG ms = 0;
	LONGLONG mms = 0;
	if (Int64Mult((LONGLONG)days, DAYS, &d) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)hours, HOURS, &h) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)minutes, MINUTES, &mi) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)seconds, SECONDS, &s) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)milliseconds, MILLISECONDS, &ms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)microseconds, MICROSECONDS, &mms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(d, h, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mi, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, s, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, ms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds, int nano100seconds)
{
	LONGLONG d = 0;
	LONGLONG h = 0;
	LONGLONG mi = 0;
	LONGLONG s = 0;
	LONGLONG ms = 0;
	LONGLONG mms = 0;
	if (Int64Mult((LONGLONG)days, DAYS, &d) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)hours, HOURS, &h) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)minutes, MINUTES, &mi) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)seconds, SECONDS, &s) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)milliseconds, MILLISECONDS, &ms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)microseconds, MICROSECONDS, &mms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(d, h, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mi, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, s, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, ms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, nano100seconds, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

Int64 TimeSpan::TimeToTicks(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds, int nano100seconds)
{
	LONGLONG span = 0;
	LONGLONG d = 0;
	LONGLONG h = 0;
	LONGLONG mi = 0;
	LONGLONG s = 0;
	LONGLONG ms = 0;
	LONGLONG mms = 0;
	if (Int64Mult((LONGLONG)days, DAYS, &d) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)hours, HOURS, &h) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)minutes, MINUTES, &mi) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)seconds, SECONDS, &s) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)milliseconds, MILLISECONDS, &ms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)microseconds, MICROSECONDS, &mms) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(d, h, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mi, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, s, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, ms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mms, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, nano100seconds, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return span;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
{
	LONGLONG d = 0;
	LONGLONG h = 0;
	LONGLONG mi = 0;
	LONGLONG s = 0;
	if (Int64Mult((LONGLONG)days, DAYS, &d) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)hours, HOURS, &h) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)minutes, MINUTES, &mi) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Mult((LONGLONG)seconds, SECONDS, &s) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(d, h, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, mi, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	if (Int64Add(span, s, &span) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return;
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}


__inline TimeSpan TimeSpan::FromDays(double value)
{
	if (isnan<double>(value))
	{
		throw std::range_error("Arg_CannotBeNaN");
	}
	LONGLONG res = 0;
	if (Int64Mult((LONGLONG)value, DAYS, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__inline TimeSpan TimeSpan::FromHours(double value)
{
	if (isnan<double>(value))
	{
		throw std::range_error("Arg_CannotBeNaN");
	}
	LONGLONG res = 0;
	if (Int64Mult((LONGLONG)value, HOURS, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__inline TimeSpan TimeSpan::FromMinutes(double value)
{
	if (isnan<double>(value))
	{
		throw std::range_error("Arg_CannotBeNaN");
	}
	LONGLONG res = 0;
	if (Int64Mult((LONGLONG)value, MINUTES, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__inline TimeSpan TimeSpan::FromSeconds(double value)
{
	if (isnan<double>(value))
	{
		throw std::range_error("Arg_CannotBeNaN");
	}
	LONGLONG res = 0;
	if (Int64Mult((LONGLONG)value, SECONDS, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__inline TimeSpan TimeSpan::FromMilliseconds(double value)
{
	if (isnan<double>(value))
	{
		throw std::range_error("Arg_CannotBeNaN");
	}
	LONGLONG res = 0;
	if (Int64Mult((LONGLONG)value, MILLISECONDS, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__inline TimeSpan TimeSpan::FromMicroseconds(double value)
{
	if (isnan<double>(value))
	{
		throw std::range_error("Arg_CannotBeNaN");
	}
	LONGLONG res = 0;
	if (Int64Mult((LONGLONG)value, MICROSECONDS, &res) == INTSAFE_E_ARITHMETIC_OVERFLOW) goto ERR;
	return TimeSpan(res);
ERR:
	throw std::overflow_error("ArgumentOutOfRange");
}

__inline TimeSpan TimeSpan::FromNano100seconds(Int64 value)
{
	return TimeSpan(value);
}

TimeSpan::~TimeSpan() {}

TimeSpan::TimeSpan(const TimeSpan& timeSpan)
{
	span = timeSpan.span;
}

std::weak_ptr<TimeSpanConverter> TimeSpanConverter::_thisWeakPtr;
std::mutex& TimeSpanConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

TimeSpanConverter::TimeSpanConverter() {}
TimeSpanConverter::~TimeSpanConverter() {}

std::shared_ptr<TimeSpanConverter> TimeSpanConverter::GetSharedInstance()
{
	std::shared_ptr<TimeSpanConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new TimeSpanConverter());
	_thisWeakPtr = p;
	return p;
}

TimeSpanConverter& TimeSpanConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool TimeSpanConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(TimeSpan))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool TimeSpanConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(TimeSpan))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any TimeSpanConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(TimeSpan)))
		{
			if (type->IsReference)
			{
				return TimeSpan(AnyVisitor<TimeSpan&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			TimeSpan* p = obj;
			if (p)
				return p->ToString();
			else
			{
				TimeSpan res = obj.as<TimeSpan>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				TimeSpan result = TimeSpan();
				if (TimeSpan::TryParse(std::wstring(tmp), loc, result))
				{
					return result;
				}
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any TimeSpanConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
{
	if (!type)return _missing;
	_type otype = obj.Type;
	if (!otype) return _missing;
	try
	{
		bool iswiden = false;
		if (type->GetIsSTDString(iswiden))
		{
			if (iswiden)
			{
				if (otype->IsReference)
				{
					return AnyVisitor<TimeSpan&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				TimeSpan* p = obj;
				if (p)
					return p->ToString();
				else
				{
					TimeSpan v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(TimeSpan)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<TimeSpan&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			TimeSpan* p = obj;
			if (p)
				return *p;
			else
			{
				TimeSpan v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any TimeSpanConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	TimeSpan result = TimeSpan();
	if (TimeSpan::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring TimeSpanConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(TimeSpan)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<TimeSpan&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			TimeSpan* p = obj;
			if (p)
				return p->ToString();
			else
			{
				TimeSpan v = obj;
				return v.ToString();
			}
		}
		return std::wstring();
	}
	catch (...)
	{
		return std::wstring();
	}
}

#pragma region Parse

enum class TTT
{
	None,
	End,
	Num,
	Sep,
	NumOverflow
};

enum class ParseFailureKind
{
	None,
	ArgumentNull,
	Format,
	FormatWithParameter,
	Overflow
};

ENUM_FLAGS(TimeSpanStandardStyles)
enum class TimeSpanStandardStyles
{
	None = 0,
	Invariant = 1,
	Localized = 2,
	Any = 3,
	RequireFull = 4
};

enum class TimeSpanThrowStyle
{
	None,
	All
};

struct TimeSpanToken
{
	TTT ttt;

	int num;

	int zeroes;

	std::wstring sep;
	TimeSpanToken()
	{
		ttt = TTT::Num;
		num = 0;
		zeroes = 0;
		sep = std::wstring();
	}

	TimeSpanToken(int number)
	{
		ttt = TTT::Num;
		num = number;
		zeroes = 0;
		sep = std::wstring();
	}

	TimeSpanToken(int leadingZeroes, int number)
	{
		ttt = TTT::Num;
		num = number;
		zeroes = leadingZeroes;
		sep = std::wstring();
	}

	bool IsInvalidNumber(int maxValue, int maxPrecision) const
	{
		if (num > maxValue)
		{
			return true;
		}
		if (maxPrecision == -1)
		{
			return false;
		}
		if (zeroes > maxPrecision)
		{
			return true;
		}
		if (num == 0 || zeroes == 0)
		{
			return false;
		}
		return (long)num >= (long)maxValue / (long)pow(10, (double)(zeroes - 1));
	}
};

struct TimeSpanTokenizer
{
	size_t m_pos;

	std::wstring m_value;

	wchar_t CurrentChar()
	{
		if (m_pos <0 || m_pos >= m_value.size())
		{
			return '\0';
		}
		return m_value[m_pos];
	}

	bool EOL()
	{
		return m_pos >= m_value.size() - 1;
	}

	wchar_t NextChar()
	{
		m_pos++;
		return CurrentChar();
	}

	void BackOne()
	{
		if (m_pos > 0)
		{
			m_pos--;
		}
	}

	TimeSpanToken GetNextToken()
	{
		TimeSpanToken timeSpanToken = TimeSpanToken();
		wchar_t currentChar = CurrentChar();
		if (currentChar == 0)
		{
			timeSpanToken.ttt = TTT::End;
			return timeSpanToken;
		}
		if (currentChar < '0' || currentChar > '9')
		{
			timeSpanToken.ttt = TTT::Sep;
			int mPos = m_pos;
			int num = 0;
			while (currentChar != 0 && (currentChar < '0' || 57 < currentChar))
			{
				currentChar = NextChar();
				num++;
			}
			timeSpanToken.sep = m_value.substr(mPos, num);
			return timeSpanToken;
		}
		timeSpanToken.ttt = TTT::Num;
		timeSpanToken.num = 0;
		timeSpanToken.zeroes = 0;
		do
		{
			if (((long)timeSpanToken.num & (ULong)-268435456) != (long)0)
			{
				timeSpanToken.ttt = TTT::NumOverflow;
				return timeSpanToken;
			}
			timeSpanToken.num = timeSpanToken.num * 10 + currentChar - 48;
			if (timeSpanToken.num == 0)
			{
				timeSpanToken.zeroes = timeSpanToken.zeroes + 1;
			}
			if (timeSpanToken.num < 0)
			{
				timeSpanToken.ttt = TTT::NumOverflow;
				return timeSpanToken;
			}
			currentChar = NextChar();
		} while (currentChar >= '0' && currentChar <= '9');
		return timeSpanToken;
	}

	void Init(const std::wstring& input)
	{
		Init(input, 0);
	}

	void Init(const std::wstring& input, int startPosition)
	{
		m_pos = startPosition;
		m_value = input;
	}
};

struct TimeSpanResult
{
	TimeSpan parsedTimeSpan;

	TimeSpanThrowStyle throwStyle;

	ParseFailureKind m_failure;

	std::string m_failureMessage;

	std::exception GetTimeSpanParseException()
	{
		return std::exception(m_failureMessage.empty() ? "" : m_failureMessage.c_str());
	}

	void Init(TimeSpanThrowStyle canThrow)
	{
		parsedTimeSpan = TimeSpan();
		throwStyle = canThrow;
	}

	void SetFailure(ParseFailureKind failure, const std::string& failureMessage)
	{
		m_failure = failure;
		m_failureMessage = failureMessage;
		if (throwStyle != TimeSpanThrowStyle::None)
		{
			throw GetTimeSpanParseException();
		}
	}
};

struct StringParser
{
	std::wstring str;

	wchar_t ch;

	int pos;

	int len;

	void NextChar()
	{
		if (pos < len)
		{
			pos++;
		}
		ch = (pos < len ? str[pos] : '\0');
	}

	wchar_t NextNonDigit()
	{
		for (int i = pos; i < len; i++)
		{
			wchar_t chr = str[i];
			if (chr < '0' || chr > '9')
			{
				return chr;
			}
		}
		return '\0';
	}

	bool ParseInt(int max, int& i, TimeSpanResult& result)
	{
		i = 0;
		int num = pos;
		while (ch >= '0' && ch <= '9')
		{
			if (((long)i & (ULong)-268435456) != (long)0)
			{
				result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
				return false;
			}
			i = i * 10 + ch - 48;
			if (i < 0)
			{
				result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
				return false;
			}
			NextChar();
		}
		if (num == pos)
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
		if (i <= max)
		{
			return true;
		}
		result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
		return false;
	}

	bool ParseTime(long long& time, TimeSpanResult& result)
	{
		int num;
		time = 0LL;
		if (!ParseInt(23, num, result))
		{
			return false;
		}
		time = num * TimeSpan::HOURS;
		if (ch != ':')
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
		NextChar();
		if (!ParseInt(59, num, result))
		{
			return false;
		}
		time = time + num * TimeSpan::MINUTES;
		if (ch == ':')
		{
			NextChar();
			if (ch != '.')
			{
				if (!ParseInt(59, num, result))
				{
					return false;
				}
				time = time + num * TimeSpan::SECONDS;
			}
			if (ch == '.')
			{
				NextChar();
				Int64 num1 = TimeSpan::SECONDS;
				while (num1 > 1 && ch >= '0' && ch <= '9')
				{
					num1 = num1 / 10;
					time = time + (Int64)((ch - 48) * (char)num1);
					NextChar();
				}
			}
		}
		return true;
	}

	void SkipBlanks()
	{
		while (ch == ' ' || ch == '\t')
		{
			NextChar();
		}
	}

	bool TryParse(const std::wstring& input, TimeSpanResult& result)
	{
		long long num;
		int num1;
		long long num2;
		result.parsedTimeSpan = 0;
		if (input.empty())
		{
			result.SetFailure(ParseFailureKind::ArgumentNull, "argument null");
			return false;
		}
		str = input;
		len = input.size();
		pos = -1;
		NextChar();
		SkipBlanks();
		bool flag = false;
		if (ch == '-')
		{
			flag = true;
			NextChar();
		}
		if (NextNonDigit() != ':')
		{
			if (!ParseInt(10675199, num1, result))
			{
				return false;
			}
			num = (Int64)num1 * TimeSpan::DAYS;
			if (ch == '.')
			{
				NextChar();
				if (!ParseTime(num2, result))
				{
					return false;
				}
				num = num + num2;
			}
		}
		else if (!ParseTime(num, result))
		{
			return false;
		}
		if (flag)
		{
			num = -num;
			if (num > (long)0)
			{
				result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
				return false;
			}
		}
		else if (num < (long)0)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
		SkipBlanks();
		if (pos < len)
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
		result.parsedTimeSpan = num;
		return true;
	}
};

struct TimeSpanRawInfo
{
	TTT lastSeenTTT;

	int tokenCount;

	int SepCount;

	int NumCount;

	std::wstring literals[6];

	TimeSpanToken numbers[5];

	FormatLiterals m_posLoc;

	FormatLiterals m_negLoc;

	bool m_posLocInit;

	bool m_negLocInit;

	std::wstring m_fullPosPattern;

	std::wstring m_fullNegPattern;

	FormatLiterals NegativeInvariant()
	{
		return NegativeInvariantFormatLiterals;
	}

	FormatLiterals NegativeLocalized()
	{
		if (!m_negLocInit)
		{
			m_negLoc = FormatLiterals();
			m_negLoc.Init(m_fullNegPattern, false);
			m_negLocInit = true;
		}
		return m_negLoc;
	}

	FormatLiterals PositiveInvariant()
	{
		return PositiveInvariantFormatLiterals;
	}

	FormatLiterals PositiveLocalized()
	{
		if (!m_posLocInit)
		{
			m_posLoc = FormatLiterals();
			m_posLoc.Init(m_fullPosPattern, false);
			m_posLocInit = true;
		}
		return m_posLoc;
	}

	bool AddNum(TimeSpanToken num, TimeSpanResult& result)
	{
		if (NumCount >= 5 || tokenCount >= 11)
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
		int numCount = NumCount;
		int num1 = numCount;
		NumCount++;
		numbers[num1] = num;
		tokenCount++;
		return true;
	}

	bool AddSep(std::wstring sep, TimeSpanResult& result)
	{
		if (SepCount >= 6 || tokenCount >= 11)
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
		int sepCount = SepCount;
		int num = sepCount;
		SepCount++;
		literals[num] = sep;
		tokenCount ++;
		return true;
	}

	bool FullAppCompatMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 5 || NumCount != 4 || !(pattern.Start() == literals[0]) || !(pattern.DayHourSep() == literals[1]) || !(pattern.HourMinuteSep() == literals[2]) || !(pattern.AppCompatLiteral == literals[3]))
		{
			return false;
		}
		return pattern.End() == literals[4];
	}

	bool FullDHMMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 4 || NumCount != 3 || !(pattern.Start() == literals[0]) || !(pattern.DayHourSep() == literals[1]) || !(pattern.HourMinuteSep() == literals[2]))
		{
			return false;
		}
		return pattern.End() == literals[3];
	}

	bool FullDHMSMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 5 || NumCount != 4 || !(pattern.Start() == literals[0]) || !(pattern.DayHourSep() == literals[1]) || !(pattern.HourMinuteSep() == literals[2]) || !(pattern.MinuteSecondSep() == literals[3]))
		{
			return false;
		}
		return pattern.End() == literals[4];
	}

	bool FullDMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 2 || NumCount != 1 || !(pattern.Start() == literals[0]))
		{
			return false;
		}
		return pattern.End() == literals[1];
	}

	bool FullHMMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 3 || NumCount != 2 || !(pattern.Start() == literals[0]) || !(pattern.HourMinuteSep() == literals[1]))
		{
			return false;
		}
		return pattern.End() == literals[2];
	}

	bool FullHMSFMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 5 || NumCount != 4 || !(pattern.Start() == literals[0]) || !(pattern.HourMinuteSep() == literals[1]) || !(pattern.MinuteSecondSep() == literals[2]) || !(pattern.SecondFractionSep() == literals[3]))
		{
			return false;
		}
		return pattern.End() == literals[4];
	}

	bool FullHMSMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 4 || NumCount != 3 || !(pattern.Start() == literals[0]) || !(pattern.HourMinuteSep() == literals[1]) || !(pattern.MinuteSecondSep() == literals[2]))
		{
			return false;
		}
		return pattern.End() == literals[3];
	}

	bool FullMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 6 || NumCount != 5 || !(pattern.Start() == literals[0]) || !(pattern.DayHourSep() == literals[1]) || !(pattern.HourMinuteSep() == literals[2]) || !(pattern.MinuteSecondSep() == literals[3]) || !(pattern.SecondFractionSep() == literals[4]))
		{
			return false;
		}
		return pattern.End() == literals[5];
	}

	void Init(const CultureInfo& loc)
	{
		lastSeenTTT = TTT::None;
		tokenCount = 0;
		SepCount = 0;
		NumCount = 0;
		m_fullPosPattern = loc.GetPattern(CultureInfoPatterns::FullTimeSpanPositivePattern);
		m_fullNegPattern = loc.GetPattern(CultureInfoPatterns::FullTimeSpanNegativePattern);
		m_posLocInit = false;
		m_negLocInit = false;
	}

	bool PartialAppCompatMatch(const FormatLiterals& pattern)
	{
		if (SepCount != 4 || NumCount != 3 || !(pattern.Start() == literals[0]) || !(pattern.HourMinuteSep() == literals[1]) || !(pattern.AppCompatLiteral == literals[2]))
		{
			return false;
		}
		return pattern.End() == literals[3];
	}

	bool ProcessToken(TimeSpanToken& tok, TimeSpanResult& result)
	{
		if (tok.ttt == TTT::NumOverflow)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
		if (tok.ttt != TTT::Sep && tok.ttt != TTT::Num)
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
		switch (tok.ttt)
		{
		case TTT::Num:
		{
						 if (tokenCount == 0 && !AddSep(std::wstring(), result))
						 {
							 return false;
						 }
						 if (AddNum(tok, result))
						 {
							 break;
						 }
						 return false;
		}
		case TTT::Sep:
		{
						 if (AddSep(tok.sep, result))
						 {
							 break;
						 }
						 return false;
		}
		}
		lastSeenTTT = tok.ttt;
		return true;
	}
};

static TimeSpanToken zero = TimeSpanToken(0);

bool ParseExactDigits(TimeSpanTokenizer& tokenizer, int minDigitLength, int& result);
bool ParseExactDigits(TimeSpanTokenizer& tokenizer, int minDigitLength, int maxDigitLength, int& zeroes, int& result);
bool ParseExactLiteral(TimeSpanTokenizer& tokenizer, std::wstring enquotedString);
bool ProcessTerminal_D(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result);
bool ProcessTerminal_DHMSF(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result);
bool ProcessTerminal_HM(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result);
bool ProcessTerminal_HM_S_D(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result);
bool ProcessTerminal_HMS_F_D(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result);
bool ProcessTerminalState(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result);
int TimeSpanParse_ParseNextChar(const std::wstring& format, size_t pos);
int TimeSpanParse_ParseRepeatPattern(const std::wstring& format, size_t pos, wchar_t patternChar);
bool TimeSpanParse_TryParseQuoteString(const std::wstring& format, int pos, std::wstring& result, int& returnValue);
bool TryParseByFormat(const std::wstring& input, std::wstring format, TimeSpanStyles styles, TimeSpanResult& result);
bool TryParseExactMultipleTimeSpan(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, TimeSpanStyles styles, TimeSpanResult& result);
std::wstring TimeSpanParse_TrimString(const std::wstring& str);
bool TryParseExactTimeSpan(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles styles, TimeSpanResult& result); 
bool TryParseTimeSpan(const std::wstring& input, TimeSpanStandardStyles style, const CultureInfo& loc, TimeSpanResult& result);
bool TryParseTimeSpanConstant(const std::wstring& input, TimeSpanResult& result);

bool TryTimeToTicks(bool positive, const TimeSpanToken& days, const TimeSpanToken& hours, const TimeSpanToken& minutes, const TimeSpanToken& seconds, const TimeSpanToken& fraction, long long& result)
{
	if (days.IsInvalidNumber(10675199, -1) || hours.IsInvalidNumber(23, -1) || minutes.IsInvalidNumber(59, -1) || seconds.IsInvalidNumber(59, -1) || fraction.IsInvalidNumber(9999999, 7))
	{
		result = 0LL;
		return false;
	}
	long long num = ((Int64)days.num * 3600LL * 24LL + (Int64)hours.num * 3600LL + (Int64)minutes.num * 60LL + (Int64)seconds.num)*1000LL;
	if (num > 922337203685477LL || num < -922337203685477LL)
	{
		result = 0LL;
		return false;
	}
	long num1 = (long)fraction.num;
	if (num1 != 0LL)
	{
		long num2 = 1000000L;
		if (fraction.zeroes > 0)
		{
			long num3 = (long)pow(10, (double)fraction.zeroes);
			num2 = num2 / num3;
		}
		while (num1 < num2)
		{
			num1 = num1 * (long)10;
		}
	}
	result = num * 10000LL + num1;
	if (!positive || result >= 0LL)
	{
		return true;
	}
	result = 0LL;
	return false;
}

bool ParseExactDigits(TimeSpanTokenizer& tokenizer, int minDigitLength, int& result)
{
	result = 0;
	int num = 0;
	return ParseExactDigits(tokenizer, minDigitLength, (minDigitLength == 1 ? 2 : minDigitLength), num, result);
}

bool ParseExactDigits(TimeSpanTokenizer& tokenizer, int minDigitLength, int maxDigitLength, int& zeroes, int& result)
{
	result = 0;
	zeroes = 0;
	int num = 0;
	while (num < maxDigitLength)
	{
		wchar_t nextChar = tokenizer.NextChar();
		if (nextChar < '0' || nextChar > '9')
		{
			tokenizer.BackOne();
			break;
		}
		else
		{
			result = result * 10 + (nextChar - 48);
			if (result == 0)
			{
				zeroes = zeroes + 1;
			}
			num++;
		}
	}
	return num >= minDigitLength;
}

bool ParseExactLiteral(TimeSpanTokenizer& tokenizer, std::wstring enquotedString)
{
	for (size_t i = 0; i < enquotedString.size(); i++)
	{
		if (enquotedString[i] != tokenizer.NextChar())
		{
			return false;
		}
	}
	return true;
}

bool ProcessTerminal_D(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result)
{
	if (raw.SepCount != 2 || raw.NumCount != 1 || (style & TimeSpanStandardStyles::RequireFull) != TimeSpanStandardStyles::None)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	bool flag = (style & TimeSpanStandardStyles::Invariant) != TimeSpanStandardStyles::None;
	bool flag1 = (style & TimeSpanStandardStyles::Localized) != TimeSpanStandardStyles::None;
	bool flag2 = false;
	bool flag3 = false;
	if (flag)
	{
		if (raw.FullDMatch(raw.PositiveInvariant()))
		{
			flag3 = true;
			flag2 = true;
		}
		if (!flag3 && raw.FullDMatch(raw.NegativeInvariant()))
		{
			flag3 = true;
			flag2 = false;
		}
	}
	if (flag1)
	{
		if (!flag3 && raw.FullDMatch(raw.PositiveLocalized()))
		{
			flag3 = true;
			flag2 = true;
		}
		if (!flag3 && raw.FullDMatch(raw.NegativeLocalized()))
		{
			flag3 = true;
			flag2 = false;
		}
	}
	long long num =  0LL;
	if (!flag3)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (!TryTimeToTicks(flag2, raw.numbers[0], zero, zero, zero, zero, num))
	{
		result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
		return false;
	}
	if (!flag2)
	{
		num = -num;
		if (num > (long)0)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
	}
	result.parsedTimeSpan = num;
	return true;
}

bool ProcessTerminal_DHMSF(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result)
{
	long long num;
	if (raw.SepCount != 6 || raw.NumCount != 5)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	bool flag = (style & TimeSpanStandardStyles::Invariant) != TimeSpanStandardStyles::None;
	bool flag1 = (style & TimeSpanStandardStyles::Localized) != TimeSpanStandardStyles::None;
	bool flag2 = false;
	bool flag3 = false;
	if (flag)
	{
		if (raw.FullMatch(raw.PositiveInvariant()))
		{
			flag3 = true;
			flag2 = true;
		}
		if (!flag3 && raw.FullMatch(raw.NegativeInvariant()))
		{
			flag3 = true;
			flag2 = false;
		}
	}
	if (flag1)
	{
		if (!flag3 && raw.FullMatch(raw.PositiveLocalized()))
		{
			flag3 = true;
			flag2 = true;
		}
		if (!flag3 && raw.FullMatch(raw.NegativeLocalized()))
		{
			flag3 = true;
			flag2 = false;
		}
	}
	if (!flag3)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (!TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], raw.numbers[4], num))
	{
		result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
		return false;
	}
	if (!flag2)
	{
		num = -num;
		if (num > (long)0)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
	}
	result.parsedTimeSpan= num;
	return true;
}

bool ProcessTerminal_HM(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result)
{
	if (raw.SepCount != 3 || raw.NumCount != 2 || (style & TimeSpanStandardStyles::RequireFull) != TimeSpanStandardStyles::None)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	bool flag = (style & TimeSpanStandardStyles::Invariant) != TimeSpanStandardStyles::None;
	bool flag1 = (style & TimeSpanStandardStyles::Localized) != TimeSpanStandardStyles::None;
	bool flag2 = false;
	bool flag3 = false;
	if (flag)
	{
		if (raw.FullHMMatch(raw.PositiveInvariant()))
		{
			flag3 = true;
			flag2 = true;
		}
		if (!flag3 && raw.FullHMMatch(raw.NegativeInvariant()))
		{
			flag3 = true;
			flag2 = false;
		}
	}
	if (flag1)
	{
		if (!flag3 && raw.FullHMMatch(raw.PositiveLocalized()))
		{
			flag3 = true;
			flag2 = true;
		}
		if (!flag3 && raw.FullHMMatch(raw.NegativeLocalized()))
		{
			flag3 = true;
			flag2 = false;
		}
	}
	long long num = (long)0;
	if (!flag3)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (!TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], zero, zero, num))
	{
		result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
		return false;
	}
	if (!flag2)
	{
		num = -num;
		if (num > (long)0)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
	}
	result.parsedTimeSpan = num;
	return true;
}

bool ProcessTerminal_HM_S_D(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result)
{
	if (raw.SepCount != 4 || raw.NumCount != 3 || (style & TimeSpanStandardStyles::RequireFull) != TimeSpanStandardStyles::None)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	bool flag = (style & TimeSpanStandardStyles::Invariant) != TimeSpanStandardStyles::None;
	bool flag1 = (style & TimeSpanStandardStyles::Localized) != TimeSpanStandardStyles::None;
	bool flag2 = false;
	bool ticks = false;
	bool flag3 = false;
	long long num = (long)0;
	if (flag)
	{
		if (raw.FullHMSMatch(raw.PositiveInvariant()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMMatch(raw.PositiveInvariant()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.PartialAppCompatMatch(raw.PositiveInvariant()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], zero, raw.numbers[2], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullHMSMatch(raw.NegativeInvariant()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMMatch(raw.NegativeInvariant()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.PartialAppCompatMatch(raw.NegativeInvariant()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], zero, raw.numbers[2], num);
			flag3 = (flag3 ? true : !ticks);
		}
	}
	if (flag1)
	{
		if (!ticks && raw.FullHMSMatch(raw.PositiveLocalized()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMMatch(raw.PositiveLocalized()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.PartialAppCompatMatch(raw.PositiveLocalized()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], zero, raw.numbers[2], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullHMSMatch(raw.NegativeLocalized()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMMatch(raw.NegativeLocalized()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.PartialAppCompatMatch(raw.NegativeLocalized()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], zero, raw.numbers[2], num);
			flag3 = (flag3 ? true : !ticks);
		}
	}
	if (!ticks)
	{
		if (flag3)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (!flag2)
	{
		num = -num;
		if (num > (long)0)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
	}
	result.parsedTimeSpan = num;
	return true;
}

bool ProcessTerminal_HMS_F_D(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result)
{
	if (raw.SepCount != 5 || raw.NumCount != 4 || (style & TimeSpanStandardStyles::RequireFull) != TimeSpanStandardStyles::None)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	bool flag = (style & TimeSpanStandardStyles::Invariant) != TimeSpanStandardStyles::None;
	bool flag1 = (style & TimeSpanStandardStyles::Localized) != TimeSpanStandardStyles::None;
	long long num = 0LL;
	bool flag2 = false;
	bool ticks = false;
	bool flag3 = false;
	if (flag)
	{
		if (raw.FullHMSFMatch(raw.PositiveInvariant()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMSMatch(raw.PositiveInvariant()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullAppCompatMatch(raw.PositiveInvariant()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullHMSFMatch(raw.NegativeInvariant()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMSMatch(raw.NegativeInvariant()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullAppCompatMatch(raw.NegativeInvariant()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
	}
	if (flag1)
	{
		if (!ticks && raw.FullHMSFMatch(raw.PositiveLocalized()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMSMatch(raw.PositiveLocalized()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullAppCompatMatch(raw.PositiveLocalized()))
		{
			flag2 = true;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullHMSFMatch(raw.NegativeLocalized()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, zero, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullDHMSMatch(raw.NegativeLocalized()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], raw.numbers[3], zero, num);
			flag3 = (flag3 ? true : !ticks);
		}
		if (!ticks && raw.FullAppCompatMatch(raw.NegativeLocalized()))
		{
			flag2 = false;
			ticks = TryTimeToTicks(flag2, raw.numbers[0], raw.numbers[1], raw.numbers[2], zero, raw.numbers[3], num);
			flag3 = (flag3 ? true : !ticks);
		}
	}
	if (!ticks)
	{
		if (flag3)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (!flag2)
	{
		num = -num;
		if (num > (long)0)
		{
			result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
			return false;
		}
	}
	result.parsedTimeSpan = num;
	return true;
}

bool ProcessTerminalState(TimeSpanRawInfo& raw, TimeSpanStandardStyles style, TimeSpanResult& result)
{
	if (raw.lastSeenTTT == TTT::Num)
	{
		TimeSpanToken timeSpanToken = TimeSpanToken();
		timeSpanToken.ttt = TTT::Sep,
		timeSpanToken.sep = std::wstring();
		if (!raw.ProcessToken(timeSpanToken, result))
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
	}
	switch (raw.NumCount)
	{
	case 1:
	{
			  return ProcessTerminal_D(raw, style, result);
	}
	case 2:
	{
			  return ProcessTerminal_HM(raw, style, result);
	}
	case 3:
	{
			  return ProcessTerminal_HM_S_D(raw, style, result);
	}
	case 4:
	{
			  return ProcessTerminal_HMS_F_D(raw, style, result);
	}
	case 5:
	{
			  return ProcessTerminal_DHMSF(raw, style, result);
	}
	}
	result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
	return false;
}

int TimeSpanParse_ParseNextChar(const std::wstring& format, size_t pos)
{
	if ((size_t)pos >= format.size() - 1)
	{
		return -1;
	}
	return format[pos + 1];
}

int TimeSpanParse_ParseRepeatPattern(const std::wstring& format, size_t pos, wchar_t patternChar)
{
	int length = format.size();
	int num = pos + 1;
	while (num < length && format[num] == patternChar)
	{
		num++;
	}
	return num - pos;
}

bool TimeSpanParse_TryParseQuoteString(const std::wstring& format, int pos, std::wstring& result, int& returnValue)
{
	returnValue = 0;
	int length = format.size();
	int num = pos;
	int num1 = pos;
	pos = num1 + 1;
	wchar_t chr = format[num1];
	bool flag = false;
	while (pos < length)
	{
		int num2 = pos;
		pos = num2 + 1;
		wchar_t chr1 = format[num2];
		if (chr1 == chr)
		{
			flag = true;
			break;
		}
		else if (chr1 != '\\')
		{
			result.push_back(chr1);
		}
		else
		{
			if (pos >= length)
			{
				return false;
			}
			int num3 = pos;
			pos = num3 + 1;
			result.push_back(format[num3]);
		}
	}
	if (!flag)
	{
		return false;
	}
	returnValue = pos - num;
	return true;
}

bool TryParseByFormat(const std::wstring& input, std::wstring format, TimeSpanStyles styles, TimeSpanResult& result)
{
	int num;
	bool flag = false;
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	int num4 = 0;
	int num5 = 0;
	int num6 = 0;
	int num7 = 0;
	int num8 = 0;
	TimeSpanTokenizer timeSpanTokenizer = TimeSpanTokenizer();
	timeSpanTokenizer.Init(input, -1);
	while (true)
	{
		if (num7 >= (int)format.size())
		{
			if (!timeSpanTokenizer.EOL())
			{
				result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
				return false;
			}
			long long num9 = (long long)0;
			bool flag5 = (styles & TimeSpanStyles::AssumeNegative) == TimeSpanStyles::None;
			if (!TryTimeToTicks(flag5, TimeSpanToken(num1), TimeSpanToken(num2), TimeSpanToken(num3), TimeSpanToken(num4), TimeSpanToken(num5, num6), num9))
			{
				result.SetFailure(ParseFailureKind::Overflow, "overflow timeSpanElementTooLarge");
				return false;
			}
			if (!flag5)
			{
				num9 = -num9;
			}
			result.parsedTimeSpan = num9;
			return true;
		}
		wchar_t chr = format[num7];
		wchar_t chr1 = chr;
		if (chr1 <= 'F')
		{
			if (chr1 != '\"')
			{
				switch (chr1)
				{
				case '%':
				{
							num = TimeSpanParse_ParseNextChar(format, num7);
							if (num < 0 || num == 37)
							{
								result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
								return false;
							}
							num8 = 1;
							goto Label3;
				}
				case '&':
				{
							result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
							return false;
				}
				case '\'':
				{
							 break;
				}
				default:
				{
						   if (chr1 == 'F')
						   {
							   num8 = TimeSpanParse_ParseRepeatPattern(format, num7, chr);
							   if (num8 > 7 || flag4)
							   {
								   result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
								   return false;
							   }
							   ParseExactDigits(timeSpanTokenizer, num8, num8, num5, num6);
							   flag4 = true;
							   goto Label3;
						   }
						   else
						   {
							   result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
							   return false;
						   }
				}
				}
			}
			std::wstring stringBuilder = std::wstring();
			if (!TimeSpanParse_TryParseQuoteString(format, num7, stringBuilder, num8))
			{
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadQuote");
				return false;
			}
			if (!ParseExactLiteral(timeSpanTokenizer, stringBuilder))
			{
				result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
				return false;
			}
		}
		else if (chr1 <= 'h')
		{
			if (chr1 == '\\')
			{
				num = TimeSpanParse_ParseNextChar(format, num7);
				if (num < 0 || timeSpanTokenizer.NextChar() != (char)num)
				{
					result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
					return false;
				}
				num8 = 2;
			}
			else
			{
				switch (chr1)
				{
				case 'd':
				{
							num8 = TimeSpanParse_ParseRepeatPattern(format, num7, chr);
							int num10 = 0;
							if (num8 > 8 || flag)
							{
								result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
								return false;
							}
							if (!ParseExactDigits(timeSpanTokenizer, (num8 < 2 ? 1 : num8), (num8 < 2 ? 8 : num8), num10, num1))
							{
								result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
								return false;
							}
							flag = true;
							break;
				}
				case 'e':
				case 'g':
				{
							result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
							return false;
				}
				case 'f':
				{
							num8 = TimeSpanParse_ParseRepeatPattern(format, num7, chr);
							if (num8 > 7 || flag4 || !ParseExactDigits(timeSpanTokenizer, num8, num8, num5, num6))
							{
								result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
								return false;
							}
							flag4 = true;
							break;
				}
				case 'h':
				{
							num8 = TimeSpanParse_ParseRepeatPattern(format, num7, chr);
							if (num8 > 2 || flag1 || !ParseExactDigits(timeSpanTokenizer, num8, num2))
							{
								result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
								return false;
							}
							flag1 = true;
							break;
				}
				default:
				{
						   result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
						   return false;
				}
				}
			}
		}
		else if (chr1 == 'm')
		{
			num8 = TimeSpanParse_ParseRepeatPattern(format, num7, chr);
			if (num8 > 2 || flag2 || !ParseExactDigits(timeSpanTokenizer, num8, num3))
			{
				result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
				return false;
			}
			flag2 = true;
		}
		else
		{
			if (chr1 != 's')
			{
				result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
				return false;
			}
			num8 = TimeSpanParse_ParseRepeatPattern(format, num7, chr);
			if (num8 > 2 || flag3 || !ParseExactDigits(timeSpanTokenizer, num8, num4))
			{
				result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
				return false;
			}
			flag3 = true;
		}
	Label3:
		num7 = num7 + num8;
	}
	result.SetFailure(ParseFailureKind::Format, "Format_InvalidString");
	return false;
}

bool TryParseExactMultipleTimeSpan(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, TimeSpanStyles styles, TimeSpanResult& result)
{
	if (input.empty())
	{
		result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
		return false;
	}	
	if (input.size() == 0)
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (formats.empty())
	{
		result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
		return false;
	}
	int fs = (int)formats.size();
	if ( fs == 0)
	{
		result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
		return false;
	}
	for (int i = 0; i < fs; i++)
	{
		if (formats[i].empty() || formats[i].size() == 0)
		{
			result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
			return false;
		}
		TimeSpanResult timeSpanResult = TimeSpanResult();
		timeSpanResult.Init(TimeSpanThrowStyle::None);
		if (TryParseExactTimeSpan(input, formats[i], loc, styles, timeSpanResult))
		{
			result.parsedTimeSpan = timeSpanResult.parsedTimeSpan;
			return true;
		}
	}
	result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
	return false;
}

std::wstring TimeSpanParse_TrimString(const std::wstring& str)
{
	std::wstring::size_type pos = str.find_first_not_of(' ');
	if (pos == std::wstring::npos)
	{
		return str;
	}
	std::wstring::size_type pos2 = str.find_last_not_of(' ');
	if (pos2 != std::wstring::npos)
	{
		return str.substr(pos, pos2 - pos + 1);
	}
	return str.substr(pos);
}

bool TryParseExactTimeSpan(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles styles, TimeSpanResult& result)
{
	if (input.empty())
	{
		result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
		return false;
	}
	if (format.empty())
	{
		result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
		return false;
	}
	if (format.size() == 0)
	{
		result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
		return false;
	}
	if (format.size() != 1)
	{
		return TryParseByFormat(input, format, styles, result);
	}
	TimeSpanStandardStyles timeSpanStandardStyle = TimeSpanStandardStyles::None;
	if (format[0] == 'c' || format[0] == 't' || format[0] == 'T')
	{
		return TryParseTimeSpanConstant(input, result);
	}
	if (format[0] != 'g')
	{
		if (format[0] != 'G')
		{
			result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
			return false;
		}
		timeSpanStandardStyle = TimeSpanStandardStyles::Localized | TimeSpanStandardStyles::RequireFull;
	}
	else
	{
		timeSpanStandardStyle = TimeSpanStandardStyles::Localized;
	}
	return TryParseTimeSpan(input, timeSpanStandardStyle, loc, result);
}

bool TryParseTimeSpan(const std::wstring& input, TimeSpanStandardStyles style, const CultureInfo& loc, TimeSpanResult& result)
{
	if (input.empty())
	{
		result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
		return false;
	}
	std::wstring inputnew = TimeSpanParse_TrimString(input);
	if (inputnew.empty())
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	TimeSpanTokenizer timeSpanTokenizer = TimeSpanTokenizer();
	timeSpanTokenizer.Init(inputnew);
	TimeSpanRawInfo timeSpanRawInfo = TimeSpanRawInfo();
	timeSpanRawInfo.Init(loc);
	for (TimeSpanToken i = timeSpanTokenizer.GetNextToken(); i.ttt != TTT::End; i = timeSpanTokenizer.GetNextToken())
	{
		if (!timeSpanRawInfo.ProcessToken(i, result))
		{
			result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
			return false;
		}
	}
	if (!timeSpanTokenizer.EOL())
	{
		result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
		return false;
	}
	if (ProcessTerminalState(timeSpanRawInfo, style, result))
	{
		return true;
	}
	result.SetFailure(ParseFailureKind::Format, "bad timeSpan");
	return false;
}

bool TryParseTimeSpanConstant(const std::wstring& input, TimeSpanResult& result)
{
	StringParser sp = StringParser();
	return sp.TryParse(input, result);
}


TimeSpan TimeSpanParse::Parse(const std::wstring& input, const CultureInfo& loc)
{
	TimeSpanResult timeSpanResult = TimeSpanResult();
	timeSpanResult.Init(TimeSpanThrowStyle::All);
	if (!TryParseTimeSpan(input, TimeSpanStandardStyles::Any, loc, timeSpanResult))
	{
		throw timeSpanResult.GetTimeSpanParseException();
	}
	return timeSpanResult.parsedTimeSpan;
}

TimeSpan TimeSpanParse::ParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles styles)
{
	TimeSpanResult timeSpanResult = TimeSpanResult();
	timeSpanResult.Init(TimeSpanThrowStyle::All);
	if (!TryParseExactTimeSpan(input, format, loc, styles, timeSpanResult))
	{
		throw timeSpanResult.GetTimeSpanParseException();
	}
	return timeSpanResult.parsedTimeSpan;
}

TimeSpan TimeSpanParse::ParseExactMultiple(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, TimeSpanStyles styles)
{
	TimeSpanResult timeSpanResult = TimeSpanResult();
	timeSpanResult.Init(TimeSpanThrowStyle::All);
	if (!TryParseExactMultipleTimeSpan(input, formats, loc, styles, timeSpanResult))
	{
		throw timeSpanResult.GetTimeSpanParseException();
	}
	return timeSpanResult.parsedTimeSpan;
}

bool TimeSpanParse::TryParse(const std::wstring& input, const CultureInfo& loc, TimeSpan& result)
{
	TimeSpanResult timeSpanResult = TimeSpanResult();
	timeSpanResult.Init(TimeSpanThrowStyle::None);
	if (!TryParseTimeSpan(input, TimeSpanStandardStyles::Any, loc, timeSpanResult))
	{
		result = TimeSpan();
		return false;
	}
	result = timeSpanResult.parsedTimeSpan;
	return true;
}

bool TimeSpanParse::TryParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles styles, TimeSpan& result)
{
	TimeSpanResult timeSpanResult = TimeSpanResult();
	timeSpanResult.Init(TimeSpanThrowStyle::None);
	if (!TryParseExactTimeSpan(input, format, loc, styles, timeSpanResult))
	{
		result = TimeSpan();
		return false;
	}
	result = timeSpanResult.parsedTimeSpan;
	return true;
}

bool TimeSpanParse::TryParseExactMultiple(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, TimeSpanStyles styles, TimeSpan& result)
{
	TimeSpanResult timeSpanResult = TimeSpanResult();
	timeSpanResult.Init(TimeSpanThrowStyle::None);
	if (!TryParseExactMultipleTimeSpan(input, formats, loc, styles, timeSpanResult))
	{
		result = TimeSpan();
		return false;
	}
	result = timeSpanResult.parsedTimeSpan;
	return true;
}

void TimeSpanParse::ValidateStyles(TimeSpanStyles style)
{
	if (style != TimeSpanStyles::None && style != TimeSpanStyles::AssumeNegative)
	{
		throw std::exception("invalid style");
	}
}

#pragma endregion


__uregister_enum(TimeSpanStyles)
{
	type.AddValue(L"None", TimeSpanStyles::None, __uhashstr(System::TimeSpanStyles::None));
	type.AddValue(L"AssumeNegative", TimeSpanStyles::AssumeNegative, __uhashstr(System::TimeSpanStyles::AssumeNegative));
	BuiltInAssemblyFactory::Regisiter(&type);
}

__inline static Object* Factory_TimeSpan_0() { return new TimeSpan(); }
__inline static Object* Factory_TimeSpan_1(Int64 microseconds) { return new TimeSpan(microseconds); }
__inline static Object* Factory_TimeSpan_2(int hours, int minutes, int seconds) { return new TimeSpan(hours, minutes, seconds); }
__inline static Object* Factory_TimeSpan_3(int days, int hours, int minutes, int seconds) { return new TimeSpan(days, hours, minutes, seconds); }
__inline static Object* Factory_TimeSpan_4(int days, int hours, int minutes, int seconds, int milliseconds) { return new TimeSpan(days, hours, minutes, seconds, milliseconds); }
__inline static Object* Factory_TimeSpan_5(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds) { return new TimeSpan(days, hours, minutes, seconds, milliseconds, microseconds); }
__inline static Object* Factory_TimeSpan_6(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds, int nano100seconds) { return new TimeSpan(days, hours, minutes, seconds, milliseconds, microseconds, nano100seconds); }

__uregister_class(TimeSpan, ClassFlags::Normal)
{
	type.AddDefaultConstructor<TimeSpan>(L"TimeSpan", ConstructorFlags::Default, &Factory_TimeSpan_0);
	type.AddConstructor(L"TimeSpan", ConstructorFlags::Default, ConstructorTypeAdapter<1, TimeSpan>::Create(&Factory_TimeSpan_1));
	type.AddConstructor(L"TimeSpan", ConstructorFlags::Default, ConstructorTypeAdapter<3, TimeSpan>::Create(&Factory_TimeSpan_2));
	type.AddConstructor(L"TimeSpan", ConstructorFlags::Default, ConstructorTypeAdapter<4, TimeSpan>::Create(&Factory_TimeSpan_3));
	type.AddConstructor(L"TimeSpan", ConstructorFlags::Default, ConstructorTypeAdapter<5, TimeSpan>::Create(&Factory_TimeSpan_4));
	type.AddConstructor(L"TimeSpan", ConstructorFlags::Default, ConstructorTypeAdapter<6, TimeSpan>::Create(&Factory_TimeSpan_5));
	type.AddConstructor(L"TimeSpan", ConstructorFlags::Default, ConstructorTypeAdapter<7, TimeSpan>::Create(&Factory_TimeSpan_6));

	type.AddMethod(L"FromDays", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, TimeSpan, TimeSpan>::Create(&TimeSpan::FromDays));
	type.AddMethod(L"FromHours", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, TimeSpan, TimeSpan>::Create(&TimeSpan::FromHours));
	type.AddMethod(L"FromMinutes", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, TimeSpan, TimeSpan>::Create(&TimeSpan::FromMinutes));
	type.AddMethod(L"FromSeconds", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, TimeSpan, TimeSpan>::Create(&TimeSpan::FromSeconds));
	type.AddMethod(L"FromMilliseconds", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, TimeSpan, TimeSpan>::Create(&TimeSpan::FromMilliseconds));
	type.AddMethod(L"FromMicroseconds", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, TimeSpan, TimeSpan>::Create(&TimeSpan::FromMicroseconds));

	type.AddProperty(L"Duration", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetDuration));
	type.AddProperty(L"Negate", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetNegate));

	type.AddProperty(L"Days", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetDays));
	type.AddProperty(L"Hours", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetHours));
	type.AddProperty(L"Milliseconds", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetMilliseconds));
	type.AddProperty(L"Minutes", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetMinutes));
	type.AddProperty(L"Seconds", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetSeconds));
	type.AddProperty(L"Microseconds", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetMicroseconds));
	type.AddProperty(L"TotalDays", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetTotalDays));
	type.AddProperty(L"TotalHours", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetTotalHours));
	type.AddProperty(L"TotalMilliseconds", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetTotalMilliseconds));
	type.AddProperty(L"TotalMinutes", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetTotalMinutes));
	type.AddProperty(L"TotalSeconds", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetTotalSeconds));
	type.AddProperty(L"TotalMicroseconds", PropertyFlags::Public, PropertyTypeOf(&TimeSpan::GetTotalMicroseconds));


	type.AddAttribute(new TypeConverterAttribute(L"TimeSpanConverter", TimeSpanConverter::GetSharedInstance()));

	type.AddField(L"span", FieldFlags::Default, FieldTypeOf<FieldPolicy::ReadOnly>(&TimeSpan::span));

	BuiltInAssemblyFactory::Regisiter(&type);

};