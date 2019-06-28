#pragma once
#include "Calendar.h"
#include "GregorianCalendar.h"
namespace System
{
	/// <summary>
	/// 东亚阴阳历类
	/// </summary>
	 class  RUNTIME_API EastAsianLunisolarCalendar : public Calendar
	{
	protected:
		friend class CalendarHelper;

		virtual void GetCalEraInfo(std::vector<EraInfo>&) const = 0;

		virtual int GetMaxCalendarYear() const = 0;

		virtual DateTime GetMaxDate() const = 0;

		virtual int GetMinCalendarYear() const = 0;

		virtual DateTime GetMinDate() const = 0;

		EastAsianLunisolarCalendar();

		void CheckEraRange(int era) const;

		void CheckTicksRange(Int64 ticks) const;

		int CheckYearMonthRange(int year, int month, int era) const;

		int CheckYearRange(int year, int era) const;

		int GetCelestialStem(int sexagenaryYear) const;

		virtual int GetGregorianYear(int year, int era) const = 0;

		virtual int GetSexagenaryYear(const DateTime& time) const;

		int GetTerrestrialBranch(int sexagenaryYear) const;

		virtual int GetYear(int year, const DateTime& time) const = 0;

		virtual int GetYearInfo(int LunarYear, int Index) const = 0;
	public:

		/// <summary>
		/// 获取算法类型
		/// </summary>
		/// <returns></returns>
		CalendarAlgorithmType GetAlgorithmType() const override;

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
		void SetTwoDigitYearMax(int value)  override;

		virtual ~EastAsianLunisolarCalendar();

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
		int GetDayOfMonth(const DateTime& time)  const override;

		/// <summary>
		/// 获取于指定日期时间是星期几
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		DayOfWeek GetDayOfWeek(const DateTime& time)  const override;

		/// <summary>
		/// 获取于指定日期时间是该年第几天
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetDayOfYear(const DateTime& time)  const override;

		/// <summary>
		/// 获取指定纪元的指定月份和年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="month">指定月份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		int GetDaysInMonth(int year, int month, int era)  const override;

		/// <summary>
		/// 获取指定纪元的指定年份中的天数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <param name="era">指定纪元</param>
		/// <returns></returns>	
		int GetDaysInYear(int year, int era)  const override;

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
		/// 获取当前纪元的指定年份的总月数
		/// </summary>
		/// <param name="year">指定年份</param>
		/// <returns></returns>	
		int GetMonthsInYear(int year, int era) const override;

		/// <summary>
		/// 获取指定日期时间的年份
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetYear(const DateTime& time)  const override;

	private:
		static int GregorianIsLeapYear(int y);

		void GregorianToLunar(int nSYear, int nSMonth, int nSDate, int& nLYear, int& nLMonth, int& nLDate) const;

		int InternalGetDaysInMonth(int year, int month) const;

		bool InternalIsLeapYear(int year) const;

		bool IsLeapDay(int year, int month, int day, int era)  const override;

		bool IsLeapMonth(int year, int month, int era)  const override;

		bool IsLeapYear(int year, int era)  const override;

		bool LunarToGregorian(int nLYear, int nLMonth, int nLDate, int& nSolarYear, int& nSolarMonth, int& nSolarDay) const;

		DateTime LunarToTime(const DateTime& time, int year, int month, int day) const;

		int MaxEraCalendarYear(int era) const;

		int MinEraCalendarYear(int era) const;

		void TimeToLunar(const DateTime& time, int& year, int& month, int& day) const;

		DateTime ToDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int era)  const override;

		int ToFourDigitYear(int year)  const override;

		REFLECT_CLASS(EastAsianLunisolarCalendar)
	};
}