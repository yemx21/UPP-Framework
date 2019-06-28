#include "RoutedEventHandlerInfo.h"
using namespace System::UI;

RoutedEventHandlerInfo::RoutedEventHandlerInfo() : Handler(nullptr), IgnoreIsHandled(false)
{

}


RoutedEventHandlerInfo::RoutedEventHandlerInfo(DelegatePtr handler, bool ignoreIsHandled) : Handler(handler), IgnoreIsHandled(ignoreIsHandled)
{
}


RoutedEventHandlerInfo::RoutedEventHandlerInfo(const RoutedEventHandlerInfo& info)
{
	Handler = info.Handler;
	IgnoreIsHandled = info.IgnoreIsHandled;
}

void RoutedEventHandlerInfo::InvokeHandler(DependencyObjectPtr target, RoutedEventArgs& args) const
{
	if (!args.IsHandled || !IgnoreIsHandled)
	{
		Handler->GenericInvoke(target, OptionalVariadicAny(args, false), false);
	}
}

RoutedEventHandlerInfoList::RoutedEventHandlerInfoList() :Next(nullptr)
{

}

bool RoutedEventHandlerInfoList::Contains(RoutedEventHandlerInfoList* handlers)
{
	RoutedEventHandlerInfoList* tempHandlers = this;
	while (tempHandlers)
	{
		if (tempHandlers == handlers)
		{
			return true;
		}

		tempHandlers = tempHandlers->Next;
	}

	return false;
}

ClassHandlers::ClassHandlers():Handlers(nullptr), HasSelfHandlers(false)
{

}

bool ClassHandlers::Equals(const ClassHandlers& classHandlers) const
{
	return (
		classHandlers.DEvent == DEvent &&
		classHandlers.Handlers == Handlers);
}

ClassHandlersStore::ClassHandlersStore(int size)
{
	_eventHandlersList.reserve(size);
}

RoutedEventHandlerInfoList* ClassHandlersStore::AddToExistingHandlers(int index, DelegatePtr handler, bool handledEventsToo)
{
	RoutedEventHandlerInfoPtr routedEventHandlerInfo = std::make_shared<RoutedEventHandlerInfo>(handler, handledEventsToo);

	RoutedEventHandlerInfoList* handlers = _eventHandlersList[index]->Handlers;
	if (!handlers || _eventHandlersList[index]->HasSelfHandlers == false)
	{
		handlers = new RoutedEventHandlerInfoList();
		handlers->Handlers.push_back(routedEventHandlerInfo);
		handlers->Next = _eventHandlersList[index]->Handlers;
		_eventHandlersList[index]->Handlers = handlers;
		_eventHandlersList[index]->HasSelfHandlers = true;
	}
	else
	{
		handlers->Handlers.push_back(routedEventHandlerInfo);
	}

	return handlers;
}

RoutedEventHandlerInfoList* ClassHandlersStore::GetExistingHandlers(int index)
{
	return _eventHandlersList[index]->Handlers;
}

int ClassHandlersStore::CreateHandlersLink(RoutedEventPtr routedEvent, RoutedEventHandlerInfoList* handlers)
{
	ClassHandlers* classHandlers = new ClassHandlers();
	classHandlers->DEvent = routedEvent;
	classHandlers->Handlers = handlers;
	classHandlers->HasSelfHandlers = false;
	_eventHandlersList.push_back(classHandlers);

	return _eventHandlersList.size() - 1;
}

void ClassHandlersStore::UpdateSubClassHandlers(RoutedEventPtr routedEvent, RoutedEventHandlerInfoList* baseClassListeners)
{
	int index = GetHandlersIndex(routedEvent);
	if (index != -1)
	{
		bool hasSelfHandlers = _eventHandlersList[index]->HasSelfHandlers;

		RoutedEventHandlerInfoList* handlers = hasSelfHandlers ?
			_eventHandlersList[index]->Handlers->Next :
			_eventHandlersList[index]->Handlers;

		bool needToChange = false;
		if (handlers)
		{
			if (baseClassListeners->Next && baseClassListeners->Next->Contains(handlers))
			{
				needToChange = true;
			}
		}
		else
		{
			needToChange = true;
		}

		if (needToChange)
		{
			if (hasSelfHandlers)
			{
				_eventHandlersList[index]->Handlers->Next = baseClassListeners;
			}
			else
			{
				_eventHandlersList[index]->Handlers = baseClassListeners;
			}
		}
	}
}

int ClassHandlersStore::GetHandlersIndex(RoutedEventPtr routedEvent)
{
	for (int i = 0; i<_eventHandlersList.size(); i++)
	{
		if (_eventHandlersList[i]->DEvent == routedEvent)
		{
			return i;
		}
	}
	return -1;
}