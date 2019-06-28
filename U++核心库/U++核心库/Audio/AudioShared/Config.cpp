//#include "Config.h"
//#include "U++\Assembly.uh"
//safe_void_ptr AudioSharedInit();     ///初始化函数
//bool AudioSharedExit();   ///退出函数
//
//System::CoreDllMain AudioSharedDllMain
//{
//	AudioSharedInit,    ///初始化函数
//	AudioSharedExit,
//	nullptr
//};
//
//HMODULE GetCurrentModuleHandle()
//{
//	HMODULE hMod = NULL;
//	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
//		reinterpret_cast<LPCWSTR>(&GetCurrentModuleHandle),
//		&hMod);
//	return hMod;
//}
//
//safe_void_ptr AudioSharedInit()
//{
//	auto res = AudioSharedDllMain.Helper->RegisiterModule(GetCurrentModuleHandle());
//	if (res.Value == FactoryError::OK || res.Value == FactoryError::AlreadyExist)
//	{
//		
//		Assembly_AudioShared& AudioSharedFactory = Assembly_AudioShared::GetInstance();
//		if(AudioSharedFactory.Add(res.State, AudioSharedDllMain) == FactoryError::OK)
//			return res.State;
//	}
//	return nullptr;
//}
//
//bool AudioSharedExit()
//{
//	Assembly_AudioShared& AudioSharedFactory = Assembly_AudioShared::GetInstance();
//	return AudioSharedFactory.Remove(AudioSharedDllMain) == FactoryError::OK;
//}
//
//AUDIOSHARED_CAPI System::CoreDllMain* CoreDllMain()
//{
//	return &AudioSharedDllMain;
//}