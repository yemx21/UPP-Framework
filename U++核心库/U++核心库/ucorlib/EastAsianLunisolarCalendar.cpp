#include "EastAsianLunisolarCalendar.h"
#include "BuiltInAssembly.h"
#include "CalendarHelper.h"
using namespace System;

static const int  DaysToMonth365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
static const int  DaysToMonth366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

CalendarAlgorithmType EastAsianLunisolarCalendar::GetAlgorithmType() const
{
	return CalendarAlgorithmType::LunisolarCalendar;
}

int EastAsianLunisolarCalendar::GetTwoDigitYearMax() const
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetBaseCalendarID(), GetYear(DateTime(2029, 1, 1)));
	}
	return twoDigitYearMax;
}

void EastAsianLunisolarCalendar::SetTwoDigitYearMax(int value)
{
	Calendar::VerifyWritable();
	if (value < 99 || value > GetMaxCalendarYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	twoDigitYearMax = value;
}


EastAsianLunisolarCalendar::EastAsianLunisolarCalendar()
{
}

EastAsianLunisolarCalendar::~EastAsianLunisolarCalendar()
{

}

DateTime EastAsianLunisolarCalendar::AddMonths(const DateTime& time, int months)
{
	int num = 0;
	if (months < -120000 || months > 120000)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	CheckTicksRange(time.Ticks);
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	TimeToLunar(time, num1, num2, num3);
	int num4 = num2 + months;
	if (num4 <= 0)
	{
		while (num4 <= 0)
		{
			num4 = num4 + (InternalIsLeapYear(num1 - 1) ? 13 : 12);
			num1--;
		}
		num2 = num4;
	}
	else
	{
		for (int i = (InternalIsLeapYear(num1) ? 13 : 12); num4 - i > 0; i = num)
		{
			num4 = num4 - i;
			num1++;
			num = (InternalIsLeapYear(num1) ? 13 : 12);
		}
		num2 = num4;
	}
	int num5 = InternalGetDaysInMonth(num1, num2);
	if (num3 > num5)
	{
		num3 = num5;
	}
	DateTime dateTime = LunarToTime(time, num1, num2, num3);
	Calendar::CheckAddResult(dateTime.Ticks, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return dateTime;
}

DateTime EastAsianLunisolarCalendar::AddYears(const DateTime& time, int years)
{
	CheckTicksRange(time.Ticks);
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	TimeToLunar(time, num, num1, num2);
	num = num + years;
	if (num1 == 13 && !InternalIsLeapYear(num))
	{
		num1 = 12;
		num2 = InternalGetDaysInMonth(num, num1);
	}
	int num3 = InternalGetDaysInMonth(num, num1);
	if (num2 > num3)
	{
		num2 = num3;
	}
	DateTime dateTime = LunarToTime(time, num, num1, num2);
	Calendar::CheckAddResult(dateTime.Ticks, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return dateTime;
}

void EastAsianLunisolarCalendar::CheckEraRange(int era) const
{
	if (era == 0)
	{
		era = GetCurrentEraValue();
	}
	if (era < GetEra(GetMinDate()) || era > GetEra(GetMaxDate()))
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
}

void EastAsianLunisolarCalendar::CheckTicksRange(Int64 ticks) const
{
	if (ticks < GetMinSupportedDateTime().Ticks || ticks > GetMaxSupportedDateTime().Ticks)
	{
		throw std::exception("ArgumentOutOfRange_CalendarRange");
	}
}

int EastAsianLunisolarCalendar::CheckYearMonthRange(int year, int month, int era) const
{
	year = CheckYearRange(year, era);
	if (month == 13 && GetYearInfo(year, 0) == 0)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	if (month < 1 || month > 13)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	return year;
}

int EastAsianLunisolarCalendar::CheckYearRange(int year, int era) const
{
	CheckEraRange(era);
	year = GetGregorianYear(year, era);
	if (year < GetMinCalendarYear() || year > GetMaxCalendarYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}

int EastAsianLunisolarCalendar::GetCelestialStem(int sexagenaryYear) const
{
	if (sexagenaryYear < 1 || sexagenaryYear > 60)
	{
		throw std::exception("ArgumentOutOfRange_Range");
	}
	return (sexagenaryYear - 1) % 10 + 1;
}

int EastAsianLunisolarCalendar::GetDayOfMonth(const DateTime& time)  const
{
	CheckTicksRange(time.Ticks);
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	TimeToLunar(time, num, num1, num2);
	return num2;
}

DayOfWeek EastAsianLunisolarCalendar::GetDayOfWeek(const DateTime& time)  const
{
	CheckTicksRange(time.Ticks);
	return (DayOfWeek)((int)(time.Ticks / TimeSpan::DAYS + 1) % 7);
}

int EastAsianLunisolarCalendar::GetDayOfYear(const DateTime& time)  const
{
	CheckTicksRange(time.Ticks);
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	TimeToLunar(time, num, num1, num2);
	for (int i = 1; i < num1; i++)
	{
		num2 = num2 + InternalGetDaysInMonth(num, i);
	}
	return num2;
}

int EastAsianLunisolarCalendar::GetDaysInMonth(int year, int month, int era)  const
{
	year = CheckYearMonthRange(year, month, era);
	return InternalGetDaysInMonth(year, month);
}

int EastAsianLunisolarCalendar::GetDaysInYear(int year, int era)  const
{
	year = CheckYearRange(year, era);
	int num = 0;
	int num1 = (InternalIsLeapYear(year) ? 13 : 12);
	while (num1 != 0)
	{
		int num2 = num1;
		num1 = num2 - 1;
		num = num + InternalGetDaysInMonth(year, num2);
	}
	return num;
}

int EastAsianLunisolarCalendar::GetLeapMonth(int year, int era) const
{
	year = CheckYearRange(year, era);
	int yearInfo = GetYearInfo(year, 0);
	if (yearInfo <= 0)
	{
		return 0;
	}
	return yearInfo + 1;
}

int EastAsianLunisolarCalendar::GetMonth(const DateTime& time) const
{
	CheckTicksRange(time.Ticks);
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	TimeToLunar(time, num, num1, num2);
	return num1;
}

int EastAsianLunisolarCalendar::GetMonthsInYear(int year, int era) const
{
	year = CheckYearRange(year, era);
	if (!InternalIsLeapYear(year))
	{
		return 12;
	}
	return 13;
}

int EastAsianLunisolarCalendar::GetSexagenaryYear(const DateTime& time) const
{
	CheckTicksRange(time.Ticks);
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	TimeToLunar(time, num, num1, num2);
	return (num - 4) % 60 + 1;
}

int EastAsianLunisolarCalendar::GetTerrestrialBranch(int sexagenaryYear) const
{
	if (sexagenaryYear < 1 || sexagenaryYear > 60)
	{
		throw std::exception("ArgumentOutOfRange_Range");
	}
	return (sexagenaryYear - 1) % 12 + 1;
}

int EastAsianLunisolarCalendar::GetYear(const DateTime& time)  const
{
	CheckTicksRange(time.Ticks);
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	TimeToLunar(time, num, num1, num2);
	return GetYear(num, time);
}


int EastAsianLunisolarCalendar::GregorianIsLeapYear(int y)
{
	if (y % 4 != 0)
	{
		return 0;
	}
	if (y % 100 != 0)
	{
		return 1;
	}
	if (y % 400 == 0)
	{
		return 1;
	}
	return 0;
}
void EastAsianLunisolarCalendar::GregorianToLunar(int nSYear, int nSMonth, int nSDate, int& nLYear, int& nLMonth, int& nLDate) const
{
	int yearInfo;
	int num;
	int daysToMonth365 = (EastAsianLunisolarCalendar::GregorianIsLeapYear(nSYear) == 1 ? DaysToMonth366[nSMonth - 1] : DaysToMonth365[nSMonth - 1]) + nSDate;
	nLYear = nSYear;
	if (nLYear != GetMaxCalendarYear() + 1)
	{
		yearInfo = GetYearInfo(nLYear, 1);
		num = GetYearInfo(nLYear, 2);
		if (nSMonth < yearInfo || nSMonth == yearInfo && nSDate < num)
		{
			nLYear = nLYear - 1;
			daysToMonth365 = daysToMonth365 + (EastAsianLunisolarCalendar::GregorianIsLeapYear(nLYear) == 1 ? 366 : 365);
			yearInfo = GetYearInfo(nLYear, 1);
			num = GetYearInfo(nLYear, 2);
		}
	}
	else
	{
		nLYear = nLYear - 1;
		daysToMonth365 = daysToMonth365 + (EastAsianLunisolarCalendar::GregorianIsLeapYear(nLYear) == 1 ? 366 : 365);
		yearInfo = GetYearInfo(nLYear, 1);
		num = GetYearInfo(nLYear, 2);
	}
	daysToMonth365 = daysToMonth365 - DaysToMonth365[yearInfo - 1];
	daysToMonth365 = daysToMonth365 - (num - 1);
	int num1 = 32768;
	int yearInfo1 = GetYearInfo(nLYear, 3);
	int num2 = ((yearInfo1 & num1) != 0 ? 30 : 29);
	nLMonth = 1;
	while (daysToMonth365 > num2)
	{
		daysToMonth365 = daysToMonth365 - num2;
		nLMonth = nLMonth + 1;
		num1 = num1 >> 1;
		num2 = ((yearInfo1 & num1) != 0 ? 30 : 29);
	}
	nLDate = daysToMonth365;
}

int EastAsianLunisolarCalendar::InternalGetDaysInMonth(int year, int month) const
{
	int num;
	num = ((GetYearInfo(year, 3) & 32768 >> (month - 1 & 31)) != 0 ? 30 : 29);
	return num;
}

bool EastAsianLunisolarCalendar::InternalIsLeapYear(int year) const
{
	return GetYearInfo(year, 0) != 0;
}

bool EastAsianLunisolarCalendar::IsLeapDay(int year, int month, int day, int era)  const
{
	year = CheckYearMonthRange(year, month, era);
	int num = InternalGetDaysInMonth(year, month);
	if (day < 1 || day > num)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	int yearInfo = GetYearInfo(year, 0);
	if (yearInfo == 0)
	{
		return false;
	}
	return month == yearInfo + 1;
}

bool EastAsianLunisolarCalendar::IsLeapMonth(int year, int month, int era)  const
{
	year = CheckYearMonthRange(year, month, era);
	int yearInfo = GetYearInfo(year, 0);
	if (yearInfo == 0)
	{
		return false;
	}
	return month == yearInfo + 1;
}

bool EastAsianLunisolarCalendar::IsLeapYear(int year, int era)  const
{
	year = CheckYearRange(year, era);
	return InternalIsLeapYear(year);
}

bool EastAsianLunisolarCalendar::LunarToGregorian(int nLYear, int nLMonth, int nLDate, int& nSolarYear, int& nSolarMonth, int& nSolarDay) const
{
	if (nLDate < 1 || nLDate > 30)
	{
		return false;
	}
	int num = nLDate - 1;
	for (int i = 1; i < nLMonth; i++)
	{
		num = num + InternalGetDaysInMonth(nLYear, i);
	}
	int yearInfo = GetYearInfo(nLYear, 1);
	int yearInfo1 = GetYearInfo(nLYear, 2);
	int num1 = EastAsianLunisolarCalendar::GregorianIsLeapYear(nLYear);
	const int* numArray = (num1 == 1 ? DaysToMonth366 : DaysToMonth365);
	nSolarDay = yearInfo1;
	if (yearInfo > 1)
	{
		nSolarDay = nSolarDay + numArray[yearInfo - 1];
	}
	nSolarDay = nSolarDay + num;
	if (nSolarDay <= num1 + 365)
	{
		nSolarYear = nLYear;
	}
	else
	{
		nSolarYear = nLYear + 1;
		nSolarDay = nSolarDay - (num1 + 365);
	}
	nSolarMonth = 1;
	while (nSolarMonth < 12 && numArray[nSolarMonth] < nSolarDay)
	{
		nSolarMonth = nSolarMonth + 1;
	}
	nSolarDay = nSolarDay - numArray[nSolarMonth - 1];
	return true;
}

DateTime EastAsianLunisolarCalendar::LunarToTime(const DateTime& time, int year, int month, int day) const
{
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	LunarToGregorian(year, month, day, num, num1, num2);
	return  CalendarHelper::GetGregorianDefaultInstance().ToDateTime(num, num1, num2, time.Hour, time.Minute, time.Second, time.Millisecond, 0);
}

int EastAsianLunisolarCalendar::MaxEraCalendarYear(int era) const
{
	std::vector<EraInfo>calEraInfo;
	GetCalEraInfo(calEraInfo);

	if (calEraInfo.size() == 0)
	{
		return GetMaxCalendarYear();
	}
	if (era == 0)
	{
		era = GetCurrentEraValue();
	}
	if (era == GetEra(GetMaxDate()))
	{
		return GetYear(GetMaxCalendarYear(), GetMaxDate());
	}
	for (int i = 0; i < (int)calEraInfo.size(); i++)
	{
		if (era == calEraInfo[i].era)
		{
			return calEraInfo[i].maxEraYear;
		}
	}
	throw std::exception("ArgumentOutOfRange_InvalidEraValue");
}

int EastAsianLunisolarCalendar::MinEraCalendarYear(int era) const
{
	std::vector<EraInfo>calEraInfo;
	GetCalEraInfo(calEraInfo);
	if (calEraInfo.size() == 0)
	{
		return GetMinCalendarYear();
	}
	if (era == 0)
	{
		era = GetCurrentEraValue();
	}
	if (era == GetEra(GetMinDate()))
	{
		return GetYear(GetMinCalendarYear(), GetMinDate());
	}
	for (int i = 0; i < (int)calEraInfo.size(); i++)
	{
		if (era == calEraInfo[i].era)
		{
			return calEraInfo[i].minEraYear;
		}
	}
	throw std::exception("ArgumentOutOfRange_InvalidEraValue");
}

void EastAsianLunisolarCalendar::TimeToLunar(const DateTime& time, int& year, int& month, int& day) const
{
	int num = 0;
	int num1 = 0;
	int dayOfMonth = 0;
	auto& defaultInstance = CalendarHelper::GetGregorianDefaultInstance();
	num = defaultInstance.GetYear(time);
	num1 = defaultInstance.GetMonth(time);
	dayOfMonth = defaultInstance.GetDayOfMonth(time);
	GregorianToLunar(num, num1, dayOfMonth, year, month, day);
}

DateTime EastAsianLunisolarCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era)  const
{
	year = CheckYearMonthRange(year, month, era);
	int num = InternalGetDaysInMonth(year, month);
	if (day < 1 || day > num)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	if (!LunarToGregorian(year, month, day, num1, num2, num3))
	{
		throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
	}
	return DateTime(num1, num2, num3, hour, minute, second, millisecond);
}

int EastAsianLunisolarCalendar::ToFourDigitYear(int year)  const
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	year = Calendar::ToFourDigitYear(year);
	CheckYearRange(year, 0);
	return year;
}

__uregister_class(EastAsianLunisolarCalendar, ClassFlags::Abstract)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}