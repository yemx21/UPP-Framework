#include "SolidColorBrush.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr SolidColorBrush::ColorProperty = make_dependencyproperty<SolidColorBrush>();

__usctor(SolidColorBrush)
{
	SolidColorBrush::ColorProperty.get()->Register<SolidColorBrush>(L"Color",
		FrameworkPropertyMetadata::Create<System::Color>(System::Color(), FrameworkPropertyMetadataOptions::AffectsRender, &ColorChanged));
	return true;
}


SolidColorBrush::SolidColorBrush(const System::Color& color)
{
	InternalSetValue(ColorProperty, color);
}

bool SolidColorBrush::ColorChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<SolidColorBrush*>(ptr);
	auto newValue = changedArgs.NewValue<System::Color>();
	auto oldValue = changedArgs.OldValue<System::Color>();

	if (me->m_renderContext)
	{
		GraphicFactory* GraphicFactory = me->m_renderContext->GetGraphicFactory();
		FeatureObjectPtr obj = me->GetResource(me->m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						return GraphicFactory->Mgr8->Run.SolidColorBrush.SetColor(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

FeatureObjectPtr SolidColorBrush::OnCreateResource(RenderContext* renderContext)
{
	if (!renderContext) return NULL;
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;

	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateSolidColorBrush(this->Color);
	if (result.State == GraphicFactoryError::OK) return make_shared<FeatureObject>(GraphicObjectTypes::SolidColorBrush, result.Value, GraphicFactory->featureLevel);
	return NULL;
}

System::Color SolidColorBrush::GetColor() const 
{
	return GetValue<System::Color>(ColorProperty);
}

void SolidColorBrush::SetColor(const System::Color& color)
{
	SetValue(ColorProperty, color);
}
