#include "CornerRadius.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "Parser.h"
using namespace System;


CornerRadius::CornerRadius() : bl(0), tl(0), br(0), tr(0)
{
}

CornerRadius::~CornerRadius() {}

CornerRadius::CornerRadius(double topLeft, double topRight, double bottomRight, double bottomLeft) :
bl(bottomLeft), tl(topLeft), br(bottomRight), tr(topRight)
{
}

double CornerRadius::GetTopLeft() const
{
	return tl;
}

double CornerRadius::GetTopRight() const
{
	return tr;
}

double CornerRadius::GetBottomLeft() const
{
	return bl;
}

double CornerRadius::GetBottomRight() const
{
	return br;
}

double& CornerRadius::GetTopLeftRef()
{
	return tl;
}


double& CornerRadius::GetTopRightRef()
{
	return tr;
}

double& CornerRadius::GetBottomLeftRef()
{
	return bl;
}

double& CornerRadius::GetBottomRightRef()
{
	return br;
}

void CornerRadius::SetTopLeft(double val)
{
	tl = val;
}

void CornerRadius::SetTopRight(double val)
{
	tr = val;
}

void CornerRadius::SetBottomLeft(double val)
{
	bl = val;
}

void CornerRadius::SetBottomRight(double val)
{
	br = val;
}

bool CornerRadius::operator == (const CornerRadius& cornerRadius) const
{
	if (tl != cornerRadius.tl && (!isnan(tl) || !isnan(cornerRadius.tl)) || tr != cornerRadius.tr && (!isnan(tr) || !isnan(cornerRadius.tr)) || bl != cornerRadius.bl && (!isnan(bl)))
	{
		return false;
	}
	if (br == cornerRadius.br)
	{
		return true;
	}
	if (!isnan(br))
	{
		return false;
	}
	return isnan(cornerRadius.br);
}

bool CornerRadius::operator == (CornerRadius& cornerRadius) const
{
	if (tl != cornerRadius.tl && (!isnan(tl) || !isnan(cornerRadius.tl)) || tr != cornerRadius.tr && (!isnan(tr) || !isnan(cornerRadius.tr)) || bl != cornerRadius.bl && (!isnan(bl) || !isnan(cornerRadius.bl)))
	{
		return false;
	}
	if (br == cornerRadius.br)
	{
		return true;
	}
	if (!isnan(br))
	{
		return false;
	}
	return isnan(cornerRadius.br);
}

CornerRadius & CornerRadius::operator = (const CornerRadius & cornerRadius)
{
	tl = cornerRadius.tl;
	tr = cornerRadius.tr;
	bl = cornerRadius.bl;
	br = cornerRadius.br;
	return *this;
}

bool CornerRadius::operator != (const CornerRadius& cornerRadius) const
{
	return !(operator==(cornerRadius));
}

bool CornerRadius::operator != (CornerRadius& cornerRadius) const
{
	return !(operator==(cornerRadius));
}



std::wstring CornerRadius::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return  Formatter::Format(L"{0:r},{1:r},{2:r},{3:r}", loc, (VariadicAny(tl), tr, bl, br));
}


bool CornerRadius::TryParse(const std::wstring& input, const CultureInfo& loc, CornerRadius& result)
{
	std::vector<Float64> subs;
	subs.reserve(4);
	Parser::Parse(input, subs, ',');
	if (subs.size() == 4)
	{
		result.tl = subs[0];
		result.tr = subs[1];
		result.bl = subs[2];
		result.br = subs[3];
		return true;
	}
	return false;
}

std::weak_ptr<CornerRadiusConverter> CornerRadiusConverter::_thisWeakPtr;
std::mutex& CornerRadiusConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

CornerRadiusConverter::CornerRadiusConverter() {}
CornerRadiusConverter::~CornerRadiusConverter() {}

std::shared_ptr<CornerRadiusConverter> CornerRadiusConverter::GetSharedInstance()
{
	std::shared_ptr<CornerRadiusConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new CornerRadiusConverter());
	_thisWeakPtr = p;
	return p;
}

CornerRadiusConverter& CornerRadiusConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool CornerRadiusConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(CornerRadius))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool CornerRadiusConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(CornerRadius))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any CornerRadiusConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(CornerRadius)))
		{
			if (type->IsReference)
			{
				return CornerRadius(AnyVisitor<CornerRadius&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			CornerRadius* p = obj;
			if (p)
				return p->ToString();
			else
			{
				CornerRadius res = obj.as<CornerRadius>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				CornerRadius result = CornerRadius();
				if (CornerRadius::TryParse(std::wstring(tmp), loc, result))
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

Any CornerRadiusConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<CornerRadius&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				CornerRadius* p = obj;
				if (p)
					return p->ToString();
				else
				{
					CornerRadius v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(CornerRadius)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<CornerRadius&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			CornerRadius* p = obj;
			if (p)
				return *p;
			else
			{
				CornerRadius v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any CornerRadiusConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	CornerRadius result = CornerRadius();
	if (CornerRadius::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring CornerRadiusConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(CornerRadius)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<CornerRadius&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			CornerRadius* p = obj;
			if (p)
				return p->ToString();
			else
			{
				CornerRadius v = obj;
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

__inline static Object* Factory_CornerRadius_0() { return new CornerRadius(); }
__inline static Object* Factory_CornerRadius_1(double pl, double pt, double pr, double pb) { return new CornerRadius(pl, pt, pr, pb); }

__uregister_class(CornerRadius, ClassFlags::Normal)
{
	type.AddDefaultConstructor<CornerRadius>(L"CornerRadius", ConstructorFlags::Default, &Factory_CornerRadius_0);
	type.AddConstructor(L"CornerRadius", ConstructorFlags::Default, ConstructorTypeAdapter<4, CornerRadius>::Create(&Factory_CornerRadius_1));


	type.AddProperty(L"TopLeft", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetTopLeft, &CornerRadius::SetTopLeft));
	type.AddProperty(L"TopRight", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetTopRight, &CornerRadius::SetTopRight));
	type.AddProperty(L"Right", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetBottomLeft, &CornerRadius::SetBottomLeft));
	type.AddProperty(L"BottomRight", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetBottomRight, &CornerRadius::SetBottomRight));
	type.AddProperty(L"TopLeftRef", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetTopLeftRef));
	type.AddProperty(L"TopRightRef", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetTopRightRef));
	type.AddProperty(L"BottomLeftRef", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetBottomLeftRef));
	type.AddProperty(L"BottomRightRef", PropertyFlags::Public, PropertyTypeOf(&CornerRadius::GetBottomRightRef));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, CornerRadius, CornerRadius&>::Create(&CornerRadius::operator=));

	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, CornerRadius, bool>::Create<const CornerRadius&>(&CornerRadius::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, CornerRadius, bool>::Create<const CornerRadius&>(&CornerRadius::operator!=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, CornerRadius, bool>::Create<CornerRadius&>(&CornerRadius::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, CornerRadius, bool>::Create<CornerRadius&>(&CornerRadius::operator!=));

	type.AddAttribute(new TypeConverterAttribute(L"CornerRadiusConverter", CornerRadiusConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};
