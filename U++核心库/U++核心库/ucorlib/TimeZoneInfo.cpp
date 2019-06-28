#include "TimeZoneInfo.h"
#include "TimeZoneInfo_impl.h"
#include "BuiltInAssembly.h"
using namespace System;

void AdjustmentRule::ValidateAdjustmentRule(const DateTime& dateStart, const DateTime& dateEnd, const TimeSpan& daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd)
{
	if (dateStart.Kind != DateTimeKind::Unspecified)
	{
		throw std::exception("Argument_DateTimeKindMustBeUnspecified");
	}
	if (dateEnd.Kind != DateTimeKind::Unspecified)
	{
		throw std::exception("Argument_DateTimeKindMustBeUnspecified");
	}
	if (daylightTransitionStart == daylightTransitionEnd)
	{
		throw std::exception("Argument_TransitionTimesAreIdentical");
	}
	if (dateStart > dateEnd)
	{
		throw std::exception("Argument_OutOfOrderDateTimes");
	}
	if (TimeZoneInfo_impl::UtcOffsetOutOfRange(daylightDelta))
	{
		throw std::exception("ArgumentOutOfRange_UtcOffset");
	}
	if (daylightDelta.Ticks % 600000000LL != 0LL)
	{
		throw std::exception("Argument_TimeSpanHasSeconds");
	}
	if (dateStart.TimeOfDay.Ticks != 0)
	{
		throw std::exception("Argument_DateTimeHasTimeOfDay");
	}
	if (dateEnd.TimeOfDay.Ticks != 0)
	{
		throw std::exception("Argument_DateTimeHasTimeOfDay");
	}
}

AdjustmentRule::AdjustmentRule()
{

}

AdjustmentRule::AdjustmentRule(const DateTime& dateStart, const DateTime& dateEnd, const TimeSpan& daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd) : m_dateStart(dateStart), m_dateEnd(dateEnd), m_daylightDelta(daylightDelta), m_daylightTransitionStart(daylightTransitionStart), m_daylightTransitionEnd(daylightTransitionEnd)
{
}

TimeZoneInfo::TimeZoneInfo() : impl(nullptr)
{

}

TimeZoneInfo::TimeZoneInfo(const TimeZoneInfo& info)
{
	impl = info.impl;
}

TimeZoneInfo::TimeZoneInfo(TimeZoneInfo&& info)
{
	impl = info.impl;
}

TimeSpan TimeZoneInfo::GetBaseUtcOffset() const
{
	return impl ? impl->m_baseUtcOffset : TimeSpan();
}

std::wstring TimeZoneInfo::GetDaylightName() const
{
	return impl ? impl->m_daylightDisplayName : std::wstring();
}

std::wstring TimeZoneInfo::GetDisplayName() const
{
	return impl ? impl->m_displayName : std::wstring();
}

std::wstring TimeZoneInfo::GetId() const
{
	return impl ? impl->m_id : std::wstring();
}

std::wstring TimeZoneInfo::GetStandardName() const
{
	return impl ? impl->m_standardDisplayName : std::wstring();
}

bool TimeZoneInfo::GetSupportsDaylightSavingTime() const
{
	return impl ? impl->m_supportsDaylightSavingTime : false;
}

TimeZoneInfo TimeZoneInfo::Local()
{
	TimeZoneInfo info = TimeZoneInfo();
	info.impl = TimeZoneInfo_impl::Local();
	return info;
}

TimeZoneInfo TimeZoneInfo::Utc()
{
	TimeZoneInfo info = TimeZoneInfo();
	info.impl = TimeZoneInfo_impl::Utc();
	return info;
}

std::vector<TimeZoneInfo> TimeZoneInfo::GetSystemTimeZones()
{
	return TimeZoneInfo_impl::GetAllTimeZoneInfos();
}


DateTime TimeZoneInfo::ConvertTime(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone)
{
	return TimeZoneInfo_impl::ConvertTime(dateTime, destinationTimeZone.impl);
}

DateTime TimeZoneInfo::ConvertTime(const DateTime& dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone)
{
	return TimeZoneInfo_impl::ConvertTime(dateTime, sourceTimeZone.impl, destinationTimeZone.impl);
}

DateTime TimeZoneInfo::ConvertTimeFromUtc(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone)
{
	return TimeZoneInfo_impl::ConvertTime(dateTime, TimeZoneInfo_impl::Utc(), destinationTimeZone.impl);
}

DateTime TimeZoneInfo::ConvertTimeToUtc(const DateTime& dateTime)
{
	return TimeZoneInfo_impl::ConvertTimeToUtc(dateTime);
}

DateTime TimeZoneInfo::ConvertTimeToUtc(const DateTime& dateTime, const TimeZoneInfo& zone)
{
	return TimeZoneInfo_impl::ConvertTime(dateTime, TimeZoneInfo_impl::Utc(), zone.impl);
}

TimeSpan TimeZoneInfo::GetUtcOffset(const DateTime& dateTime) const
{
	return impl->GetUtcOffset(dateTime);
}

bool TimeZoneInfo::IsDaylightSavingTime(const DateTime& dateTime) const
{
	return impl->IsDaylightSavingTime(dateTime);
}

bool TimeZoneInfo::IsAmbiguousTime(const DateTime& dateTime) const
{
	return impl->IsAmbiguousTime(dateTime);
}

bool TimeZoneInfo::HasSameRules(const TimeZoneInfo& otherZone) const
{
	return impl->HasSameRules(otherZone.impl);
}

bool TimeZoneInfo::IsInvalidTime(const DateTime& dateTime) const
{
	return impl->IsInvalidTime(dateTime);
}

std::weak_ptr<TimeZoneInfoConverter> TimeZoneInfoConverter::_thisWeakPtr;
std::mutex& TimeZoneInfoConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

TimeZoneInfoConverter::TimeZoneInfoConverter() {}
TimeZoneInfoConverter::~TimeZoneInfoConverter() {}

std::shared_ptr<TimeZoneInfoConverter> TimeZoneInfoConverter::GetSharedInstance()
{
	std::shared_ptr<TimeZoneInfoConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new TimeZoneInfoConverter());
	_thisWeakPtr = p;
	return p;
}

TimeZoneInfoConverter& TimeZoneInfoConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool TimeZoneInfoConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	return type->IsType(_typeof(TimeZoneInfo));
}

bool TimeZoneInfoConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(TimeZoneInfo))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any TimeZoneInfoConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(TimeZoneInfo)))
		{
			if (type->IsReference)
			{
				return TimeZoneInfo(AnyVisitor<TimeZoneInfo&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			TimeZoneInfo* p = obj;
			if (p)
				return *p;
			else
			{
				TimeZoneInfo res = obj.as<TimeZoneInfo>();
				return res;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any TimeZoneInfoConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
{
	if (!type)return _missing;
	_type otype = obj.Type;
	if (!otype) return _missing;
	try
	{
		bool iswiden = false;
		if (type->GetIsSTDString(iswiden))
		{
			if (iswiden)
			{
				if (otype->IsReference)
				{
					return AnyVisitor<TimeZoneInfo&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				TimeZoneInfo* p = obj;
				if (p)
					return p->ToString();
				else
				{
					TimeZoneInfo v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(TimeZoneInfo)))
		{
			if(otype->IsReference)
			{ 
				return AnyVisitor<TimeZoneInfo&>::Get(obj,ModifierPolicy::IgnoreAll);		
			}
			TimeZoneInfo* p = obj;
			if (p)
				return *p;
			else
			{
				TimeZoneInfo v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any TimeZoneInfoConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	return _missing;
}

std::wstring TimeZoneInfoConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(TimeZoneInfo)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<TimeZoneInfo&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			TimeZoneInfo* p = obj;
			if (p)
				return p->ToString();
			else
			{
				TimeZoneInfo v = obj;
				return v.ToString();
			}
		}
		return std::wstring();
	}
	catch (...)
	{
		return std::wstring();
	}
}

__uregister_class(TimeZoneInfo, ClassFlags::Normal)
{
	type.AddProperty(L"BaseUtcOffset", PropertyFlags::Default, PropertyTypeOf(&TimeZoneInfo::GetBaseUtcOffset));
	type.AddProperty(L"DaylightName", PropertyFlags::Default, PropertyTypeOf(&TimeZoneInfo::GetDaylightName));
	type.AddProperty(L"DisplayName", PropertyFlags::Default, PropertyTypeOf(&TimeZoneInfo::GetDisplayName));
	type.AddProperty(L"Id", PropertyFlags::Default, PropertyTypeOf(&TimeZoneInfo::GetId));
	type.AddProperty(L"StandardName", PropertyFlags::Default, PropertyTypeOf(&TimeZoneInfo::GetStandardName));
	type.AddProperty(L"SupportsDaylightSavingTime", PropertyFlags::Default, PropertyTypeOf(&TimeZoneInfo::GetSupportsDaylightSavingTime));
	type.AddMethod(L"Local", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<0, TimeZoneInfo, TimeZoneInfo>::Create(&TimeZoneInfo::Local));
	type.AddMethod(L"Utc", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<0, TimeZoneInfo, TimeZoneInfo>::Create(&TimeZoneInfo::Utc));
	type.AddMethod(L"GetSystemTimeZones", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<0, TimeZoneInfo, std::vector<TimeZoneInfo>>::Create(&TimeZoneInfo::GetSystemTimeZones));
	type.AddMethod(L"ConvertTime", MethodFlags::Default, MethodTypeAdapter<2, TimeZoneInfo, DateTime>::Create(&TimeZoneInfo::ConvertTime));
	type.AddMethod(L"ConvertTime", MethodFlags::Default, MethodTypeAdapter<3, TimeZoneInfo, DateTime>::Create(&TimeZoneInfo::ConvertTime));
	type.AddMethod(L"ConvertTimeFromUtc", MethodFlags::Default, MethodTypeAdapter<2, TimeZoneInfo, DateTime>::Create(&TimeZoneInfo::ConvertTimeFromUtc));
	type.AddMethod(L"ConvertTimeToUtc", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, DateTime>::Create(&TimeZoneInfo::ConvertTimeToUtc));
	type.AddMethod(L"ConvertTimeToUtc", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, DateTime>::Create(&TimeZoneInfo::ConvertTimeToUtc));
	type.AddMethod(L"GetUtcOffset", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, TimeSpan>::Create(&TimeZoneInfo::GetUtcOffset));
	type.AddMethod(L"IsDaylightSavingTime", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, bool>::Create(&TimeZoneInfo::IsDaylightSavingTime));
	type.AddMethod(L"IsAmbiguousTime", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, bool>::Create(&TimeZoneInfo::IsAmbiguousTime));
	type.AddMethod(L"HasSameRules", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, bool>::Create(&TimeZoneInfo::HasSameRules));
	type.AddMethod(L"IsInvalidTime", MethodFlags::Default, MethodTypeAdapter<1, TimeZoneInfo, bool>::Create(&TimeZoneInfo::IsInvalidTime));

	type.AddAttribute(new TypeConverterAttribute(L"TimeZoneInfoConverter", TimeZoneInfoConverter::GetSharedInstance()));
	BuiltInAssemblyFactory::Regisiter(&type);

};