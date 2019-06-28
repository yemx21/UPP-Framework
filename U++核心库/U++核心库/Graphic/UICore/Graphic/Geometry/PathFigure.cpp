#include "PathFigure.h"
using namespace System::UI;
using namespace System;
using namespace System::UI::Graphic;

const DependencyPropertyPtr PathFigure::StartPointProperty = make_dependencyproperty<PathFigure>();
const DependencyPropertyPtr PathFigure::IsFilledProperty = make_dependencyproperty<PathFigure>();
const DependencyPropertyPtr PathFigure::IsClosedProperty = make_dependencyproperty<PathFigure>();
const DependencyPropertyPtr PathFigure::SegmentsProperty = make_dependencyproperty<PathFigure>();

__usctor(PathFigure)
{
	PathFigure::StartPointProperty.get()->Register<PathFigure>(L"StartPoint", FrameworkPropertyMetadata::Create<System::Point>(System::Point{ 0.0,0.0 }, FrameworkPropertyMetadataOptions::AffectsUpdate));
	PathFigure::IsFilledProperty.get()->Register<PathFigure>(L"IsFilled", FrameworkPropertyMetadata::Create<bool>(true, FrameworkPropertyMetadataOptions::AffectsUpdate));
	PathFigure::IsClosedProperty.get()->Register<PathFigure>(L"IsClosed", FrameworkPropertyMetadata::Create<bool>(false, FrameworkPropertyMetadataOptions::AffectsUpdate));
	PathFigure::SegmentsProperty.get()->Register<PathFigure>(L"Segments", FrameworkPropertyMetadata::Create<ObservableCollection<PathSegmentPtr>>(nullptr, FrameworkPropertyMetadataOptions::AffectsUpdate, &SegmentsChanged));
	return true;
}

void PathFigure::OnPathSegmentsChanged(PathFigure* sender, std::shared_ptr<ObservableCollection<PathSegmentPtr>> PathSegments, NotifyCollectionChangedEventArgs<PathSegmentPtr> args)
{
	auto me = static_cast<PathFigure*>(sender);
	if (me)
	{
		me->NotifyChanged(L"Segments", L"");
	}
}

bool PathFigure::SegmentsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<PathFigure*>(ptr);
	ObservableCollection<PathSegmentPtr>& oldOC = changedArgs.OldValueRef<ObservableCollection<PathSegmentPtr>>();
	ObservableCollection<PathSegmentPtr>& newOC = changedArgs.NewValueRef<ObservableCollection<PathSegmentPtr>>();
	if (me)
	{
		if (&oldOC != nullptr) oldOC.CollectionChanged -= me->PathSegmentsChangedToken;	
		if (&newOC != nullptr) me->PathSegmentsChangedToken = newOC.CollectionChanged += (bind(&PathFigure::OnPathSegmentsChanged, me, std::placeholders::_1, std::placeholders::_2));
		return true;
	}	
	return false;
}

void PathFigure::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	NotifyChanged(dep->Name.c_str(), L"");
}

bool PathFigure::OnNotifyChanged(const wstring& notifyname)
{
	return ContainsDependencyProperty(notifyname);
}

PathFigure::PathFigure()
{
	SetValue(SegmentsProperty, ObservableCollection<PathSegmentPtr>());
}

PathFigure::PathFigure(const Point& startPoint, bool isFilled, bool isClosed)
{
	InternalSetValue(StartPointProperty, startPoint);
	InternalSetValue(IsFilledProperty, isFilled);
	InternalSetValue(IsClosedProperty, isClosed);
	SetValue(SegmentsProperty, ObservableCollection<PathSegmentPtr>());
}

PathFigure::PathFigure(const Point& startPoint, bool isFilled, bool isClosed, const ObservableCollection<PathSegmentPtr>& segments)
{
	InternalSetValue(StartPointProperty, startPoint);
	InternalSetValue(IsFilledProperty, isFilled);
	InternalSetValue(IsClosedProperty, isClosed); 
	SetValue(SegmentsProperty, segments);
}

PathFigure::~PathFigure()
{
	if (SegmentsProperty)
	{
		ObservableCollection<PathSegmentPtr>&  oldOC = GetValueRef<ObservableCollection<PathSegmentPtr>>(SegmentsProperty);
		if (&oldOC != nullptr) oldOC.CollectionChanged -= PathSegmentsChangedToken;
	}
}

System::Point PathFigure::GetStartPoint() const
{
	return GetValue<System::Point>(StartPointProperty);
}

void PathFigure::SetStartPoint(const System::Point& point)
{
	SetValue(StartPointProperty, point);
}

bool PathFigure::GetIsFilled() const
{
	return GetValue<bool>(IsFilledProperty);
}

void PathFigure::SetIsFilled(bool arc)
{
	SetValue(IsFilledProperty, arc);
}

bool PathFigure::GetIsClosed() const
{
	return GetValue<bool>(IsClosedProperty);
}

void PathFigure::SetIsClosed(bool arc)
{
	SetValue(IsClosedProperty, arc);
}

ObservableCollection<PathSegmentPtr>& PathFigure::GetSegments() const
{
	return GetValueRef<ObservableCollection<PathSegmentPtr>>(SegmentsProperty);
}

void PathFigure::SetSegments(const ObservableCollection<PathSegmentPtr>& segments)
{
	SetValue(SegmentsProperty, segments);
}
