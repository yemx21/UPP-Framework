#include "GeometryGroup.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr GeometryGroup::FillModeProperty = make_dependencyproperty<GeometryGroup>();

__usctor(GeometryGroup)
{
	GeometryGroup::FillModeProperty.get()->Register<GeometryGroup>(L"FillMode",
		FrameworkPropertyMetadata::Create<Graphic::FillMode>((Graphic::FillMode)Graphic::FillMode::Alternate, FrameworkPropertyMetadataOptions::AffectsRender, &FillModeChanged));
	
	return true;
}


bool GeometryGroup::FillModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<GeometryGroup*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FillMode>();
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
					return me->m_GraphicFactory->Mgr8->Run.GeometryGroup.SetFillMode(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

void GeometryGroup::OnGeometriesChanged(GeometryGroup* sender, std::shared_ptr<ObservableCollection<GeometryPtr>> geometries, NotifyCollectionChangedEventArgs<GeometryPtr> args)
{
	if (sender)
	{
		GraphicFactory* GraphicFactory = sender->m_GraphicFactory;
		if (GraphicFactory)
		{			
			FeatureObjectPtr obj = sender->GetResource(GraphicFactory);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					switch (GraphicFactory->featureLevel)
					{
						case FeatureLevel::D2D1WithD3D11:
						{
							const GeometryPtr* geos = geometries->address;
							unsigned int count = geometries->size;
							vector<GraphicObject> objs;
							if (count && geos)
							{
								for (unsigned int i = 0; i < count; i++)
								{
									const GeometryPtr geo = geometries->operator[](i);
									FeatureObjectPtr fgobj = geo->GetResource(GraphicFactory);
									if (fgobj)
									{
										GraphicObject tgobj = fgobj->Get();
										if (tgobj) objs.push_back(tgobj);
									}
								}
							}
							GraphicError result = GraphicFactory->Mgr8->Run.GeometryGroup.SetGeometries(gobj,&objs[0], objs.size());
							if (result == GraphicError::OK)
							{
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

GeometryGroup::GeometryGroup(Graphic::FillMode mode)
{
	SetValue(FillModeProperty, mode);
	geometriesChangedToken = Geometries.CollectionChanged += (bind(&GeometryGroup::OnGeometriesChanged, this, std::placeholders::_1, std::placeholders::_2));
}

GeometryGroup::~GeometryGroup()
{
	Geometries.CollectionChanged -= (geometriesChangedToken);
}

FeatureObjectPtr GeometryGroup::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;

	const GeometryPtr* geos = Geometries.address;
	unsigned int count = Geometries.size;
	vector<GraphicObject> objs;
	if (count && geos)
	{
		for (unsigned int i = 0; i < count; i++)
		{
			const GeometryPtr geo = Geometries[i];
			FeatureObjectPtr fgobj = geo->GetResource(GraphicFactory);
			if (fgobj)
			{
				GraphicObject tgobj = fgobj->Get();
				if (tgobj) objs.push_back(tgobj);
			}
		}
	}

	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreateGeometryGroup(&objs[0], objs.size(), FillMode);
	if (result.State == GraphicFactoryError::OK)
	{
		return make_shared<FeatureObject>(GraphicObjectTypes::GeometryGroup, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

Graphic::FillMode GeometryGroup::GetFillMode() const
{
	return GetValue<Graphic::FillMode>(FillModeProperty);
}

void GeometryGroup::SetFillMode(Graphic::FillMode mode)
{
	SetValue(FillModeProperty, mode);
}

ObservableCollection<GeometryPtr>& GeometryGroup::GetItems()
{
	return Geometries;
}

