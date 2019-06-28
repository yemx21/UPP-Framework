#include "ThaiBuddhistCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

static const std::vector<EraInfo> thaiBuddhistEraInfo = { EraInfo(1, 1, 1, 1, -543, 544, 10542) };

Calendar* ThaiBuddhistCalendar::InstanceClone() const
{
	ThaiBuddhistCalendar* cal = new ThaiBuddhistCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType ThaiBuddhistCalendar::GetAlgorithmType() const 
{
	return CalendarAlgorithmType::SolarCalendar;
}

std::vector<int> ThaiBuddhistCalendar::GetEras() const 
{
	return helper->GetEras();
}

int ThaiBuddhistCalendar::GetID() const 
{
	return 7;
}

DateTime ThaiBuddhistCalendar::GetMaxSupportedDateTime() const 
{
	return DateTime::MaxValue;
}

DateTime ThaiBuddhistCalendar::GetMinSupportedDateTime() const 
{
	return DateTime::MinValue;
}

int ThaiBuddhistCalendar::GetTwoDigitYearMax() const 
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetID(), 2572);
	}
	return twoDigitYearMax;
}

void ThaiBuddhistCalendar::SetTwoDigitYearMax(int value) 
{
	VerifyWritable();
	if (value < 99 || value > helper->GetMaxYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	twoDigitYearMax = value;
}


ThaiBuddhistCalendar::ThaiBuddhistCalendar()
{
	helper = new GregorianCalendarHelper(this, thaiBuddhistEraInfo);
}

ThaiBuddhistCalendar::~ThaiBuddhistCalendar()
{
	SAFE_DELETE(helper);
}

DateTime ThaiBuddhistCalendar::AddMonths(const DateTime& time, int months) 
{
	return helper->AddMonths(time, months);
}

DateTime ThaiBuddhistCalendar::AddYears(const DateTime& time, int years) 
{
	return helper->AddYears(time, years);
}

int ThaiBuddhistCalendar::GetDayOfMonth(const DateTime& time) const 
{
	return helper->GetDayOfMonth(time);
}

DayOfWeek ThaiBuddhistCalendar::GetDayOfWeek(const DateTime& time) const 
{
	return helper->GetDayOfWeek(time);
}

int ThaiBuddhistCalendar::GetDayOfYear(const DateTime& time) const 
{
	return helper->GetDayOfYear(time);
}

int ThaiBuddhistCalendar::GetDaysInMonth(int year, int month, int era) const 
{
	return helper->GetDaysInMonth(year, month, era);
}

int ThaiBuddhistCalendar::GetDaysInYear(int year, int era) const 
{
	return helper->GetDaysInYear(year, era);
}

int ThaiBuddhistCalendar::GetEra(const DateTime& time) const 
{
	return helper->GetEra(time);
}

int ThaiBuddhistCalendar::GetLeapMonth(int year, int era) const 
{
	return helper->GetLeapMonth(year, era);
}

int ThaiBuddhistCalendar::GetMonth(const DateTime& time) const 
{
	return helper->GetMonth(time);
}

int ThaiBuddhistCalendar::GetMonthsInYear(int year, int era) const 
{
	return helper->GetMonthsInYear(year, era);
}

int ThaiBuddhistCalendar::GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek) const 
{
	return helper->GetWeekOfYear(time, rule, firstDayOfWeek);
}

int ThaiBuddhistCalendar::GetYear(const DateTime& time) const 
{
	return helper->GetYear(time);
}

bool ThaiBuddhistCalendar::IsLeapDay(int year, int month, int day, int era) const 
{
	return helper->IsLeapDay(year, month, day, era);
}

bool ThaiBuddhistCalendar::IsLeapMonth(int year, int month, int era) const 
{
	return helper->IsLeapMonth(year, month, era);
}

bool ThaiBuddhistCalendar::IsLeapYear(int year, int era) const 
{
	return helper->IsLeapYear(year, era);
}

DateTime ThaiBuddhistCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const 
{
	return helper->ToDateTime(year, month, day, hour, minute, second, millisecond, era);
}

int ThaiBuddhistCalendar::ToFourDigitYear(int year) const 
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	return helper->ToFourDigitYear(year, GetTwoDigitYearMax());
}

__uregister_class(ThaiBuddhistCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}