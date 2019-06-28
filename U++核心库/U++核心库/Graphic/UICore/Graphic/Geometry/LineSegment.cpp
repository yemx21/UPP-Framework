#include "LineSegment.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr LineSegment::PointProperty = make_dependencyproperty<LineSegment>();

__usctor(LineSegment)
{
	LineSegment::PointProperty.get()->Register<LineSegment>(L"Point", FrameworkPropertyMetadata::Create<System::Point>(System::Point{ 0.0,0.0 }, FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}

void LineSegment::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	NotifyChanged(dep->Name.c_str(), L"");
}

bool LineSegment::OnNotifyChanged(const wstring& notifyname)
{
	return ContainsDependencyProperty(notifyname)? PathSegment::ContainsDependencyProperty(notifyname) : false;
}

LineSegment::LineSegment() :PathSegment()
{

}

LineSegment::LineSegment(const System::Point& point, bool isStroked, bool isSmoothJoin) : PathSegment(isStroked, isSmoothJoin)
{
	InternalSetValue(PointProperty, point);
}

LineSegment::~LineSegment()
{

}

System::Point LineSegment::GetPoint() const
{
	return GetValue<System::Point>(PointProperty);
}

void LineSegment::SetPoint(const System::Point& point)
{
	SetValue(PointProperty, point);
}