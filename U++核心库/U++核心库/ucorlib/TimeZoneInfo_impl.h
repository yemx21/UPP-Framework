#pragma once
#include "TimeZoneInfo.h"

namespace System
{
	struct TransitionTime
	{
	public:
		DateTime m_timeOfDay;

		Byte m_month;

		Byte m_week;

		Byte m_day;

		DayOfWeek m_dayOfWeek;

		bool m_isFixedDateRule;

		static void ValidateTransitionTime(const DateTime& timeOfDay, int month, int week, int day, DayOfWeek dayOfWeek)
		{
			if (timeOfDay.Kind != DateTimeKind::Unspecified)
			{
				throw std::exception("Argument_DateTimeKindMustBeUnspecified");
			}
			if (month < 1 || month > 12)
			{
				throw std::exception("ArgumentOutOfRange_MonthParam");
			}
			if (day < 1 || day > 31)
			{
				throw std::exception("ArgumentOutOfRange_DayParam");
			}
			if (week < 1 || week > 5)
			{
				throw std::exception("ArgumentOutOfRange_Week");
			}
			if (dayOfWeek < DayOfWeek::Sunday || dayOfWeek > DayOfWeek::Saturday)
			{
				throw std::exception("ArgumentOutOfRange_DayOfWeek");
			}
			if (timeOfDay.Year != 1 || timeOfDay.Month != 1 || timeOfDay.Day != 1 || timeOfDay.Ticks % 10000LL != 0LL)
			{
				throw std::exception("Argument_DateTimeHasTicks");
			}
		}

		static TransitionTime CreateTransitionTime(const DateTime& timeOfDay, int month, int week, int day, DayOfWeek dayOfWeek, bool isFixedDateRule)
		{
			TransitionTime::ValidateTransitionTime(timeOfDay, month, week, day, dayOfWeek);
			return TransitionTime(timeOfDay, month, week, day, dayOfWeek, isFixedDateRule);
		}

		TransitionTime() :m_month(1), m_week(0), m_day(1), m_isFixedDateRule(false)
		{

		}

		TransitionTime(const DateTime& timeOfDay, int month, int week, int day, DayOfWeek dayOfWeek, bool isFixedDateRule) : m_timeOfDay(timeOfDay), m_month(month), m_week(week), m_day(day), m_dayOfWeek(dayOfWeek), m_isFixedDateRule(isFixedDateRule)
		{
		}

		TransitionTime(const TransitionTime& tt)
		{
			m_timeOfDay = tt.m_timeOfDay;
			m_month = tt.m_month;
			m_week = tt.m_week;
			m_day = tt.m_day;
			m_dayOfWeek = tt.m_dayOfWeek;
			m_isFixedDateRule = tt.m_isFixedDateRule;
		}

		int GetDay() const
		{
			return m_day;
		}

		DayOfWeek GetDayOfWeek() const
		{
			return m_dayOfWeek;
		}

		bool IsFixedDateRule() const
		{
			return m_isFixedDateRule;
		}

		int GetMonth() const
		{
			return m_month;
		}

		DateTime GetTimeOfDay() const
		{
			return m_timeOfDay;
		}

		int GetWeek() const
		{
			return m_week;
		}

		static TransitionTime CreateFixedDateRule(const DateTime& timeOfDay, int month, int day)
		{
			return CreateTransitionTime(timeOfDay, month, 1, day, DayOfWeek::Sunday, true);
		}

		static  TransitionTime CreateFloatingDateRule(const DateTime& timeOfDay, int month, int week, DayOfWeek dayOfWeek)
		{
			return CreateTransitionTime(timeOfDay, month, week, 1, dayOfWeek, false);
		}

		bool operator ==(const TransitionTime& other) const
		{
			bool mDay = (m_isFixedDateRule != other.m_isFixedDateRule || !(m_timeOfDay == other.m_timeOfDay) ? false : m_month == other.m_month);
			if (mDay)
			{
				if (!other.m_isFixedDateRule)
				{
					mDay = (m_week != other.m_week ? false : m_dayOfWeek == other.m_dayOfWeek);
				}
				else
				{
					mDay = m_day == other.m_day;
				}
			}
			return mDay;
		}

		bool operator !=(const TransitionTime& other) const
		{
			bool mDay = (m_isFixedDateRule != other.m_isFixedDateRule || !(m_timeOfDay == other.m_timeOfDay) ? false : m_month == other.m_month);
			if (mDay)
			{
				if (!other.m_isFixedDateRule)
				{
					mDay = (m_week != other.m_week ? false : m_dayOfWeek == other.m_dayOfWeek);
				}
				else
				{
					mDay = m_day == other.m_day;
				}
			}
			return !mDay;
		}
	};

	class AdjustmentRule
	{
	public:
		DateTime m_dateStart;

		DateTime m_dateEnd;

		TimeSpan m_daylightDelta;

		TransitionTime m_daylightTransitionStart;

		TransitionTime m_daylightTransitionEnd;

		static void ValidateAdjustmentRule(const DateTime& dateStart, const DateTime& dateEnd, const TimeSpan& daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd);

		AdjustmentRule();

		AdjustmentRule(const DateTime& dateStart, const DateTime& dateEnd, const TimeSpan& daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd);

		AdjustmentRule(const AdjustmentRule& rule)
		{
			m_dateStart = rule.m_dateStart;
			m_dateEnd = rule.m_dateEnd;
			m_daylightDelta = rule.m_daylightDelta;
			m_daylightTransitionStart = rule.m_daylightTransitionStart;
			m_daylightTransitionEnd = rule.m_daylightTransitionEnd;
		}

		DateTime GetDateEnd() const
		{
			return m_dateEnd;
		}

		DateTime GetDateStart() const
		{
			return m_dateStart;
		}

		TimeSpan GetDaylightDelta() const
		{
			return m_daylightDelta;
		}

		TransitionTime GetDaylightTransitionEnd() const
		{
			return m_daylightTransitionEnd;
		}

		TransitionTime GetDaylightTransitionStart() const
		{
			return m_daylightTransitionStart;
		}

		static AdjustmentRule CreateAdjustmentRule(const DateTime& dateStart, const DateTime& dateEnd, const TimeSpan& daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd)
		{
			ValidateAdjustmentRule(dateStart, dateEnd, daylightDelta, daylightTransitionStart, daylightTransitionEnd);
			return AdjustmentRule(dateStart, dateEnd, daylightDelta, daylightTransitionStart, daylightTransitionEnd);
		}

		bool operator == (const AdjustmentRule& other) const
		{
			bool equals = (m_dateStart == other.m_dateStart
				&& m_dateEnd == other.m_dateEnd
				&& m_daylightDelta == other.m_daylightDelta);

			equals = equals && m_daylightTransitionEnd == other.m_daylightTransitionEnd && m_daylightTransitionStart == other.m_daylightTransitionStart;

			return equals;
		}

		bool operator != (const AdjustmentRule& other) const
		{
			bool equals = (m_dateStart == other.m_dateStart
				&& m_dateEnd == other.m_dateEnd
				&& m_daylightDelta == other.m_daylightDelta);

			equals = equals && m_daylightTransitionEnd == other.m_daylightTransitionEnd && m_daylightTransitionStart == other.m_daylightTransitionStart;

			return !equals;
		}
	};

	class DaylightTime
	{
	public:
		DateTime m_start;

		DateTime m_end;

		TimeSpan m_delta;

		DaylightTime()
		{
		}

		DaylightTime(const DateTime& start, const DateTime& end, const TimeSpan& delta)
		{
			m_start = start;
			m_end = end;
			m_delta = delta;
		}

		DaylightTime(const DaylightTime& time)
		{
			m_start = time.m_start;
			m_end = time.m_end;
			m_delta = time.m_delta;
		}

		TimeSpan GetDelta() const
		{
			return m_delta;
		}

		DateTime GetEnd() const
		{
			return m_end;
		}

		DateTime GetStart() const
		{
			return m_start;
		}

	};

	class TimeZoneInfo_impl
	{
	public:
		std::wstring m_id;

		std::wstring m_displayName;

		std::wstring m_standardDisplayName;

		std::wstring m_daylightDisplayName;

		TimeSpan m_baseUtcOffset;

		bool m_supportsDaylightSavingTime;

		std::vector<AdjustmentRule> m_adjustmentRules;

		static const DateTime maxDateOnly;

		static const DateTime minDateOnly;

		static std::vector<TimeZoneInfo_impl*> StaticGetAllTimeZoneInfos();

		static std::vector<TimeZoneInfo> GetAllTimeZoneInfos();

		static bool GetIsInvalidTime(const DateTime& time, const AdjustmentRule& rule, const DaylightTime& daylightTime);

		bool IsInvalidTime(const DateTime& dateTime) const;

		static bool UtcOffsetOutOfRange(const TimeSpan& offset);

		static void ValidateTimeZoneInfo(const std::wstring& id, const TimeSpan& baseUtcOffset, const std::vector<AdjustmentRule>& adjustmentRules, bool& adjustmentRulesSupportDst);

		TimeZoneInfo_impl(const std::wstring& id, TimeSpan baseUtcOffset, const std::wstring& displayName, const std::wstring& standardDisplayName, const std::wstring& daylightDisplayName, const std::vector<AdjustmentRule>& adjustmentRules, bool disableDaylightSavingTime);

		static bool CheckIsDst(const DateTime& startTime, DateTime time, DateTime endTime);

		static DateTime ConvertTime(const DateTime& dateTime, const TimeZoneInfo_impl* destinationTimeZone);

		static DateTime ConvertTimeToUtc(const DateTime&  dateTime);

		static System::DayOfWeek GetDayOfWeek(const DateTime& time);

		static DateTime TransitionTimeToDateTime(int year, const TransitionTime& transitionTime);

		static DateTimeKind GetCorrespondingKind(const TimeZoneInfo_impl* timeZone);

		static DaylightTime GetDaylightTime(int year, const AdjustmentRule& rule);

		bool GetAdjustmentRuleForTime(const DateTime& dateTime, AdjustmentRule& rule) const;

		static bool GetIsAmbiguousTime(const DateTime& time, const AdjustmentRule& rule, const DaylightTime& daylightTime);

		static bool GetIsDaylightSavings(DateTime time, const AdjustmentRule& rule, const DaylightTime& daylightTime);

		bool IsAmbiguousTime(const DateTime& dateTime) const;

		bool IsDaylightSavingTime(const DateTime& dateTime) const;

		bool HasSameRules(const TimeZoneInfo_impl* other) const;

		static bool GetIsDaylightSavingsFromUtc(const DateTime& time, int Year, const TimeSpan& utc, const AdjustmentRule& rule, bool& isAmbiguousLocalDst);

		static TimeSpan GetUtcOffsetFromUtc(const DateTime time, const TimeZoneInfo_impl* zone, bool& isDaylightSavings, bool& isAmbiguousLocalDst);

		static TimeSpan GetUtcOffsetFromUtc(const DateTime& time, const TimeZoneInfo_impl* zone);

		static TimeSpan GetUtcOffsetFromUtc(const DateTime& time, const TimeZoneInfo_impl* zone, bool& isDaylightSavings);

		static DateTime ConvertUtcToTimeZone(Int64 ticks, const TimeZoneInfo_impl* destinationTimeZone, bool& isAmbiguousLocalDst);

		static DateTime ConvertTime(const DateTime& dateTime, const  TimeZoneInfo_impl* sourceTimeZone, const TimeZoneInfo_impl* destinationTimeZone);

		static TimeSpan GetUtcOffset(const DateTime& time, const TimeZoneInfo_impl* zone);

	public:
		static std::vector<TimeZoneInfo_impl*> Caches;

		static TimeZoneInfo_impl* Local();

		static TimeZoneInfo_impl* Utc();

		TimeSpan GetUtcOffset(const DateTime& dateTime) const;

		static TimeSpan GetLocalUtcOffset(const DateTime& dateTime);
	};
}