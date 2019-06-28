#include "Size.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "Parser.h"
using namespace System;

Size::Size(double pw, double ph) : w(pw), h(ph)
{
}

Size::Size() : w(0), h(0)
{
}

Size::~Size() {}


double Size::GetWidth() const
{
	return w;
}

double Size::GetHeight() const
{
	return h;
}


void Size::SetWidth(double val)
{
	w = val;
}

void Size::SetHeight(double val)
{
	h = val;
}

double& Size::GetWidthRef()
{
	return w;
}


double& Size::GetHeightRef()
{
	return h;
}

Size Size::Scale(double scale) const
{
	return Size(w * scale, h * scale);
}

void Size::Scale(double scale)
{
	w *= scale;
	h *= scale;
}

Size::Size(const Size& size)
{
	w = size.w;
	h = size.h;
}

Size Size::operator + (const Size& size) const
{
	return Size(w + size.w, h + size.h);
}

Size Size::operator - (const Size& size) const
{
	return Size(w - size.w, h - size.h);
}

Size& Size::operator += (Size& size)
{
	w += size.w;
	h += size.h;
	return *this;
}

Size& Size::operator -= (Size& size)
{
	w -= size.w;
	h -= size.h;
	return *this;
}

Size & Size::operator = (const Size & size)
{
	w = size.w;
	h = size.h;
	return *this;
}

bool Size::operator == (Size& size) const
{
	return size.w == w && size.h == h;
}

bool Size::operator == (const Size& size) const
{
	return size.w == w && size.h == h;
}

bool Size::operator != (Size& size) const
{
	return size.w != w || size.h != h;
}

bool Size::operator != (const Size& size) const
{
	return size.w != w || size.h != h;
}

Size Size::Unknown()
{
	return Size(FP_NAN, FP_NAN);
}

__inline Size Size::Infitite()
{
	Size infitite;
	infitite.w = std::numeric_limits<double>::infinity();
	infitite.h = std::numeric_limits<double>::infinity();
	return infitite;
}

bool Size::GetIsUnknown() const
{
	return isnan(w) || isnan(h);
}

bool Size::GetIsOneDimensionalOrEmpty() const
{
	return w == 0.0 || h == 0.0;
}

std::wstring Size::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return  Formatter::Format(L"{0:r},{1:r}", loc, (VariadicAny(w), h));
}

bool Size::TryParse(const std::wstring& input, const CultureInfo& loc, Size& result)
{
	std::vector<Float64> subs;
	subs.reserve(2);
	Parser::Parse(input, subs, ',');
	if (subs.size() == 2)
	{
		result.w = subs[0];
		result.h = subs[1];
		return true;
	}
	return false;
}

std::weak_ptr<SizeConverter> SizeConverter::_thisWeakPtr;
std::mutex& SizeConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

SizeConverter::SizeConverter() {}
SizeConverter::~SizeConverter() {}

std::shared_ptr<SizeConverter> SizeConverter::GetSharedInstance()
{
	std::shared_ptr<SizeConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new SizeConverter());
	_thisWeakPtr = p;
	return p;
}

SizeConverter& SizeConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool SizeConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Size))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool SizeConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Size))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any SizeConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Size)))
		{
			if (type->IsReference)
			{
				return Size(AnyVisitor<Size&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Size* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Size res = obj.as<Size>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Size result = Size();
				if (Size::TryParse(std::wstring(tmp), loc, result))
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

Any SizeConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Size&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Size* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Size v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Size)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Size&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Size* p = obj;
			if (p)
				return *p;
			else
			{
				Size v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any SizeConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Size result = Size();
	if (Size::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring SizeConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Size)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Size&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Size* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Size v = obj;
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

__inline static Object* Factory_Size_0() { return new Size(); }
__inline static Object* Factory_Size_1(double px, double py) { return new Size(px, py); }

__uregister_class(Size, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Size>(L"Size", ConstructorFlags::Default, &Factory_Size_0);
	type.AddConstructor(L"Size", ConstructorFlags::Default, ConstructorTypeAdapter<2, Size>::Create(&Factory_Size_1));

	type.AddMethod(L"Scale", MethodFlags::Public, MethodTypeAdapter<1, Size, Size>::Create(&Size::Scale));
	type.AddMethod(L"Scale", MethodFlags::Public, MethodTypeAdapter<1, Size>::Create(&Size::Scale));

	type.AddProperty(L"Width", PropertyFlags::Public, PropertyTypeOf(&Size::GetWidth, &Size::SetWidth));
	type.AddProperty(L"Height", PropertyFlags::Public, PropertyTypeOf(&Size::GetHeight, &Size::SetHeight));
	type.AddProperty(L"WidthRef", PropertyFlags::Public, PropertyTypeOf(&Size::GetWidthRef));
	type.AddProperty(L"HeightRef", PropertyFlags::Public, PropertyTypeOf(&Size::GetHeightRef));
	type.AddProperty(L"IsUnknown", PropertyFlags::Public, PropertyTypeOf(&Size::GetIsUnknown));
	type.AddProperty(L"IsOneDimensionalOrEmpty", PropertyFlags::Public, PropertyTypeOf(&Size::GetIsOneDimensionalOrEmpty));
	type.AddProperty(L"Unknown", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<Size, Size>(&Size::Unknown));
	type.AddProperty(L"Infitite", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<Size, Size>(&Size::Infitite));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Size, Size&>::Create(&Size::operator=));
	type.AddOperator(L"Add", OperatorTypeAdapter<1, Operators::Addition, Size, Size>::Create(&Size::operator+));
	type.AddOperator(L"Subtract", OperatorTypeAdapter<1, Operators::Subtraction, Size, Size>::Create(&Size::operator-));
	type.AddOperator(L"AddAssign", OperatorTypeAdapter<1, Operators::AdditionAssignment, Size, Size&>::Create(&Size::operator+=));
	type.AddOperator(L"SubtractAssign", OperatorTypeAdapter<1, Operators::SubtractionAssignment, Size, Size&>::Create(&Size::operator-=));

	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Size, bool>::Create<const Size&>(&Size::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Size, bool>::Create<const Size&>(&Size::operator!=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Size, bool>::Create<Size&>(&Size::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Size, bool>::Create<Size&>(&Size::operator!=));

	type.AddAttribute(new TypeConverterAttribute(L"SizeConverter", SizeConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};
