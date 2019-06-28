#include "QuadraticBezierSegment.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr QuadraticBezierSegment::Point1Property = make_dependencyproperty<QuadraticBezierSegment>();
const DependencyPropertyPtr QuadraticBezierSegment::Point2Property = make_dependencyproperty<QuadraticBezierSegment>();

__usctor(QuadraticBezierSegment)
{
	QuadraticBezierSegment::Point1Property.get()->Register<QuadraticBezierSegment>(L"Point1", FrameworkPropertyMetadata::Create<System::Point>(System::Point(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	QuadraticBezierSegment::Point2Property.get()->Register<QuadraticBezierSegment>(L"Point2", FrameworkPropertyMetadata::Create<System::Point>(System::Point(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}


void QuadraticBezierSegment::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	NotifyChanged(dep->Name.c_str(), L"");
}

bool QuadraticBezierSegment::OnNotifyChanged(const wstring& notifyname)
{
	return ContainsDependencyProperty(notifyname) ? PathSegment::ContainsDependencyProperty(notifyname) : false;
}

QuadraticBezierSegment::QuadraticBezierSegment()
{

}

QuadraticBezierSegment::QuadraticBezierSegment(const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin) :PathSegment(isStroked, isSmoothJoin)
{
	InternalSetValue(Point1Property, point1);
	InternalSetValue(Point2Property, point2);
}

QuadraticBezierSegment::~QuadraticBezierSegment()
{

}

System::Point QuadraticBezierSegment::GetPoint1() const
{
	return GetValue<System::Point>(Point1Property);
}

void QuadraticBezierSegment::SetPoint1(const System::Point& point)
{
	SetValue(Point1Property, point);
}

System::Point QuadraticBezierSegment::GetPoint2() const
{
	return GetValue<System::Point>(Point2Property);
}

void QuadraticBezierSegment::SetPoint2(const System::Point& point)
{
	SetValue(Point2Property, point);
}
