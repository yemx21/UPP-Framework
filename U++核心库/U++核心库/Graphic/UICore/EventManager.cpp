#include "EventManager.h"
#include "Windows\UIElement.h"
#include "Internal\RoutedEventHandlerInfo.h"
#include "Internal\EventManager_impl.h"
#include <map>
using namespace std;
using namespace System;
using namespace System::UI;

namespace System
{
	namespace UI
	{
		class f1fqfa : public std::unary_function < IRoutedEvent*, bool >
		{
		private:
			const wchar_t* name;
			System::UI::EventStrategy strategy;
			const Type* ot;
			const Type* ht;
		public:
			explicit f1fqfa(const wchar_t*  str, System::UI::EventStrategy es, const Type* h, const Type* o) : name(str), strategy(es), ot(o), ht(h)
			{
			}

			bool operator() (IRoutedEvent* info)
			{
				try
				{
					if (!info) return false;
					if (wcscmp(info->Name, name) != 0) return false;
					if (info->EventStrategy != strategy) return false;
					if (info->OwnerType != ot)return false;
					return info->HandlerType == ht;
				}
				catch (...)
				{
					return false;
				}
			}
		};

		class vwrw : public std::unary_function < IRoutedEvent*, bool >
		{
		private:
			const wchar_t* name;
			System::UI::EventStrategy strategy;
			const Type* ot;
		public:
			explicit vwrw(const wchar_t*  str, System::UI::EventStrategy es, const Type* o) : name(str), strategy(es), ot(o)
			{
			}

			bool operator() (IRoutedEvent* info)
			{
				try
				{
					if (!info) return false;
					if (wcscmp(info->Name, name) != 0) return false;
					if (info->EventStrategy != strategy) return false;
					return info->OwnerType == ot;
				}
				catch (...)
				{
					return false;
				}
			}
		};

		class vg4wv : public std::unary_function < RoutedEventList*, bool >
		{
		private:
			RoutedEventPtr DEvent;
		public:
			explicit vg4wv(RoutedEventPtr dEvent) : DEvent(dEvent)
			{
			}

			bool operator() (RoutedEventList* info)
			{
				try
				{
					if (!info) return false;
					return info->DEvent == DEvent;
				}
				catch (...)
				{
					return false;
				}
			}
		};

		#pragma region DTypeMap

		DTypeMap::DTypeMap(int entryCount):_entryCount(entryCount)
		{
			_entries.resize(entryCount, nullptr);
		}

		ClassHandlersStore* DTypeMap::Get(DependencyObjectTypePtr dType)
		{
			if (dType->Id < _entryCount)
			{
				return _entries[dType->Id];
			}
			else
			{
				if (!_overFlow.empty())
				{
					return _overFlow[dType];
				}

				return nullptr;
			}
		}

		void DTypeMap::Set(DependencyObjectTypePtr dType, ClassHandlersStore* value)
		{
			if (dType->Id < _entryCount)
			{
				_entries[dType->Id] = value;
			}
			else
			{
				_overFlow[dType] = value;
			}

			_activeDTypes.push_back(dType);
		}

		void DTypeMap::Clear()
		{
			_entries.clear();

			_activeDTypes.clear();

			_overFlow.clear();
		}
		#pragma endregion

		#pragma region EventManager_impl

		mutex EventManager_impl::singletonlocker;
		EventManager* EventManager_impl::mgr = EventManager_impl::Create();

		EventManager* EventManager_impl::Create()
		{
			lock_guard<mutex> lock(singletonlocker);
			static EventManager _mgr;
			return &_mgr;
		}

		int EventManager_impl::GetNextAvailableGlobalIndex(const Any& value)
		{
			lock_guard<CriticalSection> lock(EventManager_impl::mgr->impl->locker);
			if (EventManager_impl::mgr->impl->_globalIndexToEventMap.size() >= INT_MAX)
			{
				throw std::exception("too many routed events");
			}

			int count = (int)EventManager_impl::mgr->impl->_globalIndexToEventMap.size();
			EventManager_impl::mgr->impl->_globalIndexToEventMap.push_back(value);
			return count;
		}

		Any& EventManager_impl::EventFromGlobalIndex(int globalIndex)
		{
			return EventManager_impl::mgr->impl->_globalIndexToEventMap[globalIndex];
		}

		int EventManager_impl::GetGlobalIndex(RoutedEventPtr evt)
		{
			return evt->_globalIndex;
		}

		EventManager_impl::EventManager_impl()
		{ 
			_dTypedClassListeners = new DTypeMap(100);
			_dTypedRoutedEventList = new DTypeMap(10);
		}

		void EventManager_impl::AddOwnerWithoutLock(IRoutedEvent* dEvent, const Type* ownerType)
		{
			if (!ownerType) return;
			if (!ownerType->IsConvertible(_typeof(DependencyObject))) return;
			if (!dEvent) return;
			auto iter = _globalEventTypes.find(ownerType);
			if (iter != _globalEventTypes.end())
			{
				iter->second.insert(dEvent);
			}
			else
			{
				_globalEventTypes.insert(make_pair(ownerType, set<IRoutedEvent*>({ dEvent })));
			}
		}

		IRoutedEvent* EventManager_impl::RegisterRoutedEvent(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType)
		{
			if (!ownerType) return nullptr;
			if (!ownerType->IsConvertible(_typeof(DependencyObject))) return nullptr;

			lock_guard<CriticalSection> lock(EventManager_impl::mgr->impl->locker);
			IRoutedEvent* de = nullptr;

			auto iter = std::find_if(_globalEvents.begin(), _globalEvents.end(), vwrw(name.c_str(), eventStrategy, ownerType));

			if (iter != _globalEvents.end())
			{
				de = *iter;
				if (de->HandlerType != handlerType)
				{
					de = nullptr;
					throw std::exception("critical error");
				}
			}
			else
			{
				size_t index = _globalEvents.size();
				if (index >= _globalEvents.max_size()) return nullptr;
				de = new IRoutedEvent(name, eventStrategy, handlerType, ownerType);
				de->_globalIndex = index;
				_globalEvents.push_back(de);
			}
			AddOwnerWithoutLock(de, ownerType);
			return de;
		}

		IRoutedEvent* EventManager_impl::RegisterRoutedEventEx(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType, const Type* inheritType)
		{
			if (!inheritType || inheritType == ownerType) return RegisterRoutedEvent(name, eventStrategy, handlerType, ownerType);

			if (!ownerType) return nullptr;
			if (!ownerType->IsConvertible(_typeof(DependencyObject))) return nullptr;

			lock_guard<CriticalSection> lock(EventManager_impl::mgr->impl->locker);
			IRoutedEvent* de = nullptr;

			auto hter = std::find_if(_globalEvents.begin(), _globalEvents.end(), f1fqfa(name.c_str(), eventStrategy, handlerType, inheritType));

			if (hter != _globalEvents.end())
				de = *hter;
			else
			{
				size_t index = _globalEvents.size();
				if (index >= _globalEvents.max_size()) return nullptr;
				de = new IRoutedEvent(name, eventStrategy, handlerType, inheritType);
				de->_globalIndex = index;
				_globalEvents.push_back(de);
			}
			AddOwnerWithoutLock(de, ownerType);
			return de;
		}

		RoutedEventHandlerInfoList* EventManager_impl::GetUpdatedDTypedClassListeners(DependencyObjectTypePtr dType, RoutedEventPtr routedEvent,
			ClassHandlersStore*& classListenersLists, int& index)
		{
			classListenersLists = _dTypedClassListeners->Get(dType);
			RoutedEventHandlerInfoList* handlers;
			if (classListenersLists)
			{
				index = classListenersLists->GetHandlersIndex(routedEvent);
				if (index != -1)
				{
					handlers = classListenersLists->GetExistingHandlers(index);
					return handlers;
				}
			}

			DependencyObjectTypePtr tempDType = dType;
			ClassHandlersStore* tempClassListenersLists = nullptr;
			RoutedEventHandlerInfoList* tempHandlers = nullptr;
			int tempIndex = -1;

			if(!_dependencyObjectType) _dependencyObjectType = DependencyObjectType::FromSystemType(_typeof(DependencyObject));

			while (tempIndex == -1 && tempDType->Id != _dependencyObjectType->Id)
			{
				tempDType = tempDType->BaseType;
				tempClassListenersLists = _dTypedClassListeners->Get(tempDType);
				if (tempClassListenersLists)
				{
					tempIndex = tempClassListenersLists->GetHandlersIndex(routedEvent);
					if (tempIndex != -1)
					{
						tempHandlers = tempClassListenersLists->GetExistingHandlers(tempIndex);
					}
				}
			}

			if (!classListenersLists)
			{
				if (dType->SystemType->IsType(_typeof(System::UI::Windows::UIElement),true))
				{
					classListenersLists = new ClassHandlersStore(80);
				}
				else
				{
					classListenersLists = new ClassHandlersStore(1);
				}

				_dTypedClassListeners->Set(dType, classListenersLists);
			}

			index = classListenersLists->CreateHandlersLink(routedEvent, tempHandlers);

			return tempHandlers;
		}

		RoutedEventHandlerInfoList* EventManager_impl::GetDTypedClassListeners(DependencyObjectTypePtr dType, RoutedEventPtr routedEvent,
			ClassHandlersStore*& classListenersLists, int& index)
		{
			classListenersLists = _dTypedClassListeners->Get(dType);
			RoutedEventHandlerInfoList* handlers;
			if (classListenersLists)
			{
				index = classListenersLists->GetHandlersIndex(routedEvent);
				if (index != -1)
				{
					handlers = classListenersLists->GetExistingHandlers(index);
					return handlers;
				}
			}

			{
				lock_guard<CriticalSection> lock(EventManager_impl::mgr->impl->locker);
				handlers = GetUpdatedDTypedClassListeners(dType, routedEvent, classListenersLists, index);
			}

			return handlers;
		}

		void EventManager_impl::ensureRoutedEvent(RoutedEventPtr dEvent)
		{
			RoutedEvent* re = dEvent.get();
			if (!re->impl)
			{
				if (re->fun_ustaticdp) re->fun_ustaticdp();
			}
		}

		EventToken EventManager_impl::RegisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, IRoutedEventDelegatePtr eventDelegate)
		{
			ensureRoutedEvent(dEvent);

			if (!ownerType) return -1;
			if (!ownerType->IsConvertible(_typeof(DependencyObject))) return -1;
			if (!dEvent) return -1;
			if (!eventDelegate) return -1;

			ClassHandlersStore* classListenersLists;
			int index = 0;
			auto dType = DependencyObjectType::FromSystemType(ownerType);

			GetDTypedClassListeners(dType, dEvent, classListenersLists, index);

			{
				lock_guard<CriticalSection> lock(EventManager_impl::mgr->impl->locker);

				RoutedEventHandlerInfoList* updatedClassListeners =
					classListenersLists->AddToExistingHandlers(index, eventDelegate, eventDelegate->IgnoreIsHandled());

				const auto& keys = _dTypedClassListeners->_activeDTypes;

				for (const auto& key :keys)
				{
					if (key->IsSubclassOf(dType))
					{
						classListenersLists = _dTypedClassListeners->Get(key);
						classListenersLists->UpdateSubClassHandlers(dEvent, updatedClassListeners);
					}
				}

			}

			return -1;
		}

		void EventManager_impl::UnregisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, EventToken token)
		{
			ensureRoutedEvent(dEvent);

			if (!ownerType) return;
			if (!ownerType->IsConvertible(_typeof(DependencyObject))) return;
			if (!dEvent) return;
			if (token == -1) return;

			lock_guard<CriticalSection> lock(EventManager_impl::mgr->impl->locker);
		}

		#pragma endregion

	}
}

EventManager::EventManager()
{
	impl = new EventManager_impl();
}

EventManager::~EventManager()
{
	SAFE_DELETE(impl);
}

RoutedEventHandlerInfoList* EventManager::GetClassEventDelegates(DependencyObjectTypePtr ty, RoutedEventPtr ev)
{
	EventManager* mgr = EventManager_impl::mgr;
	if (!mgr) return nullptr;
	if (!mgr->impl) return nullptr;
	ClassHandlersStore* chs = nullptr;
	int index = 0;
	return mgr->impl->GetDTypedClassListeners(ty, ev, chs, index);
}

IRoutedEvent* EventManager::RegisterRoutedEvent(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType)
{
	EventManager* mgr = EventManager_impl::mgr;
	if (!mgr) return nullptr;
	if (!mgr->impl) return nullptr;
	return mgr->impl->RegisterRoutedEvent(name, eventStrategy, handlerType, ownerType);
}


IRoutedEvent* EventManager::RegisterRoutedEventEx(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType, const Type* inheritType)
{
	EventManager* mgr = EventManager_impl::mgr;
	if (!mgr) return nullptr;
	if (!mgr->impl) return nullptr;
	return mgr->impl->RegisterRoutedEventEx(name, eventStrategy, handlerType, ownerType, inheritType);
}

EventToken EventManager::RegisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, IRoutedEventDelegatePtr eventDelegate)
{
	EventManager* mgr = EventManager_impl::mgr;
	if (!mgr) return -1;
	if (!mgr->impl) return -1;
	return mgr->impl->RegisterClassRoutedEvent(ownerType, dEvent, eventDelegate);
}

void EventManager::UnregisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, EventToken token)
{
	EventManager* mgr = EventManager_impl::mgr;
	if (!mgr) return;
	if (!mgr->impl) return;
	return mgr->impl->UnregisterClassRoutedEvent(ownerType, dEvent, token);
}