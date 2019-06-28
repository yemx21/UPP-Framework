#pragma once
#include "ExecutableBase.h"
#include "..\Media\Bitmap.h"
#include "..\Effects\ElementEffect.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class DrawEffectCommand : ExecutableBase
				{
					typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext>  ID2D1DeviceContextPtr;
				public:
					DrawEffectCommand();

					virtual ~DrawEffectCommand(void);
				public:
					BitmapPtr Bitmap() const { return m_bitmap; }
					void Bitmap(const BitmapPtr& val) { m_bitmap = val; }

					ElementEffect* ElementEffect() const { return m_elementEffect; }
					void ElementEffect(d2d1::ElementEffect* const val) { m_elementEffect = val; }

					EffectCompositeMode CompositeMode() const { return m_compositeMode; }
					void CompositeMode(const EffectCompositeMode val) { m_compositeMode = val; }

					System::Size TargetSize() const { return m_targetSize; }
					void TargetSize(const System::Size& val) { m_targetSize = val; }

					ID2D1DeviceContext* DeviceContext() const { return m_deviceContext; }
					void DeviceContext(ID2D1DeviceContext* const val) { m_deviceContext = val; }
				public:
					virtual void Run();
				private:
					BitmapPtr      m_bitmap;
					EffectCompositeMode       m_compositeMode;
					System::Size           m_targetSize;
					ID2D1DeviceContext*       m_deviceContext;
					d2d1::ElementEffect* m_elementEffect;

				};
			}
		}
	}
}
