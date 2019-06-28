#pragma once
#include "..\Resource.h"
using namespace System;


namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class GradientStopCollection;
			typedef std::shared_ptr < GradientStopCollection > GradientStopCollectionPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API GradientStopCollection : public FeatureDependantResource
			{
			public:
				__uctor GradientStopCollection();
				~GradientStopCollection();
			private:
				static bool ExtendModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			private:
				EventToken gradientStopsChangedToken;
				static void OnGradientStopsChanged(GradientStopCollection* sender, std::shared_ptr<ObservableCollection<GradientStop>> gradientStops, NotifyCollectionChangedEventArgs<GradientStop> args);
				static bool ItemsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr ExtendModeProperty;
				static const DependencyPropertyPtr ItemsProperty;
			public:

				Graphic::ExtendMode GetExtendMode() const;
				void SetExtendMode(Graphic::ExtendMode extendMode);

				ObservableCollection<GradientStop>& GetItems();
				void SetItems(const ObservableCollection<GradientStop>&);

				__uproperty(get = GetItems, put = SetItems) ObservableCollection<GradientStop>& Items;

				__uproperty(get = GetExtendMode, put = SetExtendMode) Graphic::ExtendMode ExtendMode;

			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
				
				__usctordef(GradientStopCollection)
				REFLECT_CLASS(GradientStopCollection)
			};
		}
	}
}
