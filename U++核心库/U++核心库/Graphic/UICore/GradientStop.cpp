#include "GradientStop.h"

using namespace System::UI;
using namespace System::UI::Media;

const DependencyPropertyPtr GradientStop::ColorProperty = make_dependencyproperty<GradientStop>();
const DependencyPropertyPtr GradientStop::OffsetProperty = make_dependencyproperty<GradientStop>();

__usctor(GradientStop)
{
	GradientStop::ColorProperty.get()->Register<GradientStop>(L"GradientStop",
		FrameworkPropertyMetadata::Create<System::Color>(System::Color(), FrameworkPropertyMetadataOptions::AffectsUpdate));

	GradientStop::OffsetProperty.get()->Register<GradientStop>(L"Offset",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate));

	return true;
}

wstring GradientStop::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return Formatter::Format(L"{0},{1:r}", loc, (VariadicAny(this->Color), this->Offset));
}

GradientStop::GradientStop()
{

}

GradientStop::GradientStop(double offset, const System::Color& color)
{
	InternalSetValue(ColorProperty, color);
	InternalSetValue(OffsetProperty, offset);
}

GradientStop::GradientStop(const GradientStop& stop)
{
	InternalSetValue(ColorProperty, stop.Color);
	InternalSetValue(OffsetProperty, stop.Offset);
}

bool GradientStop::operator == (const GradientStop& stop) const
{
	return stop.Color == Color && stop.Offset == Offset;
}

bool GradientStop::operator != (const GradientStop& stop) const
{
	return stop.Color != Color || stop.Offset != Offset;
}

System::Color GradientStop::GetColor() const 
{
	return GetValue<System::Color>(ColorProperty);
}

void GradientStop::SetColor(const System::Color& color)
{
	SetValue(ColorProperty, color);
}

double GradientStop::GetOffset() const
{
	return GetValue<double>(OffsetProperty);
}

void GradientStop::SetOffset(double offset)
{
	SetValue(OffsetProperty, offset);
}

