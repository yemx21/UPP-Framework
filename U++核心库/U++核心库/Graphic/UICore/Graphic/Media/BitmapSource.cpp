#include "BitmapSource.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\Application_impl.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr BitmapSource::UriProperty = make_dependencyproperty<BitmapSource>();

__usctor(BitmapSource)
{
	BitmapSource::UriProperty.get()->Register<BitmapSource>(L"Uri",
		FrameworkPropertyMetadata::Create<wstring>(wstring(L""), FrameworkPropertyMetadataOptions::AffectsRender, &UriChanged));
	return true;
}
 
bool BitmapSource::UriChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{ 
	auto me = static_cast<BitmapSource*>(ptr);
	wstring m_uri = changedArgs.NewValue<wstring>();
	return me->UpdateUri(m_uri);
}

BitmapSource::BitmapSource(const wstring& uri, Application* app)
{
	if (!app) throw std::exception("invalid application");
	_app = app;
	SetValue(UriProperty, uri);
}

BitmapSource::~BitmapSource()
{

}

bool BitmapSource::UpdateUri(const wstring& uri)
{
	if (System::OSHelper::IsValidAddress(_app, sizeof(Application), false))
	{
		bool ret = false;
		GraphicFactory* factory = GetGraphicFactoryFromApplication(_app);
		if (!factory) return ret;
		if (!uri.empty())
		{
			Result<GraphicObject, GraphicFactoryError> result = factory->InternalCreateBitmap(uri.c_str());
			if (result.State == GraphicFactoryError::OK)
			{
				switch (factory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
				{
					SetValue(SizeProperty, (Size)factory->Mgr8->Run.Bitmap.GetSize(result.Value).Value);
					FeatureObjectPtr resource= make_shared<FeatureObject>(GraphicObjectTypes::Bitmap, result.Value, factory->featureLevel);
					ForceResetResource(resource, factory);
					ret = true;
				}
				break;
				}
			}
		}
		return ret;
	}
	return false;
}


FeatureObjectPtr BitmapSource::OnCreateResource(RenderContext* renderContext)
{
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;
	wstring m_uri = GetValue<wstring>(UriProperty);
	if (!m_uri.empty())
	{
		Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateBitmap(m_uri.c_str());
		if (result.State == GraphicFactoryError::OK)
		{
			switch (GraphicFactory->featureLevel)
			{
				case FeatureLevel::D2D1WithD3D11:
				{
					SetValue(SizeProperty, (Size)GraphicFactory->Mgr8->Run.Bitmap.GetSize(result.Value).Value);
					return make_shared<FeatureObject>(GraphicObjectTypes::Bitmap, result.Value, GraphicFactory->featureLevel);
				}
				break;
			}		
		}
	}
	return NULL;
}

void BitmapSource::SetUri(const wstring& text)
{
	SetValue(UriProperty, text);
}

wstring BitmapSource::GetUri() const
{
	return GetValue<wstring>(UriProperty);
}
