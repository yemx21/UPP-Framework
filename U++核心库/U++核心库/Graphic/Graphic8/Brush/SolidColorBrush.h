#pragma once
#include "Brush.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class SolidColorBrush;
				typedef std::shared_ptr<SolidColorBrush> SolidColorBrushPtr;

				class SolidColorBrush : public Brush
				{
				public:
					explicit SolidColorBrush(const System::Color& c) : m_Color(c) {}
					virtual ~SolidColorBrush() {}
				public:
					void FillColor(const System::Color& c);
					auto FillColor() const->System::Color;
				private:
					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr<IUnknown> override;
				private:
					System::Color m_Color;
				};
			}
		}
	}
}