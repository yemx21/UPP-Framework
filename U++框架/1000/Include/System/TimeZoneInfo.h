#pragma once
#include "TimeSpan.h"
#include "DateTime.h"
namespace System
{
	class TimeZoneInfo_impl;

	/// <summary>
	/// 时区信息类
	/// </summary>
	class RUNTIME_API TimeZoneInfo final: __uobject
	{
	private:
		friend class TimeZoneInfo_impl;

		TimeZoneInfo_impl* impl;

		TimeZoneInfo();

	public:

		TimeZoneInfo(const TimeZoneInfo& info);

		TimeZoneInfo(TimeZoneInfo&& info);

		/// <summary>
		/// 获取协调时偏移
		/// </summary>
		/// <returns></returns>
		TimeSpan GetBaseUtcOffset() const;

		/// <summary>
		/// 获取夏令时名称
		/// </summary>
		/// <returns></returns>
		std::wstring GetDaylightName() const;

		/// <summary>
		/// 获取显示名称
		/// </summary>
		/// <returns></returns>
		std::wstring GetDisplayName() const;

		/// <summary>
		/// 获取标识
		/// </summary>
		/// <returns></returns>
		std::wstring GetId() const;

		/// <summary>
		/// 获取标准名称
		/// </summary>
		/// <returns></returns>
		std::wstring GetStandardName() const;

		/// <summary>
		/// 获取是否支持夏令时
		/// </summary>
		/// <returns></returns>
		bool GetSupportsDaylightSavingTime() const;

		/// <summary>
		/// 协调时偏移
		/// </summary>
		__uproperty(get = GetBaseUtcOffset) TimeSpan BaseUtcOffset;

		/// <summary>
		/// 夏令时名称
		/// </summary>
		__uproperty(get = GetDaylightName) std::wstring DaylightName;

		/// <summary>
		/// 获取显示名称
		/// </summary>
		/// <returns></returns>
		__uproperty(get = GetDisplayName) std::wstring DisplayName;

		/// <summary>
		/// 获取标识
		/// </summary>
		/// <returns></returns>
		__uproperty(get = GetId) std::wstring Id;

		/// <summary>
		/// 获取标准名称
		/// </summary>
		/// <returns></returns>
		__uproperty(get = GetStandardName) std::wstring StandardName;

		/// <summary>
		/// 获取是否支持夏令时
		/// </summary>
		/// <returns></returns>
		__uproperty(get = GetSupportsDaylightSavingTime) bool SupportsDaylightSavingTime;

		/// <summary>
		/// 获取当前系统时区
		/// </summary>
		/// <returns></returns>
		__uref static TimeZoneInfo Local();

		/// <summary>
		/// 获取世界协调时时区
		/// </summary>
		/// <returns></returns>
		__uref static TimeZoneInfo Utc();

		/// <summary>
		/// 获取系统所有的时区信息
		/// </summary>
		/// <returns></returns>
		__uref static std::vector<TimeZoneInfo> GetSystemTimeZones();

		/// <summary>
		/// 将指定日期时间转换到特定时区
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <param name="destinationTimeZone">特定时区</param>
		/// <returns></returns>
		__uref DateTime ConvertTime(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone);

		/// <summary>
		///  将指定日期时间从源时区转换到目标时区
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <param name="sourceTimeZone">源时区</param>
		/// <param name="destinationTimeZone">目标时区</param>
		/// <returns></returns>
		__uref DateTime ConvertTime(const DateTime& dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);

		/// <summary>
		/// 将协调世界时 (UTC) 转换为指定时区中的时间
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <param name="destinationTimeZone">指定时区</param>
		/// <returns></returns>
		__uref DateTime ConvertTimeFromUtc(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone);

		/// <summary>
		/// 将指定日期时间转换为协调世界时 (UTC)
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <returns></returns>
		__uref DateTime ConvertTimeToUtc(const DateTime& dateTime);

		/// <summary>
		/// 将指定时区中的日期时间转换为协调世界时 (UTC)
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <param name="zone">指定时区</param>
		/// <returns></returns>
		__uref DateTime ConvertTimeToUtc(const DateTime& dateTime, const TimeZoneInfo& zone);

		/// <summary>
		/// 获取该时区中的时间与协调世界时 (UTC) 之间针对特定日期时间的偏移量
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <returns></returns>
		__uref TimeSpan GetUtcOffset(const DateTime& dateTime) const;

		/// <summary>
		/// 判断该时区中的指定日期时间是否处于夏时制范围内
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <returns></returns>
		__uref bool IsDaylightSavingTime(const DateTime& dateTime) const;

		/// <summary>
		/// 判断该时区中的指定日期时间是否不明确以及是否可以映射至两个或多个协调世界时 (UTC) 时间
		/// </summary>
		/// <param name="dateTime">指定日期时间</param>
		/// <returns></returns>
		__uref bool IsAmbiguousTime(const DateTime& dateTime) const;

		/// <summary>
		/// 判断该时区和另一个时区是否具有相同的调整规则
		/// </summary>
		/// <param name="otherZone">另一个时区</param>
		/// <returns></returns>
		__uref bool HasSameRules(const TimeZoneInfo& otherZone) const;

		/// <summary>
		/// 判断指定日期时间是否无效
		/// </summary>
		/// <param name="dateTime"></param>
		/// <returns></returns>
		__uref bool IsInvalidTime(const DateTime& dateTime) const;

		REFLECT_CLASS(TimeZoneInfo)
	};

	class RUNTIME_API TimeZoneInfoConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<TimeZoneInfoConverter> _thisWeakPtr;
	protected:
		TimeZoneInfoConverter();
		TimeZoneInfoConverter(TimeZoneInfoConverter const&);
		void operator=(TimeZoneInfoConverter const&);
	public:
		virtual ~TimeZoneInfoConverter();

		static std::shared_ptr<TimeZoneInfoConverter> GetSharedInstance();

		static TimeZoneInfoConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};
}