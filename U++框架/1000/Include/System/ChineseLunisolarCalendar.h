#pragma once

#include "EastAsianLunisolarCalendar.h"

namespace System
{
	/// <summary>
	/// 中国阴阳历类
	/// </summary>
	 class RUNTIME_API ChineseLunisolarCalendar : public EastAsianLunisolarCalendar
	{
	protected:
		friend class CalendarHelper;

		void GetCalEraInfo(std::vector<EraInfo>&) const override;

		int GetMaxCalendarYear() const override;

		DateTime GetMaxDate() const override;

		int GetMinCalendarYear()const override;

		DateTime GetMinDate() const override;

		int GetGregorianYear(int year, int era) const override;

		int GetYear(int year, const DateTime& time) const override;

		int GetYearInfo(int LunarYear, int Index) const override;

		Calendar* InstanceClone() const override;
	public:

		/// <summary>
		/// 获取基础日历标识
		/// </summary>
		/// <returns></returns>
		int GetBaseCalendarID() const override;

		/// <summary>
		/// 获取最小支持年份前的年天数
		/// </summary>
		/// <returns></returns>
		int GetDaysInYearBeforeMinSupportedYear() const override;

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
		/// 创建<see cref="ChineseLunisolarCalendar"/>实例.
		/// </summary>
		ChineseLunisolarCalendar();

		virtual ~ChineseLunisolarCalendar();

		/// <summary>
		/// 获取指定日期时间的纪元值
		/// </summary>
		/// <param name="time">指定日期时间</param>
		/// <returns></returns>	
		int GetEra(const DateTime& time) const override;

		REFLECT_CLASS(ChineseLunisolarCalendar)
	};
}