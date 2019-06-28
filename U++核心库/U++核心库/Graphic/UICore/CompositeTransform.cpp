#include "CompositeTransform.h"
#include "PreCoerceCallbackHelper.h"
using namespace System::UI;
using namespace System::UI::Media;


const DependencyPropertyPtr CompositeTransform::CenterXProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::CenterYProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::ScaleXProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::ScaleYProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::SkewXProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::SkewYProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::TranslateXProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::TranslateYProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::RotationProperty = make_dependencyproperty<CompositeTransform>();
const DependencyPropertyPtr CompositeTransform::MatrixProperty = make_dependencyproperty<CompositeTransform>();


__usctor(CompositeTransform)
{
	CompositeTransform::CenterXProperty.get()->Register<CompositeTransform>(L"CenterX",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::CenterYProperty.get()->Register<CompositeTransform>(L"CenterY",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::ScaleXProperty.get()->Register<CompositeTransform>(L"ScaleX",
		FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 1.0)));

	CompositeTransform::ScaleYProperty.get()->Register<CompositeTransform>(L"ScaleY",
		FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 1.0)));

	CompositeTransform::SkewXProperty.get()->Register<CompositeTransform>(L"SkewX",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::SkewYProperty.get()->Register<CompositeTransform>(L"SkewY",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::TranslateXProperty.get()->Register<CompositeTransform>(L"TranslateX",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::TranslateYProperty.get()->Register<CompositeTransform>(L"TranslateY",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::RotationProperty.get()->Register<CompositeTransform>(L"Rotation",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));

	CompositeTransform::MatrixProperty.get()->Register<CompositeTransform>(L"Matrix",
		FrameworkPropertyMetadata::Create<System::Matrix>(System::Matrix::Identity(), FrameworkPropertyMetadataOptions::AffectsUpdate));

	return true;
}

CompositeTransform::CompositeTransform() 
{

}

CompositeTransform::~CompositeTransform()
{
}

__inline System::Matrix Calculate(double scaleX, double scaleY, double centerX, double centerY, double skewX, double skewY, double rotation, double translateX, double translateY)
{
	System::Matrix calculatedMatrix = Matrix::CreateScaling(scaleX, scaleY, centerX, centerY);

	calculatedMatrix = calculatedMatrix * Matrix::CreateSkewRadians(skewX, skewY, centerX, centerY);

	calculatedMatrix = calculatedMatrix * Matrix::CreateRotationRadians(rotation, centerX, centerY);

	calculatedMatrix = calculatedMatrix * Matrix::CreateTranslation(translateX, translateY);

	return calculatedMatrix;
}

bool CompositeTransform::CenterXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, newValue, me->CenterY, me->SkewX, me->SkewY, me->Rotation, me->TranslateX, me->TranslateY));
	return true;
}

bool CompositeTransform::CenterYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, me->CenterX, newValue, me->SkewX, me->SkewY, me->Rotation, me->TranslateX, me->TranslateY));
	return true;
}

bool CompositeTransform::ScaleXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(newValue, me->ScaleY, me->CenterX, me->CenterY, me->SkewX, me->SkewY, me->Rotation, me->TranslateX, me->TranslateY));
	return true;
}

bool CompositeTransform::ScaleYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, newValue, me->CenterX, me->CenterY, me->SkewX, me->SkewY, me->Rotation, me->TranslateX, me->TranslateY));
	return true;
}

bool CompositeTransform::SkewXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, me->CenterX, me->CenterY, newValue, me->SkewY, me->Rotation, me->TranslateX, me->TranslateY));
	return true;
}

bool CompositeTransform::SkewYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, me->CenterX, me->CenterY, me->SkewX, newValue, me->Rotation, me->TranslateX, me->TranslateY));
	return true;
}

bool CompositeTransform::TranslateXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, me->CenterX, me->CenterY, me->SkewX, me->SkewY, me->Rotation, newValue, me->TranslateY));
	return true;
}

bool CompositeTransform::TranslateYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, me->CenterX, me->CenterY, me->SkewX, me->SkewY, me->Rotation, me->TranslateX, newValue));
	return true;
}

bool CompositeTransform::RotationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CompositeTransform*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	me->InternalSetValue(MatrixProperty, Calculate(me->ScaleX, me->ScaleY, me->CenterX, me->CenterY, me->SkewX, me->SkewY, newValue, me->TranslateX, me->TranslateY));
	return true;
}

CompositeTransform::CompositeTransform(const CompositeTransform& transform)
{
	CenterX = transform.CenterX;
	CenterY = transform.CenterY;
	ScaleX = transform.ScaleX;
	ScaleY = transform.ScaleY;
	SkewX = transform.SkewX;
	SkewY = transform.SkewY;
	TranslateX = transform.TranslateX;
	TranslateY = transform.TranslateY;
	Rotation = transform.Rotation;
	InternalSetValue(MatrixProperty, transform.GetValue<System::Matrix>(MatrixProperty));
}

bool CompositeTransform::operator == (const CompositeTransform& transform) const
{
	return GetValue<System::Matrix>(MatrixProperty) == transform.GetValue<System::Matrix>(MatrixProperty);
}

bool CompositeTransform::operator != (const CompositeTransform&  transform) const
{
	return GetValue<System::Matrix>(MatrixProperty) != transform.GetValue<System::Matrix>(MatrixProperty);
}

double CompositeTransform::GetCenterX() const
{
	return GetValue<double>(CenterXProperty);
}

void CompositeTransform::SetCenterX(double val)
{
	SetValue(CenterXProperty, val);
}

double CompositeTransform::GetCenterY() const
{
	return GetValue<double>(CenterYProperty);
}

void CompositeTransform::SetCenterY(double val)
{
	SetValue(CenterYProperty, val);
}

double CompositeTransform::GetScaleX() const
{
	return GetValue<double>(ScaleXProperty);
}

void CompositeTransform::SetScaleX(double val)
{
	SetValue(ScaleXProperty, val);
}

double CompositeTransform::GetScaleY() const
{
	return GetValue<double>(ScaleYProperty);
}

void CompositeTransform::SetScaleY(double val)
{
	SetValue(ScaleYProperty, val);
}

double CompositeTransform::GetSkewX() const
{
	return GetValue<double>(SkewXProperty);
}

void CompositeTransform::SetSkewX(double val)
{
	SetValue(SkewXProperty, val);
}

double CompositeTransform::GetSkewY() const
{
	return GetValue<double>(SkewYProperty);
}

void CompositeTransform::SetSkewY(double val)
{
	SetValue(SkewYProperty, val);
}

double CompositeTransform::GetTranslateX() const
{
	return GetValue<double>(TranslateXProperty);
}

void CompositeTransform::SetTranslateX(double val)
{
	SetValue(TranslateXProperty, val);
}

double CompositeTransform::GetTranslateY() const
{
	return GetValue<double>(TranslateYProperty);
}

void CompositeTransform::SetTranslateY(double val)
{
	SetValue(TranslateYProperty, val);
}

double CompositeTransform::GetRotation() const
{
	return GetValue<double>(RotationProperty);
}

void CompositeTransform::SetRotation(double val)
{
	SetValue(RotationProperty, val);
}

CompositeTransform::operator System::Matrix()
{
	return GetValue<System::Matrix>(MatrixProperty);
}

System::Matrix CompositeTransform::GetMatrix() const
{
	return GetValue<System::Matrix>(MatrixProperty);
}

System::Matrix CompositeTransform::GetTransformMatrix()
{
	return GetValue<System::Matrix>(MatrixProperty);
}
