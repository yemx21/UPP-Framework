#pragma once
#include <map>
#include <vector>
#include "..\Resource.h"
#include "..\Media\Bitmap.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class EffectResource;
				typedef std::shared_ptr<EffectResource> EffectResourcePtr;

				class EffectResource : public DeviceDependantResource
				{
				public:
					EffectResource(const IID& effectCLSID);
					virtual ~EffectResource(void);
				public:
					void SetInput(int index, const BitmapPtr& image);
					void SetInput(int index, const EffectResourcePtr& image);
					void ClearInputs();

					template<typename T, typename U>
					void SetValue(U index, const T &value)
					{
						auto idx = static_cast<UINT32>(index);
						auto val = reinterpret_cast<const BYTE *>(&value);
						auto size = sizeof(value);

						auto settingData = std::vector<CHAR>(size);
						memcpy_s(&settingData[0], size, val, size);

						m_settings[idx] = settingData;

						if (IsResourceValid())
						{
							auto effect = GetResourceUnsafe<ID2D1Effect>();
							if (effect != nullptr)
								effect->SetValue(idx, val, (UINT32)size);
						}
					}
					virtual auto IsResourceValid() const -> bool override;
				private:
					void ApplyAllInputs(const Microsoft::WRL::ComPtr<ID2D1Effect>& effect);

					virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext)->Microsoft::WRL::ComPtr<IUnknown> override;
				private:
					mutable std::map<int, BitmapPtr>          m_imageInputs;
					mutable std::map<int, EffectResourcePtr> m_effectInputs;
					mutable IID                              m_effectCLSID;
					std::map<int, std::vector<CHAR>>         m_settings;
				};
			}
		}
	}
}
