#include "Bitmap.h"
#include "..\GraphicFactory.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr Bitmap::SizeProperty = make_dependencyproperty<Bitmap>();

__usctor(Bitmap)
{
	Bitmap::SizeProperty.get()->Register<Bitmap>(L"Size",
		FrameworkPropertyMetadata::Create<Size>(Size(), FrameworkPropertyMetadataOptions::None));
	return true;
}


Bitmap::Bitmap()
{

}

Size Bitmap::GetPixelSize() const
{
	return GetValue<Size>(SizeProperty);
}
