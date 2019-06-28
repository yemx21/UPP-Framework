#pragma once
#include "Any.h"
namespace System
{

	class IEnumerator
	{
	public:
		virtual Any GetCurrent() = 0;
		virtual safe_void_ptr GetCurrent(size_t& ptrsize) = 0;
		virtual bool MoveNext() = 0;
		virtual void Reset() = 0;
	};

	class IPairEnumerator
	{
	public:
		virtual Any GetKey() = 0;
		virtual Any GetValue() = 0;
		virtual safe_void_ptr GetKey(size_t& ptrsize) = 0;
		virtual safe_void_ptr GetValue(size_t& ptrsize) = 0;
		virtual bool MoveNext() = 0;
		virtual void Reset() = 0;
	};

	class ITurpleEnumerator
	{
	public:
		virtual Any GetValue(size_t turpleIndex) = 0;
		virtual VariadicAny GetValues() = 0;
		virtual safe_void_ptr GetValue(size_t turpleIndex, size_t& ptrsize) = 0;
		virtual std::vector<safe_void_ptr> GetValues(std::vector<size_t>& ptrsizes) = 0;
		virtual bool MoveNext() = 0;
		virtual void Reset() = 0;
	};

	class IEnumerable
	{
	public:
		virtual IEnumerator* GetEnumerator() = 0;
	};

	class IPairEnumerable
	{
	public:
		virtual IPairEnumerator* GetEnumerator() = 0;
	};

	class ITurpleEnumerable
	{
	public:
		virtual ITurpleEnumerator* GetEnumerator() = 0;
	};

	template<typename VALUE>
	class Enumerator
	{
	public:
		virtual VALUE GetCurrent() = 0;
		virtual bool MoveNext() = 0;
		virtual void Reset() = 0;
	};

	template<typename KEY, typename VALUE>
	class PairEnumerator
	{
	public:
		virtual KEY GetKey() = 0;
		virtual VALUE GetValue() = 0;
		virtual bool MoveNext() = 0;
		virtual void Reset() = 0;
	};

}