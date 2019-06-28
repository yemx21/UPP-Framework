#pragma once

#include <iterator>
#include <exception>
namespace System
{
	template< class T >
	struct DynamicArray
	{
		typedef       T                               value_type;
		typedef       T&                              reference;
		typedef const T&                              const_reference;
		typedef       T*                              iterator;
		typedef const T*                              const_iterator;
		typedef std::reverse_iterator<iterator>       reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef size_t                                size_type;
		typedef ptrdiff_t                             difference_type;

	private:
		T*        store;
		size_type count;

		void check(size_type n)
		{
			if (n >= count) throw std::out_of_range("DynamicArray");
		}

		T* alloc(size_type n)
		{
			if (n > (std::numeric_limits<size_type>::max()) / sizeof(T))
				throw std::bad_array_new_length();
			return reinterpret_cast<T*>(new char[n*sizeof(T)]);
		}

	public:
		DynamicArray() = delete;
		const DynamicArray operator=(const DynamicArray&) = delete;

		explicit DynamicArray(size_type c)
			: store(alloc(c)), count(c)
		{
			size_type i;
			try {
				for (size_type i = 0; i < count; ++i)
					new (store + i) T;
			}
			catch (...) {
				for (; i > 0; --i)
					(store + (i - 1))->~T();
				throw;
			}
		}

		DynamicArray(const DynamicArray& d) 
			: store(alloc(d.count)), count(d.count)
		{
			try { uninitialized_copy(d.begin(), d.end(), begin()); }
			catch (...) { delete store; throw; }
		}

		~DynamicArray()
		{
			for (size_type i = 0; i < count; ++i)
				(store + i)->~T();
			delete[] store;
		}

		iterator       begin() { return store; }
		const_iterator begin()  const { return store; }
		const_iterator cbegin() const { return store; }
		iterator       end() { return store + count; }
		const_iterator end()    const { return store + count; }
		const_iterator cend()   const { return store + count; }

		reverse_iterator       rbegin()
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin()  const
		{
			return reverse_iterator(end());
		}
		reverse_iterator       rend()
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend()    const
		{
			return reverse_iterator(begin());
		}

		size_type size()     const { return count; }
		size_type max_size() const { return count; }
		bool      empty()    const { return count == 0; }

		reference       operator[](size_type n) { return store[n]; }
		const_reference operator[](size_type n) const { return store[n]; }

		reference       front() { return store[0]; }
		const_reference front() const { return store[0]; }
		reference       back() { return store[count - 1]; }
		const_reference back()  const { return store[count - 1]; }

		const_reference at(size_type n) const { check(n); return store[n]; }
		reference       at(size_type n) { check(n); return store[n]; }

		T*       data() { return store; }
		const T* data() const { return store; }
	};
}
