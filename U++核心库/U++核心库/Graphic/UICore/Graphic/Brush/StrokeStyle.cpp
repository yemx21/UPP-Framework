#include "StrokeStyle.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr StrokeStyle::StartCapProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::EndCapProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::DashCapProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::LineJoinStyleProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::MiterLimitProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::LineDashStyleProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::DashOffsetProperty = make_dependencyproperty<StrokeStyle>();
const DependencyPropertyPtr StrokeStyle::DashesProperty = make_dependencyproperty<StrokeStyle>();


__usctor(StrokeStyle)
{
	StrokeStyle::StartCapProperty.get()->Register<StrokeStyle>(L"StartCap",
		FrameworkPropertyMetadata::Create<CapStyle>((CapStyle)CapStyle::Flat, FrameworkPropertyMetadataOptions::AffectsRender, &StartCapChanged));
	StrokeStyle::EndCapProperty.get()->Register<StrokeStyle>(L"EndCap",
		FrameworkPropertyMetadata::Create<CapStyle>((CapStyle)CapStyle::Flat, FrameworkPropertyMetadataOptions::AffectsRender, &EndCapChanged));
	StrokeStyle::DashCapProperty.get()->Register<StrokeStyle>(L"DashCap",
		FrameworkPropertyMetadata::Create<CapStyle>((CapStyle)CapStyle::Flat, FrameworkPropertyMetadataOptions::AffectsRender, &DashCapChanged));
	StrokeStyle::LineJoinStyleProperty.get()->Register<StrokeStyle>(L"LineJoinStyle",
		FrameworkPropertyMetadata::Create<LineJoin>((LineJoin)LineJoin::Miter, FrameworkPropertyMetadataOptions::AffectsRender, &LineJoinStyleChanged));
	StrokeStyle::MiterLimitProperty.get()->Register<StrokeStyle>(L"MiterLimit",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &MiterLimitChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	StrokeStyle::LineDashStyleProperty.get()->Register<StrokeStyle>(L"LineDashStyle",
		FrameworkPropertyMetadata::Create<DashStyle>((DashStyle)DashStyle::Solid, FrameworkPropertyMetadataOptions::AffectsRender, &LineDashStyleChanged));
	StrokeStyle::DashOffsetProperty.get()->Register<StrokeStyle>(L"DashOffset",
		FrameworkPropertyMetadata::Create<double>(0.0, FrameworkPropertyMetadataOptions::AffectsRender, &DashOffsetChanged, __upropertycoerce(__ucoerce_replace_replace_none, double, 0.0, 0.0, 0.0)));
	StrokeStyle::DashesProperty.get()->Register<StrokeStyle>(L"Dashes",
		FrameworkPropertyMetadata::Create<ObservableCollection<double>>(nullptr, FrameworkPropertyMetadataOptions::AffectsUpdate, &DashesChanged));
	return true;
}


bool StrokeStyle::StartCapChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
	auto newValue = changedArgs.NewValue<CapStyle>();	
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
						return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetStartCap(gobj, newValue) == GraphicError::OK;
						break;
				}
			}
		}
	}
	return true;
}

bool StrokeStyle::EndCapChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
	auto newValue = changedArgs.NewValue<CapStyle>();
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
					return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetEndCap(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool StrokeStyle::DashCapChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
	auto newValue = changedArgs.NewValue<CapStyle>();
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
					return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetDashCap(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool StrokeStyle::LineJoinStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
	auto newValue = changedArgs.NewValue<LineJoin>();
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
					return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetLineJoinStyle(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool StrokeStyle::MiterLimitChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetMiterLimit(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool StrokeStyle::LineDashStyleChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
	auto newValue = changedArgs.NewValue<DashStyle>();
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
					return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetLineDashStyle(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

bool StrokeStyle::DashOffsetChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
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
					return me->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetDashOffset(gobj, (float)newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

void StrokeStyle::OnDashesChanged(StrokeStyle* sender, std::shared_ptr<ObservableCollection<double>> dashes, NotifyCollectionChangedEventArgs<double> args)
{
	if (sender)
	{
		if (sender->m_GraphicFactory)
		{
			FeatureObjectPtr obj = sender->GetResource(sender->m_GraphicFactory);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					switch (sender->m_GraphicFactory->featureLevel)
					{
						case FeatureLevel::D2D1WithD3D11:
						{
							ObservableCollection<double>* tmp = dashes.get();

							sender->dashes_cache.resize(tmp->size);
							for (unsigned int i = 0; i < tmp->size;i++)
							{
								sender->dashes_cache[i] = (float)tmp->operator[](i);
							}

							if (tmp)
							{
								sender->m_GraphicFactory->Mgr8->Run.StrokeStyle.SetDashes(gobj, &sender->dashes_cache[0], tmp->size);
								sender->Invalidate(InvalidateFlags::Visual);
							}
						}
						break;
					}
				}
			}
		}
	}
}

bool StrokeStyle::DashesChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<StrokeStyle*>(ptr);
	ObservableCollection<double>& oldOC = changedArgs.OldValueRef<ObservableCollection<double>>();
	ObservableCollection<double>& newOC = changedArgs.NewValueRef<ObservableCollection<double>>();
	if (me)
	{
		if (&oldOC != nullptr) oldOC.CollectionChanged -= me->dashesChangedToken;
		if (&newOC != nullptr) me->dashesChangedToken = newOC.CollectionChanged += (bind(&StrokeStyle::OnDashesChanged, me, std::placeholders::_1, std::placeholders::_2));

		return true;
	}
	return false;
}

StrokeStyle::StrokeStyle()
{
	SetValue(DashesProperty, ObservableCollection<double>{});
}

StrokeStyle::~StrokeStyle()
{
	if (DashesProperty)
	{
		ObservableCollection<double>&  oldOC = GetValueRef<ObservableCollection<double>>(DashesProperty);
		if (&oldOC != nullptr) oldOC.CollectionChanged -= dashesChangedToken;
	}
}

FeatureObjectPtr StrokeStyle::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;

	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateStrokeStyle(StartCap, EndCap, DashCap, LineJoinStyle, MiterLimit, LineDashStyle, DashOffset, dashes_cache.size()>0 ? &dashes_cache[0] : nullptr, dashes_cache.size());
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::StrokeStyle, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

CapStyle StrokeStyle::GetStartCap() const
{
	return GetValue<CapStyle>(StartCapProperty);
}

void StrokeStyle::SetStartCap(CapStyle val)
{
	SetValue(StartCapProperty, val);
}

CapStyle StrokeStyle::GetEndCap() const
{
	return GetValue<CapStyle>(EndCapProperty);
}

void StrokeStyle::SetEndCap(CapStyle val)
{
	SetValue(EndCapProperty, val);
}

CapStyle StrokeStyle::GetDashCap() const
{
	return GetValue<CapStyle>(DashCapProperty);
}

void StrokeStyle::SetDashCap(CapStyle val)
{
	SetValue(DashCapProperty, val);
}

LineJoin StrokeStyle::GetLineJoinStyle() const 
{
	return GetValue<LineJoin>(LineJoinStyleProperty);
}

void StrokeStyle::SetLineJoinStyle(LineJoin val)
{
	SetValue(LineJoinStyleProperty, val);
}

double StrokeStyle::GetMiterLimit() const
{
	return GetValue<double>(MiterLimitProperty);
}

void StrokeStyle::SetMiterLimit(double val)
{
	return SetValue(MiterLimitProperty, val);
}

DashStyle StrokeStyle::GetLineDashStyle() const 
{
	return GetValue<DashStyle>(LineDashStyleProperty);
}

void StrokeStyle::SetLineDashStyle(DashStyle val)
{
	return SetValue(LineDashStyleProperty, val);
}

double StrokeStyle::GetDashOffset() const
{
	return GetValue<double>(DashOffsetProperty);
}

void StrokeStyle::SetDashOffset(double val)
{
	SetValue(DashOffsetProperty, val);
}

ObservableCollection<double>& StrokeStyle::GetDashes()
{
	return GetValueRef<ObservableCollection<double>>(DashesProperty);
}

void StrokeStyle::SetDashes(const ObservableCollection<double>& ds)
{
	SetValue(DashesProperty, ds);
}