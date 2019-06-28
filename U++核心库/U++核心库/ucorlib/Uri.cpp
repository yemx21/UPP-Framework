#include "UriUtils.h"
#include "StringHelper.h"
#include "BuiltInAssembly.h"
#include "Reflection.h"
#include "Formatter.h"
using namespace System;


#pragma region DomainName

namespace System
{
	class DomainName
	{
	public:
		DomainName() {}
		DomainName(const std::wstring& v);
		bool is_null() const { return value.empty(); } 
		const std::string& string() const { return value; }
		DomainName operator +(const DomainName& rhs) const
		{
			DomainName result = *this;
			return result += rhs;
		}
		DomainName& operator +=(const DomainName& rhs);
		static const wchar_t SEPARATOR_CHAR; 
		static const wchar_t char_class[256]; 
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, DomainName& v);
	private:
		std::string value;
	};

	const wchar_t DomainName::SEPARATOR_CHAR = '.';
	const wchar_t DomainName::char_class[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	DomainName::DomainName(const std::wstring& v)
	{
		std::wstring::const_iterator first = v.begin();
		if (!Parse(first, v.end(), *this) || first != v.end())
			throw std::invalid_argument("invalid domain name");
	}

	DomainName& DomainName::operator +=(const DomainName& rhs)
	{
		if (is_null())
			value = rhs.value;
		else if (!rhs.is_null()) {
			value.append(1, SEPARATOR_CHAR);
			value.append(rhs.value);
		}
		return *this;
	}

	bool DomainName::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, DomainName& v)
	{
		std::wstring::const_iterator f = first;
		int state = 0; // 0=1st char
		std::wstring::const_iterator prev_cls_1 = last;
		for (; f != last; ++f) 
		{
			wchar_t c = *f;
			wchar_t cls = DomainName::char_class[(unsigned char)c];
			if (state) 
			{ 
				if (!cls) 
				{
					if (prev_cls_1 + 1 != f) 
						break;
					if (c != DomainName::SEPARATOR_CHAR)
						break;
					state = 0;
				}
				else if (cls == 1)
					prev_cls_1 = f;
			}
			else 
			{ 
				if (cls != 1) 
					break;
				state = 1;
				prev_cls_1 = f;
			}
		}
		if (f == first)
			return false;
		f = prev_cls_1 + 1;
		v.value.assign(first, f);
		StringHelper::ToLower(v.value);
		first = f;
		return true;
	}
}

#pragma endregion

#pragma region IPv4Address

namespace System
{
	class IPv6Address;

	class IPv4Address
	{
		friend class IPv6Address;
	public:
		IPv4Address();
		IPv4Address(const std::wstring& v);
		bool is_null() const { return !octets[0] && !octets[1] && !octets[2] && !octets[3]; }
		std::wstring string() const; 
		static const wchar_t SEPARATOR_CHAR; 
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, IPv4Address& v);
	private:
		unsigned char octets[4];
	};

	const wchar_t IPv4Address::SEPARATOR_CHAR = '.';
	IPv4Address::IPv4Address()
	{
		octets[0] = 0;
		octets[1] = 0;
		octets[2] = 0;
		octets[3] = 0;
	}
	IPv4Address::IPv4Address(const std::wstring& v)
	{
		std::wstring::const_iterator first = v.begin();
		if (!Parse(first, v.end(), *this) || first != v.end())
			throw std::invalid_argument("invalid IP address");
	}
	std::wstring IPv4Address::string() const
	{
		return Formatter::Format(L"{0}{1}{2}{3}{4}{5}{7}", CultureInfo::Current(), (VariadicAny(octets[0]), SEPARATOR_CHAR, octets[1], SEPARATOR_CHAR, octets[2], SEPARATOR_CHAR, octets[3]));
	}

	bool IPv4Address::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, IPv4Address& v)
	{
		std::wstring::const_iterator f = first;
		IPv4Address ip;
		for (size_t i = 0; i < 4; ++i) 
		{
			if (f == last)
				return false;
			if (i && (*f++ != IPv4Address::SEPARATOR_CHAR || f == last))
				return false;
			int n;
			unsigned int c = (unsigned int)*f;
			if (c == '0') {
				n = 0;
				++f;
			}
			else 
			{
				if (c < '1' || '9' < c)
					return false;
				n = c - '0';
				if (++f != last) {
					c = (unsigned int)*f;
					if ('0' <= c && c <= '9')
					{ 
						n = n * 10 + (c - '0');
						if (++f != last) 
						{
							c = (unsigned int)*f;
							if ('0' <= c && c <= '9')
							{
								n = n * 10 + (c - '0');
								if (255 < n)
									return false;
								++f;
							}
						}
					}
				}
			}
			ip.octets[i] = n;
		}
		v = ip;
		first = f;
		return true;
	}
}

#pragma endregion

#pragma region IPv6Address

namespace System
{
	class IPv6Address
	{
		friend class IPv4Address;
	public:
		IPv6Address();
		IPv6Address(const std::wstring& v);
		bool is_null() const
		{
			return !hextets[0] && !hextets[1] && !hextets[2] && !hextets[3] &&
				!hextets[4] && !hextets[5] && !hextets[6] && !hextets[7];
		}
		std::wstring string(bool compress=false) const;
		static const wchar_t SEPARATOR_CHAR;
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, IPv6Address& v);
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, unsigned short& v);
	private:
		bool zero_run(size_t& first, size_t& last) const;
		unsigned short hextets[8];
	};

	const wchar_t IPv6Address::SEPARATOR_CHAR = ':';
	IPv6Address::IPv6Address()
	{
		hextets[0] = 0;
		hextets[1] = 0;
		hextets[2] = 0;
		hextets[3] = 0;
		hextets[4] = 0;
		hextets[5] = 0;
		hextets[6] = 0;
		hextets[7] = 0;
	}
	IPv6Address::IPv6Address(const std::wstring& v)
	{
		std::wstring::const_iterator first = v.begin();
		if (!Parse(first, v.end(), *this) || first != v.end())
			throw std::invalid_argument("invalid IPv6 address");
	}
	std::wstring IPv6Address::string(bool compress) const
	{
		size_t zfirst;
		size_t zlast;
		if (compress)
			zero_run(zfirst, zlast);
		else
		{
			zfirst = std::wstring::npos;
			zlast = std::wstring::npos;
		}
		std::wstringstream wss;
		for (size_t i = 0; i < 8; ++i)
			if (i < zfirst || zlast <= i) 
			{
				if (i)
					wss << SEPARATOR_CHAR;
				wss << std::hex << std::uppercase << hextets[i];
			}
			else if (i == zfirst)
				wss << SEPARATOR_CHAR;
		if (zlast == 8)
			wss << SEPARATOR_CHAR;
		return wss.str();
	}

	bool IPv6Address::zero_run(size_t& first, size_t& last) const
	{
		first = std::wstring::npos;
		size_t len = 0;
		size_t curi;
		size_t curn = 0;
		for (size_t i = 0;; ++i)
		{
			if (i == 8 || hextets[i])
			{
				if (len < curn)
				{
					first = curi;
					len = curn;
				}
				if (i == 8)
					break;
				curn = 0;
			}
			else
			{
				if (!curn)
					curi = i;
				++curn;
			}
		}
		if (!len) 
		{
			last = first;
			return false;
		}
		last = first + len;
		return true;
	}

	bool IPv6Address::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, unsigned short& v)
	{
		std::wstring::const_iterator f = first;
		unsigned int tmp = 0;
		size_t i = 0;
		for (; i < 4; ++i, ++f) 
		{
			if (f == last)
				break;
			unsigned int c = (unsigned int)*f;
			if ('0' <= c && c <= '9')
				tmp = (tmp << 4) + (c - '0');
			else if ('A' <= c && c <= 'F')
				tmp = (tmp << 4) + (c - 'A' + 10);
			else if ('a' <= c && c <= 'f')
				tmp = (tmp << 4) + (c - 'a' + 10);
			else
				break;
		}
		if (!i)
			return false;
		v = tmp;
		first = f;
		return true;
	}

	bool IPv6Address::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, IPv6Address& v)
	{
		std::wstring::const_iterator f = first;
		IPv6Address ip;
		size_t elide;
		bool elided = false;
		size_t i = 0;
		std::wstring::const_iterator digits;
		for (; i < 8; ++i) 
		{
			if (f == last)
				break;
			if (i && (*f != IPv6Address::SEPARATOR_CHAR || ++f == last))
				break;
			if (*f == IPv6Address::SEPARATOR_CHAR) 
			{ 
				if (elided)
					return false;
				ip.hextets[i] = 0;
				elide = i;
				elided = true;
				if (!i)
					++f;
			}
			else {
				std::wstring::const_iterator anchor = f;
				if (!Parse(f, last, ip.hextets[i]))
					break;
				if (i < 7 && f != last && *f == IPv4Address::SEPARATOR_CHAR) 
				{ 
					IPv4Address v4;
					if (IPv4Address::Parse(anchor, last, v4))
					{ 
						ip.hextets[i++] = ((unsigned int)v4.octets[0] << 8) | v4.octets[1];
						ip.hextets[i++] = ((unsigned int)v4.octets[2] << 8) | v4.octets[3];
						f = anchor;
						break;
					}
				}
			}
		}
		if (i == 8) 
			v = ip;
		else 
		{ 
			if (!elided)
				return false;
			size_t j = 0;
			size_t zeros = 8 - i;
			for (i = 0; i < 8; ++i) 
			{
				unsigned short u;
				if (elide < i && zeros) 
				{
					u = 0;
					--zeros;
				}
				else
					u = ip.hextets[j++];
				v.hextets[i] = u;
			}
		}
		first = f;
		return true;
	}
}

#pragma endregion


#pragma region Scheme

Uri::Scheme::Scheme() {}
Uri::Scheme::Scheme(const std::wstring& v)
{
	std::wstring::const_iterator first = v.begin();
	if (!UriHelper::Parse(first, v.end(), *this) || first != v.end())
		throw std::invalid_argument("invalid URI Scheme");
}

std::wstring Uri::Scheme::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return value;
}

bool UriHelper::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Scheme& v)
{
	std::wstring tmp;
	if (!UriHelper::Parse(UriHelper::SCHEME_TRAITS, first, last, tmp) || tmp.empty())
		return false;
	StringHelper::ToLower(tmp);
	v.value = tmp;
	return true;
}

bool Uri::Scheme::GetIsEmpty() const { return value.empty(); } 
const std::wstring& Uri::Scheme::GetValue() const { return value; } 

bool Uri::Scheme::TryParse(const std::wstring& input, const CultureInfo& loc, Uri::Scheme& result)
{
	std::wstring::const_iterator first = input.begin();
	return UriHelper::Parse(first, input.end(), result) || first != input.end();
}

__inline static Object* Factory_Scheme_0() { return new Uri::Scheme(); }
__inline static Object* Factory_Scheme_1(const std::wstring& str) { return new Uri::Scheme(str); }


__uregister_class1(Uri::Scheme, Scheme, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Uri::Scheme>(L"Scheme", ConstructorFlags::Default, &Factory_Scheme_0);
	type.AddConstructor(L"Scheme", ConstructorFlags::Default, ConstructorTypeAdapter<1, Scheme>::Create(&Factory_Scheme_1));

	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Scheme::GetIsEmpty));
	type.AddProperty(L"Value", PropertyFlags::Public, PropertyTypeOf(&Scheme::GetValue));

	type.AddMethod(L"GetIsEmpty", MethodFlags::Public, MethodTypeAdapter<0, Scheme, bool>::Create(&Scheme::GetIsEmpty));
	type.AddMethod(L"GetValue", MethodFlags::Public, MethodTypeAdapter<0, Scheme, const std::wstring&>::Create(&Scheme::GetValue));

	BuiltInAssemblyFactory::Regisiter(&type);
};

std::weak_ptr<Uri::SchemeConverter> Uri::SchemeConverter::_thisWeakPtr;
std::mutex& Uri::SchemeConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

Uri::SchemeConverter::SchemeConverter() {}
Uri::SchemeConverter::~SchemeConverter() {}

std::shared_ptr<Uri::SchemeConverter> Uri::SchemeConverter::GetSharedInstance()
{
	std::shared_ptr<Uri::SchemeConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new SchemeConverter());
	_thisWeakPtr = p;
	return p;
}

Uri::SchemeConverter& Uri::SchemeConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool Uri::SchemeConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Scheme))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool Uri::SchemeConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Scheme))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any Uri::SchemeConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Scheme)))
		{
			if (type->IsReference)
			{
				return Scheme(AnyVisitor<Scheme&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Scheme* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Scheme res = obj.as<Scheme>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Scheme result = Scheme();
				if (Scheme::TryParse(std::wstring(tmp), loc, result))
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

Any Uri::SchemeConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Scheme&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Scheme* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Scheme v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Scheme)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Scheme&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Scheme* p = obj;
			if (p)
				return *p;
			else
			{
				Scheme v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any Uri::SchemeConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Scheme result = Scheme();
	if (Scheme::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring Uri::SchemeConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Scheme)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Scheme&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Scheme* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Scheme v = obj;
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

#pragma endregion

#pragma region HostType

Uri::HostType::HostType() : value(static_cast<Int8>(Null)) {}

Uri::HostType::~HostType() {}

__uregister_enum1(Uri::HostType, HostType)
{
	type.AddValue(L"Null", Uri::HostType::Null, __uhashstr(System::Uri::HostType::Null));
	type.AddValue(L"DomainName", Uri::HostType::DomainName, __uhashstr(System::Uri::HostType::DomainName));
	type.AddValue(L"IPv4Address", Uri::HostType::IPv4Address, __uhashstr(System::Uri::HostType::IPv4Address));
	type.AddValue(L"IPv6Address", Uri::HostType::IPv6Address, __uhashstr(System::Uri::HostType::IPv6Address));

	BuiltInAssemblyFactory::Regisiter(&type);
};
#pragma endregion

#pragma region Authority
const wchar_t Authority_IP_LITERAL_BEGIN_CHAR = '[';
const wchar_t Authority_IP_LITERAL_END_CHAR = ']';
const wchar_t Authority_PORT_SEPARATOR_CHAR = ':';

Uri::Authority::Authority():port(0u) {}
Uri::Authority::Authority(const std::wstring& v) : port(0u)
{
	std::wstring::const_iterator first = v.begin();
	if (!UriHelper::Parse(first, v.end(), *this) || first != v.end())
		throw std::invalid_argument("invalid URI authority");
}

std::wstring Uri::Authority::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return EncodedString;
}

std::wstring Uri::Authority::GetEncodedString() const
{
	std::wstringstream os;
	os << host;
	if (port)
		os << Authority_PORT_SEPARATOR_CHAR << port;
	return os.str();
}

bool UriHelper::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Authority& v)
{
	std::wstring::const_iterator f = first;
	Uri::HostType e;
	IPv4Address ipaddr;
	if (IPv4Address::Parse(f, last, ipaddr))
		e = Uri::HostType::IPv4Address;
	else
	{
		DomainName dn;
		if (DomainName::Parse(f, last, dn))
			e = Uri::HostType::DomainName;
		else {
			if (f == last || *f != Authority_IP_LITERAL_BEGIN_CHAR)
				return false;
			++f;
			IPv6Address ipv6addr;
			if (!IPv6Address::Parse(f, last, ipv6addr)) 
				return false;
			if (f == last || *f != Authority_IP_LITERAL_END_CHAR)
				return false;
			e = Uri::HostType::IPv6Address;
			++f;
		}
	}
	std::wstring host(first, f);
	unsigned int port = 0;
	if (f != last && *f == Authority_PORT_SEPARATOR_CHAR) 
	{
		++f;
		std::wstring::const_iterator anchor = f;
		for (; f != last; ++f) 
		{
			wchar_t c = *f;
			if (c < '0' || '9' < c)
				break;
			else 
			{
				port = port * 10 + (c - '0');
				if (65535 < port)
					return false; 
			}
		}
		if (f == anchor)
			return false; 
	}
	v.hostType = e;
	v.host = host;
	v.port = port;
	first = f;
	return true;
}

bool Uri::Authority::GetIsEmpty() const { return host.empty(); }
const std::wstring& Uri::Authority::GetHost() const { return host; }
Uri::HostType Uri::Authority::GetHostType() const { return hostType; }
unsigned short Uri::Authority::GetPort() const { return port; }

bool Uri::Authority::TryParse(const std::wstring& input, const CultureInfo& loc, Uri::Authority& result)
{
	std::wstring::const_iterator first = input.begin();
	return UriHelper::Parse(first, input.end(), result) || first != input.end();
}

__inline static Object* Factory_Authority_0() { return new Uri::Authority(); }
__inline static Object* Factory_Authority_1(const std::wstring& str) { return new Uri::Authority(str); }

__uregister_class1(Uri::Authority, Authority, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Uri::Authority>(L"Authority", ConstructorFlags::Default, &Factory_Authority_0);
	type.AddConstructor(L"Authority", ConstructorFlags::Default, ConstructorTypeAdapter<1, Authority>::Create(&Factory_Authority_1));

	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Authority::GetIsEmpty));
	type.AddProperty(L"Host", PropertyFlags::Public, PropertyTypeOf(&Authority::GetHost));
	type.AddProperty(L"HostType", PropertyFlags::Public, PropertyTypeOf(&Authority::GetHostType));
	type.AddProperty(L"Port", PropertyFlags::Public, PropertyTypeOf(&Authority::GetPort));
	type.AddProperty(L"EncodedString", PropertyFlags::Public, PropertyTypeOf(&Authority::GetEncodedString));

	type.AddMethod(L"GetIsEmpty", MethodFlags::Public, MethodTypeAdapter<0, Authority, bool>::Create(&Authority::GetIsEmpty));
	type.AddMethod(L"GetHost", MethodFlags::Public, MethodTypeAdapter<0, Authority, const std::wstring&>::Create(&Authority::GetHost));
	type.AddMethod(L"GetHostType", MethodFlags::Public, MethodTypeAdapter<0, Authority, Uri::HostType>::Create(&Authority::GetHostType));
	type.AddMethod(L"GetPort", MethodFlags::Public, MethodTypeAdapter<0, Authority, UShort>::Create(&Authority::GetPort));
	type.AddMethod(L"GetEncodedString", MethodFlags::Public, MethodTypeAdapter<0, Authority, std::wstring>::Create(&Authority::GetEncodedString));

	BuiltInAssemblyFactory::Regisiter(&type);
};

std::weak_ptr<Uri::AuthorityConverter> Uri::AuthorityConverter::_thisWeakPtr;
std::mutex& Uri::AuthorityConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

Uri::AuthorityConverter::AuthorityConverter() {}
Uri::AuthorityConverter::~AuthorityConverter() {}

std::shared_ptr<Uri::AuthorityConverter> Uri::AuthorityConverter::GetSharedInstance()
{
	std::shared_ptr<Uri::AuthorityConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new AuthorityConverter());
	_thisWeakPtr = p;
	return p;
}

Uri::AuthorityConverter& Uri::AuthorityConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool Uri::AuthorityConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Authority))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool Uri::AuthorityConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Authority))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any Uri::AuthorityConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Authority)))
		{
			if (type->IsReference)
			{
				return Authority(AnyVisitor<Authority&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Authority* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Authority res = obj.as<Authority>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Authority result = Authority();
				if (Authority::TryParse(std::wstring(tmp), loc, result))
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

Any Uri::AuthorityConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Authority&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Authority* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Authority v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Authority)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Authority&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Authority* p = obj;
			if (p)
				return *p;
			else
			{
				Authority v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any Uri::AuthorityConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Authority result = Authority();
	if (Authority::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring Uri::AuthorityConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Authority)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Authority&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Authority* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Authority v = obj;
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

#pragma endregion

#pragma region Path

const wchar_t Path_SEPARATOR_CHAR = '/';

Uri::Path::Path() : absolute(false), isdirectory(false) {}
Uri::Path::Path(const std::wstring& v) : absolute(false), isdirectory(false)
{
	if (!v.empty())
	{
		std::wstring::const_iterator first = v.begin();
		if (!UriHelper::Parse(first, v.end(), *this) || first != v.end())
			throw std::invalid_argument("invalid URI path");
	}
}

std::wstring Uri::Path::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return EncodedString;
}

std::wstring Uri::Path::GetEncodedString() const
{
	std::wstringstream s;
	if (absolute)
		s << Path_SEPARATOR_CHAR;
	for (segments_type::const_iterator it = segments.begin(); it != segments.end(); ++it) 
	{
		if (it != segments.begin())
			s << Path_SEPARATOR_CHAR;
		s << UriHelper::Encode(UriHelper::PATH_TRAITS, *it); 
	}
	if (isdirectory)
		s << Path_SEPARATOR_CHAR;
	return s.str();
}
bool UriHelper::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Path& v)
{
	std::wstring::const_iterator f = first;
	Uri::Path tmp;
	std::string e;
	if (f != last && *f == Path_SEPARATOR_CHAR)
	{ 
		tmp.absolute = true;
		++f;
	}
	bool sep = true; 
	for (; f != last;)
	{
		if (*f == Path_SEPARATOR_CHAR)
			++f;
		else
		{
			std::wstring s;
			UriHelper::Parse(UriHelper::PATH_TRAITS, f, last, s);
			if (!s.empty())
			{
				if (!UriHelper::Decode(s))
				{
					return false;
				}
				tmp.segments.push_back(s);
			}
			if (f == last || *f != Path_SEPARATOR_CHAR)
			{
				sep = false;
				break;
			}
			++f;
		}
	}
	if (f == first) return false;
	if (sep && !tmp.segments.empty())
		tmp.isdirectory = true;
	v = tmp;
	first = f;
	return true;
}

bool Uri::Path::GetIsEmpty() const { return segments.empty() && !absolute; }
bool Uri::Path::GetIsAbsolute() const { return absolute; }
void Uri::Path::SetIsAbsolute(bool v) { absolute = v; }
bool Uri::Path::GetIsDirectory() const { return isdirectory; }
void Uri::Path::SetIsDirectory(bool v) { isdirectory = v; }

bool Uri::Path::TryParse(const std::wstring& input, const CultureInfo& loc, Uri::Path& result)
{
	std::wstring::const_iterator first = input.begin();
	return UriHelper::Parse(first, input.end(), result) || first != input.end();
}

Uri::Path::const_iterator Uri::Path::begin() const
{
	return segments.begin();
}

Uri::Path::const_iterator Uri::Path::end() const
{
	return segments.end();
}

const std::wstring& Uri::Path::front() const
{
	if (segments.empty())
		throw std::out_of_range("attempt to access empty URI path");
	return segments.front();
}

const std::wstring& Uri::Path::back() const
{
	if (segments.empty())
		throw std::out_of_range("attempt to access empty URI path");
	return segments.back();
}

void Uri::Path::popfront()
{
	if (!segments.empty()) 
	{
		segments.pop_front();
		absolute = false;
		if (segments.empty()) isdirectory = false;
	}
}

bool Uri::Path::popback(const Path& back)
{
	if (isdirectory != back.isdirectory || back.GetIsEmpty()) return false;
	Uri::Path::segments_type::reverse_iterator rit = segments.rbegin();
	Uri::Path::segments_type::reverse_iterator rend = segments.rend();
	Uri::Path::segments_type::const_reverse_iterator ritb = back.segments.rbegin();
	Uri::Path::segments_type::const_reverse_iterator rendb = back.segments.rend();
	bool found = false;
	for (; rit != rend && *rit == *ritb; ++rit)
		if (++ritb == rendb) 
		{ 
			found = true;
			break;
		}
	if (!found) return false;

	Uri::Path::segments_type::iterator it = segments.begin();
	for (; ++rit != rend;)
		++it;
	if (back.absolute && (it != segments.begin() || !absolute)) return false;
	segments.erase(it, segments.end());
	if (segments.empty()) isdirectory = false;
	return true;
}

bool Uri::Path::IsMatchPrefix(const Uri::Path& rhs) const
{
	if (absolute != rhs.absolute)
		return false;
	const_iterator itrhs = rhs.begin();
	for (const_iterator it = begin();; ++it, ++itrhs) 
	{
		if (it == end()) 
		{
			if (itrhs != rhs.end())
				return false;
			break;
		}
		if (itrhs == rhs.end())
			return true;
		if (*it != *itrhs)
			return false;
	}
	return !rhs.isdirectory || isdirectory;
}

void Uri::Path::Clear()
{
	absolute = false;
	isdirectory = false;
	segments.clear();
}

size_t Uri::Path::GetSize() const
{
	return segments.size();
}

bool Uri::Path::operator ==(const Path& rhs) const
{
	return absolute == rhs.absolute && isdirectory == rhs.isdirectory && segments == rhs.segments;
}

bool Uri::Path::operator !=(const Path& rhs) const
{
	return !(*this == rhs);
}

bool Uri::Path::operator <(const Path& rhs) const
{
	if (!absolute && rhs.absolute) return true;
	if (absolute && !rhs.absolute) return false;
	Uri::Path::segments_type::const_iterator it = segments.begin();
	Uri::Path::segments_type::const_iterator itr = rhs.segments.begin();
	for (;; ++it, ++itr) {
		if (it == segments.end() && itr != rhs.segments.end()) return true;
		if (it != segments.end() && itr == rhs.segments.end()) return false;
		int i = it->compare(*itr);
		if (i < 0) return true;
		if (0 < i) return false;
	}
	return !isdirectory && rhs.isdirectory;
}

Uri::Path Uri::Path::operator +(const std::wstring& rhs) const
{
	Path result = *this;
	return result += rhs;
}

Uri::Path& Uri::Path::operator +=(const std::wstring& rhs)
{
	segments.push_back(rhs);
	isdirectory = false;
	return *this;
}

Uri::Path Uri::Path::operator +(const Path& rhs) const
{
	Path result = *this;
	return result += rhs;
}

Uri::Path& Uri::Path::operator +=(const Path& rhs)
{
	segments.insert(segments.end(), rhs.segments.begin(), rhs.segments.end());
	isdirectory = rhs.isdirectory;
	return *this;
}

__inline static Object* Factory_Path_0() { return new Uri::Path(); }
__inline static Object* Factory_Path_1(const std::wstring& str) { return new Uri::Path(str); }

__uregister_class1(Uri::Path, Path, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Uri::Path>(L"Path", ConstructorFlags::Default, &Factory_Path_0);
	type.AddConstructor(L"Path", ConstructorFlags::Default, ConstructorTypeAdapter<1, Path>::Create(&Factory_Path_1));

	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Path::GetIsEmpty));
	type.AddProperty(L"IsAbsolute", PropertyFlags::Public, PropertyTypeOf(&Path::GetIsAbsolute, &Path::SetIsAbsolute));
	type.AddProperty(L"IsDirectory", PropertyFlags::Public, PropertyTypeOf(&Path::GetIsDirectory, &Path::SetIsDirectory));
	type.AddProperty(L"Size", PropertyFlags::Public, PropertyTypeOf(&Path::GetSize));
	type.AddProperty(L"EncodedString", PropertyFlags::Public, PropertyTypeOf(&Path::GetEncodedString));

	type.AddMethod(L"GetIsEmpty", MethodFlags::Public, MethodTypeAdapter<0, Path, bool>::Create(&Path::GetIsEmpty));
	type.AddMethod(L"IsAbsolute", MethodFlags::Public, MethodTypeAdapter<0, Path, bool>::Create(&Path::GetIsAbsolute));
	type.AddMethod(L"GetIsDirectory", MethodFlags::Public, MethodTypeAdapter<0, Path, bool>::Create(&Path::GetIsDirectory));
	type.AddMethod(L"GetSize", MethodFlags::Public, MethodTypeAdapter<0, Path, size_t>::Create(&Path::GetSize));
	type.AddMethod(L"GetEncodedString", MethodFlags::Public, MethodTypeAdapter<0, Path, std::wstring>::Create(&Path::GetEncodedString));
	type.AddMethod(L"Clear", MethodFlags::Public, MethodTypeAdapter<0, Path>::Create(&Path::Clear));
	type.AddMethod(L"front", MethodFlags::Public, MethodTypeAdapter<0, Path, const std::wstring&>::Create(&Path::front));
	type.AddMethod(L"back", MethodFlags::Public, MethodTypeAdapter<0, Path, const std::wstring&>::Create(&Path::back));
	type.AddMethod(L"popfront", MethodFlags::Public, MethodTypeAdapter<0, Path>::Create(&Path::popfront));
	type.AddMethod(L"popback", MethodFlags::Public, MethodTypeAdapter<1, Path, bool>::Create(&Path::popback));
	type.AddMethod(L"IsMatchPrefix", MethodFlags::Public, MethodTypeAdapter<1, Path, bool>::Create(&Path::IsMatchPrefix));

	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Uri::Path, bool>::Create<const Uri::Path&>(&Uri::Path::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Uri::Path, bool>::Create<const Uri::Path&>(&Uri::Path::operator!=));
	type.AddOperator(L"Less", OperatorTypeAdapter<1, Operators::LessThan, Uri::Path, bool>::Create<const Uri::Path&>(&Uri::Path::operator<));
	type.AddOperator(L"Add", OperatorTypeAdapter<1, Operators::Addition, Uri::Path, Uri::Path>::Create<const std::wstring&>(&Uri::Path::operator+));
	type.AddOperator(L"Add", OperatorTypeAdapter<1, Operators::Addition, Uri::Path, Uri::Path>::Create<const Uri::Path&>(&Uri::Path::operator+));
	type.AddOperator(L"AddAssign", OperatorTypeAdapter<1, Operators::AdditionAssignment, Uri::Path, Uri::Path&>::Create<const std::wstring&>(&Uri::Path::operator+=));
	type.AddOperator(L"AddAssign", OperatorTypeAdapter<1, Operators::AdditionAssignment, Uri::Path, Uri::Path&>::Create<const Uri::Path&>(&Uri::Path::operator+=));

	BuiltInAssemblyFactory::Regisiter(&type);
};

std::weak_ptr<Uri::PathConverter> Uri::PathConverter::_thisWeakPtr;
std::mutex& Uri::PathConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

Uri::PathConverter::PathConverter() {}
Uri::PathConverter::~PathConverter() {}

std::shared_ptr<Uri::PathConverter> Uri::PathConverter::GetSharedInstance()
{
	std::shared_ptr<Uri::PathConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new PathConverter());
	_thisWeakPtr = p;
	return p;
}

Uri::PathConverter& Uri::PathConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool Uri::PathConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Path))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool Uri::PathConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Path))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any Uri::PathConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Path)))
		{
			if (type->IsReference)
			{
				return Path(AnyVisitor<Path&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Path* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Path res = obj.as<Path>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Path result = Path();
				if (Path::TryParse(std::wstring(tmp), loc, result))
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

Any Uri::PathConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Path&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Path* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Path v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Path)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Path&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Path* p = obj;
			if (p)
				return *p;
			else
			{
				Path v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any Uri::PathConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Path result = Path();
	if (Path::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring Uri::PathConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Path)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Path&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Path* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Path v = obj;
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

#pragma endregion

#pragma region Query
const char Query_PAIRS_SEP_CHAR = '&';
const char Query_KEY_VALUE_SEP_CHAR = '=';

Uri::Query::Query() : sorted(false) {}
Uri::Query::Query(const std::wstring& v, bool dosort) : sorted(false)
{
	std::wstring::const_iterator first = v.begin();
	if (!UriHelper::Parse(first, v.end(), *this) || first != v.end())
		throw std::invalid_argument("invalid URI query");	
	if (!values.empty() && dosort)
		Sort();
}

std::wstring Uri::Query::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return EncodedString;
}

bool Uri::Query::GetIsEmpty() const
{
	return values.empty();
}

bool UriHelper::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Query& v)
{
	std::wstring::const_iterator f = first;
	Uri::Query tmp;
	for (bool eof = false; !eof;) 
	{
		std::wstring key;
		std::wstring value;
		bool nokvsep = false;
		for (int i = 0; i < 2; ++i) 
		{
			std::wstring::const_iterator anchor = f;
			for (;; ++f)
				if (f == last) 
				{
					eof = true;
					break;
				}
				else
				{
					wchar_t c = *f;
					if (c == Query_PAIRS_SEP_CHAR || (!i && c == Query_KEY_VALUE_SEP_CHAR))
						break;
					if (UriHelper::QUERY_TRAITS.char_class[(unsigned char)c] < UriHelper::char_class_e::CVAL) 
					{
						eof = true;
						break;
					}
				}
				if (i)
					value.assign(anchor, f);
				else 
				{
					key.assign(anchor, f);
					if (eof || *f == Query_PAIRS_SEP_CHAR) 
					{ 
						i = 2;
						nokvsep = true;
					}
				}
				if (!eof)
					++f; 
		}
		if (key.empty()) 
		{ 
			if (!value.empty()) 
			{
				if (tmp.values.empty()) return false;
			}
		}
		else if (nokvsep)
		{ 
			if (tmp.values.empty()) return false;
		}
		else 
		{
			if (!UriHelper::Decode(key)) 
			{
				if (tmp.values.empty()) return false;
			}
			if (!UriHelper::Decode(value))
			{
				if (!tmp.values.empty()) return false;
			}
			tmp.values.push_back(std::make_pair(key, value));
		}
	}
	v = tmp;
	first = f;
	return true;
}
bool Uri::Query::TryParse(const std::wstring& input, const CultureInfo& loc, Uri::Query& result)
{
	std::wstring::const_iterator first = input.begin();
	return UriHelper::Parse(first, input.end(), result) || first != input.end();
}
bool Uri::Query::GetIsSorted() const { return sorted; }
void Uri::Query::Sort()
{
	std::sort(values.begin(), values.end());
	sorted = true;
}

std::wstring Uri::Query::GetEncodedString() const
{
	std::wstringstream s;
	for (const_iterator it = values.begin(); it != values.end(); ++it)
	{
		if (it != values.begin())
			s << Query_PAIRS_SEP_CHAR;
		s << UriHelper::Encode(UriHelper::QUERY_TRAITS, it->first);
		s << Query_KEY_VALUE_SEP_CHAR;
		s << UriHelper::Encode(UriHelper::QUERY_TRAITS, it->second);
	}
	return s.str();
}
Uri::Query::const_iterator Uri::Query::Find(const std::wstring& key) const
{
	const_iterator it;
	if (sorted) 
	{
		const_iterator lb = std::lower_bound(values.begin(), values.end(), value_type(key, L""));
		it = (lb != values.end() && lb->first == key) ? lb : values.end();
	}
	else 
	{
		for (it = values.begin(); it != values.end(); ++it)
			if (it->first == key)
				break;
	}
	return it;
}
Uri::Query::iterator Uri::Query::Find(const std::wstring& key)
{
	iterator it;
	if (sorted) 
	{
		iterator lb = std::lower_bound(values.begin(), values.end(), value_type(key, L""));
		it = (lb != values.end() && lb->first == key) ? lb : values.end();
	}
	else
	{
		for (it = values.begin(); it != values.end(); ++it)
			if (it->first == key)
				break;
	}
	return it;
}

__inline static Object* Factory_Query_0() { return new Uri::Query(); }
__inline static Object* Factory_Query_1(const std::wstring& str, bool sorted=false) { return new Uri::Query(str, sorted); }

__uregister_class1(Uri::Query, Query, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Uri::Query>(L"Query", ConstructorFlags::Default, &Factory_Query_0);
	type.AddConstructor(L"Query", ConstructorFlags::Default, ConstructorTypeAdapter<2, Query>::Create(&Factory_Query_1));
	
	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Fragment::GetIsEmpty));
	type.AddProperty(L"IsSorted", PropertyFlags::Public, PropertyTypeOf(&Query::GetIsSorted));
	type.AddProperty(L"EncodedString", PropertyFlags::Public, PropertyTypeOf(&Query::GetEncodedString));

	type.AddMethod(L"GetIsEmpty", MethodFlags::Public, MethodTypeAdapter<0, Fragment, bool>::Create(&Fragment::GetIsEmpty));
	type.AddMethod(L"Sort", MethodFlags::Public, MethodTypeAdapter<0, Query>::Create(&Query::Sort));
	type.AddMethod(L"GetIsSorted", MethodFlags::Public, MethodTypeAdapter<0, Query, bool>::Create(&Query::GetIsSorted));
	type.AddMethod(L"GetEncodedString", MethodFlags::Public, MethodTypeAdapter<0, Query, std::wstring>::Create(&Query::GetEncodedString));

	BuiltInAssemblyFactory::Regisiter(&type);
};

std::weak_ptr<Uri::QueryConverter> Uri::QueryConverter::_thisWeakPtr;
std::mutex& Uri::QueryConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

Uri::QueryConverter::QueryConverter() {}
Uri::QueryConverter::~QueryConverter() {}

std::shared_ptr<Uri::QueryConverter> Uri::QueryConverter::GetSharedInstance()
{
	std::shared_ptr<Uri::QueryConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new QueryConverter());
	_thisWeakPtr = p;
	return p;
}

Uri::QueryConverter& Uri::QueryConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool Uri::QueryConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Query))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool Uri::QueryConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Query))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any Uri::QueryConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Query)))
		{
			if (type->IsReference)
			{
				return Query(AnyVisitor<Query&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Query* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Query res = obj.as<Query>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Query result = Query();
				if (Query::TryParse(std::wstring(tmp), loc, result))
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

Any Uri::QueryConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Query&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Query* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Query v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Query)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Query&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Query* p = obj;
			if (p)
				return *p;
			else
			{
				Query v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any Uri::QueryConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Query result = Query();
	if (Query::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring Uri::QueryConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Query)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Query&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Query* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Query v = obj;
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
#pragma endregion

#pragma region Fragment

Uri::Fragment::Fragment()  {}
Uri::Fragment::Fragment(const std::wstring& v) 
{
	std::wstring::const_iterator first = v.begin();
	if (!UriHelper::Parse(first, v.end(), *this) || first != v.end())
		throw std::invalid_argument("invalid URI Fragment");
}

std::wstring Uri::Fragment::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return EncodedString;
}

bool UriHelper::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Fragment& v)
{
	std::wstring::const_iterator f = first;
	Uri::Fragment tmp;
	UriHelper::Parse(UriHelper::FRAGMENT_TRAITS, f, last, tmp.value);
	if (tmp.IsEmpty) return false;
	if (!UriHelper::Decode(tmp.value)) return false;
	v = tmp;
	first = f;
	return true;
}

bool Uri::Fragment::GetIsEmpty() const { return value.empty(); }

std::wstring Uri::Fragment::GetEncodedString() const
{
	return UriHelper::Encode(UriHelper::FRAGMENT_TRAITS, value);
}

const std::wstring& Uri::Fragment::GetDecodedString() const
{
	return value;
}
bool Uri::Fragment::TryParse(const std::wstring& input, const CultureInfo& loc, Uri::Fragment& result)
{
	std::wstring::const_iterator first = input.begin();
	return UriHelper::Parse(first, input.end(), result) || first != input.end();
}
__inline static Object* Factory_Fragment_0() { return new Uri::Fragment(); }
__inline static Object* Factory_Fragment_1(const std::wstring& str) { return new Uri::Fragment(str); }

__uregister_class1(Uri::Fragment, Fragment, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Uri::Fragment>(L"Fragment", ConstructorFlags::Default, &Factory_Fragment_0);
	type.AddConstructor(L"Fragment", ConstructorFlags::Default, ConstructorTypeAdapter<1, Fragment>::Create(&Factory_Fragment_1));

	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Fragment::GetIsEmpty));
	type.AddProperty(L"EncodedString", PropertyFlags::Public, PropertyTypeOf(&Fragment::GetEncodedString));
	type.AddProperty(L"DecodedString", PropertyFlags::Public, PropertyTypeOf(&Fragment::GetDecodedString));

	type.AddMethod(L"GetIsEmpty", MethodFlags::Public, MethodTypeAdapter<0, Fragment, bool>::Create(&Fragment::GetIsEmpty));
	type.AddMethod(L"GetEncodedString", MethodFlags::Public, MethodTypeAdapter<0, Fragment, std::wstring>::Create(&Fragment::GetEncodedString));
	type.AddMethod(L"GetDecodedString", MethodFlags::Public, MethodTypeAdapter<0, Fragment, const std::wstring&>::Create(&Fragment::GetDecodedString));

	BuiltInAssemblyFactory::Regisiter(&type);
};

std::weak_ptr<Uri::FragmentConverter> Uri::FragmentConverter::_thisWeakPtr;
std::mutex& Uri::FragmentConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

Uri::FragmentConverter::FragmentConverter() {}
Uri::FragmentConverter::~FragmentConverter() {}

std::shared_ptr<Uri::FragmentConverter> Uri::FragmentConverter::GetSharedInstance()
{
	std::shared_ptr<Uri::FragmentConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new FragmentConverter());
	_thisWeakPtr = p;
	return p;
}

Uri::FragmentConverter& Uri::FragmentConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool Uri::FragmentConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Fragment))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool Uri::FragmentConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Fragment))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any Uri::FragmentConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Fragment)))
		{
			if (type->IsReference)
			{
				return Fragment(AnyVisitor<Fragment&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Fragment* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Fragment res = obj.as<Fragment>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Fragment result = Fragment();
				if (Fragment::TryParse(std::wstring(tmp), loc, result))
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

Any Uri::FragmentConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Fragment&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Fragment* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Fragment v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Fragment)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Fragment&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Fragment* p = obj;
			if (p)
				return *p;
			else
			{
				Fragment v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any Uri::FragmentConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Fragment result = Fragment();
	if (Fragment::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring Uri::FragmentConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Fragment)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Fragment&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Fragment* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Fragment v = obj;
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
#pragma endregion

#pragma region Uri

Uri::Uri()
{

}

Uri::Uri(const std::wstring& str)
{
	std::wstring::const_iterator first = str.begin();
	if (!UriHelper::Parse(first, str.end(), *this) || first != str.end())
		throw std::invalid_argument("invalid URI");
}
std::wstring Uri::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return EncodedString;
}
bool UriHelper::Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri& v)
{
	std::wstring::const_iterator f = first;
	Uri tmp;
	std::string e;
	if (UriHelper::Parse(f, last, tmp.scheme))
	{
		f = first;
		tmp.scheme = Uri::Scheme();
	}
	std::wstring::const_iterator anchor = f;
	if (UriHelper::ParseLiteral(f, last, UriHelper::AUTHORITY_TRAITS.begin_cstring)) 
	{
		if (!UriHelper::Parse(f, last, tmp.authority))
			f = anchor; 
	}
	else
		f = anchor;
	UriHelper::Parse(f, last, tmp.path);
	if (f != last && *f == UriHelper::QUERY_TRAITS.begin_char) {
		++f;
		UriHelper::Parse(f, last, tmp.query);
	}
	if (f != last && *f == UriHelper::FRAGMENT_TRAITS.begin_char) {
		++f;
		UriHelper::Parse(f, last, tmp.fragment);
	}
	if (tmp.IsEmpty)
		return false;
	v = tmp;
	first = f;
	return true;
}

bool UriHelper::ParseLiteral(std::wstring::const_iterator& first, std::wstring::const_iterator last, const wchar_t* v)
{
	std::wstring::const_iterator f = first;
	if (!v)
		return false;
	for (; *v;) 
	{
		if (f == last)
			return false;
		if (*f++ != *v++)
			return false;
	}
	first = f;
	return true;
}

std::wstring Uri::GetEncodedString() const
{
	std::wstringstream os;
	if (!scheme.IsEmpty)
		os << scheme.Value<< UriHelper::SCHEME_TRAITS.end_char;
	if (!authority.IsEmpty)
		os << UriHelper::AUTHORITY_TRAITS.begin_cstring << authority.EncodedString;
	os << path.EncodedString;
	if (!query.IsEmpty)
		os << UriHelper::QUERY_TRAITS.begin_char << query.EncodedString;
	if (!fragment.IsEmpty)
		os << UriHelper::FRAGMENT_TRAITS.begin_char << fragment.EncodedString;
	return os.str();
}

bool Uri::GetIsEmpty() const
{
	return scheme.IsEmpty && authority.IsEmpty && path.IsEmpty && query.IsEmpty && fragment.IsEmpty;
}

bool Uri::GetIsRelative() const
{
	return scheme.IsEmpty;
}

const Uri::Scheme& Uri::GetScheme() const
{
	return scheme;
}
Uri::Scheme& Uri::GetSchemeRef()
{
	return scheme;
}

const Uri::Authority& Uri::GetAuthority() const
{
	return authority;
}

Uri::Authority& Uri::GetAuthorityRef()
{
	return authority;
}

const Uri::Path& Uri::GetPath()
{
	return path;
}

Uri::Path& Uri::GetPathRef()
{
	return path;
}

const Uri::Query& Uri::GetQuery() const
{
	return query;
}

Uri::Query& Uri::GetQueryRef()
{
	return query;
}

const Uri::Fragment& Uri::GetFragment() const
{
	return fragment;
}

Uri::Fragment& Uri::GetFragmentRef()
{
	return fragment;
}

bool  Uri::TryParse(const std::wstring& input, const CultureInfo& loc, Uri& result)
{
	std::wstring::const_iterator first = input.begin();
	return UriHelper::Parse(first, input.end(), result) || first != input.end();
}

__inline static Object* Factory_Uri_0() { return new Uri(); }
__inline static Object* Factory_Uri_1(const std::wstring& str) { return new Uri(str); }

__uregister_class(Uri, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Uri>(L"Uri", ConstructorFlags::Default, &Factory_Uri_0);
	type.AddConstructor(L"Uri", ConstructorFlags::Default, ConstructorTypeAdapter<1, Uri>::Create(&Factory_Uri_1));

	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf(&Uri::GetIsEmpty));
	type.AddProperty(L"IsRelative", PropertyFlags::Public, PropertyTypeOf(&Uri::GetIsRelative));
	type.AddProperty(L"Scheme", PropertyFlags::Public, PropertyTypeOf(&Uri::GetScheme));
	type.AddProperty(L"SchemeRef", PropertyFlags::Public, PropertyTypeOf(&Uri::GetSchemeRef));
	type.AddProperty(L"Authority", PropertyFlags::Public, PropertyTypeOf(&Uri::GetAuthority));
	type.AddProperty(L"AuthorityRef", PropertyFlags::Public, PropertyTypeOf(&Uri::GetAuthorityRef));
	type.AddProperty(L"Path", PropertyFlags::Public, PropertyTypeOf(&Uri::GetPath));
	type.AddProperty(L"PathRef", PropertyFlags::Public, PropertyTypeOf(&Uri::GetPathRef));
	type.AddProperty(L"Query", PropertyFlags::Public, PropertyTypeOf(&Uri::GetQuery));
	type.AddProperty(L"QueryRef", PropertyFlags::Public, PropertyTypeOf(&Uri::GetQueryRef));
	type.AddProperty(L"Fragment", PropertyFlags::Public, PropertyTypeOf(&Uri::GetFragment));
	type.AddProperty(L"FragmentRef", PropertyFlags::Public, PropertyTypeOf(&Uri::GetFragmentRef));
	type.AddProperty(L"EncodedString", PropertyFlags::Public, PropertyTypeOf(&Uri::GetEncodedString));

	type.AddMethod(L"GetIsEmpty", MethodFlags::Public, MethodTypeAdapter<0, Uri, bool>::Create(&Uri::GetIsEmpty));
	type.AddMethod(L"GetIsRelative", MethodFlags::Public, MethodTypeAdapter<0, Uri, bool>::Create(&Uri::GetIsRelative));
	type.AddMethod(L"GetScheme", MethodFlags::Public, MethodTypeAdapter<0, Uri, const Uri::Scheme&>::Create(&Uri::GetScheme));
	type.AddMethod(L"GetSchemeRef", MethodFlags::Public, MethodTypeAdapter<0, Uri, Uri::Scheme&>::Create(&Uri::GetSchemeRef));
	type.AddMethod(L"GetAuthority", MethodFlags::Public, MethodTypeAdapter<0, Uri, const Uri::Authority&>::Create(&Uri::GetAuthority));
	type.AddMethod(L"GetAuthorityRef", MethodFlags::Public, MethodTypeAdapter<0, Uri, Uri::Authority&>::Create(&Uri::GetAuthorityRef));
	type.AddMethod(L"GetPath", MethodFlags::Public, MethodTypeAdapter<0, Uri, const Uri::Path&>::Create(&Uri::GetPath));
	type.AddMethod(L"GetPathRef", MethodFlags::Public, MethodTypeAdapter<0, Uri, Uri::Path&>::Create(&Uri::GetPathRef));
	type.AddMethod(L"GetQuery", MethodFlags::Public, MethodTypeAdapter<0, Uri, const Uri::Query&>::Create(&Uri::GetQuery));
	type.AddMethod(L"GetQueryRef", MethodFlags::Public, MethodTypeAdapter<0, Uri, Uri::Query&>::Create(&Uri::GetQueryRef));
	type.AddMethod(L"GetFragment", MethodFlags::Public, MethodTypeAdapter<0, Uri, const Uri::Fragment&>::Create(&Uri::GetFragment));
	type.AddMethod(L"GetFragmentRef", MethodFlags::Public, MethodTypeAdapter<0, Uri, Uri::Fragment&>::Create(&Uri::GetFragmentRef)); 
	type.AddMethod(L"GetEncodedString", MethodFlags::Public, MethodTypeAdapter<0, Uri, std::wstring>::Create(&Uri::GetEncodedString));

	BuiltInAssemblyFactory::Regisiter(&type);
};

std::weak_ptr<UriConverter> UriConverter::_thisWeakPtr;
std::mutex& UriConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

UriConverter::UriConverter() {}
UriConverter::~UriConverter() {}

std::shared_ptr<UriConverter> UriConverter::GetSharedInstance()
{
	std::shared_ptr<UriConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new UriConverter());
	_thisWeakPtr = p;
	return p;
}

UriConverter& UriConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool UriConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Uri))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool UriConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Uri))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any UriConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Uri)))
		{
			if (type->IsReference)
			{
				return Uri(AnyVisitor<Uri&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Uri* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Uri res = obj.as<Uri>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Uri result = Uri();
				if (Uri::TryParse(std::wstring(tmp), loc, result))
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

Any UriConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Uri&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Uri* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Uri v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Uri)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Uri&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Uri* p = obj;
			if (p)
				return *p;
			else
			{
				Uri v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any UriConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Uri result = Uri();
	if (Uri::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring UriConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Uri)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Uri&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Uri* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Uri v = obj;
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

#pragma endregion




#pragma region UriHelper

const wchar_t UriHelper::ENCODE_BEGIN_CHAR = '%';
const UriHelper::Traits UriHelper::SCHEME_TRAITS =
{
	0, 0, ':',
	{
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CVA2, CINV, CVA2, CVA2, CINV,
		CVA2, CVA2, CVA2, CVA2, CVA2, CVA2, CVA2, CVA2, CVA2, CVA2, CEND, CINV, CINV, CINV, CINV, CINV,
		CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CINV, CINV,
		CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CINV, CINV, // 127 7F
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
	}
};
const UriHelper::Traits UriHelper::AUTHORITY_TRAITS =
{
	L"//", 0, 0,
	{
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CEND, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, // 127 7F
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
	}
};
const UriHelper::Traits UriHelper::PATH_TRAITS =
{
	0, 0, 0,
	{   // '/' is invalid
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CVAL, CINV, CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CVAL, CINV, CINV,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CINV, CVAL,
		CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CVAL, CINV, // 127 7F
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
	}
};
const UriHelper::Traits UriHelper::QUERY_TRAITS =
{
	0, '?', 0,
	{   // '=' and '&' are invalid
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CVAL, CINV, CINV, CVAL, CVAL, CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CINV, CVAL,
		CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CVAL, CINV, // 127 7F
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
	}
};
const UriHelper::Traits UriHelper::FRAGMENT_TRAITS =
{
	0, '#', 0,
	{
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CVAL, CINV, CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CVAL, CINV, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CINV, CVAL,
		CINV, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL,
		CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CVAL, CINV, CINV, CINV, CVAL, CINV, // 127 7F
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
		CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV, CINV,
	}
};

bool ParseHex(const std::wstring& s, size_t pos, wchar_t& chr)
{
	if (s.size() < pos + 2)
		return false;
	unsigned int v;
	unsigned int c = (unsigned int)s[pos];
	if ('0' <= c && c <= '9')
		v = (c - '0') << 4;
	else if ('A' <= c && c <= 'F')
		v = (10 + (c - 'A')) << 4;
	else if ('a' <= c && c <= 'f')
		v = (10 + (c - 'a')) << 4;
	else
		return false;
	c = (unsigned int)s[pos + 1];
	if ('0' <= c && c <= '9')
		v += c - '0';
	else if ('A' <= c && c <= 'F')
		v += 10 + (c - 'A');
	else if ('a' <= c && c <= 'f')
		v += 10 + (c - 'a');
	else
		return false;
	chr = (wchar_t)v; 
	return true;
}

void AppendHex(wchar_t v, std::wstring& s)
{
	unsigned int c = (unsigned char)v & 0xF0;
	c >>= 4;
	s.insert(s.end(), (wchar_t)((9 < c) ? (c - 10) + 'A' : c + '0'));
	c = v & 0x0F;
	s.insert(s.end(), (wchar_t)((9 < c) ? (c - 10) + 'A' : c + '0'));
}

bool UriHelper::Parse(const Traits& ts, std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& comp, wchar_t* endc)
{
	std::wstring::const_iterator f = first;
	wchar_t ec = 0;
	for (; f != last; ++f) 
	{
		wchar_t c = *f;
		wchar_t cls = ts.char_class[(unsigned char)c];
		if (cls == CEND)
		{
			ec = c;
			break;
		}
		if (cls != CVAL && (f == first || cls != CVA2))
			break;
	}
	if (f == first && !ec)
		return false;
	comp.assign(first, f);
	if (endc)
		*endc = ec;
	first = ec ? f + 1 : f;
	return true;
}

std::wstring UriHelper::Encode(const Traits& ts, const std::wstring& comp)
{
	std::wstring::const_iterator f = comp.begin();
	std::wstring::const_iterator anchor = f;
	std::wstring s;
	for (; f != comp.end();) 
	{
		wchar_t c = *f;
		if (ts.char_class[(unsigned char)c] < CVAL || c == UriHelper::ENCODE_BEGIN_CHAR) 
		{ 
			s.append(anchor, f);
			s.append(1, UriHelper::ENCODE_BEGIN_CHAR);
			AppendHex(c, s); 
			anchor = ++f;
		}
		else
			++f;
	}
	return (anchor == comp.begin()) ? comp : s.append(f, comp.end());
}

bool UriHelper::Decode(std::wstring& s)
{
	size_t pos = s.find(UriHelper::ENCODE_BEGIN_CHAR);
	if (pos == std::string::npos) 
		return true;
	std::wstring v;
	for (size_t i = 0;;) 
	{
		if (pos == std::wstring::npos) 
		{
			v.append(s, i, s.size() - i); 
			break;
		}
		v.append(s, i, pos - i); 
		i = pos + 3; 
		wchar_t c;
		if (!ParseHex(s, pos + 1, c)) 
			return false;
		v.insert(v.end(), c); 
		pos = s.find(UriHelper::ENCODE_BEGIN_CHAR, i);
	}
	s = v;
	return true;
}
#pragma endregion