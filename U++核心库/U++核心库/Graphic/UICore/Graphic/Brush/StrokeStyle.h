#pragma once
#include "..\Resource.h"
#include "..\Geometry\Geometry.h"
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class StrokeStyle;
			typedef std::shared_ptr<StrokeStyle> StrokeStylePtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API StrokeStyle : public FeatureIndependantResource
			{
			public:
				__uctor StrokeStyle();
				~StrokeStyle();
			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;
			public:
				CapStyle GetStartCap() const;
				void SetStartCap(CapStyle val);

				CapStyle GetEndCap() const;
				void SetEndCap(CapStyle val);

				CapStyle GetDashCap() const;
				void SetDashCap(CapStyle val);

				LineJoin GetLineJoinStyle() const;
				void SetLineJoinStyle(LineJoin val);

				double GetMiterLimit() const;
				void SetMiterLimit(double val);

				DashStyle GetLineDashStyle() const;
				void SetLineDashStyle(DashStyle val);

				double GetDashOffset() const;
				void SetDashOffset(double val);

				ObservableCollection<double>& GetDashes();
				void SetDashes(const ObservableCollection<double>&);
			private:
				vector<float> dashes_cache;
				EventToken dashesChangedToken;
				static void OnDashesChanged(StrokeStyle* sender, std::shared_ptr<ObservableCollection<double>> dashes, NotifyCollectionChangedEventArgs<double> args);
			private:
				static bool StartCapChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool EndCapChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool DashCapChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool LineJoinStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool MiterLimitChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool LineDashStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool DashOffsetChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool DashesChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);

			public:
				static const DependencyPropertyPtr StartCapProperty;
				static const DependencyPropertyPtr EndCapProperty;
				static const DependencyPropertyPtr DashCapProperty;
				static const DependencyPropertyPtr LineJoinStyleProperty;
				static const DependencyPropertyPtr MiterLimitProperty;
				static const DependencyPropertyPtr LineDashStyleProperty;
				static const DependencyPropertyPtr DashOffsetProperty;
				static const DependencyPropertyPtr DashesProperty;

				__uproperty(get = GetDashes, put = SetDashes) ObservableCollection<double>& Dashes;
				__uproperty(get = GetStartCap, put = SetStartCap) CapStyle StartCap;
				__uproperty(get = GetEndCap, put = SetEndCap) CapStyle EndCap;
				__uproperty(get = GetDashCap, put = SetDashCap) CapStyle DashCap;
				__uproperty(get = GetLineJoinStyle, put = SetLineJoinStyle) LineJoin LineJoinStyle;
				__uproperty(get = GetMiterLimit, put = SetMiterLimit) double MiterLimit;
				__uproperty(get = GetLineDashStyle, put = SetLineDashStyle) DashStyle LineDashStyle;
				__uproperty(get = GetDashOffset, put = SetDashOffset) double DashOffset;

				__usctordef(StrokeStyle)
				REFLECT_CLASS(StrokeStyle)
			};
		}
	}
}