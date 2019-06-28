#include "Attribute.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"



namespace System
{
	class Attribute_impl
	{
	public:
		InertiaString str;
		AttributeFlags _flag;
		AttributePolicy _policy;
		bool _allowMultiple;
		bool _inherited;
		Attribute_impl(const wchar_t* name, AttributeFlags flag, AttributePolicy policy, bool allowMultiple, bool inherited) :str(name), _flag(flag), _policy(policy), _allowMultiple(allowMultiple), _inherited(inherited)
		{

		}

		~Attribute_impl() 
		{
		}
	};
}

Attribute::Attribute(const wchar_t* name, AttributeFlags flag, AttributePolicy policy, bool allowMultiple, bool inherited)
{
	impl = new Attribute_impl(name, flag, policy, allowMultiple, inherited);
}

Attribute::~Attribute()
{
	if (impl){ delete impl; impl = nullptr; }
}


AttributeFlags Attribute::GetFlag() const
{
	return impl != nullptr ? impl->_flag : AttributeFlags::Custom;
}

const wchar_t* Attribute::GetName() const
{
	return impl != nullptr ? impl->str->c_str() : nullptr;
}

TypeConverter::~TypeConverter() {}

TypeConverterAttribute::~TypeConverterAttribute()
{

}


bool TypeConverterAttribute::CanConvertFrom(_type type) const
{
	return _converter->CanConvertFrom(type);
}

bool TypeConverterAttribute::CanConvertTo(_type type) const
{
	return _converter->CanConvertFrom(type);
}

Any TypeConverterAttribute::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	return _converter->ConvertFrom(obj, loc);
}

Any TypeConverterAttribute::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
{
	return _converter->ConvertTo(obj, type, loc);
}

Any TypeConverterAttribute::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	return _converter->ConvertFromString(str, loc);
}

std::wstring TypeConverterAttribute::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	return _converter->ConvertToString(obj, loc);
}