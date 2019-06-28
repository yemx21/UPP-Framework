#pragma once
#include "Config.h"
#include <vector>
#include "Common.h"
namespace System
{
	class Assembly;
	class NameSpaceEnumerator_impl;
	class NameSpaceEnumerator;
	class Type;
	class NameSpace_impl;
	class TypeEnumerator_impl;

	/// <summary>
	/// 命名空间错误
	/// </summary>
	enum class NameSpaceError
	{
		/// <summary>
		/// 成功
		/// </summary>
		Success,

		/// <summary>
		/// 无错误
		/// </summary>
		None,

		/// <summary>
		/// 空类型
		/// </summary>
		EmptyType,

		/// <summary>
		/// 无效类型
		/// </summary>
		InvalidType,

		/// <summary>
		/// 已存在
		/// </summary>
		Existed,

		/// <summary>
		/// 未知错误
		/// </summary>
		Unknown,

		/// <summary>
		/// 空命名空间
		/// </summary>
		EmptyNameSpace,

		/// <summary>
		/// 无效命名空间
		/// </summary>
		InvalidNameSpace,

		/// <summary>
		/// 已被另一个父级享有
		/// </summary>
		OwnedByOther,
	};

	/// <summary>
	/// 类型枚举器类
	/// </summary>
	class RUNTIME_API TypeEnumerator:_noncopyable
	{
	private:
		friend class NameSpace;
		TypeEnumerator_impl* impl;
		TypeEnumerator();
	public:
		~TypeEnumerator();
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
		_type GetCurrent() const;

		/// <summary>
		/// 获取当前子项所属命名空间
		/// </summary>
		/// <returns></returns>
		const NameSpace* GetCurrentNameSpace() const;

		/// <summary>
		/// 获取当前子项所属程序集
		/// </summary>
		/// <returns></returns>
		const Assembly* GetCurrentAssembly() const;

		/// <summary>
		/// 获取下一个子项
		/// </summary>
		/// <returns></returns>
		void Next();

		/// <summary>
		/// 获取上一个子项
		/// </summary>
		/// <returns></returns>
		void Previous();

		/// <summary>
		/// 获取是否迭代至末尾
		/// </summary>
		/// <returns></returns>
		bool GetIsEnd() const;

		/// <summary>
		/// 当前子项名称
		/// </summary>
		__declspec(property(get = GetCurrentName)) const wchar_t* CurrentName;

		/// <summary>
		/// 当前子项
		/// </summary>
		__declspec(property(get = GetCurrent)) _type Current;

		/// <summary>
		/// 当前子项所属命名空间
		/// </summary>
		__declspec(property(get = GetCurrentNameSpace)) const NameSpace* CurrentNameSpace;

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
	/// 命名空间
	/// </summary>
	class RUNTIME_API NameSpace final: _noncopyable
	{
	private:
		friend class Assembly;
		friend class NameSpace_impl;
		friend class NameSpaceEnumerator_impl;
		NameSpace_impl* internal;
	public:		

		/// <summary>
		/// 获取类型枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<TypeEnumerator> GetTypeEnumerator() const;

		/// <summary>
		/// 获取命名空间枚举器
		/// </summary>
		/// <returns></returns>
		std::unique_ptr<NameSpaceEnumerator> GetNameSpaceEnumerator(bool subNameSpace) const;

		/// <summary>
		/// 增加命名空间
		/// </summary>
		/// <returns></returns>
		NameSpaceError AddNameSpace(NameSpace* space);

		/// <summary>
		/// 注册指定类型
		/// </summary>		
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		NameSpaceError Register(_type type);

		/// <summary>
		/// 卸载指定类型
		/// </summary>		
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		NameSpaceError UnRegister(_type type);

		/// <summary>
		/// 卸载指定类型
		/// </summary>		
		/// <param name="subnamespace">子命名空间名称</param>
		/// <param name="type">指定类型</param>
		/// <returns></returns>
		NameSpaceError UnRegister(const wchar_t* subnamespace, _type type);


		/// <summary>
		/// 创建<see cref="NameSpace"/>实例.
		/// </summary>
		/// <param name="name">名称</param>
		NameSpace(const wchar_t* name);

		~NameSpace();

		/// <summary>
		/// 查找指定名称的类型
		/// </summary>
		/// <param name="name">名称</param>
		/// <returns></returns>
		_type FindType(const wchar_t* name) const;

		/// <summary>
		/// 查找指定名称的命名空间
		/// </summary>
		/// <param name="name">指定名称</param>
		/// <returns></returns>
		const NameSpace* FindNameSpace(const wchar_t* name) const;

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetName() const;

		/// <summary>
		/// 获取所属程序集
		/// </summary>
		/// <returns></returns>
		const Assembly* GetAssembly() const;

		/// <summary>
		/// 获取父级命名空间
		/// </summary>
		/// <returns></returns>
		const NameSpace* GetParent() const;

		/// <summary>
		/// 名称
		/// </summary>
		/// <returns></returns>
		_declspec(property(get = GetName)) const wchar_t* Name;

		/// <summary>
		///所属程序集
		/// </summary>
		/// <returns></returns>
		_declspec(property(get = GetAssembly)) const System::Assembly* Assembly;

		/// <summary>
		///父级命名空间
		/// </summary>
		/// <returns></returns>
		_declspec(property(get = GetParent)) const NameSpace* Parent;

		bool operator ==(const NameSpace& space) const;
		bool operator !=(const NameSpace& space) const;

	};
}