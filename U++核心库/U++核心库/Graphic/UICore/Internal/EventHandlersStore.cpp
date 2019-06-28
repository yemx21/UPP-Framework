#include "EventHandlersStore.h"
#include "EventManager_impl.h"
using namespace System::UI;

EventPrivateKey::EventPrivateKey()
{
	GlobalIndex = EventManager_impl::GetNextAvailableGlobalIndex(this);
}

EventHandlersStore::EventHandlersStore()
{
}

EventHandlersStore::EventHandlersStore(const EventHandlersStore& source)
{
	_entries = source._entries;
}

void EventHandlersStore::Add(EventPrivateKeyPtr key, MulticastRoutedEventDelegatePtr handler)
{
	if (!key || !handler) return;

	MulticastRoutedEventDelegatePtr existingDelegate = Get(key);

	if (!existingDelegate)
	{
		_entries[key->GlobalIndex] = handler;
	}
	else
	{
		existingDelegate->Combine(handler);
		//_entries[key->GlobalIndex] =  exi Delegate.Combine(existingDelegate, handler);
	}
}

void EventHandlersStore::Remove(EventPrivateKeyPtr key, MulticastRoutedEventDelegatePtr handler)
{
	if (!key || !handler) return;

	MulticastRoutedEventDelegatePtr existingDelegate = Get(key);
	if (existingDelegate)
	{
		existingDelegate->Remove(handler);
		if (existingDelegate->IsEmpty())
		{
			_entries[key->GlobalIndex] = nullptr;// DependencyProperty.UnsetValue;
		}
		else
		{
			_entries[key->GlobalIndex] = existingDelegate;
		}
	}
}

MulticastRoutedEventDelegatePtr EventHandlersStore::Get(EventPrivateKeyPtr key)
{
	if (!key) return nullptr;
	const Any& existingDelegate = _entries[key->GlobalIndex];
	if (existingDelegate.IsNull)
	{
		return nullptr;
	}
	return existingDelegate.as<MulticastRoutedEventDelegatePtr>();
}

std::vector<RoutedEventHandlerInfoPtr>* EventHandlersStore::Get(RoutedEventPtr key)
{
	if (!key) return nullptr;
	const Any& list = _entries[EventManager_impl::GetGlobalIndex(key)];
	if (list.IsNull)
	{
		return nullptr;
	}
	return list.as<std::vector<RoutedEventHandlerInfoPtr>*>();
}

void EventHandlersStore::AddRoutedEventHandler(RoutedEventPtr routedEvent, MulticastRoutedEventDelegatePtr handler, bool handledEventsToo)
{
	if (!routedEvent || !handler) return;

	RoutedEventHandlerInfoPtr routedEventHandlerInfo = std::make_shared<RoutedEventHandlerInfo>(handler, handledEventsToo);

	std::vector<RoutedEventHandlerInfoPtr>* handlers = Get(routedEvent);
	if (!handlers)
	{
		handlers = new std::vector<RoutedEventHandlerInfoPtr>();
		_entries[EventManager_impl::GetGlobalIndex(routedEvent)] = handlers;
	}

	// Add the RoutedEventHandlerInfo to the list
	handlers->push_back(routedEventHandlerInfo);
}

void EventHandlersStore::RemoveRoutedEventHandler(RoutedEventPtr routedEvent, MulticastRoutedEventDelegatePtr handler)
{
	if (!routedEvent || !handler) return;

	std::vector<RoutedEventHandlerInfoPtr>* handlers = Get(routedEvent);
	if (handlers && !handlers->empty())
	{
		if ((handlers->size() == 1) && (handlers->at(0)->Handler == handler))
		{
			_entries[EventManager_impl::GetGlobalIndex(routedEvent)] = nullptr;// DependencyProperty.UnsetValue;
		}
		else
		{
			for (auto iter = handlers->begin(); iter != handlers->end();)
			{
				auto infoptr = (*iter);
				if (infoptr->Handler == handler)
				{
					handlers->erase(iter);
					break;
				}
			}
		}
	}
}

bool EventHandlersStore::Contains(RoutedEventPtr routedEvent)
{
	if (!routedEvent) return false;
	std::vector<RoutedEventHandlerInfoPtr>* handlers = Get(routedEvent);
	return handlers && !handlers->empty();
}