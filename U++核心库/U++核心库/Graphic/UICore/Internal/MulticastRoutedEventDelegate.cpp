#include "MulticastRoutedEventDelegate.h"
using namespace System::UI;

bool MulticastRoutedEventDelegate::IgnoreIsHandled() const 
{ 
	return _ignoreIsHandled; 
}

MulticastRoutedEventDelegate::MulticastRoutedEventDelegate() : _base(nullptr), _ignoreIsHandled(false)
{
}

MulticastRoutedEventDelegate::MulticastRoutedEventDelegate(std::shared_ptr<RoutedEventHandler_base> base, bool ignoreIsHandled) : _base(base), _ignoreIsHandled(ignoreIsHandled)
{

}

MulticastRoutedEventDelegate::MulticastRoutedEventDelegate(const MulticastRoutedEventDelegate& eventDelegate)
{
	_base = eventDelegate._base;
	_children = eventDelegate._children;
	_ignoreIsHandled = eventDelegate._ignoreIsHandled;
}

MulticastRoutedEventDelegate::MulticastRoutedEventDelegate(MulticastRoutedEventDelegate&& eventDelegate)
{
	_base = eventDelegate._base;
	_children = eventDelegate._children;
	_ignoreIsHandled = eventDelegate._ignoreIsHandled;
}

bool MulticastRoutedEventDelegate::IsEmpty() const
{
	return !_base;
}

void MulticastRoutedEventDelegate::Combine(MulticastRoutedEventDelegatePtr child)
{
	_children.insert(child);
}

void MulticastRoutedEventDelegate::Remove(MulticastRoutedEventDelegatePtr child)
{
	if (child.get() == this)
		_base = nullptr;
	else
		_children.erase(child);
}

bool MulticastRoutedEventDelegate::GenericInvoke(const Any& sender, RoutedEventArgs& args, bool nothrow) const
{
	try
	{
		if (_base)
		{
			DependencyObjectPtr obj = AnyVisitor<DependencyObjectPtr>::Get(sender);
			RoutedEventArgs& Targs = dynamic_cast<RoutedEventArgs&>(args);
			if (obj)
			{
				_base->Invoke(obj, Targs);
				for (auto& handler : _children)
				{
					handler->GenericInvoke(sender, args, nothrow);
				}

				return true;
			}
		}
	}
	catch (...)
	{
		if (!nothrow) throw;
	}
	return false;
}