#include "Point.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "Parser.h"
using namespace System;


Point::Point(double px, double py) : x(px), y(py)
{
}

Point::Point() : x(0), y(0)
{
}

Point::~Point() {}


double Point::GetX() const
{
	return x;
}

double Point::GetY() const
{
	return y;
}

double& Point::GetXRef()
{
	return x;
}

double& Point::GetYRef()
{
	return y;
}

void Point::SetX(double val)
{
	x = val;
}

void Point::SetY(double val)
{
	y = val;
}

Point::Point(const Point& point)
{
	x = point.x;
	y = point.y;
}

Point Point::operator + (const Point& point) const
{
	return Point(x + point.x, y + point.y);
}

Point Point::operator - (const Point& point) const
{
	return Point(x - point.x, y - point.y);
}

Point& Point::operator += (Point& point)
{
	x += point.x;
	y += point.y;
	return *this;
}

Point& Point::operator -= (Point& point)
{
	x -= point.x;
	y -= point.y;
	return *this;
}

Point & Point::operator = (const Point & point)
{
	x = point.x;
	y = point.y;
	return *this;
}

bool Point::operator == (Point& point) const
{
	return point.x == x && point.y == y;
}

bool Point::operator == (const Point& point) const
{
	return point.x == x && point.y == y;
}

bool Point::operator != (Point& point) const
{
	return point.x != x || point.y != y;
}

bool Point::operator != (const Point& point) const
{
	return point.x != x || point.y != y;
}

Point Point::Unknown()
{
	return Point(FP_NAN, FP_NAN);
}

bool Point::GetIsUnknown() const
{
	return isnan(x) || isnan(y);
}

std::wstring Point::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return  Formatter::Format(L"{0:r},{1:r}", loc, (VariadicAny(x), y));
}


bool Point::TryParse(const std::wstring& input, const CultureInfo& loc, Point& result)
{
	std::vector<Float64> subs;
	subs.reserve(2);
	Parser::Parse(input, subs, ',');
	if (subs.size() == 2)
	{
		result.x = subs[0];
		result.y = subs[1];
		return true;
	}
	return false;
}

std::weak_ptr<PointConverter> PointConverter::_thisWeakPtr;
std::mutex& PointConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

PointConverter::PointConverter() {}
PointConverter::~PointConverter() {}

std::shared_ptr<PointConverter> PointConverter::GetSharedInstance()
{
	std::shared_ptr<PointConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new PointConverter());
	_thisWeakPtr = p;
	return p;
}

PointConverter& PointConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool PointConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Point))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool PointConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Point))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any PointConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Point)))
		{
			if (type->IsReference)
			{
				return Point(AnyVisitor<Point&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Point* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Point res = obj.as<Point>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Point result = Point();
				if (Point::TryParse(std::wstring(tmp), loc, result))
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

Any PointConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Point&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Point* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Point v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Point)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Point&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Point* p = obj;
			if (p)
				return *p;
			else
			{
				Point v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any PointConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Point result = Point();
	if (Point::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring PointConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Point)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Point&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Point* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Point v = obj;
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

__inline static Object* Factory_Point_0() { return new Point(); }
__inline static Object* Factory_Point_1(double px, double py) { return new Point(px, py); }

__uregister_class(Point, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Point>(L"Point", ConstructorFlags::Default, &Factory_Point_0);
	type.AddConstructor(L"Point", ConstructorFlags::Default, ConstructorTypeAdapter<2, Point>::Create(&Factory_Point_1));

	type.AddProperty(L"X", PropertyFlags::Public, PropertyTypeOf(&Point::GetX, &Point::SetX));
	type.AddProperty(L"Y", PropertyFlags::Public, PropertyTypeOf(&Point::GetY, &Point::SetY));
	type.AddProperty(L"XRef", PropertyFlags::Public, PropertyTypeOf(&Point::GetXRef));
	type.AddProperty(L"YRef", PropertyFlags::Public, PropertyTypeOf(&Point::GetYRef));
	type.AddProperty(L"IsUnknown", PropertyFlags::Public, PropertyTypeOf(&Point::GetIsUnknown));
	type.AddProperty(L"Unknown", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<Point, Point>(&Point::Unknown));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Point, Point&>::Create(&Point::operator=));
	type.AddOperator(L"Add", OperatorTypeAdapter<1, Operators::Addition, Point, Point>::Create(&Point::operator+));
	type.AddOperator(L"Subtract", OperatorTypeAdapter<1, Operators::Subtraction, Point, Point>::Create(&Point::operator-));
	type.AddOperator(L"AddAssign", OperatorTypeAdapter<1, Operators::AdditionAssignment, Point, Point&>::Create(&Point::operator+=));
	type.AddOperator(L"SubtractAssign", OperatorTypeAdapter<1, Operators::SubtractionAssignment, Point, Point&>::Create(&Point::operator-=));

	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Point, bool>::Create<const Point&>(&Point::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Point, bool>::Create<const Point&>(&Point::operator!=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Point, bool>::Create<Point&>(&Point::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Point, bool>::Create<Point&>(&Point::operator!=));

	type.AddAttribute(new TypeConverterAttribute(L"PointConverter", PointConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};
