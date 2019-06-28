#include "DependencyObject.h"
#include <algorithm>
#include "EventManager.h"
#include "RoutedEvent.h"
#include "Windows\Visual.h"
#include "Windows\UIElement.h"
#include "Internal\RoutedEventHandlerInfo.h"
#include "Internal\EventHandlersStore.h"
#include "Internal\MulticastRoutedEventDelegate.h"
using namespace std;
using namespace System::UI;
using namespace System::UI::Windows;

namespace System
{
	namespace UI
	{
		struct RoutedEventItem
		{
			RoutedEventHandlerInfo Info;
			DependencyObjectPtr Target;

			RoutedEventItem(DependencyObjectPtr target, RoutedEventHandlerInfo info) : Target(target), Info(info)
			{

			}

			RoutedEventItem(const RoutedEventItem& item)
			{
				Info = item.Info;
				Target = item.Target;
			}

			~RoutedEventItem() {}

			void InvokeHandler(RoutedEventArgs& args) const
			{
				Info.InvokeHandler(Target, args);
			}
		};

		struct DependencySourceItem
		{
			size_t StartIndex;
			DependencyObjectPtr Source;

			DependencySourceItem(DependencyObjectPtr source, size_t startIndex) : Source(source), StartIndex(startIndex)
			{

			}

			DependencySourceItem(const DependencySourceItem& item)
			{
				Source = item.Source;
				StartIndex = item.StartIndex;
			}
		};

		struct DependencyBranchNode
		{
			DependencyObjectPtr Node;
			DependencyObjectPtr Source;

			DependencyBranchNode(DependencyObjectPtr node, DependencyObjectPtr source) : Node(node), Source(source) {}
			DependencyBranchNode(const DependencyBranchNode& node)
			{
				Node = node.Node;
				Source = node.Source;
			}
		};

		class RoutedEventRoute
		{
		public:
			RoutedEventPtr DEvent;
			vector<RoutedEventItem> EventItems;
			vector<DependencySourceItem> SourceItems;
			deque<DependencyBranchNode> BranchNodeStack;

			RoutedEventRoute()
			{

			}

			RoutedEventRoute(RoutedEventPtr dEvent) : DEvent(dEvent)
			{

			}

			~RoutedEventRoute()
			{

			}

			RoutedEventRoute(const RoutedEventRoute& route)
			{
				DEvent = route.DEvent;
				EventItems = route.EventItems;
				SourceItems = route.SourceItems;
				BranchNodeStack = route.BranchNodeStack;
			}

			void Add(DependencyObjectPtr target, DelegatePtr handler, bool ignoreIsHandled)
			{
				if (!target || !handler) return;
				EventItems.push_back(RoutedEventItem(target, RoutedEventHandlerInfo(handler, ignoreIsHandled)));
			}

			void AddSource(DependencyObjectPtr source)
			{
				SourceItems.push_back(DependencySourceItem(source, EventItems.size()));
			}


			void Remove(DelegatePtr handler)
			{
				if (!handler) return;
				EventItems.erase(std::remove_if(EventItems.begin(), EventItems.end(),
					[&handler](const RoutedEventItem& rei) {return rei.Info.Handler == handler; }));
			}


			void Clear()
			{
				DEvent.reset();
				EventItems.clear();
				BranchNodeStack.clear();
				SourceItems.clear();
			}

			DependencyObjectPtr PeekBranchNode()
			{
				if (BranchNodeStack.size() == 0) return nullptr;
				auto& top = BranchNodeStack.front();
				return top.Node;
			}

			DependencyObjectPtr PeekBranchSource()
			{
				if (BranchNodeStack.size() == 0) return nullptr;
				auto& top = BranchNodeStack.front();
				return top.Source;
			}

			DependencyObjectPtr PopBranchNode()
			{
				if (BranchNodeStack.size() == 0) return nullptr;
				auto& end = BranchNodeStack.end();
				return end->Node;
			}

			void PushBranchNode(DependencyObjectPtr node, DependencyObjectPtr source)
			{
				BranchNodeStack.push_back(DependencyBranchNode(node, source));
			}

			DependencyObjectPtr GetBubbleSource(size_t index, size_t& endIndex)
			{
				if (SourceItems.size() == 0)
				{
					endIndex = EventItems.size();
					return nullptr;
				}
				if (index < SourceItems[0].StartIndex)
				{
					endIndex = SourceItems[0].StartIndex;
					return nullptr;
				}
				for (size_t i = 0; i < SourceItems.size() - 1; i++)
				{
					if (index >= SourceItems[i].StartIndex && index < SourceItems[i + 1].StartIndex)
					{
						DependencySourceItem& item = SourceItems[i + 1];
						endIndex = item.StartIndex;
						return SourceItems[i].Source;
					}
				}
				endIndex = EventItems.size();
				DependencySourceItem& sourceItem = SourceItems[SourceItems.size() - 1];
				return sourceItem.Source;
			}

			DependencyObjectPtr GetTunnelSource(size_t index, size_t& startIndex)
			{
				if (SourceItems.size() == 0)
				{
					startIndex = 0;
					return nullptr;
				}
				if (index < SourceItems[0].StartIndex)
				{
					startIndex = 0;
					return nullptr;
				}
				for (size_t i = 0; i < SourceItems.size() - 1; i++)
				{
					if (index >= SourceItems[i].StartIndex && index < SourceItems[i + 1].StartIndex)
					{
						DependencySourceItem& item = SourceItems[i];
						startIndex = item.StartIndex;
						return item.Source;
					}
				}
				DependencySourceItem& item = SourceItems[SourceItems.size() - 1];
				startIndex = item.StartIndex;
				return item.Source;
			}

			void InvokeHandlers(DependencyObjectPtr source, RoutedEventArgs& args, bool reRaised)
			{

				if (!source || &args == nullptr) return;
				if (!args.Source) return;
				if (!args.Event) return;
				if (args.Event != DEvent) return;
				if (args.Event->EventStrategy == EventStrategy::Bubble || args.Event->EventStrategy == EventStrategy::Direct)
				{
					size_t num1 = 0;
					for (size_t i = 0; i < EventItems.size(); i++)
					{
						if (i >= num1)
						{
							DependencyObjectPtr bubbleSource = GetBubbleSource(i, num1);
							if (!reRaised)
							{
								if (bubbleSource)
								{
									args.Source = bubbleSource;
								}
								else
								{
									args.Source = source;
								}
							}
						}
						EventItems[i].InvokeHandler(args);

					}
					return;
				}
				Int64 num = 0;
				size_t count = EventItems.size();
				for (Int64 j = (Int64)EventItems.size() - 1; j >= 0; j = num)
				{
					DependencyObjectPtr target = EventItems[(size_t)j].Target;
					num = j;
					while (num >= 0 && EventItems[(size_t)num].Target == target)
					{
						num--;
					}
					for (Int64 k = num + 1; k <= j; k++)
					{
						if (k < (Int64)count)
						{
							DependencyObjectPtr tunnelSource = GetTunnelSource((size_t)k, count);
							if (tunnelSource != nullptr)
							{
								args.Source = tunnelSource;
							}
							else
							{
								args.Source = source;
							}
						}
						EventItems[(size_t)k].InvokeHandler(args);
					}
				}
			}

			void InvokeHandlers(DependencyObjectPtr source, RoutedEventArgs& args)
			{
				InvokeHandlers(source, args, false);
			}

			void ReInvokeHandlers(DependencyObjectPtr source, RoutedEventArgs& args)
			{
				InvokeHandlers(source, args, true);
			}
		};

		class RoutedEventFactory
		{
		private:
			mutex locker;
			RoutedEventRoute** routes;
			size_t stackTop;
			static RoutedEventFactory& getInstance()
			{
				static RoutedEventFactory factory;
				return factory;
			}

			RoutedEventFactory() : stackTop(0), routes(nullptr)
			{

			}

			~RoutedEventFactory()
			{
				if (routes)
				{
					SAFE_DELETE(routes[0]);
					SAFE_DELETE(routes[1]);
					delete routes;
					routes = nullptr;
				}
			}

			RoutedEventRoute* OnPop()
			{
				lock_guard<mutex> lock(locker);
				if (stackTop > 0)
				{
					return routes[--stackTop];
				}
				return nullptr;
			}

			void OnPush(RoutedEventRoute* eventRoute)
			{
				lock_guard<mutex> lock(locker);
				if (!routes)
				{
					routes = new RoutedEventRoute*[2];
					routes[0] = new RoutedEventRoute();
					routes[1] = new RoutedEventRoute();
				}
				if (stackTop < 2) routes[stackTop++] = eventRoute;
			}

			void OnRecycleObject(RoutedEventRoute* eventRoute)
			{
				eventRoute->Clear();

				OnPush(eventRoute);
			}

			RoutedEventRoute* OnFetchObject(RoutedEventPtr dEvent)
			{
				RoutedEventRoute* eventRoute = OnPop();
				if (eventRoute != nullptr)
				{
					eventRoute->DEvent = dEvent;
				}
				else
				{
					eventRoute = new RoutedEventRoute(dEvent);
				}
				return eventRoute;
			}
		public:
			static RoutedEventRoute* Pop()
			{
				RoutedEventFactory& factory = getInstance();
				factory.OnPop();
			}

			static void Push(RoutedEventRoute* eventRoute)
			{
				RoutedEventFactory& factory = getInstance();
				factory.OnPush(eventRoute);
			}

			static void RecycleObject(RoutedEventRoute* eventRoute)
			{
				RoutedEventFactory& factory = getInstance();
				factory.OnRecycleObject(eventRoute);
			}

			static RoutedEventRoute* FetchObject(RoutedEventPtr dEvent)
			{
				RoutedEventFactory& factory = getInstance();
				return factory.OnFetchObject(dEvent);
			}

		};

		class bres123 : public std::unary_function < RoutedEventList*, bool >
		{
		private:
			RoutedEventPtr DEvent;
		public:
			explicit bres123(RoutedEventPtr dEvent) : DEvent(dEvent)
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

		class mnru654dfs : public std::unary_function < RoutedEventList*, bool >
		{
		private:
			const Type* HandlerType;
			RoutedEventPtr DEvent;
		public:
			explicit mnru654dfs(const Type* dEventHandlerType, RoutedEventPtr dEvent) : HandlerType(dEventHandlerType), DEvent(dEvent)
			{
			}

			bool operator() (RoutedEventList* info)
			{
				try
				{
					if (!info) return false;
					if (info->DEvent != DEvent) return false;
					if (info->DEvent) return info->DEvent->HandlerType == HandlerType;
					return false;
				}
				catch (...)
				{
					return false;
				}
			}
		};
	}
}

bool IRoutedEventDelegate::GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow) const
{
	if (args.Size != 1) return false;
	return GenericInvoke(sender, args.At(0).as<RoutedEventArgs&>(), nothrow);
}

bool IRoutedEventDelegate::GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow) const
{
	if (args.Size != 1) return false;
	return GenericInvoke(sender, args.At(0).as<RoutedEventArgs&>(), nothrow);
}

bool IRoutedEventDelegate::HasReturn() const
{
	return false;
}

RoutedEventList::RoutedEventList(RoutedEventPtr dEvent):_tokens(-1)
{
	_dEvent = dEvent;
}

RoutedEventList::~RoutedEventList()
{
	for (auto iter = _delegates.begin(); iter != _delegates.end();)
	{
		iter = _delegates.erase(iter);
	}
}

EventToken RoutedEventList::Add(DelegatePtr eventDelegate) const
{
	auto token = _tokens.operator++();
	_delegates[token] = eventDelegate;
	return token;
}

EventToken RoutedEventList::Remove(EventToken token) const
{
	auto iter = _delegates.find(token);
	if (iter != _delegates.end())
	{
		_delegates.erase(iter);
		return token;
	}
	return -1;
}

RoutedEventPtr RoutedEventList::GetDEvent() const
{
	return _dEvent;
}


bool RoutedEventHandler_base::Invoke(DependencyObjectPtr sender, RoutedEventArgs& args, bool nothrow) const
{
	return false;
}

#pragma region DependencyObjectType

namespace System
{
	namespace UI
	{
		class DependencyObjectType_impl
		{
		private:
			static mutex locker;
			static map<_type, DependencyObjectTypePtr> DTypeFromCLRType;
			static int DTypeCount;
		public:
			static DependencyObjectTypePtr FromSystemTypeInternal(_type systemType);

			static DependencyObjectTypePtr FromSystemTypeRecursive(_type systemType);
		};

		mutex DependencyObjectType_impl::locker;
		map<_type, DependencyObjectTypePtr> DependencyObjectType_impl::DTypeFromCLRType;
		int DependencyObjectType_impl::DTypeCount = 0;

		DependencyObjectTypePtr DependencyObjectType_impl::FromSystemTypeInternal(_type systemType)
		{
			DependencyObjectTypePtr retVal;
			{
				lock_guard<mutex> lock(locker);
				retVal = FromSystemTypeRecursive(systemType);
			}
			return retVal;
		}

		DependencyObjectTypePtr DependencyObjectType_impl::FromSystemTypeRecursive(_type systemType)
		{
			DependencyObjectTypePtr dType;
			auto dTypeiter = DTypeFromCLRType.find(systemType);
			if (dTypeiter == DTypeFromCLRType.end())
			{
				dType = DependencyObjectType::Create();
				dType->_systemType = systemType;
				DTypeFromCLRType[systemType] = dType;
				if (!systemType->IsType(_typeof(DependencyObject), true))
				{
					if (systemType->IsClass)
					{
						const ClassInfo* clsty = static_cast<const ClassInfo*>(systemType);
						if (clsty->NumberOfBaseClasses)
						{
							auto baseclsquery = clsty->GetBaseClassEnumerator();
							_type basety = nullptr;
							while (!baseclsquery->IsEnd)
							{
								if (baseclsquery->Current->IsBaseOf(_typeof(DependencyObject)))
								{
									basety = baseclsquery->Current;
									break;
								}
								baseclsquery->Next();
							}
							if (basety)
							{
								dType->_baseDType = FromSystemTypeRecursive(basety);
							}
							else if(clsty->IsBaseOf(_typeof(DependencyObject)))
							{
								dType->_baseDType = FromSystemTypeRecursive(_typeof(DependencyObject));
							}
						}
					}
				}
				dType->_id = DTypeCount++;
			}
			else
			{
				dType = dTypeiter->second;
			}
			return dType;
		}	
	}
}

DependencyObjectTypePtr DependencyObjectType::Create()
{
	struct DependencyObjectType1 : public DependencyObjectType {};

	return std::make_shared<DependencyObjectType1>();
}

DependencyObjectType::DependencyObjectType() : _id(0), _systemType(nullptr), _baseDType(nullptr)
{

}
DependencyObjectTypePtr DependencyObjectType::FromSystemType(_type systemType)
{
	if (!systemType)
	{
		return nullptr;
	}
	
	if (!systemType->IsBaseOfOrSameAs(_typeof(DependencyObject)))
	{
		return nullptr;
	}

	return DependencyObjectType_impl::FromSystemTypeInternal(systemType);
}

bool DependencyObjectType::IsInstanceOfType(DependencyObject* dependencyObject)
{
	if (!dependencyObject)
	{
		DependencyObjectTypePtr dType = dependencyObject->DependencyObjectType;
		do
		{
			if (dType->Id == Id)
			{
				return true;
			}

			dType = dType->_baseDType;
		} while (dType != nullptr);
	}
	return false;
}

bool DependencyObjectType::IsSubclassOf(DependencyObjectTypePtr dependencyObjectType)
{
	if (dependencyObjectType)
	{
		DependencyObjectTypePtr dType = _baseDType;

		while (dType)
		{
			if (dType->Id == dependencyObjectType->Id)
			{
				return true;
			}

			dType = dType->_baseDType;
		}
	}
	return false;
}

uhash DependencyObjectType::GetHashCode() const
{
	return _id;
}

int DependencyObjectType::GetId() const
{
	return _id;
}

_type DependencyObjectType::GetSystemType() const
{
	return _systemType;
}

const std::wstring& DependencyObjectType::GetName() const
{
	return _systemType->Name;
}

DependencyObjectTypePtr DependencyObjectType::GetBaseType() const
{
	return  _baseDType;
}

DependencyObjectTypePtr DependencyObject::GetDependencyObjectType() const
{
	if (!dType)
	{
		dType = System::UI::DependencyObjectType::FromSystemType(this->GetType());
	}
	return dType;
}

#pragma endregion

void DependencyObject::RegisterRoutedEventBase(std::shared_ptr<RoutedEventHandler_base> uievtbase, RoutedEventPtr dEvent, bool handledEventsToo)
{
	MulticastRoutedEventDelegatePtr delptr=	std::make_shared<MulticastRoutedEventDelegate>(uievtbase, handledEventsToo);
	eventStore->AddRoutedEventHandler(dEvent, delptr, handledEventsToo);
}

void DependencyObject::OnAddToEventRoute(RoutedEventRoute* route, RoutedEventArgs& e) const
{

}

void DependencyObject::AddToEventRoute(RoutedEventRoute* route, RoutedEventArgs& e)
{
	if (!route || &e == nullptr) return;
	RoutedEventHandlerInfoList* classListeners = EventManager::GetClassEventDelegates(DependencyObjectType, e.Event);

	auto this_ptr = std::shared_ptr<DependencyObject>(this, null_deleter());

	while (classListeners)
	{
		for (const auto& handler :classListeners->Handlers)
		{
			route->Add(this_ptr, handler->Handler, handler->IgnoreIsHandled);
		}

		classListeners = classListeners->Next;
	}

	auto instanceListeners_ptr = eventStore->Get(e.Event);
	if (instanceListeners_ptr)
	{
		const auto& instanceListeners = *instanceListeners_ptr;
		for(const auto& handler : instanceListeners)
		{
			route->Add(this_ptr, handler->Handler, handler->IgnoreIsHandled);
		}
	}

	OnAddToEventRoute(route, e);
}

DependencyObjectPtr DependencyObject::AdjustEventSource(RoutedEventArgs& args) const
{
	return nullptr;
}

void DependencyObject::OnAddInputPreHandler(RoutedEventRoute*, RoutedEventArgs&) const
{

}

void DependencyObject::OnAddInputPostHandler(RoutedEventRoute*, RoutedEventArgs&) const
{

}

void DependencyObject::AddInputPreHandler(RoutedEventRoute* route, RoutedEventArgs& args)
{

}

void DependencyObject::AddInputPostHandler(RoutedEventRoute* route, RoutedEventArgs&args)
{

}

bool DependencyObject::BuildRouteCore(RoutedEventRoute* route, RoutedEventArgs&args)
{
	return false;
}

void DependencyObject::BuildRoute(RoutedEventRoute* route, RoutedEventArgs& args)
{
	DependencyObjectPtr e = std::shared_ptr<DependencyObject>(this, null_deleter());
	BuildRouteHelper(e, route, args);
}

void DependencyObject::BuildRouteHelper(DependencyObjectPtr e, RoutedEventRoute* route, RoutedEventArgs& args)
{
	if (!route || &args == nullptr)  return;
	if (!args.Source)  return;
	auto dEvent = args.Event;
	if (!dEvent) return;
	if (dEvent != route->DEvent) return;

	if (dEvent->EventStrategy == EventStrategy::Direct)
	{
		UIElementPtr uiElement = std::dynamic_pointer_cast<UIElement>(e);
		if (uiElement) uiElement->AddToEventRoute(route, args);
	}
	else
	{
		int cElements = 0;

		while (e)
		{
			UIElementPtr uiElement = std::dynamic_pointer_cast<UIElement>(e);
			if(uiElement)

			if (cElements++ > 4096) throw std::exception("tree loop");

			DependencyObjectPtr newSource = nullptr;
			if (uiElement)
			{
				newSource = uiElement->AdjustEventSource(args);
			}
			
			if (newSource)
			{
				route->AddSource(newSource);
			}

			bool continuePastVisualTree = false;
			if (uiElement)
			{
				uiElement->AddInputPreHandler(route, args);
				continuePastVisualTree = uiElement->BuildRouteCore(route, args);
				uiElement->AddToEventRoute(route, args);
				uiElement->AddInputPostHandler(route, args);
				e = uiElement.GetUIParent(continuePastVisualTree);
			}
			
			if (e == args.Source)
			{
				route->AddSource(e);
			}
		}
	}
}


void DependencyObject::RaiseEvent(RoutedEventArgs& args)
{
	RoutedEventRoute* route = RoutedEventFactory::FetchObject(args.Event);
	if (!route) return;
	//try
	//{
		auto this_ptr = std::shared_ptr<DependencyObject>(this, null_deleter());
		args.Source = this_ptr;
		BuildRoute(route, args);
		route->InvokeHandlers(this_ptr, args);
		args.Source = args.OriginalSource;
	//}
	//catch (...)
	//{

	//}
	RoutedEventFactory::RecycleObject(route);
}

DependencyObject::DependencyObject() : isInvalidatable(false), inheritanceContext(nullptr), inheritanceContextSet(false)
{
	eventStore = std::make_shared<EventHandlersStore>();
}


DependencyObject::~DependencyObject()
{
    
}

SharedVoid DependencyObject::GetValue(const DependencyPropertyPtr& dependencyProperty)  const
{
	try
	{
		DependencyObject* refV = const_cast<DependencyObject*>(this);
		if (refV)
		{
			auto value = refV->GetValueInternal(dependencyProperty);
			return value;
		}
	}
	catch (...)
	{
		
	}
	return nullptr;
}

void DependencyObject::ensureDependencyProperty(const DependencyPropertyPtr& dependencyProperty)
{
	DependencyProperty* dp = dependencyProperty.get();
	if (!dp->impl)
	{
		if (dp->fun_ustaticdp) dp->fun_ustaticdp();
	}
}
void DependencyObject::ensureRoutedEvent(const RoutedEventPtr& routedEvent)
{
	RoutedEvent* re = routedEvent.get();
	if (!re->impl)
	{
		if (re->fun_ustaticdp) re->fun_ustaticdp();
	}
}

SharedVoid DependencyObject::GetValueInternal(const DependencyPropertyPtr& dependencyProperty)
{
	ensureDependencyProperty(dependencyProperty);

    bool hasValue = false;

    DependantValueEntryPtr entry;

    auto entryIt = valueEntries.find(dependencyProperty);

    if(entryIt != end(valueEntries))
    {
         entry = (*entryIt).second;
         if(entry->HasCalculatedValue())
            hasValue = true;
    }
    else
    {
        entry = make_shared<DependantValueEntry>(dependencyProperty, this);
		valueEntries[dependencyProperty] = entry;
    }

    bool isInheritable = dependencyProperty->PropertyMetadata->Inherits();
    
    if(!hasValue && isInheritable && inheritanceContext)
    {
        auto inheritedValue = inheritanceContext->GetValueInternal(dependencyProperty);
        entry->InheritatedValue(inheritedValue);
        return inheritedValue;
    }

    if(!hasValue)
    {
        entry->InheritatedValue(dependencyProperty->PropertyMetadata->DefaultValue());
        return dependencyProperty->PropertyMetadata->DefaultValue();
    }
	return valueEntries[dependencyProperty]->CalculatedValue();
}

bool DependencyObject::CoerceValue(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& oldValue, SharedVoid& newValue)
{
	ensureDependencyProperty(dependencyProperty);
	auto preCoerceCallback = dependencyProperty->PropertyMetadata->PreCoerceCallback();
	if (preCoerceCallback) return preCoerceCallback(this, oldValue, newValue);
	return true;
}

bool DependencyObject::ContainsDependencyProperty(const DependencyPropertyPtr& dependencyProperty)
{
	ensureDependencyProperty(dependencyProperty);
	return valueEntries.find(dependencyProperty) != end(valueEntries);
}

bool DependencyObject::ContainsDependencyProperty(const wstring& dependencyPropertyName, StringComparison comparison)
{
	for (auto& iter : valueEntries)
	{
		if (StringHelper::Compare(iter.first->name.get(), dependencyPropertyName, comparison) == 0) return true;
	}
	return false;
}

void DependencyObject::SetValueImpl(const DependencyPropertyPtr& dependencyProperty, const shared_ptr<void>& value, bool noevent, const ValueSource valueSource )
{
	ensureDependencyProperty(dependencyProperty);

    DependantValueEntryPtr entry;

    auto entryIt = valueEntries.find(dependencyProperty);

    if(entryIt != end(valueEntries))
    {
        entry = (*entryIt).second;
    }
    else
    {
        entry = make_shared<DependantValueEntry>(dependencyProperty, this);
        valueEntries[dependencyProperty] = entry;
    }

    if ((entry->IsAnimated() || entry->HasBaseValue()) && valueSource == ValueSource::Inherited)
    {
         entry->InheritatedValue(value);
         return;
    }

    
    auto currentValue = GetValue(dependencyProperty);

	auto nvalue = value;
	if (!CoerceValue(dependencyProperty, currentValue, nvalue)) return;

    if(AreValuesSame(dependencyProperty, currentValue, nvalue)) return;

	if (noevent)
	{
		switch (valueSource)
		{
		case ValueSource::Animation:
			entry->AnimatedValue(nvalue);
			break;
		case ValueSource::Explicit:
			entry->BaseValue(nvalue);
			break;
		case ValueSource::Inherited:
			entry->InheritatedValue(nvalue);
			break;
		}
		return;
	}

	auto changeCallback = dependencyProperty->PropertyMetadata->ChangeCallback();

	auto args = PropertyChangedEventArgs(currentValue, nvalue);
	bool result = true;
	if (changeCallback && dependencyProperty->IsOfOwnerType(this))
	{
		result = changeCallback(this, args);
	}
	if (result)
	{
		switch (valueSource)
		{
		case ValueSource::Animation:
			entry->AnimatedValue(nvalue);
			break;
		case ValueSource::Explicit:
			entry->BaseValue(nvalue);
			break;
		case ValueSource::Inherited:
			entry->InheritatedValue(nvalue);
			break;
		}
		OnPropertyChanged(dependencyProperty, args);

		FrameworkPropertyMetadataPtr meta = dependencyProperty->PropertyMetadata;
		if (meta)
		{
			if (Enums::Contains(meta->FrameworkPropertyMetadataOptionsFlags(), FrameworkPropertyMetadataOptions::AffectsUpdate))
			{
				Updated(dependencyProperty, args);
			}
		}
	}
}

void DependencyObject::SetInheritanceContext(DependencyObjectPtr inheritanceContext )
{
    inheritanceContext = inheritanceContext;
    inheritanceContextSet = true;

    for(auto& pair : valueEntries)
    {
        auto& dependencyProperty = pair.first;

        if(dependencyProperty->PropertyMetadata->Inherits())
        {
            auto& entry = valueEntries[dependencyProperty];

            auto currentValue = entry->InheritatedValue();
            
            auto value = GetValue(dependencyProperty);

            if(AreValuesSame(dependencyProperty, currentValue, value))
                return;

            valueEntries[dependencyProperty]->InheritatedValue(value);

			auto changeCallback = dependencyProperty->PropertyMetadata->ChangeCallback();

			auto args = PropertyChangedEventArgs(currentValue, value);
			if (changeCallback && dependencyProperty->IsOfOwnerType(this))
			{
				changeCallback(this, args);
			}
			OnPropertyChanged(dependencyProperty, args);
			FrameworkPropertyMetadataPtr meta = dependencyProperty->PropertyMetadata;
			if (meta)
			{
				if (Enums::Contains(meta->FrameworkPropertyMetadataOptionsFlags(), FrameworkPropertyMetadataOptions::AffectsUpdate))
				{
					Updated(dependencyProperty, args);
				}
			}
        }
    }

    inheritanceContext = inheritanceContext;
}

void DependencyObject::ClearValue(const DependencyPropertyPtr& dependencyProperty )
{
    auto entryIt = valueEntries.find(dependencyProperty);

    if(entryIt != end(valueEntries))
    {
       DependantValueEntryPtr& entry = (*entryIt).second;

       if(!entry->HasBaseValue())
       {
           return;
       }
       auto oldValue = GetValue(dependencyProperty);

       entry->BaseValue(nullptr);

       auto newValue = GetValue(dependencyProperty);

       if (AreValuesSame(dependencyProperty, oldValue, newValue))
       {
           return;
       }
	   auto changeCallback = dependencyProperty->PropertyMetadata->ChangeCallback();

	   auto args = PropertyChangedEventArgs(oldValue, newValue);
	   if (changeCallback && dependencyProperty->IsOfOwnerType(this))
	   {
		   changeCallback(this, args);
	   }
	   OnPropertyChanged(dependencyProperty, args);
	   FrameworkPropertyMetadataPtr meta = dependencyProperty->PropertyMetadata;
	   if (meta)
	   {
		   if (Enums::Contains(meta->FrameworkPropertyMetadataOptionsFlags(), FrameworkPropertyMetadataOptions::AffectsUpdate))
		   {
			   Updated(dependencyProperty, args);
		   }
	   }
    }
}

void DependencyObject::SetInheritedValue(const DependencyPropertyPtr& property, const SharedVoid& value )
{
    DependantValueEntryPtr entry;

    auto entryIt = valueEntries.find(property);

    if(entryIt != end(valueEntries))
    {
        auto& entry = (*entryIt).second;

        if(entry->IsAnimated() || entry->HasBaseValue())
        {
            entry->InheritatedValue(value);
            return;
        }
        
        SetValueImpl(property, value, false, ValueSource::Inherited);
    }
}


bool DependencyObject::AreValuesSame(const DependencyPropertyPtr& dependencyProperty,
                                     const SharedVoid& oldValue, 
                                     const SharedVoid& newValue )
{
	ensureDependencyProperty(dependencyProperty);
    auto equalityCallback = dependencyProperty->PropertyMetadata->EqualityCallback();
    if(equalityCallback)
    {
        /* check if one or the other is null first */
        if((!oldValue && newValue) || (oldValue && !newValue))
            return false;

        return equalityCallback(oldValue, newValue);
    }
    else
    {
        return (oldValue == newValue);
    }
}

void DependencyObject::Initialize()
{

}

void DependencyObject::OnPropertyChanged(const DependencyPropertyPtr&, const PropertyChangedEventArgs&)
{

}

void DependencyObject::Updated(const DependencyPropertyPtr&, const PropertyChangedEventArgs&)
{

}
