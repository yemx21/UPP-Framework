#pragma once
#include "Calendars.h"
namespace System
{
	class CalendarHelper
	{
	public:
		static GregorianCalendar& GetGregorianDefaultInstance();

		static bool GregorianTryToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era, DateTime& result);

		static JapaneseCalendar& GetJapaneseDefaultInstance();

		static TaiwanCalendar& GetTaiwanDefaultInstance();

	};
}