#pragma once
#include "Config.h"
#include <locale>
#include <locale.h>
#include <algorithm>

namespace System
{
	/// <summary>
	/// 编码
	/// </summary>
	enum class Encodings
	{
		/// <summary>
		/// ANSI编码
		/// </summary>
		Ansi,
		/// <summary>
		/// UTF8编码
		/// </summary>
		Utf8,
		/// <summary>
		/// UTF16小端编码
		/// </summary>
		Utf16LE,
		/// <summary>
		/// 默认Unicode编码
		/// </summary>
		Unicode = Utf16LE,
		/// <summary>
		/// UTF16大端编码
		/// </summary>
		Utf16BE,
		/// <summary>
		/// UTF32小端编码
		/// </summary>
		Utf32LE,
		/// <summary>
		/// UTF32大端编码
		/// </summary>
		Utf32BE,
	};

	/// <summary>
	/// 字符串比较
	/// </summary>
	enum class StringComparison
	{
		/// <summary>
		/// 当前区域
		/// </summary>
		CurrentCulture,
		/// <summary>
		/// 忽略大小写的当前区域
		/// </summary>
		CurrentCultureIgnoreCase,
		/// <summary>
		/// 非区域关联
		/// </summary>
		InvariantCulture,
		/// <summary>
		/// 忽略大小写的非区域关联
		/// </summary>
		InvariantCultureIgnoreCase,
	};

	class EncodingHelper;

	class CultureInfo;

	class CultureInfo_impl;

	/// <summary>
	/// 区域性格式符
	/// </summary>
	enum class CultureInfoPatterns
	{
		/// <summary>
		/// 长时间正值全格式符
		/// </summary>
		FullTimeSpanPositivePattern,
		/// <summary>
		/// 长时间符值全格式符
		/// </summary>
		FullTimeSpanNegativePattern,
		/// <summary>
		/// 年月格式符
		/// </summary>
		YearMonthPattern,
		/// <summary>
		/// 短日期格式符
		/// </summary>
		ShortDatePattern,
		/// <summary>
		/// 长日期格式符
		/// </summary>
		LongDatePattern,
		/// <summary>
		/// 短时间格式符
		/// </summary>
		ShortTimePattern,
		/// <summary>
		/// 长时间格式符
		/// </summary>
		LongTimePattern,
		/// <summary>
		/// 月日格式符
		/// </summary>
		MonthDayPattern,

		/// <summary>
		/// 长日期短时间全格式符
		/// </summary>
		FullLongDateShortTimePattern,

		/// <summary>
		/// 长日期长时间全格式符
		/// </summary>
		FullLongDateLongTimePattern,

		/// <summary>
		/// 短日期短时间格式符
		/// </summary>
		GeneralShortDateShortTimePattern,

		/// <summary>
		/// 短日期长时间格式符
		/// </summary>
		GeneralShortDateLongTimePattern,

		/// <summary>
		/// 日期长偏移格式符
		/// </summary>
		DateTimeOffsetPattern,
	};

	/// <summary>
	/// 区域性常数名称集合
	/// </summary>
	enum class CultureInfoConstNames
	{
		/// <summary>
		/// 星期日
		/// </summary>
		Sunday,
		/// <summary>
		/// 星期一
		/// </summary>
		Monday,
		/// <summary>
		/// 星期二
		/// </summary>
		Tuesday,
		/// <summary>
		/// 星期三
		/// </summary>
		Wednesday,
		/// <summary>
		/// 星期四
		/// </summary>
		Thursday,
		/// <summary>
		/// 星期五
		/// </summary>
		Friday,
		/// <summary>
		/// 星期六
		/// </summary>
		Saturday,

		/// <summary>
		/// 星期日（简写）
		/// </summary>
		Abbreviated_Sunday,
		/// <summary>
		/// 星期一（简写）
		/// </summary>
		Abbreviated_Monday,
		/// <summary>
		/// 星期二（简写）
		/// </summary>
		Abbreviated_Tuesday,
		/// <summary>
		/// 星期三（简写）
		/// </summary>
		Abbreviated_Wednesday,
		/// <summary>
		///  星期四（简写）
		/// </summary>
		Abbreviated_Thursday,
		/// <summary>
		/// 星期五（简写）
		/// </summary>
		Abbreviated_Friday,
		/// <summary>
		/// 星期六（简写）
		/// </summary>
		Abbreviated_Saturday,

		/// <summary>
		/// 一月
		/// </summary>
		January,
		/// <summary>
		/// 二月
		/// </summary>
		February,
		/// <summary>
		/// 三月
		/// </summary>
		March,
		/// <summary>
		/// 四月
		/// </summary>
		April,
		/// <summary>
		/// 五月
		/// </summary>
		May,
		/// <summary>
		/// 六月
		/// </summary>
		June,
		/// <summary>
		/// 七月
		/// </summary>
		July,
		/// <summary>
		/// 八月
		/// </summary>
		August,
		/// <summary>
		/// 九月
		/// </summary>
		September,
		/// <summary>
		/// 十月
		/// </summary>
		October,
		/// <summary>
		/// 十一月
		/// </summary>
		November,
		/// <summary>
		/// 十二月
		/// </summary>
		December,

		/// <summary>
		/// 一月（简写）
		/// </summary>
		Abbreviated_January,
		/// <summary>
		/// 二月（简写）
		/// </summary>
		Abbreviated_February,
		/// <summary>
		/// 三月（简写）
		/// </summary>
		Abbreviated_March,
		/// <summary>
		/// 四月（简写）
		/// </summary>
		Abbreviated_April,
		/// <summary>
		/// 五月（简写）
		/// </summary>
		Abbreviated_May,
		/// <summary>
		/// 六月（简写）
		/// </summary>
		Abbreviated_June,
		/// <summary>
		/// 七月（简写）
		/// </summary>
		Abbreviated_July,
		/// <summary>
		/// 八月（简写）
		/// </summary>
		Abbreviated_August,
		/// <summary>
		/// 九月（简写）
		/// </summary>
		Abbreviated_September,
		/// <summary>
		/// 十月（简写）
		/// </summary>
		Abbreviated_October,
		/// <summary>
		/// 十一月（简写）
		/// </summary>
		Abbreviated_November,
		/// <summary>
		///  十二月（简写）
		/// </summary>
		Abbreviated_December,

		/// <summary>
		/// 一月（所有格）
		/// </summary>
		Genitive_January,
		/// <summary>
		/// 二月（所有格）
		/// </summary>
		Genitive_February,
		/// <summary>
		/// 三月（所有格）
		/// </summary>
		Genitive_March,
		/// <summary>
		/// 四月（所有格）
		/// </summary>
		Genitive_April,
		/// <summary>
		/// 五月（所有格）
		/// </summary>
		Genitive_May,
		/// <summary>
		/// 六月（所有格）
		/// </summary>
		Genitive_June,
		/// <summary>
		/// 七月（所有格）
		/// </summary>
		Genitive_July,
		/// <summary>
		/// 八月（所有格）
		/// </summary>
		Genitive_August,
		/// <summary>
		/// 九月（所有格）
		/// </summary>
		Genitive_September,
		/// <summary>
		/// 十月（所有格）
		/// </summary>
		Genitive_October,
		/// <summary>
		/// 十一月（所有格）
		/// </summary>
		Genitive_November,
		/// <summary>
		/// 十二月（所有格）
		/// </summary>
		Genitive_December,

		/// <summary>
		/// 一月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_January,
		/// <summary>
		/// 二月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_February,
		/// <summary>
		/// 三月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_March,
		/// <summary>
		/// 四月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_April,
		/// <summary>
		/// 五月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_May,
		/// <summary>
		/// 六月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_June,
		/// <summary>
		/// 七月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_July,
		/// <summary>
		/// 八月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_August,
		/// <summary>
		/// 九月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_September,
		/// <summary>
		/// 十月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_October,
		/// <summary>
		/// 十一月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_November,
		/// <summary>
		/// 十二月（所有格简写）
		/// </summary>
		Genitive_Abbreviated_December,


		/// <summary>
		/// 一月（闰年）
		/// </summary>
		Leap_January,
		/// <summary>
		/// 二月（闰年）
		/// </summary>
		Leap_February,
		/// <summary>
		/// 三月（闰年）
		/// </summary>
		Leap_March,
		/// <summary>
		/// 四月（闰年）
		/// </summary>
		Leap_April,
		/// <summary>
		/// 五月（闰年）
		/// </summary>
		Leap_May,
		/// <summary>
		/// 六月（闰年）
		/// </summary>
		Leap_June,
		/// <summary>
		/// 七月（闰年）
		/// </summary>
		Leap_July,
		/// <summary>
		/// 八月（闰年）
		/// </summary>
		Leap_August,
		/// <summary>
		/// 九月（闰年）
		/// </summary>
		Leap_September,
		/// <summary>
		/// 十月（闰年）
		/// </summary>
		Leap_October,
		/// <summary>
		/// 十一月（闰年）
		/// </summary>
		Leap_November,
		/// <summary>
		/// 十二月（闰年）
		/// </summary>
		Leap_December,



		/// <summary>
		/// 公元
		/// </summary>
		Era,

		/// <summary>
		/// 公元（简写）
		/// </summary>
		Abbreviated_Era,

		/// <summary>
		/// 英文公元（简写）
		/// </summary>
		Abbreviated_EnglishEra,

		/// <summary>
		/// 日期分隔符
		/// </summary>
		DateSeparator,
		/// <summary>
		/// 时间分隔符
		/// </summary>
		TimeSeparator,

		/// <summary>
		/// 上午标志
		/// </summary>
		AMDesignator,
		/// <summary>
		/// 下午标志
		/// </summary>
		PMDesignator,

		/// <summary>
		/// 小数点
		/// </summary>
		DecimalPoint,
		/// <summary>
		/// 千位分隔符
		/// </summary>
		ThousandsSeparator,
		/// <summary>
		/// 货币符号
		/// </summary>
		CurrencySymbol,
		/// <summary>
		/// 货币小数点
		/// </summary>
		MoneyDecimalPoint,
		/// <summary>
		/// 货币千位分隔符
		/// </summary>
		MoneyThousandsSeparator,
		/// <summary>
		/// 正值符号
		/// </summary>
		PositiveSignSymbol,
		/// <summary>
		/// 负值符号
		/// </summary>
		NegativeSignSymbol,

	};

	/// <summary>
	/// 区域性货币常量枚举
	/// </summary>
	enum class CultureInfoMoneyConsts
	{
		/// <summary>
		/// 货币小数点
		/// </summary>
		Fraction,
		/// <summary>
		/// 正值货币符号位置
		/// </summary>
		PositiveCurrencySymbolPos,
		/// <summary>
		/// 正值货币分隔符位置
		/// </summary>
		PositiveSeparatorPos,
		/// <summary>
		/// 货币正号位置
		/// </summary>
		PositiveSignSymbolPos,
		/// <summary>
		/// 负值货币符号位置
		/// </summary>
		NegativeCurrencySymbolPos,
		/// <summary>
		/// 负值货币分隔符位置
		/// </summary>
		NegativeSeparatorPos,
		/// <summary>
		/// 货币负号位置
		/// </summary>
		NegativeSignSymbolPos,
	};

	/// <summary>
	/// 区域性布尔型常量枚举
	/// </summary>
	enum class CultureInfoBooleanFlags
	{
		/// <summary>
		/// 星期首日为星期一
		/// </summary>
		FirstDayofWeekIsMonday,
		/// <summary>
		/// 强制使用两位数字表示年份
		/// </summary>
		///<remarks>日本和台湾地区为true</remarks>
		HasForceTwoDigitYears, 
		/// <summary>
		/// 使用所有格
		/// </summary>
		UseGenitiveMonth,
		/// <summary>
		/// 含有第13个月
		/// </summary>
		Has13Months
	};

	class Calendar;
	class DateTimeFormat;

	/// <summary>
	/// 区域性信息类
	/// </summary>
	 class RUNTIME_API CultureInfo final
	{
	private:
		friend class CultureInfo_impl;
		CultureInfo_impl* impl;
		friend class EncodingHelper;
	protected:
		friend class DateTimeFormat;
		int GetInternalValue(int id) const;
	public:		

		/// <summary>
		/// 美洲
		/// </summary>
		static const char* america;

		/// <summary>
		/// 香港 （中国）
		/// </summary>
		static const char* hongKong;

		/// <summary>
		/// 台湾（中国）
		/// </summary>
		static const char* taiWan;

		/// <summary>
		/// 中国
		/// </summary>
		static const char* china;

		/// <summary>
		/// 日本
		/// </summary>
		static const char* japan;

		/// <summary>
		/// 南韩
		/// </summary>
		static const char* southkorea;

		/// <summary>
		/// 英国
		/// </summary>
		static const char* unitedKingdom;

		/// <summary>
		/// 美国
		/// </summary>
		static const char* unitedStates;
	public:
		/// <summary>
		/// 创建 <see cref="CultureInfo"/>实例.
		/// </summary>
		/// <param name="locName">区域名称</param>
		CultureInfo(const char* locName);

		CultureInfo(const CultureInfo& info);

		CultureInfo(CultureInfo&& info);

		~CultureInfo();

		CultureInfo& operator =(const CultureInfo& info);

		bool Is(const char* locname) const;

		/// <summary>
		/// 获取宽字符名称
		/// </summary>
		const wchar_t* GetWidenName() const;

		/// <summary>
		/// 获取名称
		/// </summary>
		const char* GetName() const;

		/// <summary>
		/// 获取区域性信息表示 （C语言）
		/// </summary>
		const std::locale GetLocale() const;

		/// <summary>
		/// 获取分组区域性信息表示 （C语言）
		/// </summary>
		const std::locale GetLocale_Group() const;

		/// <summary>
		/// 获取区域性信息表示 （C++语言）
		/// </summary>
		const _locale_t GetLocale_t() const;

		/// <summary>
		/// 获取当前区域性信息
		/// </summary>
		static CultureInfo Current();

		/// <summary>
		/// 获取非依赖性区域性信息
		/// </summary>
		static CultureInfo& Invariant();

		/// <summary>
		/// 获取区域性货币常量符号
		/// </summary>
		/// <param name="moneyconst">货币常量枚举</param>
		/// <returns></returns>
		const char GetMoneyConst(CultureInfoMoneyConsts moneyconst) const;

		/// <summary>
		/// 获取区域性格式符个数
		/// </summary>
		/// <param name="pattern">格式符枚举</param>
		/// <returns></returns>
		unsigned int GetPatternCount(CultureInfoPatterns pattern) const;

		/// <summary>
		/// 获取区域性格式符
		/// </summary>
		/// <param name="pattern">格式符枚举</param>
		/// <returns></returns>
		const wchar_t* GetPattern(CultureInfoPatterns pattern, unsigned int index = 0) const;

		/// <summary>
		/// 获取区域性常量名个数
		/// </summary>
		/// <param name="name">常量名枚举</param>
		/// <returns></returns>
		unsigned int GetConstsNameCount(CultureInfoConstNames name) const;

		/// <summary>
		/// 获取区域性常量名
		/// </summary>
		/// <param name="name">常量名枚举</param>
		/// <returns></returns>
		const wchar_t* GetConstsName(CultureInfoConstNames name, unsigned int index=0) const;

		/// <summary>
		/// 获取区域性布尔型常量名
		/// </summary>
		/// <param name="name">布尔型常量枚举</param>
		/// <returns></returns>
		bool GetBooleanFlags(CultureInfoBooleanFlags flag) const;

		/// <summary>
		/// 获取区域性默认日历
		/// </summary>
		/// <returns></returns>
		const System::Calendar* GetCalendar() const;

		/// <summary>
		/// 获取区域性可选日历标识
		/// </summary>
		/// <returns></returns>
		std::vector<int> GetOptionalCalendarIds() const;

		/// <summary>
		/// 从指定可选日历标识中产生新的区域性信息
		/// </summary>
		/// <returns></returns>
		CultureInfo CreateByCalendarId(int)const;

		/// <summary>
		/// 从指定日历中产生新的区域性信息
		/// </summary>
		/// <returns></returns>
		CultureInfo CreateByCalendar(System::Calendar* newCalendar)const;

		/// <summary>
		/// 名称
		/// </summary>
		_declspec(property(get = GetName)) const char* Name;

		/// <summary>
		/// 宽字符名称
		/// </summary>
		_declspec(property(get = GetWidenName)) const wchar_t* WidenName;

		/// <summary>
		/// 区域性信息表示 （C语言）
		/// </summary>
		_declspec(property(get = GetLocale)) const std::locale CLocale;

		/// <summary>
		/// 分组区域性信息表示 （C语言）
		/// </summary>
		_declspec(property(get = GetLocale_Group)) const std::locale Locale_Group;

		/// <summary>
		/// 区域性信息表示 （C++语言）
		/// </summary>
		_declspec(property(get = GetLocale_t)) const _locale_t CPlusLocale;

		/// <summary>
		/// 区域性默认日历
		/// </summary>
		_declspec(property(get = GetCalendar)) const System::Calendar* Calendar;
	};
}
