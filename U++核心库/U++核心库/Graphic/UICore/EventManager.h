#pragma once
#include "DependencyObject.h"
#include "RoutedEvent.h"
namespace System
{
	namespace UI
	{
		class RoutedEventHandlerInfoList;

		class UICORE_API EventManager final
		{
		private:
			friend class EventManager_impl;
			EventManager_impl* impl;
			EventManager();
		protected:
			friend DependencyObject;
			friend RoutedEvent;

			static RoutedEventHandlerInfoList* GetClassEventDelegates(DependencyObjectTypePtr, RoutedEventPtr);

			static IRoutedEvent* RegisterRoutedEvent(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType);

			static IRoutedEvent* RegisterRoutedEventEx(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType, const Type* inheritType);

		public:
			~EventManager();

			static EventToken RegisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, IRoutedEventDelegatePtr eventDelegate);

			static void UnregisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, EventToken token);

			template<class T, class TEventArgs>
			static typename std::enable_if<System::is_same_or_base_of<RoutedEventArgs, TEventArgs>::value, EventToken>::type RegisterClassRoutedEvent(RoutedEventPtr dEvent, std::function<void(DependencyObjectPtr, TEventArgs&)> callBack, bool ignoreIsHandled)
			{
				return RegisterClassRoutedEvent(TypeFactory::Create<T>(), dEvent, std::make_shared<RoutedEventDelegate<TEventArgs>>(callBack, ignoreIsHandled));
			}

		};
	}
}