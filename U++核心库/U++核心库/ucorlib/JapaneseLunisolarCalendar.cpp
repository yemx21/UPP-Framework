#include "JapaneseLunisolarCalendar.h"
#include "BuiltInAssembly.h"
#include "CalendarHelper.h"
using namespace System;

static const DateTime minDate = DateTime(1960, 1, 28);
static const DateTime maxDate = DateTime(DateTime(2050, 1, 22, 23, 59, 59, 999).Ticks + 9999LL);
static const int yinfo[][4] = { { 6, 1, 28, 44368 }, { 0, 2, 15, 43856 }, { 0, 2, 5, 19808 }, { 4, 1, 25, 42352 }, { 0, 2, 13, 42352 }, { 0, 2, 2, 21104 }, { 3, 1, 22, 26928 }, { 0, 2, 9, 55632 }, { 7, 1, 30, 27304 }, { 0, 2, 17, 22176 }, { 0, 2, 6, 39632 }, { 5, 1, 27, 19176 }, { 0, 2, 15, 19168 }, { 0, 2, 3, 42208 }, { 4, 1, 23, 53864 }, { 0, 2, 11, 53840 }, { 8, 1, 31, 54600 }, { 0, 2, 18, 46400 }, { 0, 2, 7, 54944 }, { 6, 1, 28, 38608 }, { 0, 2, 16, 38320 }, { 0, 2, 5, 18864 }, { 4, 1, 25, 42200 }, { 0, 2, 13, 42160 }, { 10, 2, 2, 45656 }, { 0, 2, 20, 27216 }, { 0, 2, 9, 27968 }, { 6, 1, 29, 46504 }, { 0, 2, 18, 11104 }, { 0, 2, 6, 38320 }, { 5, 1, 27, 18872 }, { 0, 2, 15, 18800 }, { 0, 2, 4, 25776 }, { 3, 1, 23, 27216 }, { 0, 2, 10, 59984 }, { 8, 1, 31, 27976 }, { 0, 2, 19, 23248 }, { 0, 2, 8, 11104 }, { 5, 1, 28, 37744 }, { 0, 2, 16, 37600 }, { 0, 2, 5, 51552 }, { 4, 1, 24, 58536 }, { 0, 2, 12, 54432 }, { 0, 2, 1, 55888 }, { 2, 1, 22, 23208 }, { 0, 2, 9, 22208 }, { 7, 1, 29, 43736 }, { 0, 2, 18, 9680 }, { 0, 2, 7, 37584 }, { 5, 1, 26, 51544 }, { 0, 2, 14, 43344 }, { 0, 2, 3, 46240 }, { 3, 1, 23, 47696 }, { 0, 2, 10, 46416 }, { 9, 1, 31, 21928 }, { 0, 2, 19, 19360 }, { 0, 2, 8, 42416 }, { 5, 1, 28, 21176 }, { 0, 2, 16, 21168 }, { 0, 2, 5, 43344 }, { 4, 1, 25, 46248 }, { 0, 2, 12, 27296 }, { 0, 2, 1, 44368 }, { 2, 1, 22, 21928 }, { 0, 2, 10, 19296 }, { 6, 1, 29, 42352 }, { 0, 2, 17, 42352 }, { 0, 2, 7, 21104 }, { 5, 1, 27, 26928 }, { 0, 2, 13, 55600 }, { 0, 2, 3, 23200 }, { 3, 1, 23, 43856 }, { 0, 2, 11, 38608 }, { 11, 1, 31, 19176 }, { 0, 2, 19, 19168 }, { 0, 2, 8, 42192 }, { 6, 1, 28, 53864 }, { 0, 2, 15, 53840 }, { 0, 2, 4, 54560 }, { 5, 1, 24, 55968 }, { 0, 2, 12, 46752 }, { 0, 2, 1, 38608 }, { 2, 1, 22, 19160 }, { 0, 2, 10, 18864 }, { 7, 1, 30, 42168 }, { 0, 2, 17, 42160 }, { 0, 2, 6, 45648 }, { 5, 1, 26, 46376 }, { 0, 2, 14, 27968 }, { 0, 2, 2, 44448 } };

static std::vector<EraInfo> TrimEras(const std::vector<EraInfo>& baseEras)
{
	std::vector<EraInfo> eraInfoArray = std::vector<EraInfo>();
	int num = 0;
	for (int i = 0; i < (int)baseEras.size(); i++)
	{
		if (baseEras[i].yearOffset + baseEras[i].minEraYear < 2049)
		{
			if (baseEras[i].yearOffset + baseEras[i].maxEraYear < 1960)
			{
				break;
			}
			eraInfoArray.push_back(baseEras[i]);
			num++;
		}
	}
	if (num == 0)
	{
		return baseEras;
	}
	return eraInfoArray;
}


Calendar* JapaneseLunisolarCalendar::InstanceClone() const
{
	JapaneseLunisolarCalendar* cal = new JapaneseLunisolarCalendar();
	cal->twoDigitYearMax = twoDigitYearMax;
	return cal;
}

int JapaneseLunisolarCalendar::GetBaseCalendarID()  const 
{
	return 3;
}

void JapaneseLunisolarCalendar::GetCalEraInfo(std::vector<EraInfo>& infos) const
{
	std::vector<EraInfo> ifs = JapaneseCalendar::GetEraInfo();
	for (auto& f : ifs)
	{
		infos.push_back(f);
	}
}

int JapaneseLunisolarCalendar::GetDaysInYearBeforeMinSupportedYear() const 
{
	return 354;
}

std::vector<int> JapaneseLunisolarCalendar::GetEras() const 
{
	return helper->GetEras();
}

int JapaneseLunisolarCalendar::GetID() const 
{
	return 14;
}

int JapaneseLunisolarCalendar::GetMaxCalendarYear() const 
{
	return 2049;
}

DateTime JapaneseLunisolarCalendar::GetMaxDate() const 
{
	return maxDate;
}

DateTime JapaneseLunisolarCalendar::GetMaxSupportedDateTime() const 
{
	return maxDate;
}

int JapaneseLunisolarCalendar::GetMinCalendarYear() const 
{
	return 1960;
}

DateTime JapaneseLunisolarCalendar::GetMinDate() const 
{
	return minDate;
}

DateTime JapaneseLunisolarCalendar::GetMinSupportedDateTime() const 
{
	return minDate;
}


JapaneseLunisolarCalendar::JapaneseLunisolarCalendar()
{
	helper = new GregorianCalendarHelper(this, TrimEras(JapaneseCalendar::GetEraInfo()));
}

JapaneseLunisolarCalendar::~JapaneseLunisolarCalendar()
{
	SAFE_DELETE(helper);
}

int JapaneseLunisolarCalendar::GetEra(const DateTime& time)const 
{
	return helper->GetEra(time);
}

int JapaneseLunisolarCalendar::GetGregorianYear(int year, int era)const 
{
	return helper->GetGregorianYear(year, era);
}

int JapaneseLunisolarCalendar::GetYear(int year, const DateTime& time) const 
{
	return helper->GetYear(year, time);
}

int JapaneseLunisolarCalendar::GetYearInfo(int LunarYear, int Index) const 
{
	if (LunarYear < 1960 || LunarYear > 2049)
	{
		throw std::exception("ArgumentOutOfRange_Year");
	}
	return yinfo[LunarYear - 1960][Index];
}



__uregister_class(JapaneseLunisolarCalendar, ClassFlags::Normal)
{
	type.AddBaseClass<Calendar>();

	BuiltInAssemblyFactory::Regisiter(&type);
}