#pragma once
#include "DependencyProperty.h"
#include "DependantValueEntry.h"
#include "RoutedEvent.h"

#include <map>
#include <memory>
#include <type_traits>

namespace System
{
	namespace UI
	{
		class DependencyObject;
		typedef std::shared_ptr<DependencyObject> DependencyObjectPtr;
		class RoutedEventRoute;

		class EventManager_impl;

		class UICORE_API IRoutedEventDelegate: public Delegate
		{
		public:
			virtual bool IgnoreIsHandled() const = 0;
			virtual bool GenericInvoke(const Any& sender, RoutedEventArgs& args, bool nothrow = true) const = 0;

			bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow) const override;
			bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow) const override;
			bool HasReturn() const override;
		};
		typedef std::shared_ptr<IRoutedEventDelegate> IRoutedEventDelegatePtr;

		template<class TEventArgs>
		class UICORE_API RoutedEventDelegate : public IRoutedEventDelegate
		{
			static_assert(System::is_same_or_base_of<RoutedEventArgs, TEventArgs>::value, "TEventArgs must be based of System::RoutedEventArgs");
		public:
			typedef std::function<void(DependencyObjectPtr, TEventArgs&)> CallbackFunction;
		private:
			bool _ignoreIsHandled;
			CallbackFunction _funptr;
		public:

			bool IgnoreIsHandled() const override { return _ignoreIsHandled; }

			RoutedEventDelegate(CallbackFunction fun, bool ignoreIsHandled) : _funptr(fun), _ignoreIsHandled(ignoreIsHandled)
			{

			}

			RoutedEventDelegate(const RoutedEventDelegate& eventDelegate)
			{
				_funptr = eventDelegate._funptr;
				_ignoreIsHandled = eventDelegate._ignoreIsHandled;
			}

			RoutedEventDelegate(RoutedEventDelegate&& eventDelegate)
			{
				_funptr = eventDelegate._funptr;
				_ignoreIsHandled = eventDelegate._ignoreIsHandled;
			}

			bool GenericInvoke(const Any& sender, RoutedEventArgs& args, bool nothrow = true) const override
			{
				try
				{
					DependencyObjectPtr obj = AnyVisitor<DependencyObjectPtr>::Get(sender);
					TEventArgs& Targs = dynamic_cast<TEventArgs&>(args);
					if (obj) _funptr(obj, Targs);
				}
				catch (...)
				{
					if (!nothrow) throw;
				}
				return false;
			}
		};

		class UICORE_API RoutedEventList final
		{
		private:
			friend class EventManager;
			friend class EventManager_impl;
			friend DependencyObject;
			template<class TEventArgs>
			friend class RoutedEventHandler;
			RoutedEventPtr _dEvent;
			mutable map<EventToken, DelegatePtr> _delegates;
			mutable atomic<EventToken> _tokens;

			RoutedEventList(RoutedEventPtr);

			~RoutedEventList();

			EventToken Add(DelegatePtr) const;

			EventToken Remove(EventToken) const;

		public:
			RoutedEventPtr GetDEvent() const;
			_declspec(property(get = GetDEvent)) RoutedEventPtr DEvent;
		};

		class MulticastRoutedEventDelegate;

		class UICORE_API RoutedEventHandler_base
		{
		protected:
			friend DependencyObject;
			friend MulticastRoutedEventDelegate;

			virtual bool Invoke(DependencyObjectPtr sender, RoutedEventArgs& args, bool nothrow = true) const;
		};

		template<class TEventArgs>
		class RoutedEventHandler final: public RoutedEventHandler_base
		{
			static_assert(System::is_same_or_base_of<RoutedEventArgs, TEventArgs>::value, "TEventArgs must be based on or same as RoutedEventArgs");	
		public:
			typedef std::function<void(DependencyObjectPtr, TEventArgs&)> CallbackFunction;

			RoutedEventHandler() : RoutedEventHandler_base()
			{

			}

			/// <summary>
			/// 注册回调
			/// </summary>
			/// <param name="f">回调函数地址</param>
			/// <returns>事件标记</returns>
			EventToken operator += (CallbackFunction f) const
			{
				auto token = m_tokenCounter++;

				m_handlerMap[token] = f;
				return token;
			}

			/// <summary>
			/// 注销回调
			/// </summary>
			/// <param name="token">事件标记</param>
			/// <returns>事件标记</returns>
			EventToken operator -= (EventToken token) const
			{
				m_handlerMap.erase(token);
				return token;
			}
			
			bool Invoke(DependencyObjectPtr sender, RoutedEventArgs& args, bool nothrow = false) const override
			{
				if (nothrow)
				{
					try
					{
						for (auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++)
						{
							TEventArgs& targ = dynamic_cast<TEventArgs&>(args);
							(*i).second(sender, targ);
						}
						return true;
					}
					catch (...)
					{

					}
				}
				else
				{
					for (auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++)
					{
						TEventArgs& targ = dynamic_cast<TEventArgs&>(args);
						(*i).second(sender, targ);
					}
					return true;
				}
				return false;
			}


		private:
			friend DependencyObject;


			mutable std::atomic<EventToken> m_tokenCounter;
			mutable std::map<EventToken, CallbackFunction> m_handlerMap;

		};

		class DependencyObjectType;
		class DependencyObjectType_impl;
		typedef std::shared_ptr<DependencyObjectType> DependencyObjectTypePtr;

		__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API DependencyObjectType: __uobject
		{
		private:
			friend DependencyObjectType_impl;
			int _id;
			_type _systemType;
			DependencyObjectTypePtr _baseDType;
			DependencyObjectType();
			static DependencyObjectTypePtr Create();
		public:
			static DependencyObjectTypePtr FromSystemType(_type systemType);

			bool IsInstanceOfType(DependencyObject* obj);
			bool IsSubclassOf(DependencyObjectTypePtr ty);

			uhash GetHashCode() const override;

			int GetId() const;
			_type GetSystemType() const;
			const std::wstring& GetName() const;
			DependencyObjectTypePtr GetBaseType() const;

			_declspec(property(get = GetId)) int Id;
			_declspec(property(get = GetSystemType)) _type SystemType;
			_declspec(property(get = GetName)) const std::wstring& Name;
			_declspec(property(get = GetBaseType)) DependencyObjectTypePtr BaseType;

			REFLECT_CLASS(DependencyObjectType);
		};

		class EventHandlersStore;

		__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API DependencyObject : public IInvalidatable, __vuobject, std::enable_shared_from_this<DependencyObject>
		{
			friend DependencyProperty;
			template<class TEventArgs>
			friend class RoutedEventHandler;
		public:
			DependencyObject();
			virtual ~DependencyObject();

			DependencyObjectTypePtr GetDependencyObjectType() const;

			__uproperty(get = GetDependencyObjectType) DependencyObjectTypePtr DependencyObjectType;

		public:
			template <typename T>
			typename std::enable_if<std::is_base_of<DependantObject, T>::value, std::shared_ptr<T>>::type
				GetValuePtr(const DependencyPropertyPtr& dependencyProperty)  const
			{
				auto val = GetValue(dependencyProperty);

				return std::dynamic_pointer_cast<T>(std::static_pointer_cast<DependantObject>(val));
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, T>::type
				GetValue(const DependencyPropertyPtr& dependencyProperty) const
			{
				auto val = GetValue(dependencyProperty);

				return *static_cast<T*>(val.get());
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, T&>::type
				GetValueRef(const DependencyPropertyPtr& dependencyProperty) const
			{
				auto val = GetValue(dependencyProperty);

				return *static_cast<T*>(val.get());
			}

			SharedVoid GetValue(const DependencyPropertyPtr& dependencyProperty) const;

		public:
			template <class T>
			void SetValue(const DependencyPropertyPtr& dependencyProperty, const T& value)
			{
				/* probably use template specialization instead of SFINAE */
				SetValueTemplate<T>(dependencyProperty, value);
			}

			void ClearValue(const DependencyPropertyPtr& dependencyProperty);

			virtual void Invalidate(InvalidateFlags flags)
			{
			}

		protected:
			template<class TEventArgs>
			void RegisterRoutedEvent(RoutedEventHandler<TEventArgs>* evtHandler, RoutedEventPtr dEvent, bool handledEventsToo = false)
			{
				ensureRoutedEvent(dEvent);
				std::shared_ptr<RoutedEventHandler<TEventArgs>> evtptr{ evtHandler, null_deleter() };
				std::shared_ptr<RoutedEventHandler_base> uievtbase = std::dynamic_pointer_cast<RoutedEventHandler_base>(evtptr);
				RegisterRoutedEventBase(uievtbase, dEvent, handledEventsToo);
			}

			virtual DependencyObjectPtr AdjustEventSource(RoutedEventArgs& args) const;

			virtual bool BuildRouteCore(RoutedEventRoute*, RoutedEventArgs&);

		private:
			void RegisterRoutedEventBase(std::shared_ptr<RoutedEventHandler_base> uievtbase, RoutedEventPtr dEvent, bool handledEventsToo);
			void BuildRoute(RoutedEventRoute*, RoutedEventArgs&);
			static void BuildRouteHelper(DependencyObjectPtr e, RoutedEventRoute* route, RoutedEventArgs& args);
			virtual void OnAddToEventRoute(RoutedEventRoute*, RoutedEventArgs&) const;
			void AddToEventRoute(RoutedEventRoute*, RoutedEventArgs&);
			virtual void OnAddInputPreHandler(RoutedEventRoute*, RoutedEventArgs&) const;
			void AddInputPreHandler(RoutedEventRoute*, RoutedEventArgs&);
			virtual void OnAddInputPostHandler(RoutedEventRoute*, RoutedEventArgs&) const;
			void AddInputPostHandler(RoutedEventRoute*, RoutedEventArgs&);
		private:
			void ensureDependencyProperty(const DependencyPropertyPtr&);
			void ensureRoutedEvent(const RoutedEventPtr&);
		public:
			void RaiseEvent(RoutedEventArgs& e);
		protected:
			static void ReRaiseEventAs(DependencyObjectPtr sender, RoutedEventArgs& args, RoutedEventPtr newEvent);

			bool ContainsDependencyProperty(const DependencyPropertyPtr& dependencyProperty);

			bool ContainsDependencyProperty(const wstring& dependencyPropertyName, StringComparison comparsion = StringComparison::CurrentCulture);

			template <class T>
			void InternalSetValue(const DependencyPropertyPtr& dependencyProperty, const T& value)
			{
				/* probably use template specialization instead of SFINAE */
				SetValueTemplate<T>(dependencyProperty, value, true);
			}

			void SetInheritanceContext(DependencyObjectPtr inheritanceContext);

			virtual void OnPropertyChanged(const DependencyPropertyPtr& property, const PropertyChangedEventArgs&);

			virtual void Updated(const DependencyPropertyPtr& property, const PropertyChangedEventArgs&);

			virtual void SetInheritedValue(const DependencyPropertyPtr& property, const SharedVoid& value);

			virtual void Initialize();
		private:
			enum class ValueSource
			{
				Inherited,
				Explicit,
				Animation
			};

			template <typename T>
			void
				SetValueTemplate(const DependencyPropertyPtr& dp, const DependantObjectPtr& value, bool noevent = false)
			{
				auto newVal = std::static_pointer_cast<DependantObject>(value);
				SetValueImpl(dp, value, noevent);
			}

			template <typename T>
			typename std::enable_if<std::is_assignable<SharedVoid, T>::value && !std::is_assignable<DependantObjectPtr, T>::value>::type
				SetValueTemplate(const DependencyPropertyPtr& dp, const T& value, bool noevent = false)
			{
				SetValueImpl(dp, value, noevent);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value>::type
				SetValueTemplate(const DependencyPropertyPtr& dp, const T& value, bool noevent = false)
			{
				auto boxedValue = std::make_shared<T>(value);
				SetValueImpl(dp, boxedValue, noevent);
			}

			void SetValueImpl(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& value, bool noevent = false, const ValueSource valueSource = ValueSource::Explicit);

		private:
			bool AreValuesSame(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& oldValue, const SharedVoid& newValue);
			bool CoerceValue(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& oldValue, SharedVoid& newValue);
		private:
			SharedVoid GetValueInternal(const DependencyPropertyPtr& dependencyProperty);
		private:
			std::shared_ptr<EventHandlersStore> eventStore;
			std::map<const DependencyPropertyPtr, DependantValueEntryPtr> valueEntries;
			bool                                                    isInvalidatable;
			DependencyObjectPtr                                       inheritanceContext;
			bool                                                    inheritanceContextSet;
			std::vector<DependantObjectPtr> queuedDependantObjects; /* workaround for shared_from this in ctor */
			mutable DependencyObjectTypePtr dType;

			REFLECT_CLASS(DependencyObject)
		};
	}
}