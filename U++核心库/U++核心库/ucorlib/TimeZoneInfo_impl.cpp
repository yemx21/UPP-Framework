#include "TimeZoneInfo_impl.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;

namespace System
{
	struct TimeZoneInfoComparer
	{
	protected:
		wchar_t name[32];
		bool Id;
	public:
		TimeZoneInfoComparer(wchar_t sname[32], bool isId = false)
		{
			wcscpy_s(name, sname);
			Id = isId;
		}

		bool operator ()(TimeZoneInfo_impl* const& obj) const
		{
			return Id ? wcscmp(obj->m_id.c_str(), name) == 0 : wcscmp(obj->m_standardDisplayName.c_str(), name) == 0;
		}
	};
}

const DateTime TimeZoneInfo_impl::maxDateOnly = DateTime(9999, 12, 31);
const DateTime TimeZoneInfo_impl::minDateOnly = DateTime(1, 1, 2);

__inline bool TransitionTimeFromTimeZoneInformation(const API_TIME_ZONE_INFORMATION& timeZoneInformation, TransitionTime& transitionTime, bool readStartDate)
{
	if (timeZoneInformation.StandardDate.wMonth == 0)
	{
		transitionTime = TransitionTime();
		return false;
	}
	if (readStartDate)
	{
		if (timeZoneInformation.DaylightDate.wYear != 0)
		{
			transitionTime = TransitionTime::CreateFixedDateRule(DateTime(1, 1, 1, timeZoneInformation.DaylightDate.wHour, timeZoneInformation.DaylightDate.wMinute, timeZoneInformation.DaylightDate.wSecond, timeZoneInformation.DaylightDate.wMilliseconds), timeZoneInformation.DaylightDate.wMonth, timeZoneInformation.DaylightDate.wDay);
		}
		else
		{
			transitionTime = TransitionTime::CreateFloatingDateRule(DateTime(1, 1, 1, timeZoneInformation.DaylightDate.wHour, timeZoneInformation.DaylightDate.wMinute, timeZoneInformation.DaylightDate.wSecond, timeZoneInformation.DaylightDate.wMilliseconds), timeZoneInformation.DaylightDate.wMonth, timeZoneInformation.DaylightDate.wDay, (DayOfWeek)timeZoneInformation.DaylightDate.wDayOfWeek);
		}
	}
	else if (timeZoneInformation.StandardDate.wYear != 0)
	{
		transitionTime = TransitionTime::CreateFixedDateRule(DateTime(1, 1, 1, timeZoneInformation.StandardDate.wHour, timeZoneInformation.StandardDate.wMinute, timeZoneInformation.StandardDate.wSecond, timeZoneInformation.StandardDate.wMilliseconds), timeZoneInformation.StandardDate.wMonth, timeZoneInformation.StandardDate.wDay);
	}
	else
	{
		transitionTime = TransitionTime::CreateFloatingDateRule(DateTime(1, 1, 1, timeZoneInformation.StandardDate.wHour, timeZoneInformation.StandardDate.wMinute, timeZoneInformation.StandardDate.wSecond, timeZoneInformation.StandardDate.wMilliseconds), timeZoneInformation.StandardDate.wMonth, timeZoneInformation.StandardDate.wDay, (DayOfWeek)timeZoneInformation.StandardDate.wDayOfWeek);
	}
	return true;
}

__inline bool CreateAdjustmentRuleFromTimeZoneInformation(const API_TIME_ZONE_INFORMATION& timeZoneInformation, const DateTime& startDate, const DateTime& endDate, AdjustmentRule& rule)
{
	TransitionTime transitionTime;
	TransitionTime transitionTime1;
	if (timeZoneInformation.StandardDate.wMonth == 0)
	{
		return false;
	}
	if (!TransitionTimeFromTimeZoneInformation(timeZoneInformation, transitionTime, true))
	{
		return  false;
	}
	if (!TransitionTimeFromTimeZoneInformation(timeZoneInformation, transitionTime1, false))
	{
		return  false;
	}
	if (transitionTime == transitionTime1)
	{
		return false;
	}
	rule = AdjustmentRule::CreateAdjustmentRule(startDate, endDate, TimeSpan(0, -timeZoneInformation.DaylightBias, 0), transitionTime, transitionTime1);
	return true;
}

__inline bool CheckDaylightSavingTimeNotSupported(const API_TIME_ZONE_INFORMATION& timeZone)
{
	if (timeZone.DaylightDate.wYear != timeZone.StandardDate.wYear || timeZone.DaylightDate.wMonth != timeZone.StandardDate.wMonth || timeZone.DaylightDate.wDayOfWeek != timeZone.StandardDate.wDayOfWeek || timeZone.DaylightDate.wDay != timeZone.StandardDate.wDay || timeZone.DaylightDate.wHour != timeZone.StandardDate.wHour || timeZone.DaylightDate.wMinute != timeZone.StandardDate.wMinute || timeZone.DaylightDate.wSecond != timeZone.StandardDate.wSecond)
	{
		return false;
	}
	return timeZone.DaylightDate.wMilliseconds == timeZone.StandardDate.wMilliseconds;
}

typedef struct tagTZREG
{
	API_INT32 Bias;
	API_INT32 StandardBias;
	API_INT32 DaylightBias;
	API_SYSTEMTIME StandardDate;
	API_SYSTEMTIME DaylightDate;
} TZREG;

std::vector<TimeZoneInfo_impl*> TimeZoneInfo_impl::StaticGetAllTimeZoneInfos()
{
	std::vector<TimeZoneInfo_impl*> result;
	API_PTR hKey;

	if (Microsoft::winapi_RegOpenKey(API_HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones", 0, API_KEY_READ, &hKey) == API_ERROR_SUCCESS)
	{
		API_UINT32 dwKeyCount = 0;
		if (Microsoft::winapi_RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwKeyCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == API_ERROR_SUCCESS)
		{
			API_TIME_ZONE_INFORMATION* pTZI = new API_TIME_ZONE_INFORMATION[dwKeyCount];
			for (auto iIndex = 0; iIndex < dwKeyCount; ++iIndex)
			{
				API_UINT32 dwLen = _countof(pTZI[0].StandardName);
				Microsoft::winapi_RegEnumKey(hKey, iIndex, pTZI[iIndex].StandardName, &dwLen, NULL, NULL, NULL, NULL);
			}

			TZREG tzr;
			for (auto i = 0; i < dwKeyCount; ++i)
			{
				API_PTR hKeyItem;
				if (Microsoft::winapi_RegOpenKey(hKey, pTZI[i].StandardName, 0, API_KEY_READ, &hKeyItem) == API_ERROR_SUCCESS)
				{
					API_UINT32 dwLen = sizeof(pTZI[0].DaylightName);
					API_UINT32 dwType = API_REG_SZ;
					Microsoft::winapi_RegQueryValue(hKeyItem, L"Dlt", NULL, &dwType, (API_BYTE*)pTZI[i].DaylightName, &dwLen);
					dwLen = sizeof(TZREG);
					dwType = API_REG_BINARY;
					Microsoft::winapi_RegQueryValue(hKeyItem, L"TZI", NULL, &dwType, (API_BYTE*)&tzr, &dwLen);

					wchar_t stdname[32];
					Microsoft::winapi_RegQueryValue(hKeyItem, L"Std", NULL, &dwType, (API_BYTE*)&stdname, &dwLen);

					wchar_t disname[32];
					Microsoft::winapi_RegQueryValue(hKeyItem, L"Display", NULL, &dwType, (API_BYTE*)&disname, &dwLen);

					pTZI[i].Bias = tzr.Bias;
					pTZI[i].DaylightBias = tzr.DaylightBias;
					pTZI[i].DaylightDate = tzr.DaylightDate;
					pTZI[i].StandardBias = tzr.StandardBias;
					pTZI[i].StandardDate = tzr.StandardDate;

					AdjustmentRule rules;
					CreateAdjustmentRuleFromTimeZoneInformation(pTZI[i], minDateOnly, maxDateOnly, rules);

					TimeZoneInfo_impl* info = new TimeZoneInfo_impl(pTZI[i].StandardName, TimeSpan(-pTZI[i].Bias * TimeSpan::MINUTES), disname, stdname, pTZI[i].DaylightName, std::vector < AdjustmentRule > {rules}, false);

					result.push_back(info);

					Microsoft::winapi_RegCloseKey(hKeyItem);
				}
			}

			SAFE_DELETE(pTZI);
		}
		Microsoft::winapi_RegCloseKey(hKey);
	}
	return result;
}
std::vector<TimeZoneInfo_impl*> TimeZoneInfo_impl::Caches = StaticGetAllTimeZoneInfos();

std::vector<TimeZoneInfo> TimeZoneInfo_impl::GetAllTimeZoneInfos()
{
	std::vector<TimeZoneInfo> result;
	size_t s = Caches.size();
	result.reserve(s);
	for (size_t i = 0; i < s; i++)
	{
		TimeZoneInfo tmp = TimeZoneInfo();
		tmp.impl = Caches[i];
		result.push_back(tmp);
	}
	return result;
}

bool TimeZoneInfo_impl::GetIsInvalidTime(const DateTime& time, const AdjustmentRule& rule, const DaylightTime& daylightTime)
{
	DateTime start;
	DateTime end;
	DateTime dateTime;
	DateTime dateTime1;
	bool flag = false;
	if (rule.m_daylightDelta.Ticks==0LL)
	{
		return flag;
	}
	if (rule.m_daylightDelta.Ticks>=0LL )
	{
		start = daylightTime.GetStart();
		end = daylightTime.GetStart() + rule.m_daylightDelta;
	}
	else
	{
		start = daylightTime.GetEnd();
		end = daylightTime.GetEnd() - rule.m_daylightDelta;
	}
	flag = (time < start ? false : time < end);
	if (!flag && start.Year != end.Year)
	{
		try
		{
			dateTime = start.AddYears(1);
			dateTime1 = end.AddYears(1);
			flag = (time < dateTime ? false : time < dateTime1);
		}
		catch (...)
		{
		}
		if (!flag)
		{
			try
			{
				dateTime = start.AddYears(-1);
				dateTime1 = end.AddYears(-1);
				flag = (time < dateTime ? false : time < dateTime1);
			}
			catch (...)
			{
			}
		}
	}
	return flag;
}

bool TimeZoneInfo_impl::IsInvalidTime(const DateTime& dateTime) const
{
	bool isInvalidTime = false;
	if (dateTime.Kind == DateTimeKind::Unspecified || dateTime.Kind == DateTimeKind::Local && GetCorrespondingKind(this) == DateTimeKind::Local)
	{
		AdjustmentRule adjustmentRuleForTime;
		if (GetAdjustmentRuleForTime(dateTime, adjustmentRuleForTime))
		{
			isInvalidTime = false;
		}
		else
		{
			DaylightTime daylightTime = GetDaylightTime(dateTime.Year, adjustmentRuleForTime);
			isInvalidTime = GetIsInvalidTime(dateTime, adjustmentRuleForTime, daylightTime);
		}
	}
	return isInvalidTime;
}

bool TimeZoneInfo_impl::UtcOffsetOutOfRange(const TimeSpan& offset)
{
	if (offset.TotalHours < -14)
	{
		return true;
	}
	return offset.TotalHours > 14;
}

void TimeZoneInfo_impl::ValidateTimeZoneInfo(const std::wstring& id, const TimeSpan& baseUtcOffset, const std::vector<AdjustmentRule>& adjustmentRules, bool& adjustmentRulesSupportDst)
{
	if (id.empty())
	{
		throw std::exception("id");
	}
	if (UtcOffsetOutOfRange(baseUtcOffset))
	{
		throw std::exception("ArgumentOutOfRange_UtcOffset");
	}
	if (baseUtcOffset.Ticks % 600000000LL != 0LL)
	{
		throw std::exception("Argument_TimeSpanHasSeconds");
	}
	adjustmentRulesSupportDst = false;
	if (adjustmentRules.size() != 0)
	{
		adjustmentRulesSupportDst = true;
		AdjustmentRule adjustmentRule;
		bool first = false;
		for (int i = 0; i < (int)adjustmentRules.size(); i++)
		{
			adjustmentRule = adjustmentRules[i];
			if (UtcOffsetOutOfRange(baseUtcOffset + adjustmentRule.m_daylightDelta))
			{
				throw std::exception("ArgumentOutOfRange_UtcOffsetAndDaylightDelta");
			}
			if (first && adjustmentRule.m_dateStart <= adjustmentRule.m_dateEnd)
			{
				throw std::exception("Argument_AdjustmentRulesOutOfOrder");
			}
			first = true;
		}
	}
}

TimeZoneInfo_impl::TimeZoneInfo_impl(const std::wstring& id, TimeSpan baseUtcOffset, const std::wstring& displayName, const std::wstring& standardDisplayName, const std::wstring& daylightDisplayName, const std::vector<AdjustmentRule>& adjustmentRules, bool disableDaylightSavingTime)
{
	bool flag;
	ValidateTimeZoneInfo(id, baseUtcOffset, adjustmentRules, flag);
	if (!disableDaylightSavingTime && adjustmentRules.size()>0)
	{
		m_adjustmentRules = adjustmentRules;
	}
	m_id = id;
	m_baseUtcOffset = baseUtcOffset;
	m_displayName = displayName;
	m_standardDisplayName = standardDisplayName;
	m_daylightDisplayName = daylightDisplayName;
	m_supportsDaylightSavingTime = (!flag ? false : !disableDaylightSavingTime);
}


bool TimeZoneInfo_impl::CheckIsDst(const DateTime& startTime, DateTime time, DateTime endTime)
{
	bool flag;
	int year = startTime.Year;
	int num = endTime.Year;
	if (year != num)
	{
		endTime = endTime.AddYears(year - num);
	}
	int year1 = time.Year;
	if (year != year1)
	{
		time = time.AddYears(year - year1);
	}
	if (startTime <= endTime)
	{
		flag = (time < startTime ? false : time < endTime);
	}
	else
	{
		flag = (time < endTime ? true : time >= startTime);
	}
	return flag;
}

DateTime TimeZoneInfo_impl::ConvertTime(const DateTime& dateTime, const TimeZoneInfo_impl* destinationTimeZone)
{
	if (destinationTimeZone == nullptr)
	{
		throw std::exception("destinationTimeZone");
	}
	if (dateTime.Kind == DateTimeKind::Utc)
	{
		return ConvertTime(dateTime, Utc(), destinationTimeZone);
	}
	return ConvertTime(dateTime, Local(), destinationTimeZone);
}

DateTime TimeZoneInfo_impl::ConvertTimeToUtc(const DateTime&  dateTime)
{
	if (dateTime.Kind == DateTimeKind::Utc)
	{
		return dateTime;
	}
	return ConvertTime(dateTime, Local(), Utc());
}

System::DayOfWeek TimeZoneInfo_impl::GetDayOfWeek(const DateTime& time)
{
	return (System::DayOfWeek)((time.Ticks / TimeSpan::DAYS + 1LL) % 7LL);
}

DateTime TimeZoneInfo_impl::TransitionTimeToDateTime(int year, const TransitionTime& transitionTime)
{
	DateTime dateTime;
	DateTime timeOfDay = transitionTime.m_timeOfDay;
	if (transitionTime.IsFixedDateRule())
	{
		int num = DateTime::DaysInMonth(year, transitionTime.m_month);
		dateTime = DateTime(year, transitionTime.m_month, (num < transitionTime.m_day ? num : transitionTime.m_day), timeOfDay.Hour, timeOfDay.Minute, timeOfDay.Second, timeOfDay.Millisecond);
	}
	else if (transitionTime.m_week > 4)
	{
		int num1 = DateTime::DaysInMonth(year, transitionTime.m_month);
		dateTime = DateTime(year, transitionTime.m_month, num1, timeOfDay.Hour, timeOfDay.Minute, timeOfDay.Second, timeOfDay.Millisecond);
		int dayOfWeek = (int)GetDayOfWeek(dateTime) - (int)transitionTime.m_dayOfWeek;
		if (dayOfWeek < 0)
		{
			dayOfWeek = dayOfWeek + 7;
		}
		if (dayOfWeek > 0)
		{
			dateTime = dateTime.AddDays((double)(-dayOfWeek));
		}
	}
	else
	{
		dateTime = DateTime(year, transitionTime.m_month, 1, timeOfDay.Hour, timeOfDay.Minute, timeOfDay.Second, timeOfDay.Millisecond);
		int dayOfWeek1 = GetDayOfWeek(dateTime);
		int week = (int)transitionTime.m_dayOfWeek - dayOfWeek1;
		if (week < 0)
		{
			week = week + 7;
		}
		week = week + 7 * (transitionTime.m_week - 1);
		if (week > 0)
		{
			dateTime = dateTime.AddDays((double)week);
		}
	}
	return dateTime;
}

DateTimeKind TimeZoneInfo_impl::GetCorrespondingKind(const TimeZoneInfo_impl* timeZone)
{
	DateTimeKind dateTimeKind;
	if (timeZone != Utc())
	{
		dateTimeKind = (timeZone != Local() ? DateTimeKind::Unspecified : DateTimeKind::Local);
	}
	else
	{
		dateTimeKind = DateTimeKind::Utc;
	}
	return dateTimeKind;
}

DaylightTime TimeZoneInfo_impl::GetDaylightTime(int year, const AdjustmentRule& rule)
{
	TimeSpan daylightDelta = rule.m_daylightDelta;
	DateTime dateTime = TransitionTimeToDateTime(year, rule.m_daylightTransitionStart);
	DateTime dateTime1 = TransitionTimeToDateTime(year, rule.m_daylightTransitionEnd);
	return DaylightTime(dateTime, dateTime1, daylightDelta);
}

bool TimeZoneInfo_impl::GetAdjustmentRuleForTime(const DateTime& dateTime, AdjustmentRule& rule) const
{
	if (m_adjustmentRules.size() == 0)
	{
		return false;
	}
	DateTime date = dateTime.Date;
	for (int i = 0; i < (int)m_adjustmentRules.size(); i++)
	{
		if (m_adjustmentRules[i].m_dateStart <= date && m_adjustmentRules[i].m_dateEnd >= date)
		{
			m_adjustmentRules[i];
			return true;
		}
	}
	return false;
}

bool TimeZoneInfo_impl::GetIsAmbiguousTime(const DateTime& time, const AdjustmentRule& rule, const DaylightTime& daylightTime)
{
	DateTime start;
	DateTime end;
	DateTime dateTime;
	bool flag = false;
	if (rule.m_daylightDelta.Ticks == 0LL)
	{
		return flag;
	}
	if (rule.m_daylightDelta.Ticks <= 0LL)
	{
		start = daylightTime.GetStart();
		end = daylightTime.GetStart() + rule.m_daylightDelta;
	}
	else
	{
		start = daylightTime.GetEnd();
		end = daylightTime.GetEnd() - rule.m_daylightDelta;
	}
	flag = (time < end ? false : time < start);
	if (!flag && start.Year != end.Year)
	{
		try
		{
			dateTime = start.AddYears(1);
			flag = (time < end.AddYears(1) ? false : time < dateTime);
		}
		catch (...)
		{
		}
		if (!flag)
		{
			try
			{
				dateTime = start.AddYears(-1);
				flag = (time < end.AddYears(-1) ? false : time < dateTime);
			}
			catch (...)
			{
			}
		}
	}
	return flag;
}

bool TimeZoneInfo_impl::GetIsDaylightSavings(DateTime time, const AdjustmentRule& rule, const DaylightTime& daylightTime)
{
	DateTime start;
	DateTime end;
	if (time.Kind != DateTimeKind::Local)
	{
		bool daylightDelta = rule.m_daylightDelta.Ticks > 0;
		start = daylightTime.GetStart() + (daylightDelta ? rule.m_daylightDelta : TimeSpan());
		end = daylightTime.GetEnd() + (daylightDelta ? -rule.m_daylightDelta : TimeSpan());
	}
	else
	{
		start = daylightTime.GetStart() + daylightTime.GetDelta();
		end = daylightTime.GetEnd();
	}
	bool flag = CheckIsDst(start, time, end);
	if (flag && time.Kind == DateTimeKind::Local && GetIsAmbiguousTime(time, rule, daylightTime))
	{
		flag = time.IsAmbiguousDaylightSavingTime;
	}
	return flag;
}

bool TimeZoneInfo_impl::IsAmbiguousTime(const DateTime& dateTime) const
{
	DateTime dateTime1;
	if (!m_supportsDaylightSavingTime)
	{
		return false;
	}
	if (dateTime.Kind == DateTimeKind::Local)
	{
		dateTime1 = ConvertTime(dateTime, Local(), this);
	}
	else if (dateTime.Kind != DateTimeKind::Utc)
	{
		dateTime1 = dateTime;
	}
	else
	{
		dateTime1 = ConvertTime(dateTime, Utc(), this);
	}
	AdjustmentRule adjustmentRuleForTime;
	if (GetAdjustmentRuleForTime(dateTime1, adjustmentRuleForTime))
	{
		return false;
	}
	DaylightTime daylightTime = GetDaylightTime(dateTime1.Year, adjustmentRuleForTime);
	return GetIsAmbiguousTime(dateTime1, adjustmentRuleForTime, daylightTime);
}

bool TimeZoneInfo_impl::HasSameRules(const TimeZoneInfo_impl* other) const
{
	bool flag;
	if (other == nullptr)
	{
		return false;
	}
	if (m_baseUtcOffset != other->m_baseUtcOffset || m_supportsDaylightSavingTime != other->m_supportsDaylightSavingTime)
	{
		return false;
	}
	std::vector<AdjustmentRule> mAdjustmentRules = m_adjustmentRules;
	std::vector<AdjustmentRule> adjustmentRuleArray = other->m_adjustmentRules;
	if (mAdjustmentRules.size() != 0 || adjustmentRuleArray.size() != 0)
	{
		flag = (mAdjustmentRules.size()==0 ? false : adjustmentRuleArray.size() != 0);
	}
	else
	{
		flag = true;
	}
	bool flag1 = flag;
	if (!flag1)
	{
		return false;
	}
	if (mAdjustmentRules.size() != 0)
	{
		if ((int)mAdjustmentRules.size() != (int)adjustmentRuleArray.size())
		{
			return false;
		}
		for (int i = 0; i < (int)mAdjustmentRules.size(); i++)
		{
			if (mAdjustmentRules[i] !=adjustmentRuleArray[i])
			{
				return false;
			}
		}
	}
	return flag1;
}

bool TimeZoneInfo_impl::IsDaylightSavingTime(const DateTime& dateTime) const
{
	DateTime dateTime1;
	bool flag;
	if (!m_supportsDaylightSavingTime || m_adjustmentRules.size()==0)
	{
		return false;
	}
	if (dateTime.Kind != DateTimeKind::Local)
	{
		if (dateTime.Kind == DateTimeKind::Utc)
		{
			if (GetCorrespondingKind(this) == DateTimeKind::Utc)
			{
				return false;
			}
			GetUtcOffsetFromUtc(dateTime, this, flag);
			return flag;
		}
		dateTime1 = dateTime;
	}
	else
	{
		dateTime1 = ConvertTime(dateTime, Local(), this);
	}

	AdjustmentRule adjustmentRuleForTime;
	if (GetAdjustmentRuleForTime(dateTime1, adjustmentRuleForTime))
	{
		return false;
	}
	DaylightTime daylightTime = GetDaylightTime(dateTime1.Year, adjustmentRuleForTime);
	return GetIsDaylightSavings(dateTime1, adjustmentRuleForTime, daylightTime);
}

bool TimeZoneInfo_impl::GetIsDaylightSavingsFromUtc(const DateTime& time, int Year, const TimeSpan& utc, const AdjustmentRule& rule, bool& isAmbiguousLocalDst)
{
	DateTime delta;
	DateTime dateTime;
	isAmbiguousLocalDst = false;
	TimeSpan timeSpan = utc;
	DaylightTime daylightTime = GetDaylightTime(Year, rule);
	DateTime start = daylightTime.GetStart() - timeSpan;
	DateTime end = (daylightTime.GetEnd() - timeSpan) - rule.m_daylightDelta;
	if (daylightTime.GetDelta().Ticks <= (long)0)
	{
		delta = start;
		dateTime = start - daylightTime.GetDelta();
	}
	else
	{
		delta = end - daylightTime.GetDelta();
		dateTime = end;
	}
	bool flag = CheckIsDst(start, time, end);
	if (flag)
	{
		isAmbiguousLocalDst = (time < delta ? false : time < dateTime);
		if (!isAmbiguousLocalDst && delta.Year != dateTime.Year)
		{
			try
			{
				delta.AddYears(1);
				dateTime.AddYears(1);
				isAmbiguousLocalDst = (time < delta ? false : time < dateTime);
			}
			catch (...)
			{
			}
			if (!isAmbiguousLocalDst)
			{
				try
				{
					delta.AddYears(-1);
					dateTime.AddYears(-1);
					isAmbiguousLocalDst = (time < delta ? false : time < dateTime);
				}
				catch (...)
				{
				}
			}
		}
	}
	return flag;
}

TimeSpan TimeZoneInfo_impl::GetUtcOffsetFromUtc(const DateTime time, const TimeZoneInfo_impl* zone, bool& isDaylightSavings, bool& isAmbiguousLocalDst)
{
	int year;
	AdjustmentRule adjustmentRuleForTime;
	isDaylightSavings = false;
	isAmbiguousLocalDst = false;
	TimeSpan baseUtcOffset = zone->m_baseUtcOffset;
	bool success = false;
	if (time > maxDateOnly)
	{
		success = zone->GetAdjustmentRuleForTime(DateTime::MaxValue, adjustmentRuleForTime);
		year = 9999;
	}
	else if (time >= minDateOnly)
	{
		DateTime dateTime = time + baseUtcOffset;
		year = time.Year;
		success = zone->GetAdjustmentRuleForTime(dateTime, adjustmentRuleForTime);
	}
	else
	{
		success = zone->GetAdjustmentRuleForTime(DateTime::MinValue, adjustmentRuleForTime);
		year = 1;
	}
	if (success)
	{
		isDaylightSavings = GetIsDaylightSavingsFromUtc(time, year, zone->m_baseUtcOffset, adjustmentRuleForTime, isAmbiguousLocalDst);
		baseUtcOffset = baseUtcOffset + (isDaylightSavings ? adjustmentRuleForTime.m_daylightDelta : TimeSpan());
	}
	return baseUtcOffset;
}

TimeSpan TimeZoneInfo_impl::GetUtcOffsetFromUtc(const DateTime& time, const TimeZoneInfo_impl* zone)
{
	bool flag;
	return GetUtcOffsetFromUtc(time, zone, flag);
}

TimeSpan TimeZoneInfo_impl::GetUtcOffsetFromUtc(const DateTime& time, const TimeZoneInfo_impl* zone, bool& isDaylightSavings)
{
	bool flag;
	return GetUtcOffsetFromUtc(time, zone, isDaylightSavings, flag);
}

DateTime TimeZoneInfo_impl::ConvertUtcToTimeZone(Int64 ticks, const TimeZoneInfo_impl* destinationTimeZone, bool& isAmbiguousLocalDst)
{
	DateTime maxValue;
	DateTime dateTime;
	if (ticks <= DateTime::MaxValue.Ticks)
	{
		maxValue = (ticks >= DateTime::MinValue.Ticks ? DateTime(ticks) : DateTime::MinValue);
	}
	else
	{
		maxValue = DateTime::MaxValue;
	}
	TimeSpan utcOffsetFromUtc = GetUtcOffsetFromUtc(maxValue, destinationTimeZone, isAmbiguousLocalDst);
	ticks = ticks + utcOffsetFromUtc.Ticks;
	if (ticks <= DateTime::MaxValue.Ticks)
	{
		dateTime = (ticks >= DateTime::MinValue.Ticks ? DateTime(ticks) : DateTime::MinValue);
	}
	else
	{
		dateTime = DateTime::MaxValue;
	}
	return dateTime;
}

DateTime TimeZoneInfo_impl::ConvertTime(const DateTime& dateTime, const TimeZoneInfo_impl* sourceTimeZone, const TimeZoneInfo_impl* destinationTimeZone)
{
	if (sourceTimeZone == nullptr)
	{
		throw std::exception("sourceTimeZone");
	}
	if (destinationTimeZone == nullptr)
	{
		throw std::exception("destinationTimeZone");
	}
	DateTimeKind correspondingKind = GetCorrespondingKind(sourceTimeZone);

	TimeSpan baseUtcOffset = sourceTimeZone->m_baseUtcOffset;
	AdjustmentRule adjustmentRuleForTime;
	if (sourceTimeZone->GetAdjustmentRuleForTime(dateTime, adjustmentRuleForTime))
	{
		bool isDaylightSavings = false;
		DaylightTime daylightTime = GetDaylightTime(dateTime.Year, adjustmentRuleForTime);

		isDaylightSavings = GetIsDaylightSavings(dateTime, adjustmentRuleForTime, daylightTime);
		baseUtcOffset = baseUtcOffset + (isDaylightSavings ? adjustmentRuleForTime.m_daylightDelta : TimeSpan());
	}

	DateTimeKind dateTimeKind = GetCorrespondingKind(destinationTimeZone);
	if (dateTime.Kind != DateTimeKind::Unspecified && correspondingKind != DateTimeKind::Unspecified && correspondingKind == dateTimeKind)
	{
		return dateTime;
	}
	long long ticks = dateTime.Ticks - baseUtcOffset.Ticks;
	bool flag = false;
	DateTime timeZone = ConvertUtcToTimeZone(ticks, destinationTimeZone, flag);
	if (dateTimeKind != DateTimeKind::Local)
	{
		return DateTime(timeZone.Ticks, dateTimeKind);
	}
	return DateTime(timeZone.Ticks, (bool)DateTimeKind::Local);
}

TimeSpan TimeZoneInfo_impl::GetUtcOffset(const DateTime& time, const TimeZoneInfo_impl* zone)
{
	TimeSpan baseUtcOffset = zone->m_baseUtcOffset;
	AdjustmentRule adjustmentRuleForTime;
	if (zone->GetAdjustmentRuleForTime(time, adjustmentRuleForTime))
	{
		DaylightTime daylightTime = GetDaylightTime(time.Year, adjustmentRuleForTime);
		bool isDaylightSavings = GetIsDaylightSavings(time, adjustmentRuleForTime, daylightTime);
		baseUtcOffset = baseUtcOffset + (isDaylightSavings ? adjustmentRuleForTime.m_daylightDelta : TimeSpan());
	}
	return baseUtcOffset;
}

TimeZoneInfo_impl* TimeZoneInfo_impl::Local()
{
	API_TIME_ZONE_INFORMATION current;
	memset(&current, 0, sizeof(API_TIME_ZONE_INFORMATION));
	Microsoft::winapi_GetTimeZoneInformation(current);

	auto Citer = find_if(Caches.begin(), Caches.end(), TimeZoneInfoComparer(current.StandardName));
	if (Citer != Caches.end())
	{
		TimeZoneInfo_impl* tmp = *Citer;
		return tmp;
	}
	else
		return NULL;
}

TimeZoneInfo_impl* TimeZoneInfo_impl::Utc()
{
	auto Citer = find_if(Caches.begin(), Caches.end(), TimeZoneInfoComparer(L"UTC", true));
	if (Citer != Caches.end())
	{
		TimeZoneInfo_impl* tmp = *Citer;
		return tmp;
	}
	else
		return NULL;
}

TimeSpan TimeZoneInfo_impl::GetUtcOffset(const DateTime& dateTime) const
{
	if (dateTime.Kind == DateTimeKind::Local)
	{
		if (GetCorrespondingKind(this) != DateTimeKind::Local)
		{
			DateTime dateTime1 = ConvertTime(dateTime, Local(), Utc());
			return GetUtcOffsetFromUtc(dateTime1, this);
		}
	}
	else if (dateTime.Kind == DateTimeKind::Utc)
	{
		if (GetCorrespondingKind(this) == DateTimeKind::Utc)
		{
			return m_baseUtcOffset;
		}
		return GetUtcOffsetFromUtc(dateTime, this);
	}
	return GetUtcOffset(dateTime, this);
}

TimeSpan TimeZoneInfo_impl::GetLocalUtcOffset(const DateTime& dateTime)
{
	return Local()->GetUtcOffset(dateTime);
}