#pragma once
#include "DateTime.h"



namespace System
{	
	/// <summary>
	/// 日历算法类型枚举
	/// </summary>
	class RUNTIME_API CalendarAlgorithmType : __uobject
	{
		__uvalue_begin(CalendarAlgorithmType, UShort)
		/// <summary>
		/// 未知
		/// </summary>
		__uvalue(Unknown)
		/// <summary>
		/// 阳历
		/// </summary>
			__uvalue(SolarCalendar)
		/// <summary>
		/// 阴历
		/// </summary>
		__uvalue(LunarCalendar)
		/// <summary>
		/// 阴阳历
		/// </summary>
			__uvalue(LunisolarCalendar)
		__uvalue_end(CalendarAlgorithmType, Unknown)
	};

	/// <summary>
	/// 年份第一周规则枚举
	/// </summary>
	class RUNTIME_API CalendarWeekRule : __uobject
	{
		__uvalue_begin(CalendarWeekRule, UShort)
		/// <summary>
		/// 第一周从该年的第一天开始，到所指定周的下一个首日前结束
		/// </summary>
		__uvalue(FirstDay)
		/// <summary>
		/// 第一周从所指定周首日的第一个匹配项开始，可以是年的第一天或其后某一天
		/// </summary>
		__uvalue(FirstFullWeek)
		/// <summary>
		/// 第一周是第一个在指定的周首日前包含四天或更多天的周
		/// </summary>
		__uvalue(FirstFourDayWeek)
		__uvalue_end(CalendarWeekRule, FirstDay)
	};

	class CalendarData;

	/// <summary>
	/// 纪元年份信息
	/// </summary>
	class RUNTIME_API EraInfo
	{
	public:
		/// <summary>
		/// 纪元值
		/// </summary>
		int era;

		/// <summary>
		/// 时间值
		/// </summary>
		Int64 ticks;

		/// <summary>
		/// 年份偏移
		/// </summary>
		int yearOffset;

		/// <summary>
		/// 最小纪元年份
		/// </summary>
		int minEraYear;

		/// <summary>
		/// 最大纪元年份
		/// </summary>
		int maxEraYear;

		/// <summary>
		/// 纪元名称
		/// </summary>
		std::wstring eraName;

		/// <summary>
		/// 纪元名称（简写）
		/// </summary>
		std::wstring abbrevEraName;

		/// <summary>
		/// 纪元名称（英文）
		/// </summary>
		std::wstring englishEraName;

		/// <summary>
		/// 创建<see cref="EraInfo"/>实例.
		/// </summary>
		/// <param name="era">纪元值</param>
		/// <param name="startYear">开始年份</param>
		/// <param name="startMonth">开始周数</param>
		/// <param name="startDay">开始日数</param>
		/// <param name="yearOffset">年份偏移</param>
		/// <param name="minEraYear">最小纪元年份</param>
		/// <param name="maxEraYear">最大纪元年份</param>
		EraInfo(int era, int startYear, int startMonth, int startDay, int yearOffset, int minEraYear, int maxEraYear);

		/// <summary>
		/// 创建<see cref="EraInfo"/>实例.
		/// </summary>
		/// <param name="era">纪元值</param>
		/// <param name="startYear">开始年份</param>
		/// <param name="startMonth">开始周数</param>
		/// <param name="startDay">开始日数</param>
		/// <param name="yearOffset">年份偏移</param>
		/// <param name="minEraYear">最小纪元年份</param>
		/// <param name="maxEraYear">最大纪元年份</param>
		/// <param name="eraName">纪元名称</param>
		/// <param name="abbrevEraName">纪元名称（简写）</param>
		/// <param name="englishEraName">纪元名称（英文）</param>
		EraInfo(int era, int startYear, int startMonth, int startDay, int yearOffset, int minEraYear, int maxEraYear, const std::wstring& eraName, const std::wstring& abbrevEraName, const std::wstring& englishEraName);

		EraInfo(const EraInfo& info);
	};

	class GregorianCalendarHelper;

	class DateTimeParse;

	class CalendarHelper;

	/// <summary>
	/// 日历类
	/// </summary>
	 class RUNTIME_API Calendar : __uobject, _noncopyable
	{
	private:
		mutable int m_currentEraValue;
		bool m_isReadOnly;
		void SetReadOnlyState(bool readOnly);
	protected:

		friend class GregorianCalendarHelper;
		friend class DateTimeParse;
		friend class CalendarHelper;

		mutable int twoDigitYearMax;
		Calendar();

		DateTime Add(const DateTime& time, double value, int scale);

		static void CheckAddResult(Int64 ticks, const DateTime& minValue, const DateTime& maxValue);

		int GetFirstDayWeekOfYear(const DateTime& time, int firstDayOfWeek) const;

		static int GetSystemTwoDigitYearSetting(int CalID, int defaultYearValue);

		virtual int GetWeekOfYear(const DateTime& time, CalendarWeekRule rule, DayOfWeek firstDayOfWeek) const;

		int GetWeekOfYearFullDays(const DateTime& time, int firstDayOfWeek, int fullDays) const;

		int GetWeekOfYearOfMinSupportedDateTime(int firstDayOfWeek, int minimumDaysInFirstWeek) const;

		virtual bool IsValidDay(int year, int month, int day, int era) const;

		virtual bool IsValidMonth(int year, int month, int era)const;

		virtual bool IsValidYear(int year, int era)const;

		static Int64 TimeToTicks(int hour, int minute, int second, int millisecond);

		virtual bool TryToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era, DateTime& result) const;

		/// <summary>
		/// 获取当前日历对象的深度副本
		/// </summary>
		/// <returns></returns>	
		virtual Calendar* InstanceClone() const = 0;

		void VerifyWritable();

	public:

		/// <summary>
		/// 获取算法类型
		/// </summary>
		/// <returns></returns>
		virtual CalendarAlgorithmType GetAlgorithmType() const;

		/// <summary>
		/// 获取基础日历标识
		/// </summary>
		/// <returns></returns>
		virtual int GetBaseCalendarID() const;

		/// <summary>
		/// 获取当前纪元值
		/// </summary>
		/// <returns></returns>
		virtual int GetCurrentEraValue() const;

		virtual ~Calendar();

		/// <summary>
		/// 获取最小支持年份前的年天数
		/// </summary>
		/// <returns></returns>
		virtual int GetDaysInYearBeforeMinSupportedYear() const;

		/// <summary>
		/// 获取当前日历中的纪元列表
		/// </summary>
		/// <returns></returns>
		virtual std::vector<int>GetEras() const = 0;

		/// <summary>
		/// 获取日历标识
		/// </summary>
		/// <returns></returns>
		virtual int GetID() const;

		/// <summary>
		/// 获取当前日历是否为只读
		/// </summary>
		/// <returns></returns>
		bool GetIsReadOnly() const;

		/// <summary>
		/// 获取支持的最晚日期时间
		/// </summary>
		/// <returns></returns>
		virtual DateTime GetMaxSupportedDateTime() const;

		/// <summary>
		/// 获取支持的最早日期时间
		/// </summary>
		/// <returns></returns>
		virtual DateTime GetMinSupportedDateTime() const;

		/// <summary>
		/// 获取可用两位数年份表示的 100 年范围内的最后一年
		/// </summary>
		/// <returns></returns>
		virtual int GetTwoDigitYearMax() const;

		/// <summary>
		/// 设置可用两位数年份表示的 100 年范围内的最后一年
		/// </summary>
		/// <param name="value">年份</param>
		/// <returns></returns>
		virtual void SetTwoDigitYearMax(int value);

		/// <summary>
		/// 获取于指定日期时间相距指定天数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="days">指定天数</param>
		/// <returns></returns>
		virtual DateTime AddDays(const DateTime& time, int days);

		/// <summary>
		/// 获取于指定日期时间相距指定小时数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="hours">指定小时数</param>
		/// <returns></returns>
		virtual DateTime AddHours(const DateTime& time, int hours);

		/// <summary>
		/// 获取于指定日期时间相距指定毫秒数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="milliseconds">指定毫秒数</param>
		/// <returns></returns>
		virtual DateTime AddMilliseconds(const DateTime& time, double milliseconds);

		/// <summary>
		/// 获取于指定日期时间相距指定分粥数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="minutes">指定分钟数</param>
		/// <returns></returns>
		virtual DateTime AddMinutes(const DateTime& time, int minutes);

		/// <summary>
		/// 获取于指定日期时间相距指定月数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="months">指定月数</param>
		/// <returns></returns>
		virtual DateTime AddMonths(const DateTime& time, int months) = 0;

		/// <summary>
		/// 获取于指定日期时间相距指定秒数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="seconds">指定秒数</param>
		/// <returns></returns>
		virtual DateTime AddSeconds(const DateTime& time, int seconds);

		/// <summary>
		/// 获取于指定日期时间相距指定周数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="weeks">指定周数</param>
		/// <returns></returns>
		virtual DateTime AddWeeks(const DateTime& time, int weeks);

		/// <summary>
		/// 获取于指定日期时间相距指定年数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="years">指定年数</param>
		/// <returns></returns>
		virtual DateTime AddYears(const DateTime& time, int years) = 0;

		/// <summary>
		/// 获取于指定日期时间是该月的几号
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetDayOfMonth(const DateTime& time) const = 0;

		/// <summary>
		/// 获取于指定日期时间是星期几
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual DayOfWeek GetDayOfWeek(const DateTime& time) const = 0;

		/// <summary>
		/// 获取于指定日期时间是该年第几天
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetDayOfYear(const DateTime& time) const = 0;

		/// <summary>
		/// 获取当前纪元的指定月份和年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <returns></returns>	
		virtual int GetDaysInMonth(int year, int month) const;

		/// <summary>
		/// 获取指定纪元的指定月份和年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		virtual int GetDaysInMonth(int year, int month, int era) const = 0;

		/// <summary>
		/// 获取当前纪元的指定年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <returns></returns>	
		virtual int GetDaysInYear(int year) const;

		/// <summary>
		/// 获取指定纪元的指定年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		virtual int GetDaysInYear(int year, int era) const = 0;

		/// <summary>
		/// 获取指定日期时间的纪元值
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetEra(const DateTime& time) const = 0;
		
		/// <summary>
		/// 获取指定日期时间的小时数
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetHour(const DateTime& time) const;

		/// <summary>
		/// 获取当期那纪元的指定年份的闰月
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <returns></returns>	
		virtual int GetLeapMonth(int year) const;

		/// <summary>
		/// 获取指定纪元的指定年份的闰月
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		virtual int GetLeapMonth(int year, int era) const;

		/// <summary>
		/// 获取指定日期时间的毫秒数
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual double GetMilliseconds(const DateTime& time) const;

		/// <summary>
		/// 获取指定日期时间的分钟数
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetMinute(const DateTime& time) const;

		/// <summary>
		/// 获取指定日期时间的月份
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetMonth(const DateTime& time) const = 0;

		/// <summary>
		/// 获取当前纪元的指定年份的总月数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <returns></returns>	
		virtual int GetMonthsInYear(int year)const;

		/// <summary>
		/// 获取指定纪元的指定年份的总月数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		virtual int GetMonthsInYear(int year, int era) const = 0;

		/// <summary>
		/// 获取指定日期时间的分钟数
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetSecond(const DateTime& time)const;

		/// <summary>
		/// 获取指定日期时间的年份
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		virtual int GetYear(const DateTime& time) const = 0;

		/// <summary>
		/// 判断当前纪元中的指定年月日是闰日
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="day">指定天数</param>
		/// <returns></returns>	
		virtual bool IsLeapDay(int year, int month, int day) const;

		/// <summary>
		/// 判断指定纪元中的指定年月日是闰日
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="day">指定天数</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		virtual bool IsLeapDay(int year, int month, int day, int era) const = 0;

		/// <summary>
		/// 判断当前纪元中的指定年月是闰月
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <returns></returns>	
		virtual bool IsLeapMonth(int year, int month) const;

		/// <summary>
		/// 判断指定纪元中的指定年月是闰月
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		virtual bool IsLeapMonth(int year, int month, int era) const = 0;

		/// <summary>
		/// 判断当前纪元中的指定年份是闰年
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <returns></returns>	
		virtual bool IsLeapYear(int year) const;

		/// <summary>
		/// 判断指定纪元中的指定年份是闰年
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		virtual bool IsLeapYear(int year, int era) const = 0;

		/// <summary>
		/// 获取指定日历对象的只读副本
		/// </summary>
		/// <returns></returns>	
		static Calendar* GetReadOnly(const Calendar* calendar);

		/// <summary>
		/// 获取当前日历对象的非只读副本
		/// </summary>
		/// <returns></returns>	
		Calendar* Clone() const;

		/// <summary>
		/// 返回当前纪元中的指定年月日和时间的日期时间
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="day">指定天数</param>
		/// <param name="hour">指定小时数</param>
		/// <param name="minute">指定分钟数</param>
		/// <param name="second">指定秒数</param>
		/// <param name="millisecond">指定毫秒数</param>
		/// <returns></returns>	
		virtual DateTime ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond) const;

		/// <summary>
		/// 返回指定纪元中的指定年月日和时间的日期时间
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="day">指定天数</param>
		/// <param name="hour">指定小时数</param>
		/// <param name="minute">指定分钟数</param>
		/// <param name="second">指定秒数</param>
		/// <param name="millisecond">指定毫秒数</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		virtual DateTime ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const = 0;

		/// <summary>
		/// 返回指定年份对应的四位数年份
		/// </summary>
		/// <returns></returns>	
		virtual int ToFourDigitYear(int year) const;

		REFLECT_CLASS(Calendar)
	};

	
}