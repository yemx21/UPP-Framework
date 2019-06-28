#include "GeometryGroup.h"

#include "CombinedGeometry.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;

void GeometryGroup::OnGeometriesChanged(GeometryGroup* sender, std::shared_ptr<ObservableCollection<intptr_t>> geometries, NotifyCollectionChangedEventArgs<intptr_t> args)
{
	if (sender)
	{
		sender->Invalidate();
	}
}

Microsoft::WRL::ComPtr<ID2D1Geometry> GeometryGroup::OnCreateGeometry()
{
	try
	{
		ComPtr<ID2D1GeometryGroup> path;
		std::vector<ID2D1Geometry*> geometries;
		unsigned int count = Geometries.size;
		if (count)
		{
			for (ObservableCollection<intptr_t>::iterator iter = Geometries.begin(); iter != Geometries.end(); iter++)
			{
				intptr_t v = *iter;
				if (v)
				{
					GeometryPtr* g = reinterpret_cast<GeometryPtr*>(v);
					if (g)
					{
						ID2D1Geometry* geo = GetResourceFrom<ID2D1Geometry>(*g);
						if (geo)
							geometries.push_back(geo);
					}
				}
			}
		}
		m_Factory->CreateGeometryGroup((D2D1_FILL_MODE)(UInt)m_fillMode, geometries.size() > 0 ? &geometries[0] : NULL, geometries.size(), path.GetAddressOf());

		geometries.clear();

		return path;
	}
	catch (...)
	{
		return NULL;
	}
}
