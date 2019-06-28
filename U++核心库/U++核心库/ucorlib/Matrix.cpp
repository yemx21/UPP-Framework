#include "Matrix.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "FloatingHelper.h"
#include "Parser.h"
using namespace System;



static const std::wstring Matrix_Identity = std::wstring(L"Identity");

std::wstring Matrix::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	if (IsIdentity) return Matrix_Identity;
	return Formatter::Format(L"{0:r},{1:r},{2:r},{3:r},{4:r},{5:r}", loc, (VariadicAny(_11), _12, _21, _21, _31, _32));
}


bool Matrix::TryParse(const std::wstring& input, const CultureInfo& loc, Matrix& result)
{
	std::vector<Float64> subs;
	subs.reserve(6);
	Parser::Parse(input, subs, ',');
	if (subs.size() == 6)
	{
		result._11 = subs[0];
		result._12 = subs[1];
		result._21 = subs[2];
		result._22 = subs[3];
		result._31 = subs[4];
		result._32 = subs[5];
		result.flag = 0;
		if (result._21 != 0.0 || result._12 != 0.0)
		{
			result.flag = 4;
		}
		else
		{
			if (result._11 != 1.0 || result._22 != 1.0)
				result.flag = 2;
			if (result._31 != 0.0 || result._32 != 0.0)
				result.flag |= 1;
			if ((result.flag & 3) != 0)
				return true; 
			result.flag = 0;
		}
		return true;
	}
	else if (StringHelper::Compare(input, Matrix_Identity, StringComparison::InvariantCultureIgnoreCase) == 0)
	{
		result._11 = 1.0;
		result._12 = 0.0;
		result._21 = 0.0;
		result._22 = 1.0;
		result._31 = 0.0;
		result._32 = 0.0;
		result.flag = 0;
		return true;
	}
	return false;
}


Matrix::Matrix(double m11, double m12, double m21, double m22, double m31, double m32) : _11(m11), _12(m12), _21(m21), _22(m22), _31(m31), _32(m32)
{
	flag = 0;
	if (_21 != 0.0 || _12 != 0.0)
	{
		flag = 4;
	}
	else
	{
		if (_11 != 1.0 || _22 != 1.0)
			flag = 2;
		if (_31 != 0.0 || _32 != 0.0)
			flag |= 1;
		if ((flag & 3) != 0)
			return;
		flag = 0;
	}
}


Matrix::Matrix() : _11(0.0f), _12(0.0f), _21(0.0f), _22(0.0f), _31(0.0f), _32(0.0f), flag(4)
{

}


Matrix::Matrix(const Matrix& matrix)
{
	_11 = matrix._11;
	_12 = matrix._12;
	_21 = matrix._21;
	_22 = matrix._22;
	_31 = matrix._31;
	_32 = matrix._32;
	flag = matrix.flag;
}

Matrix::~Matrix()
{
}


__inline Matrix Matrix::Identity()
{
	Matrix identity;

	identity._11 = 1.f;
	identity._12 = 0.f;
	identity._21 = 0.f;
	identity._22 = 1.f;
	identity._31 = 0.f;
	identity._32 = 0.f;

	return identity;
}


bool Matrix::GetIsIdentity() const
{
	if (flag == 0) return true;
	if (_11 == 1.f && _12 == 0.f && (_21 == 0.f && _22 == 1.f) && _31 == 0.f)
		return _32 == 0.f;
	else
		return false;
}

double Matrix::GetDeterminant() const
{
	switch (flag)
	{
	case 0:
	case 1:
		return 1.0;
	case 2:
	case 3:
		return _11 * _22;
	default:
		return _11 * _22 - _12 * _21;
	}
}

bool Matrix::GetHasInverse() const
{
	return !FloatingHelper::IsZero(Determinant);
}


double Matrix::GetM11() const
{
	if (flag == 0)
		return 1.0;
	else
		return _11;
}


void Matrix::SetM11(double val)
{
	if (flag == 0)
	{
		flag = 2;
		_11 = val;
		_12 = 0.0;
		_21 = 0.0;
		_22 = 1.0;
		_31 = 0.0;
		_32 = 0.0;
	}
	else
	{
		_11 = val;
		if (flag == 4) return;
		flag |= 2;
	}
}

double Matrix::GetM12() const
{
	if (flag == 0)
		return 0.0;
	else
		return _12;
}

void Matrix::SetM12(double val)
{
	if (flag == 0)
	{
		flag = 4;
		_11 = 1.0;
		_12 = val;
		_21 = 0.0;
		_22 = 1.0;
		_31 = 0.0;
		_32 = 0.0;
	}
	else
	{
		_12 = val;
		flag = 4;
	}
}

double Matrix::GetM21() const
{
	if (flag == 0)
		return 0.0;
	else
		return _21;
}

void Matrix::SetM21(double val)
{
	if (flag == 0)
	{
		flag = 4;
		_11 = 1.0;
		_12 = 0.0;
		_21 = val;
		_22 = 1.0;
		_31 = 0.0;
		_32 = 0.0;
	}
	else
	{
		_21 = val;
		flag = 4;
	}
}

double Matrix::GetM22() const
{
	if (flag == 0)
		return 1.0;
	else
		return _22;
}

void Matrix::SetM22(double val)
{
	if (flag == 0)
	{
		flag = 2;
		_11 = 1.0;
		_12 = 0.0;
		_21 = 0.0;
		_22 = val;
		_31 = 0.0;
		_32 = 0.0;
	}
	else
	{
		_22 = val;
		if (flag == 4) return;
		flag |= 2;
	}
}

double Matrix::GetM31() const
{
	if (flag == 0)
		return 0.0;
	else
		return _31;
}

void Matrix::SetM31(double val)
{
	if (flag == 0)
	{
		flag = 1;
		_11 = 1.0;
		_12 = 0.0;
		_21 = 0.0;
		_22 = 1.0;
		_31 = val;
		_32 = 0.0;
	}
	else
	{
		_31 = val;
		if (flag == 4) return;
		flag |= 1;
	}
}

double Matrix::GetM32() const
{
	if (flag == 0)
		return 0.0;
	else
		return _32;
}

void Matrix::SetM32(double val)
{
	if (flag == 0)
	{
		flag = 1;
		_11 = 1.0;
		_12 = 0.0;
		_21 = 0.0;
		_22 = 1.0;
		_31 = 0.0;
		_32 = val;
	}
	else
	{
		_32 = val;
		if (flag == 4) return;
		flag |= 1;
	}
}

bool Matrix::GetIsDistinguishedIdentity() const
{
	return flag == 0;
}

Matrix Matrix::operator* (const Matrix &matrix) const
{
	Matrix result = Matrix(*this);

	char matrixTypes1 = result.flag;
	char matrixTypes2 = matrix.flag;
	if (matrixTypes2 == 0)
		return result;
	if (matrixTypes1 == 0)
		result = matrix;
	else if (matrixTypes2 == 1)
	{
		result._31 += matrix._31;
		result._32 += matrix._32;
		if (matrixTypes1 == 4)
			return result;
		result.flag |= 1;
	}
	else if (matrixTypes1 == 1)
	{
		double num1 = result._31;
		double num2 = result._32;
		result = matrix;
		result._31 = num1 * matrix._11 + num2 * matrix._21 + matrix._31;
		result._32 = num1 * matrix._12 + num2 * matrix._22 + matrix._32;
		if (matrixTypes2 == 4)
			result.flag = 4;
		else
			result.flag = 1 | 2;
	}
	else
	{
		switch (char((int)matrixTypes1 << 4) | matrixTypes2)
		{
		case 34:
			result._11 *= matrix._11;
			result._22 *= matrix._22;
			break;
		case 35:
			result._11 *= matrix._11;
			result._22 *= matrix._22;
			result._31 = matrix._31;
			result._32 = matrix._32;
			result.flag = 3;
			break;
		case 36:
		case 52:
		case 66:
		case 67:
		case 68:
			result._11 = result._11 * matrix._11 + result._12 * matrix._21;
			result._12 = result._11 * matrix._12 + result._12 * matrix._22;
			result._21 = result._21 * matrix._11 + result._22 * matrix._21;
			result._22 = result._21 * matrix._12 + result._22 * matrix._22;
			result._31 = result._31 * matrix._11 + result._32 * matrix._21 + matrix._31;
			result._32 = result._31 * matrix._12 + result._32 * matrix._22 + matrix._32;
			result.flag = 0;
			if (result._21 != 0.0 || result._12 != 0.0)
			{
				result.flag = 4;
			}
			else
			{
				if (result._11 != 1.0 || result._22 != 1.0)
					result.flag = 2;
				if (result._31 != 0.0 || result._32 != 0.0)
					result.flag |= 1;
				if ((result.flag & 3) != 0)
					break;
				result.flag = 0;
			}
			break;
		case 50:
			result._11 *= matrix._11;
			result._22 *= matrix._22;
			result._31 *= matrix._11;
			result._32 *= matrix._22;
			break;
		case 51:
			result._11 *= matrix._11;
			result._22 *= matrix._22;
			result._31 = matrix._11 * result._31 + matrix._31;
			result._32 = matrix._22 * result._32 + matrix._32;
			break;
		}
	}
	return result;
}

bool Matrix::operator == (const Matrix &matrix) const
{
	if (IsDistinguishedIdentity || matrix.IsDistinguishedIdentity)
		return IsIdentity == matrix.IsIdentity;
	if (M11 == matrix.M11 && M12 == matrix.M12 && (M21 == matrix.M21 && M22 == matrix.M22) && M31 == matrix.M31)
		return M32 == matrix.M32;
	else
		return false;
	return matrix._11 == _11 && matrix._12 == _12 && matrix._21 == _21 && matrix._22 == _22 && matrix._31 == _31 && matrix._32 == _32;
}

bool Matrix::operator != (const Matrix &matrix) const
{
	return !(operator==(matrix));
}

void Matrix::SetIdentity()
{
	flag = 0;
}


Matrix Matrix::CreateRotationRadians(double angle)
{
	return CreateRotationRadians(angle, 0.0, 0.0);
}

Matrix Matrix::CreateRotationRadians(double angle, double centerX, double centerY)
{
	Matrix matrix = Matrix();
	double m12 = sin(angle);
	double num = cos(angle);
	double offsetX = centerX * (1.0 - num) + centerY * m12;
	double offsetY = centerY * (1.0 - num) - centerX * m12;
	matrix._11 = num;
	matrix._12 = m12;
	matrix._21 = -m12;
	matrix._22 = num;
	matrix._31 = offsetX;
	matrix._32 = offsetY;
	matrix.flag = 4;
	return matrix;
}



Matrix Matrix::CreateScaling(double scaleX, double scaleY, double centerX, double centerY)
{
	Matrix matrix = Matrix();
	matrix._11 = scaleX;
	matrix._12 = 0.0;
	matrix._21 = 0.0;
	matrix._22 = scaleY;
	matrix._31 = centerX - scaleX * centerX;
	matrix._32 = centerY - scaleY * centerY;
	matrix.flag = 3;
	return matrix;
}

Matrix Matrix::CreateScaling(double scaleX, double scaleY)
{
	Matrix matrix = Matrix();
	matrix._11 = scaleX;
	matrix._12 = 0.0;
	matrix._21 = 0.0;
	matrix._22 = scaleY;
	matrix._31 = 0.0;
	matrix._32 = 0.0;
	matrix.flag = 2;
	return matrix;
}


Matrix Matrix::CreateSkewRadians(double skewX, double skewY)
{
	Matrix matrix = Matrix();
	matrix._11 = 1.0;
	matrix._12 = tan(skewY);
	matrix._21 = tan(skewX);
	matrix._22 = 1.0;
	matrix._31 = 0.0;
	matrix._32 = 0.0;
	matrix.flag = 4;
	return matrix;
}

Matrix Matrix::CreateSkewRadians(double skewX, double skewY, double centerX, double centerY)
{

	Matrix matrix = Matrix();
	matrix._11 = 1.0;
	matrix._12 = tan(skewY);
	matrix._21 = tan(skewX);
	matrix._22 = 1.0;
	matrix._31 = - centerY * matrix._21;
	matrix._32 = -centerX * matrix._12;
	matrix.flag = 4;
	return matrix;
}

Matrix Matrix::CreateTranslation(double offsetX, double offsetY)
{
	Matrix matrix = Matrix();
	matrix._11 = 1.0;
	matrix._12 = 0.0;
	matrix._21 = 0.0;
	matrix._22 = 1.0;
	matrix._31 = offsetX;
	matrix._32 = offsetY;
	matrix.flag = 1;
	return matrix;
}


void Matrix::Multiply(const Matrix& matrix)
{
	char matrixTypes2 = matrix.flag;
	if (matrixTypes2 == 0)
		return;
	if (flag == 0)
	{
		_11 = matrix._11;
		_12 = matrix._12;
		_21 = matrix._21;
		_22 = matrix._22;
		_31 = matrix._31;
		_32 = matrix._32;
		flag = matrix.flag;
	}
	else if (matrixTypes2 == 1)
	{
		_31 += matrix._31;
		_32 += matrix._32;
		if (flag == 4)
			return;
		flag |= 1;
	}
	else if (flag == 1)
	{
		double num1 = _31;
		double num2 = _32;
		_11 = matrix._11;
		_12 = matrix._12;
		_21 = matrix._21;
		_22 = matrix._22;
		_31 = matrix._31;
		_32 = matrix._32;
		flag = matrix.flag;
		_31 = num1 * matrix._11 + num2 * matrix._21 + matrix._31;
		_32 = num1 * matrix._12 + num2 * matrix._22 + matrix._32;
		if (matrixTypes2 == 4)
			flag = 4;
		else
			flag = 1 | 2;
	}
	else
	{
		switch (char((int)flag << 4) | matrixTypes2)
		{
		case 34:
			_11 *= matrix._11;
			_22 *= matrix._22;
			break;
		case 35:
			_11 *= matrix._11;
			_22 *= matrix._22;
			_31 = matrix._31;
			_32 = matrix._32;
			flag = 3;
			break;
		case 36:
		case 52:
		case 66:
		case 67:
		case 68:
			_11 = _11 * matrix._11 + _12 * matrix._21;
			_12 = _11 * matrix._12 + _12 * matrix._22;
			_21 = _21 * matrix._11 + _22 * matrix._21;
			_22 = _21 * matrix._12 + _22 * matrix._22;
			_31 = _31 * matrix._11 + _32 * matrix._21 + matrix._31;
			_32 = _31 * matrix._12 + _32 * matrix._22 + matrix._32;
			flag = 0;
			if (_21 != 0.0 || _12 != 0.0)
			{
				flag = 4;
			}
			else
			{
				if (_11 != 1.0 || _22 != 1.0)
					flag = 2;
				if (_31 != 0.0 || _32 != 0.0)
					flag |= 1;
				if ((flag & 3) != 0)
					break;
				flag = 0;
			}
			break;
		case 50:
			_11 *= matrix._11;
			_22 *= matrix._22;
			_31 *= matrix._11;
			_32 *= matrix._22;
			break;
		case 51:
			_11 *= matrix._11;
			_22 *= matrix._22;
			_31 = matrix._11 * _31 + matrix._31;
			_32 = matrix._22 * _32 + matrix._32;
			break;
		}
	}
}


void Matrix::Rotate(double angle)
{
	angle = fmod(angle, 360.0);
	Multiply(CreateRotationRadians(angle * (M_PI / 180.0)));
}


void Matrix::RotateAt(double angle, double centerX, double centerY)
{
	angle = fmod(angle, 360.0);
	Multiply(CreateRotationRadians(angle * (M_PI / 180.0), centerX, centerY));
}


void Matrix::Scale(double scaleX, double scaleY)
{
	Multiply(CreateScaling(scaleX, scaleY));
}


void Matrix::ScaleAt(double scaleX, double scaleY, double centerX, double centerY)
{
	Multiply(CreateScaling(scaleX, scaleY, centerX, centerY));
}


void Matrix::Skew(double skewX, double skewY)
{
	skewX = fmod(skewX, 360.0);
	skewY = fmod(skewY, 360.0);
	Multiply(CreateSkewRadians(skewX * (M_PI / 180.0), skewY * (M_PI / 180.0)));
}


void Matrix::Translate(double offsetX, double offsetY)
{
	if (flag == 0)
	{
		_11 = 1.0;
		_12 = 0.0;
		_21 = 0.0;
		_22 = 1.0;
		_31 = offsetX;
		_32 = offsetY;
		flag = 1;
	}
	else if (flag == 4)
	{
		_31 += offsetX;
		_32 += offsetY;
	}
	else
	{
		_31 += offsetX;
		_32 += offsetY;
		flag |= 1;
	}
}


void Matrix::MultiplyPoint(double& x, double& y) const
{
	switch (flag)
	{
		case 0:
		{
			return;
		}
		case 1:
		{
			x = x + _31;
			y = y + _32;
			return;
		}
		case 2:
		{
			x = x * _11;
			y = y * _22;
			return;
		}
		case 3:
		{
			x = x * _11;
			x = x + _31;
			y = y * _22;
			y = y +_32;
			return;
		}
	}
	double num1 = y * _21 + _31;
	double num2 = x * _12 + _32;
	x *= _11;
	x += num1;
	y *= _22;
	y += num2;
}


Point Matrix::Transform(const Point& point) const
{
	double x = point.X, y = point.Y;
	MultiplyPoint(x, y);
	return Point(x, y);
}


void Matrix::Transform(Point* points, size_t pointCount) const
{
	if (points == nullptr) return;
	for (size_t index = 0; index < pointCount; ++index)
	{
		MultiplyPoint(points[index].XRef, points[index].YRef);
	}
}


void Matrix::Transform(std::vector<Point>& points) const
{
	if (points.empty()) return;
	for (size_t index = 0; index < points.size(); ++index)
		MultiplyPoint(points[index].XRef, points[index].YRef);
}


void Matrix::Invert()
{
	double determinant = Determinant;
	if (FloatingHelper::IsZero(determinant))
		throw std::exception("transform not invertible");
	switch (flag)
	{
	case 0:
		break;
	case 1:
		_31 = -_31;
		_32 = -_32;
		break;
	case 2:
		_11 = 1.0 / _11;
		_22 = 1.0 / _22;
		break;
	case 3:
		_11 = 1.0 / _11;
		_22 = 1.0 / _22;
		_31 = -_31 * _11;
		_32 = -_32 * _22;
		break;
	default:
		double num = 1.0 / determinant;
		_11 = _22 * num;
		_12 = -_12 * num;
		_21 = -_21 * num;
		_22 = _11 * num;
		_31 = (_21 * _32 - _31 * _22) * num;
		_32 = (_31 * _12 - _11 * _32) * num;
		flag = 4;
		break;
	}
}

std::weak_ptr<MatrixConverter> MatrixConverter::_thisWeakPtr;
std::mutex& MatrixConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

MatrixConverter::MatrixConverter() {}
MatrixConverter::~MatrixConverter() {}

std::shared_ptr<MatrixConverter> MatrixConverter::GetSharedInstance()
{
	std::shared_ptr<MatrixConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new MatrixConverter());
	_thisWeakPtr = p;
	return p;
}

MatrixConverter& MatrixConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool MatrixConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Matrix))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool MatrixConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Matrix))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any MatrixConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Matrix)))
		{
			if (type->IsReference)
			{
				return Matrix(AnyVisitor<Matrix&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Matrix* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Matrix res = obj.as<Matrix>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Matrix result = Matrix();
				if (Matrix::TryParse(std::wstring(tmp), loc, result))
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

Any MatrixConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
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
					return AnyVisitor<Matrix&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Matrix* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Matrix v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Matrix)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Matrix&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Matrix* p = obj;
			if (p)
				return *p;
			else
			{
				Matrix v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any MatrixConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Matrix result = Matrix();
	if (Matrix::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring MatrixConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Matrix)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Matrix&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Matrix* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Matrix v = obj;
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

__inline static Object* Factory_Matrix_0() { return new Matrix(); }
__inline static Object* Factory_Matrix_1(double m11, double m12, double m21, double m22, double m31, double m32) { return new Matrix(m11, m12, m21, m22, m31, m32); }

__uregister_class(Matrix, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Matrix>(L"Matrix", ConstructorFlags::Default, &Factory_Matrix_0);
	type.AddConstructor(L"Matrix", ConstructorFlags::Default, ConstructorTypeAdapter<6, Matrix>::Create(&Factory_Matrix_1));

	type.AddMethod(L"SetIdentity", MethodFlags::Public, MethodTypeAdapter<0, Matrix>::Create(&Matrix::SetIdentity));
	type.AddMethod(L"CreateRotationRadians", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<3, Matrix, Matrix>::Create(&Matrix::CreateRotationRadians));
	type.AddMethod(L"CreateRotationRadians", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, Matrix, Matrix>::Create(&Matrix::CreateRotationRadians));
	type.AddMethod(L"CreateScaling", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<4, Matrix, Matrix>::Create(&Matrix::CreateScaling));
	type.AddMethod(L"CreateScaling", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Matrix, Matrix>::Create(&Matrix::CreateScaling));
	type.AddMethod(L"CreateSkewRadians", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<4, Matrix, Matrix>::Create(&Matrix::CreateSkewRadians));
	type.AddMethod(L"CreateSkewRadians", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Matrix, Matrix>::Create(&Matrix::CreateSkewRadians));

	type.AddMethod(L"CreateTranslation", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, Matrix, Matrix>::Create(&Matrix::CreateTranslation));
	type.AddMethod(L"Multiply", MethodFlags::Public, MethodTypeAdapter<1,Matrix>::Create(&Matrix::Multiply));
	type.AddMethod(L"Invert", MethodFlags::Public, MethodTypeAdapter<0, Matrix>::Create(&Matrix::Invert));
	type.AddMethod(L"Rotate", MethodFlags::Public, MethodTypeAdapter<1, Matrix>::Create(&Matrix::Rotate));
	type.AddMethod(L"RotateAt", MethodFlags::Public, MethodTypeAdapter<3, Matrix>::Create(&Matrix::RotateAt));
	type.AddMethod(L"Scale", MethodFlags::Public, MethodTypeAdapter<2, Matrix>::Create(&Matrix::Scale));
	type.AddMethod(L"ScaleAt", MethodFlags::Public, MethodTypeAdapter<4, Matrix>::Create(&Matrix::ScaleAt));
	type.AddMethod(L"Skew", MethodFlags::Public, MethodTypeAdapter<2, Matrix>::Create(&Matrix::Skew));
	type.AddMethod(L"Translate", MethodFlags::Public, MethodTypeAdapter<2, Matrix>::Create(&Matrix::Translate));
	type.AddMethod(L"MultiplyPoint", MethodFlags::Public, MethodTypeAdapter<2, Matrix>::Create(&Matrix::MultiplyPoint));
	type.AddMethod(L"Transform", MethodFlags::Public, MethodTypeAdapter<1, Matrix, Point>::Create(&Matrix::Transform));
	type.AddMethod(L"Transform", MethodFlags::Public, MethodTypeAdapter<2, Matrix>::Create(&Matrix::Transform));
	type.AddMethod(L"Transform", MethodFlags::Public, MethodTypeAdapter<1, Matrix>::Create(&Matrix::Transform));

	type.AddProperty(L"M11", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetM11, &Matrix::SetM11));
	type.AddProperty(L"M12", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetM12, &Matrix::SetM12));
	type.AddProperty(L"M21", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetM21, &Matrix::SetM21));
	type.AddProperty(L"M22", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetM22, &Matrix::SetM22));
	type.AddProperty(L"M31", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetM31, &Matrix::SetM31));
	type.AddProperty(L"M32", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetM32, &Matrix::SetM32));
	type.AddProperty(L"Determinant", PropertyFlags::Public, PropertyTypeOf(&Matrix::GetDeterminant));

	type.AddProperty(L"Identity", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<Matrix, Matrix>(&Matrix::Identity));
	type.AddProperty(L"HasInverse", PropertyFlags::Public, PropertyTypeOf<Matrix>(&Matrix::GetHasInverse));
	type.AddProperty(L"IsDistinguishedIdentity", PropertyFlags::Public, PropertyTypeOf<Matrix>(&Matrix::GetIsDistinguishedIdentity));
	type.AddProperty(L"IsIdentity", PropertyFlags::Public, PropertyTypeOf<Matrix>(&Matrix::GetIsIdentity));


	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Matrix, Matrix&>::Create(&Matrix::operator=));
	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Matrix, bool>::Create(&Matrix::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Matrix, bool>::Create(&Matrix::operator!=));
	type.AddOperator(L"Multiply", OperatorTypeAdapter<1, Operators::Multiplication, Matrix, Matrix>::Create(&Matrix::operator*));


	type.AddAttribute(new TypeConverterAttribute(L"MatrixConverter", MatrixConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};