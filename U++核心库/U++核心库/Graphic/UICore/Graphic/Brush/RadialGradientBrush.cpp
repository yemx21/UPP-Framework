#include "RadialGradientBrush.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System::UI;
using namespace System::UI::Graphic;
 
const DependencyPropertyPtr RadialGradientBrush::CenterProperty = make_dependencyproperty<RadialGradientBrush>();
const DependencyPropertyPtr RadialGradientBrush::GradientOriginOffsetProperty = make_dependencyproperty<RadialGradientBrush>();
const DependencyPropertyPtr RadialGradientBrush::RadiusXProperty = make_dependencyproperty<RadialGradientBrush>();
const DependencyPropertyPtr RadialGradientBrush::RadiusYProperty = make_dependencyproperty<RadialGradientBrush>();
const DependencyPropertyPtr RadialGradientBrush::GradientStopsProperty = make_dependencyproperty<RadialGradientBrush>();

__usctor(RadialGradientBrush)
{
	RadialGradientBrush::CenterProperty.get()->Register<RadialGradientBrush>(L"Center",
		FrameworkPropertyMetadata::Create<Point>(Point(0.5, 0.5), FrameworkPropertyMetadataOptions::AffectsRender, &CenterChanged));

	RadialGradientBrush::GradientOriginOffsetProperty.get()->Register<RadialGradientBrush>(L"GradientOriginOffset",
		FrameworkPropertyMetadata::Create<Point>(Point(0.5, 0.5), FrameworkPropertyMetadataOptions::AffectsRender, &GradientOriginOffsetChanged));

	RadialGradientBrush::RadiusXProperty.get()->Register<RadialGradientBrush>(L"RadiusX",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &RadiusXChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));

	RadialGradientBrush::RadiusYProperty.get()->Register<RadialGradientBrush>(L"RadiusY",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &RadiusYChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));

	RadialGradientBrush::GradientStopsProperty.get()->Register<RadialGradientBrush>(L"GradientStops",
		FrameworkPropertyMetadata::Create<GradientStopCollectionPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender, &GradientStopsChanged));
	return true;
}


RadialGradientBrush::RadialGradientBrush(const GradientStopCollectionPtr& gradientStops, Point center, Point gradientOriginOffset, float radiusX, float radiusY)
{
	InternalSetValue(GradientStopsProperty, gradientStops);
	InternalSetValue(CenterProperty, center);
	InternalSetValue(GradientOriginOffsetProperty, gradientOriginOffset);
	InternalSetValue(RadiusXProperty, radiusX);
	InternalSetValue(RadiusYProperty, radiusY);
}

bool RadialGradientBrush::CenterChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RadialGradientBrush*>(ptr);
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
						return GraphicFactory->Mgr8->Run.RadialGradientBrush.SetCenterPoint(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool RadialGradientBrush::GradientOriginOffsetChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RadialGradientBrush*>(ptr);
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
						return GraphicFactory->Mgr8->Run.RadialGradientBrush.SetGradientOriginOffset(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool RadialGradientBrush::RadiusXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RadialGradientBrush*>(ptr);
	auto newValue = changedArgs.NewValue<float>();
	auto oldValue = changedArgs.OldValue<float>();

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
						return GraphicFactory->Mgr8->Run.RadialGradientBrush.SetRadiusX(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool RadialGradientBrush::RadiusYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RadialGradientBrush*>(ptr);
	auto newValue = changedArgs.NewValue<float>();
	auto oldValue = changedArgs.OldValue<float>();

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
						return GraphicFactory->Mgr8->Run.RadialGradientBrush.SetRadiusY(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool RadialGradientBrush::GradientStopsChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RadialGradientBrush*>(ptr);
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
						return GraphicFactory->Mgr8->Run.RadialGradientBrush.SetGradientStopCollection(gobj, gsobj) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

FeatureObjectPtr RadialGradientBrush::OnCreateResource(RenderContext* renderContext)
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
				Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateRadialGradientBrush(gobj, Center, GradientOriginOffset, RadiusX, RadiusY);
				if (result.State == GraphicFactoryError::OK) return make_shared<FeatureObject>(GraphicObjectTypes::RadialGradientBrush, result.Value, GraphicFactory->featureLevel);
			}
		}
	}
	return NULL;
}

Point RadialGradientBrush::GetCenter() const 
{
	return GetValue<Point>(CenterProperty);
}

void RadialGradientBrush::SetCenter(const Point& start)
{
	SetValue(CenterProperty, start);
}

Point RadialGradientBrush::GetGradientOriginOffset() const
{
	return GetValue<Point>(GradientOriginOffsetProperty);
}

void RadialGradientBrush::SetGradientOriginOffset(const Point& end)
{
	SetValue(GradientOriginOffsetProperty, end);
}

double RadialGradientBrush::GetRadiusX() const
{
	return GetValue<double>(RadiusXProperty);
}

void RadialGradientBrush::SetRadiusX(double radiusX)
{
	SetValue(RadiusXProperty, radiusX);
}

double RadialGradientBrush::GetRadiusY() const
{
	return GetValue<float>(RadiusYProperty);
}

void RadialGradientBrush::SetRadiusY(double radiusY)
{
	SetValue(RadiusYProperty, radiusY);
}

GradientStopCollectionPtr RadialGradientBrush::GetGradientStops()const
{
	return GetValuePtr<GradientStopCollection>(GradientStopsProperty);
}

void RadialGradientBrush::SetGradientStops(const GradientStopCollectionPtr& gradientStops)
{
	SetValue(GradientStopsProperty, gradientStops);
}
