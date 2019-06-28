#pragma once
#include "Calendar.h"

namespace System
{
	/// <summary>
	/// 波斯日历类
	/// </summary>
	 class RUNTIME_API PersianCalendar : public Calendar
	{
	protected:
		friend class CalendarHelper;

		Calendar* InstanceClone() const override;

		virtual int GetDatePart(Int64 ticks, int part) const;

		Int64 DaysUpToPersianYear(int PersianYear) const;

		Int64 GetAbsoluteDatePersian(int year, int month, int day) const;

	public:

		/// <summary>
		/// 获取算法类型
		/// </summary>
		/// <returns></returns>
		CalendarAlgorithmType GetAlgorithmType() const override;

		/// <summary>
		/// 获取基础日历标识
		/// </summary>
		/// <returns></returns>
		int GetBaseCalendarID() const override;

		/// <summary>
		/// 获取当前日历中的纪元列表
		/// </summary>
		/// <returns></returns>
		std::vector<int> GetEras() const override;

		/// <summary>
		/// 获取日历标识
		/// </summary>
		/// <returns></returns>
		int GetID() const override;

		/// <summary>
		/// 获取支持的最晚日期时间
		/// </summary>
		/// <returns></returns>
		DateTime GetMaxSupportedDateTime() const override;

		/// <summary>
		/// 获取支持的最早日期时间
		/// </summary>
		/// <returns></returns>
		DateTime GetMinSupportedDateTime() const override;

		/// <summary>
		/// 获取可用两位数年份表示的 100 年范围内的最后一年
		/// </summary>
		/// <returns></returns>
		int GetTwoDigitYearMax() const override;

		/// <summary>
		/// 设置可用两位数年份表示的 100 年范围内的最后一年
		/// </summary>
		/// <param name="value">年份</param>
		/// <returns></returns>
		void SetTwoDigitYearMax(int value) override;

		/// <summary>
		/// 创建<see cref="PersianCalendar"/>实例.
		/// </summary>
		PersianCalendar();

		virtual ~PersianCalendar();

		/// <summary>
		/// 获取于指定日期时间相距指定月数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="months">指定月数</param>
		/// <returns></returns>
		DateTime AddMonths(const DateTime& time, int months) override;

		/// <summary>
		/// 获取于指定日期时间相距指定年数的日期时间
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <param name="years">指定年数</param>
		/// <returns></returns>
		DateTime AddYears(const DateTime& time, int years) override;

		/// <summary>
		/// 获取于指定日期时间是该月的几号
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>
		int GetDayOfMonth(const DateTime& time) const override;

		/// <summary>
		/// 获取于指定日期时间是星期几
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		DayOfWeek GetDayOfWeek(const DateTime& time) const override;

		/// <summary>
		/// 获取于指定日期时间是该年第几天
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetDayOfYear(const DateTime& time) const override;

		/// <summary>
		/// 获取指定纪元的指定月份和年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		int GetDaysInMonth(int year, int month, int era) const override;

		/// <summary>
		/// 获取指定纪元的指定年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		int GetDaysInYear(int year, int era) const override;

		/// <summary>
		/// 获取指定日期时间的纪元值
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetEra(const DateTime& time) const override;

		/// <summary>
		/// 获取指定纪元的指定年份的闰月
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		int GetLeapMonth(int year, int era) const override;

		/// <summary>
		/// 获取指定日期时间的月份
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetMonth(const DateTime& time) const override;

		/// <summary>
		/// 获取指定纪元的指定年份的总月数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>
		int GetMonthsInYear(int year, int era) const override;

		/// <summary>
		/// 获取指定日期时间的年份
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetYear(const DateTime& time) const override;

		/// <summary>
		/// 判断指定纪元中的指定年月日是闰日
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="day">指定天数</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		bool IsLeapDay(int year, int month, int day, int era) const override;

		/// <summary>
		/// 判断指定纪元中的指定年月是闰月
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		bool IsLeapMonth(int year, int month, int era) const override;

		/// <summary>
		/// 判断指定纪元中的指定年份是闰年
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元值</param>
		/// <returns></returns>	
		bool IsLeapYear(int year, int era) const override;

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
		DateTime ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era) const override;

		/// <summary>
		/// 返回指定年份对应的四位数年份
		/// </summary>
		/// <returns></returns>	
		int ToFourDigitYear(int year) const override;

		REFLECT_CLASS(PersianCalendar)
	};
}