#include "Surface.h"
#include "Texture2D.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\GraphicFactory.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr Surface::SizeProperty = make_dependencyproperty<Surface>();
const DependencyPropertyPtr Surface::Texture2DProperty = make_dependencyproperty<Surface>();

__usctor(Surface)
{
	Surface::SizeProperty.get()->Register<Surface>(L"Size", FrameworkPropertyMetadata::Create<Size>(Size(), FrameworkPropertyMetadataOptions::None));
	Surface::Texture2DProperty.get()->Register<Surface>(L"Texture2D", FrameworkPropertyMetadata::Create<Texture2DPtr>(nullptr, FrameworkPropertyMetadataOptions::None));
	return true;
}


Surface::Surface()
{

}

Surface::Surface(const Texture2DPtr& texture2D)
{
	InternalSetValue(Texture2DProperty, texture2D);
}

Size Surface::GetPixelSize() const
{
	return GetValue<Size>(SizeProperty);
}

Texture2DPtr Surface::GetTexture2D() const
{
	return GetValuePtr<Graphic::Texture2D>(Texture2DProperty);
}

auto Surface::OnCreateResource(RenderContext* renderContext)->FeatureObjectPtr
{
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;
	Texture2DPtr texptr = GetTexture2D();
	if (texptr)
	{
		FeatureObjectPtr obj = texptr->GetResource(renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{				
						Result<GraphicObject, GraphicError> result = GraphicFactory->Mgr8->Run.Texture2D.GetSurface(gobj);
						if (result.State == GraphicError::OK)
						{
							return make_shared<FeatureObject>(GraphicObjectTypes::Surface, result.Value, GraphicFactory->featureLevel);
						}
					}
					break;
				}
			}
		}
	}
	return NULL;
}

bool Surface::CopyTo(const SurfacePtr& surfaceCPU)
{
	if (!surfaceCPU) return false;
	if (m_renderContext)
	{
		GraphicFactory* GraphicFactory = m_renderContext->GetGraphicFactory();
		if (!GraphicFactory) return false;
		Texture2DPtr texptr = GetTexture2D();
		if (texptr)
		{
			FeatureObjectPtr obj = texptr->GetResource(m_renderContext);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					auto tx2 = surfaceCPU->Texture2D;
					if (tx2)
					{
						if (tx2->Mode != Texture2DMode::CpuRead) return false;
						FeatureObjectPtr obj1 = tx2->GetResource(m_renderContext);
						if (obj1)
						{
							GraphicObject gobj1 = obj1->Get();
							if (gobj1)
							{
								switch (GraphicFactory->featureLevel)
								{
									case FeatureLevel::D2D1WithD3D11:
									{
										if (GraphicFactory->Dev8)
										{
											Result<GraphicError> result = GraphicFactory->Mgr8->Run.Surface.CopyTo(gobj, gobj1, GraphicFactory->Dev8->Get());
											return result.Value == GraphicError::OK;
										}
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}