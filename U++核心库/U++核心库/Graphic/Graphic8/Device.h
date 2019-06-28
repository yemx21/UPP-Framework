#pragma once

#include "Header.h"

#include <memory>
#include <wrl.h>
#include <queue>


namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Device;
				typedef std::shared_ptr<Device> DevicePtr;

				class Device
				{
					typedef Microsoft::WRL::ComPtr<ID3D11Device> ID3D11DevicePtr;
				public:
					Device();
					~Device(void);
				};
			}
		}
	}
}