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
				class ExecutableBase
				{
				public:
					ExecutableBase() {}
					virtual ~ExecutableBase() {}

					virtual void Run() = 0;
				protected:
					static auto IsResourceValidFrom(const DeviceDependantResourcePtr& resource) -> bool
					{
						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(DeviceDependantResource* const resource) -> bool
					{
						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(const IndependantResourcePtr& resource) -> bool
					{
						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(IndependantResource* const resource) -> bool
					{
						return resource->IsResourceValid();
					}

					template<typename T>
					T* GetResourceFrom(const IndependantResourcePtr& independantResource)
					{
						return independantResource->GetResource<T>();
					}

					template<typename T>
					T* GetResourceFrom(IndependantResource* const independantResource)
					{
						return independantResource->GetResource<T>();
					}

					template<typename T>
					T* GetResourceFrom(const DeviceDependantResourcePtr& deviceDependantResource, ID2D1DeviceContext* deviceContext)
					{
						return deviceDependantResource->GetResource<T>(deviceContext);
					}

					template<typename T>
					T* GetResourceFrom(DeviceDependantResource* const deviceDependantResource, ID2D1DeviceContext* deviceContext)
					{
						return deviceDependantResource->GetResource<T>(deviceContext);
					}
				};
			}
		}
	}
}
