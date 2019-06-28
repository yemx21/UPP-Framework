#include "CultureInfo_impl.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;

wchar_t NumGroup::do_thousands_sep() const { return thousands_sep; }
std::string NumGroup::do_grouping() const { return grouping; }

NumGroup::NumGroup(const std::locale& loc)
{
	grouping = std::use_facet<std::numpunct<wchar_t>>(loc).grouping();
	thousands_sep = std::use_facet<std::numpunct<wchar_t>>(loc).thousands_sep();
}

DateTokenHashValue::DateTokenHashValue(const std::wstring& tokenStr, DateTokenType tokenType, int tokenValue)
{
	this->tokenString = tokenStr;
	this->tokenType = tokenType;
	this->tokenValue = tokenValue;
}

DateTokenHashValue::DateTokenHashValue(const DateTokenHashValue& v)
{
	tokenString = v.tokenString;
	tokenType = v.tokenType;
	tokenValue = v.tokenValue;
}

void CultureInfo_impl::InsertAtCurrentHashNode(std::vector<DateTokenHashValue*>& hashTable, const std::wstring& str, wchar_t ch, DateTokenType tokenType, int tokenValue, int pos, int hashcode, int hashProbe)
{
	DateTokenHashValue* tokenHashValue = hashTable[hashcode];
	hashTable[hashcode] = new DateTokenHashValue(str, tokenType, tokenValue);
	while (true)
	{
		int num = pos + 1;
		pos = num;
		if (num >= 199)
		{
			break;
		}
		hashcode = hashcode + hashProbe;
		if (hashcode >= 199)
		{
			hashcode = hashcode - 199;
		}
		DateTokenHashValue* tokenHashValue1 = hashTable[hashcode];
		if (tokenHashValue1 == nullptr)
		{
			hashTable[hashcode] = tokenHashValue;
			if (tokenHashValue1 == nullptr)
			{
				return;
			}
			tokenHashValue = tokenHashValue1;
		}
		else if (tolower(tokenHashValue1->tokenString[0], _locale) == ch)
		{
			hashTable[hashcode] = tokenHashValue;
			if (tokenHashValue1 == nullptr)
			{
				return;
			}
			tokenHashValue = tokenHashValue1;
		}
	}
}

void CultureInfo_impl::InsertHash(std::vector<DateTokenHashValue*>& hashTable, const std::wstring& str, DateTokenType tokenType, int tokenValue)
{
	if (str.empty())
	{
		return;
	}
	int num = 0;
	if (iswspace(str[0]) || iswspace(str[str.length() - 1]))
	{
		StringHelper::Trim(str);
		if (str.empty())
		{
			return;
		}
	}
	wchar_t lower = tolower(str[0], _locale);
	int num1 = lower % L'Ç';
	int num2 = 1 + lower % L'Å';
	do
	{
		DateTokenHashValue* tokenHashValue = hashTable[num1];
		if (tokenHashValue == nullptr)
		{
			hashTable[num1] = new DateTokenHashValue(str, tokenType, tokenValue);
			return;
		}
		if (str.length() >= tokenHashValue->tokenString.length() && StringHelper::Compare(str, 0, tokenHashValue->tokenString, 0, tokenHashValue->tokenString.length(), _localeplus, true) == 0)
		{
			if (str.length() > tokenHashValue->tokenString.length())
			{
				InsertAtCurrentHashNode(hashTable, str, lower, tokenType, tokenValue, num, num1, num2);
				return;
			}
			int num3 = (int)tokenType;
			int num4 = (int)tokenHashValue->tokenType;
			if ((num4 & 255) == 0 && (num3 & 255) != 0 || (num4 & 65280) == 0 && (num3 & 65280) != 0)
			{
				DateTokenHashValue* tokenHashValue2 = tokenHashValue;
				tokenHashValue2->tokenType = tokenHashValue2->tokenType | tokenType;
				if (tokenValue != 0)
				{
					tokenHashValue->tokenValue = tokenValue;
				}
			}
		}
		num++;
		num1 = num1 + num2;
		if (num1 < 199)
		{
			continue;
		}
		num1 = num1 - 199;
	} while (num < 199);
}

void CultureInfo_impl::InsertHash(std::vector<DateTokenHashValue*>& hashTable, const wchar_t* str, DateTokenType tokenType, int tokenValue)
{
	InsertHash(hashTable, str != nullptr ? std::wstring(str) : std::wstring(), tokenType, tokenValue);
}

void CultureInfo_impl::AddMonthNames(std::vector<DateTokenHashValue*>& temp, const std::wstring& monthPostfix)
{
	for (int i = 1; i <= 13; i++)
	{
		std::wstring monthName = _data->GetConstsName((CultureInfoConstNames)(13 + i));
		if (monthName.length() > 0)
		{
			if (monthPostfix.empty())
			{
				InsertHash(temp, monthName, DateTokenType::MonthToken, i);
			}
			else
			{
				InsertHash(temp, (monthName + monthPostfix), DateTokenType::MonthToken, i);
			}
		}
		monthName = _data->GetConstsName((CultureInfoConstNames)(25 + i));
		InsertHash(temp, monthName, DateTokenType::MonthToken, i);
	}
}

void CultureInfo_impl::CreateTokenHashTable()
{
	if (_dateTokens.size() == 0)
	{
		_dateTokens.resize(199);
		bool flag = _wcsicmp(_data->langName.c_str(), L"ko") == 0;
		std::wstring str = StringHelper::Trim(std::wstring(_data->timeSeparator));
		if (L"," != str)
		{
			InsertHash(_dateTokens, L",", DateTokenType::IgnorableSymbol, 0);
		}
		if (L"." != str)
		{
			InsertHash(_dateTokens, L".", DateTokenType::IgnorableSymbol, 0);
		}
		if (L"시" != str && L"時" != str && L"时" != str)
		{
			InsertHash(_dateTokens, _data->timeSeparator, DateTokenType::SEP_Time, 0);
		}
		InsertHash(_dateTokens, _data->amDesignator, DateTokenType::NumberToken | DateTokenType::YearNumberToken | DateTokenType::Am | DateTokenType::SEP_Am, 0);
		InsertHash(_dateTokens, _data->pmDesignator, DateTokenType::Pm | DateTokenType::SEP_Unk | DateTokenType::SEP_Am | DateTokenType::SEP_Pm, 1);
		if (_wcsicmp(_data->langName.c_str(), L"sq") == 0) //Albanian  阿尔巴尼亚
		{
			InsertHash(_dateTokens, (L"." + _data->amDesignator), DateTokenType::NumberToken | DateTokenType::YearNumberToken | DateTokenType::Am | DateTokenType::SEP_Am, 0);
			InsertHash(_dateTokens, (L"." + _data->pmDesignator), DateTokenType::Pm | DateTokenType::SEP_Unk | DateTokenType::SEP_Am | DateTokenType::SEP_Pm, 1);
		}
		InsertHash(_dateTokens, L"年", DateTokenType::SEP_YearSuff, 0);
		InsertHash(_dateTokens, L"년", DateTokenType::SEP_YearSuff, 0);
		InsertHash(_dateTokens, L"月", DateTokenType::SEP_MonthSuff, 0);
		InsertHash(_dateTokens, L"월", DateTokenType::SEP_MonthSuff, 0);
		InsertHash(_dateTokens, L"日", DateTokenType::SEP_DaySuff, 0);
		InsertHash(_dateTokens, L"일", DateTokenType::SEP_DaySuff, 0);
		InsertHash(_dateTokens, L"時", DateTokenType::SEP_HourSuff, 0);
		InsertHash(_dateTokens, L"时", DateTokenType::SEP_HourSuff, 0);
		InsertHash(_dateTokens, L"分", DateTokenType::SEP_MinuteSuff, 0);
		InsertHash(_dateTokens, L"秒", DateTokenType::SEP_SecondSuff, 0);
		if (flag)
		{
			InsertHash(_dateTokens, L"시", DateTokenType::SEP_HourSuff, 0);
			InsertHash(_dateTokens, L"분", DateTokenType::SEP_MinuteSuff, 0);
			InsertHash(_dateTokens, L"초", DateTokenType::SEP_SecondSuff, 0);
		}

		if (_wcsicmp(_data->langName.c_str(), L"ky") != 0) //吉尔吉斯语
		{
			InsertHash(_dateTokens, L"-", DateTokenType::SEP_DateOrOffset, 0);
		}
		else
		{
			InsertHash(_dateTokens, L"-", DateTokenType::IgnorableSymbol, 0);
		}

		wstringvector strArrays = _data->GetDateWords();
		DateTimeFormatFlags formatFlags = (DateTimeFormatFlags)_data->formatFlags;
		bool flag1 = false;
		if (strArrays.size() > 0)
		{
			for (int i = 0; i < (int)strArrays.size(); i++)
			{
				switch (strArrays[i][0])
				{
				case L'\xE000':
				{
					AddMonthNames(_dateTokens, strArrays[i].substr(1));
					break;
				}
				case L'\xE001':
				{
					std::wstring str1 = strArrays[i].substr(1);
					InsertHash(_dateTokens, str1, DateTokenType::IgnorableSymbol, 0);

					std::wstring tmp = StringHelper::Trim(_data->dateSeparator);

					if (StringHelper::Compare(tmp, str1, StringComparison::InvariantCultureIgnoreCase) != 0)
					{
						break;
					}
					flag1 = true;
					break;
				}
				default:
				{
					InsertHash(_dateTokens, strArrays[i], DateTokenType::DateWordToken, 0);
					if (_wcsicmp(_data->langName.c_str(), L"eu") != 0)
					{
						break;
					}
					InsertHash(_dateTokens, (L"." + strArrays[i]), DateTokenType::DateWordToken, 0);
					break;
				}
				}
			}
		}
		if (!flag1)
		{
			InsertHash(_dateTokens, _data->dateSeparator, DateTokenType::SEP_Date, 0);
		}
		AddMonthNames(_dateTokens, std::wstring());
		for (int j = 1; j <= 13; j++)
		{
			InsertHash(_dateTokens, _data->GetConstsName((CultureInfoConstNames)(25 + j)), DateTokenType::MonthToken, j);
		}
		if ((formatFlags & DateTimeFormatFlags::UseGenitiveMonth) != DateTimeFormatFlags::None)
		{
			for (int k = 1; k <= 13; k++)
			{
				InsertHash(_dateTokens, _data->GetConstsName((CultureInfoConstNames)(37 + k)), DateTokenType::MonthToken, k);
			}
		}
		if ((formatFlags & DateTimeFormatFlags::UseLeapYearMonth) != DateTimeFormatFlags::None)
		{
			for (int l = 1; l <= 13; l++)
			{
				InsertHash(_dateTokens, _data->GetConstsName((CultureInfoConstNames)(61 + l)), DateTokenType::MonthToken, l);
			}
		}
		for (int m = 0; m < 7; m++)
		{
			InsertHash(_dateTokens, _data->GetConstsName((CultureInfoConstNames)m), DateTokenType::DayOfWeekToken, m);
			InsertHash(_dateTokens, _data->GetConstsName((CultureInfoConstNames)(7 + m)), DateTokenType::DayOfWeekToken, m);
		}

		std::vector<int> eras = _defaultCal->GetEras();
		for (int n = 1; n <= (int)eras.size(); n++)
		{
			InsertHash(_dateTokens, _data->GetConstsName(CultureInfoConstNames::Era, n - 1), DateTokenType::EraToken, n);
			InsertHash(_dateTokens, _data->GetConstsName(CultureInfoConstNames::Abbreviated_Era, n - 1), DateTokenType::EraToken, n);
		}

		if (_wcsicmp(_data->langName.c_str(), L"ja") == 0)
		{
			for (int o = 0; o < 7; o++)
			{
				InsertHash(_dateTokens, _data->GetConstsName((CultureInfoConstNames)(7 + o)), DateTokenType::DayOfWeekToken, o);
			}
			if (_defaultCal->Is(_typeof(JapaneseCalendar)))
			{
				try
				{
					CultureInfo jaci = CultureInfo(CultureInfo::japan);

					int n = (int)jaci.GetConstsNameCount(CultureInfoConstNames::Era);
					for (int i = 0; i < n; i++)
					{
						InsertHash(_dateTokens, jaci.GetConstsName(CultureInfoConstNames::Era, i), DateTokenType::JapaneseEraToken, i + 1);
					}

					n = (int)jaci.GetConstsNameCount(CultureInfoConstNames::Abbreviated_Era);
					for (int i = 0; i < n; i++)
					{
						InsertHash(_dateTokens, jaci.GetConstsName(CultureInfoConstNames::Abbreviated_Era, i), DateTokenType::JapaneseEraToken, i + 1);
					}

					n = (int)jaci.GetConstsNameCount(CultureInfoConstNames::Abbreviated_EnglishEra);
					for (int i = 0; i < n; i++)
					{
						InsertHash(_dateTokens, jaci.GetConstsName(CultureInfoConstNames::Abbreviated_EnglishEra, i), DateTokenType::JapaneseEraToken, i + 1);
					}
				}
				catch (...) {}
			}
		}
		else if (_wcsicmp(_data->culName.c_str(), L"zh-TW") == 0)
		{
			try
			{
				CultureInfo jaci = CultureInfo(CultureInfo::taiWan);

				int n = (int)jaci.GetConstsNameCount(CultureInfoConstNames::Era);
				for (int i = 0; i < n; i++)
				{
					InsertHash(_dateTokens, jaci.GetConstsName(CultureInfoConstNames::Era, i), DateTokenType::JapaneseEraToken, i + 1);
				}
			}
			catch (...) {}
		}

		CultureData* icd = CultureData::Create(nullptr);

		InsertHash(_dateTokens, icd->amDesignator, DateTokenType::NumberToken | DateTokenType::YearNumberToken | DateTokenType::Am | DateTokenType::SEP_Am, 0);
		InsertHash(_dateTokens, icd->pmDesignator, DateTokenType::Pm | DateTokenType::SEP_Unk | DateTokenType::SEP_Am | DateTokenType::SEP_Pm, 1);
		for (int r = 1; r <= 12; r++)
		{
			InsertHash(_dateTokens, icd->GetConstsName((CultureInfoConstNames)(13 + r)), DateTokenType::MonthToken, r);
			InsertHash(_dateTokens, icd->GetConstsName((CultureInfoConstNames)(25 + r)), DateTokenType::MonthToken, r);
		}
		for (int s = 0; s < 7; s++)
		{
			InsertHash(_dateTokens, icd->GetConstsName((CultureInfoConstNames)s), DateTokenType::DayOfWeekToken, s);
			InsertHash(_dateTokens, icd->GetConstsName((CultureInfoConstNames)(7 + s)), DateTokenType::DayOfWeekToken, s);
		}

		int ean = (int)icd->cals[0]->saEraNames.size();
		for (int en = 0; en < ean; en++)
		{
			InsertHash(_dateTokens, icd->cals[0]->saEraNames[en], DateTokenType::EraToken, en + 1);
		}

		InsertHash(_dateTokens, L"T", DateTokenType::SEP_LocalTimeMark, 0);
		InsertHash(_dateTokens, L"GMT", DateTokenType::TimeZoneToken, 0);
		InsertHash(_dateTokens, L"Z", DateTokenType::TimeZoneToken, 0);
		InsertHash(_dateTokens, L"/", DateTokenType::SEP_Date, 0);
		InsertHash(_dateTokens, L":", DateTokenType::SEP_Time, 0);
	}
}

bool CultureInfo_impl::IsTaiwanSku()
{
	if (!s_haveIsTaiwanSku)
	{
		s_isTaiwanSku = Microsoft::winapi_GetSystemDefaultUILanguage() == 1028;
		s_haveIsTaiwanSku = true;
	}
	return s_isTaiwanSku;
}

Calendar* CultureInfo_impl::CreateCalendarInstance(int calType)
{
	switch (calType)
	{
	case 1:
	{
		return new GregorianCalendar();
	}
	case 2:
	case 9:
	case 10:
	case 11:
	case 12:
	{
		return new GregorianCalendar((GregorianCalendarTypes)calType);
	}
	case 3:
	{
		return new JapaneseCalendar();
	}
	case 4:
	{
		return new TaiwanCalendar();
	}
	case 5:
	{
		return new KoreanCalendar();
	}
	case 6:
	{
		return new HijriCalendar();
	}
	case 7:
	{
		return new ThaiBuddhistCalendar();
	}
	case 8:
	{
		return new HebrewCalendar();
	}
	case 13:
	case 16:
	case 17:
	case 18:
	case 19:
	{
		return new GregorianCalendar();
	}
	case 14:
	{
		return new JapaneseLunisolarCalendar();
	}
	case 15:
	{
		return new ChineseLunisolarCalendar();
	}
	case 20:
	{
		return new KoreanLunisolarCalendar();
	}
	case 21:
	{
		return new TaiwanLunisolarCalendar();
	}
	case 22:
	{
		return new PersianCalendar();
	}
	case 23:
	{
		return new UmAlQuraCalendar();
	}
	default:
	{
		return new GregorianCalendar();
	}
	}
}

CultureInfo_impl::CultureInfo_impl(const char* locName)
{
	isInvariant = false;
	try
	{
		if (locName != nullptr)
		{
			_locale = std::locale(locName);
			_localeplus = _create_locale(LC_ALL, locName);
			_locale_group = std::locale(_locale, new NumGroup(_locale));
			_name = Converter::Convert(_locale.name().c_str(), _locale, _localeplus);

			_lcid = Microsoft::winapi_LocaleNameToLCID(_name.c_str(), 0);
			_data = CultureData::Create(this);

			if (_data->cals.size() > 0)
				_defaultCal = CreateCalendarInstance(_data->cals[0]->CalId);
			else
				_defaultCal = CreateCalendarInstance(1);

			_calId = _defaultCal->GetID();
			_defaultCalData = _data->cals[0];
		}
		else
		{
			isInvariant = true;
			_locale = std::locale::classic();
			_localeplus = _create_locale(LC_ALL, "ivl");
			_locale_group = std::locale(_locale, new NumGroup(_locale));
			_name = L"";
			_lcid = Microsoft::winapi_LocaleNameToLCID(API_LOCALE_NAME_INVARIANT, 0);
			_data = CultureData::Create(nullptr);

			if (_data->cals.size() > 0)
				_defaultCal = CreateCalendarInstance(_data->cals[0]->CalId);
			else
				_defaultCal = CreateCalendarInstance(1);
			_calId = _defaultCal->GetID();
			_defaultCalData = _data->cals[0];
		}
		CreateTokenHashTable();
	}
	catch (...)
	{
		throw std::exception("invalid locName");
	}
}

CultureInfo_impl::CultureInfo_impl(const char* locName, bool noCalInit)
{
	isInvariant = false;
	try
	{
		if (locName != nullptr)
		{
			_locale = std::locale(locName);
			_localeplus = _create_locale(LC_ALL, locName);
			_locale_group = std::locale(_locale, new NumGroup(_locale));
			_name = Converter::Convert(_locale.name().c_str(), _locale, _localeplus);

			_lcid = Microsoft::winapi_LocaleNameToLCID(_name.c_str(), 0);
			if (!noCalInit)
			{
				_data = CultureData::Create(this);
				if (_data->cals.size() > 0)
					_defaultCal = CreateCalendarInstance(_data->cals[0]->CalId);
				else
					_defaultCal = CreateCalendarInstance(1);

				_calId = _defaultCal->GetID();
			}
			else
			{
				_data = nullptr;
				_defaultCal = nullptr;
				_defaultCalData = nullptr;
			}
		}
		else
		{
			isInvariant = true;
			_locale = std::locale::classic();
			_localeplus = _create_locale(LC_ALL, "ivl");
			_locale_group = std::locale(_locale, new NumGroup(_locale));
			_name = L"";
			_lcid = Microsoft::winapi_LocaleNameToLCID(API_LOCALE_NAME_INVARIANT, 0);
			if (!noCalInit)
			{
				_data = CultureData::Create(nullptr);
				if (_data->cals.size() > 0)
					_defaultCal = CreateCalendarInstance(_data->cals[0]->CalId);
				else
					_defaultCal = CreateCalendarInstance(1);
				_calId = _defaultCal->GetID();
			}
			else
			{
				_data = nullptr;
				_defaultCal = nullptr;
				_defaultCalData = nullptr;
			}
		}
		CreateTokenHashTable();
	}
	catch (...)
	{
		throw std::exception("invalid locName");
	}
}

CultureInfo_impl::~CultureInfo_impl()
{
	_free_locale(_localeplus);
	for (auto& ds : _dateTokens)
	{
		SAFE_DELETE(ds);
	}
	_dateTokens.clear();
	if (!isInvariant) SAFE_DELETE(_data);
	SAFE_DELETE(_defaultCal);
}

CultureInfo_impl* CultureInfo_impl::CreateByCalendarId(int id) const
{
	auto iter = std::find_if(_caches.begin(), _caches.end(), CultureInfo_impl_Compare0(id));
	if (iter != _caches.end()) return *iter;
	CultureInfo_impl* impl = new CultureInfo_impl(isInvariant ? nullptr : _locale.name().c_str(), true);
	if (id != _calId)
	{
		for (auto& cal : impl->_data->cals)
		{
			if (id == cal->CalId)
			{
				impl->_data = CultureData::Create(impl, id);
				impl->_calId = id;
				impl->_defaultCal = CreateCalendarInstance(id);
				impl->_defaultCalData = cal;
				_caches.push_back(impl);
				return impl;
			}
		}
	}
	return const_cast<CultureInfo_impl*>(this);
}

CultureInfo_impl* CultureInfo_impl::CreateByCalendarId(Calendar* ca) const
{
	if (!ca)
	{
		auto iter = std::find_if(_caches.begin(), _caches.end(), CultureInfo_impl_Compare1(ca));
		if (iter != _caches.end()) return *iter;
		CultureInfo_impl* impl = new CultureInfo_impl(isInvariant ? nullptr : _locale.name().c_str(), true);
		int id = ca->GetID();
		for (auto& cal : impl->_data->cals)
		{
			if (id == cal->CalId)
			{
				impl->_data = CultureData::Create(impl, id);
				impl->_calId = id;
				impl->_defaultCal = ca;
				impl->_defaultCalData = cal;
				_caches.push_back(impl);
				return impl;
			}
		}
	}
	return const_cast<CultureInfo_impl*>(this);
}

bool CultureInfo_impl::YearMonthAdjustment(const CultureInfo& loc, int& year, int& month, bool parsedMonthName)
{
	if (((DateTimeFormatFlags)loc.impl->_data->formatFlags & DateTimeFormatFlags::UseHebrewRule) != DateTimeFormatFlags::None)
	{
		if (year < 1000)
		{
			year = year + 5000;
		}
		if (year < loc.Calendar->GetYear(loc.Calendar->GetMinSupportedDateTime()) || year > loc.Calendar->GetYear(loc.Calendar->GetMaxSupportedDateTime()))
		{
			return false;
		}
		if (parsedMonthName && !loc.Calendar->IsLeapYear(year))
		{
			if (month >= 8)
			{
				month = month - 1;
			}
			else if (month == 7)
			{
				return false;
			}
		}
	}
	return true;
}

CultureInfo_impl* CultureInfo_impl::GetImpl(const CultureInfo& loc)
{
	return loc.impl;
}