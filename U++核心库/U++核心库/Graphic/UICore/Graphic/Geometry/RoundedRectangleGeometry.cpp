#include "RoundedRectangleGeometry.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;
 

const DependencyPropertyPtr RoundedRectangleGeometry::RadiusXProperty = make_dependencyproperty<RoundedRectangleGeometry>();
const DependencyPropertyPtr RoundedRectangleGeometry::RadiusYProperty = make_dependencyproperty<RoundedRectangleGeometry>();
const DependencyPropertyPtr RoundedRectangleGeometry::RectProperty = make_dependencyproperty<RoundedRectangleGeometry>();

__usctor(RoundedRectangleGeometry)
{
	RoundedRectangleGeometry::RadiusXProperty.get()->Register<RoundedRectangleGeometry>(L"RadiusX",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &RadiusXChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	RoundedRectangleGeometry::RadiusYProperty.get()->Register<RoundedRectangleGeometry>(L"RadiusY",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &RadiusYChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	RoundedRectangleGeometry::RectProperty.get()->Register<RoundedRectangleGeometry>(L"Rect",
		FrameworkPropertyMetadata::Create<System::Rect>(System::Rect::Empty(), FrameworkPropertyMetadataOptions::AffectsRender, &RectChanged));
	return true;
}


bool RoundedRectangleGeometry::RadiusXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RoundedRectangleGeometry*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.RoundedRectangleGeometry.SetRadiusX(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool RoundedRectangleGeometry::RadiusYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RoundedRectangleGeometry*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.RoundedRectangleGeometry.SetRadiusY(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool RoundedRectangleGeometry::RectChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RoundedRectangleGeometry*>(ptr);
	auto newValue = changedArgs.NewValue<System::Rect>();
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
					return me->m_GraphicFactory->Mgr8->Run.RoundedRectangleGeometry.SetRect(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

RoundedRectangleGeometry::RoundedRectangleGeometry(const System::Rect& rect, double radiusX, double radiusY)
{
	InternalSetValue(RectProperty, rect);
	InternalSetValue(RadiusXProperty, radiusX);
	InternalSetValue(RadiusYProperty, radiusY);
}

RoundedRectangleGeometry::~RoundedRectangleGeometry()
{
}

FeatureObjectPtr RoundedRectangleGeometry::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateRoundedRectangleGeometry(this->Rect, RadiusX, RadiusY);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::RoundedRectangleGeometry, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

System::Rect RoundedRectangleGeometry::GetRect() const 
{
	return GetValue<System::Rect>(RectProperty);
}

void RoundedRectangleGeometry::SetRect(const System::Rect& rect)
{
	SetValue(RectProperty, rect);
}

double RoundedRectangleGeometry::GetRadiusX() const
{
	return GetValue<double>(RadiusXProperty);
}

void RoundedRectangleGeometry::SetRadiusX(double radiusX)
{
	SetValue(RadiusXProperty, radiusX);
}

double RoundedRectangleGeometry::GetRadiusY() const
{
	return GetValue<double>(RadiusYProperty);
}

void RoundedRectangleGeometry::SetRadiusY(double radiusY)
{
	SetValue(RadiusYProperty, radiusY);
}
