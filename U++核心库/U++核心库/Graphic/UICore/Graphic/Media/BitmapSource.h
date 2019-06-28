#pragma once
#include "Bitmap.h"
#include "..\..\Application.h"
using namespace System;
using namespace std;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class BitmapSource;
			typedef std::shared_ptr<BitmapSource>  BitmapSourcePtr;

			class RenderContext;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API BitmapSource : public Bitmap
			{
			public:
				__uctor BitmapSource(const wstring& uri, Application* app);
				~BitmapSource();
			public:
				static const DependencyPropertyPtr UriProperty;
			protected:
				FeatureObjectPtr OnCreateResource(RenderContext* renderContext) override;
			private:
				static bool UriChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				void SetUri(const wstring& uri);
				wstring GetUri() const;
				bool UpdateUri(const wstring& uri);
			public:
				__uproperty(get = GetUri, put = SetUri) wstring Uri;
			private:
				friend RenderContext;
				Application* _app;

				__usctordef(BitmapSource)
				REFLECT_CLASS(BitmapSource)
			};
		}
	}
}