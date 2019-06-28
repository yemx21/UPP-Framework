#pragma once
#include "PathSegment.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class PathFigure;
			typedef std::shared_ptr<PathFigure> PathFigurePtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API PathFigure : public DependantObject, public DependencyObject, public IObservableObject<PathFigure >
			{
			public:
				__udefctor PathFigure();
				__uctor PathFigure(const Point& startPoint, bool isFilled, bool isClosed);
				__uctor PathFigure(const Point& startPoint, bool isFilled, bool isClosed, const ObservableCollection<PathSegmentPtr>& segments);
				virtual ~PathFigure();
			protected:
				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;
				virtual bool OnNotifyChanged(const wstring& notifyname) override;
			private:
				EventToken PathSegmentsChangedToken;
				static void OnPathSegmentsChanged(PathFigure* sender, std::shared_ptr<ObservableCollection<PathSegmentPtr>> PathSegments, NotifyCollectionChangedEventArgs<PathSegmentPtr> args);
				static bool SegmentsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr StartPointProperty;

				static const DependencyPropertyPtr IsFilledProperty;

				static const DependencyPropertyPtr SegmentsProperty;

				static const DependencyPropertyPtr IsClosedProperty;
			public:

				System::Point GetStartPoint() const;
				void SetStartPoint(const System::Point&);

				bool GetIsFilled() const;
				void SetIsFilled(bool);

				ObservableCollection<PathSegmentPtr>& GetSegments() const;
				void SetSegments(const ObservableCollection<PathSegmentPtr>&);

				bool GetIsClosed() const;
				void SetIsClosed(bool);

				__uproperty(get = GetStartPoint, put = SetStartPoint) System::Point StartPoint;
				__uproperty(get = GetIsFilled, put = SetIsFilled) bool IsFilled;
				__uproperty(get = GetSegments, put = SetSegments) ObservableCollection<PathSegmentPtr>& Segments;
				__uproperty(get = GetIsClosed, put = SetIsClosed) bool IsClosed;

				__usctordef(PathFigure)
				REFLECT_CLASS(PathFigure)
			};
		}
	}
}