#include "CombinedGeometry.h"

using namespace System;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1Geometry> CombinedGeometry::OnCreateGeometry()
{
    ComPtr<ID2D1PathGeometry> path;
    ComPtr<ID2D1GeometrySink> sink;

	if (m_Factory->CreatePathGeometry(&path) != S_OK)
	{
		return nullptr;
	}
	if (path->Open(&sink) != S_OK)
	{
		path = nullptr;
		return nullptr;
	}

    auto g1 = GetResourceFrom<ID2D1Geometry>(m_Geometry1);
    auto g2 = GetResourceFrom<ID2D1Geometry>(m_Geometry2);

	if (g1->CombineWithGeometry(g2, (D2D1_COMBINE_MODE)(UInt)m_Mode, nullptr/*optional transform*/, sink.Get()) != S_OK)
	{
		path = nullptr;
		sink = nullptr;
		return nullptr;
	}

	if (sink->Close() != S_OK)
	{
		path = nullptr;
		sink = nullptr;
		return nullptr;
	}

	return path;
}
