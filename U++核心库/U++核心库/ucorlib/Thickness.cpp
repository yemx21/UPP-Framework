#include "Thickness.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "Parser.h"
using namespace System;


Thickness::Thickness() : l(0), t(0), r(0), b(0)
{
}

Thickness::Thickness(double left, double top, double right, double bottom) : l(left), t(top), r(right), b(bottom)
{
}

Thickness::Thickness(const Thickness& thickness)
{
	l = thickness.l;
	t = thickness.t;
	r = thickness.r;
	b = thickness.b;
}

double Thickness::GetLeft() const
{
	return l;
}

double Thickness::GetTop() const
{
	return t;
}

double Thickness::GetRight() const
{
	return r;
}

double Thickness::GetBottom() const
{
	return b;
}

double& Thickness::GetLeftRef()
{
	return l;
}

double& Thickness::GetTopRef()
{
	return t;
}

double& Thickness::GetRightRef()
{
	return r;
}

double& Thickness::GetBottomRef()
{
	return b;
}

void Thickness::SetLeft(double val)
{
	l = val;
}

void Thickness::SetTop(double val)
{
	t = val;
}

void Thickness::SetRight(double val)
{
	r = val;
}

void Thickness::SetBottom(double val)
{
	b = val;
}

bool Thickness::operator == (const Thickness& thickness) const
{
	if (l != thickness.l && (!isnan(l) || !isnan(thickness.l)) || t != thickness.t && (!isnan(t) || !isnan(thickness.t)) || r != thickness.r && (!isnan(r) || !isnan(thickness.r)))
	{
		return false;
	}
	if (b == thickness.b)
	{
		return true;
	}
	if (!isnan(b))
	{
		return false;
	}
	return isnan(thickness.b);
}

bool Thickness::operator == (Thickness& thickness) const
{
	if (l != thickness.l && (!isnan(l) || !isnan(thickness.l)) || t != thickness.t && (!isnan(t) || !isnan(thickness.t)) || r != thickness.r && (!isnan(r) || !isnan(thickness.r)))
	{
		return false;
	}
	if (b == thickness.b)
	{
		return true;
	}
	if (!isnan(b))
	{
		return false;
	}
	return isnan(thickness.b);
}

Thickness & Thickness::operator = (const Thickness & thickness)
{
	l = thickness.l;
	t = thickness.t;
	r = thickness.r;
	b = thickness.b;
	return *this;
}

bool Thickness::operator != (const Thickness& thickness) const
{
	return !(operator==(thickness));
}

bool Thickness::operator != (Thickness& thickness) const
{
	return !(operator==(thickness));
}

std::wstring Thickness::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return  Formatter::Format(L"{0:r},{1:r},{2:r},{3:r}", loc, (VariadicAny(l), t, r, b));
}


bool Thickness::TryParse(const std::wstring& input, const CultureInfo& loc, Thickness& result)
{
	std::vector<Float64> subs;
	subs.reserve(4);
	Parser::Parse(input, subs, ',');
	if (subs.size() == 4)
	{
		result.l = subs[0];
		result.t = subs[1];
		result.r = subs[2];
		result.b = subs[3];
		return true;
	}
	return false;
}

std::weak_ptr<ThicknessConverter> ThicknessConverter::_thisWeakPtr;
std::mutex& ThicknessConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

ThicknessConverter::ThicknessConverter() {}
ThicknessConverter::~ThicknessConverter() {}

std::shared_ptr<ThicknessConverter> ThicknessConverter::GetSharedInstance()
{
	std::shared_ptr<ThicknessConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new ThicknessConverter());
	_thisWeakPtr = p;
	return p;
}

ThicknessConverter& ThicknessConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool ThicknessConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Thickness))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool ThicknessConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Thickness))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any ThicknessConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Thickness)))
		{
			if (type->IsReference)
			{
				return Thickness(AnyVisitor<Thickness&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Thickness* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Thickness res = obj.as<Thickness>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Thickness result = Thickness();
				if (Thickness::TryParse(std::wstring(tmp), loc, result))
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

Any ThicknessConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Thickness&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Thickness* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Thickness v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Thickness)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Thickness&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Thickness* p = obj;
			if (p)
				return *p;
			else
			{
				Thickness v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any ThicknessConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Thickness result = Thickness();
	if (Thickness::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring ThicknessConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Thickness)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Thickness&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Thickness* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Thickness v = obj;
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

__inline static Object* Factory_Thickness_0() { return new Thickness(); }
__inline static Object* Factory_Thickness_1(double pl, double pt, double pr, double pb) { return new Thickness(pl, pt, pr, pb); }

__uregister_class(Thickness, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Thickness>(L"Thickness", ConstructorFlags::Default, &Factory_Thickness_0);
	type.AddConstructor(L"Thickness", ConstructorFlags::Default, ConstructorTypeAdapter<4, Thickness>::Create(&Factory_Thickness_1));


	type.AddProperty(L"Left", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetLeft, &Thickness::SetLeft));
	type.AddProperty(L"Top", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetTop, &Thickness::SetTop));
	type.AddProperty(L"Right", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetRight, &Thickness::SetRight));
	type.AddProperty(L"Bottom", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetBottom, &Thickness::SetBottom));
	type.AddProperty(L"LeftRef", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetLeftRef));
	type.AddProperty(L"TopRef", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetTopRef));
	type.AddProperty(L"RightRef", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetRightRef));
	type.AddProperty(L"BottomRef", PropertyFlags::Public, PropertyTypeOf(&Thickness::GetBottomRef));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Thickness, Thickness&>::Create(&Thickness::operator=));

	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Thickness, bool>::Create<const Thickness&>(&Thickness::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Thickness, bool>::Create<const Thickness&>(&Thickness::operator!=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Thickness, bool>::Create<Thickness&>(&Thickness::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Thickness, bool>::Create<Thickness&>(&Thickness::operator!=));

	type.AddAttribute(new TypeConverterAttribute(L"ThicknessConverter", ThicknessConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};
