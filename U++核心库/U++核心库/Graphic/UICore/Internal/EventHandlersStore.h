#pragma once
#include "..\DependencyObject.h"
#include "RoutedEventHandlerInfo.h"
#include "MulticastRoutedEventDelegate.h"
namespace System
{
	namespace UI
	{
		class EventPrivateKey
		{
		public:
			int GlobalIndex;
			EventPrivateKey();
		};
		typedef std::shared_ptr<EventPrivateKey> EventPrivateKeyPtr;

		class EventHandlersStore
		{
		public:
			std::unordered_map<int, Any> _entries;

			EventHandlersStore();

			EventHandlersStore(const EventHandlersStore& source);

			void Add(EventPrivateKeyPtr key, MulticastRoutedEventDelegatePtr handler);

			void Remove(EventPrivateKeyPtr key, MulticastRoutedEventDelegatePtr handler);

			MulticastRoutedEventDelegatePtr Get(EventPrivateKeyPtr key);

			std::vector<RoutedEventHandlerInfoPtr>* Get(RoutedEventPtr key);

			void AddRoutedEventHandler(RoutedEventPtr routedEvent, MulticastRoutedEventDelegatePtr handler, bool handledEventsToo);

			void RemoveRoutedEventHandler(RoutedEventPtr routedEvent, MulticastRoutedEventDelegatePtr handler);

			bool Contains(RoutedEventPtr routedEvent);

			/*static void OnEventHandlersIterationCallback(ArrayList list, int key, object value)
			{
				RoutedEvent routedEvent = GlobalEventManager.EventFromGlobalIndex(key) as RoutedEvent;
				if (routedEvent != null && ((FrugalObjectList<RoutedEventHandlerInfo>)value).Count > 0)
				{
					list.Add(routedEvent);
				}
			}*/

			//static FrugalMapIterationCallback _iterationCallback = new FrugalMapIterationCallback(OnEventHandlersIterationCallback);

		};
	}
}
