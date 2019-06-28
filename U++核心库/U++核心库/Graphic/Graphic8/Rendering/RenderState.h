#pragma once
#include "..\Media\Bitmap.h"

#include <memory>
#include <wrl.h>
#include <vector>

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				enum class ClipStateType
				{
					AxisAlignedClip,
					Layer
				};

				struct ClipState
				{
					System::Rect clipArea;
					std::shared_ptr<ID2D1Layer> layer;
					ClipStateType clipType;
					D2D1::Matrix3x2F transform;

					ClipState(std::shared_ptr<ID2D1Layer>& layer, const D2D1::Matrix3x2F& currentTransform)
					{
						transform = currentTransform;
						this->layer = layer;
						clipType = ClipStateType::Layer;
					}

					ClipState(const System::Rect& clipAreaRect, const D2D1::Matrix3x2F& currentTransform)
					{
						clipArea = clipAreaRect;
						transform = currentTransform;
						clipType = ClipStateType::AxisAlignedClip;
					}

					void PushClip(Microsoft::WRL::ComPtr<ID2D1DeviceContext>& deviceContext)
					{
						deviceContext->SetTransform(transform);
						if (clipType == ClipStateType::AxisAlignedClip)
						{
							deviceContext->PushAxisAlignedClip(D2DRect(clipArea), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
						}
						else
						{
							deviceContext->PopLayer();
						}
					}

					void PopClip(Microsoft::WRL::ComPtr<ID2D1DeviceContext>& deviceContext)
					{
						if (!layer)
							deviceContext->PopAxisAlignedClip();
						else
						{

						}
					}
				};

				struct RenderState
				{
					RenderState()
					{
					}

					RenderState(const BitmapPtr& rt) : renderTarget(rt)
					{

					}
					~RenderState()
					{
						renderTarget = nullptr;
					}

					BitmapPtr              renderTarget;
					std::vector<ClipState> clipQueue;
				};
			}
		}
	}
}