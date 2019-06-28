#include "Resource.h"
#include "..\Application_impl.h"
using namespace System::UI;
using namespace System::UI::Graphic;

FeatureOwner::FeatureOwner() :GraphicFactory_(nullptr) {}

void FeatureOwner::Register(GraphicFactory* GraphicFactory)
{
	GraphicFactory_ = GraphicFactory;
}

bool FeatureOwner::HasOwner() { return GraphicFactory_ != NULL; }


FeatureObject::FeatureObject() throw() : ptr_(NULL), type_(GraphicObjectTypes::Invalid), level_(FeatureLevel::Invalid)
{
}

FeatureObject::FeatureObject(GraphicObjectTypes type, GraphicObject obj, FeatureLevel level) throw() : ptr_(obj), type_(type), level_(level)
{
}

FeatureObject::FeatureObject(const FeatureObject& other) throw() : ptr_(other.ptr_), type_(other.type_), level_(other.level_)
{
}

void FeatureObject::InternalRelease() throw()
{
	if (ptr_ && type_ != GraphicObjectTypes::Invalid)
		if (!owner_.DeleteObject(type_, ptr_, level_))
			MessageBoxA(NULL, "memory leak", NULL, 0); // 核心对象回收失败
		else
		{
			type_ = GraphicObjectTypes::Invalid;
			ptr_ = 0;
		}
}

FeatureObject::~FeatureObject() throw()
{
	InternalRelease();
}
FeatureObject& FeatureObject::operator=(decltype(__nullptr)) throw()
{
	InternalRelease();
	return *this;
}

FeatureObject& FeatureObject::operator=(const FeatureObject &other) throw()
{
	if (ptr_ != other.ptr_)
	{
		FeatureObject(other).Swap(*this);
	}
	return *this;
}

void FeatureObject::Swap(_Inout_ FeatureObject& r)
{
	GraphicObject tmp = ptr_;
	ptr_ = r.ptr_;
	r.ptr_ = tmp;
	GraphicObjectTypes tmp1 = type_;
	type_ = r.type_;
	r.type_ = tmp1;
	FeatureLevel tmp2 = level_;
	level_ = r.level_;
	r.level_ = tmp2;
}
GraphicObject FeatureObject::Get() const throw()
{
	return ptr_;
}

GraphicObjectTypes FeatureObject::GetType()const throw()
{
	return type_;
}

FeatureLevel FeatureObject::GetLevel()const throw()
{
	return level_;
}

void FeatureObject::Reset()
{
	InternalRelease();
}



FeatureDependantResource::FeatureDependantResource() : m_renderContext(NULL), m_isDirty(true), m_lastResource(NULL)
{
}

bool FeatureDependantResource::IsResourceValid() const
{
	if (m_isDirty || !m_lastResource)
		return false;

	return true;
}

bool FeatureDependantResource::IsResourceValidFrom(const FeatureDependantResourcePtr& resource)
{
	if (resource == nullptr)
		return false;

	return resource->IsResourceValid();
}

bool FeatureDependantResource::IsResourceValidFrom(FeatureDependantResource* const resource)
{
	return resource->IsResourceValid();
}

void FeatureDependantResource::InvalidateResource()
{
	m_isDirty = true;
	NotifyInvalidatables(InvalidateFlags::Visual);
}

void FeatureDependantResource::Invalidate(InvalidateFlags flags)
{
	m_isDirty = true;
	NotifyInvalidatables(flags);
}

FeatureObjectPtr FeatureDependantResource::GetResourceFrom(const FeatureDependantResourcePtr& featureDependantResource, RenderContext* renderContext)
{
	return featureDependantResource->GetResource(renderContext);
}

FeatureObjectPtr FeatureDependantResource::GetResourceFrom(const FeatureDependantResourcePtr& featureDependantResource)
{
	return featureDependantResource->GetResource(m_renderContext);
}

void FeatureDependantResource::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	InvalidateResource();
}

void FeatureDependantResource::SetRenderContext(RenderContext* renderContext)
{
	if (m_renderContext != renderContext)
	{
		m_renderContext = renderContext;
		InvalidateResource();
	}
}

GraphicFactory* FeatureDependantResource::GetGraphicFactoryFromApplication(System::UI::Application* app)
{
	return Application_impl::GetGraphicFactory(app);
}

GraphicFactory* FeatureIndependantResource::GetGraphicFactoryFromApplication(System::UI::Application* app)
{
	return Application_impl::GetGraphicFactory(app);
}

void FeatureDependantResource::ForceResetResource(FeatureObjectPtr resource, GraphicFactory* factory)
{
	m_isDirty = false;
	if (m_lastResource)
	{
		m_lastResource->Reset();
		m_lastResource = nullptr;
	}

	m_lastResource = resource;
	if (m_lastResource)
	{
		m_lastResource->owner_.Register(factory);
		m_isDirty = false;
		NotifyChanged(L"Resource", L"");
	}
}

void FeatureIndependantResource::ForceResetResource(FeatureObjectPtr resource, GraphicFactory* factory)
{
	m_isDirty = false;
	if (m_lastResource)
	{
		m_lastResource->Reset();
		m_lastResource = nullptr;
	}
	SetGraphicFactory(factory);
	m_lastResource = resource;
	if (m_lastResource)
	{
		m_lastResource->owner_.Register(factory);
		m_isDirty = false;
		NotifyChanged(L"Resource", L"");
	}
}


FeatureIndependantResource::FeatureIndependantResource() : m_GraphicFactory(nullptr), m_isDirty(true), m_lastResource(NULL), IObservableObject()
{
}

bool FeatureIndependantResource::IsResourceValid() const
{
	if (m_isDirty || !m_lastResource)
		return false;

	return true;
}

bool FeatureIndependantResource::IsResourceValidFrom(const FeatureIndependantResourcePtr& resource)
{
	if (resource == nullptr)
		return false;

	return resource->IsResourceValid();
}

bool FeatureIndependantResource::IsResourceValidFrom(FeatureIndependantResource* const resource) 
{
	return resource->IsResourceValid();
}

void FeatureIndependantResource::InvalidateResource()
{
	m_isDirty = true;
	NotifyInvalidatables(InvalidateFlags::Visual);
}

void FeatureIndependantResource::Invalidate(InvalidateFlags flags)
{
	m_isDirty = true;
	NotifyInvalidatables(flags);
}

FeatureObjectPtr FeatureIndependantResource::GetResource(GraphicFactory* GraphicFactory)
{
	SetGraphicFactory(GraphicFactory);

	if (!IsResourceValid())
	{
		m_lastResource = OnCreateResource(m_GraphicFactory);
		if (m_lastResource)
		{
			m_lastResource->owner_.Register(m_GraphicFactory);
			m_isDirty = false;
			NotifyChanged(L"Resource", L"");
		}
	}

	return m_lastResource;
}

FeatureObjectPtr FeatureIndependantResource::GetResourceFrom(const FeatureIndependantResourcePtr& featureIndependantResource, GraphicFactory* GraphicFactory)
{
	return featureIndependantResource->GetResource(GraphicFactory);
}

FeatureObjectPtr FeatureIndependantResource::GetResourceFrom(const FeatureIndependantResourcePtr& featureIndependantResource)
{
	return featureIndependantResource->GetResource(m_GraphicFactory);
}

void FeatureIndependantResource::Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args)
{
	InvalidateResource();
}

void FeatureIndependantResource::SetGraphicFactory(GraphicFactory* GraphicFactory)
{
	if (m_GraphicFactory != GraphicFactory)
	{
		m_GraphicFactory = GraphicFactory;
		InvalidateResource();
	}
}