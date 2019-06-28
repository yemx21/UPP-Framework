#include "LinearGradientBrush.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr LinearGradientBrush::StartProperty = make_dependencyproperty<LinearGradientBrush>();
const DependencyPropertyPtr LinearGradientBrush::EndProperty = make_dependencyproperty<LinearGradientBrush>();
const DependencyPropertyPtr LinearGradientBrush::GradientStopsProperty = make_dependencyproperty<LinearGradientBrush>();

__usctor(LinearGradientBrush)
{
	LinearGradientBrush::StartProperty.get()->Register<LinearGradientBrush>(L"Start",
		FrameworkPropertyMetadata::Create<Point>(Point(0.5, 0.0), FrameworkPropertyMetadataOptions::AffectsRender, &StartChanged));

	LinearGradientBrush::EndProperty.get()->Register<LinearGradientBrush>(L"End",
		FrameworkPropertyMetadata::Create<Point>(Point(0.5, 1.0), FrameworkPropertyMetadataOptions::AffectsRender, &EndChanged));

	LinearGradientBrush::GradientStopsProperty.get()->Register<LinearGradientBrush>(L"GradientStops",
		FrameworkPropertyMetadata::Create<GradientStopCollectionPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender, &GradientStopsChanged));

	return true;
}


bool LinearGradientBrush::StartChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<LinearGradientBrush*>(ptr);
	auto newValue = changedArgs.NewValue<Point>();
	auto oldValue = changedArgs.OldValue<Point>();

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
						return GraphicFactory->Mgr8->Run.LinearGradientBrush.SetStartPoint(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool LinearGradientBrush::EndChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<LinearGradientBrush*>(ptr);
	auto newValue = changedArgs.NewValue<Point>();
	auto oldValue = changedArgs.OldValue<Point>();

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
						return GraphicFactory->Mgr8->Run.LinearGradientBrush.SetEndPoint(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool LinearGradientBrush::GradientStopsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<LinearGradientBrush*>(ptr);
	auto newValue = changedArgs.NewValue<GradientStopCollectionPtr>();
	auto oldValue = changedArgs.OldValue<GradientStopCollectionPtr>();

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
						GraphicObject gsobj = NULL;
						if (newValue)
						{
							FeatureObjectPtr gsfobj = newValue->GetResource(me->m_renderContext);
							if (gsfobj) gsobj = gsfobj->Get();
						}	
						return GraphicFactory->Mgr8->Run.LinearGradientBrush.SetGradientStopCollection(gobj, gsobj) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

FeatureObjectPtr LinearGradientBrush::OnCreateResource(RenderContext* renderContext)
{
	if (!renderContext) return NULL;
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;
	GradientStopCollectionPtr gsc = GradientStops;
	if (gsc)
	{
		FeatureObjectPtr obj = gsc->GetResource(renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateLinearGradientBrush(gobj, Start, End);
				if (result.State == GraphicFactoryError::OK) return make_shared<FeatureObject>(GraphicObjectTypes::LinearGradientBrush, result.Value, GraphicFactory->featureLevel);
			}
		}
	}
	return NULL;
}

LinearGradientBrush::LinearGradientBrush(const GradientStopCollectionPtr& gradientStops, const Point& start, const Point& end)
{
	InternalSetValue(GradientStopsProperty, gradientStops);
	InternalSetValue(StartProperty, start);
	InternalSetValue(EndProperty, end);
}

Point LinearGradientBrush::GetStart() const
{
	return GetValue<Point>(StartProperty);
}

void LinearGradientBrush::SetStart(const Point& start)
{
	SetValue(StartProperty, start);
}

Point LinearGradientBrush::GetEnd() const
{
	return GetValue<Point>(EndProperty);
}

void LinearGradientBrush::SetEnd(const Point& end)
{
	SetValue(EndProperty, end);
}

GradientStopCollectionPtr LinearGradientBrush::GetGradientStops() const
{
	return GetValuePtr<GradientStopCollection>(GradientStopsProperty);
}

void LinearGradientBrush::SetGradientStops(const GradientStopCollectionPtr& gradientStops)
{
	SetValue(GradientStopsProperty, gradientStops);
}