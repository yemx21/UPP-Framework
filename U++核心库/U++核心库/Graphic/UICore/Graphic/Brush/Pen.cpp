#include "Pen.h"
#include "..\GraphicFactory.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr Pen::ThicknessProperty = make_dependencyproperty<Pen>();
const DependencyPropertyPtr Pen::StrokeStyleProperty = make_dependencyproperty<Pen>();
const DependencyPropertyPtr Pen::BrushProperty = make_dependencyproperty<Pen>();

__usctor(Pen)
{
	Pen::ThicknessProperty.get()->Register<Pen>(L"Thickness",
		FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsRender, __upropertycoerce(__ucoerce_replace_replace_replace, double, 1.0, 0.0, 0.0, 1.0, 1.0)));

	Pen::StrokeStyleProperty.get()->Register<Pen>(L"StrokeStyle",
		FrameworkPropertyMetadata::Create<StrokeStylePtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));

	Pen::BrushProperty.get()->Register<Pen>(L"Brush",
		FrameworkPropertyMetadata::Create<BrushPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));
	return true;
}


Pen::Pen() 
{
}

Pen::~Pen()
{
}

FeatureObjectPtr Pen::OnCreateResource(RenderContext* renderContext)
{
	return nullptr;
}

void Pen::SetThickness(double thickness)
{
	SetValue(ThicknessProperty, thickness);
}

double Pen::GetThickness() const
{
	return GetValue<double>(ThicknessProperty);
}

void Pen::SetBrush(const BrushPtr& brush)
{
	SetValue(BrushProperty, brush);
}

BrushPtr Pen::GetBrush() const
{
	return GetValuePtr<Graphic::Brush>(BrushProperty);
}

StrokeStylePtr Pen::GetStrokeStyle() const
{
	return GetValuePtr<Graphic::StrokeStyle>(StrokeStyleProperty);
}

void Pen::SetStrokeStyle(const StrokeStylePtr& strokeStyle)
{
	SetValue(StrokeStyleProperty, strokeStyle);
}
