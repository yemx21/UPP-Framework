#include "PathSegment.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr PathSegment::IsStrokedProperty = make_dependencyproperty<PathSegment>();
const DependencyPropertyPtr PathSegment::IsSmoothJoinProperty = make_dependencyproperty<PathSegment>();

__usctor(PathSegment)
{
	PathSegment::IsStrokedProperty.get()->Register<PathSegment>(L"IsStroked", FrameworkPropertyMetadata::Create<bool>(true, FrameworkPropertyMetadataOptions::AffectsUpdate));
	PathSegment::IsSmoothJoinProperty.get()->Register<PathSegment>(L"IsSmoothJoin", FrameworkPropertyMetadata::Create<bool>(false, FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}

void PathSegment::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	NotifyChanged(dep->Name.c_str(), L"");
}

bool PathSegment::OnNotifyChanged(const wstring& notifyname)
{
	return ContainsDependencyProperty(notifyname);
}

PathSegment::PathSegment()
{

}

PathSegment::PathSegment(bool isStroked, bool isSmoothJoin)
{
	InternalSetValue(IsStrokedProperty, isStroked);
	InternalSetValue(IsSmoothJoinProperty, isSmoothJoin);
}

PathSegment::~PathSegment()
{

}

bool PathSegment::GetIsStroked() const
{
	return GetValue<bool>(IsStrokedProperty);
}

void PathSegment::SetIsStroked(bool val)
{
	SetValue(IsStrokedProperty, val);
}

bool PathSegment::GetIsSmoothJoin() const
{
	return GetValue<bool>(IsSmoothJoinProperty);
}

void PathSegment::SetIsSmoothJoin(bool val)
{
	SetValue(IsSmoothJoinProperty, val);
}
