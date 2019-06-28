#pragma once
#include "..\EventManager.h"
#include "..\Windows\UIElement.h"
#include "RoutedEventHandlerInfo.h"
#include <map>
using namespace std;

namespace System
{
	namespace UI
	{
		struct DependencyObjectTypePtrHash
		{
			std::size_t operator()(const DependencyObjectTypePtr& k) const
			{
				return k->Id;
			}
		};

		class DTypeMap
		{
		public:
			DTypeMap(int entryCount);

			ClassHandlersStore* Get(DependencyObjectTypePtr dType);
			void Set(DependencyObjectTypePtr dType, ClassHandlersStore*);

			void Clear();

			int _entryCount;
			std::vector<ClassHandlersStore*> _entries;
			unordered_map<DependencyObjectTypePtr, ClassHandlersStore*, DependencyObjectTypePtrHash>  _overFlow;
			std::vector<DependencyObjectTypePtr> _activeDTypes;
		};

		class EventManager_impl
		{
		private:
			friend class EventManager;
			static mutex singletonlocker;
			static EventManager* mgr;
			DTypeMap*  _dTypedClassListeners;
			DTypeMap*  _dTypedRoutedEventList;
			map<const Type*, set<IRoutedEvent*>> _globalEventTypes;
			vector<IRoutedEvent*> _globalEvents;
			int _countRoutedEvents;
			DependencyObjectTypePtr _dependencyObjectType;
			std::vector<Any> _globalIndexToEventMap;
			CriticalSection locker;
		public:

			static void ensureRoutedEvent(RoutedEventPtr);

			static EventManager* Create();

			static int GetNextAvailableGlobalIndex(const Any& value);

			static Any& EventFromGlobalIndex(int globalIndex);

			static int GetGlobalIndex(RoutedEventPtr evt);

			EventManager_impl();

			void AddOwnerWithoutLock(IRoutedEvent* dEvent, const Type* ownerType);

			IRoutedEvent* RegisterRoutedEvent(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType);

			IRoutedEvent* RegisterRoutedEventEx(const wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType, const Type* inheritType);

			RoutedEventHandlerInfoList* GetUpdatedDTypedClassListeners(DependencyObjectTypePtr dType, RoutedEventPtr routedEvent,
				ClassHandlersStore*& classListenersLists, int& index);

			RoutedEventHandlerInfoList* GetDTypedClassListeners(DependencyObjectTypePtr dType, RoutedEventPtr routedEvent,
				ClassHandlersStore*& classListenersLists, int& index);

			EventToken RegisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, IRoutedEventDelegatePtr eventDelegate);

			void UnregisterClassRoutedEvent(const Type* ownerType, RoutedEventPtr dEvent, EventToken token);
		};

	}
}

