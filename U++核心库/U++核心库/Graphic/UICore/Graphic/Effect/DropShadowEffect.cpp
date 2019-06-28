#include "DropShadowEffect.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr DropShadowEffect::BlurSizeProperty = make_dependencyproperty<DropShadowEffect>();
const DependencyPropertyPtr DropShadowEffect::OptimizationProperty = make_dependencyproperty<DropShadowEffect>();
const DependencyPropertyPtr DropShadowEffect::ColorProperty = make_dependencyproperty<DropShadowEffect>();
const DependencyPropertyPtr DropShadowEffect::EnableTransformProperty = make_dependencyproperty<DropShadowEffect>();
const DependencyPropertyPtr DropShadowEffect::ShadowTransformProperty = make_dependencyproperty<DropShadowEffect>();
const DependencyPropertyPtr DropShadowEffect::DirectionProperty = make_dependencyproperty<DropShadowEffect>();
const DependencyPropertyPtr DropShadowEffect::ShadowDepthProperty = make_dependencyproperty<DropShadowEffect>();

__usctor(DropShadowEffect)
{
	DropShadowEffect::BlurSizeProperty.get()->Register<DropShadowEffect>(L"BlurSize",
		FrameworkPropertyMetadata::Create<double>(3.0, FrameworkPropertyMetadataOptions::AffectsRender, &BlurSizeChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	DropShadowEffect::OptimizationProperty.get()->Register<DropShadowEffect>(L"Optimization",
		FrameworkPropertyMetadata::Create<BlurOptimization>((BlurOptimization)BlurOptimization::Balanced, FrameworkPropertyMetadataOptions::AffectsRender, &OptimizationChanged));
	DropShadowEffect::ColorProperty.get()->Register<DropShadowEffect>(L"Color",
		FrameworkPropertyMetadata::Create<System::Color>(System::Color(), FrameworkPropertyMetadataOptions::AffectsRender, &ColorChanged));
	DropShadowEffect::EnableTransformProperty.get()->Register<DropShadowEffect>(L"EnableTransform",
		FrameworkPropertyMetadata::Create<bool>(false, FrameworkPropertyMetadataOptions::AffectsRender, &EnableTransformChanged));
	DropShadowEffect::ShadowTransformProperty.get()->Register<DropShadowEffect>(L"ShadowTransform",
		FrameworkPropertyMetadata::Create<TransformPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender, &ShadowTransformChanged));
	DropShadowEffect::DirectionProperty.get()->Register<DropShadowEffect>(L"Direction",
		FrameworkPropertyMetadata::Create<double>(315.0, FrameworkPropertyMetadataOptions::AffectsRender, &DirectionChanged, __upropertycoerce(__ucoerce_replace_none_none, double, 0.0)));
	DropShadowEffect::ShadowDepthProperty.get()->Register<DropShadowEffect>(L"ShadowDepth",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &ShadowDepthChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	return true;
}

bool DropShadowEffect::BlurSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetBlurSize(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool DropShadowEffect::OptimizationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetOptimization(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool DropShadowEffect::ColorChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
	auto newValue = changedArgs.NewValue<System::Color>();
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetColor(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool DropShadowEffect::EnableTransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
	auto newValue = changedArgs.NewValue<bool>();
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetEnableTransform(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool DropShadowEffect::ShadowTransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
	auto newValue = changedArgs.NewValue<TransformPtr>();
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetShadowTransform(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool DropShadowEffect::DirectionChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetDirection(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool DropShadowEffect::ShadowDepthChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<DropShadowEffect*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.DropShadowEffect.SetShadowDepth(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

DropShadowEffect::DropShadowEffect(double blurSize, BlurOptimization optimization, const System::Color& color, double direction, double shadowDepth)
{
	InternalSetValue(BlurSizeProperty, blurSize);
	InternalSetValue(OptimizationProperty, optimization);
	InternalSetValue(ColorProperty, color);
	InternalSetValue(EnableTransformProperty, false);
	InternalSetValue(DirectionProperty, direction);
	InternalSetValue(ShadowDepthProperty, shadowDepth);
}

DropShadowEffect::DropShadowEffect(double blurSize, BlurOptimization optimization, const System::Color& color, const TransformPtr& transform)
{
	InternalSetValue(BlurSizeProperty, blurSize);
	InternalSetValue(OptimizationProperty, optimization);
	InternalSetValue(ColorProperty, color);
	InternalSetValue(EnableTransformProperty, true);
	InternalSetValue(ShadowTransformProperty, transform);
}

DropShadowEffect::~DropShadowEffect()
{
}

FeatureObjectPtr DropShadowEffect::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateDropShadowEffect(BlurSize, Optimization, Color, ShadowTransform, EnableTransform, Direction, ShadowDepth);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::TextFormat, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

double DropShadowEffect::GetBlurSize()const
{
	return GetValue<double>(BlurSizeProperty);
}

void DropShadowEffect::SetBlurSize(double blurSize)
{
	SetValue(BlurSizeProperty, blurSize);
}

BlurOptimization DropShadowEffect::GetOptimization() const 
{
	return GetValue<BlurOptimization>(OptimizationProperty);
}

void DropShadowEffect::SetOptimization(BlurOptimization optimization)
{
	SetValue(OptimizationProperty, optimization);
}

System::Color DropShadowEffect::GetColor() const
{
	return GetValue<System::Color>(ColorProperty);
}

void DropShadowEffect::SetColor(const System::Color& color)
{
	SetValue(ColorProperty, color);
}

bool DropShadowEffect::GetEnableTransform() const
{
	return GetValue<bool>(EnableTransformProperty);
}

void DropShadowEffect::SetEnableTransform(bool enableTransform)
{
	SetValue(EnableTransformProperty, enableTransform);
}

void DropShadowEffect::SetShadowTransform(const TransformPtr& transform)
{
	SetValue(ShadowTransformProperty, transform);
}

TransformPtr DropShadowEffect::GetShadowTransform() const
{
	return GetValuePtr<Transform>(ShadowTransformProperty);
}

double DropShadowEffect::GetDirection() const
{
	return GetValue<double>(DirectionProperty);
}

void DropShadowEffect::SetDirection(double direction)
{
	SetValue(DirectionProperty, direction);
}

double DropShadowEffect::GetShadowDepth() const
{
	return GetValue<float>(ShadowDepthProperty);
}
void DropShadowEffect::SetShadowDepth(double shadowDepth)
{
	SetValue(ShadowDepthProperty, shadowDepth);
}
