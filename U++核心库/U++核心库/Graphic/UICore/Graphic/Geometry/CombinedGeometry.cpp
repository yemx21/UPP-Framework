#include "CombinedGeometry.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr CombinedGeometry::ModeProperty = make_dependencyproperty<CombinedGeometry>();
const DependencyPropertyPtr CombinedGeometry::Geometry1Property = make_dependencyproperty<CombinedGeometry>();
const DependencyPropertyPtr CombinedGeometry::Geometry2Property = make_dependencyproperty<CombinedGeometry>();

__usctor(CombinedGeometry)
{
	CombinedGeometry::ModeProperty.get()->Register<CombinedGeometry>(L"Mode",
		FrameworkPropertyMetadata::Create<CombineMode>((CombineMode)CombineMode::Union, FrameworkPropertyMetadataOptions::AffectsRender, &ModeChanged));

	CombinedGeometry::Geometry1Property.get()->Register<CombinedGeometry>(L"Geometry1",
		FrameworkPropertyMetadata::Create<GeometryPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::AffectsUpdate));

	CombinedGeometry::Geometry2Property.get()->Register<CombinedGeometry>(L"Geometry2",
		FrameworkPropertyMetadata::Create<GeometryPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::AffectsUpdate));
	return true;
}

bool CombinedGeometry::ModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<CombinedGeometry*>(ptr);
	auto newValue = changedArgs.NewValue<CombineMode>();
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
					return me->m_GraphicFactory->Mgr8->Run.CombinedGeometry.SetCombineMode(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

CombinedGeometry::CombinedGeometry(const GeometryPtr& g1, const GeometryPtr& g2, CombineMode mode)
{
	SetValue(ModeProperty, mode);
	
}

CombinedGeometry::~CombinedGeometry()
{
}

FeatureObjectPtr CombinedGeometry::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;

	GeometryPtr geo1 = Geometry1;
	GeometryPtr geo2 = Geometry2;

	FeatureObjectPtr obj1 = NULL;
	FeatureObjectPtr obj2= NULL;
	if (geo1) obj1 = geo1->GetResource(GraphicFactory);
	if (geo2) obj2 = geo2->GetResource(GraphicFactory);
	GraphicObject gobj1 = NULL;
	GraphicObject gobj2 = NULL;
	if (obj1) gobj1 = obj1->Get();
	if (obj2) gobj2 = obj2->Get();
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateCombinedGeometry(gobj1, gobj2, Mode);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::CombinedGeometry, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

CombineMode CombinedGeometry::GetMode() const
{
	return GetValue<CombineMode>(ModeProperty);
}

void CombinedGeometry::SetMode(CombineMode mode)
{
	SetValue(ModeProperty, mode);
}

GeometryPtr CombinedGeometry::GetGeometry1() const
{
	return GetValuePtr<Geometry>(Geometry1Property);
}

void CombinedGeometry::SetGeometry1(const GeometryPtr& geometry1)
{
	SetValue(Geometry1Property, geometry1);
}

GeometryPtr CombinedGeometry::GetGeometry2() const
{
	return GetValuePtr<Geometry>(Geometry2Property);
}

void CombinedGeometry::SetGeometry2(const GeometryPtr& geometry2)
{
	SetValue(Geometry2Property, geometry2);
}