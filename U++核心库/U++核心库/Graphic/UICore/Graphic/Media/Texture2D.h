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
			class Texture2D;
			typedef std::shared_ptr<Texture2D>  Texture2DPtr;

			class RenderContext;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Texture2D : public FeatureDependantResource
			{
			public:
				__udefctor Texture2D();
				__uctor Texture2D(double width, double height, Texture2DMode mode);
				__uctor Texture2D(const Size& size, Texture2DMode mode);
				virtual ~Texture2D() {}
			public:
				static const DependencyPropertyPtr SizeProperty;
				static const DependencyPropertyPtr ModeProperty;
			protected:
				virtual auto OnCreateResource(RenderContext* renderContext)->FeatureObjectPtr;
			public:
				__uref Size GetPixelSize() const;
				__uref Texture2DMode GetMode() const;
				__uproperty(get = GetPixelSize)Size PixelSize;
				__uproperty(get = GetMode) Texture2DMode Mode;

				__uref bool Map(BitmapData* data);
				__uref void Unmap();
			private:
				friend class RenderContext;

				__usctordef(Texture2D)
				REFLECT_CLASS(Texture2D)
			};
		}
	}
}