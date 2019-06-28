#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class GeometryGroup;
			typedef std::shared_ptr<GeometryGroup> GeometryGroupPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API GeometryGroup : public Geometry
			{
			public:
				__uctor GeometryGroup(Graphic::FillMode mode);
				~GeometryGroup();

				Graphic::FillMode GetFillMode() const;
				void SetFillMode(Graphic::FillMode mode);

			private:
				ObservableCollection<GeometryPtr> Geometries;
				EventToken geometriesChangedToken;
				static void OnGeometriesChanged(GeometryGroup* sender, std::shared_ptr<ObservableCollection<GeometryPtr>> geometries, NotifyCollectionChangedEventArgs<GeometryPtr> args);

			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;

			private:
				static bool FillModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr FillModeProperty;

				ObservableCollection<GeometryPtr>& GetItems();

				__uproperty(get = GetFillMode, put = SetFillMode) Graphic::FillMode FillMode;
				__uproperty(get = GetItems) ObservableCollection<GeometryPtr>& Items;

				__usctordef(GeometryGroup)
				REFLECT_CLASS(GeometryGroup)
			};
		}
	}
}