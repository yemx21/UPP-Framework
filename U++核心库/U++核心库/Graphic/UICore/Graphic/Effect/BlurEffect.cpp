#include "BlurEffect.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr BlurEffect::BlurSizeProperty = make_dependencyproperty<BlurEffect>();
const DependencyPropertyPtr BlurEffect::OptimizationProperty = make_dependencyproperty<BlurEffect>();
const DependencyPropertyPtr BlurEffect::BorderModeProperty = make_dependencyproperty<BlurEffect>();

__usctor(BlurEffect)
{
	BlurEffect::BlurSizeProperty.get()->Register<BlurEffect>(L"BlurSize",
		FrameworkPropertyMetadata::Create<double>(3.0, FrameworkPropertyMetadataOptions::AffectsRender, &BlurSizeChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	BlurEffect::OptimizationProperty.get()->Register<BlurEffect>(L"Optimization",
		FrameworkPropertyMetadata::Create<BlurOptimization>((BlurOptimization)BlurOptimization::Balanced, FrameworkPropertyMetadataOptions::AffectsRender, &OptimizationChanged));
	BlurEffect::BorderModeProperty.get()->Register<BlurEffect>(L"BorderMode",
		FrameworkPropertyMetadata::Create<BlurBorderMode>((BlurBorderMode)BlurBorderMode::Soft, FrameworkPropertyMetadataOptions::AffectsRender, &BorderModeChanged));
	return true;
}

BlurEffect::BlurEffect(double blurSize, BlurOptimization optimization, BlurBorderMode borderMode)
{
	InternalSetValue(BlurSizeProperty, blurSize);
	InternalSetValue(OptimizationProperty, optimization);
	InternalSetValue(BorderModeProperty, borderMode);
}

bool BlurEffect::BlurSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BlurEffect*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.BlurEffect.SetBlurSize(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool BlurEffect::OptimizationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BlurEffect*>(ptr);
	auto newValue = changedArgs.NewValue<BlurOptimization>();
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
					return me->m_GraphicFactory->Mgr8->Run.BlurEffect.SetOptimization(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool BlurEffect::BorderModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BlurEffect*>(ptr);
	auto newValue = changedArgs.NewValue<BlurBorderMode>();
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
					return me->m_GraphicFactory->Mgr8->Run.BlurEffect.SetBorderMode(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

BlurEffect::~BlurEffect()
{
}

FeatureObjectPtr BlurEffect::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateBlurEffect(BlurSize, Optimization, BorderMode);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::Effect, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

double BlurEffect::GetBlurSize() const
{
	return GetValue<float>(BlurSizeProperty);
}

void BlurEffect::SetBlurSize(double blurSize)
{
	SetValue(BlurSizeProperty, blurSize);
}

BlurOptimization BlurEffect::GetOptimization() const
{
	return GetValue<BlurOptimization>(OptimizationProperty);
}

void BlurEffect::SetOptimization(BlurOptimization optimization)
{
	SetValue(OptimizationProperty, optimization);
}

BlurBorderMode BlurEffect::GetBorderMode() const
{
	return GetValue<BlurBorderMode>(BorderModeProperty);
}

void BlurEffect::SetBorderMode(BlurBorderMode borderMode)
{
	SetValue(BorderModeProperty, borderMode);
}
