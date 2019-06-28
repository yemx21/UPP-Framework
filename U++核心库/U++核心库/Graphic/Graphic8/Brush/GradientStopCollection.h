#pragma once
#include "..\Resource.h"
using namespace System;


namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class GradientStopCollection;
				typedef std::shared_ptr < GradientStopCollection > GradientStopCollectionPtr;

				class GradientStopCollection : public DeviceDependantResource
				{
				public:
					GradientStopCollection() : m_ExtendMode(ExtendMode::Clamp) { InitializeCriticalSection(&cs); }
					virtual ~GradientStopCollection() { LeaveCriticalSection(&cs); DeleteCriticalSection(&cs); }
				public:
					void SetGradientStops(const ObservableCollection<GradientStop>& gradientStops);

					auto ExtendMode() const->Graphic::ExtendMode;
					void ExtendMode(Graphic::ExtendMode extendMode);

				protected:
					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr < IUnknown > ;
				private:
					ObservableCollection<GradientStop> m_stops;
					Graphic::ExtendMode                m_ExtendMode;
					CRITICAL_SECTION cs;
				};
			}
		}
	}
}
