#include "BuiltinAssembly.h"

using namespace System;
using namespace Internal;


BuiltInAssemblyAudioFactory::BuiltInAssemblyAudioFactory() : Audio(nullptr)
{
	AudioShared = Assembly::Create(L"AudioShared.dll", Version(1, 0),{ 0x1694661c, 0x2d9c, 0x4b38, { 0xb4, 0x3f, 0xa4, 0x77, 0x3d, 0x88, 0x3c, 0x5f } });
	System = new NameSpace(L"System");
	Audio = new NameSpace(L"Audio");
	AudioShared->AddNameSpace(System);
	System->AddNameSpace(Audio);
}

BuiltInAssemblyAudioFactory& BuiltInAssemblyAudioFactory::GetBuiltIn()
{
	static BuiltInAssemblyAudioFactory instance;
	return instance;
}

Type* BuiltInAssemblyAudioFactory::Regisiter(Type* type)
{
	Internal::BuiltInAssemblyAudioFactory& AudioFactory = Internal::BuiltInAssemblyAudioFactory::GetBuiltIn();
	if (AudioFactory.Audio)
	{
		NameSpaceError err = AudioFactory.Audio->Register(type);
		if (err == NameSpaceError::Success || err == NameSpaceError::Existed)
			return type;
	}
	return nullptr;
}

FactoryError BuiltInAssemblyAudioFactory::Add(const safe_void_ptr& mod, System::CoreDllMain dll)
{
	Internal::BuiltInAssemblyAudioFactory& AudioFactory = Internal::BuiltInAssemblyAudioFactory::GetBuiltIn();
	if (AudioFactory.AudioShared && mod)
	{
		AudioFactory.Module = mod;
		return dll.Helper->AddAssembly(AudioFactory.Module, AudioFactory.AudioShared, nullptr, 0);
	}
	return FactoryError::Unknown;
}


FactoryError BuiltInAssemblyAudioFactory::Remove(System::CoreDllMain dll)
{	
	Internal::BuiltInAssemblyAudioFactory& AudioFactory = Internal::BuiltInAssemblyAudioFactory::GetBuiltIn();
	if (AudioFactory.AudioShared)
	{
		return dll.Helper->RemoveAssembly(AudioFactory.Module, AudioFactory.AudioShared, nullptr, 0);
	}
	return FactoryError::OK;
}
