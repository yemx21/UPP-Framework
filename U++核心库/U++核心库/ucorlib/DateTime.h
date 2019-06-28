#pragma once
#include "TimeSpan.h"

namespace System
{
	/// <summary>
	/// 日期时间类型
	/// </summary>
	class RUNTIME_API DateTimeKind : __uobject
	{
		__uvalue_begin(DateTimeKind, UInt32)
		/// <summary>
		/// 无限定
		/// </summary>
			__uvalue(Unspecified)
		/// <summary>
		/// 协调世界时
		/// </summary>
		__uvalue(Utc)
		/// <summary>
		/// 当地时
		/// </summary>
		__uvalue(Local)
		__uvalue_end(DateTimeKind, Unspecified)
	};

	/// <summary>
	/// 日期（星期）
	/// </summary>
	class RUNTIME_API DayOfWeek : __uobject
	{
		__uvalue_begin(DayOfWeek, UInt32)
		/// <summary>
		/// 星期日
		/// </summary>
			__uvalue(Sunday)
		/// <summary>
		/// 星期一
		/// </summary>
		__uvalue(Monday)
		/// <summary>
		/// 星期二
		/// </summary>
		__uvalue(Tuesday)
		/// <summary>
		/// 星期三
		/// </summary>
		__uvalue(Wednesday)
		/// <summary>
		/// 星期四
		/// </summary>
		__uvalue(Thursday)
		/// <summary>
		/// 星期五
		/// </summary>
		__uvalue(Friday)
		/// <summary>
		/// 星期六
		/// </summary>
		__uvalue(Saturday)
		__uvalue_end(DayOfWeek, Monday)
	};

	/// <summary>
	/// 日期风格枚举
	/// </summary>
	class RUNTIME_API DateTimeStyles : __uobject
	{
		__uvalue_begin(DateTimeStyles, UInt32)
		/// <summary>
		/// 无
		/// </summary>
		__uvalue(None, 0U)
		/// <summary>
		/// 在分析期间必须忽略前导空白字符
		/// </summary>
		__uvalue(AllowLeadingWhite, 1U)
		/// <summary>
		/// 在分析期间必须忽略尾随空白字符
		/// </summary>
		__uvalue(AllowTrailingWhite, 2U)
		/// <summary>
		/// 在分析期间必须忽略字符串中间的多余空白字符
		/// </summary>
		__uvalue(AllowInnerWhite, 4U)
		/// <summary>
		/// 在分析期间必须忽略字符串任意位置的多余空白字符，但空白字符出现在 DateTimeFormatInfo 格式模式中的情况除外。
		/// </summary>
		/// <remarks>该值是 AllowLeadingWhite、AllowTrailingWhite 和 AllowInnerWhite 值的组合</remarks>
		__uvalue(AllowWhiteSpaces, 7U)
		/// <summary>
		/// 如果分析的字符串只包含时间而不包含日期，则分析方法会假定以下公历日期：年 = 1、月 = 1 并且日 = 1。 如果没有使用此值，则假定为当前日期。
		/// </summary>
		__uvalue(NoCurrentDateDefault, 8U)
		/// <summary>
		/// 以协调世界时 (UTC) 形式返回日期和时间。 如果输入字符串表示本地时间（通过时区说明符或 AssumeLocal），则会将日期和时间从本地时间转换为 UTC。 如果输入字符串表示 UTC 时间（通过时区说明符或 AssumeUniversal），则不进行任何转换。 如果输入字符串不表示本地时间或 UTC 时间，则不进行任何转换，并且生成的 Kind 属性为 Unspecified。
		/// </summary>
		/// <remarks>此值不能与 RoundtripKind 一起使用</remarks>
		__uvalue(AdjustToUniversal, 16U)
		/// <summary>
		/// 如果分析的字符串中未指定任何时区，则假定该字符串表示本地时间。
		/// </summary>
		/// <remarks>此值不能与 AssumeUniversal 或 RoundtripKind 一起使用</remarks>
		__uvalue(AssumeLocal, 32U)
		/// <summary>
		/// 如果分析的字符串中未指定任何时区，则假定该字符串表示 UTC
		/// </summary>
		/// <remarks>此值不能与 AssumeLocal 或 RoundtripKind 一起使用</remarks>
		__uvalue(AssumeUniversal, 64U)
		/// <summary>
		/// 在使用“o”或“r”标准格式说明符将 DateTime 对象先转换为字符串，然后再将该字符串转换回 DateTime 对象时，将保留日期的 DateTimeKind 字段。
		/// </summary>
		__uvalue(RoundtripKind, 128U)
		__uvalue_end(DateTimeStyles, None)
	};
	__uenum_operator(DateTimeStyles)

	class TimeZoneInfo;
	struct TimeZoneInfoComparer;

	class DateTime;
	class DateTimeFormat;

	/// <summary>
	/// 日期时间类
	/// </summary>
	class RUNTIME_API DateTime final: __uobject
	{
	protected:
		virtual std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const;
	protected:
		Int64 dateData;
		DateTimeKind _kind;
		bool _iadst;

		int GetDatePart(int part)  const;

		static bool IsLeapYear(int year);

		Int64 DateToTicks(int year, int month, int day)  const;

		Int64 TimeToTicks(int hour, int minute, int second)  const;

		DateTime Add(double value, Int64 scale)  const;

	public:

		static DateTime const MinValue;
		static DateTime const MaxValue;


		/// <summary>
		/// 获取当前日期时间
		/// </summary>
		/// <returns></returns>
		__uref static DateTime Now();

		/// <summary>
		/// 获取当前协调时日期时间
		/// </summary>
		/// <returns></returns>
		__uref static DateTime UtcNow();

		/// <summary>
		/// 获取日期（星期）
		/// </summary>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		__uref System::DayOfWeek GetDayOfWeek(const CultureInfo& loc) const;

		/// <summary>
		/// 获取天数在指定月份位置
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="year">月份</param>
		/// <returns></returns>
		__uref static int DaysInMonth(int year, int month);

		/// <summary>
		/// 获取日期
		/// </summary>
		/// <returns></returns>
		DateTime GetDate() const;

		/// <summary>
		/// 获取天数
		/// </summary>
		/// <returns></returns>
		int GetDay() const;

		/// <summary>
		/// 获取天数( 年）
		/// </summary>
		/// <returns></returns>
		int GetDayOfYear() const;

		/// <summary>
		/// 获取天数( 月）
		/// </summary>
		/// <returns></returns>
		int GetDayOfMonth() const;

		/// <summary>
		/// 获取小时数
		/// </summary>
		/// <returns></returns>
		int GetHour() const;

		/// <summary>
		/// 获取日期时间类型
		/// </summary>
		/// <returns></returns>
		DateTimeKind GetKind() const;

		/// <summary>
		/// 创建指定日期时间类型的
		/// </summary>
		/// <param name="value">指定日期时间对象</param>
		/// <param name="kind">指定日期时间类型.</param>
		/// <returns></returns>
		__uref static DateTime SpecifyKind(const DateTime& value, DateTimeKind kind);

		/// <summary>
		/// 获取微秒数
		/// </summary>
		/// <returns></returns>
		int GetMicrosecond() const;

		/// <summary>
		/// 获取毫秒数
		/// </summary>
		/// <returns></returns>
		int GetMillisecond() const;

		/// <summary>
		/// 获取分钟数
		/// </summary>
		/// <returns></returns>
		int GetMinute() const;

		/// <summary>
		/// 获取年份
		/// </summary>
		/// <returns></returns>
		int GetYear() const;

		/// <summary>
		/// 获取年份
		/// </summary>
		/// <returns></returns>
		int GetMonth() const;

		/// <summary>
		/// 获取秒数
		/// </summary>
		/// <returns></returns>
		int GetSecond() const;

		/// <summary>
		/// 获取总微秒数
		/// </summary>
		/// <returns></returns>
		long long GetTicks() const;

		/// <summary>
		/// 设置总微秒数
		/// </summary>
		/// <returns></returns>
		void SetTicks(long long ticks);

		/// <summary>
		/// 获取当前所在日时间
		/// </summary>
		/// <returns></returns>
		TimeSpan GetTimeOfDay() const;

		/// <summary>
		/// 获取是否基于模糊动态时区的夏令时
		/// </summary>
		/// <returns></returns>
		bool GetIsAmbiguousDaylightSavingTime()const;

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		__uref DateTime();

		/// <summary>
		///  创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="ticks">时间（微秒）</param>
		__uref DateTime(long long ticks);

		/// <summary>
		///  创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="ticks">时间（微秒）</param>
		/// <param name="isAmbiguousDst">本地模糊动态时区</param>
		__uref DateTime(long long ticks, bool isAmbiguousDst);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="ticks">时间（微秒）</param>
		/// <param name="kind">日期时间类型</param>
		__uref DateTime(long long ticks, DateTimeKind kind);

		/// <summary>
		/// I创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份.</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		__uref DateTime(int year, int month, int day);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		__uref DateTime(int year, int month, int day, int hour, int minute, int second);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		/// <param name="kind">日期时间类型</param>
		__uref DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		/// <param name="millisecond">毫秒数</param>
		__uref DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		/// <param name="millisecond">毫秒数</param>
		/// <param name="kind">日期时间类型</param>
		__uref DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		/// <param name="millisecond">毫秒数</param>
		/// <param name="microsecond">微秒数</param>
		__uref DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond);

		/// <summary>
		/// 创建<see cref="DateTime"/>实例.
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		/// <param name="millisecond">毫秒数</param>
		/// <param name="microsecond">微秒数</param>
		/// <param name="kind">日期时间类型</param>
		__uref DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, DateTimeKind kind);

		DateTime AddDays(double value) const;

		DateTime AddHours(double value) const;

		DateTime AddMilliseconds(double value) const;

		DateTime AddMinutes(double value) const;

		DateTime AddMonths(int months) const;

		DateTime AddSeconds(double value) const;

		DateTime AddTicks(Int64 value) const;

		DateTime AddYears(int value) const;

		DateTime ToUniversalTime() const;

		/// <summary>
		/// 是否基于模糊动态时区的夏令时
		/// </summary>
		__uproperty(get = GetIsAmbiguousDaylightSavingTime) bool IsAmbiguousDaylightSavingTime;

		/// <summary>
		/// 日期时间类型
		/// </summary>
		__uproperty(get = GetKind) DateTimeKind Kind;

		/// <summary>
		/// 总微秒数
		/// </summary>
		__uproperty(get = GetTicks, put = SetTicks) long long Ticks;

		/// <summary>
		/// 获取日期
		/// </summary>
		/// <returns></returns>
		__uproperty(get = GetDate) DateTime Date;

		/// <summary>
		/// 年份
		/// </summary>
		__uproperty(get = GetYear) int Year;
		/// <summary>
		/// 月份
		/// </summary>
		__uproperty(get = GetMonth) int Month;
		/// <summary>
		/// 天数
		/// </summary>
		__uproperty(get = GetDay) int Day;
		/// <summary>
		/// 小时数
		/// </summary>
		__uproperty(get = GetHour) int Hour;
		/// <summary>
		/// 分钟数
		/// </summary>
		__uproperty(get = GetMinute) int Minute;
		/// <summary>
		/// 秒数
		/// </summary>
		__uproperty(get = GetSecond) int Second;
		/// <summary>
		/// 毫秒数
		/// </summary>
		__uproperty(get = GetMillisecond) int Millisecond;
		/// <summary>
		/// 微秒数
		/// </summary>
		__uproperty(get = GetMicrosecond) int Microsecond;
		/// <summary>
		/// 天数（月）
		/// </summary>
		__uproperty(get = GetDayOfMonth) int DayOfMonth;
		/// <summary>
		/// 天数（年）
		/// </summary>
		__uproperty(get = GetDayOfYear) int DayOfYear;
		/// <summary>
		/// 时间（天）
		/// </summary>
		__uproperty(get = GetTimeOfDay) TimeSpan TimeOfDay;

		__uref DateTime(const DateTime& dateTime);

		__uref DateTime & operator=(const DateTime & dateTime);

		__uref DateTime operator + (const DateTime& dateTime) const;

		__uref DateTime operator - (const DateTime& dateTime) const;

		__uref DateTime operator + (const TimeSpan& timeSpan) const;

		__uref DateTime operator - (const TimeSpan& timeSpan) const;

		__uref DateTime& operator += (DateTime& dateTime);

		__uref DateTime& operator -= (DateTime& dateTime);

		__uref DateTime& operator += (TimeSpan& timeSpan);

		__uref DateTime& operator -= (TimeSpan& timeSpan);

		__uref bool operator == (DateTime&  dateTime) const;

		__uref bool operator == (const DateTime&  dateTime) const;

		__uref bool operator != (DateTime&  dateTime) const;

		__uref bool operator != (const DateTime&  dateTime) const;

		__uref bool operator > (const DateTime&  dateTime) const;

		__uref bool operator < (const DateTime&  dateTime) const;

		__uref bool operator >=(const DateTime&  dateTime) const;

		__uref bool operator <= (const DateTime&  dateTime) const;
		
		/// <summary>
		/// 尝试创建<see cref="DateTime"/>实例
		/// </summary>
		/// <param name="year">年份</param>
		/// <param name="month">月份</param>
		/// <param name="day">天数</param>
		/// <param name="hour">小时数</param>
		/// <param name="minute">分钟数</param>
		/// <param name="second">秒数</param>
		/// <param name="millisecond">毫秒数</param>
		/// <param name="microsecond">微秒数</param>
		/// <param name="kind">日期时间类型</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryCreate(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, DateTimeKind kind, DateTime& result);

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值.</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, DateTime& result);

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值.</param>
		/// <param name="format">格式</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="style">指定风格</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, DateTimeStyles style, DateTime& result);

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值.</param>
		/// <param name="formats">格式</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="style">指定风格</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParseExactMultiple(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, DateTimeStyles style, DateTime& result);

		REFLECT_CLASS(DateTime)
	};

	class RUNTIME_API DateTimeConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<DateTimeConverter> _thisWeakPtr;
	protected:
		DateTimeConverter();
		DateTimeConverter(DateTimeConverter const&);
		void operator=(DateTimeConverter const&);
	public:
		virtual ~DateTimeConverter();

		static std::shared_ptr<DateTimeConverter> GetSharedInstance();

		static DateTimeConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}