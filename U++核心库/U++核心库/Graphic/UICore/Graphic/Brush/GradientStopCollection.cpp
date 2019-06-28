#include "GradientStopCollection.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr GradientStopCollection::ExtendModeProperty = make_dependencyproperty<GradientStopCollection>();
const DependencyPropertyPtr GradientStopCollection::ItemsProperty = make_dependencyproperty<GradientStopCollection>();

__usctor(GradientStopCollection)
{
	GradientStopCollection::ExtendModeProperty.get()->Register<GradientStopCollection>(L"ExtendMode",
		FrameworkPropertyMetadata::Create<Graphic::ExtendMode>((Graphic::ExtendMode)Graphic::ExtendMode::Clamp, FrameworkPropertyMetadataOptions::AffectsRender, &ExtendModeChanged));

	GradientStopCollection::ItemsProperty.get()->Register<GradientStopCollection>(L"Items",
		FrameworkPropertyMetadata::Create<ObservableCollection<GradientStop>>(nullptr, FrameworkPropertyMetadataOptions::AffectsUpdate, &ItemsChanged));
	return true;
}



bool GradientStopCollection::ExtendModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<GradientStopCollection*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::ExtendMode>();
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
						return GraphicFactory->Mgr8->Run.GradientStopCollection.SetExtendMode(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

void GradientStopCollection::OnGradientStopsChanged(GradientStopCollection* sender, std::shared_ptr<ObservableCollection<GradientStop>> stops, NotifyCollectionChangedEventArgs<GradientStop> args)
{
	if (sender)
	{
		if (sender->m_renderContext)
		{
			GraphicFactory* GraphicFactory = sender->m_renderContext->GetGraphicFactory();
			FeatureObjectPtr obj = sender->GetResource(sender->m_renderContext);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					switch (GraphicFactory->featureLevel)
					{
						case FeatureLevel::D2D1WithD3D11:
						{
							GraphicFactory->Mgr8->Run.GradientStopCollection.SetGradientStops(gobj, *stops.get());
							sender->Invalidate(InvalidateFlags::Visual);
						}
						break;
					}
				}
			}
		}
	}
}

bool GradientStopCollection::ItemsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<GradientStopCollection*>(ptr);
	ObservableCollection<GradientStop>& oldOC = changedArgs.OldValueRef<ObservableCollection<GradientStop>>();
	ObservableCollection<GradientStop>& newOC = changedArgs.NewValueRef<ObservableCollection<GradientStop>>();
	if (me)
	{
		if (&oldOC != nullptr) oldOC.CollectionChanged -= me->gradientStopsChangedToken;
		if (&newOC != nullptr) me->gradientStopsChangedToken = newOC.CollectionChanged += (bind(&GradientStopCollection::OnGradientStopsChanged, me, std::placeholders::_1, std::placeholders::_2));

		return true;
	}
	return false;
}

GradientStopCollection::GradientStopCollection()
{
	SetValue(ItemsProperty, ObservableCollection<GradientStop>{});
}


GradientStopCollection::~GradientStopCollection()
{
	if (ItemsProperty)
	{
		ObservableCollection<GradientStop>&  oldOC = GetValueRef<ObservableCollection<GradientStop>>(ItemsProperty);
		if (&oldOC != nullptr) oldOC.CollectionChanged -= gradientStopsChangedToken;
	}
}

FeatureObjectPtr GradientStopCollection::OnCreateResource(RenderContext* renderContext)
{
	if (!renderContext) return NULL;
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;

	ObservableCollection<GradientStop>& gs = GetValueRef<ObservableCollection<GradientStop>>(ItemsProperty);
	if (&gs == nullptr) return NULL;

	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateGradientStopCollection(this->ExtendMode, gs);
	if (result.State == GraphicFactoryError::OK) return make_shared<FeatureObject>(GraphicObjectTypes::GradientStopCollection, result.Value, GraphicFactory->featureLevel);
	return NULL;
}

ObservableCollection<GradientStop>& GradientStopCollection::GetItems()
{
	return GetValueRef<ObservableCollection<GradientStop>>(ItemsProperty);
}

void GradientStopCollection::SetItems(const ObservableCollection<GradientStop>& items)
{
	SetValue(ItemsProperty, items);
}

Graphic::ExtendMode GradientStopCollection::GetExtendMode()  const 
{
	return GetValue<Graphic::ExtendMode>(ExtendModeProperty);
}

void GradientStopCollection::SetExtendMode(Graphic::ExtendMode val)
{
	SetValue(ExtendModeProperty, val);
}
