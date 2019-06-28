#pragma once
#include "Common.h"
#include "Version.h"
#include "NameSpace.h"
#include "NameSpace_impl.h"
#include <algorithm>
namespace System
{
	class BuiltInAssemblyFactory;

	class NameSpaceNameComparer
	{
	private:
		const wchar_t* name;
	public:
		explicit NameSpaceNameComparer(const wchar_t* str) : name(str)
		{
		}
		bool operator() (NameSpace* mod)
		{
			if (name == nullptr)
			{
				if (mod->Name == nullptr) return true;
				if (wcslen(mod->Name) == 0) return true;
			}
			if (mod == nullptr) return false;
			bool dss=wcscmp(mod->Name, name) == 0;
			return dss;
		}
	};

	class NameSpaceComparer
	{
	private:
		const wchar_t* name;
	public:
		explicit NameSpaceComparer(NameSpace* mod)
		{
			name = mod != NULL ? mod->Name : 0;
		}
		bool operator() (NameSpace* mod)
		{
			if (name == nullptr)
			{
				if (mod->Name == nullptr) return true;
				if (wcslen(mod->Name) == 0) return true;
			}
			if (mod == nullptr) return false;
			return wcscmp(mod->Name, name) == 0;
		}
	};

	class yield_iterator_warpper;

	class yield_iterator_impl
	{
	private:
		friend yield_iterator_warpper;
		yield_iterator_warpper* warpper;
	protected:

		void ensureiterateImpl();

		void iterateImpl();

		virtual void oniterate(void* p);

		void onyieldImpl();

	public:
		yield_iterator_impl(void*);
		~yield_iterator_impl();

		bool done();
	};

	template <typename T, typename Arg>
	class yield_iterator : public yield_iterator_impl, public std::iterator<std::forward_iterator_tag, T>
	{
	private:
		T m_value;
	public:
		yield_iterator(Arg* arg) : yield_iterator_impl(arg)
		{
		}

		virtual ~yield_iterator()
		{
		}

		T operator*()
		{
			ensureiterateImpl();
			return m_value;
		}
		yield_iterator& operator++()
		{
			iterateImpl();
			return *this;
		}

	protected:

		void yieldImpl(T value)
		{
			m_value = value;
			onyieldImpl();
		}

		// Yields the argument.
#define yield_return(x) { yieldImpl(x); if (yield_iterator::done()) return; }

// Break the iteration, simply by returning.
#define yield_break { return; }

// Allows the user implemented iterate function to call nested functions
// which in turn uses the yield_return or yield_nested macros.
#define yield_nested(x) { x; if (yield_iterator::done()) return; }

	public:

		bool operator==(const yield_iterator& yit) const
		{
			return this->m_value == yit.m_value;
		}

		bool operator!=(const yield_iterator& yit) const
		{
			return !(*this == yit);
		}

	private:
		struct yield_iterator_end {};

	public:
		yield_iterator_end& end()
		{
			static yield_iterator_end end;
			return end;
		}

		bool operator==(const yield_iterator_end& yite) const
		{
			return done();
		}

		bool operator!=(const yield_iterator_end& yite) const
		{
			return !(*this == yite);
		}

		virtual void iterate(Arg* p) = 0;

		void oniterate(void* p) override
		{
			iterate((Arg*)p);
		}
	};

	class NameSpaceSubEnumerator_impl : public yield_iterator<NameSpace*, std::vector<NameSpace*>>
	{
	public:
		NameSpaceSubEnumerator_impl(std::vector<NameSpace*>* owner) : yield_iterator<NameSpace*, std::vector<NameSpace*>>(owner)
		{

		}

		void iterate(std::vector<NameSpace*>* p);
	};


	class NameSpaceEnumerator_impl
	{
	public:
		typedef std::vector<NameSpace*>* base;
		typedef std::vector<NameSpace*>::const_iterator base_iterator;
		typedef std::vector<NameSpace*>::size_type pos_type;

	protected:
		friend class Assembly;
		friend class Assembly_impl;
		friend class NameSpace;
		friend class NameSpaceEnumerator;

		friend class NameSpaceSubEnumerator_impl;

		__inline static std::vector<NameSpace*>* GetChildren(NameSpace* p)
		{
			if (p->internal) return &p->internal->Children;
			return nullptr;
		}

		base iterOwner;
		base_iterator citer;
		std::mutex lock;

		NameSpaceSubEnumerator_impl* sub;

		NameSpaceEnumerator_impl(base owner, bool subNameSpace);

		~NameSpaceEnumerator_impl();

		void _Count(unsigned int& sum, NameSpace* space) const;

		unsigned int Count() const;

		const wchar_t* GetCurrentName() const;

		const NameSpace* GetCurrent() const;

		const Assembly* GetCurrentAssembly() const;

		void Next();

		bool IsEnd() const;
	};

	class Assembly_impl
	{
	protected:
		friend class Assembly;
		friend class BuiltInAssemblyFactory;
		InertiaString Name;
		System::Version* Version;
		uGUID Token;
		std::vector<NameSpace*> Registry;

		Assembly_impl(const wchar_t* name) : Name(name)
		{
			Version = new System::Version();
			memset(&Token, 0, sizeof(uGUID));
		}

		Assembly_impl(const wchar_t* name, System::Version ver) : Name(name)
		{
			Version = new System::Version(ver);
			memset(&Token, 0, sizeof(uGUID));
		}

		Assembly_impl(const wchar_t* name, System::Version ver, uGUID publicKey) : Name(name), Token(publicKey)
		{
			Version = new System::Version(ver);
		}

		~Assembly_impl()
		{
			SAFE_DELETE(Version);
			for (auto& i : Registry)
			{
				SAFE_DELETE(i);
			}
			Registry.clear();
		}

		static Assembly* Create(const wchar_t* name)
		{
			Assembly* assembly = new Assembly();
			assembly->internal = new Assembly_impl(name);
			return assembly;
		}
		static Assembly* Create(const wchar_t* name, System::Version ver)
		{
			Assembly* assembly = new Assembly();
			assembly->internal = new Assembly_impl(name, ver);
			return assembly;
		}

		static Assembly* Create(const wchar_t* name, System::Version ver, uGUID publicKey)
		{
			Assembly* assembly = new Assembly();
			assembly->internal = new Assembly_impl(name, ver, publicKey);
			return assembly;
		}

		static Assembly* Create(const wchar_t* name, NameSpace* space)
		{
			Assembly* assembly = new Assembly();
			assembly->internal = new Assembly_impl(name);
			assembly->internal->Registry.push_back(space);
			return assembly;
		}

		static Assembly* Create(const wchar_t* name, System::Version ver, NameSpace* space)
		{
			Assembly* assembly = new Assembly();
			assembly->internal = new Assembly_impl(name, ver); 
			assembly->internal->Registry.push_back(space);
			return assembly;
		}


		static Assembly* Create(const wchar_t* name, System::Version ver, uGUID publicKey, NameSpace* space)
		{
			Assembly* assembly = new Assembly();
			assembly->internal = new Assembly_impl(name, ver, publicKey);
			assembly->internal->Registry.push_back(space);
			return assembly;
		}

		static NameSpace* Get(const Assembly* assembly, const wchar_t* name)
		{
			if (!assembly) return nullptr;
			auto iter = std::find_if(assembly->internal->Registry.begin(), assembly->internal->Registry.end(), NameSpaceNameComparer(name));
			if (iter != assembly->internal->Registry.end()) return *iter;
			return nullptr;
		}


		bool operator ==(const Assembly_impl& assembly) const
		{
			return Name == assembly.Name && memcmp(&Token, &assembly.Token, sizeof(uGUID))==0 && Version==assembly.Version;
		}

		bool operator !=(const Assembly_impl& assembly) const
		{
			return Name != assembly.Name || memcmp(&Token, &assembly.Token, sizeof(uGUID)) != 0 || Version != assembly.Version;
		}

	};

}