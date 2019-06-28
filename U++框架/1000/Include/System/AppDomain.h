#pragma once
#include "Config.h"
#include "Reflection.h"
#include "IApplication.h"
namespace System
{
	class AppDomain;

	class Factory;

	class AssemblyEnumerator_impl;

	/// <summary>
	/// 程序集枚举器类
	/// </summary>
	class RUNTIME_API AssemblyEnumerator: _noncopyable
	{
	private:
		friend class AppDomain;
		friend class AssemblyEnumerator_impl;
		AssemblyEnumerator_impl* impl;
		AssemblyEnumerator();
	public:
		~AssemblyEnumerator();

		unsigned int GetCount() const;

		const wchar_t* GetCurrentName() const;

		Version GetCurrentVersion() const;

		uGUID GetCurrentPublicKey() const;

		const Assembly* GetCurrent(char* privateKey = nullptr, unsigned int privateKeySize = 0) const;

		void Next();

		void Previous();

		bool GetIsEnd() const;

		__declspec(property(get = GetCurrentName)) const wchar_t* CurrentName;
		__declspec(property(get = GetCurrentVersion)) Version CurrentVersion;
		__declspec(property(get = GetCurrentPublicKey)) uGUID CurrentPublicKey;
		__declspec(property(get = GetCount)) unsigned int Count;
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	class AppDomain_impl;

	/// <summary>
	/// 程序域类
	/// </summary>
	class RUNTIME_API AppDomain final: _noncopyable
	{
	protected:
		friend class Factory;
		friend class AppDomain_impl;
		static AppDomain_impl* impl;
	private:
		AppDomain();
	public:
		/// <summary>
		/// 加载指定路径的基于U++的动态链接库(.dll)
		/// </summary>
		/// <param name="path">本地路径</param>
		/// <returns></returns>
		static Result<FactoryDllError, safe_void_ptr> LoadDll(const wchar_t* path);

		/// <summary>
		/// 卸载指定基于U++的模块
		/// </summary>
		/// <param name="module">指定模块</param>
		/// <returns></returns>
		static Result<FactoryDllError> UnLoadDll(const safe_void_ptr& module);

		/// <summary>
		/// 获取当前程序域的程序集枚举器
		/// </summary>
		/// <returns></returns>
		static std::unique_ptr<AssemblyEnumerator> GetAssemblyEnumerator();

		/// <summary>
		/// 获取指定模块的程序集枚举器
		/// </summary>
		/// <param name="module">指定模块</param>
		/// <returns></returns>
		static std::unique_ptr<AssemblyEnumerator> GetAssemblyEnumerator(const safe_void_ptr& module);

		/// <summary>
		/// 查找指定程序集
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <param name="ver">指定版本号</param>
		/// <returns></returns>
		static const Assembly* FindAssembly(const wchar_t* name, const System::Version& ver);

		/// <summary>
		/// 查找指定程序集
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <param name="ver">指定版本号</param>
		/// <param name="privateKey">私匙</param>
		/// <param name="privateKeySize">私匙长度</param>
		/// <returns></returns>
		static const Assembly* FindAssembly(const wchar_t* name, const System::Version& ver, char* privateKey, unsigned int privateKeySize);

		/// <summary>
		/// 查找指定程序集
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <param name="ver">指定版本号</param>
		/// <param name="publicKey">公匙</param>
		/// <param name="privateKey">私匙</param>
		/// <param name="privateKeySize">私匙长度</param>
		/// <returns></returns>
		static const Assembly* FindAssembly(const wchar_t* name, const System::Version& ver, uGUID publicKey, char* privateKey, unsigned int privateKeySize);


		/// <summary>
		/// 生成128位的哈希值
		/// </summary>
		///<remarks>返回0则说明内部错误</remarks>
		/// <returns></returns>
		static UInt128 GenerateHashId128();

		/// <summary>
		/// 注销128位的哈希值
		/// </summary>
		static void DestoryHashId128(const UInt128& id);

		/// <summary>
		/// 加载语言包
		/// </summary>
		/// <returns></returns>
		static Result<bool> LoadLanguagePackage(const wchar_t* path);


		~AppDomain();
	};
}