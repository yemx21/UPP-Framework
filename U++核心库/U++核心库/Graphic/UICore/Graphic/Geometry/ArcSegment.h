#pragma once
#include "PathSegment.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class ArcSegment;
			typedef std::shared_ptr<ArcSegment> ArcSegmentPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API ArcSegment : public PathSegment
			{
			public:
				__udefctor ArcSegment();
				__uctor ArcSegment(const System::Point& point, const System::Size& size, double rotationAngle, ArcSize arcSize, Graphic::SweepDirection sweepDirection, bool isStroked, bool isSmoothJoin);
				virtual ~ArcSegment();
			protected:
				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;
				virtual bool OnNotifyChanged(const wstring& notifyname) override;
			public:
				static const DependencyPropertyPtr PointProperty;

				static const DependencyPropertyPtr SizeProperty;

				static const DependencyPropertyPtr RotationAngleProperty;

				static const DependencyPropertyPtr ArcSizeProperty;

				static const DependencyPropertyPtr SweepDirectionProperty;


				System::Point GetPoint() const;
				void SetPoint(const System::Point&);

				System::Size GetSize() const;
				void SetSize(const System::Size&);

				double GetRotationAngle() const;
				void SetRotationAngle(double);

				Graphic::ArcSize GetArcSize() const;
				void SetArcSize(Graphic::ArcSize);

				Graphic::SweepDirection GetSweepDirection() const;
				void SetSweepDirection(Graphic::SweepDirection);

				__uproperty(get = GetPoint, put = SetPoint) System::Point Point;
				__uproperty(get = GetSize, put = SetSize) System::Size Size;
				__uproperty(get = GetRotationAngle, put = SetRotationAngle) double RotationAngle;
				__uproperty(get = GetArcSize, put = SetArcSize) Graphic::ArcSize ArcSize;
				__uproperty(get = GetSweepDirection, put = SetSweepDirection) Graphic::SweepDirection SweepDirection;

				__usctordef(ArcSegment)
				REFLECT_CLASS(ArcSegment)
			};
		}
	}
}