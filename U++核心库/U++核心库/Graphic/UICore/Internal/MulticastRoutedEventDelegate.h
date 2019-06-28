#pragma once
#include "..\DependencyObject.h"
#include <set>
namespace System
{
	namespace UI
	{
		class MulticastRoutedEventDelegate;
		typedef std::shared_ptr<MulticastRoutedEventDelegate> MulticastRoutedEventDelegatePtr;


		class MulticastRoutedEventDelegate : public IRoutedEventDelegate
		{
		private:
			bool _ignoreIsHandled;
			std::shared_ptr<RoutedEventHandler_base> _base;
			std::set<MulticastRoutedEventDelegatePtr> _children;
		public:

			bool IgnoreIsHandled() const override;

			MulticastRoutedEventDelegate();

			MulticastRoutedEventDelegate(std::shared_ptr<RoutedEventHandler_base> base, bool ignoreIsHandled);

			MulticastRoutedEventDelegate(const MulticastRoutedEventDelegate& eventDelegate);

			MulticastRoutedEventDelegate(MulticastRoutedEventDelegate&& eventDelegate);

			bool IsEmpty() const;

			void Combine(MulticastRoutedEventDelegatePtr child);

			void Remove(MulticastRoutedEventDelegatePtr child);

			bool GenericInvoke(const Any& sender, RoutedEventArgs& args, bool nothrow = true) const override;
		};

	}
}
