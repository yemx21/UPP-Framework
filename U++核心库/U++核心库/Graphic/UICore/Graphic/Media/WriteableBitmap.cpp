#include "WriteableBitmap.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System::UI;
using namespace System::UI::Graphic;

WriteableBitmap::WriteableBitmap(const Size& pixelSize)
{
	SetValue(SizeProperty, pixelSize);
}

WriteableBitmap::~WriteableBitmap()
{

}

FeatureObjectPtr WriteableBitmap::OnCreateResource(RenderContext* renderContext)
{
	GraphicFactory* GraphicFactory = renderContext->GetGraphicFactory();
	if (!GraphicFactory) return NULL;

	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateBitmap(PixelSize);
	if (result.State == GraphicFactoryError::OK)
	{
		switch (GraphicFactory->featureLevel)
		{
		case FeatureLevel::D2D1WithD3D11:
		{
			SetValue(SizeProperty, (Size)GraphicFactory->Mgr8->Run.Bitmap.GetSize(result.Value).Value);
			return make_shared<FeatureObject>(GraphicObjectTypes::Bitmap, result.Value, GraphicFactory->featureLevel);
		}
			break;
		}
	}
	
	return NULL;
}
