#pragma once
#include "PathSegment.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class LineSegment;
			typedef std::shared_ptr<LineSegment> LineSegmentPtr;


			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API LineSegment : public PathSegment
			{
			public:
				__udefctor LineSegment();
				__uctor LineSegment(const System::Point& point, bool isStroked, bool isSmoothJoin);
				virtual ~LineSegment();
			protected:
				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args);
				virtual bool OnNotifyChanged(const wstring& notifyname);

			public:
				static const DependencyPropertyPtr PointProperty;

				System::Point GetPoint() const;
				void SetPoint(const System::Point&);

				__uproperty(get = GetPoint, put = SetPoint) System::Point Point;

				__usctordef(LineSegment)
				REFLECT_CLASS(LineSegment)
			};
		}
	}
}