#pragma once
#include "..\Feature.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class FeatureManager8
			{
			private:
				static HMODULE dll;
				static unsigned int ref;
				static Feature8* feature;
				static FeatureInfo* info;
			public:
				FeatureManager8()
				{
					if (!dll)
					{
						CoInitializeEx(nullptr, COINIT_MULTITHREADED);
						dll = LoadLibrary(Feature_8);
						if (dll)
						{
							GetFeatureInfo pInfoFunction = (GetFeatureInfo)GetProcAddress(dll, MAKEINTRESOURCEA(1));
							if (pInfoFunction)
							{
								info = pInfoFunction();
								if (info)
								{
									if (info->FeatureLevel == (unsigned int)FeatureLevel::D2D1WithD3D11)
									{
										GetFeature8 pFeatureFunction = (GetFeature8)GetProcAddress(dll, MAKEINTRESOURCEA(2));
										if (pFeatureFunction)
										{
											feature = pFeatureFunction();
										}
										pFeatureFunction = NULL;
									}
								}
							}
							pInfoFunction = NULL;
						}
					}
					if (!dll || !info || !feature || !info)
					{
						SAFE_DELETE(info);
						SAFE_DELETE(feature);
						if (dll) { FreeLibrary(dll); dll = NULL; }
						throw 1;
					}
					else
					{
						ref++;
					}
				}
				~FeatureManager8()
				{
					ref--;
					if (ref == 0 && dll)
					{
						SAFE_DELETE(info);
						SAFE_DELETE(feature);
						CoUninitialize();
						FreeLibrary(dll);
					}
				}
			public:
				const Feature8& GetRun()const
				{
					return *feature;
				}
				_declspec(property(get = GetRun))const Feature8& Run;
			};

		}
	}
}

