#include "Types.h"
#include "Utils.h"
#include <vector>
using namespace uppcl;

#pragma region UVersion

UVersion::UVersion(int major, int minor, int build, int revision) :_Major(major), _Minor(minor), _Build(build), _Revision(revision)
{

}

UVersion::UVersion(int major, int minor, int build) : _Major(major), _Minor(minor), _Build(build), _Revision(-1)
{

}

UVersion::UVersion(int major, int minor) : _Major(major), _Minor(minor), _Build(-1), _Revision(-1)
{

}

UVersion::UVersion() : _Major(0), _Minor(0), _Build(-1), _Revision(-1)
{

}

UVersion::UVersion(const UVersion& ver)
{
	_Major = ver._Major;
	_Minor = ver._Minor;
	_Build = ver._Build;
	_Revision = ver._Revision;
}

int UVersion::GetMajor() const
{
	return _Major;
}
int UVersion::GetMinor() const
{
	return _Minor;
}

int UVersion::GetBuild() const
{
	return _Build;
}

int UVersion::GetRevision() const
{
	return _Revision;
}

short UVersion::GetMajorRevision() const
{
	return (short)(_Revision >> 16);
}

short UVersion::GetMinorRevision() const
{
	return (short)(_Revision & 65535);
}

void UVersion::SetMajor(int major)
{
	_Major = major;
}

void UVersion::SetMinor(int minor)
{
	_Minor = minor;
}

void UVersion::SetBuild(int build)
{
	_Build = build;
}

void UVersion::SetRevision(int revision)
{
	_Revision = revision;
}

bool UVersion::GetIsEmpty() const
{
	return _Build == 0 && _Revision == 0 && _Major == 0 && _Minor == 0;
}


bool UVersion::operator == (UVersion& ver) const
{
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool UVersion::operator == (const UVersion& ver) const
{
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool UVersion::operator != (UVersion& ver) const
{
	return _Major != ver._Major || _Minor != ver._Minor || _Build != ver._Build || _Revision != ver._Revision;
}


bool UVersion::operator != (const UVersion& ver) const
{
	return _Major != ver._Major || _Minor != ver._Minor || _Build != ver._Build || _Revision != ver._Revision;
}


bool UVersion::operator > (UVersion& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}


bool UVersion::operator > (const UVersion& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}

bool UVersion::operator < (UVersion& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}


bool UVersion::operator < (const UVersion& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}


bool UVersion::operator >= (UVersion& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	if (_Revision > ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}

bool UVersion::operator >= (const UVersion& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	if (_Revision > ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool UVersion::operator <= (UVersion& ver) const
{
	if (_Major < ver._Major) return true;
	if (_Minor < ver._Minor) return true;
	if (_Build < ver._Build) return true;
	if (_Revision < ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool UVersion::operator <= (const UVersion& ver) const
{
	if (_Major < ver._Major) return true;
	if (_Minor < ver._Minor) return true;
	if (_Build < ver._Build) return true;
	if (_Revision < ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool UVersion::TryParse(const std::string& input, UVersion& result)
{
	int num;
	int num1;
	int num2;
	int num3;

	std::vector<std::string> strArrays;
	utils::Split(input, std::string("."), strArrays);

	int length = (int)strArrays.size();
	if (length < 2 || length > 4) return false;

	if (!utils::TryParseInt32(strArrays[0], num)) return false;
	if (!utils::TryParseInt32(strArrays[1], num1)) return false;

	length = length - 2;

	if (length <= 0)
		result = UVersion(num, num1);
	else
	{
		if (!utils::TryParseInt32(strArrays[2], num2)) return false;
		length--;
		if (length <= 0)
		{
			result = UVersion(num, num1, num2);
		}
		else
		{
			if (!utils::TryParseInt32(strArrays[3], num3)) return false;
			result = UVersion(num, num1, num2, num3);
		}
	}
	return true;
}

std::string UVersion::ToString(const char* fmt) const
{
	std::string wfmt = fmt ? std::string(fmt) : std::string();

	int fc = 4;
	if (wfmt.size() == 3)
	{
		if (wfmt[0] == 'F' && wfmt[1] == 'C')
		{
			switch (wfmt[3])
			{
			case '2':
				fc = 2;
				break;
			case '3':
				fc = 3;
				break;
			}
		}
	}
	else
	{
		if (_Build < 0)
			fc = 2;
		else if (_Revision < 0)
			fc = 3;
	}


	switch (fc)
	{
	case 1:
		return std::to_string(_Major);
	case 2:
		return std::to_string(_Major) + "." + std::to_string(_Minor);
	case 3:
		return _Build < 0 ? std::to_string(_Major) + "." + std::to_string(_Minor) : std::to_string(_Major) + "." + std::to_string(_Minor) + "." + std::to_string(_Build);
	case 4:
		return _Revision < 0 ? std::to_string(_Major) + "." + std::to_string(_Minor) + "." + std::to_string(_Build) : std::to_string(_Major) + "." + std::to_string(_Minor) + "." + std::to_string(_Build) + "." + std::to_string(_Revision);
	case 0:
	default:
		return std::string();
	}
	return std::string();
}

#pragma endregion

#pragma region UAssemblyDef
UAssemblyDef::UAssemblyDef(const std::string& ass, const UVersion& version, GUID& guid)
{
	name = ass;
	ver = version;
	memcpy_s(&pbk, sizeof(GUID), &guid, sizeof(GUID));
}

UAssemblyDef::UAssemblyDef(const UAssemblyDef& def)
{
	memcpy_s(&pbk, sizeof(GUID), &def.pbk, sizeof(GUID));
	ver = def.ver;
	name = def.name;
}

const std::string& UAssemblyDef::PublicKey()
{
	char output[84];
	_snprintf_s(output, sizeof(output), "{ 0x%08x, 0x%04hx, 0x%04hx, { 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x } }", pbk.Data1, pbk.Data2, pbk.Data3, pbk.Data4[0], pbk.Data4[1], pbk.Data4[2], pbk.Data4[3], pbk.Data4[4], pbk.Data4[5], pbk.Data4[6], pbk.Data4[7]);
	return std::string(output);
}

const std::string& UAssemblyDef::VersionStr() 
{ 
	return ver.ToString(); 
}
const UVersion& UAssemblyDef::Ver()
{ 
	return ver; 
}
const std::string& UAssemblyDef::Name()
{ 
	return name;
}

UAssemblyDef* UAssemblyDef::Create(const std::string& ass, const std::string& ver_str, const std::string& guid_str)
{
	UVersion  ver;
	if (UVersion::TryParse(ver_str, ver))
	{
		_GUID pbk = {};
		if (guid_str.empty())
			return new UAssemblyDef(ass, ver, pbk);
		else
		{
			unsigned long p0;
			int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
			int err = sscanf_s(guid_str.c_str(), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);
			if (err == 11)
			{
				pbk.Data1 = p0;
				pbk.Data2 = p1;
				pbk.Data3 = p2;
				pbk.Data4[0] = p3;
				pbk.Data4[1] = p4;
				pbk.Data4[2] = p5;
				pbk.Data4[3] = p6;
				pbk.Data4[4] = p7;
				pbk.Data4[5] = p8;
				pbk.Data4[6] = p9;
				pbk.Data4[7] = p10;
				return new UAssemblyDef(ass, ver, pbk);
			}
		}
	}
	return nullptr;
}

#pragma endregion

#pragma region UAssemblyDefMap
UAssemblyDefMap::UAssemblyDefMap()
{

}

UAssemblyDefMap::~UAssemblyDefMap()
{
	for (auto iter = defs.begin(); iter != defs.end();)
	{
		UAssemblyDef* def = *iter;
		if (def)
		{
			delete def;
			def = nullptr;
		}
		iter = defs.erase(iter);
	}
}

void UAssemblyDefMap::Parse(const std::string& str)
{
	std::vector<std::string> params;
	utils::Split(str, std::string(";"), params);
	if (!params.empty())
	{
		for (const std::string& p : params)
		{
			std::string ass, ver, pbk;
			std::vector<std::string> splits;
			utils::Split(p, std::string(","), splits);
			if (!splits.empty())
			{
				for (const std::string& s : splits)
				{
					std::string rs = utils::Trim(s);
					std::vector<std::string> ss;
					utils::Split(rs, std::string("="), ss);
					if (ss.size() == 2)
					{
						llvm::StringRef label_r = ss[0];
						if (label_r.startswith_lower("asm"))
						{
							ass = utils::Trim(ss[1]);
						}
						else if (label_r.startswith_lower("ver"))
						{
							ver = utils::Trim(ss[1]);
						}
						else if (label_r.startswith_lower("pbk"))
						{
							pbk = utils::Trim(ss[1]);
						}
					}
				}
			}
			if (!ass.empty() && !ver.empty())
			{

				//std::cout << "try to create UAssemblyDef: asm=" << ass << "version=" << ver << "publickey=" << pbk << std::endl;
				UAssemblyDef* def = UAssemblyDef::Create(ass, ver, pbk);
				auto iter = std::find_if(defs.begin(), defs.end(), assdef_map_finder_t(def));
				if (iter == defs.end())
				{
					defs.push_back(def);
				}
			}
		}
	}
}

UAssemblyDef* UAssemblyDefMap::Query(const std::string& name, const UVersion& ver)
{

	auto iter = std::find_if(defs.begin(), defs.end(), assdef_map_finder1_t(name, ver));
	if (iter != defs.end())
	{
		return *iter;
	}
	return nullptr;
}

std::vector<UVersion> UAssemblyDefMap::Query(const std::string& name)
{
	std::vector<UVersion> res;
	for (UAssemblyDef* def : defs)
	{
		if (def->name == name)
			res.push_back(def->ver);
	}
	return res;
}

unsigned int UAssemblyDefMap::Count() const
{
	return defs.size();
}

#pragma endregion

#pragma region UNamespace

UNamespace::UNamespace(const std::string& name, const std::string fullname) :Name(name), FullName(fullname)
{

}

UNamespace::UNamespace(const UNamespace& ns)
{
	Name = ns.Name;
	Children = ns.Children;
	FullName = ns.FullName;
}

void UNamespace::Add(const UNamespace& ns)
{
	Children.push_back(ns);
}

UNamespace& UNamespace::Get(size_t index)
{
	return Children.at(index);
}

UNamespace& UNamespace::Get(std::vector<UNamespace>::iterator iter)
{
	return *iter;
}

bool UNamespace::operator ==(const std::string& na)
{
	return na.compare(Name) == 0;
}

bool UNamespace::operator !=(const std::string& na)
{
	return na.compare(Name) != 0;
}

void UNamespace::GetAllNamespace(std::set<std::string>& nss, bool full)
{
	nss.insert(full ? FullName : Name);
	for (auto& ns : Children)
	{
		ns.GetAllNamespace(nss);
	}
}
#pragma endregion

#pragma region UAssembly
void UAssembly::AddNamespace(std::queue<std::string>& nss, UNamespace& par)
{
	if (!nss.empty())
	{
		std::string cur = nss.front();
		if (cur.empty() || std::all_of(cur.begin(), cur.end(), isspace))
		{
			nss.pop();
			AddNamespace(nss, par);
			return;
		}
		auto iter = std::find(par.Children.begin(), par.Children.end(), cur);
		if (iter != par.Children.end())
		{
			nss.pop();
			AddNamespace(nss, par.Get(iter));
		}
		else
		{
			par.Add(UNamespace(cur, par.FullName + "_" + cur));
			nss.pop();
			AddNamespace(nss, par.Get(par.Children.size() - 1));
		}
	}
}

void UAssembly::AddNamespace(std::queue<std::string>& nss)
{
	if (!nss.empty())
	{
		std::string cur = nss.front();
		if (cur.empty() || std::all_of(cur.begin(), cur.end(), isspace))
		{
			nss.pop();
			AddNamespace(nss);
			return;
		}
		auto iter = std::find(Children.begin(), Children.end(), cur);
		if (iter != Children.end())
		{
			nss.pop();
			AddNamespace(nss, *iter);
		}
		else
		{
			Children.push_back(UNamespace(cur, cur));
			nss.pop();
			AddNamespace(nss, Children[Children.size() - 1]);
		}
	}
}

UAssembly::UAssembly(const UAssembly& ns)
{
	Name = ns.Name;
	Ver = ns.Ver;
	Ver_str = ns.Ver_str;
	Children = ns.Children;
	SplitedVersion = ns.SplitedVersion;
}

UAssembly::UAssembly(const std::string& name, const std::string& ver) :Name(name), Ver_str(ver)
{
	UVersion::TryParse(ver, Ver);
	SplitedVersion = ver;
	SplitedVersion = utils::ReplaceAllDistinct(SplitedVersion, std::string("."), std::string("_"));
}

UAssembly::UAssembly(const std::string& name, const UVersion& ver) :Name(name), Ver(ver)
{
	Ver_str = ver.ToString();
	SplitedVersion = Ver_str;
	SplitedVersion = utils::ReplaceAllDistinct(SplitedVersion, std::string("."), std::string("_"));
}

void UAssembly::AddNamespace(const std::string& name)
{
	std::queue<std::string> nss;
	utils::Split(name, std::string("::"), nss);
	AddNamespace(nss);
}

void UAssembly::GetAllNamespace(std::set<std::string>& nss, bool full)
{
	for (auto& ns : Children)
	{
		ns.GetAllNamespace(nss, full);
	}
}

#pragma endregion

#pragma region UAssemblyMap
UAssemblyMap::UAssemblyMap(const std::string& name) : Name(name) {}

UAssemblyMap::UAssemblyMap(const UAssemblyMap& map)
{
	Name = map.Name;
	Children = map.Children;
}

void UAssemblyMap::AddNamespace(const std::string& ns, const std::string& ver)
{
	auto iter = std::find_if(Children.begin(), Children.end(), ass_finder_t(ver));
	if (iter != Children.end())
	{
		iter->AddNamespace(ns);
	}
	else
	{
		Children.push_back(UAssembly(Name, ver));
		Children[Children.size() - 1].AddNamespace(ns);
	}
}

void UAssemblyMap::GetAllNamespace(std::set<std::string>& nss, bool full)
{
	for (auto& ass : Children)
	{
		ass.GetAllNamespace(nss, full);
	}
}

#pragma endregion

#pragma region UAssemblyPool
UAssemblyPool::UAssemblyPool()
{

}

void UAssemblyPool::Parse(const std::string& def, const std::string& exclude)
{
	Defs.Parse(def);
	Excludes.Parse(exclude);
}

void UAssemblyPool::Add(const std::string& ass, const std::string ver, const std::string& ns)
{
	if (!ass.empty() && !ns.empty())
	{
		auto iter = std::find_if(Pool.begin(), Pool.end(), ass_map_finder_t(ass));
		if (iter != Pool.end())
		{
			iter->AddNamespace(ns, ver);
		}
		else
		{
			Pool.push_back(UAssemblyMap(ass));
			Pool[Pool.size() - 1].AddNamespace(ns, ver);
		}
	}
}

std::vector<UVersion> UAssemblyPool::Query(const std::string& ass)
{
	std::vector<UVersion> defs = Defs.Query(ass);

	std::vector<UVersion> exs = Excludes.Query(ass);

	if (exs.empty()) return defs;
	for (UVersion ex : exs)
	{
		auto iter = std::find(defs.begin(), defs.end(), ex);
		if (iter != defs.end())
		{
			defs.erase(iter);
		}
	}
	return defs;
}
#pragma endregion