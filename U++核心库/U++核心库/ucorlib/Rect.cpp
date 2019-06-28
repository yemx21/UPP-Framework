#pragma once

#include "Rect.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "Parser.h"
#include <algorithm>
using namespace System;

Rect::Rect(double left, double top, double width, double height) : l(left), t(top)
{
	if (width < 0) throw std::exception("width can not be negative");
	if (height < 0) throw std::exception("height can not be negative");
	w = width;
	h = height;
}

 Rect::Rect(const Point& xy, const System::Size& size) : l(xy.X), t(xy.Y)
{
	w = size.Width;
	h = size.Height;
	if (w < 0) throw std::exception("width can not be negative");
	if (h < 0) throw std::exception("height can not be negative");
}

 Rect::Rect(const Point& p1, const Point& p2)
{
	l = std::min(p1.X, p2.X);
	t = std::min(p1.Y, p2.Y);
	w = std::max<double>(std::max<double>(p1.X, p2.X) - l, 0);
	h = std::max<double>(std::max<double>(p1.Y, p2.Y) - t, 0);
}

Rect::Rect(const System::Size& size)
{
	l = 0;
	t = 0;
	w = size.Width;
	h = size.Height;
	if (w < 0) throw std::exception("width can not be negative");
	if (h < 0) throw std::exception("height can not be negative");
}

 Rect::Rect() : l(0), t(0), w(0), h(0)
{
}

Rect::~Rect() {}

double Rect::GetLeft() const
{
	return l;
}

double Rect::GetRight() const
{
	if (IsEmpty)
	{
		return -FP_INFINITE;
	}
	return l + w;
}

void Rect::SetRight(double val)
{
	double tw = val - l;
	if (tw<0) throw std::exception("width can not be negative");
	w = tw;
}

double Rect::GetTop() const
{
	return t;
}

double Rect::GetBottom() const
{
	if (IsEmpty)
	{
		return  -FP_INFINITE;
	}
	return t + h;
}

void Rect::SetBottom(double val)
{
	double th = val - t;
	if (th<0) throw std::exception("height can not be negative");
	h = th;
}

void Rect::SetLeft(double val)
{
	l = val;
}

void Rect::SetTop(double val)
{
	t = val;
}

double Rect::GetWidth()const
{
	return w;
}

double& Rect::GetWidthRef()
{
	return w;
}


void Rect::SetWidth(double val)
{
	if (val<0) throw std::exception("width can not be negative");
	w = val;
}


double Rect::GetHeight()const
{
	return h;
}

double& Rect::GetHeightRef()
{
	return h;
}


void Rect::SetHeight(double val)
{
	if (val<0) throw std::exception("height can not be negative");
	h = val;
}

Point Rect::GetLocation() const
{
	return Point(l, t);
}

void Rect::SetLocation(const Point& point)
{
	l = point.X;
	t = point.Y;
}

System::Size Rect::GetSize() const
{
	if (IsEmpty) return System::Size();
	return System::Size(w, h);
}


void Rect::SetSize(const System::Size& size)
{
	if (size.IsOneDimensionalOrEmpty)
	{
		w = -FP_INFINITE;
		return;
	}
	if (IsEmpty)
	{
		throw std::exception("empty rect can not be modified");
	}
	w = size.Width;
	h = size.Height;
}

bool Rect::GetIsEmpty() const
{
	return  w < 0 || h < 0;
}

bool Rect::Contains(double x, double y) const
{
	if (IsEmpty) return false;
	if (x >= l && x - w <= l && y >= t)
		return y - h <= t;
	else
		return false;
}

 bool Rect::Contains(const Point& point) const
{
	return Contains(point.X, point.Y);
}

bool Rect::Contains(const Rect& rect) const
{
	if (IsEmpty || rect.IsEmpty || (l > rect.l || t > rect.t) || l + w < rect.l + rect.w)
		return false;
	else
		return t + h >= rect.t + rect.h;
}

void Rect::Inflate(const System::Size& size)
{
	Inflate(size.Width, size.Height);
}

void Rect::Inflate(double width, double height)
{
	if (IsEmpty) throw std::exception("rect is empty");
	l -= width;
	t -= height;
	w += width;
	w += width;
	h += height;
	h += height;
	if (w >= 0.0 && h >= 0.0) return;
	l = 0;
	t = 0;
	w = -FP_INFINITE;
	h = -FP_INFINITE;
}

Rect Rect::Empty()
{
	return Rect();
}

Rect Rect::Inflate(const Rect& rect, const System::Size& size)
{
	Rect result = rect;
	result.Inflate(size.Width, size.Height);
	return result;
}

Rect Rect::Inflate(const Rect& rect, double width, double height)
{
	Rect result = rect;
	result.Inflate(width, height);
	return result;
}

void Rect::Scale(double scaleX, double scaleY)
{
	if (IsEmpty) return;
	l *= scaleX;
	t *= scaleY;
	w *= scaleX;
	h *= scaleY;
	if (scaleX < 0.0)
	{
		l += w;
		w *= -1.0;
	}
	if (scaleY >= 0.0) return;
	t += h;
	h *= -1.0;
}


bool Rect::IntersectsWith(const Rect& rect)
{
	if (IsEmpty || rect.IsEmpty || (rect.l > l + w || rect.Right <l) || rect.t > t + h)
		return false;
	else
		return rect.Bottom >= t;
}

void Rect::Intersect(const Rect& rect)
{
	if (!IntersectsWith(rect))
	{
		w = -FP_INFINITE;
		h = -FP_INFINITE;
	}
	else
	{
		double num1 = std::max(l, rect.l);
		double num2 = std::max(t, rect.t);
		double r = l + w;
		double rr = rect.Right;
		double b = t + h;
		double rb = rect.Bottom;
		w = std::max(std::min(r, rr) - num1, 0.0);
		h = std::max(std::min(b, rb) - num2, 0.0);
		l = num1;
		t = num2;
	}
}

Rect Rect::Intersect(const Rect& rect1, const Rect& rect2)
{
	Rect result = rect1;
	result.Intersect(rect2);
	return result;
}

void Rect::Union(const Rect& rect)
{
	if (IsEmpty)
	{
		l = rect.l;
		t = rect.t;
		w = rect.w;
		h = rect.h;
	}
	else
	{
		if (rect.IsEmpty) return;
		double num1 = std::min(l, rect.l);
		double num2 = std::min(t, rect.t);
		double r = l + w;
		double rr = rect.Right;
		double b = t + h;
		double rb = rect.Bottom;
		w = !isfinite(rect.w) || !isfinite(w) ? -FP_INFINITE : std::max(std::max(r, rr) - num1, 0.0);
		h = !isfinite(rect.h) || !isfinite(h) ? -FP_INFINITE : std::max(std::max(b, rb) - num2, 0.0);
		l = num1;
		t = num2;
	}
}

Rect Rect::Union(const Rect& rect1, const Rect& rect2)
{
	Rect result = rect1;
	result.Union(rect2);
	return result;
}

void Rect::Union(const Point& point)
{
	Union(Rect(point, point));
}

Rect Rect::Union(const Rect& rect, const Point& point)
{
	Rect result = rect;
	result.Union(point);
	return result;
}

void Rect::Offset(double offsetX, double offsetY)
{
	if (IsEmpty) throw std::exception("rect is empty");
	l += offsetX;
	t += offsetY;
}

Rect Rect::Offset(const Rect& rect, double offsetX, double offsetY)
{
	Rect result = rect;
	result.Offset(offsetX, offsetY);
	return result;
}


Rect::Rect(const Rect& rect)
{
	l = rect.l;
	t = rect.t;
	w = rect.w;
	h = rect.h;
}

Rect & Rect::operator = (const Rect & rect)
{
	l = rect.l;
	t = rect.t;
	w = rect.w;
	h = rect.h;
	return *this;
}

bool Rect::operator == (Rect& rect) const
{
	return rect.l == l && rect.t == t && rect.w == w && rect.h == h;
}

bool Rect::operator == (const Rect& rect) const
{
	return rect.l == l && rect.t == t && rect.w == w && rect.h == h;
}

bool Rect::operator != (Rect& rect) const
{
	return rect.l != l || rect.t != t || rect.w != w || rect.h != h;
}

bool Rect::operator != (const Rect& rect) const
{
	return rect.l != l || rect.t != t || rect.w != w || rect.h != h;
}

Rect Rect::Unknown()
{
	return Rect(FP_NAN, FP_NAN, FP_NAN, FP_NAN);
}

bool Rect::GetIsUnknown() const
{
	return isnan(l) || isnan(t) || isnan(w) || isnan(h);
}

std::wstring Rect::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return  Formatter::Format(L"{0:r},{1:r},{2:r},{3:r}", loc, (VariadicAny(l), t, w, h));
}


bool Rect::TryParse(const std::wstring& input, const CultureInfo& loc, Rect& result)
{
	std::vector<Float64> subs;
	subs.reserve(4);
	Parser::Parse(input, subs, ',');
	if (subs.size() == 4)
	{
		result.l = subs[0];
		result.t = subs[1];
		result.w = subs[2];
		result.h = subs[3];
		return true;
	}
	return false;
}

std::weak_ptr<RectConverter> RectConverter::_thisWeakPtr;
std::mutex& RectConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

RectConverter::RectConverter() {}
RectConverter::~RectConverter() {}

std::shared_ptr<RectConverter> RectConverter::GetSharedInstance()
{
	std::shared_ptr<RectConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new RectConverter());
	_thisWeakPtr = p;
	return p;
}

RectConverter& RectConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool RectConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Rect))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool RectConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Rect))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any RectConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Rect)))
		{
			if (type->IsReference)
			{
				return Rect(AnyVisitor<Rect&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Rect* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Rect res = obj.as<Rect>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Rect result = Rect();
				if (Rect::TryParse(std::wstring(tmp), loc, result))
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

Any RectConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Rect&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Rect* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Rect v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Rect)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Rect&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Rect* p = obj;
			if (p)
				return *p;
			else
			{
				Rect v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any RectConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Rect result = Rect();
	if (Rect::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring RectConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Rect)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Rect&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Rect* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Rect v = obj;
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

__inline static Object* Factory_Rect_0() { return new Rect(); }
__inline static Object* Factory_Rect_1(double left, double top, double width, double height) { return new Rect(left,  top, width, height); }
__inline static Object* Factory_Rect_2(const Point& xy, const Size& size) { return new Rect(xy, size); }

__uregister_class(Rect, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Rect>(L"Rect", ConstructorFlags::Default, &Factory_Rect_0);
	type.AddConstructor(L"Rect", ConstructorFlags::Default, ConstructorTypeAdapter<4, Rect>::Create(&Factory_Rect_1));
	type.AddConstructor(L"Rect", ConstructorFlags::Default, ConstructorTypeAdapter<2, Rect>::Create(&Factory_Rect_2));

	type.AddMethod(L"Contains", MethodFlags::Public, MethodTypeAdapter<2,Rect, bool>::Create(&Rect::Contains));
	type.AddMethod(L"Contains", MethodFlags::Public, MethodTypeAdapter<1, Rect, bool>::Create<const Point&>(&Rect::Contains));
	type.AddMethod(L"Contains", MethodFlags::Public, MethodTypeAdapter<1, Rect, bool>::Create<const Rect&>(&Rect::Contains));

	type.AddMethod(L"Inflate", MethodFlags::Public, MethodTypeAdapter<1, Rect>::Create(&Rect::Inflate));
	type.AddMethod(L"Inflate", MethodFlags::Public, MethodTypeAdapter<2, Rect>::Create<double, double>(&Rect::Inflate));
	type.AddMethod(L"Inflate", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Rect, Rect>::Create<const Rect&, const System::Size&>(&Rect::Inflate));
	type.AddMethod(L"Inflate", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<3, Rect, Rect>::Create(&Rect::Inflate));

	type.AddMethod(L"Scale", MethodFlags::Public, MethodTypeAdapter<2, Rect>::Create(&Rect::Scale));

	type.AddMethod(L"IntersectsWith", MethodFlags::Public, MethodTypeAdapter<1, Rect, bool>::Create(&Rect::IntersectsWith));
	type.AddMethod(L"Intersect", MethodFlags::Public, MethodTypeAdapter<1, Rect>::Create(&Rect::Intersect));
	type.AddMethod(L"Intersect", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Rect, Rect>::Create(&Rect::Intersect));

	type.AddMethod(L"Union", MethodFlags::Public, MethodTypeAdapter<1, Rect>::Create<const Rect&>(&Rect::Union));
	type.AddMethod(L"Union", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Rect, Rect>::Create<const Rect&, const Rect&>(&Rect::Union));
	type.AddMethod(L"Union", MethodFlags::Public, MethodTypeAdapter<1, Rect>::Create<const Point&>(&Rect::Union));
	type.AddMethod(L"Union", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Rect, Rect>::Create<const Rect&, const Point&>(&Rect::Union));

	type.AddMethod(L"Offset", MethodFlags::Public, MethodTypeAdapter<2, Rect>::Create(&Rect::Offset));
	type.AddMethod(L"Offset", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<3, Rect, Rect>::Create(&Rect::Offset));

	type.AddProperty(L"Left", PropertyFlags::Public, PropertyTypeOf(&Rect::GetLeft, &Rect::SetLeft));
	type.AddProperty(L"Top", PropertyFlags::Public, PropertyTypeOf(&Rect::GetTop, &Rect::SetTop));
	type.AddProperty(L"Width", PropertyFlags::Public, PropertyTypeOf(&Rect::GetWidth, &Rect::SetWidth));
	type.AddProperty(L"Height", PropertyFlags::Public, PropertyTypeOf(&Rect::GetHeight, &Rect::SetHeight));
	type.AddProperty(L"Right", PropertyFlags::Public, PropertyTypeOf(&Rect::GetRight));
	type.AddProperty(L"Bottom", PropertyFlags::Public, PropertyTypeOf(&Rect::GetBottom));
	type.AddProperty(L"Location", PropertyFlags::Public, PropertyTypeOf<Rect, Point>(&Rect::GetLocation));
	type.AddProperty(L"Size", PropertyFlags::Public, PropertyTypeOf<Rect, System::Size>(&Rect::GetSize, &Rect::SetSize));

	type.AddProperty(L"IsUnknown", PropertyFlags::Public, PropertyTypeOf(&Rect::GetIsUnknown));
	type.AddProperty(L"Unknown", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<Rect, Rect>(&Rect::Unknown));
	type.AddProperty(L"Empty", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<Rect, Rect>(&Rect::Empty));
	type.AddProperty(L"IsEmpty", PropertyFlags::Public, PropertyTypeOf<Rect>(&Rect::GetIsEmpty));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Rect, Rect&>::Create(&Rect::operator=));

	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Rect, bool>::Create<const Rect&>(&Rect::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Rect, bool>::Create<const Rect&>(&Rect::operator!=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Rect, bool>::Create<Rect&>(&Rect::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Rect, bool>::Create<Rect&>(&Rect::operator!=));

	type.AddAttribute(new TypeConverterAttribute(L"RectConverter", RectConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};

