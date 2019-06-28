#include "GregorianCalendar.h"
#include "BuiltInAssembly.h"

using namespace System;

static const int DaysToMonth365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

static const int DaysToMonth366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

Calendar* GregorianCalendar::InstanceClone() const
{
	GregorianCalendar* cal=new GregorianCalendar(m_type);
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType GregorianCalendar::GetAlgorithmType() const
{
	return CalendarAlgorithmType::SolarCalendar;
}

GregorianCalendarTypes GregorianCalendar::GetCalendarType() const
{
	return m_type;
}

void GregorianCalendar::SetCalendarType(GregorianCalendarTypes value)
{
	Calendar::VerifyWritable();
	GregorianCalendarTypes gregorianCalendarType = value;
	switch (gregorianCalendarType)
	{
	case GregorianCalendarTypes::Localized:
	case GregorianCalendarTypes::USEnglish:
	{
		m_type = value;
		return;
	}
	default:
	{
		switch (gregorianCalendarType)
		{
		case GregorianCalendarTypes::MiddleEastFrench:
		case GregorianCalendarTypes::Arabic:
		case GregorianCalendarTypes::TransliteratedEnglish:
		case GregorianCalendarTypes::TransliteratedFrench:
		{
			m_type = value;
			return;
		}
		}
		throw std::exception("ArgumentOutOfRange_Enum");
	}
	}
}

std::vector<int> GregorianCalendar::GetEras() const
{
	return std::vector<int>{ 1 };
}

int GregorianCalendar::GetID() const
{
	return (int)m_type;
}

DateTime GregorianCalendar::GetMaxSupportedDateTime() const
{
	return DateTime::MaxValue;
}

DateTime GregorianCalendar::GetMinSupportedDateTime() const
{
	return DateTime::MinValue;
}

int GregorianCalendar::GetTwoDigitYearMax() const
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetID(), 2029);
	}
	return twoDigitYearMax;
}

void GregorianCalendar::SetTwoDigitYearMax(int value)
{
	Calendar::VerifyWritable();
	if (value < 99 || value > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Range");
	}
	twoDigitYearMax = value;
}


GregorianCalendar::GregorianCalendar(GregorianCalendarTypes type)
{
	if (type < GregorianCalendarTypes::Localized || type > GregorianCalendarTypes::TransliteratedFrench)
	{
		throw std::exception("ArgumentOutOfRange_Range");
	}
	m_type = type;
}

GregorianCalendar::GregorianCalendar() : GregorianCalendar(GregorianCalendarTypes::Localized)
{
}

GregorianCalendar::~GregorianCalendar() {}

DateTime GregorianCalendar::AddMonths(const DateTime& time, int months)
{
	if (months < -120000 || months > 120000)
	{
		throw std::exception("ArgumentOutOfRange_Range");
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
	const int* numArray = (datePart % 4 != 0 || datePart % 100 == 0 && datePart % 400 != 0 ? DaysToMonth365 : DaysToMonth366);
	int num2 = numArray[num] - numArray[num - 1];
	if (datePart1 > num2)
	{
		datePart1 = num2;
	}
	Int64 ticks = DateToTicks(datePart, num, datePart1) + time.Ticks % TimeSpan::DAYS;
	Calendar::CheckAddResult(ticks, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return DateTime(ticks);
}

DateTime GregorianCalendar::AddYears(const DateTime& time, int years) 
{
	return AddMonths(time, years * 12);
}


Int64 GregorianCalendar::DateToTicks(int year, int month, int day)  const
{
	return GetAbsoluteDate(year, month, day) * TimeSpan::DAYS;
}

Int64 GregorianCalendar::GetAbsoluteDate(int year, int month, int day)
{
	if (year >= 1 && year <= 9999 && month >= 1 && month <= 12)
	{
		const int* numArray = (year % 4 != 0 || year % 100 == 0 && year % 400 != 0 ? DaysToMonth365 : DaysToMonth366);
		if (day >= 1 && day <= numArray[month] - numArray[month - 1])
		{
			int num = year - 1;
			int num1 = num * 365 + num / 4 - num / 100 + num / 400 + numArray[month - 1] + day - 1;
			return (long)num1;
		}
	}
	throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
}


int GregorianCalendar::GetDatePart(Int64 ticks, int part)  const
{
	bool flag;
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
	const int* numArray = (flag ? DaysToMonth366 : DaysToMonth365);
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

int GregorianCalendar::GetDayOfMonth(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 3);
}

DayOfWeek GregorianCalendar::GetDayOfWeek(const DateTime& time) const
{
	return (DayOfWeek)((int)(time.Ticks / TimeSpan::DAYS + 1) % 7);
}

int GregorianCalendar::GetDayOfYear(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 1);
}

int GregorianCalendar::GetDaysInMonth(int year, int month, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Range");
	}
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	const int* numArray = (year % 4 != 0 || year % 100 == 0 && year % 400 != 0 ? DaysToMonth365 : DaysToMonth366);
	return numArray[month] - numArray[month - 1];
}

int GregorianCalendar::GetDaysInYear(int year, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
	{
		return 366;
	}
	return 365;
}

int GregorianCalendar::GetEra(const DateTime& time) const
{
	return 1;
}

int GregorianCalendar::GetLeapMonth(int year, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return 0;
}

int GregorianCalendar::GetMonth(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 2);
}

int GregorianCalendar::GetMonthsInYear(int year, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return 12;
}

int GregorianCalendar::GetYear(const DateTime& time) const
{
	return GetDatePart(time.Ticks, 0);
}

bool GregorianCalendar::IsLeapDay(int year, int month, int day, int era) const
{
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	if (day < 1 || day > Calendar::GetDaysInMonth(year, month))
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	if (!Calendar::IsLeapYear(year))
	{
		return false;
	}
	if (month == 2 && day == 29)
	{
		return true;
	}
	return false;
}

bool GregorianCalendar::IsLeapMonth(int year, int month, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	return false;
}

bool GregorianCalendar::IsLeapYear(int year, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	if (year < 1 || year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
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

DateTime GregorianCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
	return DateTime(year, month, day, hour, minute, second, millisecond);
}

int GregorianCalendar::ToFourDigitYear(int year) const
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	if (year > 9999)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return Calendar::ToFourDigitYear(year);
}

bool GregorianCalendar::TryToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era, DateTime& result) const
{
	if (era != 0 && era != 1)
	{
		result = DateTime::MinValue;
		return false;
	}
	return DateTime::TryCreate(year, month, day, hour, minute, second, millisecond, 0, DateTimeKind::Local, result);
}

__uregister_class(GregorianCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}