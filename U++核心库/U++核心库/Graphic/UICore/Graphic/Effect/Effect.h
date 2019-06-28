#pragma once
#include "..\Resource.h"
#include "..\Media\BitmapSource.h"
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class Effect;
			typedef std::shared_ptr<Effect> EffectPtr;
			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Effect : public FeatureIndependantResource
			{
			public:
				virtual ~Effect() {};
			protected:
				virtual FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory);
				Effect();

				REFLECT_CLASS(Effect)
			};
		}
	}
}

