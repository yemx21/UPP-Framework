#include "Application.h"
#include "Application_impl.h"
#include "Graphic\GraphicFactory.h"
#include "Windows\Window.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Graphic;


Application_impl::Application_impl(HINSTANCE hInstance) : factory(nullptr), instance(hInstance)
{
}

Result<Application_impl*> Application_impl::Create(HINSTANCE hInstance, FeatureLevel level)
{
	Result<Application_impl*> result(nullptr);
	auto res = GraphicFactory::CreateGraphicFactory(level);
	if (res.State == GraphicFactoryError::OK)
	{
		Application_impl* impl = new Application_impl(hInstance);
		impl->factory = res.Value;
		result.Value = impl;
	}
	else
		result.WriteMessage(res.Message);
	return result;
}

GraphicFactory* Application_impl::GetGraphicFactory(Application* app)
{
	return app->impl->factory;
}

bool DistributedApplication_impl::Register(Application* app, DistributedApplication* disapp)
{
	if (app) return app->Register(disapp);
	return false;
}

void DistributedApplication_impl::Unregister(Application* app, DistributedApplication* disapp)
{
	if (app) app->Unregister(disapp);
}


Result<Application*> Application::Create(HINSTANCE hInstance, FeatureLevel level, const CultureInfo& culture)
{
	Result<Application*> result(nullptr);
	auto res = Application_impl::Create(hInstance, level);
	if (res.Value)
	{
		Application* app = new Application(culture);
		app->impl = res.Value;
		app->Identifier = (intptr_t)hInstance;
		result.Value = app;
	}
	else
		result.WriteMessage(res.Message);
	return result;
}

Application::Application(const CultureInfo& culture) :IApplication(0ull, culture)
{

}

Application::~Application()
{

}

HINSTANCE Application::GetNativeNameSpaceHandle() const
{
	return impl->instance;
}

void Application::OnCultureChanged(const CultureInfo& oldinfo, const CultureInfo& newinfo)
{
	std::lock_guard<CriticalSection> locker(cs);
	for (DistributedApplication* disapp : disapps)
	{
		disapp->OnCultureChanged(oldinfo, newinfo);
	}
}

bool Application::OnAssembliesChanging(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	std::lock_guard<CriticalSection> locker(cs);
	bool ret = true;
	for (DistributedApplication* disapp : disapps)
	{
		ret &= disapp->OnAssembliesChanging(notify, name, ver, publicKey);
	}
	return ret;
}

void Application::OnAssembliesChanged(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	std::lock_guard<CriticalSection> locker(cs);
	for (DistributedApplication* disapp : disapps)
	{
		disapp->OnAssembliesChanged(notify, name, ver, publicKey);
	}
}

bool Application::Register(DistributedApplication* disapp)
{
	std::lock_guard<CriticalSection> locker(cs);
	if (!disapp) return false;
	if (std::find(disapps.begin(), disapps.end(), disapp) != disapps.end()) return true;
	disapps.push_back(disapp);
	return true;
}

void Application::Unregister(DistributedApplication* disapp)
{
	std::lock_guard<CriticalSection> locker(cs);
	disapps.remove(disapp);
}

DistributedApplication::DistributedApplication(Application* app)
{
	_parent = app;
	if (!DistributedApplication_impl::Register(_parent, this)) throw std::exception("critical error: bad application");
}

DistributedApplication::~DistributedApplication()
{
	DistributedApplication_impl::Unregister(_parent, this);
}

void DistributedApplication::OnCultureChanged(const CultureInfo&, const CultureInfo&)
{

}

bool DistributedApplication::OnAssembliesChanging(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	return true;
}

void DistributedApplication::OnAssembliesChanged(AssembliesChangedNotification, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	
}