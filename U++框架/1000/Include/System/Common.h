#pragma once
#include "KeyWords.h"
#include <type_traits>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <unordered_set>
#include <valarray>
#include <forward_list>
#include <list>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>
#include <memory>


namespace System
{
	typedef std::vector<std::wstring> wstringvector;

	struct null_deleter {
		void operator()(void const *) const {}
	};

	/// <summary>
	/// 缺失类型
	/// </summary>
	const class _missing_t
	{
	public:
		template<class T>
		inline operator T*() const
		{
			return 0;
		}

		template<class C, class T>
		inline operator T C::*() const
		{
			return 0;
		}

	private:
		void operator&() const;
	}_missing = {};

	/// <summary>
	/// 无默认值类型
	/// </summary>
	const class _nodefault_t
	{
	public:
		template<class T>
		inline operator T*() const
		{
			return 0;
		}

		template<class C, class T>
		inline operator T C::*() const
		{
			return 0;
		}

	private:
		void operator&() const;
	}_nodefault = {};

	template<class T>
	struct remove_shared_ptr
	{
		typedef T type;
	};

	template<class T>
	struct remove_shared_ptr<std::shared_ptr<T>>
	{
		typedef T type;
	};

	template<class T>
	struct remove_weak_ptr
	{
		typedef T type;
	};

	template<class T>
	struct remove_weak_ptr<std::weak_ptr<T>>
	{
		typedef T type;
	};

	template<class T>
	struct remove_unique_ptr
	{
		typedef T type;
	};

	template<class T>
	struct remove_unique_ptr<std::unique_ptr<T>>
	{
		typedef T type;
	};

	template<class T>
	struct remove_ptrbase
	{
		typedef typename remove_weak_ptr<typename remove_unique_ptr<typename remove_shared_ptr<T>::type>::type>::type type;
	};


	template<class T>
	struct deduce
	{
		typedef typename std::remove_cv<typename std::remove_pointer<typename std::remove_pointer<typename std::decay<T>::type>::type>::type>::type type;
	};

	/// <summary>
	/// 修饰符
	/// </summary>
	enum class Modifiers : unsigned char
	{
		None,
		Const,
		Volatile,
		ConstVolatile,
	};

	/// <summary>
	/// 申明
	/// </summary>
	enum class Declarators : unsigned char
	{
		Value,
		Pointer,
		LValueReference,
		RValueReference
	};

	template<class T>
	struct remove_md
	{
		typedef typename std::remove_reference<typename deduce<T>::type>::type type;
	};

	/// <summary>
	/// 容器类型
	/// </summary>
	enum class ContainerTypes
	{
		Unknown,
		Array,
		STL_array,
		STL_valarray,
		STL_vector,
		STL_deque,
		STL_forward_list,
		STL_list,
		STL_set,
		STL_multiset,
		STL_unordered_set,
		STL_unordered_multiset,
		STL_map,
		STL_multimap,
		STL_unordered_map,
		STL_unordered_multimap,
		STL_stack,
		STL_queue,
		STL_priority_queue,
		ObservableCollection,
	};

	template <typename> struct is_template : std::false_type {};

	template <template <typename...> class Tmpl, typename ...Args>
	struct is_template<Tmpl<Args...>> : std::true_type{};

	template <typename C, typename F, typename = void>
	struct is_call_possible : public std::false_type {};

	template <typename C, typename R, typename... A>
	struct is_call_possible<C, R(A...),
		typename std::enable_if<
		std::is_same<R, void>::value ||
		std::is_convertible<decltype(
			std::declval<C>().operator()(std::declval<A>()...)
			), R>::value
		>::type
	> : public std::true_type {};

	template<typename _Base, typename _Der>
	struct is_same_or_base_of
	{
		static const bool value = std::is_same<_Base, _Der>::value || std::is_base_of < _Base, _Der >::value;
	};

	template<typename T>
	struct has_const_iterator
	{
	private:
		typedef char                      yes;
		typedef struct { char array[2]; } no;

		template<typename C> static yes test(typename C::const_iterator*);
		template<typename C> static no  test(...);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(yes);
		typedef T type;
	};

	template<class Container>
	auto begin(Container &&c) -> decltype(c.begin()) { return c.begin(); }

	template<class Container>
	auto end(Container &&c) -> decltype(c.end()) { return c.end(); }

	template<class T, size_t size>
	T *begin(T(&array)[size]) { return (&array[0]); }

	template<class T, size_t size>
	T *end(T(&array)[size]) { return (&array[0] + size); }

	template <typename T>
	struct has_begin_end
	{
		typedef char true_type;
		typedef char false_type[2];

		template <typename U> static true_type& test(decltype(begin(*((U*)0))) *b = 0,
			decltype(end(*((U*)0))) *e = 0);

		template <typename U> static false_type& test(...);

		enum { value = (sizeof(true_type) == sizeof test<T>(0)) };
	};

	template<typename T>
	struct is_container : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::value>
	{
	};

	template<typename T, std::size_t N> struct is_container<T[N]> : public ::std::true_type{};

	template<typename T, std::size_t N1, std::size_t N2> struct is_container<T[N1][N2]> : public ::std::true_type{};

	template<typename T, std::size_t N1, std::size_t N2, std::size_t N3> struct is_container<T[N1][N2][N3]> : public ::std::true_type{};

	template<typename T, std::size_t N1, std::size_t N2, std::size_t N3, std::size_t N4> struct is_container<T[N1][N2][N3][N4]> : public ::std::true_type{};

	template<typename T, std::size_t N1, std::size_t N2, std::size_t N3, std::size_t N4, std::size_t N5> struct is_container<T[N1][N2][N3][N4][N5]> : public ::std::true_type{};


	template<std::size_t N> struct is_container<char[N]> : public ::std::false_type{};

	template <typename T> struct is_container<std::valarray<T>> : public ::std::true_type{};

	template<typename T>
	struct is_array
	{
		typedef char(&yes)[2];
		template<typename U, size_t Size>
		static yes check(std::array<U, Size>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_valarray
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::valarray<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_vector
	{
		typedef char(&yes)[2];
		template<typename U, typename A>
		static yes check(std::vector<U, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_deque
	{
		typedef char(&yes)[2];
		template<typename U, typename A>
		static yes check(std::deque<U, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_forward_list
	{
		typedef char(&yes)[2];
		template<typename U, typename A>
		static yes check(std::forward_list<U, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_list
	{
		typedef char(&yes)[2];
		template<typename U, typename A>
		static yes check(std::list<U, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_set
	{
		typedef char(&yes)[2];
		template<typename U, typename K, typename A>
		static yes check(std::set<U, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_multiset
	{
		typedef char(&yes)[2];
		template<typename U, typename K, typename A>
		static yes check(std::multiset<U, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_unordered_set
	{
		typedef char(&yes)[2];
		template<typename U, typename H, typename K, typename A>
		static yes check(std::unordered_set<U, H, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_unordered_multiset
	{
		typedef char(&yes)[2];
		template<typename U, typename H, typename K, typename A>
		static yes check(std::unordered_multiset<U, H, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_map
	{
		typedef char(&yes)[2];
		template<typename U, typename K, typename A>
		static yes check(std::map<U, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_multimap
	{
		typedef char(&yes)[2];
		template<typename U, typename K, typename A>
		static yes check(std::multimap<U, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_unordered_map
	{
		typedef char(&yes)[2];
		template<typename U, typename H, typename K, typename A>
		static yes check(std::unordered_map<U, H, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_unordered_multimap
	{
		typedef char(&yes)[2];
		template<typename U, typename H, typename K, typename A>
		static yes check(std::unordered_multimap<U, H, K, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};


	template<typename T>
	struct is_stack
	{
		typedef char(&yes)[2];
		template<typename U,  typename A>
		static yes check(std::stack<U, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_queue
	{
		typedef char(&yes)[2];
		template<typename U, typename A>
		static yes check(std::queue<U, A>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_priority_queue
	{
		typedef char(&yes)[2];
		template<typename U, typename A, typename C>
		static yes check(std::priority_queue<U, A, C>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

#if !_HAS_CXX17	
	/// <summary>
	/// 内存指针类型
	/// </summary>
	enum class MemoryPtrTypes
	{
		Unknown,
		SharedPtr,
		WeakPtr,
		UniquePtr,
		AutoPtr,
	};

	template<typename T>
	struct is_shared_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::shared_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_weak_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::weak_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_unique_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::unique_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_auto_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::auto_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};
	template<typename T>
	struct is_memory_ptr
	{
		typedef typename std::remove_reference<typename std::remove_pointer<typename std::remove_pointer<T>::type>::type>::type base;
		static const bool isShared = is_shared_ptr<base>::value;
		static const bool isWeak = is_weak_ptr<base>::value;
		static const bool isUnique = is_unique_ptr<base>::value;
		static const bool isAuto = is_auto_ptr<base>::value;
	public:
		static const bool value = isShared || isWeak || isUnique || isAuto;
	};

	template<bool SHARED, bool WEAK, bool UNIQUE, bool AUTO>
	struct DetectMemoryPtr_impl
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::Unknown;
	};

	template<>
	struct DetectMemoryPtr_impl<true, false, false, false>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::SharedPtr;
	};

	template<>
	struct DetectMemoryPtr_impl<false, true, false, false>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::WeakPtr;
	};

	template<>
	struct DetectMemoryPtr_impl< false, false, true, false>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::UniquePtr;
	};

	template<>
	struct DetectMemoryPtr_impl<false, false, false, true>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::AutoPtr;
	};

	/// <summary>
	/// 内存指针检测类
	/// </summary>
	template<typename T>
	struct DetectMemoryPtr
	{
		typedef typename std::remove_reference<typename std::remove_pointer<typename std::remove_pointer<T>::type>::type>::type base;
	public:
		static const bool isShared = is_shared_ptr<base>::value;
		static const bool isWeak = is_weak_ptr<base>::value;
		static const bool isUnique = is_unique_ptr<base>::value;
		static const bool isAuto = is_auto_ptr<base>::value;
		static const MemoryPtrTypes value = DetectMemoryPtr_impl<isShared, isWeak, isUnique, isAuto>::value;
	};

#else
	/// <summary>
	/// 内存指针类型
	/// </summary>
	enum class MemoryPtrTypes
	{
		Unknown,
		SharedPtr,
		WeakPtr,
		UniquePtr,
	};

	template<typename T>
	struct is_shared_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::shared_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_weak_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::weak_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_unique_ptr
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(std::unique_ptr<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_memory_ptr
	{
		typedef typename std::remove_reference<typename std::remove_pointer<typename std::remove_pointer<T>::type>::type>::type base;
		static const bool isShared = is_shared_ptr<base>::value;
		static const bool isWeak = is_weak_ptr<base>::value;
		static const bool isUnique = is_unique_ptr<base>::value;
	public:
		static const bool value = isShared || isWeak || isUnique;
	};

	template<bool SHARED, bool WEAK, bool UNIQUE>
	struct DetectMemoryPtr_impl
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::Unknown;
	};

	template<>
	struct DetectMemoryPtr_impl<true, false, false>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::SharedPtr;
	};

	template<>
	struct DetectMemoryPtr_impl<false, true, false>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::WeakPtr;
	};

	template<>
	struct DetectMemoryPtr_impl< false, false, true>
	{
		static const MemoryPtrTypes value = MemoryPtrTypes::UniquePtr;
	};

	/// <summary>
	/// 内存指针检测类
	/// </summary>
	template<typename T>
	struct DetectMemoryPtr
	{
		typedef typename std::remove_reference<typename std::remove_pointer<typename std::remove_pointer<T>::type>::type>::type base;
	public:
		static const bool isShared = is_shared_ptr<base>::value;
		static const bool isWeak = is_weak_ptr<base>::value;
		static const bool isUnique = is_unique_ptr<base>::value;
		static const MemoryPtrTypes value = DetectMemoryPtr_impl<isShared, isWeak, isUnique>::value;
	};

#endif

	template< bool C_ > struct bool_
	{
		static const bool value = C_;
		typedef bool_ type;
		typedef bool value_type;
		operator bool() const { return this->value; }
	};

	template<class T>
	class ObservableObject;

	template<typename T>
	struct is_iobservable
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(ObservableObject<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	typedef std::shared_ptr<void> SharedVoid;

	template<class _Ty, bool isObservable = is_iobservable<typename remove_shared_ptr<_Ty>::type>::value, bool isshared = is_shared_ptr<_Ty>::value>
	class ObservableCollection;


	template<typename T>
	struct is_observable_collection
	{
		typedef char(&yes)[2];
		template<typename U>
		static yes check(ObservableCollection<U>*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_std_string
	{
		typedef char(&yes)[2];
		template<>
		static yes check(std::string*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<typename T>
	struct is_std_wstring
	{
		typedef char(&yes)[2];
		template<>
		static yes check(std::wstring*);
		static char check(...);

		static const bool value = (sizeof(check((T*)0)) == sizeof(yes));
	};

	template<bool ARRAY, bool STDARRAY, bool STDVALARRAY, bool STDVECTOR, bool STDDEQUE, bool STDFORWARDLIST, bool STDLIST,
		bool STDSET, bool STDMULTISET, bool STDUNORDEREDSET, bool STDUNORDEREDMULTISET,
		bool STDMAP, bool STDMULTIMAP, bool STDUNORDEREDMAP, bool STDUNORDEREDMULTIMAP, 
	bool STDSTACK, bool STDQUEUE, bool STDPRIORITY, bool OBSERVABLECOLLECTION>
	struct DetectSTLContainer_impl
	{
		static const ContainerTypes value = ContainerTypes::Unknown;
	};

	template<>
	struct DetectSTLContainer_impl<true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::Array;
	};

	template<>
	struct DetectSTLContainer_impl<false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_array;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_valarray;
	};

	template<>
	struct DetectSTLContainer_impl< false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_vector;
	};

	template<>
	struct DetectSTLContainer_impl< false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_deque;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_forward_list;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_list;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_set;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_multiset;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_unordered_set;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_unordered_multiset;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_map;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_multimap;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_unordered_map;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_unordered_multimap;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_stack;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_queue;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false>
	{
		static const ContainerTypes value = ContainerTypes::STL_priority_queue;
	};

	template<>
	struct DetectSTLContainer_impl<false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true>
	{
		static const ContainerTypes value = ContainerTypes::ObservableCollection;
	};


	/// <summary>
	/// 容器检测类
	/// </summary>
	template<typename T>
	struct DetectSTLContainer
	{
		typedef typename std::remove_reference<typename std::remove_pointer<typename std::remove_pointer<T>::type>::type>::type base;
	public:
		static const bool isArray = std::is_array<base>::value;
		static const bool is_array = System::is_array<base>::value;
		static const bool is_valarray = System::is_valarray<base>::value;
		static const bool is_vector = System::is_vector<base>::value;
		static const bool is_deque = System::is_deque<base>::value;
		static const bool is_forward_list = System::is_forward_list<base>::value;
		static const bool is_list = System::is_list<base>::value;
		static const bool is_set = System::is_set<base>::value;
		static const bool is_multiset = System::is_multiset<base>::value;
		static const bool is_unordered_set = System::is_unordered_set<base>::value;
		static const bool is_unordered_multiset = System::is_unordered_multiset<base>::value;
		static const bool is_map = System::is_map<base>::value;
		static const bool is_multimap = System::is_multimap<base>::value;
		static const bool is_unordered_map = System::is_unordered_map<base>::value;
		static const bool is_unordered_multimap = System::is_unordered_multimap<base>::value;
		static const bool is_stack = System::is_stack<base>::value;
		static const bool is_queue = System::is_queue<base>::value;
		static const bool is_priority_queue = System::is_priority_queue<base>::value;
		static const bool is_observable_collection = System::is_observable_collection<base>::value;
		static const ContainerTypes value = DetectSTLContainer_impl<isArray, is_array, is_valarray, is_vector, is_deque, is_forward_list, is_list, is_set, is_multiset, is_unordered_set, is_unordered_multiset, is_map, is_multimap, is_unordered_map, is_unordered_multimap, is_stack, is_queue, is_priority_queue, is_observable_collection>::value;
	};	
}