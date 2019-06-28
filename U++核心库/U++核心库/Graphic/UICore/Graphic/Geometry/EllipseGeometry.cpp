#include "EllipseGeometry.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr EllipseGeometry::RadiusXProperty = make_dependencyproperty<EllipseGeometry>();
const DependencyPropertyPtr EllipseGeometry::RadiusYProperty = make_dependencyproperty<EllipseGeometry>();
const DependencyPropertyPtr EllipseGeometry::CenterProperty = make_dependencyproperty<EllipseGeometry>();


__usctor(EllipseGeometry)
{
	EllipseGeometry::RadiusXProperty.get()->Register<EllipseGeometry>(L"RadiusX",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &RadiusXChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	EllipseGeometry::RadiusYProperty.get()->Register<EllipseGeometry>(L"RadiusY",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &RadiusYChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	EllipseGeometry::CenterProperty.get()->Register<EllipseGeometry>(L"Center",
		FrameworkPropertyMetadata::Create<Point>(Point(0.5, 0.5), FrameworkPropertyMetadataOptions::AffectsRender, &CenterChanged));

	return true;
}

bool EllipseGeometry::RadiusXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<EllipseGeometry*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	if (me->m_GraphicFactory)
	{
		FeatureObjectPtr obj = me->GetResource(me->m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (me->m_GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
					return me->m_GraphicFactory->Mgr8->Run.EllipseGeometry.SetRadiusX(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool EllipseGeometry::RadiusYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<EllipseGeometry*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
	if (me->m_GraphicFactory)
	{
		FeatureObjectPtr obj = me->GetResource(me->m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (me->m_GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
					return me->m_GraphicFactory->Mgr8->Run.EllipseGeometry.SetRadiusY(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool EllipseGeometry::CenterChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<EllipseGeometry*>(ptr);
	auto newValue = changedArgs.NewValue<Point>();
	if (me->m_GraphicFactory)
	{
		FeatureObjectPtr obj = me->GetResource(me->m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (me->m_GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
					return me->m_GraphicFactory->Mgr8->Run.EllipseGeometry.SetCenterPoint(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

EllipseGeometry::EllipseGeometry(const Point& center, double radiusX, double radiusY)
{
	SetValue(CenterProperty, center);
	SetValue(RadiusYProperty, radiusX);
	SetValue(RadiusYProperty, radiusY);
}

EllipseGeometry::~EllipseGeometry()
{
}

FeatureObjectPtr EllipseGeometry::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateEllipseGeometry(Center, RadiusX, RadiusY);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::EllipseGeometry, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

double EllipseGeometry::GetRadiusX() const
{
	return GetValue<double>(RadiusXProperty);
}

void EllipseGeometry::SetRadiusX(double radiusX)
{
	SetValue(RadiusXProperty, radiusX);
}

double EllipseGeometry::GetRadiusY() const
{
	return GetValue<double>(RadiusYProperty);
}

void EllipseGeometry::SetRadiusY(double radiusY)
{
	SetValue(RadiusYProperty, radiusY);
}

Point EllipseGeometry::GetCenter() const
{
	return GetValue<Point>(CenterProperty);
}

void EllipseGeometry::SetCenter(const Point& point)
{
	SetValue(CenterProperty, point);
}