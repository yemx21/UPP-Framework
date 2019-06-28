#include "TextLayout.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr TextLayout::TextFormatProperty = make_dependencyproperty<TextLayout>();
const DependencyPropertyPtr TextLayout::MaxSizeProperty = make_dependencyproperty<TextLayout>();
const DependencyPropertyPtr TextLayout::TextProperty = make_dependencyproperty<TextLayout>();
const DependencyPropertyPtr TextLayout::LayoutBoundsProperty = make_dependencyproperty<TextLayout>();

__usctor(TextLayout)
{
	TextLayout::TextFormatProperty.get()->Register<TextLayout>(L"TextFormat",
		FrameworkPropertyMetadata::Create<TextFormatPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender, &TextFormatChanged));
	TextLayout::MaxSizeProperty.get()->Register<TextLayout>(L"MaxSize",
		FrameworkPropertyMetadata::Create<Size>(Size(), FrameworkPropertyMetadataOptions::AffectsRender, &MaxSizeChanged));
	TextLayout::TextProperty.get()->Register<TextLayout>(L"Text",
		FrameworkPropertyMetadata::Create<std::wstring>(std::wstring(L""), FrameworkPropertyMetadataOptions::AffectsRender, &TextChanged));
	TextLayout::LayoutBoundsProperty.get()->Register<TextLayout>(L"LayoutBounds",
		FrameworkPropertyMetadata::Create<Rect>(Rect(), FrameworkPropertyMetadataOptions::None));
	return true;
}


bool TextLayout::TextFormatChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextLayout*>(ptr);
	auto newValue = changedArgs.NewValue<TextFormatPtr>();
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
						GraphicObject gsobj = NULL;
						if (newValue)
						{
							FeatureObjectPtr gsfobj = newValue->GetResource(me->m_GraphicFactory);
							if (gsfobj) gsobj = gsfobj->Get();
							if (gsobj)
							{
								if (me->m_GraphicFactory->Mgr8->Run.TextLayout.SetTextFormat(gobj, gsobj) == GraphicError::OK)
								{
									Result<Rect, GraphicError> result_ls = me->m_GraphicFactory->Mgr8->Run.TextLayout.GetLayoutBounds(gobj);
									if (result_ls.State == GraphicError::OK)
									{
										me->InternalSetValue(LayoutBoundsProperty, result_ls.Value);
									}
									return true;
								}
								else
									return false;
							}
							else
							{
								return true;
							}
						}
						else
							return false;
					break;
				}
			}
		}
	}
	return true;
}

bool TextLayout::MaxSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextLayout*>(ptr);
	auto newValue = changedArgs.NewValue<Size>();
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
				{
					if (me->m_GraphicFactory->Mgr8->Run.TextLayout.SetMaxSize(gobj, newValue) == GraphicError::OK)
					{
						Result<Rect, GraphicError> result_ls = me->m_GraphicFactory->Mgr8->Run.TextLayout.GetLayoutBounds(gobj);
						if (result_ls.State == GraphicError::OK)
						{
							me->InternalSetValue(LayoutBoundsProperty, result_ls.Value);
						}
						return true;
					}
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

bool TextLayout::TextChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<TextLayout*>(ptr);
	auto newValue = changedArgs.NewValue<std::wstring>();
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
				{
					if (me->m_GraphicFactory->Mgr8->Run.TextLayout.SetText(gobj, newValue.c_str()) == GraphicError::OK)
					{
						Result<Rect, GraphicError> result_ls = me->m_GraphicFactory->Mgr8->Run.TextLayout.GetLayoutBounds(gobj);
						if (result_ls.State == GraphicError::OK)
						{
							me->InternalSetValue(LayoutBoundsProperty, result_ls.Value);
						}
						return true;
					}
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

TextLayout::TextLayout(const TextFormatPtr& format, const std::wstring& text) 
{
	InternalSetValue(TextFormatProperty, format);
	InternalSetValue(TextProperty, text);
}

TextLayout::~TextLayout()
{
}

FeatureObjectPtr TextLayout::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;

	TextFormatPtr tfp = TextFormat;
	if (tfp)
	{
		Graphic::TextFormat*  tf = tfp.get();
		if (tf)
		{
			FeatureObjectPtr obj = tf->GetResource(GraphicFactory);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateTextLayout(gobj, MaxSize, Text.c_str());
					if (result.State == GraphicFactoryError::OK)
					{
						FeatureObjectPtr FObj = make_shared<FeatureObject>(GraphicObjectTypes::TextLayout, result.Value, GraphicFactory->featureLevel);
						Result<Rect, GraphicError> result_ls = GraphicFactory->Mgr8->Run.TextLayout.GetLayoutBounds(result.Value);
						if (result_ls.State == GraphicError::OK)
						{
							InternalSetValue(LayoutBoundsProperty, result_ls.Value);
						}
						return FObj;
					}
				}
			}
		}
	}
	return NULL;
}

TextFormatPtr TextLayout::GetTextFormat() const 
{
	return GetValuePtr<Graphic::TextFormat>(TextFormatProperty);
}

void TextLayout::SetTextFormat(const TextFormatPtr& textFormat)
{
	SetValue(TextFormatProperty, textFormat);
}

Size TextLayout::GetMaxSize() const 
{
	return GetValue<Size>(MaxSizeProperty);
}

void TextLayout::SetMaxSize(const Size& maxSize)
{
	SetValue(MaxSizeProperty, maxSize);
}

void TextLayout::SetText(const std::wstring& text)
{
	SetValue(TextProperty, text);
}

std::wstring TextLayout::GetText() const 
{
	return GetValue<std::wstring>(TextProperty);
}

Rect TextLayout::GetLayoutBounds() const 
{
	return GetValue<Rect>(LayoutBoundsProperty);
}

bool TextLayout::HitTest(const Point& point)
{
	if (m_GraphicFactory)
	{
		FeatureObjectPtr obj = GetResource(m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (m_GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
					{
						Result<bool, GraphicError> result = m_GraphicFactory->Mgr8->Run.TextLayout.HitTest(gobj, point);
						if (result.State == GraphicError::OK)
							return result.Value;
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
