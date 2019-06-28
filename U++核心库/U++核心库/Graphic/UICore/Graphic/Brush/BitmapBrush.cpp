#include "BitmapBrush.h"
#include "..\Media\BitmapSource.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr BitmapBrush::ExtendModeXProperty = make_dependencyproperty<BitmapBrush>();
const DependencyPropertyPtr BitmapBrush::ExtendModeYProperty = make_dependencyproperty<BitmapBrush>();
const DependencyPropertyPtr BitmapBrush::SourceRectProperty = make_dependencyproperty<BitmapBrush>();
const DependencyPropertyPtr BitmapBrush::ImageSourceProperty = make_dependencyproperty<BitmapBrush>();
const DependencyPropertyPtr BitmapBrush::InterpolationProperty = make_dependencyproperty<BitmapBrush>();

__usctor(BitmapBrush)
{
	BitmapBrush::ExtendModeXProperty.get()->Register<BitmapBrush>(L"ExtendModeX",
		FrameworkPropertyMetadata::Create<ExtendMode>((ExtendMode)ExtendMode::Clamp, FrameworkPropertyMetadataOptions::AffectsRender, &ExtendModeXChanged));

	BitmapBrush::ExtendModeYProperty.get()->Register<BitmapBrush>(L"ExtendModeY",
		FrameworkPropertyMetadata::Create<ExtendMode>((ExtendMode)ExtendMode::Clamp, FrameworkPropertyMetadataOptions::AffectsRender, &ExtendModeYChanged));

	BitmapBrush::SourceRectProperty.get()->Register<BitmapBrush>(L"SourceRect",
		FrameworkPropertyMetadata::Create<Rect>(Rect::Empty(), FrameworkPropertyMetadataOptions::AffectsRender, &SourceRectChanged));

	BitmapBrush::ImageSourceProperty.get()->Register<BitmapBrush>(L"ImageSource",
		FrameworkPropertyMetadata::Create<BitmapSourcePtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender, &ImageSourceChanged));

	BitmapBrush::InterpolationProperty.get()->Register<BitmapBrush>(L"Interpolation",
		FrameworkPropertyMetadata::Create<InterpolationMode>((InterpolationMode)InterpolationMode::Linear, FrameworkPropertyMetadataOptions::AffectsRender, &InterpolationChanged));

	return true;
}


BitmapBrush::BitmapBrush()
{

}

BitmapBrush::~BitmapBrush()
{

}

bool BitmapBrush::ExtendModeXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BitmapBrush*>(ptr);
	auto newValue = changedArgs.NewValue<ExtendMode>();
	auto oldValue = changedArgs.OldValue<ExtendMode>();

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
						return GraphicFactory->Mgr8->Run.BitmapBrush.SetExtendModeX(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}



bool BitmapBrush::ExtendModeYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BitmapBrush*>(ptr);
	auto newValue = changedArgs.NewValue<ExtendMode>();
	auto oldValue = changedArgs.OldValue<ExtendMode>();

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
						return GraphicFactory->Mgr8->Run.BitmapBrush.SetExtendModeY(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}


bool BitmapBrush::SourceRectChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BitmapBrush*>(ptr);
	auto newValue = changedArgs.NewValue<Rect>();
	auto oldValue = changedArgs.OldValue<Rect>();

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
						return GraphicFactory->Mgr8->Run.BitmapBrush.SetSourceRect(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool BitmapBrush::ImageSourceChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BitmapBrush*>(ptr);
	auto newValue = changedArgs.NewValue<BitmapSourcePtr>();
	auto oldValue = changedArgs.OldValue<BitmapSourcePtr>();

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
						FeatureObjectPtr bobj = newValue->GetResource(me->m_renderContext);
						GraphicObject gbobj = bobj->Get();
						if (gbobj)
							return GraphicFactory->Mgr8->Run.BitmapBrush.SetBitmap(gobj, gbobj) == GraphicError::OK;
						else
							return false;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool BitmapBrush::InterpolationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<BitmapBrush*>(ptr);
	auto newValue = changedArgs.NewValue<InterpolationMode>();
	auto oldValue = changedArgs.OldValue<InterpolationMode>();

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
						return GraphicFactory->Mgr8->Run.BitmapBrush.SetInterpolationMode(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

FeatureObjectPtr BitmapBrush::OnCreateResource(RenderContext* renderContext)
{
	if (!renderContext) return NULL;
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;
	BitmapSourcePtr bbp = ImageSource;
	if (bbp)
	{
		FeatureObjectPtr obj = bbp->GetResource(renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateBitmapBrush(gobj, ExtendModeX, ExtendModeY, SourceRect, Interpolation);
				if (result.State == GraphicFactoryError::OK)
				{
					return make_shared<FeatureObject>(GraphicObjectTypes::BitmapBrush, result.Value, GraphicFactory->featureLevel);
				}
			}
		}
	}
	return NULL;
}

ExtendMode BitmapBrush::GetExtendModeX() const
{
	return GetValue<ExtendMode>(ExtendModeXProperty);
}

void BitmapBrush::SetExtendModeX(ExtendMode extendMode)
{
	SetValue(ExtendModeXProperty, extendMode);
}

ExtendMode BitmapBrush::GetExtendModeY() const
{
	return GetValue<ExtendMode>(ExtendModeYProperty);
}

void BitmapBrush::SetExtendModeY(ExtendMode extendMode)
{
	SetValue(ExtendModeYProperty, extendMode);
}

Rect BitmapBrush::GetSourceRect() const
{
	return GetValue<Rect>(SourceRectProperty);
}

void BitmapBrush::SetSourceRect(const Rect& rect)
{
	SetValue(SourceRectProperty, rect);
}

BitmapSourcePtr BitmapBrush::GetImageSource() const
{
	return GetValuePtr<BitmapSource>(ImageSourceProperty);
}

void BitmapBrush::SetImageSource(const BitmapSourcePtr& image)
{
	SetValue(ImageSourceProperty, image);
}

InterpolationMode BitmapBrush::GetInterpolation() const
{
	return GetValue<InterpolationMode>(InterpolationProperty);
}

void BitmapBrush::SetInterpolation(InterpolationMode mode)
{
	SetValue(InterpolationProperty, mode);
}
