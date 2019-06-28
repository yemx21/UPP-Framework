#include "JapaneseCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

static const std::vector<EraInfo> japaneseEraInfo = { EraInfo(4, 1989, 1, 8, 1988, 1, 8011, L"平成", L"平", L"H"), EraInfo(3, 1926, 12, 25, 1925, 1, 64, L"昭和", L"昭", L"S"), EraInfo(2, 1912, 7, 30, 1911, 1, 15, L"大正", L"大", L"T"), EraInfo(1, 1868, 1, 1, 1867, 1, 45, L"明治", L"明", L"M") };
static const DateTime calendarMinValue = DateTime(1868, 9, 8);


Calendar* JapaneseCalendar::InstanceClone() const
{
	JapaneseCalendar* cal=new JapaneseCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType JapaneseCalendar::GetAlgorithmType() const
{
	return CalendarAlgorithmType::SolarCalendar;
}

std::vector<int> JapaneseCalendar::GetEras() const
{
	return helper->GetEras();
}

int JapaneseCalendar::GetID() const
{
	return 3;
}

DateTime JapaneseCalendar::GetMaxSupportedDateTime() const
{
	return DateTime::MaxValue;
}

DateTime JapaneseCalendar::GetMinSupportedDateTime() const
{
	return calendarMinValue;
}

int JapaneseCalendar::GetTwoDigitYearMax() const
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetID(), 99);
	}
	return twoDigitYearMax;
}

void JapaneseCalendar::SetTwoDigitYearMax(int value)
{
	VerifyWritable();
	if (value < 99 || value > helper->GetMaxYear())
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	twoDigitYearMax = value;
}

JapaneseCalendar::JapaneseCalendar()
{
	helper = new GregorianCalendarHelper(this, JapaneseCalendar::GetEraInfo());
}

JapaneseCalendar::~JapaneseCalendar()
{
	SAFE_DELETE(helper);
}

wstringvector JapaneseCalendar::AbbrevEraNames()
{
	std::vector<EraInfo> eraInfo = JapaneseCalendar::GetEraInfo();
	wstringvector strArrays = wstringvector((int)eraInfo.size());
	for (int i = 0; i < (int)eraInfo.size(); i++)
	{
		strArrays[i] = eraInfo[(int)eraInfo.size() - i - 1].abbrevEraName;
	}
	return strArrays;
}

DateTime JapaneseCalendar::AddMonths(const DateTime& time, int months)
{
	return helper->AddMonths(time, months);
}

DateTime JapaneseCalendar::AddYears(const DateTime& time, int years)
{
	return helper->AddYears(time, years);
}

wstringvector JapaneseCalendar::EnglishEraNames()
{
	std::vector<EraInfo> eraInfo = JapaneseCalendar::GetEraInfo();
	wstringvector strArrays = wstringvector((int)eraInfo.size());
	for (int i = 0; i < (int)eraInfo.size(); i++)
	{
		strArrays[i] = eraInfo[(int)eraInfo.size() - i - 1].englishEraName;
	}
	return strArrays;
}

wstringvector JapaneseCalendar::EraNames()
{
	std::vector<EraInfo> eraInfo = JapaneseCalendar::GetEraInfo();
	wstringvector strArrays = wstringvector((int)eraInfo.size());
	for (int i = 0; i < (int)eraInfo.size(); i++)
	{
		strArrays[i] = eraInfo[(int)eraInfo.size() - i - 1].eraName;
	}
	return strArrays;
}

int JapaneseCalendar::GetDayOfMonth(const DateTime& time) const
{
	return helper->GetDayOfMonth(time);
}

DayOfWeek JapaneseCalendar::GetDayOfWeek(const DateTime& time) const
{
	return helper->GetDayOfWeek(time);
}

int JapaneseCalendar::GetDayOfYear(const DateTime& time)  const
{
	return helper->GetDayOfYear(time);
}

int JapaneseCalendar::GetDaysInMonth(int year, int month, int era)  const
{
	return helper->GetDaysInMonth(year, month, era);
}

int JapaneseCalendar::GetDaysInYear(int year, int era) const
{
	return helper->GetDaysInYear(year, era);
}


int JapaneseCalendar::GetEra(const DateTime& time)  const
{
	return helper->GetEra(time);
}

std::vector<EraInfo> JapaneseCalendar::GetEraInfo()
{
	return japaneseEraInfo;
}

int JapaneseCalendar::GetLeapMonth(int year, int era) const
{
	return helper->GetLeapMonth(year, era);
}

int JapaneseCalendar::GetMonth(const DateTime& time) const
{
	return helper->GetMonth(time);
}

int JapaneseCalendar::GetMonthsInYear(int year, int era) const
{
	return helper->GetMonthsInYear(year, era);
}

int JapaneseCalendar::GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek) const
{
	return helper->GetWeekOfYear(time, rule, firstDayOfWeek);
}

int JapaneseCalendar::GetYear(const DateTime& time) const
{
	return helper->GetYear(time);
}

bool JapaneseCalendar::IsLeapDay(int year, int month, int day, int era) const
{
	return helper->IsLeapDay(year, month, day, era);
}

bool JapaneseCalendar::IsLeapMonth(int year, int month, int era) const
{
	return helper->IsLeapMonth(year, month, era);
}

bool JapaneseCalendar::IsLeapYear(int year, int era) const
{
	return helper->IsLeapYear(year, era);
}

bool JapaneseCalendar::IsValidYear(int year, int era)  const
{
	return helper->IsValidYear(year, era);
}

DateTime JapaneseCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const
{
	return helper->ToDateTime(year, month, day, hour, minute, second, millisecond, era);
}

int JapaneseCalendar::ToFourDigitYear(int year) const
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

__uregister_class(JapaneseCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}