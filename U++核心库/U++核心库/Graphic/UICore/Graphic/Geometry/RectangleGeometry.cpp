#include "RectangleGeometry.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr RectangleGeometry::RectProperty = make_dependencyproperty<RectangleGeometry>();

__usctor(RectangleGeometry)
{
	RectangleGeometry::RectProperty.get()->Register<RectangleGeometry>(L"Rect",
		FrameworkPropertyMetadata::Create<System::Rect>(System::Rect::Empty(), FrameworkPropertyMetadataOptions::AffectsRender, &RectChanged));
	return true;
}

bool RectangleGeometry::RectChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<RectangleGeometry*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.RectangleGeometry.SetRect(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

RectangleGeometry::RectangleGeometry(const System::Rect& rect)
{
	SetValue(RectProperty, rect);
}

RectangleGeometry::~RectangleGeometry()
{
}

FeatureObjectPtr RectangleGeometry::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateRectangleGeometry(this->Rect);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::RectangleGeometry, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

System::Rect RectangleGeometry::GetRect() const 
{
	return GetValue<System::Rect>(RectProperty);
}

void RectangleGeometry::SetRect(const System::Rect& rect)
{
	SetValue(RectProperty, rect);
}