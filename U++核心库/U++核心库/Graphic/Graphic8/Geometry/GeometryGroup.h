#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{

				class GeometryGroup;
				typedef std::shared_ptr<GeometryGroup> GeometryGroupPtr;



				class GeometryGroup : public Geometry
				{
				private:
					EventToken geometriesChangedToken;
					static void OnGeometriesChanged(GeometryGroup* sender, std::shared_ptr<ObservableCollection<intptr_t>> geometries, NotifyCollectionChangedEventArgs<intptr_t> args);

				public:
					ObservableCollection<intptr_t> Geometries;

					GeometryGroup(GeometryPtr* gs, unsigned int count, Graphic::FillMode mode)
						:m_fillMode(mode)
					{
						if (gs && count > 0)
							for (unsigned int i = 0; i < count; i++)
								Geometries.add((intptr_t)(&gs[i]));

						geometriesChangedToken = Geometries.CollectionChanged += (bind(&GeometryGroup::OnGeometriesChanged, this, std::placeholders::_1, std::placeholders::_2));
					}

					GeometryGroup(Graphic::FillMode mode)
						:m_fillMode(mode)
					{

					}

					~GeometryGroup()
					{
						Geometries.CollectionChanged -= (geometriesChangedToken);
						Geometries.clear();
					}


				public:
					void FillMode(FillMode mode)
					{
						m_fillMode = mode;
						Invalidate();
					}

					Graphic::FillMode FillMode()
					{
						return m_fillMode;
					}

				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					Graphic::FillMode m_fillMode;
				};
			}
		}
	}
}