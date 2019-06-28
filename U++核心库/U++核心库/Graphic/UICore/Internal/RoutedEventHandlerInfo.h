#pragma once
#include "..\DependencyObject.h"

namespace System
{
	namespace UI
	{
		struct RoutedEventHandlerInfo
		{
			DelegatePtr  Handler;
			bool IgnoreIsHandled;

			RoutedEventHandlerInfo();

			RoutedEventHandlerInfo(DelegatePtr handler, bool ignoreIsHandled);

			RoutedEventHandlerInfo(const RoutedEventHandlerInfo& info);

			void InvokeHandler(DependencyObjectPtr target, RoutedEventArgs& args) const;
		};

		typedef std::shared_ptr<RoutedEventHandlerInfo> RoutedEventHandlerInfoPtr;

		struct RoutedEventHandlerInfoList
		{
			std::vector<RoutedEventHandlerInfoPtr> Handlers;
			RoutedEventHandlerInfoList* Next;

			RoutedEventHandlerInfoList();

			bool Contains(RoutedEventHandlerInfoList* handlers);
		};

		struct ClassHandlers
		{
			RoutedEventPtr DEvent;
			RoutedEventHandlerInfoList* Handlers;
			bool HasSelfHandlers;

			ClassHandlers();

			bool Equals(const ClassHandlers& classHandlers) const;

		};

		inline bool operator== (const ClassHandlers& classHandlers1, const ClassHandlers& classHandlers2) throw ()
		{
			return classHandlers1.Equals(classHandlers2);
		}

		inline bool operator!= (const ClassHandlers& classHandlers1, const ClassHandlers& classHandlers2) throw ()
		{
			return !classHandlers1.Equals(classHandlers2);
		}

		class ClassHandlersStore
		{
		public:
			std::vector<ClassHandlers*> _eventHandlersList;

			ClassHandlersStore(int size);
		
			RoutedEventHandlerInfoList* AddToExistingHandlers(int index, DelegatePtr handler, bool handledEventsToo);

			RoutedEventHandlerInfoList* GetExistingHandlers(int index);

			int CreateHandlersLink(RoutedEventPtr routedEvent, RoutedEventHandlerInfoList* handlers);

			void UpdateSubClassHandlers(RoutedEventPtr routedEvent, RoutedEventHandlerInfoList* baseClassListeners);

			int GetHandlersIndex(RoutedEventPtr routedEvent);
		};
	}
}
