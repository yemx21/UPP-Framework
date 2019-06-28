#include "Calendar.h"
#include "CalendarData.h"
#include "BuiltInAssembly.h"
using namespace System;


EraInfo::EraInfo(int era, int startYear, int startMonth, int startDay, int yearOffset, int minEraYear, int maxEraYear)
{
	this->era = era;
	this->yearOffset = yearOffset;
	this->minEraYear = minEraYear;
	this->maxEraYear = maxEraYear;
	ticks = (DateTime(startYear, startMonth, startDay)).Ticks;
}


EraInfo::EraInfo(int era, int startYear, int startMonth, int startDay, int yearOffset, int minEraYear, int maxEraYear, const std::wstring& eraName, const std::wstring& abbrevEraName, const std::wstring& englishEraName)
{
	this->era = era;
	this->yearOffset = yearOffset;
	this->minEraYear = minEraYear;
	this->maxEraYear = maxEraYear;
	ticks = (DateTime(startYear, startMonth, startDay)).Ticks;
	this->eraName = eraName;
	this->abbrevEraName = abbrevEraName;
	this->englishEraName = englishEraName;
}

EraInfo::EraInfo(const EraInfo& info)
{
	era = info.era;
	yearOffset = info.yearOffset;
	minEraYear = info.minEraYear;
	maxEraYear = info.maxEraYear;
	ticks = info.ticks;
	eraName = info.eraName;
	abbrevEraName = info.abbrevEraName;
	englishEraName = info.englishEraName;
}


CalendarAlgorithmType Calendar::GetAlgorithmType() const
{
	return CalendarAlgorithmType::Unknown;
}

int Calendar::GetBaseCalendarID() const
{
	return GetID();
}

int Calendar::GetCurrentEraValue() const
{
	if (m_currentEraValue == -1)
	{
		CalendarData* cd = CalendarData::GetCalendarData(GetBaseCalendarID());
		m_currentEraValue = cd != nullptr ? cd->iCurrentEra : 1; 
	}
	return m_currentEraValue;
}

Calendar::Calendar()
{
	m_currentEraValue = -1;
	m_isReadOnly = false;
	twoDigitYearMax = -1;
}

Calendar::~Calendar()
{

}

int Calendar::GetDaysInYearBeforeMinSupportedYear() const
{
	return 365;
}

int Calendar::GetID() const
{
	return -1;
}

bool Calendar::GetIsReadOnly() const
{
	return m_isReadOnly;
}

DateTime Calendar::GetMaxSupportedDateTime() const
{
	return DateTime::MaxValue;
}

DateTime Calendar::GetMinSupportedDateTime() const
{
	return DateTime::MinValue;
}

int Calendar::GetTwoDigitYearMax() const
{
	return twoDigitYearMax;
}

void Calendar::SetTwoDigitYearMax(int value)
{
	VerifyWritable();
	twoDigitYearMax = value;
}

DateTime Calendar::Add(const DateTime& time, double value, int scale)
{
	double num = value * (double)scale + (value >= 0 ? 0.5 : -0.5);
	if (num <= -315537897600000 || num >= 315537897600000)
	{
		throw std::exception("ArgumentOutOfRange_AddValue");
	}
	Int64 num1 = (Int64)num;
	Int64 ticks = time.Ticks + num1 * 10000LL;
	CheckAddResult(ticks, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return DateTime(ticks);
}

DateTime Calendar::AddDays(const DateTime& time, int days)
{
	return Add(time, (double)days, 86400000);
}

DateTime Calendar::AddHours(const DateTime& time, int hours)
{
	return Add(time, (double)hours, 3600000);
}

DateTime Calendar::AddMilliseconds(const DateTime& time, double milliseconds)
{
	return Add(time, milliseconds, 1);
}

DateTime Calendar::AddMinutes(const DateTime& time, int minutes)
{
	return Add(time, (double)minutes, 60000);
}

DateTime Calendar::AddSeconds(const DateTime& time, int seconds)
{
	return Add(time, (double)seconds, 1000);
}

DateTime Calendar::AddWeeks(const DateTime& time, int weeks)
{
	return AddDays(time, weeks * 7);
}

void Calendar::CheckAddResult(Int64 ticks, const DateTime& minValue, const DateTime& maxValue)
{
	if (ticks < minValue.Ticks || ticks > maxValue.Ticks)
	{
		throw std::exception("Argument_ResultCalendarRange");
	}
}

int Calendar::GetDaysInMonth(int year, int month) const
{
	return GetDaysInMonth(year, month, 0);
}

int Calendar::GetDaysInYear(int year) const
{
	return GetDaysInYear(year, 0);
}

int Calendar::GetFirstDayWeekOfYear(const DateTime& time, int firstDayOfWeek) const
{
	int dayOfYear = GetDayOfYear(time) - 1;
	int dayOfWeek = (int)GetDayOfWeek(time) - dayOfYear % 7;
	int num = (dayOfWeek - firstDayOfWeek + 14) % 7;
	return (dayOfYear + num) / 7 + 1;
}

int Calendar::GetHour(const DateTime& time) const
{
	return (int)(time.Ticks / TimeSpan::HOURS % 24LL);
}

int Calendar::GetLeapMonth(int year) const
{
	return GetLeapMonth(year, 0);
}

int Calendar::GetLeapMonth(int year, int era) const
{
	if (!IsLeapYear(year, era))
	{
		return 0;
	}
	int monthsInYear = GetMonthsInYear(year, era);
	for (int i = 1; i <= monthsInYear; i++)
	{
		if (IsLeapMonth(year, i, era))
		{
			return i;
		}
	}
	return 0;
}

double Calendar::GetMilliseconds(const DateTime& time) const
{
	return (double)(time.Ticks / 10000LL % 1000);
}

int Calendar::GetMinute(const DateTime& time) const
{
	return (int)(time.Ticks / 600000000LL % 60);
}

int Calendar::GetMonthsInYear(int year) const
{
	return GetMonthsInYear(year, 0);
}

int Calendar::GetSecond(const DateTime& time) const
{
	return (int)(time.Ticks / 10000000 % (Int64)60);
}

int Calendar::GetSystemTwoDigitYearSetting(int CalID, int defaultYearValue)
{
	int num = CalendarData::GetTwoDigitYearMax(CalID);
	if (num < 0)
	{
		num = defaultYearValue;
	}
	return num;
}

int Calendar::GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek) const
{
	if (firstDayOfWeek < DayOfWeek::Sunday || firstDayOfWeek > DayOfWeek::Saturday)
	{
		throw std::exception("firstDayOfWeekOutOfRange_Range");
	}
	if( rule== CalendarWeekRule::FirstDay)
	{
		return GetFirstDayWeekOfYear(time, (int)firstDayOfWeek);
	}
	else if (rule == CalendarWeekRule::FirstFullWeek)
	{
		return GetWeekOfYearFullDays(time, (int)firstDayOfWeek, 7);
	}
	else if (rule == CalendarWeekRule::FirstFourDayWeek)
	{
		return GetWeekOfYearFullDays(time, (int)firstDayOfWeek, 4);
	}
	throw std::exception("ruleOutOfRange_Range");
}

int Calendar::GetWeekOfYearFullDays(const DateTime& time, int firstDayOfWeek, int fullDays) const
{
	int dayOfYear = GetDayOfYear(time) - 1;
	int dayOfWeek = (int)GetDayOfWeek(time) - dayOfYear % 7;
	int num = (firstDayOfWeek - dayOfWeek + 14) % 7;
	if (num != 0 && num >= fullDays)
	{
		num = num - 7;
	}
	int num1 = dayOfYear - num;
	if (num1 >= 0)
	{
		return num1 / 7 + 1;
	}
	if (time <= GetMinSupportedDateTime().AddDays((double)dayOfYear))
	{
		return GetWeekOfYearOfMinSupportedDateTime(firstDayOfWeek, fullDays);
	}
	return GetWeekOfYearFullDays(time.AddDays((double)(-(dayOfYear + 1))), firstDayOfWeek, fullDays);
}

int Calendar::GetWeekOfYearOfMinSupportedDateTime(int firstDayOfWeek, int minimumDaysInFirstWeek) const
{
	int dayOfYear = GetDayOfYear(GetMinSupportedDateTime()) - 1;
	int dayOfWeek = (int)GetDayOfWeek(GetMinSupportedDateTime()) - dayOfYear % 7;
	int num = (firstDayOfWeek + 7 - dayOfWeek) % 7;
	if (num == 0 || num >= minimumDaysInFirstWeek)
	{
		return 1;
	}
	int daysInYearBeforeMinSupportedYear = GetDaysInYearBeforeMinSupportedYear() - 1;
	int num1 = dayOfWeek - 1 - daysInYearBeforeMinSupportedYear % 7;
	int num2 = (firstDayOfWeek - num1 + 14) % 7;
	int num3 = daysInYearBeforeMinSupportedYear - num2;
	if (num2 >= minimumDaysInFirstWeek)
	{
		num3 = num3 + 7;
	}
	return num3 / 7 + 1;
}


bool Calendar::IsLeapDay(int year, int month, int day) const
{
	return IsLeapDay(year, month, day, 0);
}


bool Calendar::IsLeapMonth(int year, int month) const
{
	return IsLeapMonth(year, month, 0);
}

bool Calendar::IsLeapYear(int year) const
{
	return IsLeapYear(year, 0);
}

bool Calendar::IsValidDay(int year, int month, int day, int era) const
{
	if (!IsValidMonth(year, month, era) || day < 1)
	{
		return false;
	}
	return day <= GetDaysInMonth(year, month, era);
}

bool Calendar::IsValidMonth(int year, int month, int era) const
{
	if (!IsValidYear(year, era) || month < 1)
	{
		return false;
	}
	return month <= GetMonthsInYear(year, era);
}

bool Calendar::IsValidYear(int year, int era) const
{
	if (year < GetYear(GetMinSupportedDateTime()))
	{
		return false;
	}
	return year <= GetYear(GetMaxSupportedDateTime());
}

Calendar* Calendar::GetReadOnly(const Calendar* calendar)
{
	if (calendar == nullptr)
	{
		return nullptr;
	}
	Calendar* calendar1 = calendar->Clone();
	calendar1->SetReadOnlyState(true);
	return calendar1;
}

void Calendar::SetReadOnlyState(bool readOnly)
{
	m_isReadOnly = readOnly;
}

Int64  Calendar::TimeToTicks(int hour, int minute, int second, int millisecond)
{
	if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60 || second < 0 || second >= 60)
	{
		throw std::exception("ArgumentOutOfRange_BadHourMinuteSecond");
	}
	if (millisecond < 0 || millisecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_BadMilliSecond");
	}
	return TimeSpan::TimeToTicks(0, hour, minute, second, millisecond, 0,0);
}

DateTime Calendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond) const
{
	return ToDateTime(year, month, day, hour, minute, second, millisecond, 0);
}

int Calendar::ToFourDigitYear(int year) const
{
	if (year < 0)
	{
		throw std::exception("year can not be negative");
	}
	if (year >= 100)
	{
		return year;
	}
	return (GetTwoDigitYearMax() / 100 - (year > GetTwoDigitYearMax() % 100 ? 1 : 0)) * 100 + year;
}

bool Calendar::TryToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era, DateTime& result) const
{
	bool flag;
	result = DateTime::MinValue;
	try
	{
		result = ToDateTime(year, month, day, hour, minute, second, millisecond, era);
		flag = true;
	}
	catch (...)
	{
		flag = false;
	}
	return flag;
}

void Calendar::VerifyWritable()
{
	if (m_isReadOnly)
	{
		throw std::exception("InvalidOperation_ReadOnly");
	}
}

Calendar* Calendar::Clone() const
{
	Calendar* res = InstanceClone();
	if (res) res->SetReadOnlyState(false);
	return res;
}


__uregister_enum(CalendarAlgorithmType)
{
	type.AddValue(L"Unknown", CalendarAlgorithmType::Unknown, __uhashstr(System::CalendarAlgorithmType::Unknown));
	type.AddValue(L"SolarCalendar", CalendarAlgorithmType::SolarCalendar, __uhashstr(System::CalendarAlgorithmType::SolarCalendar));
	type.AddValue(L"LunarCalendar", CalendarAlgorithmType::LunarCalendar, __uhashstr(System::CalendarAlgorithmType::LunarCalendar));
	type.AddValue(L"LunisolarCalendar", CalendarAlgorithmType::LunisolarCalendar, __uhashstr(System::CalendarAlgorithmType::LunisolarCalendar));

	BuiltInAssemblyFactory::Regisiter(&type);
}

__uregister_enum(CalendarWeekRule)
{
	type.AddValue(L"FirstDay", CalendarWeekRule::FirstDay, __uhashstr(System::CalendarWeekRule::FirstDay));
	type.AddValue(L"FirstFullWeek", CalendarWeekRule::FirstFullWeek, __uhashstr(System::CalendarWeekRule::FirstFullWeek));
	type.AddValue(L"FirstFourDayWeek", CalendarWeekRule::FirstFourDayWeek, __uhashstr(System::CalendarWeekRule::FirstFourDayWeek));

	BuiltInAssemblyFactory::Regisiter(&type);
}

__uregister_class(Calendar, ClassFlags::Abstract)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}
