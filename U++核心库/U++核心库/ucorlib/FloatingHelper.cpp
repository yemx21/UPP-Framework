#include "FloatingHelper.h"
#include <float.h>
#include <limits>
#include <math.h>

using namespace System;

static const float FLT_MAX_PRECISION = 16777215.f;
static const float INVERSE_FLT_MAX_PRECISION = 1.f / FLT_MAX_PRECISION;

bool FloatingHelper::AreClose(float a, float b)
{
	if (a == b) return true;
	float single = (abs(a) + abs(b) + 10.f) * FLT_EPSILON;
	float single1 = a - b;
	if (-single >= single1) return false;
	return single > single1;
}

bool FloatingHelper::IsCloseToDivideByZero(float numerator, float denominator)
{
	return abs(denominator) <= abs(numerator) * INVERSE_FLT_MAX_PRECISION;
}

bool FloatingHelper::IsOne(float a)
{
	return abs(a - 1.f) < 10.f * FLT_EPSILON;
}

bool FloatingHelper::IsZero(float a)
{
	return abs(a) < 10.f * FLT_EPSILON;
}


bool FloatingHelper::AreClose(double value1, double value2)
{
	if (value1 == value2) return true;
	double num = (abs(value1) + abs(value2) + 10) * 2.22044604925031E-16;
	double num1 = value1 - value2;
	if (-num >= num1) return false;
	return num > num1;
}

bool FloatingHelper::AreClose(const Point& point1, const Point& point2)
{
	if (!AreClose(point1.X, point2.X)) return false;
	return AreClose(point1.Y, point2.Y);
}

bool FloatingHelper::AreClose(const Size& size1, const Size& size2)
{
	if (!AreClose(size1.Width, size2.Width))	return false;
	return AreClose(size1.Height, size2.Height);
}

bool FloatingHelper::AreClose(const Rect& rect1, const Rect& rect2)
{
	if (rect1.IsEmpty) return rect2.IsEmpty;
	if (rect2.IsEmpty || !AreClose(rect1.Left, rect2.Left) || !AreClose(rect1.Top, rect2.Top) || !AreClose(rect1.Height, rect2.Height))  return false;
	return AreClose(rect1.Width, rect2.Width);
}

int FloatingHelper::DoubleToInt(double val)
{
	if (0 >= val)return (int)(val - 0.5);
	return (int)(val + 0.5);
}

bool FloatingHelper::GreaterThan(double value1, double value2)
{
	if (value1 <= value2) return false;
	return !AreClose(value1, value2);
}

bool FloatingHelper::GreaterThanOrClose(double value1, double value2)
{
	if (value1 > value2) return true;
	return AreClose(value1, value2);
}

bool FloatingHelper::IsBetweenZeroAndOne(double val)
{
	if (!GreaterThanOrClose(val, 0)) return false;
	return LessThanOrClose(val, 1);
}

bool FloatingHelper::IsNaN(double value)
{
	return isnan(value);
}

bool FloatingHelper::IsInfinity(double value)
{
	return isinf(value);
}

bool FloatingHelper::IsNaNOrInfinity(double value)
{
	if (isnan(value)) return true;
	return isinf(value);
}

bool FloatingHelper::IsPositiveInfinity(double value)
{
	return value == std::numeric_limits<double>::infinity();
}

bool FloatingHelper::IsNegativeInfinity(double value)
{
	return value == -std::numeric_limits<double>::infinity();
}

bool FloatingHelper::IsOne(double value)
{
	return abs(value - 1) < 2.22044604925031E-15;
}

bool FloatingHelper::IsZero(double value)
{
	return abs(value) < 2.22044604925031E-15;
}

bool FloatingHelper::LessThan(double value1, double value2)
{
	if (value1 >= value2) return false;
	return !AreClose(value1, value2);
}

bool FloatingHelper::LessThanOrClose(double value1, double value2)
{
	if (value1 < value2)	return true;
	return AreClose(value1, value2);
}

bool FloatingHelper::RectHasNaN(const Rect& r)
{
	if (!isnan(r.Left) && !isnan(r.Top) && !isnan(r.Height) && !isnan(r.Width)) return false;
	return true;
}