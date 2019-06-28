#include "ArcSegment.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr ArcSegment::PointProperty = make_dependencyproperty<ArcSegment>();
const DependencyPropertyPtr ArcSegment::SizeProperty = make_dependencyproperty<ArcSegment>();
const DependencyPropertyPtr ArcSegment::RotationAngleProperty = make_dependencyproperty<ArcSegment>();
const DependencyPropertyPtr ArcSegment::ArcSizeProperty = make_dependencyproperty<ArcSegment>();
const DependencyPropertyPtr ArcSegment::SweepDirectionProperty = make_dependencyproperty<ArcSegment>();

__usctor(ArcSegment)
{
	ArcSegment::PointProperty.get()->Register<ArcSegment>(L"Point", FrameworkPropertyMetadata::Create<System::Point>(System::Point(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	ArcSegment::SizeProperty.get()->Register<ArcSegment>(L"Size", FrameworkPropertyMetadata::Create<System::Size>(System::Size(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	ArcSegment::RotationAngleProperty.get()->Register<ArcSegment>(L"RotationAngle", FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsUpdate));
	ArcSegment::ArcSizeProperty.get()->Register<ArcSegment>(L"ArcSize", FrameworkPropertyMetadata::Create<Graphic::ArcSize>((Graphic::ArcSize)Graphic::ArcSize::Small, FrameworkPropertyMetadataOptions::AffectsUpdate));
	ArcSegment::SweepDirectionProperty.get()->Register<ArcSegment>(L"SweepDirection", FrameworkPropertyMetadata::Create<Graphic::SweepDirection>((Graphic::SweepDirection)Graphic::SweepDirection::CounterClockwise, FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}


void ArcSegment::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	NotifyChanged(dep->Name.c_str(), L"");
}

bool ArcSegment::OnNotifyChanged(const wstring& notifyname)
{
	return ContainsDependencyProperty(notifyname) ? PathSegment::ContainsDependencyProperty(notifyname) : false;
}

ArcSegment::ArcSegment()
{

}

ArcSegment::ArcSegment(const System::Point& point, const System::Size& size, double rotationAngle, Graphic::ArcSize arcSize, Graphic::SweepDirection sweepDirection, bool isStroked, bool isSmoothJoin) : PathSegment(isStroked, isSmoothJoin)
{
	InternalSetValue(PointProperty, point);
	InternalSetValue(SizeProperty, size);
	InternalSetValue(RotationAngleProperty, rotationAngle);
	InternalSetValue(ArcSizeProperty, arcSize);
	InternalSetValue(SweepDirectionProperty, sweepDirection);
}

ArcSegment::~ArcSegment()
{

}

System::Point ArcSegment::GetPoint() const
{
	return GetValue<System::Point>(PointProperty);
}

void ArcSegment::SetPoint(const System::Point& point)
{
	SetValue(PointProperty, point);
}

System::Size ArcSegment::GetSize() const
{
	return GetValue<System::Size>(SizeProperty);
}

void ArcSegment::SetSize(const System::Size& size)
{
	SetValue(SizeProperty, size);
}

double ArcSegment::GetRotationAngle() const
{
	return GetValue<double>(RotationAngleProperty);
}

void ArcSegment::SetRotationAngle(double angle)
{
	SetValue(RotationAngleProperty, angle);
}

System::UI::Graphic::ArcSize ArcSegment::GetArcSize() const
{
	return GetValue<Graphic::ArcSize>(ArcSizeProperty);
}

void ArcSegment::SetArcSize(Graphic::ArcSize arc)
{
	SetValue(ArcSizeProperty, arc);
}

System::UI::Graphic::SweepDirection ArcSegment::GetSweepDirection() const
{
	return GetValue<Graphic::SweepDirection>(SweepDirectionProperty);
}

void ArcSegment::SetSweepDirection(Graphic::SweepDirection sweep)
{
	SetValue(SweepDirectionProperty, sweep);
}