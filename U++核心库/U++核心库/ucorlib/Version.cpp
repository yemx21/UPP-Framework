#include "Version.h"
#include "Parser.h"
#include "StringHelper.h"
#include "BuiltInAssembly.h"
#include "Reflection.h"
using namespace System;



Version::Version(int major, int minor, int build, int revision) :_Major(major), _Minor(minor), _Build(build), _Revision(revision)
{

}

Version::Version(int major, int minor, int build) : _Major(major), _Minor(minor), _Build(build), _Revision(-1)
{

}

Version::Version(int major, int minor) : _Major(major), _Minor(minor), _Build(-1), _Revision(-1)
{

}

Version::Version() : _Major(0), _Minor(0), _Build(-1), _Revision(-1)
{

}

Version::Version(const Version& ver)
{
	_Major = ver._Major;
	_Minor = ver._Minor;
	_Build = ver._Build;
	_Revision = ver._Revision;
}

int Version::GetMajor() const
{
	return _Major;
}
int Version::GetMinor() const
{
	return _Minor;
}

int Version::GetBuild() const
{
	return _Build;
}

int Version::GetRevision() const
{
	return _Revision;
}

short Version::GetMajorRevision() const
{
	return (short)(_Revision >> 16);
}

short Version::GetMinorRevision() const
{
	return (short)(_Revision & 65535);
}

void Version::SetMajor(int major)
{
	_Major = major;
}

void Version::SetMinor(int minor)
{
	_Minor = minor;
}

void Version::SetBuild(int build)
{
	_Build = build;
}

void Version::SetRevision(int revision)
{
	_Revision = revision;
}

bool Version::GetIsEmpty() const
{
	return _Build == 0 && _Revision == 0 && _Major == 0 && _Minor == 0;
}


bool Version::operator == (Version& ver) const
{
	return _Major== ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool Version::operator == (const Version& ver) const
{
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool Version::operator != (Version& ver) const
{
	return _Major != ver._Major || _Minor != ver._Minor || _Build != ver._Build || _Revision != ver._Revision;
}


bool Version::operator != (const Version& ver) const
{
	return _Major != ver._Major || _Minor != ver._Minor || _Build != ver._Build || _Revision != ver._Revision;
}


bool Version::operator > (Version& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}


bool Version::operator > (const Version& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}

bool Version::operator < (Version& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}


bool Version::operator < (const Version& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	return _Revision > ver._Revision;
}


bool Version::operator >= (Version& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	if (_Revision > ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}

bool Version::operator >= (const Version& ver) const
{
	if (_Major > ver._Major) return true;
	if (_Minor > ver._Minor) return true;
	if (_Build > ver._Build) return true;
	if (_Revision > ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool Version::operator <= (Version& ver) const
{
	if (_Major < ver._Major) return true;
	if (_Minor < ver._Minor) return true;
	if (_Build < ver._Build) return true;
	if (_Revision < ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool Version::operator <= (const Version& ver) const
{
	if (_Major < ver._Major) return true;
	if (_Minor < ver._Minor) return true;
	if (_Build < ver._Build) return true;
	if (_Revision < ver._Revision) return true;
	return _Major == ver._Major && _Minor == ver._Minor && _Build == ver._Build && _Revision == ver._Revision;
}


bool Version::TryParse(const std::wstring& input, const CultureInfo& loc, Version& result)
{
	int num;
	int num1;
	int num2;
	int num3;

	std::vector<std::wstring> strArrays;
	StringHelper::Split(input, std::wstring(L"."), strArrays);

	int length = (int)strArrays.size();
	if (length < 2 || length > 4) return false;

	if (!Parser::TryParseInt32(strArrays[0], num)) return false;
	if (!Parser::TryParseInt32(strArrays[1], num1)) return false;

	length = length - 2;

	if (length <= 0)
		result = Version(num, num1);
	else
	{
		if (!Parser::TryParseInt32(strArrays[2], num2)) return false;
		length--;
		if (length <= 0)
		{
			result = Version(num, num1, num2);
		}
		else
		{
			if (!Parser::TryParseInt32(strArrays[3], num3)) return false;
			result = Version(num, num1, num2, num3);
		}
	}
	return true;
}

std::wstring Version::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	std::wstring wfmt = fmt? std::wstring(fmt) :std::wstring();

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
		return std::to_wstring(_Major);
	case 2:
		return std::to_wstring(_Major) + L"." + std::to_wstring(_Minor);
	case 3:
		return _Build<0 ? std::to_wstring(_Major) + L"." + std::to_wstring(_Minor) : std::to_wstring(_Major) + L"." + std::to_wstring(_Minor) + L"." + std::to_wstring(_Build);
	case 4:
		return _Revision<0 ? std::to_wstring(_Major) + L"." + std::to_wstring(_Minor) + L"." + std::to_wstring(_Build) : std::to_wstring(_Major) + L"." + std::to_wstring(_Minor) + L"." + std::to_wstring(_Build) + L"." + std::to_wstring(_Revision);
	case 0:
	default:
		return std::wstring();
	}
	return std::wstring();
}

std::weak_ptr<VersionConverter> VersionConverter::_thisWeakPtr;
std::mutex& VersionConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

VersionConverter::VersionConverter() {}
VersionConverter::~VersionConverter() {}

std::shared_ptr<VersionConverter> VersionConverter::GetSharedInstance()
{
	std::shared_ptr<VersionConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new VersionConverter());
	_thisWeakPtr = p;
	return p;
}

VersionConverter& VersionConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool VersionConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Version))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool VersionConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Version))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any VersionConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Version)))
		{
			if (type->IsReference)
			{
				return Version(AnyVisitor<Version&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Version* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Version res = obj.as<Version>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Version result = Version();
				if (Version::TryParse(std::wstring(tmp), loc, result))
				{
					return result;
				}
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any VersionConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
{
	if (!type)return _missing;
	_type otype = obj.Type;
	if (!otype) return _missing;
	try
	{
		bool iswiden = false;
		if (type->GetIsSTDString(iswiden))
		{
			if (iswiden)
			{
				if (otype->IsReference)
				{
					return AnyVisitor<Version&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Version* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Version v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Version)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Version&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Version* p = obj;
			if (p)
				return *p;
			else
			{
				Version v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any VersionConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Version result = Version();
	if (Version::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring VersionConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Version)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Version&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Version* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Version v = obj;
				return v.ToString();
			}
		}
		return std::wstring();
	}
	catch (...)
	{
		return std::wstring();
	}
}

__inline static Object* Factory_Version_0() { return new Version(); }
__inline static Object* Factory_Version_1(int major, int minor) { return new Version(major, minor); }
__inline static Object* Factory_Version_2(int major, int minor, int build) { return new Version(major, minor, build); }
__inline static Object* Factory_Version_3(int major, int minor, int build, int revision) { return new Version(major, minor, build, revision); }


__uregister_class(Version, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Version>(L"Version", ConstructorFlags::Default, &Factory_Version_0);
	type.AddConstructor(L"Version", ConstructorFlags::Default, ConstructorTypeAdapter<2, Version>::Create(&Factory_Version_1));
	type.AddConstructor(L"Version", ConstructorFlags::Default, ConstructorTypeAdapter<3, Version>::Create(&Factory_Version_2));
	type.AddConstructor(L"Version", ConstructorFlags::Default, ConstructorTypeAdapter<4, Version>::Create(&Factory_Version_3));

	type.AddProperty(L"Major", PropertyFlags::Public, PropertyTypeOf(&Version::GetMajor, &Version::SetMajor));
	type.AddProperty(L"Minor", PropertyFlags::Public, PropertyTypeOf(&Version::GetMinor, &Version::SetMinor));
	type.AddProperty(L"Build", PropertyFlags::Public, PropertyTypeOf(&Version::GetBuild, &Version::SetBuild));
	type.AddProperty(L"Revision", PropertyFlags::Public, PropertyTypeOf(&Version::GetRevision, &Version::SetRevision));
	type.AddProperty(L"MajorRevision", PropertyFlags::Public, PropertyTypeOf(&Version::GetMajorRevision));
	type.AddProperty(L"MinorRevision", PropertyFlags::Public, PropertyTypeOf(&Version::GetMinorRevision));
	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Version::GetIsEmpty));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Version, Version&>::Create(&Version::operator=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Version, bool>::Create<const Version&>(&Version::operator==));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Version, bool>::Create<Version&>(&Version::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Version, bool>::Create<const Version&>(&Version::operator!=));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Version, bool>::Create<Version&>(&Version::operator!=));
	type.AddOperator(L"GreaterThan", OperatorTypeAdapter<1, Operators::GreaterThan, Version, bool>::Create<const Version&>(&Version::operator>));
	type.AddOperator(L"GreaterThan", OperatorTypeAdapter<1, Operators::GreaterThan, Version, bool>::Create<Version&>(&Version::operator>));
	type.AddOperator(L"GreaterThanOrEqualTo", OperatorTypeAdapter<1, Operators::GreaterThanOrEqualTo, Version, bool>::Create<const Version&>(&Version::operator>=));
	type.AddOperator(L"GreaterThanOrEqualTo", OperatorTypeAdapter<1, Operators::GreaterThanOrEqualTo, Version, bool>::Create<Version&>(&Version::operator>=));
	type.AddOperator(L"LessThan", OperatorTypeAdapter<1, Operators::LessThan, Version, bool>::Create<const Version&>(&Version::operator<));
	type.AddOperator(L"LessThan", OperatorTypeAdapter<1, Operators::LessThan, Version, bool>::Create<Version&>(&Version::operator<));
	type.AddOperator(L"LessThanOrEqualTo", OperatorTypeAdapter<1, Operators::LessThanOrEqualTo, Version, bool>::Create<const Version&>(&Version::operator<=));
	type.AddOperator(L"LessThanOrEqualTo", OperatorTypeAdapter<1, Operators::LessThanOrEqualTo, Version, bool>::Create<Version&>(&Version::operator<=));

	type.AddAttribute(new TypeConverterAttribute(L"VersionConverter", VersionConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);

};