#include "InertiaString.h"
#include <unordered_map>
#include <thread>
#include "CriticalSection.h"
using namespace System;

	struct InertiaStringCache
	{
		typedef std::unordered_map<std::wstring, std::weak_ptr<const std::wstring>> container_type;
	private:
		container_type container;
		CriticalSection mutex;
		InertiaStringCache() = default;
		~InertiaStringCache() = default;
	public:
		InertiaStringCache(InertiaStringCache const&) = delete;
		InertiaStringCache(InertiaStringCache&&) = delete;

		InertiaStringCache& operator = (InertiaStringCache const&) = delete;
		InertiaStringCache& operator = (InertiaStringCache&&) = delete;

		static InertiaStringCache& ref() throw();
		
		std::shared_ptr<const std::wstring> find(const std::wstring& value) throw();

	private:
		std::shared_ptr<const std::wstring> insert(const std::wstring& value) throw();

		static void remove(std::wstring const* ptr) throw();

	};

	InertiaStringCache& InertiaStringCache::ref() throw()
	{
		static InertiaStringCache instance;
		return instance;
	}

	std::shared_ptr<const std::wstring> InertiaStringCache::find(const std::wstring& value) throw()
	{
		std::lock_guard<CriticalSection> lock{ this->mutex };
		auto iter = this->container.find(value);
		if (iter != this->container.end()) { return iter->second.lock(); }
		return insert(value);
	}

	std::shared_ptr<const std::wstring> InertiaStringCache::insert(const std::wstring& value) throw()
	{
		auto result = this->container.emplace(value, std::shared_ptr<std::add_const_t<std::wstring>> { });
		if (!std::get<1>(result)) {}
		auto iter = std::get<0>(result);
		auto const& key = std::get<0>(*iter);
		std::shared_ptr<std::add_const_t<std::wstring>> shared{ std::addressof(key), remove };
		this->container[key] = shared;
		return shared;
	}

	void InertiaStringCache::remove(std::wstring const* ptr) throw()
	{
		if (!ptr) { return; }
		InertiaStringCache& cache = ref();
		try
		{
			std::lock_guard<CriticalSection> lock{cache.mutex };
			cache.container.erase(*ptr);
		}
		catch (...)
		{
		}
	}


	InertiaString::InertiaString(const std::wstring& str) : handle{ InertiaStringCache::ref().find(str) }
	{

	}

	InertiaString::InertiaString(const wchar_t* str) : handle{ InertiaStringCache::ref().find(str) }
	{
	}

	InertiaString::InertiaString(const wchar_t* str, size_t len) : handle{ InertiaStringCache::ref().find(len==0 ? std::wstring(): std::wstring(str, len)) }
	{

	}

	InertiaString::InertiaString() : handle{ InertiaStringCache::ref().find(std::wstring()) }
	{

	}


	const std::wstring* InertiaString::operator -> () const throw()
	{
		return this->handle.get();
	}

	InertiaString::operator const std::wstring&() const throw()
	{
		return *this->handle;
	}

	const std::wstring& InertiaString::get() const throw()
	{
		return *this->handle;
	}

	void InertiaString::swap(InertiaString& that) throw()
	{
		std::swap(this->handle, that.handle);
	}

	namespace System
	{
		bool operator == (InertiaString const& lhs, InertiaString const& rhs)
		{
			return std::equal_to<std::wstring> { }(lhs, rhs);
		}

		bool operator != (InertiaString const& lhs, InertiaString const& rhs)
		{
			return std::not_equal_to<std::wstring> { }(lhs, rhs);
		}

		bool operator >= (InertiaString const& lhs, InertiaString const& rhs)
		{
			return std::greater_equal<std::wstring> { }(lhs, rhs);
		}

		bool operator <= (InertiaString const& lhs, InertiaString const& rhs)
		{
			return std::less_equal<std::wstring> { }(lhs, rhs);
		}

		bool operator > (InertiaString const& lhs, InertiaString const& rhs)
		{
			return std::greater<std::wstring> { }(lhs, rhs);
		}

		bool operator < (InertiaString const& lhs, InertiaString const& rhs)
		{
			return std::less<std::wstring> { }(lhs, rhs);
		}

		bool operator == (InertiaString const& lhs, std::wstring const& rhs)
		{
			return std::equal_to<std::wstring> { }(lhs, rhs);
		}

		bool operator != (InertiaString const& lhs, std::wstring const& rhs)
		{
			return std::not_equal_to<std::wstring> { }(lhs, rhs);
		}

		bool operator >= (InertiaString const& lhs, std::wstring const& rhs)
		{
			return std::greater_equal<std::wstring> { }(lhs, rhs);
		}

		bool operator <= (InertiaString const& lhs, std::wstring const& rhs)
		{
			return std::less_equal<std::wstring> { }(lhs, rhs);
		}

		bool operator > (InertiaString const& lhs, std::wstring const& rhs)
		{
			return std::greater<std::wstring> { }(lhs, rhs);
		}

		bool operator < (InertiaString const& lhs, std::wstring const& rhs)
		{
			return std::less<std::wstring> { }(lhs, rhs);
		}
	}