#include "SaturationEffect.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr SaturationEffect::AmountProperty = make_dependencyproperty<SaturationEffect>();

__usctor(SaturationEffect)
{
	SaturationEffect::AmountProperty.get()->Register<SaturationEffect>(L"Amount",
		FrameworkPropertyMetadata::Create<double>(3.0, FrameworkPropertyMetadataOptions::AffectsRender, &AmountChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	return true;
}

bool SaturationEffect::AmountChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<SaturationEffect*>(ptr);
	auto newValue = changedArgs.NewValue<float>();
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
					return me->m_GraphicFactory->Mgr8->Run.SaturationEffect.SetAmount(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

SaturationEffect::SaturationEffect(float Amount)
{
	SetValue(AmountProperty, Amount);
}

SaturationEffect::~SaturationEffect()
{
}

FeatureObjectPtr SaturationEffect::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateSaturationEffect(Amount);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::TextFormat, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

double SaturationEffect::GetAmount() const
{
	return GetValue<double>(AmountProperty);
}

void SaturationEffect::SetAmount(double Amount)
{
	SetValue(AmountProperty, Amount);
}