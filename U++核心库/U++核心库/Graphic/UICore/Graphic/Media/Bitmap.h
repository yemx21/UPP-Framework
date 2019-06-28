#pragma once
#include "..\Resource.h"

using namespace System;
using namespace std;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class Bitmap;
			typedef std::shared_ptr<Bitmap>  BitmapPtr;

			class RenderContext;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Bitmap : public FeatureDependantResource
			{
			protected:
				Bitmap();
			public:
				virtual ~Bitmap() {}
			public:
				static const DependencyPropertyPtr SizeProperty;
			public:
				__uref Size GetPixelSize() const;
				__uproperty(get = GetPixelSize) Size PixelSize;
			private:
				friend RenderContext;

				__usctordef(Bitmap)
				REFLECT_CLASS(Bitmap)
			};
		}
	}
}