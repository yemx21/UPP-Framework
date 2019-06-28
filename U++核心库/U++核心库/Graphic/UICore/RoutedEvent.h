#pragma once
#include "DependantObject.h"
__uimport("EventManager.h")

namespace System
{
	namespace UI
	{
		__uenum __uobjdef(asm = UICore, ns = System::UI, ver = 1.0) UICORE_API EventStrategy : __uobject
		{
			__uvalue_begin(EventStrategy, UInt)
			__uvalue(Tunnel)
			__uvalue(Bubble)
			__uvalue(Direct)
			__uvalue_end(EventStrategy, Tunnel)
		};

		class EventManager;
		class EventManager_impl;
		class IRoutedEventDelegate;
		typedef std::shared_ptr<IRoutedEventDelegate> IRoutedEventDelegatePtr;

		__uclass __uobjdef(asm = UICore, ns = System::UI, ver = 1.0) UICORE_API RoutedEventArgs : public EventArgs, __uobject
		{
		private:
			RoutedEventPtr _dependencyEvent;
			bool _isHandled;
			DependencyObjectPtr _source;
			DependencyObjectPtr _originalSource;
		protected:
			bool _isInvoking;
		public:
			bool GetIsHandled() const;

			void SetIsHandled(bool val);

			DependencyObjectPtr GetOriginalSource() const;

			RoutedEventPtr GetRoutedEvent() const;

			void SetRoutedEvent(RoutedEventPtr DEvent);

			DependencyObjectPtr GetSource() const;

			void SetSource(DependencyObjectPtr value);

			__udefctor RoutedEventArgs();
			__uctor RoutedEventArgs(RoutedEventPtr dEvent);
			__uctor RoutedEventArgs(RoutedEventPtr dEvent, DependencyObjectPtr source);

			__uproperty(get = GetIsHandled, put = SetIsHandled) bool IsHandled;
			__uproperty(get = GetOriginalSource) DependencyObjectPtr OriginalSource;
			__uproperty(get = GetSource, put = SetSource) DependencyObjectPtr Source;
			__uproperty(get = GetRoutedEvent, put = SetRoutedEvent) RoutedEventPtr Event;
		protected:
			friend class DependencyObject;
			virtual void InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender);

			void InvokeHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender);

			virtual void OnSetSource(DependencyObjectPtr source);

			REFLECT_CLASS(RoutedEventArgs);
		};

		class RoutedEvent;

		class UICORE_API IRoutedEvent
		{
		private:
			friend RoutedEvent;
			friend EventManager_impl;
			InertiaString _name;
			EventStrategy _eventStrategy;
			const Type* _handlerType;
			const Type* _ownerType;
			size_t _globalIndex;

			IRoutedEvent(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType);
		public:
			~IRoutedEvent() = default;

			const Type* GetHandlerType() const;

			const wchar_t* GetName() const;

			const Type* GetOwnerType() const;

			System::UI::EventStrategy GetEventStrategy() const;

			_declspec(property(get = GetHandlerType)) const Type* HandlerType;
			_declspec(property(get = GetName)) const wchar_t* Name;
			_declspec(property(get = GetOwnerType)) const Type* OwnerType;
			_declspec(property(get = GetEventStrategy)) System::UI::EventStrategy EventStrategy;

		};


		class UICORE_API RoutedEvent
		{
		private:
			typedef void(*fun__ustatic_ctor_impl)();
			fun__ustatic_ctor_impl fun_ustaticdp;
			std::shared_ptr<IRoutedEvent> impl;
		public:
			RoutedEvent(fun__ustatic_ctor_impl fun);
		protected:
			friend DependencyObject;
			friend EventManager_impl;
		public:			
			void Register1(const std::wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType, const Type* inheritType);
			void Register1(const std::wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType);
		public:
			template <class T, class IT, typename TEventArgs>
			void Register(const std::wstring& name, EventStrategy eventStrategy)
			{
				static_assert(is_ustatic_constructor_supported<T>::value, "__usctordef is not defined in T");
				static_assert(System::is_same_or_base_of<DependencyObject, IT>::value, "IT must be same as or based DependencyObject");
				static_assert(System::is_same_or_base_of<RoutedEventArgs, TEventArgs>::value, "TEventArgs must be same as or based on RoutedEventArgs");
				return Register1(name, eventStrategy, TypeFactory::Create<TEventArgs>(), TypeFactory::Create<T>(), TypeFactory::Create<IT>());
			}

			template <class T, typename TEventArgs>
			void Register(const std::wstring& name, EventStrategy eventStrategy)
			{
				static_assert(is_ustatic_constructor_supported<T>::value, "__usctordef is not defined in T");
				static_assert(System::is_same_or_base_of<RoutedEventArgs, TEventArgs>::value, "TEventArgs must be same as or based on RoutedEventArgs");
				return Register1(name, eventStrategy, TypeFactory::Create<TEventArgs>(), TypeFactory::Create<T>());
			}

			std::shared_ptr<IRoutedEvent> operator ->();
		};

		template<class T>
		class RoutedEventCore : public RoutedEvent
		{
		public:
			RoutedEventCore() :RoutedEvent(&T::__ustatic_ctor_impl)
			{

			}
		};

		template<class T, class... _Types>
		inline RoutedEventPtr make_routedevent(_Types&&... _Args)
		{
			std::_Ref_count_obj<RoutedEventCore<T>> *_Rx =
				new std::_Ref_count_obj<RoutedEventCore<T>>(std::forward<_Types>(_Args)...);

			RoutedEventPtr _Ret;
			_Ret._Resetp0(_Rx->_Getptr(), _Rx);
			return (_Ret);
		}
	}
}