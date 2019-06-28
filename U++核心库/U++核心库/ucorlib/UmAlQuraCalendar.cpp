#include "UmAlQuraCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

namespace System
{
	struct DateMapping
	{
		int HijriMonthsLengthFlags;

		DateTime GregorianDate;

		DateMapping(int MonthsLengthFlags, int GYear, int GMonth, int GDay)
		{
			HijriMonthsLengthFlags = MonthsLengthFlags;
			GregorianDate = DateTime(GYear, GMonth, GDay);
		}
	};
}

static const std::vector<DateMapping> HijriYearInfo = { DateMapping(746, 1900, 4, 30), DateMapping(1769, 1901, 4, 19), DateMapping(3794, 1902, 4, 9), DateMapping(3748, 1903, 3, 30), DateMapping(3402, 1904, 3, 18), DateMapping(2710, 1905, 3, 7), DateMapping(1334, 1906, 2, 24), DateMapping(2741, 1907, 2, 13), DateMapping(3498, 1908, 2, 3), DateMapping(2980, 1909, 1, 23), DateMapping(2889, 1910, 1, 12), DateMapping(2707, 1911, 1, 1), DateMapping(1323, 1911, 12, 21), DateMapping(2647, 1912, 12, 9), DateMapping(1206, 1913, 11, 29), DateMapping(2741, 1914, 11, 18), DateMapping(1450, 1915, 11, 8), DateMapping(3413, 1916, 10, 27), DateMapping(3370, 1917, 10, 17), DateMapping(2646, 1918, 10, 6), DateMapping(1198, 1919, 9, 25), DateMapping(2397, 1920, 9, 13), DateMapping(748, 1921, 9, 3), DateMapping(1749, 1922, 8, 23), DateMapping(1706, 1923, 8, 13), DateMapping(1365, 1924, 8, 1), DateMapping(1195, 1925, 7, 21), DateMapping(2395, 1926, 7, 10), DateMapping(698, 1927, 6, 30), DateMapping(1397, 1928, 6, 18), DateMapping(2994, 1929, 6, 8), DateMapping(1892, 1930, 5, 29), DateMapping(1865, 1931, 5, 18), DateMapping(1621, 1932, 5, 6), DateMapping(683, 1933, 4, 25), DateMapping(1371, 1934, 4, 14), DateMapping(2778, 1935, 4, 4), DateMapping(1748, 1936, 3, 24), DateMapping(3785, 1937, 3, 13), DateMapping(3474, 1938, 3, 3), DateMapping(3365, 1939, 2, 20), DateMapping(2637, 1940, 2, 9), DateMapping(685, 1941, 1, 28), DateMapping(1389, 1942, 1, 17), DateMapping(2922, 1943, 1, 7), DateMapping(2898, 1943, 12, 28), DateMapping(2725, 1944, 12, 16), DateMapping(2635, 1945, 12, 5), DateMapping(1175, 1946, 11, 24), DateMapping(2359, 1947, 11, 13), DateMapping(694, 1948, 11, 2), DateMapping(1397, 1949, 10, 22), DateMapping(3434, 1950, 10, 12), DateMapping(3410, 1951, 10, 2), DateMapping(2710, 1952, 9, 20), DateMapping(2349, 1953, 9, 9), DateMapping(605, 1954, 8, 29), DateMapping(1245, 1955, 8, 18), DateMapping(2778, 1956, 8, 7), DateMapping(1492, 1957, 7, 28), DateMapping(3497, 1958, 7, 17), DateMapping(3410, 1959, 7, 7), DateMapping(2730, 1960, 6, 25), DateMapping(1238, 1961, 6, 14), DateMapping(2486, 1962, 6, 3), DateMapping(884, 1963, 5, 24), DateMapping(1897, 1964, 5, 12), DateMapping(1874, 1965, 5, 2), DateMapping(1701, 1966, 4, 21), DateMapping(1355, 1967, 4, 10), DateMapping(2731, 1968, 3, 29), DateMapping(1370, 1969, 3, 19), DateMapping(2773, 1970, 3, 8), DateMapping(3538, 1971, 2, 26), DateMapping(3492, 1972, 2, 16), DateMapping(3401, 1973, 2, 4), DateMapping(2709, 1974, 1, 24), DateMapping(1325, 1975, 1, 13), DateMapping(2653, 1976, 1, 2), DateMapping(1370, 1976, 12, 22), DateMapping(2773, 1977, 12, 11), DateMapping(1706, 1978, 12, 1), DateMapping(1685, 1979, 11, 20), DateMapping(1323, 1980, 11, 8), DateMapping(2647, 1981, 10, 28), DateMapping(1198, 1982, 10, 18), DateMapping(2422, 1983, 10, 7), DateMapping(1388, 1984, 9, 26), DateMapping(2901, 1985, 9, 15), DateMapping(2730, 1986, 9, 5), DateMapping(2645, 1987, 8, 25), DateMapping(1197, 1988, 8, 13), DateMapping(2397, 1989, 8, 2), DateMapping(730, 1990, 7, 23), DateMapping(1497, 1991, 7, 12), DateMapping(3506, 1992, 7, 1), DateMapping(2980, 1993, 6, 21), DateMapping(2890, 1994, 6, 10), DateMapping(2645, 1995, 5, 30), DateMapping(693, 1996, 5, 18), DateMapping(1397, 1997, 5, 7), DateMapping(2922, 1998, 4, 27), DateMapping(3026, 1999, 4, 17), DateMapping(3012, 2000, 4, 6), DateMapping(2953, 2001, 3, 26), DateMapping(2709, 2002, 3, 15), DateMapping(1325, 2003, 3, 4), DateMapping(1453, 2004, 2, 21), DateMapping(2922, 2005, 2, 10), DateMapping(1748, 2006, 1, 31), DateMapping(3529, 2007, 1, 20), DateMapping(3474, 2008, 1, 10), DateMapping(2726, 2008, 12, 29), DateMapping(2390, 2009, 12, 18), DateMapping(686, 2010, 12, 7), DateMapping(1389, 2011, 11, 26), DateMapping(874, 2012, 11, 15), DateMapping(2901, 2013, 11, 4), DateMapping(2730, 2014, 10, 25), DateMapping(2381, 2015, 10, 14), DateMapping(1181, 2016, 10, 2), DateMapping(2397, 2017, 9, 21), DateMapping(698, 2018, 9, 11), DateMapping(1461, 2019, 8, 31), DateMapping(1450, 2020, 8, 20), DateMapping(3413, 2021, 8, 9), DateMapping(2714, 2022, 7, 30), DateMapping(2350, 2023, 7, 19), DateMapping(622, 2024, 7, 7), DateMapping(1373, 2025, 6, 26), DateMapping(2778, 2026, 6, 16), DateMapping(1748, 2027, 6, 6), DateMapping(1701, 2028, 5, 25), DateMapping(1355, 2029, 5, 14), DateMapping(2711, 2030, 5, 3), DateMapping(1358, 2031, 4, 23), DateMapping(2734, 2032, 4, 11), DateMapping(1452, 2033, 4, 1), DateMapping(2985, 2034, 3, 21), DateMapping(3474, 2035, 3, 11), DateMapping(2853, 2036, 2, 28), DateMapping(1611, 2037, 2, 16), DateMapping(3243, 2038, 2, 5), DateMapping(1370, 2039, 1, 26), DateMapping(2901, 2040, 1, 15), DateMapping(1746, 2041, 1, 4), DateMapping(3749, 2041, 12, 24), DateMapping(3658, 2042, 12, 14), DateMapping(2709, 2043, 12, 3), DateMapping(1325, 2044, 11, 21), DateMapping(2733, 2045, 11, 10), DateMapping(876, 2046, 10, 31), DateMapping(1881, 2047, 10, 20), DateMapping(1746, 2048, 10, 9), DateMapping(1685, 2049, 9, 28), DateMapping(1325, 2050, 9, 17), DateMapping(2651, 2051, 9, 6), DateMapping(1210, 2052, 8, 26), DateMapping(2490, 2053, 8, 15), DateMapping(948, 2054, 8, 5), DateMapping(2921, 2055, 7, 25), DateMapping(2898, 2056, 7, 14), DateMapping(2726, 2057, 7, 3), DateMapping(1206, 2058, 6, 22), DateMapping(2413, 2059, 6, 11), DateMapping(748, 2060, 5, 31), DateMapping(1753, 2061, 5, 20), DateMapping(3762, 2062, 5, 10), DateMapping(3412, 2063, 4, 30), DateMapping(3370, 2064, 4, 18), DateMapping(2646, 2065, 4, 7), DateMapping(1198, 2066, 3, 27), DateMapping(2413, 2067, 3, 16), DateMapping(3434, 2068, 3, 5), DateMapping(2900, 2069, 2, 23), DateMapping(2857, 2070, 2, 12), DateMapping(2707, 2071, 2, 1), DateMapping(1323, 2072, 1, 21), DateMapping(2647, 2073, 1, 9), DateMapping(1334, 2073, 12, 30), DateMapping(2741, 2074, 12, 19), DateMapping(1706, 2075, 12, 9), DateMapping(3731, 2076, 11, 27), DateMapping(0, 2077, 11, 17) };
static const DateTime minDate = DateTime(1900, 4, 30);
static const DateTime maxDate = DateTime(DateTime(2077, 11, 16, 23, 59, 59, 999).Ticks + 9999LL);

static void CheckEraRange(int era)
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
}

static void CheckTicksRange(long long ticks)
{
	if (ticks < minDate.Ticks || ticks > maxDate.Ticks)
	{
		throw std::exception("ArgumentOutOfRange_CalendarRange");
	}
}

static void CheckYearRange(int year, int era)
{
	CheckEraRange(era);
	if (year < 1318 || year > 1500)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
}

static void CheckYearMonthRange(int year, int month, int era)
{
	CheckYearRange(year, era);
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
}


static void ConvertGregorianToHijri(const DateTime& time, int& HijriYear, int& HijriMonth, int& HijriDay)
{
	int num;
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	int ticks = (int)((time.Ticks - minDate.Ticks) / TimeSpan::DAYS) / 355;
	do
	{
		num = ticks + 1;
		ticks = num;
	} while (time>HijriYearInfo[num].GregorianDate);
	if (time != HijriYearInfo[ticks].GregorianDate)
	{
		ticks--;
	}
	TimeSpan timeSpan = (time - HijriYearInfo[ticks].GregorianDate).Ticks;
	num1 = ticks + 1318;
	num2 = 1;
	num3 = 1;
	double totalDays = timeSpan.TotalDays;
	int hijriMonthsLengthFlags = HijriYearInfo[ticks].HijriMonthsLengthFlags;
	int num4 = 29 + (hijriMonthsLengthFlags & 1);
	while (totalDays >= (double)num4)
	{
		totalDays = totalDays - (double)num4;
		hijriMonthsLengthFlags = hijriMonthsLengthFlags >> 1;
		num4 = 29 + (hijriMonthsLengthFlags & 1);
		num2++;
	}
	num3 = num3 + (int)totalDays;
	HijriDay = num3;
	HijriMonth = num2;
	HijriYear = num1;
}

static void ConvertHijriToGregorian(int HijriYear, int HijriMonth, int HijriDay, int& yg, int& mg, int& dg)
{
	int hijriDay = HijriDay - 1;
	int hijriYear = HijriYear - 1318;
	DateTime gregorianDate = HijriYearInfo[hijriYear].GregorianDate;
	int hijriMonthsLengthFlags = HijriYearInfo[hijriYear].HijriMonthsLengthFlags;
	for (int i = 1; i < HijriMonth; i++)
	{
		hijriDay = hijriDay + 29 + (hijriMonthsLengthFlags & 1);
		hijriMonthsLengthFlags = hijriMonthsLengthFlags >> 1;
	}
	gregorianDate = gregorianDate.AddDays((double)hijriDay);
	yg = gregorianDate.Year;
	mg = gregorianDate.Month;
	dg = gregorianDate.Day;
}

static Int64 GetAbsoluteDateUmAlQura(int year, int month, int day)
{
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	ConvertHijriToGregorian(year, month, day, num, num1, num2);
	return GregorianCalendar::GetAbsoluteDate(num, num1, num2);
}

static int RealGetDaysInYear(int year)
{
	int num = 0;
	int hijriMonthsLengthFlags = HijriYearInfo[year - 1318].HijriMonthsLengthFlags;
	for (int i = 1; i <= 12; i++)
	{
		num = num + 29 + (hijriMonthsLengthFlags & 1);
		hijriMonthsLengthFlags = hijriMonthsLengthFlags >> 1;
	}
	return num;
}

Calendar* UmAlQuraCalendar::InstanceClone() const
{
	UmAlQuraCalendar* cal = new UmAlQuraCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType UmAlQuraCalendar::GetAlgorithmType() const 
{
	return CalendarAlgorithmType::LunarCalendar;
}

int UmAlQuraCalendar::GetBaseCalendarID() const 
{
	return 6;
}

int UmAlQuraCalendar::GetDaysInYearBeforeMinSupportedYear() const 
{
	return 355;
}

std::vector<int> UmAlQuraCalendar::GetEras() const 
{
	return std::vector<int>{ 1 };
}

int UmAlQuraCalendar::GetID() const 
{
	return 23;
}

DateTime UmAlQuraCalendar::GetMaxSupportedDateTime() const 
{
	return maxDate;
}

DateTime UmAlQuraCalendar::GetMinSupportedDateTime() const 
{
	return minDate;
}

int UmAlQuraCalendar::GetTwoDigitYearMax() const 
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetID(), 1451);
	}
	return twoDigitYearMax;
}

void UmAlQuraCalendar::SetTwoDigitYearMax(int value) 
{
	if (value != 99 && (value < 1318 || value > 1500))
	{
		throw std::exception("ArgumentOutOfRange");
	}
	VerifyWritable();
	twoDigitYearMax = value;
}

UmAlQuraCalendar::UmAlQuraCalendar()
{
}

UmAlQuraCalendar::~UmAlQuraCalendar() {}

DateTime UmAlQuraCalendar::AddMonths(const DateTime& time, int months)
{
	if (months < -120000 || months > 120000)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	int datePart = GetDatePart(time, 0);
	int num = GetDatePart(time, 2);
	int datePart1 = GetDatePart(time, 3);
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
	if (datePart1 > 29)
	{
		int daysInMonth = GetDaysInMonth(datePart, num, 0);
		if (datePart1 > daysInMonth)
		{
			datePart1 = daysInMonth;
		}
	}
	CheckYearRange(datePart, 1);
	DateTime dateTime = DateTime(GetAbsoluteDateUmAlQura(datePart, num, datePart1) * TimeSpan::DAYS + time.Ticks % TimeSpan::DAYS);
	Calendar::CheckAddResult(dateTime.Ticks, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return dateTime;
}

DateTime UmAlQuraCalendar::AddYears(const DateTime& time, int years)
{
	return AddMonths(time, years * 12);
}




int UmAlQuraCalendar::GetDatePart(const DateTime& time, int part) const
{
	int num = 0;
	int num1 = 0;
	int num2 = 0;
	CheckTicksRange(time.Ticks);
	ConvertGregorianToHijri(time, num, num1, num2);
	if (part == 0)
	{
		return num;
	}
	if (part == 2)
	{
		return num1;
	}
	if (part == 3)
	{
		return num2;
	}
	if (part != 1)
	{
		throw std::exception("InvalidOperation_DateTimeParsing");
	}
	return (int)(GetAbsoluteDateUmAlQura(num, num1, num2) - GetAbsoluteDateUmAlQura(num, 1, 1) + (long)1);
}

int UmAlQuraCalendar::GetDayOfMonth(const DateTime& time) const 
{
	return GetDatePart(time, 3);
}

DayOfWeek UmAlQuraCalendar::GetDayOfWeek(const DateTime& time) const 
{
	return (DayOfWeek)((int)(time.Ticks / TimeSpan::DAYS + 1) % 7);
}

int UmAlQuraCalendar::GetDayOfYear(const DateTime& time)const 
{
	return GetDatePart(time, 1);
}

int UmAlQuraCalendar::GetDaysInMonth(int year, int month, int era)const 
{
	CheckYearMonthRange(year, month, era);
	if ((HijriYearInfo[year - 1318].HijriMonthsLengthFlags & 1 << (month - 1 & 31)) == 0)
	{
		return 29;
	}
	return 30;
}

int UmAlQuraCalendar::GetDaysInYear(int year, int era) const
{
	CheckYearRange(year, era);
	return RealGetDaysInYear(year);
}

int UmAlQuraCalendar::GetEra(const DateTime& time)const 
{
	CheckTicksRange(time.Ticks);
	return 1;
}

int UmAlQuraCalendar::GetLeapMonth(int year, int era)const 
{
	CheckYearRange(year, era);
	return 0;
}

int UmAlQuraCalendar::GetMonth(const DateTime& time)const 
{
	return GetDatePart(time, 2);
}

int UmAlQuraCalendar::GetMonthsInYear(int year, int era)const 
{
	CheckYearRange(year, era);
	return 12;
}

int UmAlQuraCalendar::GetYear(const DateTime& time)const 
{
	return GetDatePart(time, 0);
}

bool UmAlQuraCalendar::IsLeapDay(int year, int month, int day, int era)const 
{
	if (day >= 1 && day <= 29)
	{
		CheckYearMonthRange(year, month, era);
		return false;
	}
	int daysInMonth = GetDaysInMonth(year, month, era);
	if (day < 1 || day > daysInMonth)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
	return false;
}

bool UmAlQuraCalendar::IsLeapMonth(int year, int month, int era)const 
{
	CheckYearMonthRange(year, month, era);
	return false;
}

bool UmAlQuraCalendar::IsLeapYear(int year, int era)const 
{
	CheckYearRange(year, era);
	return RealGetDaysInYear(year) == 355;
}


DateTime UmAlQuraCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era)const 
{
	if (day < 1 || day > 29)
	{
		int daysInMonth = GetDaysInMonth(year, month, era);
		if (day < 1 || day > daysInMonth)
		{
			throw std::exception("ArgumentOutOfRange_Day");
		}
	}
	else
	{
		CheckYearMonthRange(year, month, era);
	}
	Int64 absoluteDateUmAlQura = GetAbsoluteDateUmAlQura(year, month, day);
	if (absoluteDateUmAlQura < 0L)
	{
		throw std::exception("ArgumentOutOfRange_BadYearMonthDay");
	}
	return DateTime(absoluteDateUmAlQura * TimeSpan::DAYS + Calendar::TimeToTicks(hour, minute, second, millisecond));
}

int UmAlQuraCalendar::ToFourDigitYear(int year) const 
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_Year");
	}
	if (year < 100)
	{
		return Calendar::ToFourDigitYear(year);
	}
	if (year < 1318 || year > 1500)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}

__uregister_class(UmAlQuraCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}