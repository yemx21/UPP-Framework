#pragma once
#include "Calendar.h"
#include "Optional.h"
#include "GregorianCalendar.h"
namespace System
{
	class GregorianCalendarHelper : _noncopyable
	{
	public:
		static const int DaysToMonth365[];

		static const int  DaysToMonth366[];

		int m_maxYear;

		int m_minYear;

		Calendar* m_Cal;

		std::vector<EraInfo> m_EraInfo;

		mutable Optional<std::vector<int>> m_eras;

		DateTime m_minDate;

		std::vector<int> GetEras() const;

		int GetMaxYear() const;

		GregorianCalendarHelper(Calendar* cal, const std::vector<EraInfo>& eraInfo);

		virtual ~GregorianCalendarHelper();

		DateTime AddMonths(const DateTime& time, int months);

		DateTime AddYears(const DateTime& time, int years);

		void CheckTicksRange(Int64 ticks) const;

		static Int64 DateToTicks(int year, int month, int day);

		static long GetAbsoluteDate(int year, int month, int day);

		virtual int GetDatePart(Int64 ticks, int part) const;

		int GetDayOfMonth(const DateTime& time) const;

		DayOfWeek GetDayOfWeek(const DateTime& time) const;

		int GetDayOfYear(const DateTime& time) const;

		int GetDaysInMonth(int year, int month, int era) const;

		int GetDaysInYear(int year, int era)  const;

		int GetEra(const DateTime& time) const;

		int GetGregorianYear(int year, int era) const;

		int GetLeapMonth(int year, int era) const;

		int GetMonth(const DateTime& time) const;

		int GetMonthsInYear(int year, int era) const;

		virtual int GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek)  const;

		int GetYear(const DateTime& time)  const;

		int GetYear(int year, const DateTime& time)  const;

		bool IsLeapDay(int year, int month, int day, int era)  const;

		bool IsLeapMonth(int year, int month, int era)  const;

		bool IsLeapYear(int year, int era)  const;

		bool IsValidYear(int year, int era) const;

		static Int64 TimeToTicks(int hour, int minute, int second, int millisecond);

		DateTime ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era)  const;

		int ToFourDigitYear(int year, int twoDigitYearMax)  const;
	};
}