#pragma once
#include "PathSegment.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class BezierSegment;
			typedef std::shared_ptr<BezierSegment> BezierSegmentPtr;


			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API BezierSegment : public PathSegment
			{
			public:
				BezierSegment();
				BezierSegment(const Point& point1, const Point& point2, const Point& point3, bool isStroked, bool isSmoothJoin);
				virtual ~BezierSegment();
			protected:
				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;
				virtual bool OnNotifyChanged(const wstring& notifyname) override;

			public:
				static const DependencyPropertyPtr Point1Property;

				static const DependencyPropertyPtr Point2Property;

				static const DependencyPropertyPtr Point3Property;


				System::Point GetPoint1() const;
				void SetPoint1(const System::Point&);

				System::Point GetPoint2() const;
				void SetPoint2(const System::Point&);

				System::Point GetPoint3() const;
				void SetPoint3(const System::Point&);

				__uproperty(get = GetPoint1, put = SetPoint1) System::Point Point1;
				__uproperty(get = GetPoint2, put = SetPoint2) System::Point Point2;
				__uproperty(get = GetPoint3, put = SetPoint3) System::Point Point3;

				__usctordef(BezierSegment)
				REFLECT_CLASS(BezierSegment)
			};
		}
	}
}