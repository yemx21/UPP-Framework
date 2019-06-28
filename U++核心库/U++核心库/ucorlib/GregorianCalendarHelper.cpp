#include "GregorianCalendarHelper.h"
#include "CalendarHelper.h"
using namespace System;

const int GregorianCalendarHelper::DaysToMonth365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
const int GregorianCalendarHelper::DaysToMonth366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

std::vector<int> GregorianCalendarHelper::GetEras() const
{
	if (!m_eras)
	{
		m_eras = std::vector<int>(m_EraInfo.size());
		for (int i = 0; i < (int)m_EraInfo.size(); i++)
		{
			(*m_eras)[i] = m_EraInfo[i].era;
		}
	}
	return *m_eras;
}

int GregorianCalendarHelper::GetMaxYear() const
{
	return m_maxYear;
}

GregorianCalendarHelper::GregorianCalendarHelper(Calendar* cal, const std::vector<EraInfo>& eraInfo)
{
	m_Cal = cal;
	m_EraInfo = eraInfo;
	m_minDate = m_Cal->GetMinSupportedDateTime();
	m_maxYear = m_EraInfo[0].maxEraYear;
	m_minYear = m_EraInfo[0].minEraYear;
}

GregorianCalendarHelper::~GregorianCalendarHelper()
{

}

DateTime GregorianCalendarHelper::AddMonths(const DateTime& time, int months)
{
	if (months < -120000 || months > 120000)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	CheckTicksRange(time.Ticks);
	int datePart = GetDatePart(time.Ticks, 0);
	int num = GetDatePart(time.Ticks, 2);
	int datePart1 = GetDatePart(time.Ticks, 3);
	int num1 = num - 1 + months;
	if (num1 < 0)
	{
		num = 12 + (num1 + 1) % 12;
		datePart = datePart + (num1 - 11) / 12;
	}
	else
	{
		num = num1 % 12 + 1;
		datePart = datePart + num1 / 12;
	}
	const int* numArray = (datePart % 4 != 0 || datePart % 100 == 0 && datePart % 400 != 0 ? GregorianCalendarHelper::DaysToMonth365 : GregorianCalendarHelper::DaysToMonth366);
	int num2 = numArray[num] - numArray[num - 1];
	if (datePart1 > num2)
	{
		datePart1 = num2;
	}
	Int64 ticks = GregorianCalendarHelper::DateToTicks(datePart, num, datePart1) + time.Ticks % TimeSpan::DAYS;
	Calendar::CheckAddResult(ticks, m_Cal->GetMinSupportedDateTime(), m_Cal->GetMaxSupportedDateTime());
	return DateTime(ticks);
}

DateTime GregorianCalendarHelper::AddYears(const DateTime& time, int years)
{
	return AddMonths(time, years * 12);
}

void GregorianCalendarHelper::CheckTicksRange(Int64 ticks) const
{
	if (ticks < m_Cal->GetMinSupportedDateTime().Ticks || ticks > m_Cal->GetMaxSupportedDateTime().Ticks)
	{
		throw std::exception("ArgumentOutOfRange_CalendarRange");
	}
}

Int64 GregorianCalendarHelper::DateToTicks(int year, int month, int day)
{
	return GregorianCalendarHelper::GetAbsoluteDate(year, month, day) * TimeSpan::DAYS;
}

long GregorianCalendarHelper::GetAbsoluteDate(int year, int month, int day)
{
	if (year >= 1 && year <= 9999 && month >= 1 && month <= 12)
	{
		const int* numArray = (year % 4 != 0 || year % 100 == 0 && year % 400 != 0 ? GregorianCalendarHelper::DaysToMonth365 : GregorianCalendarHelper::DaysToMonth366);
		if (day >= 1 && day <= numArray[month] - numArray[month - 1])
		{
			int num = year - 1;
			int num1 = num * 365 + num / 4 - num / 100 + num / 400 + numArray[month - 1] + day - 1;
			return (long)num1;
		}
	}
	throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
}

int GregorianCalendarHelper::GetDatePart(Int64 ticks, int part) const
{
	bool flag;
	CheckTicksRange(ticks);
	int num = (int)(ticks / TimeSpan::DAYS);
	int num1 = num / 146097;
	num = num - num1 * 146097;
	int num2 = num / 36524;
	if (num2 == 4)
	{
		num2 = 3;
	}
	num = num - num2 * 36524;
	int num3 = num / 1461;
	num = num - num3 * 1461;
	int num4 = num / 365;
	if (num4 == 4)
	{
		num4 = 3;
	}
	if (part == 0)
	{
		return num1 * 400 + num2 * 100 + num3 * 4 + num4 + 1;
	}
	num = num - num4 * 365;
	if (part == 1)
	{
		return num + 1;
	}
	if (num4 != 3)
	{
		flag = false;
	}
	else
	{
		flag = (num3 != 24 ? true : num2 == 3);
	}
	const int*  numArray = (flag ? GregorianCalendarHelper::DaysToMonth366 : GregorianCalendarHelper::DaysToMonth365);
	int num5 = num >> 6;
	while (num >= numArray[num5])
	{
		num5++;
	}
	if (part == 2)
	{
		return num5;
	}
	return num - numArray[num5 - 1] + 1;
}

int GregorianCalendarHelper::GetDayOfMonth(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 3);
}

DayOfWeek GregorianCalendarHelper::GetDayOfWeek(const DateTime& time) const
{
	CheckTicksRange(time.Ticks);
	return (DayOfWeek)((int)((time.Ticks / TimeSpan::DAYS + 1) % (long)7));
}

int GregorianCalendarHelper::GetDayOfYear(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 1);
}

int GregorianCalendarHelper::GetDaysInMonth(int year, int month, int era) const
{
	year = GetGregorianYear(year, era);
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	const int* numArray = (year % 4 != 0 || year % 100 == 0 && year % 400 != 0 ? GregorianCalendarHelper::DaysToMonth365 : GregorianCalendarHelper::DaysToMonth366);
	return numArray[month] - numArray[month - 1];
}

int GregorianCalendarHelper::GetDaysInYear(int year, int era)  const
{
	year = GetGregorianYear(year, era);
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
	{
		return 366;
	}
	return 365;
}

int GregorianCalendarHelper::GetEra(const DateTime& time) const
{
	Int64 ticks = time.Ticks;
	for (int i = 0; i < (int)m_EraInfo.size(); i++)
	{
		if (ticks >= m_EraInfo[i].ticks)
		{
			return m_EraInfo[i].era;
		}
	}
	throw std::exception("ArgumentOutOfRange_Era");
}

int GregorianCalendarHelper::GetGregorianYear(int year, int era) const
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	if (era == 0)
	{
		era = m_Cal->GetCurrentEraValue();
	}
	for (int i = 0; i < (int)m_EraInfo.size(); i++)
	{
		if (era == m_EraInfo[i].era)
		{
			if (year < m_EraInfo[i].minEraYear || year > m_EraInfo[i].maxEraYear)
			{
				throw std::exception("ArgumentOutOfRange_Year");
			}
			return m_EraInfo[i].yearOffset + year;
		}
	}
	throw std::exception("ArgumentOutOfRange_InvalidEraValue");
}

int GregorianCalendarHelper::GetLeapMonth(int year, int era) const
{
	year = GetGregorianYear(year, era);
	return 0;
}

int GregorianCalendarHelper::GetMonth(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 2);
}

int GregorianCalendarHelper::GetMonthsInYear(int year, int era) const
{
	year = GetGregorianYear(year, era);
	return 12;
}

int GregorianCalendarHelper::GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek)  const
{
	CheckTicksRange(time.Ticks);
	return CalendarHelper::GetGregorianDefaultInstance().GetWeekOfYear(time, rule, firstDayOfWeek);
}

int GregorianCalendarHelper::GetYear(const DateTime& time)  const
{
	Int64 ticks = time.Ticks;
	int datePart = GetDatePart(ticks, 0);
	for (int i = 0; i < (int)m_EraInfo.size(); i++)
	{
		if (ticks >= m_EraInfo[i].ticks)
		{
			return datePart - m_EraInfo[i].yearOffset;
		}
	}
	throw std::exception("Argument_NoEra");
}

int GregorianCalendarHelper::GetYear(int year, const DateTime& time)  const
{
	Int64 ticks = time.Ticks;
	for (int i = 0; i < (int)m_EraInfo.size(); i++)
	{
		if (ticks >= m_EraInfo[i].ticks)
		{
			return year - m_EraInfo[i].yearOffset;
		}
	}
	throw std::exception("Argument_NoEra");
}

bool GregorianCalendarHelper::IsLeapDay(int year, int month, int day, int era)  const
{
	if (day < 1 || day > GetDaysInMonth(year, month, era))
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	if (!IsLeapYear(year, era))
	{
		return false;
	}
	if (month == 2 && day == 29)
	{
		return true;
	}
	return false;
}

bool GregorianCalendarHelper::IsLeapMonth(int year, int month, int era)  const
{
	year = GetGregorianYear(year, era);
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	return false;
}

bool GregorianCalendarHelper::IsLeapYear(int year, int era)  const
{
	year = GetGregorianYear(year, era);
	if (year % 4 != 0)
	{
		return false;
	}
	if (year % 100 != 0)
	{
		return true;
	}
	return year % 400 == 0;
}

bool GregorianCalendarHelper::IsValidYear(int year, int era) const
{
	if (year < 0)
	{
		return false;
	}
	if (era == 0)
	{
		era = m_Cal->GetCurrentEraValue();
	}
	for (int i = 0; i < (int)m_EraInfo.size(); i++)
	{
		if (era == m_EraInfo[i].era)
		{
			if (year >= m_EraInfo[i].minEraYear && year <= m_EraInfo[i].maxEraYear)
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

Int64 GregorianCalendarHelper::TimeToTicks(int hour, int minute, int second, int millisecond)
{
	if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60 || second < 0 || second >= 60)
	{
		throw std::exception("ArgumentOutOfRange_BadHourMinuteSecond");
	}
	if (millisecond < 0 || millisecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Millisecond");
	}
	return TimeSpan::TimeToTicks(0, hour, minute, second, millisecond, 0,0);
}

DateTime GregorianCalendarHelper::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era)  const
{
	year = GetGregorianYear(year, era);
	Int64 ticks = GregorianCalendarHelper::DateToTicks(year, month, day) + GregorianCalendarHelper::TimeToTicks(hour, minute, second, millisecond);
	CheckTicksRange(ticks);
	return DateTime(ticks);
}

int GregorianCalendarHelper::ToFourDigitYear(int year, int twoDigitYearMax)  const
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedPosNum_Year");
	}
	if (year < 100)
	{
		int num = year % 100;
		return (twoDigitYearMax / 100 - (num > twoDigitYearMax % 100 ? 1 : 0)) * 100 + num;
	}
	if (year < m_minYear || year > m_maxYear)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}