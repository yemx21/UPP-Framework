#include "TaiwanCalendar.h"
#include "BuiltInAssembly.h"



static const std::vector<EraInfo> taiwanEraInfo = { EraInfo(1, 1912, 1, 1, 1911, 1, 8088) };
static const DateTime calendarMinValue = DateTime(1912, 1, 1);

Calendar* TaiwanCalendar::InstanceClone() const
{
	TaiwanCalendar* cal = new TaiwanCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType TaiwanCalendar::GetAlgorithmType() const
{
	return CalendarAlgorithmType::SolarCalendar;
}

std::vector<int> TaiwanCalendar::GetEras() const
{
	return helper->GetEras();
}

int TaiwanCalendar::GetID() const
{
	return 4;
}

DateTime TaiwanCalendar::GetMaxSupportedDateTime() const
{
	return DateTime::MaxValue;
}

DateTime TaiwanCalendar::GetMinSupportedDateTime() const
{
	return calendarMinValue;
}

int TaiwanCalendar::GetTwoDigitYearMax() const
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetID(), 99);
	}
	return twoDigitYearMax;
}

void TaiwanCalendar::SetTwoDigitYearMax(int value)
{
	VerifyWritable();
	if (value < 99 || value > helper->GetMaxYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	twoDigitYearMax = value;
}

TaiwanCalendar::TaiwanCalendar()
{
	helper = new GregorianCalendarHelper(this, taiwanEraInfo);
}

TaiwanCalendar::~TaiwanCalendar()
{
	SAFE_DELETE(helper);
}

DateTime TaiwanCalendar::AddMonths(const DateTime& time, int months)
{
	return helper->AddMonths(time, months);
}

DateTime TaiwanCalendar::AddYears(const DateTime& time, int years)
{
	return helper->AddYears(time, years);
}

int TaiwanCalendar::GetDayOfMonth(const DateTime& time) const
{
	return helper->GetDayOfMonth(time);
}

DayOfWeek TaiwanCalendar::GetDayOfWeek(const DateTime& time) const
{
	return helper->GetDayOfWeek(time);
}

int TaiwanCalendar::GetDayOfYear(const DateTime& time) const
{
	return helper->GetDayOfYear(time);
}

int TaiwanCalendar::GetDaysInMonth(int year, int month, int era)  const
{
	return helper->GetDaysInMonth(year, month, era);
}

int TaiwanCalendar::GetDaysInYear(int year, int era) const
{
	return helper->GetDaysInYear(year, era);
}

int TaiwanCalendar::GetEra(const DateTime& time) const
{
	return helper->GetEra(time);
}

int TaiwanCalendar::GetLeapMonth(int year, int era) const
{
	return helper->GetLeapMonth(year, era);
}

int TaiwanCalendar::GetMonth(const DateTime& time) const
{
	return helper->GetMonth(time);
}

int TaiwanCalendar::GetMonthsInYear(int year, int era) const
{
	return helper->GetMonthsInYear(year, era);
}

int TaiwanCalendar::GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek) const
{
	return helper->GetWeekOfYear(time, rule, firstDayOfWeek);
}

int TaiwanCalendar::GetYear(const DateTime& time) const
{
	return helper->GetYear(time);
}

bool TaiwanCalendar::IsLeapDay(int year, int month, int day, int era) const
{
	return helper->IsLeapDay(year, month, day, era);
}

bool TaiwanCalendar::IsLeapMonth(int year, int month, int era) const
{
	return helper->IsLeapMonth(year, month, era);
}

bool TaiwanCalendar::IsLeapYear(int year, int era) const
{
	return helper->IsLeapYear(year, era);
}

DateTime TaiwanCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const
{
	return helper->ToDateTime(year, month, day, hour, minute, second, millisecond, era);
}

int TaiwanCalendar::ToFourDigitYear(int year) const
{
	if (year <= 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedPosNum_Year");
	}
	if (year > helper->GetMaxYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}

__uregister_class(TaiwanCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}