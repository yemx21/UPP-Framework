#pragma once
#include "Reflection.h"
#include "Object.h"
namespace System
{
	/// <summary>
	/// 核心动态库接口
	/// </summary>
	struct CoreDllMain
	{
		/// <summary>
		/// 初始化函数
		/// </summary>
		Result<bool, safe_void_ptr> (*Init)();

		/// <summary>
		/// 退出函数
		/// </summary>
		Result<bool> (*Exit)();
		
		/// <summary>
		///  帮助工厂
		/// </summary>
		const FactoryHelper* Helper;
	};
	/// <summary>
	/// 初始化函数
	/// </summary>
	typedef CoreDllMain* (*CoreDllMainEntry)();       ///声明接口函数地址

}
