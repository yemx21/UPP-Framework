#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>



namespace System
{
	class DateTimeFormat; 
	class TimeSpanFormat;
	class TimeSpanParse;
	class TimeSpanConverter;

	/// <summary>
	/// 时间间隔风格枚举
	/// </summary>
	class RUNTIME_API TimeSpanStyles : __uobject
	{
		__uvalue_begin(TimeSpanStyles, UInt32)
		/// <summary>
		/// 无
		/// </summary>
		__uvalue(None)
		/// <summary>
		/// 假设为负
		/// </summary>
		__uvalue(AssumeNegative)
		__uvalue_end(TimeSpanStyles, None)
	};

	/// <summary>
	/// 时间间隔类
	/// </summary>
	class RUNTIME_API TimeSpan final:__uobject
	{
	public:
		/// <summary>
		/// 刻度（天）
		/// </summary>
		static const Int64 DAYS;
		/// <summary>
		/// 刻度（小时）
		/// </summary>
		static const Int64 HOURS;
		/// <summary>
		/// 刻度（分钟）
		/// </summary>
		static const Int64 MINUTES;
		/// <summary>
		/// 刻度（秒）
		/// </summary>
		static const Int64 SECONDS;
		/// <summary>
		/// 刻度（毫秒）
		/// </summary>
		static const Int64 MILLISECONDS;
		/// <summary>
		/// 刻度（微秒）
		/// </summary>
		static const Int64 MICROSECONDS;

	private:
		friend class DateTimeFormat;
		friend class TimeSpanFormat;
		friend class TimeSpanParse;
		friend class DateTime;
		Int64 span;		
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
	public:
		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		__uref TimeSpan();

		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="nano100seconds">以100纳秒为单位的时间值</param>
		__uref TimeSpan(Int64 nano100seconds);

		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="hours">小时数</param>
		/// <param name="minutes">分钟数</param>
		/// <param name="seconds">秒数</param>
		__uref TimeSpan(int hours, int minutes, int seconds);

		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="days">天数</param>
		/// <param name="hours">小时数</param>
		/// <param name="minutes">分钟数</param>
		/// <param name="seconds">秒数</param>
		__uref TimeSpan(int days, int hours, int minutes, int seconds);

		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="days">天数</param>
		/// <param name="hours">小时数</param>
		/// <param name="minutes">分钟数</param>
		/// <param name="seconds">秒数</param>
		/// <param name="milliseconds">毫秒数</param>
		__uref TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);

		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="days">天数</param>
		/// <param name="hours">小时数</param>
		/// <param name="minutes">分钟数</param>
		/// <param name="seconds">秒数</param>
		/// <param name="milliseconds">毫秒数</param>
		/// <param name="microseconds">微秒数</param>
		__uref TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds);

		/// <summary>
		/// 创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="days">天数</param>
		/// <param name="hours">小时数</param>
		/// <param name="minutes">分钟数</param>
		/// <param name="seconds">秒数</param>
		/// <param name="milliseconds">毫秒数</param>
		/// <param name="microseconds">微秒数</param>
		/// <param name="nano100seconds">以100纳秒为单位的时间值</param>
		__uref TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds, int nano100seconds);

		~TimeSpan();

		__uref TimeSpan(const TimeSpan& timeSpan);

		/// <summary>
		/// 获取时间跨度
		/// </summary>
		/// <returns></returns>
		TimeSpan GetDuration() const;
		/// <summary>
		/// 获取当前时间的负值
		/// </summary>
		/// <returns></returns>
		TimeSpan GetNegate() const;
		/// <summary>
		/// 获取天数
		/// </summary>
		/// <returns></returns>
		int GetDays() const;
		/// <summary>
		/// 获取小时数
		/// </summary>
		/// <returns></returns>
		int GetHours() const;
		/// <summary>
		/// 获取分钟数
		/// </summary>
		/// <returns></returns>
		int GetMinutes() const;
		/// <summary>
		/// 获取秒数
		/// </summary>
		/// <returns></returns>
		int GetSeconds() const;
		/// <summary>
		/// 获取毫秒数
		/// </summary>
		/// <returns></returns>
		int GetMilliseconds() const;
		/// <summary>
		/// 获取微秒数
		/// </summary>
		/// <returns></returns>
		int GetMicroseconds() const;
		/// <summary>
		/// 获取总天数
		/// </summary>
		/// <returns></returns>
		double GetTotalDays() const;
		/// <summary>
		/// 获取总小时数
		/// </summary>
		/// <returns></returns>
		double GetTotalHours() const;
		/// <summary>
		/// 获取总分钟数
		/// </summary>
		/// <returns></returns>
		double GetTotalMinutes() const;
		/// <summary>
		/// 获取总秒数
		/// </summary>
		/// <returns></returns>
		double GetTotalSeconds() const;
		/// <summary>
		/// 获取总毫秒数
		/// </summary>
		/// <returns></returns>
		double GetTotalMilliseconds() const;
		/// <summary>
		/// 获取总微秒数
		/// </summary>
		/// <returns></returns>
		double GetTotalMicroseconds() const;

		/// <summary>
		/// 获取总时间值
		/// </summary>
		/// <returns></returns>
		Int64 GetTicks() const;


		/// <summary>
		/// 天数
		/// </summary>
		__uproperty(get = GetDuration) TimeSpan Duration;
		/// <summary>
		/// 天数
		/// </summary>
		__uproperty(get = GetNegate) TimeSpan Negate;
		/// <summary>
		/// 天数
		/// </summary>
		__uproperty(get = GetDays) int Days;
		/// <summary>
		/// 小时数
		/// </summary>
		__uproperty(get = GetHours) int Hours;
		/// <summary>
		/// 分钟数
		/// </summary>
		__uproperty(get = GetMinutes) int Minutes;
		/// <summary>
		/// 秒数
		/// </summary>
		__uproperty(get = GetSeconds) int Seconds;
		/// <summary>
		/// 毫秒数
		/// </summary>
		__uproperty(get = GetMilliseconds) int Milliseconds;
		/// <summary>
		/// 微秒数
		/// </summary>
		__uproperty(get = GetMicroseconds) int Microseconds;
		/// <summary>
		/// 总天数
		/// </summary>
		__uproperty(get = GetTotalDays)double TotalDays;
		/// <summary>
		/// 总小时数
		/// </summary>
		__uproperty(get = GetTotalHours) double TotalHours;
		/// <summary>
		/// 总分钟数
		/// </summary>
		__uproperty(get = GetTotalMinutes)double TotalMinutes;
		/// <summary>
		/// 总秒数
		/// </summary>
		__uproperty(get = GetTotalSeconds)double TotalSeconds;
		/// <summary>
		/// 总毫秒数
		/// </summary>
		__uproperty(get = GetTotalMilliseconds) double TotalMilliseconds;
		/// <summary>
		/// 总微秒数
		/// </summary>
		__uproperty(get = GetTotalMicroseconds) double TotalMicroseconds;

		/// <summary>
		/// 获取总时间值
		/// </summary>
		/// <returns></returns>
		__uproperty(get = GetTicks) Int64 Ticks;

		/// <summary>
		/// 转换为时间值
		/// </summary>
		/// <param name="days">天数</param>
		/// <param name="hours">小时数</param>
		/// <param name="minutes">分钟数</param>
		/// <param name="seconds">秒数</param>
		/// <param name="milliseconds">毫秒数</param>
		/// <param name="microseconds">微秒数</param>
		/// <param name="nano100seconds">以100纳秒为单位的时间值</param>
		__uref static Int64 TimeToTicks(int days, int hours, int minutes, int seconds, int milliseconds, int microseconds, int nano100seconds);

		/// <summary>
		/// 从总天数创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">总天数</param>
		/// <returns></returns>
		__uref static TimeSpan FromDays(double value);

		/// <summary>
		/// 从总小数数创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">总小时数</param>
		/// <returns></returns>
		__uref static TimeSpan FromHours(double value);

		/// <summary>
		/// 从总分钟数创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">总分钟数</param>
		/// <returns></returns>
		__uref static TimeSpan FromMinutes(double value);

		/// <summary>
		/// 从总秒数创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">总秒数</param>
		/// <returns></returns>
		__uref static TimeSpan FromSeconds(double value);

		/// <summary>
		/// 从总毫秒数创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">总毫秒数</param>
		/// <returns></returns>
		__uref static TimeSpan FromMilliseconds(double value);

		/// <summary>
		/// 从总微秒数创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">总微秒数</param>
		/// <returns></returns>
		__uref static TimeSpan FromMicroseconds(double value);

		/// <summary>
		/// 从以100纳秒为单位的时间值创建<see cref="TimeSpan"/>实例.
		/// </summary>
		/// <param name="value">以100纳秒为单位的时间值</param>
		/// <returns></returns>
		__uref static TimeSpan TimeSpan::FromNano100seconds(Int64 value);


		__uref TimeSpan & operator=(const TimeSpan & timeSpan);

		__uref TimeSpan operator + (const TimeSpan& timeSpan) const;

		__uref TimeSpan operator - (const TimeSpan& timeSpan) const;

		__uref TimeSpan& operator += (TimeSpan& timeSpan);

		__uref TimeSpan& operator -= (TimeSpan& timeSpan);

		__uref bool operator == (TimeSpan&  timeSpan) const;

		__uref bool operator == (const TimeSpan&  timeSpan) const;

		__uref bool operator != (TimeSpan&  timeSpan) const;

		__uref bool operator != (const TimeSpan&  timeSpan) const;

		__uref bool operator > (TimeSpan&  timeSpan) const;

		__uref bool operator >= (TimeSpan&  timeSpan) const;

		__uref bool operator < (TimeSpan&  timeSpan) const;

		__uref bool operator <= (TimeSpan&  timeSpan) const;

		__uref TimeSpan operator - () const;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, TimeSpan& result);

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="format">格式</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="style">指定风格</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, TimeSpanStyles style, TimeSpan& result);


		REFLECT_CLASS(TimeSpan)

	};

	class RUNTIME_API TimeSpanConverter: public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<TimeSpanConverter> _thisWeakPtr;
	protected:
		TimeSpanConverter(); 
		TimeSpanConverter(TimeSpanConverter const&); 
		void operator=(TimeSpanConverter const&); 
	public:
		virtual ~TimeSpanConverter();

		static std::shared_ptr<TimeSpanConverter> GetSharedInstance();

		static TimeSpanConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}