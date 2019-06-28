#include "GradientStopCollection.h"

using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic::d2d1;
using namespace std;
using namespace Microsoft::WRL;

void GradientStopCollection::SetGradientStops(const ObservableCollection<GradientStop>& gradientStops)
{
	EnterCriticalSection(&cs);
	m_stops=gradientStops;
	LeaveCriticalSection(&cs);
	Invalidate();
}

ComPtr<IUnknown> GradientStopCollection::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
	EnterCriticalSection(&cs);
	int noOfStops = int(m_stops.size);

	auto stops = unique_ptr<D2D1_GRADIENT_STOP[]>(new D2D1_GRADIENT_STOP[noOfStops]);

	for (int i = 0; i < noOfStops; ++i)
	{
		stops[i].color = D2DColor(m_stops[i].Color);
		stops[i].position = m_stops[i].Offset;
	}

	ID2D1GradientStopCollection* gsc = nullptr;
	if (deviceContext->CreateGradientStopCollection(stops.get(),
		noOfStops,
		D2D1_GAMMA_2_2,
		(D2D1_EXTEND_MODE)(UInt)m_ExtendMode,
		&gsc) != S_OK)
	{
		gsc = nullptr;
	}
	LeaveCriticalSection(&cs);
	return gsc;
}


auto GradientStopCollection::ExtendMode() const->Graphic::ExtendMode
{
	return m_ExtendMode;
}

void GradientStopCollection::ExtendMode(Graphic::ExtendMode extendMode)
{
	m_ExtendMode = extendMode;
    Invalidate();
}
