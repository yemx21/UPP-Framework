#include "Texture2D.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr Texture2D::SizeProperty = make_dependencyproperty<Texture2D>();
const DependencyPropertyPtr Texture2D::ModeProperty = make_dependencyproperty<Texture2D>();

__usctor(Texture2D)
{
	Texture2D::SizeProperty.get()->Register<Texture2D>(L"Size", FrameworkPropertyMetadata::Create<Size>(Size(), FrameworkPropertyMetadataOptions::None));
	Texture2D::ModeProperty.get()->Register<Texture2D>(L"Mode", FrameworkPropertyMetadata::Create<Texture2DMode>((UInt)Texture2DMode::Target, FrameworkPropertyMetadataOptions::None));
	return true;
}

Texture2D::Texture2D()
{

}

Texture2D::Texture2D(double width, double height, Texture2DMode mode)
{
	InternalSetValue(SizeProperty, Size(width, height));
	InternalSetValue(ModeProperty, mode);
}

Texture2D::Texture2D(const Size& size, Texture2DMode mode)
{
	InternalSetValue(SizeProperty, size);
	InternalSetValue(ModeProperty, mode);
}

Size Texture2D::GetPixelSize() const
{
	return GetValue<Size>(SizeProperty);
}

Texture2DMode Texture2D::GetMode() const
{
	return GetValue<Texture2DMode>(ModeProperty);
}

auto Texture2D::OnCreateResource(RenderContext* renderContext)->FeatureObjectPtr
{
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;
	if (!PixelSize.IsOneDimensionalOrEmpty)
	{
		Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateTexture2D(PixelSize, Mode);
		if (result.State == GraphicFactoryError::OK)
		{
			switch (GraphicFactory->featureLevel)
			{
				case FeatureLevel::D2D1WithD3D11:
				{	
					return make_shared<FeatureObject>(GraphicObjectTypes::Texture2D, result.Value, GraphicFactory->featureLevel);
				}
				break;
			}
		}
	}
	return NULL;
}

bool Texture2D::Map(BitmapData* data)
{
	if (!data) return false;
	if (m_renderContext)
	{
		GraphicFactory* GraphicFactory = m_renderContext->GetGraphicFactory();
		if (!GraphicFactory) return false;
		FeatureObjectPtr obj = GetResource(m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						if (GraphicFactory->Dev8)
						{
							BitmapData8 da;
							Result<GraphicError> result = GraphicFactory->Mgr8->Run.Texture2D.Map(gobj, GraphicFactory->Dev8->Get(), &da);
							if (result.Value == GraphicError::OK)
							{
								data->Bits = da.Bits;
								data->DepthPitch = da.DepthPitch;
								data->RowPitch = da.RowPitch;
								return true;
							}
						}
					}
					break;
				}
			}
		}
	}
	return false;
}

void Texture2D::Unmap()
{
	if (m_renderContext)
	{
		GraphicFactory* GraphicFactory = m_renderContext->GetGraphicFactory();
		if (!GraphicFactory) return;
		FeatureObjectPtr obj = GetResource(m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						if (GraphicFactory->Dev8)
						{
							GraphicFactory->Mgr8->Run.Texture2D.Unmap(gobj, GraphicFactory->Dev8->Get());
						}
					}
					break;
				}
			}
		}
	}
}