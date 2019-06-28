#include "CultureInfo_impl.h"
#include <map>
#include "OSHelper.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;

static std::map<size_t, CultureInfo_impl*> CultureInfo_caches;
std::hash<std::string> CultureInfo_impl::_hashFun;

bool CultureInfo_impl::s_haveIsTaiwanSku = false;
bool CultureInfo_impl::s_isTaiwanSku = false;

const char* CultureInfo::america = "en-US";
const char* CultureInfo::hongKong = "zh-HK";
const char* CultureInfo::china = "zh-CN";
const char* CultureInfo::taiWan = "zh-TW";
const char* CultureInfo::japan = "ja-JP";
const char* CultureInfo::southkorea = "ko-KR";
const char* CultureInfo::unitedKingdom = "en-GB";
const char* CultureInfo::unitedStates = "en-US";

CultureInfo CultureInfo::Current()
{
	try
	{
		API_UINT32 cid = Microsoft::winapi_GetThreadLocale();
		int len = Microsoft::winapi_GetLocaleInfo_ANSI(cid, API_LOCALE_SNAME, NULL, 0);
		if (len > 0)
		{
			char* pstr = (char*)malloc(len * sizeof(char));
			if (Microsoft::winapi_GetLocaleInfo_ANSI(cid, API_LOCALE_SNAME, pstr, len))
			{
				CultureInfo result = CultureInfo(pstr);
				free(pstr);
				return result;
			}
		}
		return CultureInfo(CultureInfo::china);
	}
	catch (...){}
	return CultureInfo(CultureInfo::china);
}

/// <summary>
/// 获取非依赖性区域性信息
/// </summary>
CultureInfo& CultureInfo::Invariant()
{
	static CultureInfo invariant = CultureInfo(nullptr);
	return invariant;
}

CultureInfo::CultureInfo(const char* locName) 
{
	impl = nullptr;

	size_t id = CultureInfo_impl::_hashFun(std::string(locName==nullptr ? "": locName));
	auto iter = CultureInfo_caches.find(id);
	if (iter != CultureInfo_caches.end())
		impl = iter->second;
	try
	{
		if (!impl)
		{
			impl = new CultureInfo_impl(locName);
			if (impl) CultureInfo_caches.insert(std::make_pair(id, impl));
		}
	}
	catch (...)
	{
		SAFE_DELETE(impl);
		throw;
	}
}

bool CultureInfo::Is(const char* locname) const
{
	if (!impl) return false;
	return StringHelper::Compare(impl->_locale.name(), std::string(locname), StringComparison::InvariantCulture) == 0;
}

CultureInfo::CultureInfo(const CultureInfo& info)
{
	impl = info.impl;
}

CultureInfo::CultureInfo(CultureInfo&& info)
{
	impl = info.impl;
}

CultureInfo::~CultureInfo()
{

}

CultureInfo & System::CultureInfo::operator=(const CultureInfo & info)
{
	impl = info.impl;
	return *this;
}

__forceinline static void CheckImpl(CultureInfo_impl* impl)
{
	if (!OSHelper::IsValidAddress(impl, sizeof(CultureInfo_impl), false)) throw std::exception("critical cultureInfo error");
}

const wchar_t* CultureInfo::GetWidenName() const
{
	return impl != nullptr ? impl->_name.c_str() : nullptr;
}

const char* CultureInfo::GetName() const
{
	return impl != nullptr ? impl->_locale.name().c_str() : nullptr;
}

const std::locale CultureInfo::GetLocale() const
{
	CheckImpl(impl);
	return impl->_locale;
}

const std::locale CultureInfo::GetLocale_Group() const
{
	CheckImpl(impl);
	return impl->_locale_group;
}

const _locale_t CultureInfo::GetLocale_t() const
{
	return  impl != nullptr ? impl->_localeplus : nullptr;
}

const System::Calendar* CultureInfo::GetCalendar() const
{
	return  impl != nullptr ? impl->_defaultCal : nullptr;
}

const char CultureInfo::GetMoneyConst(CultureInfoMoneyConsts moneyconst) const
{
	CheckImpl(impl);
	return impl->_data->GetMoneyConst(moneyconst);
}

unsigned int CultureInfo::GetPatternCount(CultureInfoPatterns pattern) const
{
	try
	{
		CheckImpl(impl);
		return impl->_data->GetPatternCount(pattern);
	}
	catch (...){}
	return 0;
}

const wchar_t* CultureInfo::GetPattern(CultureInfoPatterns pattern, unsigned int index) const
{
	try
	{
		CheckImpl(impl);
		return impl->_data->GetPattern(pattern, index);
	}
	catch (...){}
	return nullptr;
}

unsigned int CultureInfo::GetConstsNameCount(CultureInfoConstNames name) const
{
	try
	{
		CheckImpl(impl);
		return impl->_data->GetConstsNameCount(name);
	}
	catch (...){}
	return 0;

}

const wchar_t* CultureInfo::GetConstsName(CultureInfoConstNames name, unsigned int index) const
{
	try
	{
		CheckImpl(impl);
		return impl->_data->GetConstsName(name, index);
	}
	catch (...){}
	return nullptr;
}

bool CultureInfo::GetBooleanFlags(CultureInfoBooleanFlags flag) const
{
	try
	{
		CheckImpl(impl);
		return impl->_data->GetBooleanFlags(flag);
	}
	catch (...){}
	return false;
}

int CultureInfo::GetInternalValue(int id) const
{
	try
	{
		CheckImpl(impl);
		switch (id)
		{
		case 0: 
			return (int)impl->_data->formatFlags;
		case 1:
			if (impl->_data->cals.size() > 0)
			{
				auto& cal = impl->_defaultCalData;
				if (cal)
					return cal->CalId == 2 || cal->CalId == 3;
			}
			return 0;
		}
	}
	catch (...){}
	return -1;
}


std::vector<int> CultureInfo::GetOptionalCalendarIds() const
{
	CheckImpl(impl);
	std::vector<int> res;
	for (auto& cal : impl->_data->cals)
	{
		res.push_back(cal->CalId);
	}
	return res;
}


CultureInfo CultureInfo::CreateByCalendarId(int id) const
{
	CultureInfo ci = *this;
	for (auto& cal : impl->_data->cals)
	{
		if (cal->CalId == id)
		{
			ci.impl = ci.impl->CreateByCalendarId(id);
			break;
		}
	}
	return ci;
}

CultureInfo CultureInfo::CreateByCalendar(System::Calendar* newCalendar)const
{
	CultureInfo ci = *this;
	if (newCalendar)
	{
		for (auto& cal : impl->_data->cals)
		{
			if (cal->CalId == newCalendar->GetID())
			{
				ci.impl = ci.impl->CreateByCalendarId(newCalendar);
				break;
			}
		}
	}
	return ci;
}