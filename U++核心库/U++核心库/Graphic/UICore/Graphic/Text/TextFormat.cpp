#include "TextFormat.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;


const DependencyPropertyPtr TextFormat::FontFamilyProperty = make_dependencyproperty<TextFormat>();
const DependencyPropertyPtr TextFormat::FontSizeProperty = make_dependencyproperty<TextFormat>();
const DependencyPropertyPtr TextFormat::FontWeightProperty = make_dependencyproperty<TextFormat>();
const DependencyPropertyPtr TextFormat::FontStyleProperty = make_dependencyproperty<TextFormat>();
const DependencyPropertyPtr TextFormat::FontStretchProperty = make_dependencyproperty<TextFormat>();

__usctor(TextFormat)
{
	TextFormat::FontFamilyProperty.get()->Register<TextFormat>(L"FontFamily",
		FrameworkPropertyMetadata::Create<std::wstring>(std::wstring(L""), FrameworkPropertyMetadataOptions::AffectsRender, &FontFamilyChanged));
	TextFormat::FontSizeProperty.get()->Register<TextFormat>(L"FontSize",
		FrameworkPropertyMetadata::Create<double>(12.0, FrameworkPropertyMetadataOptions::AffectsRender, &FontSizeChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 1.0, 1.0, 1.0)));
	TextFormat::FontWeightProperty.get()->Register<TextFormat>(L"FontWeight",
		FrameworkPropertyMetadata::Create<Graphic::FontWeight>((Graphic::FontWeight)FontWeight::Normal, FrameworkPropertyMetadataOptions::AffectsRender, &FontWeightChanged));
	TextFormat::FontStyleProperty.get()->Register<TextFormat>(L"FontStyle",
		FrameworkPropertyMetadata::Create<Graphic::FontStyle>((Graphic::FontStyle)FontStyle::Normal, FrameworkPropertyMetadataOptions::AffectsRender, &FontStyleChanged));
	TextFormat::FontStretchProperty.get()->Register<TextFormat>(L"FontStretch",
		FrameworkPropertyMetadata::Create<Graphic::FontStretch>((Graphic::FontStretch)FontStretch::Normal, FrameworkPropertyMetadataOptions::AffectsRender, &FontStretchChanged));
	return true;
}

bool TextFormat::FontFamilyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextFormat*>(ptr);
	auto newValue = changedArgs.NewValue<const std::wstring>();
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
					return me->m_GraphicFactory->Mgr8->Run.TextFormat.SetFontFamily(gobj, newValue.c_str()) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool TextFormat::FontSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextFormat*>(ptr);
	auto newValue = changedArgs.NewValue<double>();
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
					return me->m_GraphicFactory->Mgr8->Run.TextFormat.SetFontSize(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool TextFormat::FontWeightChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextFormat*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FontWeight>();
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
					return me->m_GraphicFactory->Mgr8->Run.TextFormat.SetFontWeight(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool TextFormat::FontStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextFormat*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FontStyle>();
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
					return me->m_GraphicFactory->Mgr8->Run.TextFormat.SetFontStyle(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool TextFormat::FontStretchChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextFormat*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FontStretch>();
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
					return me->m_GraphicFactory->Mgr8->Run.TextFormat.SetFontStretch(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

TextFormat::TextFormat(const std::wstring& fontFamily, double fontSize, Graphic::FontWeight fontWeight, Graphic::FontStyle fontStyle, Graphic::FontStretch fontStretch)
{
	SetValue(FontFamilyProperty, fontFamily);
	SetValue(FontSizeProperty, fontSize);
	SetValue(FontWeightProperty, fontWeight);
	SetValue(FontStyleProperty, fontStyle);
	SetValue(FontStretchProperty, fontStretch);
}

TextFormat::~TextFormat()
{
}

FeatureObjectPtr TextFormat::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;
	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateTextFormat(FontFamily.c_str(), FontSize, this->FontWeight, this->FontStyle, this->FontStretch);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::TextFormat, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

std::wstring TextFormat::GetFontFamily() const 
{
	return GetValue<std::wstring>(FontFamilyProperty);
}

void TextFormat::SetFontFamily(const std::wstring& fontFamliy)
{
	SetValue(FontFamilyProperty, fontFamliy);
}

double TextFormat::GetFontSize() const 
{
	return GetValue<double>(FontSizeProperty);
}

void TextFormat::SetFontSize(double fontSize)
{
	SetValue(FontSizeProperty, fontSize);
}

Graphic::FontWeight TextFormat::GetFontWeight() const
{
	return GetValue<Graphic::FontWeight>(FontWeightProperty);
}

void TextFormat::SetFontWeight(Graphic::FontWeight val)
{
	SetValue(FontWeightProperty, val);
}

Graphic::FontStyle TextFormat::GetFontStyle() const 
{
	return GetValue<Graphic::FontStyle>(FontStyleProperty);
}

void TextFormat::SetFontStyle(Graphic::FontStyle val)
{
	SetValue(FontStyleProperty, val);
}

Graphic::FontStretch TextFormat::GetFontStretch() const 
{
	return GetValue<Graphic::FontStretch>(FontStretchProperty);
}

void TextFormat::SetFontStretch(Graphic::FontStretch val)
{
	SetValue(FontStretchProperty, val);
}
