#include "UamlServiceManager.h"
#include "UamlReader.h"
using namespace System::Uaml;

void UamlServiceManager::OnCultureChanged(const CultureInfo& oldinfo, const CultureInfo& newinfo)
{

}

bool UamlServiceManager::OnAssembliesChanging(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{
	return true;
}

void UamlServiceManager::OnAssembliesChanged(AssembliesChangedNotification notify, const wchar_t* name, const System::Version& ver, const UInt128& publicKey)
{

}

UamlServiceManager::UamlServiceManager(System::UI::Application* app): DistributedApplication(app)
{

}

UamlServiceManager::~UamlServiceManager()
{

}


Result<UamlServiceManagerPtr> UamlServiceManager::Create(System::UI::Application* app)
{
	try
	{
		return Result<UamlServiceManagerPtr>(std::make_shared<UamlServiceManager>(app));
	}
	catch (...) {}
	return Result<UamlServiceManagerPtr>(nullptr);
}


Result<UamlReaderPtr> UamlServiceManager::CreateReader()
{
	try
	{
		return Result<UamlReaderPtr>(std::make_shared<UamlReader>(std::shared_ptr<UamlServiceManager>(this)));
	}
	catch (...) {}
	return Result<UamlReaderPtr>(nullptr);
}

