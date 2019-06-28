#include "CalendarHelper.h"

using namespace System;

GregorianCalendar& CalendarHelper::GetGregorianDefaultInstance()
{
	static GregorianCalendar cal;
	return cal;
}

bool CalendarHelper::GregorianTryToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era, DateTime& result)
{
	auto& cal = GetGregorianDefaultInstance();
	return cal.TryToDateTime(year, month, day, hour, minute, second, millisecond, era, result);
}

JapaneseCalendar& CalendarHelper::GetJapaneseDefaultInstance()
{
	static JapaneseCalendar cal;
	return cal;
}

TaiwanCalendar& CalendarHelper::GetTaiwanDefaultInstance()
{
	static TaiwanCalendar cal;
	return cal;
}

