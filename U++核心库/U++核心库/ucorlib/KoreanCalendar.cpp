#include "KoreanCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

static std::vector<EraInfo> koreanEraInfo = { EraInfo(1, 1, 1, 1, -2333, 2334, 12332) };

Calendar* KoreanCalendar::InstanceClone() const
{
	KoreanCalendar* cal = new KoreanCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType KoreanCalendar::GetAlgorithmType() const
{
	return CalendarAlgorithmType::SolarCalendar;
}

std::vector<int> KoreanCalendar::GetEras() const
{
	return helper->GetEras();
}

int KoreanCalendar::GetID() const
{
	return 5;
}

DateTime KoreanCalendar::GetMaxSupportedDateTime() const
{
	return DateTime::MaxValue;
}

DateTime KoreanCalendar::GetMinSupportedDateTime() const
{
	return DateTime::MinValue;
}

int KoreanCalendar::GetTwoDigitYearMax() const
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = GetSystemTwoDigitYearSetting(GetID(), 4362);
	}
	return twoDigitYearMax;
}

void KoreanCalendar::SetTwoDigitYearMax(int value)
{
	VerifyWritable();
	if (value < 99 || value > helper->GetMaxYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	twoDigitYearMax = value;
}

KoreanCalendar::KoreanCalendar()
{
	helper = new GregorianCalendarHelper(this, koreanEraInfo);
}

KoreanCalendar::~KoreanCalendar()
{
	SAFE_DELETE(helper);
}

DateTime KoreanCalendar::AddMonths(const DateTime& time, int months)
{
	return helper->AddMonths(time, months);
}

DateTime KoreanCalendar::AddYears(const DateTime& time, int years)
{
	return helper->AddYears(time, years);
}

int KoreanCalendar::GetDayOfMonth(const DateTime& time) const
{
	return helper->GetDayOfMonth(time);
}

DayOfWeek KoreanCalendar::GetDayOfWeek(const DateTime& time)const
{
	return helper->GetDayOfWeek(time);
}

int KoreanCalendar::GetDayOfYear(const DateTime& time)const
{
	return helper->GetDayOfYear(time);
}

int KoreanCalendar::GetDaysInMonth(int year, int month, int era)const
{
	return helper->GetDaysInMonth(year, month, era);
}

int KoreanCalendar::GetDaysInYear(int year, int era)const
{
	return helper->GetDaysInYear(year, era);
}

int KoreanCalendar::GetEra(const DateTime& time) const
{
	return helper->GetEra(time);
}

int KoreanCalendar::GetLeapMonth(int year, int era) const
{
	return helper->GetLeapMonth(year, era);
}

int KoreanCalendar::GetMonth(const DateTime& time) const
{
	return helper->GetMonth(time);
}

int KoreanCalendar::GetMonthsInYear(int year, int era)const
{
	return helper->GetMonthsInYear(year, era);
}

int KoreanCalendar::GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek) const
{
	return helper->GetWeekOfYear(time, rule, firstDayOfWeek);
}

int KoreanCalendar::GetYear(const DateTime& time) const
{
	return helper->GetYear(time);
}

bool KoreanCalendar::IsLeapDay(int year, int month, int day, int era) const
{
	return helper->IsLeapDay(year, month, day, era);
}

bool KoreanCalendar::IsLeapMonth(int year, int month, int era) const
{
	return helper->IsLeapMonth(year, month, era);
}

bool KoreanCalendar::IsLeapYear(int year, int era) const
{
	return helper->IsLeapYear(year, era);
}

DateTime KoreanCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const
{
	return helper->ToDateTime(year, month, day, hour, minute, second, millisecond, era);
}

int KoreanCalendar::ToFourDigitYear(int year) const
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	return helper->ToFourDigitYear(year, GetTwoDigitYearMax());
}

__uregister_class(KoreanCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}