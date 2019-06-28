#include "HebrewCalendar.h"
#include "BuiltInAssembly.h"
using namespace System;

static const int HebrewTable[] =  { 7, 3, 17, 3, 0, 4, 11, 2, 21, 6, 1, 3, 13, 2, 25, 4, 5, 3, 16, 2, 27, 6, 9, 1, 20, 2, 0, 6, 11, 3, 23, 4, 4, 2, 14, 3, 27, 4, 8, 2, 18, 3, 28, 6, 11, 1, 22, 5, 2, 3, 12, 3, 25, 4, 6, 2, 16, 3, 26, 6, 8, 2, 20, 1, 0, 6, 11, 2, 24, 4, 4, 3, 15, 2, 25, 6, 8, 1, 19, 2, 29, 6, 9, 3, 22, 4, 3, 2, 13, 3, 25, 4, 6, 3, 17, 2, 27, 6, 7, 3, 19, 2, 31, 4, 11, 3, 23, 4, 5, 2, 15, 3, 25, 6, 6, 2, 19, 1, 29, 6, 10, 2, 22, 4, 3, 3, 14, 2, 24, 6, 6, 1, 17, 3, 28, 5, 8, 3, 20, 1, 32, 5, 12, 3, 22, 6, 4, 1, 16, 2, 26, 6, 6, 3, 17, 2, 0, 4, 10, 3, 22, 4, 3, 2, 14, 3, 24, 6, 5, 2, 17, 1, 28, 6, 9, 2, 19, 3, 31, 4, 13, 2, 23, 6, 3, 3, 15, 1, 27, 5, 7, 3, 17, 3, 29, 4, 11, 2, 21, 6, 3, 1, 14, 2, 25, 6, 5, 3, 16, 2, 28, 4, 9, 3, 20, 2, 0, 6, 12, 1, 23, 6, 4, 2, 14, 3, 26, 4, 8, 2, 18, 3, 0, 4, 10, 3, 21, 5, 1, 3, 13, 1, 24, 5, 5, 3, 15, 3, 27, 4, 8, 2, 19, 3, 29, 6, 10, 2, 22, 4, 3, 3, 14, 2, 26, 4, 6, 3, 18, 2, 28, 6, 10, 1, 20, 6, 2, 2, 12, 3, 24, 4, 5, 2, 16, 3, 28, 4, 8, 3, 19, 2, 0, 6, 12, 1, 23, 5, 3, 3, 14, 3, 26, 4, 7, 2, 17, 3, 28, 6, 9, 2, 21, 4, 1, 3, 13, 2, 25, 4, 5, 3, 16, 2, 27, 6, 9, 1, 19, 3, 0, 5, 11, 3, 23, 4, 4, 2, 14, 3, 25, 6, 7, 1, 18, 2, 28, 6, 9, 3, 21, 4, 2, 2, 12, 3, 25, 4, 6, 2, 16, 3, 26, 6, 8, 2, 20, 1, 0, 6, 11, 2, 22, 6, 4, 1, 15, 2, 25, 6, 6, 3, 18, 1, 29, 5, 9, 3, 22, 4, 2, 3, 13, 2, 23, 6, 4, 3, 15, 2, 27, 4, 7, 3, 19, 2, 31, 4, 11, 3, 21, 6, 3, 2, 15, 1, 25, 6, 6, 2, 17, 3, 29, 4, 10, 2, 20, 6, 3, 1, 13, 3, 24, 5, 4, 3, 16, 1, 27, 5, 7, 3, 17, 3, 0, 4, 11, 2, 21, 6, 1, 3, 13, 2, 25, 4, 5, 3, 16, 2, 29, 4, 9, 3, 19, 6, 30, 2, 13, 1, 23, 6, 4, 2, 14, 3, 27, 4, 8, 2, 18, 3, 0, 4, 11, 3, 22, 5, 2, 3, 14, 1, 26, 5, 6, 3, 16, 3, 28, 4, 10, 2, 20, 6, 30, 3, 11, 2, 24, 4, 4, 3, 15, 2, 25, 6, 8, 1, 19, 2, 29, 6, 9, 3, 22, 4, 3, 2, 13, 3, 25, 4, 7, 2, 17, 3, 27, 6, 9, 1, 21, 5, 1, 3, 11, 3, 23, 4, 5, 2, 15, 3, 25, 6, 6, 2, 19, 1, 29, 6, 10, 2, 22, 4, 3, 3, 14, 2, 24, 6, 6, 1, 18, 2, 28, 6, 8, 3, 20, 4, 2, 2, 12, 3, 24, 4, 4, 3, 16, 2, 26, 6, 6, 3, 17, 2, 0, 4, 10, 3, 22, 4, 3, 2, 14, 3, 24, 6, 5, 2, 17, 1, 28, 6, 9, 2, 21, 4, 1, 3, 13, 2, 23, 6, 5, 1, 15, 3, 27, 5, 7, 3, 19, 1, 0, 5, 10, 3, 22, 4, 2, 3, 13, 2, 24, 6, 4, 3, 15, 2, 27, 4, 8, 3, 20, 4, 1, 2, 11, 3, 22, 6, 3, 2, 15, 1, 25, 6, 7, 2, 17, 3, 29, 4, 10, 2, 21, 6, 1, 3, 13, 1, 24, 5, 5, 3, 15, 3, 27, 4, 8, 2, 19, 6, 1, 1, 12, 2, 22, 6, 3, 3, 14, 2, 26, 4, 6, 3, 18, 2, 28, 6, 10, 1, 20, 6, 2, 2, 12, 3, 24, 4, 5, 2, 16, 3, 28, 4, 9, 2, 19, 6, 30, 3, 12, 1, 23, 5, 3, 3, 14, 3, 26, 4, 7, 2, 17, 3, 28, 6, 9, 2, 21, 4, 1, 3, 13, 2, 25, 4, 5, 3, 16, 2, 27, 6, 9, 1, 19, 6, 30, 2, 11, 3, 23, 4, 4, 2, 14, 3, 27, 4, 7, 3, 18, 2, 28, 6, 11, 1, 22, 5, 2, 3, 12, 3, 25, 4, 6, 2, 16, 3, 26, 6, 8, 2, 20, 4, 30, 3, 11, 2, 24, 4, 4, 3, 15, 2, 25, 6, 8, 1, 18, 3, 29, 5, 9, 3, 22, 4, 3, 2, 13, 3, 23, 6, 6, 1, 17, 2, 27, 6, 7, 3, 20, 4, 1, 2, 11, 3, 23, 4, 5, 2, 15, 3, 25, 6, 6, 2, 19, 1, 29, 6, 10, 2, 20, 6, 3, 1, 14, 2, 24, 6, 4, 3, 17, 1, 28, 5, 8, 3, 20, 4, 1, 3, 12, 2, 22, 6, 2, 3, 14, 2, 26, 4, 6, 3, 17, 2, 0, 4, 10, 3, 20, 6, 1, 2, 14, 1, 24, 6, 5, 2, 15, 3, 28, 4, 9, 2, 19, 6, 1, 1, 12, 3, 23, 5, 3, 3, 15, 1, 27, 5, 7, 3, 17, 3, 29, 4, 11, 2, 21, 6, 1, 3, 12, 2, 25, 4, 5, 3, 16, 2, 28, 4, 9, 3, 19, 6, 30, 2, 12, 1, 23, 6, 4, 2, 14, 3, 26, 4, 8, 2, 18, 3, 0, 4, 10, 3, 22, 5, 2, 3, 14, 1, 25, 5, 6, 3, 16, 3, 28, 4, 9, 2, 20, 6, 30, 3, 11, 2, 23, 4, 4, 3, 15, 2, 27, 4, 7, 3, 19, 2, 29, 6, 11, 1, 21, 6, 3, 2, 13, 3, 25, 4, 6, 2, 17, 3, 27, 6, 9, 1, 20, 5, 30, 3, 10, 3, 22, 4, 3, 2, 14, 3, 24, 6, 5, 2, 17, 1, 28, 6, 9, 2, 21, 4, 1, 3, 13, 2, 23, 6, 5, 1, 16, 2, 27, 6, 7, 3, 19, 4, 30, 2, 11, 3, 23, 4, 3, 3, 14, 2, 25, 6, 5, 3, 16, 2, 28, 4, 9, 3, 21, 4, 2, 2, 12, 3, 23, 6, 4, 2, 16, 1, 26, 6, 8, 2, 20, 4, 30, 3, 11, 2, 22, 6, 4, 1, 14, 3, 25, 5, 6, 3, 18, 1, 29, 5, 9, 3, 22, 4, 2, 3, 13, 2, 23, 6, 4, 3, 15, 2, 27, 4, 7, 3, 20, 4, 1, 2, 11, 3, 21, 6, 3, 2, 15, 1, 25, 6, 6, 2, 17, 3, 29, 4, 10, 2, 20, 6, 3, 1, 13, 3, 24, 5, 4, 3, 17, 1, 28, 5, 8, 3, 18, 6, 1, 1, 12, 2, 22, 6, 2, 3, 14, 2, 26, 4, 6, 3, 17, 2, 28, 6, 10, 1, 20, 6, 1, 2, 12, 3, 24, 4, 5, 2, 15, 3, 28, 4, 9, 2, 19, 6, 33, 3, 12, 1, 23, 5, 3, 3, 13, 3, 25, 4, 6, 2, 16, 3, 26, 6, 8, 2, 20, 4, 30, 3, 11, 2, 24, 4, 4, 3, 15, 2, 25, 6, 8, 1, 18, 6, 33, 2, 9, 3, 22, 4, 3, 2, 13, 3, 25, 4, 6, 3, 17, 2, 27, 6, 9, 1, 21, 5, 1, 3, 11, 3, 23, 4, 5, 2, 15, 3, 25, 6, 6, 2, 19, 4, 33, 3, 10, 2, 22, 4, 3, 3, 14, 2, 24, 6, 6, 1 };

static const int LunarMonthLen[][14]= { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 30, 29, 29, 29, 30, 29, 30, 29, 30, 29, 30, 29, 0 }, { 0, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 0 }, { 0, 30, 30, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 0 }, { 0, 30, 29, 29, 29, 30, 30, 29, 30, 29, 30, 29, 30, 29 }, { 0, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30, 29, 30, 29 }, { 0, 30, 30, 30, 29, 30, 30, 29, 30, 29, 30, 29, 30, 29 } };

static const DateTime calendarMinValue = DateTime(1583, 1, 1);
static const DateTime calendarMaxValue = DateTime(DateTime(2239, 9, 29, 23, 59, 59, 999).Ticks + 9999LL);

namespace System
{
	class __DateBuffer
	{
	public:
		int year;

		int month;

		int day;

		__DateBuffer() : year(0), month(0), day(0)
		{
		}

		__DateBuffer(const __DateBuffer& buffer)
		{
			year = buffer.year;
			month = buffer.month;
			day = buffer.day;
		}
	};

	static int GetResult(__DateBuffer& result, int part)
	{
		switch (part)
		{
		case 0:
		{
			return result.year;
		}
		case 1:
		{
			throw std::exception("InvalidOperation_DateTimeParsing");
		}
		case 2:
		{
			return result.month;
		}
		case 3:
		{
			return result.day;
		}
		default:
		{
			throw std::exception("InvalidOperation_DateTimeParsing");
		}
		}
	}

	static int GetLunarMonthDay(int gregorianYear, __DateBuffer& lunarDate)
	{
		int num = gregorianYear - 1583;
		if (num < 0 || num > 656)
		{
			throw std::exception("gregorianYear");
		}
		num = num * 2;
		lunarDate.day = HebrewTable[num];
		int hebrewTable = HebrewTable[num + 1];
		int num1 = lunarDate.day;
		if (num1 == 0)
		{
			lunarDate.month = 5;
			lunarDate.day = 1;
		}
		else
		{
			switch (num1)
			{
			case 30:
			{
				lunarDate.month = 3;
				break;
			}
			case 31:
			{
				lunarDate.month = 5;
				lunarDate.day = 2;
				break;
			}
			case 32:
			{
				lunarDate.month = 5;
				lunarDate.day = 3;
				break;
			}
			case 33:
			{
				lunarDate.month = 3;
				lunarDate.day = 29;
				break;
			}
			default:
			{
				lunarDate.month = 4;
				break;
			}
			}
		}
		return hebrewTable;
	}
}

static void CheckEraRange(int era)
{
	if (era != 0 && era != 1)
	{
		throw std::exception("ArgumentOutOfRange_InvalidEraValue");
	}
}

static void CheckHebrewYearValue(int y, int era, const std::string& varName)
{
	CheckEraRange(era);
	if (y > 5999 || y < 5343)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
}

static void CheckTicksRange(Int64 ticks)
{
	if (ticks < calendarMinValue.Ticks || ticks > calendarMaxValue.Ticks)
	{
		throw std::exception("ArgumentOutOfRange_CalendarRange");
	}
}


static int GetDayDifference(int lunarYearType, int month1, int day1, int month2, int day2)
{
	if (month1 == month2)
	{
		return day1 - day2;
	}
	bool flag = month1 > month2;
	if (flag)
	{
		int num = month1;
		int num1 = day1;
		month1 = month2;
		day1 = day2;
		month2 = num;
		day2 = num1;
	}
	int lunarMonthLen = LunarMonthLen[lunarYearType][month1] - day1;
	month1++;
	while (month1 < month2)
	{
		int num2 = month1;
		month1 = num2 + 1;
		lunarMonthLen = lunarMonthLen + LunarMonthLen[lunarYearType][num2];
	}
	lunarMonthLen = lunarMonthLen + day2;
	if (flag)
	{
		return lunarMonthLen;
	}
	return -lunarMonthLen;
}

static int GetHebrewYearType(int year, int era)
{
	CheckHebrewYearValue(year, era, "year");
	return HebrewTable[(year - 3760 - 1583) * 2 + 1];
}

Calendar* HebrewCalendar::InstanceClone() const
{
	HebrewCalendar* cal = new HebrewCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

CalendarAlgorithmType HebrewCalendar::GetAlgorithmType() const 
{
	return CalendarAlgorithmType::LunisolarCalendar;
}

std::vector<int> HebrewCalendar::GetEras() const 
{
	return std::vector<int>{ 1 };
}

int HebrewCalendar::GetID() const 
{
	return 8;
}

DateTime HebrewCalendar::GetMaxSupportedDateTime() const 
{
	return calendarMaxValue;
}

DateTime HebrewCalendar::GetMinSupportedDateTime() const 
{
	return calendarMinValue;
}

int HebrewCalendar::GetTwoDigitYearMax() const 
{
	if (twoDigitYearMax == -1)
	{
		twoDigitYearMax = Calendar::GetSystemTwoDigitYearSetting(GetID(), 5790);
	}
	return twoDigitYearMax;
}

void HebrewCalendar::SetTwoDigitYearMax(int value) 
{
	VerifyWritable();
	if (value != 99)
	{
		CheckHebrewYearValue(value, 1, "value");
	}
	twoDigitYearMax = value;
}

HebrewCalendar::HebrewCalendar()
{
}

HebrewCalendar::~HebrewCalendar() {}

DateTime HebrewCalendar::AddMonths(const DateTime& time, int months) 
{
	int monthsInYear;
	int num;
	DateTime dateTime;
	try
	{
		int datePart = GetDatePart(time.Ticks, 0);
		int datePart1 = GetDatePart(time.Ticks, 2);
		int num1 = GetDatePart(time.Ticks, 3);
		if (months < 0)
		{
			int num2 = datePart1 + months;
			num = num2;
			if (num2 <= 0)
			{
				months = -months;
				months = months - datePart1;
				datePart--;
				while (true)
				{
					int monthsInYear1 = GetMonthsInYear(datePart, 0);
					monthsInYear = monthsInYear1;
					if (months <= monthsInYear1)
					{
						break;
					}
					datePart--;
					months = months - monthsInYear;
				}
				monthsInYear = GetMonthsInYear(datePart, 0);
				num = monthsInYear - months;
			}
		}
		else
		{
			num = datePart1 + months;
			while (true)
			{
				int monthsInYear2 = GetMonthsInYear(datePart, 0);
				monthsInYear = monthsInYear2;
				if (num <= monthsInYear2)
				{
					break;
				}
				datePart++;
				num = num - monthsInYear;
			}
		}
		int daysInMonth = GetDaysInMonth(datePart, num, 0);
		if (num1 > daysInMonth)
		{
			num1 = daysInMonth;
		}
		DateTime dateTime1 = ToDateTime(datePart, num, num1, 0, 0, 0, 0, 0);
		dateTime = DateTime(dateTime1.Ticks + time.Ticks % TimeSpan::DAYS);
	}
	catch (...)
	{
		throw std::exception("ArgumentOutOfRange_AddValue_Month");
	}
	return dateTime;
}

DateTime HebrewCalendar::AddYears(const DateTime& time, int years) 
{
	int datePart = GetDatePart(time.Ticks, 0);
	int num = GetDatePart(time.Ticks, 2);
	int datePart1 = GetDatePart(time.Ticks, 3);
	datePart = datePart + years;
	CheckHebrewYearValue(datePart, 0, "years");
	int monthsInYear = GetMonthsInYear(datePart, 0);
	if (num > monthsInYear)
	{
		num = monthsInYear;
	}
	int daysInMonth = GetDaysInMonth(datePart, num, 0);
	if (datePart1 > daysInMonth)
	{
		datePart1 = daysInMonth;
	}
	DateTime dateTime = ToDateTime(datePart, num, datePart1, 0, 0, 0, 0, 0);
	Int64 ticks = dateTime.Ticks + time.Ticks % TimeSpan::DAYS;
	Calendar::CheckAddResult(ticks, GetMinSupportedDateTime(), GetMaxSupportedDateTime());
	return DateTime(ticks);
}

void HebrewCalendar::CheckHebrewDayValue(int year, int month, int day, int era) const
{
	int daysInMonth = GetDaysInMonth(year, month, era);
	if (day < 1 || day > daysInMonth)
	{
		throw std::exception("ArgumentOutOfRange_Day");
	}
}

void HebrewCalendar::CheckHebrewMonthValue(int year, int month, int era) const
{
	int monthsInYear = GetMonthsInYear(year, era);
	if (month < 1 || month > monthsInYear)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
}

int HebrewCalendar::GetDatePart(Int64 ticks, int part) const
{
	CheckTicksRange(ticks);
	DateTime dateTime = DateTime(ticks);
	int year = dateTime.Year;
	int month = dateTime.Month;
	int day = dateTime.Day;
	__DateBuffer _DateBuffer = __DateBuffer();
	_DateBuffer.year = 3760;
	int lunarMonthDay = GetLunarMonthDay(year, _DateBuffer);
	__DateBuffer _DateBuffer1 = __DateBuffer(_DateBuffer);

	Int64 absoluteDate = GregorianCalendar::GetAbsoluteDate(year, month, day);
	if (month == 1 && day == 1)
	{
		return GetResult(_DateBuffer1, part);
	}
	Int64 lunarMonthLen = absoluteDate - GregorianCalendar::GetAbsoluteDate(year, 1, 1);
	if (lunarMonthLen + (Int64)_DateBuffer.day <= (Int64)LunarMonthLen[lunarMonthDay, _DateBuffer.month])
	{
		__DateBuffer _DateBuffer2 = _DateBuffer1;
		_DateBuffer2.day = _DateBuffer2.day + (int)lunarMonthLen;
		return GetResult(_DateBuffer1, part);
	}
	__DateBuffer _DateBuffer3 = _DateBuffer1;
	_DateBuffer3.month = _DateBuffer3.month + 1;
	_DateBuffer1.day = 1;
	lunarMonthLen = lunarMonthLen - (Int64)(LunarMonthLen[lunarMonthDay, _DateBuffer.month] - _DateBuffer.day);
	if (lunarMonthLen > (Int64)1)
	{
		while (lunarMonthLen > (Int64)LunarMonthLen[lunarMonthDay, _DateBuffer1.month])
		{
			auto numArray = LunarMonthLen;
			__DateBuffer _DateBuffer4 = _DateBuffer1;
			int num = _DateBuffer4.month;
			int num1 = num;
			_DateBuffer4.month = num + 1;
			lunarMonthLen = lunarMonthLen - (Int64)numArray[lunarMonthDay, num1];
			if (_DateBuffer1.month <= 13 && LunarMonthLen[lunarMonthDay, _DateBuffer1.month] != 0)
			{
				continue;
			}
			__DateBuffer _DateBuffer5 = _DateBuffer1;
			_DateBuffer5.year = _DateBuffer5.year + 1;
			lunarMonthDay = HebrewTable[(year + 1 - 1583) * 2 + 1];
			_DateBuffer1.month = 1;
		}
		__DateBuffer _DateBuffer6 = _DateBuffer1;
		_DateBuffer6.day = _DateBuffer6.day + (int)(lunarMonthLen - (Int64)1);
	}
	return GetResult(_DateBuffer1, part);
}

int HebrewCalendar::GetDayOfMonth(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 3);
}

DayOfWeek HebrewCalendar::GetDayOfWeek(const DateTime& time) const 
{
	return (DayOfWeek)((int)(time.Ticks / TimeSpan::DAYS + 1) % 7);
}

int HebrewCalendar::GetDayOfYear(const DateTime& time) const 
{
	DateTime dateTime;
	int year = GetYear(time);
	dateTime = (year != 5343 ? ToDateTime(year, 1, 1, 0, 0, 0, 0, 0) : DateTime(1582, 9, 27));
	return (int)((time.Ticks - dateTime.Ticks) / TimeSpan::DAYS) + 1;
}

int HebrewCalendar::GetDaysInMonth(int year, int month, int era) const 
{
	CheckEraRange(era);
	int hebrewYearType = GetHebrewYearType(year, era);
	CheckHebrewMonthValue(year, month, era);
	int lunarMonthLen = LunarMonthLen[hebrewYearType][month];
	if (lunarMonthLen == 0)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	return lunarMonthLen;
}

int HebrewCalendar::GetDaysInYear(int year, int era) const 
{
	CheckEraRange(era);
	int hebrewYearType = GetHebrewYearType(year, era);
	if (hebrewYearType < 4)
	{
		return 352 + hebrewYearType;
	}
	return 382 + (hebrewYearType - 3);
}

int HebrewCalendar::GetEra(const DateTime& time) const 
{
	return 1;
}

int HebrewCalendar::GetLeapMonth(int year, int era) const 
{
	if (IsLeapYear(year, era))
	{
		return 7;
	}
	return 0;
}

int HebrewCalendar::GetMonth(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 2);
}

int HebrewCalendar::GetMonthsInYear(int year, int era) const 
{
	if (!IsLeapYear(year, era))
	{
		return 12;
	}
	return 13;
}

int HebrewCalendar::GetYear(const DateTime& time) const 
{
	return GetDatePart(time.Ticks, 0);
}

DateTime HebrewCalendar::HebrewToGregorian(int hebrewYear, int hebrewMonth, int hebrewDay, int hour, int minute, int second, int millisecond)
{
	int num = hebrewYear - 3760;
	__DateBuffer _DateBuffer = __DateBuffer();
	int lunarMonthDay = GetLunarMonthDay(num, _DateBuffer);
	if (hebrewMonth == _DateBuffer.month && hebrewDay == _DateBuffer.day)
	{
		return DateTime(num, 1, 1, hour, minute, second, millisecond);
	}
	int dayDifference = GetDayDifference(lunarMonthDay, hebrewMonth, hebrewDay, _DateBuffer.month, _DateBuffer.day);
	DateTime dateTime = DateTime(num, 1, 1);
	return DateTime(dateTime.Ticks + (Int64)dayDifference * TimeSpan::DAYS + TimeToTicks(hour, minute, second, millisecond));
}

bool HebrewCalendar::IsLeapDay(int year, int month, int day, int era) const 
{
	if (IsLeapMonth(year, month, era))
	{
		CheckHebrewDayValue(year, month, day, era);
		return true;
	}
	if (IsLeapYear(year, 0) && month == 6 && day == 30)
	{
		return true;
	}
	CheckHebrewDayValue(year, month, day, era);
	return false;
}

bool HebrewCalendar::IsLeapMonth(int year, int month, int era) const 
{
	bool flag = IsLeapYear(year, era);
	CheckHebrewMonthValue(year, month, era);
	if (flag && month == 7)
	{
		return true;
	}
	return false;
}

bool HebrewCalendar::IsLeapYear(int year, int era) const 
{
	CheckHebrewYearValue(year, era, "year");
	return (7LL * (Int64)year +1LL) %19LL < 7LL;
}

DateTime HebrewCalendar::ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const 
{
	CheckHebrewYearValue(year, era, "year");
	CheckHebrewMonthValue(year, month, era);
	CheckHebrewDayValue(year, month, day, era);
	DateTime gregorian = HebrewToGregorian(year, month, day, hour, minute, second, millisecond);
	CheckTicksRange(gregorian.Ticks);
	return gregorian;
}

int HebrewCalendar::ToFourDigitYear(int year) const 
{
	if (year < 0)
	{
		throw std::exception("ArgumentOutOfRange_NeedNonNegNum_year");
	}
	if (year < 100)
	{
		return Calendar::ToFourDigitYear(year);
	}
	if (year > 5999 || year < 5343)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return year;
}

__uregister_class(HebrewCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}