#pragma once
#include "DependencyProperty.h"

namespace System
{
	namespace UI
	{
		class DependantValueEntry;
		typedef std::shared_ptr<DependantValueEntry> DependantValueEntryPtr;

		class DependencyObject;

		class DependantValueEntry
		{
		public:
			DependantValueEntry(const DependencyPropertyPtr& dependencyProperty, DependencyObject* const dependencyObject);

			virtual ~DependantValueEntry();

			DependencyPropertyPtr DependencyProperty() const;

			SharedVoid BaseValue() const;
			void BaseValue(const SharedVoid val);

			SharedVoid AnimatedValue() const;
			void AnimatedValue(const SharedVoid& val);

			SharedVoid InheritatedValue() const;
			void InheritatedValue(const SharedVoid& val);

			bool HasCalculatedValue() const;
			bool HasBaseValue() const;

			bool HasInheritedValue() const;

			bool HasAnimatedValue() const;

			void UnregisterDependants();

			SharedVoid CalculatedValue()  const;
			bool IsAnimated() const;
		private:
			void RegisterDependantCalculatedValue();
			DependencyPropertyPtr dependencyProperty;
			SharedVoid            baseValue;
			SharedVoid            animatedValue;
			SharedVoid            inheritatedValue;
			bool                  isAnimated;
			DependencyObject*     dependencyObject;
		};
	}
}