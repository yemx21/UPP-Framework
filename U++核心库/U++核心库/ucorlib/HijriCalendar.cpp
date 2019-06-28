#include "HijriCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

static const int HijriMonthDays[] = { 0, 30, 59, 89, 118, 148, 177, 207, 236, 266, 295, 325, 355 };
static const DateTime calendarMinValue = DateTime(622, 7, 18);
static const DateTime calendarMaxValue = DateTime::MaxValue;

void CheckEraRange(int era)
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
}

void CheckTicksRange(Int64 ticks)
{
	if (ticks < calendarMinValue.Ticks || ticks > calendarMaxValue.Ticks)
	{
		throw std::exception("ArgumentOutOfRange_CalendarRange");
	}
}

void CheckYearRange(int year, int era)
{
	CheckEraRange(era);
	if (year < 1 || year > 9666)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
}

void CheckYearMonthRange(int year, int month, int era)
{
	CheckYearRange(year, era);
	if (year == 9666 && month > 4)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
}



Calendar* HijriCalendar::InstanceClone() const
{
	HijriCalendar* cal = new HijriCalendar();
	cal->m_HijriAdvance = m_HijriAdvance;
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType HijriCalendar::GetAlgorithmType() const 
{
	return CalendarAlgorithmType::LunarCalendar;
}

int HijriCalendar::GetDaysInYearBeforeMinSupportedYear() const 
{
	return 354;
}

std::vector<int> HijriCalendar::GetEras() const 
{
	return { 1 };
}

int HijriCalendar::GetHijriAdjustment() const
{
	return m_HijriAdvance;
}

void HijriCalendar::SetHijriAdjustment(int value)
{
	if (value < -2 || value > 2)
	{
		throw std::exception("ArgumentOutOfRange_Bounds_Lower_Upper_HijriAdjustment");
	}
	VerifyWritable();
	m_HijriAdvance = value;
}

int HijriCalendar::GetID() const 
{
	return 6;
}

DateTime HijriCalendar::GetMaxSupportedDateTime() const 
{
	return calendarMaxValue;
}

DateTime HijriCalendar::GetMinSupportedDateTime() const 
{
	return calendarMinValue;
}

int HijriCalendar::GetTwoDigitYearMax() const 
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = GetSystemTwoDigitYearSetting(GetID(), 1451);
	}
	return twoDigitYearMax;
}

void HijriCalendar::SetTwoDigitYearMax(int value)
{
	VerifyWritable();
	if (value < 99 || value > 9666)
	{
		throw std::exception("Invalid Year");
	}
	twoDigitYearMax = value;
}

HijriCalendar::HijriCalendar() : m_HijriAdvance (0)
{
}

HijriCalendar::~HijriCalendar() {}

DateTime HijriCalendar::AddMonths(const DateTime& time, int months) 
{
	if (months < -120000 || months > 120000)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
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
	int daysInMonth = GetDaysInMonth(datePart, num, 0);
	if (datePart1 > daysInMonth)
	{
		datePart1 = daysInMonth;
	}
	Int64 absoluteDateHijri = GetAbsoluteDateHijri(datePart, num, datePart1) * TimeSpan::DAYS + time.Ticks % TimeSpan::DAYS;
	Calendar::CheckAddResult(absoluteDateHijri, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return DateTime(absoluteDateHijri);
}

DateTime HijriCalendar::AddYears(const DateTime& time, int years) 
{
	return AddMonths(time, years * 12);
}


int HijriCalendar::GetDatePart(Int64 ticks, int part) const
{
	CheckTicksRange(ticks);
	Int64 hijriAdjustment = ticks / TimeSpan::DAYS + (Int64)1;
	hijriAdjustment = hijriAdjustment + (Int64)GetHijriAdjustment();
	int num = (int)((hijriAdjustment - 227013LL) * 30LL / 10631LL) + 1;
	Int64 hijriYear = DaysUpToHijriYear(num);
	Int64 daysInYear = (Int64)GetDaysInYear(num, 0);
	if (hijriAdjustment < hijriYear)
	{
		hijriYear = hijriYear - daysInYear;
		num--;
	}
	else if (hijriAdjustment == hijriYear)
	{
		num--;
		hijriYear = hijriYear - (Int64)GetDaysInYear(num, 0);
	}
	else if (hijriAdjustment > hijriYear + daysInYear)
	{
		hijriYear = hijriYear + daysInYear;
		num++;
	}
	if (part == 0)
	{
		return num;
	}
	int num1 = 1;
	hijriAdjustment = hijriAdjustment - hijriYear;
	if (part == 1)
	{
		return (int)hijriAdjustment;
	}
	while (num1 <= 12 && hijriAdjustment > (Int64)HijriMonthDays[num1 - 1])
	{
		num1++;
	}
	num1--;
	if (part == 2)
	{
		return num1;
	}
	int hijriMonthDays = (int)(hijriAdjustment - (Int64)HijriMonthDays[num1 - 1]);
	if (part != 3)
	{
		throw std::exception("InvalidOperation_DateTimeParsing");
	}
	return hijriMonthDays;
}

int HijriCalendar::GetDayOfMonth(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 3);
}

DayOfWeek HijriCalendar::GetDayOfWeek(const DateTime& time) const 
{
	return (DayOfWeek)((int)(time.Ticks / TimeSpan::DAYS + 1LL) % 7);
}

int HijriCalendar::GetDayOfYear(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 1);
}

int HijriCalendar::GetDaysInMonth(int year, int month, int era) const 
{
	CheckYearMonthRange(year, month, era);
	if (month == 12)
	{
		if (!IsLeapYear(year, 0))
		{
			return 29;
		}
		return 30;
	}
	if (month % 2 != 1)
	{
		return 29;
	}
	return 30;
}

int HijriCalendar::GetDaysInYear(int year, int era) const 
{
	CheckYearRange(year, era);
	if (!IsLeapYear(year, 0))
	{
		return 354;
	}
	return 355;
}

Int64 HijriCalendar::DaysUpToHijriYear(int HijriYear) const
{
	int hijriYear = (HijriYear - 1) / 30 * 30;
	int num = HijriYear - hijriYear - 1;
	Int64 num1 = (Int64)hijriYear * 10631LL / 30LL + 227013LL;
	while (num > 0)
	{
		num1 = num1 + (Int64)(354 + (IsLeapYear(num, 0) ? 1 : 0));
		num--;
	}
	return num1;
}


Int64 HijriCalendar::GetAbsoluteDateHijri(int y, int m, int d) const
{
	return DaysUpToHijriYear(y) + (Int64)HijriMonthDays[m - 1] + (Int64)d - 1LL - (Int64)GetHijriAdjustment();
}

int HijriCalendar::GetEra(const DateTime& time) const 
{
	CheckTicksRange(time.Ticks);
	return 1;
}

int HijriCalendar::GetLeapMonth(int year, int era) const 
{
	CheckYearRange(year, era);
	return 0;
}

int HijriCalendar::GetMonth(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 2);
}

int HijriCalendar::GetMonthsInYear(int year, int era) const 
{
	CheckYearRange(year, era);
	return 12;
}

int HijriCalendar::GetYear(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 0);
}

bool HijriCalendar::IsLeapDay(int year, int month, int day, int era)  const 
{
	int daysInMonth = GetDaysInMonth(year, month, era);
	if (day < 1 || day > daysInMonth)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	if (!IsLeapYear(year, era) || month != 12)
	{
		return false;
	}
	return day == 30;
}

bool HijriCalendar::IsLeapMonth(int year, int month, int era)  const 
{
	CheckYearMonthRange(year, month, era);
	return false;
}

bool HijriCalendar::IsLeapYear(int year, int era) const 
{
	CheckYearRange(year, era);
	return (year * 11 + 14) % 30 < 11;
}

DateTime HijriCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const 
{
	int daysInMonth = GetDaysInMonth(year, month, era);
	if (day < 1 || day > daysInMonth)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	Int64 absoluteDateHijri = GetAbsoluteDateHijri(year, month, day);
	if (absoluteDateHijri < (Int64)0)
	{
		throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
	}
	return DateTime(absoluteDateHijri * TimeSpan::DAYS + TimeToTicks(hour, minute, second, millisecond));
}

int HijriCalendar::ToFourDigitYear(int year) const 
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum");
	}
	if (year < 100)
	{
		return Calendar::ToFourDigitYear(year);
	}
	if (year > 9666)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}

__uregister_class(HijriCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}