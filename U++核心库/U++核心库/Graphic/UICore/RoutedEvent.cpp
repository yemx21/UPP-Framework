#include "RoutedEvent.h"
#include "DependencyObject.h"
#include "EventManager.h"
#include "Internal\EventManager_impl.h"
using namespace System::UI;

bool RoutedEventArgs::GetIsHandled() const
{
	return _isHandled;
}

void RoutedEventArgs::SetIsHandled(bool val)
{
	if (_dependencyEvent) _isHandled = val;
}

DependencyObjectPtr RoutedEventArgs::GetOriginalSource() const
{
	return _originalSource;
}

RoutedEventPtr RoutedEventArgs::GetRoutedEvent() const
{
	return _dependencyEvent;
}

void RoutedEventArgs::SetRoutedEvent(RoutedEventPtr DEvent)
{
	if (!_isInvoking) _dependencyEvent = DEvent;
}

DependencyObjectPtr RoutedEventArgs::GetSource() const
{
	return _source;
}

void RoutedEventArgs::SetSource(DependencyObjectPtr value)
{
	if (_isInvoking || !_dependencyEvent) return;

	DependencyObjectPtr obj = value;
	if (_source || _originalSource)
	{
		if (_source != obj)
		{
			_source = obj;
			OnSetSource(obj);
		}
		return;
	}
	DependencyObjectPtr obj1 = obj;
	DependencyObjectPtr obj2 = obj1;
	_originalSource = obj1;
	_source = obj2;
	OnSetSource(obj);
}

RoutedEventArgs::RoutedEventArgs() : _isInvoking(false), _source(nullptr), _originalSource(nullptr), _isHandled(false)
{

}

RoutedEventArgs::RoutedEventArgs(RoutedEventPtr dEvent): _isInvoking(false), _source(nullptr), _originalSource(nullptr), _isHandled(false)
{ 
	_dependencyEvent = dEvent;
}

RoutedEventArgs::RoutedEventArgs(RoutedEventPtr dEvent, DependencyObjectPtr source): _isInvoking(false), _isHandled(false)
{
	_dependencyEvent = dEvent;
	DependencyObjectPtr obj = source;
	DependencyObjectPtr obj1 = obj;
	_originalSource = obj;
	_source = obj1;
}

void RoutedEventArgs::InvokeEventHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr sender)
{
	if (handler == nullptr || !sender || !_dependencyEvent) return;
	_isInvoking = true;
	try
	{
		handler->GenericInvoke(sender, *this);
		_isInvoking = false;
	}
	catch (...)
	{
		_isInvoking = false;
	}
}

void RoutedEventArgs::InvokeHandler(IRoutedEventDelegatePtr handler, DependencyObjectPtr  sender)
{
	_isInvoking = true;
	try
	{
		InvokeEventHandler(handler, sender);
		_isInvoking = false;
	}
	catch (...)
	{
		_isInvoking = false;
	}
}

void RoutedEventArgs::OnSetSource(DependencyObjectPtr source)
{
}

IRoutedEvent::IRoutedEvent(const wstring& name, System::UI::EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType) : _name(name), _eventStrategy(eventStrategy), _handlerType(handlerType), _ownerType(ownerType)
{
	_globalIndex = EventManager_impl::GetNextAvailableGlobalIndex(this);
}

const Type* IRoutedEvent::GetHandlerType() const
{
	return _handlerType;
}

const wchar_t*  IRoutedEvent::GetName() const
{
	return _name->c_str();
}

const Type* IRoutedEvent::GetOwnerType() const
{
	return _ownerType;
}

System::UI::EventStrategy IRoutedEvent::GetEventStrategy() const
{
	return _eventStrategy;
}


RoutedEvent::RoutedEvent(fun__ustatic_ctor_impl fun): impl(nullptr), fun_ustaticdp(fun) {}


void RoutedEvent::Register1(const std::wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType)
{
	impl = std::shared_ptr<IRoutedEvent>(EventManager::RegisterRoutedEvent(name, eventStrategy, handlerType, ownerType));
}

void RoutedEvent::Register1(const std::wstring& name, EventStrategy eventStrategy, const Type* handlerType, const Type* ownerType, const Type* inheritType)
{
	impl = std::shared_ptr<IRoutedEvent>(EventManager::RegisterRoutedEventEx(name, eventStrategy, handlerType, ownerType, inheritType));
}


std::shared_ptr<IRoutedEvent> RoutedEvent::operator ->()
{
	if (fun_ustaticdp) fun_ustaticdp();
	return impl;
}

std::shared_ptr<IRoutedEvent> RoutedEventPtr::operator->() const noexcept
{
	return (this->_Get()->operator ->());
}
