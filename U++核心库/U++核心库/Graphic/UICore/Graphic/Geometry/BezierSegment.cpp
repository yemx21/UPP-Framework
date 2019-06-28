#include "BezierSegment.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr BezierSegment::Point1Property = make_dependencyproperty<BezierSegment>();
const DependencyPropertyPtr BezierSegment::Point2Property = make_dependencyproperty<BezierSegment>();
const DependencyPropertyPtr BezierSegment::Point3Property = make_dependencyproperty<BezierSegment>();

__usctor(BezierSegment)
{
	BezierSegment::Point1Property.get()->Register<BezierSegment>(L"Point1", FrameworkPropertyMetadata::Create<System::Point>(System::Point(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	BezierSegment::Point2Property.get()->Register<BezierSegment>(L"Point2", FrameworkPropertyMetadata::Create<System::Point>(System::Point(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	BezierSegment::Point3Property.get()->Register<BezierSegment>(L"Point3", FrameworkPropertyMetadata::Create<System::Point>(System::Point(), FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}


void BezierSegment::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	NotifyChanged(dep->Name.c_str(), L"");
}

bool BezierSegment::OnNotifyChanged(const wstring& notifyname)
{
	return ContainsDependencyProperty(notifyname) ? PathSegment::ContainsDependencyProperty(notifyname) : false;
}

BezierSegment::BezierSegment()
{

}

BezierSegment::BezierSegment(const Point& point1, const Point& point2, const Point& point3, bool isStroked, bool isSmoothJoin) : PathSegment(isStroked, isSmoothJoin)
{
	InternalSetValue(Point1Property, point1);
	InternalSetValue(Point2Property, point2);
	InternalSetValue(Point3Property, point3);
}

BezierSegment::~BezierSegment()
{

}

System::Point BezierSegment::GetPoint1() const
{
	return GetValue<System::Point>(Point1Property);
}

void BezierSegment::SetPoint1(const System::Point& point)
{
	SetValue(Point1Property, point);
}

System::Point BezierSegment::GetPoint2() const
{
	return GetValue<System::Point>(Point2Property);
}

void BezierSegment::SetPoint2(const System::Point& point)
{
	SetValue(Point2Property, point);
}

System::Point BezierSegment::GetPoint3() const
{
	return GetValue<System::Point>(Point3Property);
}

void BezierSegment::SetPoint3(const System::Point& point)
{
	SetValue(Point3Property, point);
}
