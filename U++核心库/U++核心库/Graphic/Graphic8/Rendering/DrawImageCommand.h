#pragma once
#include "ExecutableBase.h"
#include "..\Media\Bitmap.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class DrawImageCommand : ExecutableBase
				{
					typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext>  ID2D1DeviceContextPtr;
				public:
					DrawImageCommand();

					virtual ~DrawImageCommand(void);
				public:
					Bitmap* Bitmap() const { return m_bitmap; }
					void Bitmap(d2d1::Bitmap * const val) { m_bitmap = val; }

					EffectCompositeMode CompositeMode() const { return m_compositeMode; }
					void CompositeMode(const EffectCompositeMode val) { m_compositeMode = val; }

					System::Size TargetSize() const { return m_targetSize; }
					void TargetSize(const System::Size& val) { m_targetSize = val; }

					ID2D1DeviceContext* DeviceContext() const { return m_deviceContext; }
					void DeviceContext(ID2D1DeviceContext* const val) { m_deviceContext = val; }
				public:
					virtual void Run();
				private:
					d2d1::Bitmap*     m_bitmap;
					EffectCompositeMode    m_compositeMode;
					System::Size        m_targetSize;
					ID2D1DeviceContext*    m_deviceContext;
				};
			}
		}
	}
}