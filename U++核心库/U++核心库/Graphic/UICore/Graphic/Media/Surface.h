#pragma once
#include "..\Resource.h"
__uimport("Texture2D.h")

using namespace System;
using namespace std;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class Surface;
			typedef std::shared_ptr<Surface>  SurfacePtr;

			class Texture2D;
			typedef std::shared_ptr<Texture2D>  Texture2DPtr;

			class RenderContext;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Surface : public FeatureDependantResource
			{
			public:
				__udefctor Surface();
				__uctor Surface(const Texture2DPtr& texture2D);
				virtual ~Surface() {}
			public:
				static const DependencyPropertyPtr SizeProperty;
				static const DependencyPropertyPtr Texture2DProperty;
			protected:
				virtual auto OnCreateResource(RenderContext* renderContext)->FeatureObjectPtr;
			public:
				__uref Size GetPixelSize() const;
				__uref Texture2DPtr GetTexture2D() const;
				__uproperty(get = GetPixelSize)Size PixelSize;
				__uproperty(get = GetTexture2D) Texture2DPtr Texture2D;

				__uref bool CopyTo(const SurfacePtr& surfaceCPU);

			private:
				friend class RenderContext;

				__usctordef(Surface)
				REFLECT_CLASS(Surface)
			};
		}
	}
}