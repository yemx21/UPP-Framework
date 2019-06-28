#pragma once
#include "UICore_Config.h"
#include <vector>

using namespace std;

namespace System
{
	namespace UI
	{
		class DependantValueEntry;
		class DependencyObject;

		struct IInvalidatable;
		typedef weak_ptr<IInvalidatable>   IInvalidatableWeakPtr;
		typedef shared_ptr<IInvalidatable> IInvalidatablePtr;


		__uenum __uobjdef(asm = UICore, ns = System::UI, ver = 1.0) UICORE_API InvalidateFlags : __uobject
		{
			__uvalue_begin(InvalidateFlags, UInt)
			__uvalue(Measure)
			__uvalue(Arrange)
			__uvalue(Visual)
			__uvalue(Transform)
			__uvalue_end(InvalidateFlags, Measure)
		};

		struct IInvalidatable
		{
			virtual void Invalidate(InvalidateFlags flags) = 0;
		};

		class DependantObject;
		typedef std::shared_ptr<DependantObject> DependantObjectPtr;

		class UICORE_API DependantObject
		{
		public:
			DependantObject();
			virtual ~DependantObject();

			std::wstring GetName() const;

			void SetName(const std::wstring& name);

			_declspec(property(get = GetName, put = SetName)) std::wstring Name;

		protected:
			void NotifyInvalidatables(InvalidateFlags flags);

		private:
			void RegisterInvalidatable(IInvalidatable* invalidatable);
			void UnregisterInvalidatable(IInvalidatable* invalidatable);

		private:
			InertiaString name;
			std::vector<IInvalidatable*>  invalidatables;
			friend DependencyObject;
			friend DependantValueEntry;
		};

		class DependencyProperty;
		typedef std::shared_ptr<DependencyObject> DependencyObjectPtr;

		template<class T>
		class DependencyPropertyCore;

		class IRoutedEvent;
		typedef std::shared_ptr<IRoutedEvent> IRoutedEventPtr;
		class RoutedEvent;

		class RoutedEventArgs;

		class UICORE_API RoutedEventPtr : public std::shared_ptr<RoutedEvent>
		{
		public:
			std::shared_ptr<IRoutedEvent> operator->() const noexcept;
		};
	}
}

#define __usctordef(...) private:\
static bool __ustatic_ctor_called;\
static bool __ustatic_ctor();\
protected:\
template<class T>\
friend class System::UI::DependencyPropertyCore;\
friend System::UI::DependencyProperty;\
template<class T>\
friend class System::UI::RoutedEventCore;\
friend System::UI::RoutedEvent;\
friend System::UI::has_usctor<__VA_ARGS__>;\
friend System::UI::has_usctorimpl<__VA_ARGS__>;\
friend System::UI::has_usctorcalled<__VA_ARGS__>;\
static void __ustatic_ctor_impl(){\
if(__ustatic_ctor_called) return;\
__ustatic_ctor_called = true;\
if(!__ustatic_ctor()) __ustatic_ctor_called = false;}

#define __usctor(CLASS)\
bool CLASS::__ustatic_ctor_called = false;\
bool CLASS::__ustatic_ctor()


namespace System
{
	namespace UI
	{
		template <typename U>
		struct has_usctor
		{
			template<typename T, T> struct helper;
			template<typename T>
			static std::uint8_t check(helper<bool(*)(void), &T::__ustatic_ctor>*);
			template<typename T> static std::uint16_t check(...);
			static const bool value = sizeof(check<U>(0)) == sizeof(std::uint8_t);
		};

		template <typename U>
		struct has_usctorimpl
		{
			template<typename T, T> struct helper;
			template<typename T>
			static std::uint8_t check(helper<void(*)(void), &T::__ustatic_ctor_impl>*);
			template<typename T> static std::uint16_t check(...);
			static const bool value = sizeof(check<U>(0)) == sizeof(std::uint8_t);
		};

		template<typename T>
		struct has_usctorcalled
		{
			struct Fallback { bool __ustatic_ctor_called; };
			struct Derived : T, Fallback { };

			template<typename C, C> struct ChT;

			template<typename C> static char(&f(ChT<bool Fallback::*, &C::__ustatic_ctor_called>*))[1];
			template<typename C> static char(&f(...))[2];

			static bool const value = sizeof(f<Derived>(0)) == 2;
		};

		template <typename T>
		struct is_ustatic_constructor_supported
		{
			static const bool value = has_usctor<T>::value && has_usctorimpl<T>::value && has_usctorcalled<T>::value;
		};
	}
}

namespace System
{
	__uenum_flag(System::UI::InvalidateFlags)
}