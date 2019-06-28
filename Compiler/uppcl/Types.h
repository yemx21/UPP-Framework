#pragma once
#include <string>
#include <vector>
#include <memory>
#include <any>
#include <guiddef.h>
#include <set>
#include <map>
#include <queue>
#include <llvm/Support/raw_ostream.h>

//#define __uobj(...)  \
//__pragma(warning(push)) \
//__pragma(warning(disable:5030)) \
//[[uobj(__VA_ARGS__)]]\
//__pragma(warning(pop))

namespace clang {
	class CXXMethodDecl;
	class CXXRecordDecl;
	class CXXConstructorDecl;
	class MSPropertyDecl;
	class CXXConversionDecl;
	class EnumDecl;
	class Preprocessor;
	class Sema;
	class TypeDecl;
	class Type;
	class QualType;
	class EnumConstantDecl;
}

namespace uppcl
{
	class UVersion
	{
	private:
		int _Build;
		int _Revision;
		int _Major;
		int _Minor;
	public:
		/// <summary>
		/// 创建<see cref="UVersion"/>实例.
		/// </summary>
		/// <param name="major">主版本号</param>
		/// <param name="minor">次要版本号</param>
		/// <param name="build">内部版本号</param>
		/// <param name="revision">修订号</param>
		UVersion(int major, int minor, int build, int revision);

		/// <summary>
		/// 创建<see cref="UVersion"/>实例.
		/// </summary>
		/// <param name="major">主版本号</param>
		/// <param name="minor">次要版本号</param>
		/// <param name="build">内部版本号</param>
		UVersion(int major, int minor, int build);

		/// <summary>
		/// 创建<see cref="UVersion"/>实例.
		/// </summary>
		/// <param name="major">主版本号</param>
		/// <param name="minor">次要版本号</param>
		UVersion(int major, int minor);

		/// <summary>
		/// 创建<see cref="UVersion"/>实例.
		/// </summary>
		UVersion();

		UVersion(const UVersion& ver);

		/// <summary>
		/// 获取主版本号
		/// </summary>
		/// <returns></returns>
		int GetMajor() const;

		/// <summary>
		/// 获取次要版本号
		/// </summary>
		/// <returns></returns>
		int GetMinor() const;

		/// <summary>
		/// 获取内部版本号
		/// </summary>
		/// <returns></returns>
		int GetBuild() const;

		/// <summary>
		/// 获取修订号
		/// </summary>
		/// <returns></returns>
		int GetRevision() const;

		/// <summary>
		/// 获取修订号的高16位
		/// </summary>
		/// <returns></returns>
		short GetMajorRevision() const;

		/// <summary>
		/// 获取修订号的低16位
		/// </summary>
		/// <returns></returns>
		short GetMinorRevision() const;

		/// <summary>
		/// 设置主版本号
		/// </summary>
		/// <param name="major">主版本号</param>
		void SetMajor(int major);

		/// <summary>
		/// 设置次要版本号
		/// </summary>
		/// <param name="major">次要版本号</param>
		void SetMinor(int minor);


		/// <summary>
		/// 设置内部版本号
		/// </summary>
		/// <param name="major">内部版本号</param>
		void SetBuild(int build);

		/// <summary>
		/// 设置修订号
		/// </summary>
		/// <param name="major">修订号</param>
		void SetRevision(int revision);

		/// <summary>
		/// 判断是否为空
		/// </summary>
		/// <returns></returns>
		bool GetIsEmpty() const;

		/// <summary>
		/// 主版本号
		/// </summary>

		_declspec(property(get = GetMajor, put = SetMajor)) int Major;
		/// <summary>
		/// 次要版本号
		/// </summary>
		_declspec(property(get = GetMinor, put = SetMinor)) int Minor;
		/// <summary>
		/// 内部版本号
		/// </summary>
		_declspec(property(get = GetBuild, put = SetBuild)) int Build;
		/// <summary>
		/// 修订号
		/// </summary>
		_declspec(property(get = GetRevision, put = SetRevision)) int Revision;
		/// <summary>
		/// 修订号的高16位
		/// </summary>
		_declspec(property(get = GetMajorRevision)) short MajorRevision;
		/// <summary>
		/// 修订号的低16位
		/// </summary>
		_declspec(property(get = GetMinorRevision)) short MinorRevision;
		/// <summary>
		/// 是否为空
		/// </summary>
		_declspec(property(get = GetIsEmpty)) bool IsEmpty;

		bool operator == (UVersion& ver) const;
		bool operator == (const UVersion& ver) const;
		bool operator != (UVersion& ver) const;
		bool operator != (const UVersion& ver) const;
		bool operator > (UVersion& ver) const;
		bool operator > (const UVersion& Ver) const;
		bool operator < (UVersion& ver) const;
		bool operator < (const UVersion& ver) const;
		bool operator >= (UVersion& ver) const;
		bool operator >= (const UVersion& ver) const;
		bool operator <= (UVersion& ver) const;
		bool operator <= (const UVersion& ver) const;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::string& input, UVersion& result);
		std::string ToString(const char* fmt = nullptr) const;
	};

	struct UClass
	{
		clang::CXXRecordDecl *Record = nullptr;
		UVersion Version;
		std::string Description;
		std::string FullName;
		std::string Name;
		std::string SplitedQualifiedName;
		std::string Assembly;
		std::string Namespace;
		std::string SplitedNamespace;
		std::string SplitedVersion;

		std::vector<const clang::CXXRecordDecl*> Bases;

		std::vector<clang::CXXMethodDecl*> Methods;
		std::vector<clang::CXXMethodDecl*> Operators;
		clang::CXXConstructorDecl* DefaultConstructor = nullptr;
		clang::CXXMethodDecl* StaticDefaultConstructor = nullptr;
		std::vector<clang::CXXMethodDecl*> StaticConstructors;
		std::vector<clang::CXXConstructorDecl*> Constructors;
		std::map<clang::EnumConstantDecl*, std::string> Values;

		std::vector<clang::MSPropertyDecl*> MSProperties;
		std::vector<clang::CXXConversionDecl*> Conversions;

		bool Valid = false;
		explicit UClass() {}
	};

//#include <clang/Frontend/CompilerInstance.h>
//#include "uversion.h"

//#include <iostream>
	class UAssemblyDefMap;
	struct assdef_map_finder_t;
	struct assdef_map_finder1_t;

	class UAssemblyDef
	{
		friend struct assdef_map_finder_t;
		friend struct assdef_map_finder1_t;
		friend class UAssemblyDefMap;
		GUID pbk;
		UVersion ver;
		std::string name;
		UAssemblyDef(const std::string& ass, const UVersion& version, GUID& guid);

		UAssemblyDef(const UAssemblyDef& def);
	public:
		const std::string& PublicKey();
		const std::string& VersionStr();
		const UVersion& Ver();
		const std::string& Name();

		static UAssemblyDef* Create(const std::string& ass, const std::string& ver_str, const std::string& guid_str);
	};

	struct assdef_map_finder_t
	{
		UAssemblyDef* cache = nullptr;
		assdef_map_finder_t(UAssemblyDef* def)
			: cache(def)
		{
		}
		bool operator()(UAssemblyDef* p)
		{
			if (p == cache) return true;
			if (!p || !cache) return false;
			return p->name == cache->name && p->ver == cache->ver && memcmp(&p->pbk, &cache->pbk, sizeof(GUID)) == 0;
		}

	};

	struct assdef_map_finder1_t
	{
		std::string ass;
		UVersion ver;
		assdef_map_finder1_t(const std::string& name, const UVersion& version)
			: ass(name), ver(version)
		{
		}
		bool operator()(UAssemblyDef* p)
		{
			if (!p) return false;
			return p->name == ass && p->ver == ver;
		}

	};

	class UAssemblyDefMap
	{
		std::vector<UAssemblyDef*> defs;
	public:
		UAssemblyDefMap();
		~UAssemblyDefMap();

		unsigned int Count() const;

		void Parse(const std::string& str);

		UAssemblyDef* Query(const std::string& name, const UVersion& ver);
		std::vector<UVersion> Query(const std::string& name);
	};

	class UNamespace
	{
	public:
		std::string Name;
		std::string FullName;
		std::vector<UNamespace> Children;
		UNamespace(const std::string& name, const std::string fullname);
		UNamespace(const UNamespace& ns);

		void Add(const UNamespace& ns);

		UNamespace& Get(size_t index);
		UNamespace& Get(std::vector<UNamespace>::iterator iter);

		bool operator ==(const std::string& na);
		bool operator !=(const std::string& na);

		void GetAllNamespace(std::set<std::string>& nss, bool full = true);

		void LoopWriteAddNameSpace(llvm::raw_fd_ostream& Out, const std::string& suffix, const std::string& splitver, int& count);
	};

	class UAssembly
	{
	private:
		void AddNamespace(std::queue<std::string>& nss, UNamespace& par);
		void AddNamespace(std::queue<std::string>& nss);
	public:
		std::string Name;
		UVersion Ver;
		std::string Ver_str;
		std::vector<UNamespace> Children;
		std::string SplitedVersion;

		UAssembly(const UAssembly& ns);
		UAssembly(const std::string& name, const std::string& ver);
		UAssembly(const std::string& name, const UVersion& ver);

		void AddNamespace(const std::string& name);
		void GetAllNamespace(std::set<std::string>& nss, bool full = true);

		void LoopWriteAddNameSpace(llvm::raw_fd_ostream& Out, const std::string& suffixr, int& count);
	};

	typedef struct ass_finder_t
	{
		std::string Version;

		ass_finder_t(const std::string& ver)
			: Version(ver)
		{
		}
		bool operator()(const UAssembly& p)
		{
			return p.Ver_str.compare(Version) == 0;
		}

	}ass_finder_t;

	class UAssemblyMap
	{
	public:
		std::string Name;
		std::vector<UAssembly> Children;

		UAssemblyMap(const std::string& name);
		UAssemblyMap(const UAssemblyMap& map);

		void AddNamespace(const std::string& ns, const std::string& ver);
		void GetAllNamespace(std::set<std::string>& nss, bool full = true);

		void LoopWriteAddNameSpace(llvm::raw_fd_ostream& Out, int& count);
	};

	typedef struct ass_map_finder_t
	{
		std::string Name;
		ass_map_finder_t(const std::string& name)
			: Name(name)
		{
		}
		bool operator()(const UAssemblyMap& p)
		{
			return p.Name.compare(Name) == 0;
		}

	}ass_map_finder_t;

	class UAssemblyPool
	{
	public:
		std::vector<UAssemblyMap> Pool;
		std::vector<std::string> GeneratedPool;
		UAssemblyDefMap Defs;
		UAssemblyDefMap Excludes;

		UAssemblyPool();

		void Parse(const std::string& def, const std::string& exclude);

		void Add(const std::string& ass, const std::string ver, const std::string& ns);

		std::vector<UVersion>Query(const std::string& ass);
	};
}