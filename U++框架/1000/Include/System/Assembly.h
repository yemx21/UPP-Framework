#pragma once
#include "Version.h"
//#include <guiddef.h>
#include <vector>
#include "Common.h"
namespace System
{
	class NameSpace;
	class Factory;

	/// <summary>
	/// 程序集错误枚举
	/// </summary>
	enum class AssemblyError
	{
		/// <summary>
		/// 成功
		/// </summary>
		Success,

		/// <summary>
		/// 空命名空间
		/// </summary>
		EmptyNameSpace,

		/// <summary>
		/// 无效命名空间
		/// </summary>
		InvalidNameSpace,

		/// <summary>
		/// 已存在
		/// </summary>
		Existed,

		/// <summary>
		/// 已被另一个父级享有
		/// </summary>
		OwnedByOther,
	};

	class Assembly_impl;

	class NameSpaceEnumerator_impl;

	/// <summary>
	/// 命名空间枚举器类
	/// </summary>
	class RUNTIME_API NameSpaceEnumerator : _noncopyable
	{
	private:
		friend class NameSpace;
		friend class Assembly;
		friend class NameSpaceEnumerator_impl;
		NameSpaceEnumerator_impl* impl;
		NameSpaceEnumerator();
	public:
		~NameSpaceEnumerator();

		/// <summary>
		/// 获取子项数
		/// </summary>
		/// <returns></returns>
		unsigned int GetCount() const;

		/// <summary>
		/// 获取当前子项名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetCurrentName() const;

		/// <summary>
		/// 获取当前子项
		/// </summary>
		/// <returns></returns>
		const NameSpace* GetCurrent() const;

		/// <summary>
		/// 获取子项所属程序集
		/// </summary>
		/// <returns></returns>
		const Assembly* GetCurrentAssembly() const;

		/// <summary>
		/// 下一子项
		/// </summary>
		void Next();

		/// <summary>
		/// 判断是否迭代至末尾
		/// </summary>
		bool GetIsEnd() const;

		/// <summary>
		/// 当前子项名称
		/// </summary>
		__declspec(property(get = GetCurrentName)) const wchar_t* CurrentName;

		/// <summary>
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) const NameSpace* Current;

		/// <summary>
		/// 当前子项所属程序集
		/// </summary>
		__declspec(property(get = GetCurrentAssembly)) const Assembly* CurrentAssembly;

		/// <summary>
		/// 子项数
		/// </summary>
		__declspec(property(get = GetCount)) unsigned int Count;

		/// <summary>
		/// 是否迭代至末尾
		/// </summary>
		__declspec(property(get = GetIsEnd)) bool IsEnd;
	};

	/// <summary>
	/// 程序集
	/// </summary>
	class RUNTIME_API Assembly final: _noncopyable
	{
	private:
		friend class Factory;
		friend class Assembly_impl;
		Assembly_impl*  internal; 
		Assembly(); 
	public:
		/// <summary>
		/// 获取命名空间枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<NameSpaceEnumerator> GetNameSpaceEnumerator(bool subNameSpace) const;

		/// <summary>
		/// 增加命名空间
		/// </summary>
		/// <returns></returns>
		AssemblyError AddNameSpace(NameSpace* space);

		/// <summary>
		/// 删除命名空间
		/// </summary>
		/// <returns></returns>
		AssemblyError RemoveNameSpace(NameSpace* space);

		/// <summary>
		/// 创建程序集
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <returns></returns>
		static Assembly* Create(const wchar_t* name);

		/// <summary>
		/// 创建程序集
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <param name="ver">版本号</param>
		/// <returns></returns>
		static Assembly* Create(const wchar_t* name, const System::Version& ver);

		/// <summary>
		/// 创建程序集
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <param name="ver">版本号</param>
		/// <param name="publicKey">公匙</param>
		/// <returns></returns>
		static Assembly* Create(const wchar_t* name, const System::Version& ver, uGUID publicKey);

		/// <summary>
		/// 查找指定名称的命名空间
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <returns></returns>
		const NameSpace* FindNameSpace(const wchar_t* name) const;

		~Assembly();

		const wchar_t* GetName() const;
		const uGUID GetToken() const;
		const System::Version* GetVersion() const;

		_declspec(property(get = GetName)) const wchar_t* Name;
		_declspec(property(get = GetToken)) const uGUID Token;
		_declspec(property(get = GetVersion)) const System::Version* Version;

		bool operator ==(const Assembly& assembly) const;
		bool operator !=(const Assembly& assembly) const;

	};
}