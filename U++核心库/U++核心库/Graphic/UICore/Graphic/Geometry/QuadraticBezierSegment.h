#pragma once
#include "PathSegment.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class QuadraticBezierSegment;
			typedef std::shared_ptr<QuadraticBezierSegment> QuadraticBezierSegmentPtr;


			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API QuadraticBezierSegment : public PathSegment
			{
			public:
				__udefctor QuadraticBezierSegment();
				__uctor QuadraticBezierSegment(const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin);
				virtual ~QuadraticBezierSegment();
			protected:
				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;
				virtual bool OnNotifyChanged(const wstring& notifyname) override;
			public:
				static const DependencyPropertyPtr Point1Property;

				static const DependencyPropertyPtr Point2Property;

				System::Point GetPoint1() const;
				void SetPoint1(const System::Point&);

				System::Point GetPoint2() const;
				void SetPoint2(const System::Point&);

				__uproperty(get = GetPoint1, put = SetPoint1) System::Point Point1;
				__uproperty(get = GetPoint2, put = SetPoint2) System::Point Point2;

				__usctordef(QuadraticBezierSegment)
				REFLECT_CLASS(QuadraticBezierSegment)
			};
		}
	}
}