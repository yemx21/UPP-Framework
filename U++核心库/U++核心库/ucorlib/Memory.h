#pragma once
#include <memory>
namespace System
{
	template<size_t ALIGN>
	class AlignedMemoryBlock_t
	{
	public:
		typedef AlignedMemoryBlock_t<ALIGN> self;
		AlignedMemoryBlock_t() : _ptr(), _size() {}

		void * ptr() { return _ptr; }
		const void * ptr() const { return _ptr; }
		size_t size() const { return _size; }

		void resize(size_t s)
		{
			if (s == _size){}
			else if (s == 0)
			{
				_aligned_free(_ptr);
				_ptr = NULL;
			}
			else
			{
				void * ptr;
				if (_ptr == NULL)
					ptr = _aligned_malloc(s, ALIGN);
				else
					ptr = _aligned_realloc(_ptr, s, ALIGN);
				if (ptr == NULL) throw std::bad_alloc();
				_ptr = ptr;
			}
			_size = s;
		}

		~AlignedMemoryBlock_t() { _aligned_free(_ptr); }

		self const & operator=(self const & other)
		{
			assign(other);
			return *this;
		}

		AlignedMemoryBlock_t(self const & other) : _ptr(), _size()
		{
			assign(other);
		}

		AlignedMemoryBlock_t(self && other)
		{
			_ptr = other._ptr;
			_size = other._size;
			other._ptr = NULL;
			other._size = 0;
		}

		void assign(self const & other)
		{
			resize(other.size());
			memcpy(ptr(), other.ptr(), size());
		}

		self const & operator=(self && other)
		{
			_aligned_free(_ptr);
			_ptr = other._ptr;
			_size = other._size;
			other._ptr = NULL;
			other._size = 0;
			return *this;
		}
	private:
		void * _ptr;
		size_t _size;
	};


	template<typename TYPE, size_t ALIGN>
	//对齐内存堆，对齐大小必须为2的整数次幂
	class AlignedMemoryBlock
	{
	public:
		typedef AlignedMemoryBlock<TYPE, ALIGN> self;
		void resize(size_t s) { m.resize(s *sizeof(TYPE)); }
		//获取长度
		size_t count() const { return m.size() / sizeof(TYPE); }
		//获取内存大小
		size_t size() const { return m.size(); }
		TYPE * ptr() { return reinterpret_cast<TYPE*>(m.ptr()); }
		const TYPE * ptr() const { return reinterpret_cast<const TYPE*>(m.ptr()); }
		AlignedMemoryBlock() {}
		AlignedMemoryBlock(self const & other) : m(other.m) {}
		AlignedMemoryBlock(self && other) : m(std::move(other.m)) {}
		self const & operator=(self const & other) { m = other.m; return *this; }
		self const & operator=(self && other) { m = std::move(other.m); return *this; }
	private:
		AlignedMemoryBlock_t<ALIGN> m;
	};
}