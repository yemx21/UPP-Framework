#include "PersianCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

static const int DaysToMonth[] ={ 0, 31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336 };
static const int LeapYears33[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 };
static const DateTime minDate = DateTime(622, 3, 21);
static const DateTime maxDate = DateTime::MaxValue;

static void CheckEraRange(int era)
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
}

static void CheckTicksRange(Int64 ticks)
{
	if (ticks < minDate.Ticks || ticks > maxDate.Ticks)
	{
		throw std::exception("ArgumentOutOfRange_CalendarRange");
	}
}

static void CheckYearRange(int year, int era)
{
	CheckEraRange(era);
	if (year < 1 || year > 9378)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
}

static void CheckYearMonthRange(int year, int month, int era)
{
	CheckYearRange(year, era);
	if (year == 9378 && month > 10)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
}



Calendar* PersianCalendar::InstanceClone() const
{
	PersianCalendar* cal = new PersianCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType PersianCalendar::GetAlgorithmType() const 
{
	return CalendarAlgorithmType::SolarCalendar;
}

int PersianCalendar::GetBaseCalendarID() const 
{
	return 1;
}

std::vector<int> PersianCalendar::GetEras() const 
{
	return{ 1 };
}

int PersianCalendar::GetID() const 
{
	return 22;
}

DateTime PersianCalendar::GetMaxSupportedDateTime() const 
{
	return maxDate;
}

DateTime PersianCalendar::GetMinSupportedDateTime() const 
{
	return minDate;
}

int PersianCalendar::GetTwoDigitYearMax() const 
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = GetSystemTwoDigitYearSetting(GetID(), 1410);
	}
	return twoDigitYearMax;
}

void PersianCalendar::SetTwoDigitYearMax(int value) 
{
	VerifyWritable();
	if (value < 99 || value > 9378)
	{
		throw std::exception("ArgumentOutOfRange");
	}
	twoDigitYearMax = value;
}

PersianCalendar::PersianCalendar()
{
}

PersianCalendar::~PersianCalendar() {}

DateTime PersianCalendar::AddMonths(const DateTime& time, int months) 
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
	Int64 absoluteDatePersian = GetAbsoluteDatePersian(datePart, num, datePart1) * TimeSpan::DAYS + time.Ticks % TimeSpan::DAYS;
	Calendar::CheckAddResult(absoluteDatePersian, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return DateTime(absoluteDatePersian);
}

DateTime PersianCalendar::AddYears(const DateTime& time, int years) 
{
	return AddMonths(time, years * 12);
}


Int64 PersianCalendar::DaysUpToPersianYear(int PersianYear) const
{
	int persianYear = (PersianYear - 1) / 33;
	int num = (PersianYear - 1) % 33;
	Int64 num1 = (Int64)persianYear * 12053LL + 226894LL;
	while (num > 0)
	{
		num1 = num1 + 365LL;
		if (IsLeapYear(num, 0))
		{
			num1 = num1 + (Int64)1;
		}
		num--;
	}
	return num1;
}

Int64 PersianCalendar::GetAbsoluteDatePersian(int year, int month, int day) const
{
	if (year < 1 || year > 9378 || month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
	}
	return DaysUpToPersianYear(year) + (Int64)DaysToMonth[month - 1] + (Int64)day - 1LL;
}

int PersianCalendar::GetDatePart(Int64 ticks, int part) const
{
	CheckTicksRange(ticks);
	Int64 num = ticks / TimeSpan::DAYS + 1LL;
	int num1 = (int)((num - 226894LL) * 33LL / 12053LL) + 1LL;
	Int64 persianYear = DaysUpToPersianYear(num1);
	Int64 daysInYear = (Int64)GetDaysInYear(num1, 0);
	if (num < persianYear)
	{
		persianYear = persianYear - daysInYear;
		num1--;
	}
	else if (num == persianYear)
	{
		num1--;
		persianYear = persianYear - (Int64)GetDaysInYear(num1, 0);
	}
	else if (num > persianYear + daysInYear)
	{
		persianYear = persianYear + daysInYear;
		num1++;
	}
	if (part == 0)
	{
		return num1;
	}
	num = num - persianYear;
	if (part == 1)
	{
		return (int)num;
	}
	int num2 = 0;
	while (num2 < 12 && num >(Int64)DaysToMonth[num2])
	{
		num2++;
	}
	if (part == 2)
	{
		return num2;
	}
	int daysToMonth = (int)(num - (Int64)DaysToMonth[num2 - 1]);
	if (part != 3)
	{
		throw std::exception("InvalidOperation_DateTimeParsing");
	}
	return daysToMonth;
}

int PersianCalendar::GetDayOfMonth(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 3);
}

DayOfWeek PersianCalendar::GetDayOfWeek(const DateTime& time) const 
{
	return (DayOfWeek)((int)(time.Ticks / TimeSpan::DAYS + 1) % 7);
}

int PersianCalendar::GetDayOfYear(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 1);
}

int PersianCalendar::GetDaysInMonth(int year, int month, int era) const 
{
	CheckYearMonthRange(year, month, era);
	if (month == 10 && year == 9378)
	{
		return 10;
	}
	if (month != 12)
	{
		if (month <= 6)
		{
			return 31;
		}
		return 30;
	}
	if (!IsLeapYear(year, 0))
	{
		return 29;
	}
	return 30;
}

int PersianCalendar::GetDaysInYear(int year, int era) const 
{
	CheckYearRange(year, era);
	if (year == 9378)
	{
		return DaysToMonth[9] + 10;
	}
	if (!IsLeapYear(year, 0))
	{
		return 365;
	}
	return 366;
}

int PersianCalendar::GetEra(const DateTime& time) const 
{
	CheckTicksRange(time.Ticks);
	return 1;
}

int PersianCalendar::GetLeapMonth(int year, int era) const 
{
	CheckYearRange(year, era);
	return 0;
}

int PersianCalendar::GetMonth(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 2);
}

int PersianCalendar::GetMonthsInYear(int year, int era) const 
{
	CheckYearRange(year, era);
	if (year == 9378)
	{
		return 10;
	}
	return 12;
}

int PersianCalendar::GetYear(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 0);
}

bool PersianCalendar::IsLeapDay(int year, int month, int day, int era) const 
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

bool PersianCalendar::IsLeapMonth(int year, int month, int era) const 
{
	CheckYearMonthRange(year, month, era);
	return false;
}

bool PersianCalendar::IsLeapYear(int year, int era) const 
{
	CheckYearRange(year, era);
	return LeapYears33[year % 33] == 1;
}

DateTime PersianCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const 
{
	int daysInMonth = GetDaysInMonth(year, month, era);
	if (day < 1 || day > daysInMonth)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	Int64 absoluteDatePersian = GetAbsoluteDatePersian(year, month, day);
	if (absoluteDatePersian < (Int64)0)
	{
		throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
	}
	return DateTime(absoluteDatePersian * TimeSpan::DAYS + Calendar::TimeToTicks(hour, minute, second, millisecond));
}

int PersianCalendar::ToFourDigitYear(int year) const 
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	if (year < 100)
	{
		return Calendar::ToFourDigitYear(year);
	}
	if (year > 9378)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}

__uregister_class(PersianCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}