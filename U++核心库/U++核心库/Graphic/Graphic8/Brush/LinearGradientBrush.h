#pragma once
#include "..\Resource.h"
#include "Brush.h"
#include "GradientStopCollection.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class LinearGradientBrush;
				typedef std::shared_ptr<LinearGradientBrush> LinearGradientBrushPtr;

				class LinearGradientBrush : public Brush
				{
				public:
					LinearGradientBrush(GradientStopCollectionPtr gradientStopCollection, Point start, Point end);
					LinearGradientBrush();;
					virtual ~LinearGradientBrush(void);
				public:
					auto GradientStopCollection() const -> const GradientStopCollectionPtr;
					void GradientStopCollection(const GradientStopCollectionPtr& stopCollection);
					auto Start() const->Point;
					void Start(Point val);

					auto End() const->Point;
					void End(Point val);
				protected:
					virtual auto IsResourceValid() const -> bool override;
					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr < IUnknown > ;
				private:
					GradientStopCollectionPtr m_gradientStopCollection;
					Point                     m_Start;
					Point                     m_End;
				};
			}
		}
	}
}

