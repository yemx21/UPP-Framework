#pragma once
#include "Bitmap.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class WriteableBitmap;
			typedef std::shared_ptr<WriteableBitmap>  WriteableBitmapPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API WriteableBitmap : public Bitmap
			{
			public:
				__uctor WriteableBitmap(const Size& pixelSize);
				~WriteableBitmap();
			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			private:
				friend RenderContext;

				REFLECT_CLASS(WriteableBitmap)
			};
		}
	}
}