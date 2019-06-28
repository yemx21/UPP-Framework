#include "MatrixTransform.h"

using namespace System::UI;
using namespace System::UI::Media;
 
const DependencyPropertyPtr MatrixTransform::MatrixProperty = make_dependencyproperty<MatrixTransform>();

__usctor(MatrixTransform)
{
	MatrixTransform::MatrixProperty.get()->Register<MatrixTransform>(L"Matrix",
		FrameworkPropertyMetadata::Create<System::Matrix>(System::Matrix::Identity(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}

wstring MatrixTransform::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return Matrix.ToString(fmt, loc); 
}

MatrixTransform::MatrixTransform()
{
}

MatrixTransform::MatrixTransform(const System::Matrix& matrix)
{
	InternalSetValue(MatrixProperty, matrix);
}

MatrixTransform::MatrixTransform(const MatrixTransform& transform)
{
	InternalSetValue(MatrixProperty, transform.Matrix);
}

MatrixTransform::~MatrixTransform()
{
}

bool MatrixTransform::operator == (const MatrixTransform& transform) const
{
	return  transform.Matrix == Matrix;
}

bool MatrixTransform::operator != (const MatrixTransform& transform) const
{
	return transform.Matrix != Matrix;
}

System::Matrix MatrixTransform::GetMatrix() const
{
	return GetValue<System::Matrix>(MatrixProperty);
}

void MatrixTransform::SetMatrix(const System::Matrix& matrix)
{
	SetValue(MatrixProperty, matrix);
}

System::Matrix MatrixTransform::GetTransformMatrix()
{
    return Matrix;
}

MatrixTransform::operator System::Matrix()
{
    return Matrix;
}

double MatrixTransform::GetM11() const
{
    return Matrix.M11;
}

void MatrixTransform::SetM11(double val)
{
	System::Matrix tmp = Matrix;
	tmp.SetM11(val);
	Matrix = tmp;
}

double MatrixTransform::GetM12() const
{
    return Matrix.M12;
}

void MatrixTransform::SetM12(double val)
{
	System::Matrix tmp = Matrix;
	tmp.SetM12(val);
	Matrix = tmp;
}

double MatrixTransform::GetM21() const
{
    return Matrix.M21;
}

void MatrixTransform::SetM21(double val)
{
	System::Matrix tmp = Matrix;
	tmp.SetM21(val);
	Matrix = tmp;
}

double MatrixTransform::GetM22() const
{
    return Matrix.M22;
}

void MatrixTransform::SetM22(double val)
{
	System::Matrix tmp = Matrix;
	tmp.SetM22(val);
	Matrix = tmp;
}

double MatrixTransform::GetOffsetX() const
{
    return Matrix.M31;
}

void MatrixTransform::SetOffsetX(double val)
{
	System::Matrix tmp = Matrix;
	tmp.SetM31(val);
	Matrix = tmp;
}

double MatrixTransform::GetOffsetY() const
{
    return Matrix.M32;
}

void MatrixTransform::SetOffsetY(double val)
{
	System::Matrix tmp = Matrix;
	tmp.SetM32(val);
	Matrix = tmp;
}
