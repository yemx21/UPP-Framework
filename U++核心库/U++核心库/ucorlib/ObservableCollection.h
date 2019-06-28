#pragma once
#include "Config.h"
#include "Common.h"
#include "CriticalSection.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <atomic>


namespace System
{
	/// <summary>
	/// 集合的项更改动作枚举
	/// </summary>
	enum class NotifyCollectionChangedAction
	{
		/// <summary>
		/// 添加
		/// </summary>
		Add,
		/// <summary>
		/// 插入
		/// </summary>
		Insert,
		/// <summary>
		/// 移除
		/// </summary>
		Remove,
		/// <summary>
		/// 替换
		/// </summary>
		Replace,
		/// <summary>
		/// 移动
		/// </summary>
		Move,
		/// <summary>
		/// 重置
		/// </summary>
		Reset,
		/// <summary>
		/// 子项更新
		/// </summary>
		Updated,
	};

	/// <summary>
	/// 实例
	/// </summary>
	class Instance
	{
	protected:
		intptr_t _id;
	public:
		Instance() { _id = (intptr_t)(this); }
		auto getid()-> const intptr_t { return _id; }
		_declspec(property(get = getid)) intptr_t id;

		bool InstanceEquals(const Instance* obj) const
		{
			if (!obj) return false;
			return InstanceEquals(*obj);
		}

		virtual bool InstanceEquals(const Instance& obj) const
		{
			return obj._id == _id;
		}
	};

	/// <summary>
	/// 可监视宿主
	/// </summary>
	template<class T, class BASET, bool isSharedPtr = is_shared_ptr<T>::value>
	class IObservableOwner;

	template<class T, typename BASET>
	/// <summary>
	/// 可监视宿主
	/// </summary>
	class IObservableOwner<T, BASET, false> :public Instance
	{
	protected:
		friend class ObservableObject<typename BASET>;
		virtual void Notify(T* sender) = 0;

	};

	template<class T, class BASET>
	/// <summary>
	/// 可监视宿主
	/// </summary>
	class IObservableOwner<T, BASET, true> :public Instance
	{
	protected:
		friend class ObservableObject<typename BASET>;
		virtual void Notify(T* sender) = 0;

	};

	/// <summary>
	/// 对象比较器
	/// </summary>
	struct IObjectComparer
	{
	public:
		Instance* it;
		explicit IObjectComparer(Instance* s) : it(s) {}
		bool operator ()(Instance* e) { return it != nullptr ? it->InstanceEquals(e) : it == e; }
	};


	class IObservableObject
	{
	protected:
		template <class T, bool isObservable, bool isSharedObservablePtr>
		friend class ObservableCollection;

		virtual void RegisterOwner(void* owner)
		{

		}

		virtual void UnRegisterOwner(void* owner)
		{
		}

		virtual void RegisterSharedOwner(void* owner)
		{

		}

		virtual void UnRegisterSharedOwner(void* owner)
		{
		}

	public:
		virtual void NotifyChanged(const wchar_t* notifyname, ...)
		{

		}
	};


	template<class T>
	/// <summary>
	/// 可监视对象
	/// </summary>
	class ObservableObject : std::enable_shared_from_this<ObservableObject<T>>, public IObservableObject
	{
	private:
		std::vector<IObservableOwner<ObservableObject<T>, T>*> owners;
		std::vector<IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>*> sharedOwners;
	protected:
		friend class ObservableCollection<ObservableObject<T>, true, false>;
		friend class ObservableCollection<ObservableObject<T>, false, false>;

		friend class ObservableCollection<std::shared_ptr<ObservableObject<T>>, true, true>;
		friend class ObservableCollection<std::shared_ptr<ObservableObject<T>>, false, true>;

		virtual bool OnNotifyChanged(const std::wstring& notifyname) { return true; }
	public:

		using basetype = T;

		ObservableObject()
		{
			owners = std::vector<IObservableOwner<ObservableObject<T>, T>*>(0);
			sharedOwners = std::vector<IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>*>(0);
		}

		/// <summary>
		/// 提示更改
		/// </summary>	
		///<remarks>提示名称不为空，多值必须以L""结尾</remarks>
		/// <param name ='notifyname'>非空值的提示名称</param>
		/// <returns></returns>
		void NotifyChanged(const wchar_t* notifyname, ...) override
		{
			bool result = false;
			va_list vl;
			va_start(vl, notifyname);
			std::wstring name = notifyname != nullptr ? notifyname : L"";
			while (!name.empty())
			{
				if (OnNotifyChanged(std::wstring(name))) result = true;
				name = va_arg(vl, wchar_t*);
			}
			va_end(vl);

			if (result)
			{
				for (typename std::vector<IObservableOwner<ObservableObject<T>, T>*>::iterator iter = owners.begin(); iter != owners.end(); ++iter)
				{
					IObservableOwner<ObservableObject<T>, T>* owner = *iter;
					ObservableObject* staticthis = const_cast<ObservableObject*>(this);
					if (owner) owner->Notify(staticthis);
				}
				for (typename std::vector<IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>*>::iterator iter = sharedOwners.begin(); iter != sharedOwners.end(); ++iter)
				{
					IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>* owner = *iter;
					auto tmp = this->shared_from_this();
					if (owner) owner->Notify(&tmp);
				}
			}
		}
	protected:
		void RegisterOwner(void* owner) override
		{
			IObservableOwner<ObservableObject<T>, T>* host = static_cast<IObservableOwner<ObservableObject<T>, T>*>(owner);
			if (std::find(owners.begin(), owners.end(), host) != owners.end()) return;
			owners.push_back(host);
		}

		void UnRegisterOwner(void* owner) override
		{
			IObservableOwner<ObservableObject<T>, T>* host = static_cast<IObservableOwner<ObservableObject<T>, T>*>(owner);
			typename std::vector<IObservableOwner<ObservableObject<T>, T>*>::iterator iter = std::find_if(owners.begin(), owners.end(), IObjectComparer(host));
			if (iter != owners.end()) owners.erase(iter);
		}

		void RegisterSharedOwner(void* owner) override
		{
			IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>* host = static_cast<IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>*>(owner);
			if (std::find(sharedOwners.begin(), sharedOwners.end(), host) != sharedOwners.end()) return;
			sharedOwners.push_back(host);
		}

		void UnRegisterSharedOwner(void* owner) override
		{
			IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>* host = static_cast<IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>*>(owner);
			typename std::vector<IObservableOwner<std::shared_ptr<ObservableObject<T>>, T>*>::iterator iter = std::find_if(sharedOwners.begin(), sharedOwners.end(), IObjectComparer(host));
			if (iter != sharedOwners.end()) sharedOwners.erase(iter);
		}
	};


	template<class _Ty, bool isSharedPtr = is_shared_ptr<_Ty>::value>
	/// <summary>
	/// 集合通知项
	/// </summary>	
	class NotifyCollectionItem;

	template<class _Ty>
	/// <summary>
	/// 集合通知项
	/// </summary>	
	class NotifyCollectionItem<_Ty, false>
	{
	public:

	private:
		int m_Index;
		std::shared_ptr<_Ty> m_Value;
	public:
		NotifyCollectionItem(int index, const _Ty& value) : m_Value(std::make_shared<_Ty>(value)), m_Index(index) {}
		NotifyCollectionItem(const NotifyCollectionItem& other)
		{
			m_Value = other.m_Value;
			m_Index = other.m_Index;
		}

		NotifyCollectionItem(NotifyCollectionItem&& other)
		{
			m_Value = other.m_Value;
			m_Index = other.m_Index;
		}

		NotifyCollectionItem& operator=(NotifyCollectionItem&& _Right)
		{
			if (m_Value != _Right.m_Value)
			{
				m_Value = _Right.m_Value;
			}
			m_Index = _Right.m_Index;
			return (*this);
		}

		NotifyCollectionItem& operator=(const NotifyCollectionItem& _Right)
		{
			if (m_Value != _Right.m_Value)
			{
				m_Value = _Right.m_Value;
			}
			m_Index = _Right.m_Index;
			return (*this);
		}

		bool operator == (const NotifyCollectionItem& item)
		{
			return item.m_Index == m_Index && item.m_Value == m_Value;
		}

		template <class T>
		T Value() const
		{
			return *(T*)(m_Value.get());
		}

		int getIndex()
		{
			return m_Index;
		}

		_declspec(property(get = getIndex)) int Index;
	};

	template<class _Ty>
	/// <summary>
	/// 集合通知项
	/// </summary>	
	class NotifyCollectionItem<_Ty, true>
	{

	private:
		NotifyCollectionItem() :m_Value(-1) {}
		int m_Index;
		_Ty m_Value;
	public:
		static NotifyCollectionItem Empty()
		{
			return NotifyCollectionItem();
		}

		bool isEmpty() const
		{
			return m_Index == -1 || m_Value.get() == NULL;
		}

		NotifyCollectionItem(int index, const _Ty& value) : m_Value(value), m_Index(index) {}
		NotifyCollectionItem(const NotifyCollectionItem& other)
		{
			m_Value = other.m_Value;
			m_Index = other.m_Index;
		}

		NotifyCollectionItem(NotifyCollectionItem&& other)
		{
			m_Value = other.m_Value;
			m_Index = other.m_Index;
		}

		NotifyCollectionItem& operator=(NotifyCollectionItem&& _Right)
		{
			if (m_Value != _Right.m_Value)
			{
				m_Value = _Right.m_Value;
			}
			m_Index = _Right.m_Index;
			return (*this);
		}

		NotifyCollectionItem& operator=(const NotifyCollectionItem& _Right)
		{
			if (m_Value != _Right.m_Value)
			{
				m_Value = _Right.m_Value;
			}
			m_Index = _Right.m_Index;
			return (*this);
		}

		bool operator == (const NotifyCollectionItem& item)
		{
			return item.m_Index == m_Index && item.m_Value == m_Value;
		}

		template <class T>
		T Value() const
		{
			return *(T*)(m_Value.get());
		}

		int getIndex()
		{
			return m_Index;
		}

		_declspec(property(get = getIndex)) int Index;
	};

	template<class _Ty>
	/// <summary>
	/// 集合通知事件参数
	/// </summary>	
	class NotifyCollectionChangedEventArgs
	{
	public:
		NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, const _Ty& oldValue, int oldIndex, const _Ty& newValue, int newIndex) :
			m_action(action)
		{
			m_oldItems.push_back(NotifyCollectionItem<_Ty>(oldIndex, oldValue));
			m_newItems.push_back(NotifyCollectionItem<_Ty>(newIndex, newValue));
		}

		NotifyCollectionChangedEventArgs() : m_action(NotifyCollectionChangedAction::Reset)
		{

		}

		NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action) : m_action(action)
		{

		}

		NotifyCollectionChangedEventArgs(const _Ty& value, int index, NotifyCollectionChangedAction action = NotifyCollectionChangedAction::Updated) : m_action(action)
		{
			if (action == NotifyCollectionChangedAction::Updated)
			{
				m_oldItems.push_back(NotifyCollectionItem<_Ty>(index, value));
				m_newItems.push_back(NotifyCollectionItem<_Ty>(index, value));
			}
			else if (action == NotifyCollectionChangedAction::Add || action == NotifyCollectionChangedAction::Insert)
			{
				m_newItems.push_back(NotifyCollectionItem<_Ty>(index, value));
			}
			else if (action == NotifyCollectionChangedAction::Remove)
			{
				m_oldItems.push_back(NotifyCollectionItem<_Ty>(index, value));
			}
		}

	protected:
		friend class ObservableCollection<_Ty, true, false>;
		friend class ObservableCollection<_Ty, true, true>;
		friend class ObservableCollection<_Ty, false, true>;
		friend class ObservableCollection<_Ty, false, false>;

		friend class ObservableCollection<std::shared_ptr<_Ty>, true, false>;
		friend class ObservableCollection<std::shared_ptr<_Ty>, true, true>;
		friend class ObservableCollection<std::shared_ptr<_Ty>, false, true>;
		friend class ObservableCollection<std::shared_ptr<_Ty>, false, false>;

		void AddOldItem(const _Ty& value, int index)
		{
			m_oldItems.push_back(NotifyCollectionItem<_Ty>(index, value));
		}

		void AddNewItem(const _Ty& value, int index)
		{
			m_newItems.push_back(NotifyCollectionItem<_Ty>(index, value));
		}

	public:
		auto getAction()-> const NotifyCollectionChangedAction
		{
			return m_action;
		}

		NotifyCollectionItem<_Ty> getOldItem(int index)
		{
			if (index >= 0 && index <= m_oldItems.size())
				return m_oldItems[index];
			else
				return NotifyCollectionItem<_Ty>::Empty();
		}

		NotifyCollectionItem<_Ty> getNewItem(int index)
		{
			if (index >= 0 && index <= m_newItems.size())
				return m_newItems[index];
			else
				return NotifyCollectionItem<_Ty>::Empty();
		}

		_declspec(property(get = getAction)) NotifyCollectionChangedAction Action;
		_declspec(property(get = getOldItem)) NotifyCollectionItem<_Ty> OldItems[];
		_declspec(property(get = getNewItem)) NotifyCollectionItem<_Ty> NewItems[];

	private:
		std::vector<NotifyCollectionItem<_Ty>> m_oldItems;
		std::vector<NotifyCollectionItem<_Ty>> m_newItems;
		NotifyCollectionChangedAction m_action;
	};


	template<class _Ty>
	/// <summary>
	/// 集合通知事件
	/// </summary>	
	class NotifyCollectionChangedEvent
	{
	public:
		NotifyCollectionChangedEvent() : m_tokenCounter(0)
		{

		}
		typedef std::function<void(const std::shared_ptr<ObservableCollection<_Ty>>&, const NotifyCollectionChangedEventArgs<_Ty>&)> CallbackFunction;

	public:
		void Invoke(const std::shared_ptr<ObservableCollection<_Ty>>& sender, const NotifyCollectionChangedEventArgs<_Ty>& arg)
		{
			for (auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++)
			{
				(*i).second(sender, arg);
			}
		}

		void operator()(const std::shared_ptr<ObservableCollection<_Ty>>& sender, const NotifyCollectionChangedEventArgs<_Ty>& arg)
		{
			Invoke(sender, arg);
		}

		EventToken operator += (CallbackFunction f) const
		{
			auto token = m_tokenCounter++;

			m_handlerMap[token] = f;
			return token;
		}

		EventToken operator -= (EventToken token) const
		{
			m_handlerMap.erase(token);
			return token;
		}

	private:
		mutable std::atomic<EventToken> m_tokenCounter;
		mutable std::map<EventToken, CallbackFunction> m_handlerMap;
	};


#pragma region ObservableCollection_Ty_false_false
	template<class _Ty>
	/// <summary>
	/// 可监视集合
	/// </summary>	
	class ObservableCollection<_Ty, false, false>
	{
	public:
		typedef typename std::vector<_Ty>::value_type value_type;
		typedef typename std::vector<_Ty>::iterator iterator;
		typedef typename std::vector<_Ty>::const_iterator const_iterator;
	protected:
		bool multiTask;
		CriticalSection cs;
		std::vector<_Ty> innerList;
		std::shared_ptr<ObservableCollection<_Ty, false, false>> sharedthis;
	public:
		NotifyCollectionChangedEvent<_Ty> CollectionChanged;
		ObservableCollection() : multiTask(false)
		{
			sharedthis = std::make_shared<ObservableCollection<_Ty, false, false>>(*this);
		}

		~ObservableCollection()
		{
		}

		ObservableCollection(const ObservableCollection& other) : multiTask(false)
		{
			innerList = std::vector<_Ty>(other.innerList);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection(ObservableCollection&& other) : multiTask(false)
		{
			innerList = std::vector<_Ty>(other.innerList);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection& operator=(ObservableCollection&& _Right)
		{
			if (innerList != _Right.innerList)
			{
				if (!multiTask)
				{
					cs.Enter();
					innerList = _Right.innerList;
					cs.Leave();
				}
				else
					innerList = _Right.innerList;
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
			}
			return (*this);
		}

		ObservableCollection& operator=(const ObservableCollection& _Right)
		{
			if (innerList != _Right.innerList)
			{
				if (!multiTask)
				{
					cs.Enter();
					innerList = _Right.innerList;
					cs.Leave();
				}
				else
					innerList = _Right.innerList;
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
			}
			return (*this);
		}

		iterator begin() noexcept
		{
			return innerList.begin();
		}

		const_iterator begin() const noexcept
		{
			return innerList.begin();
		}

		iterator end() noexcept
		{
			return innerList.end();
		}

		const_iterator end() const noexcept
		{
			return innerList.end();
		}

		void enterMultiTasks(int reserveCount = -1)
		{
			if (!multiTask)
			{
				cs.Enter();
				multiTask = true;
				if (reserveCount > 0)
					if (reserveCount > innerList.size())
						innerList.reserve(reserveCount);
			}
		}

		void leaveMultiTasks()
		{
			if (multiTask)
			{
				cs.Leave();
				multiTask = false;
			}
		}

		void add(value_type&& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.push_back(_Val);
				cs.Leave();
			}
			else
				innerList.push_back(_Val);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		void add(const value_type& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.push_back(_Val);
				cs.Leave();
			}
			else
				innerList.push_back(_Val);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		iterator insert(iterator _Where, _Ty&& _Val)
		{
			try
			{
				if (!multiTask)
				{
					cs.Enter();
					iterator iter = innerList.insert(_Where, _Val);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
				else
				{
					iterator iter = innerList.insert(_Where, _Val);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		iterator insert(iterator _Where, const _Ty& _Val)
		{
			try
			{
				if (!multiTask)
				{
					cs.Enter();
					iterator iter = innerList.insert(_Where, _Val);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
				else
				{
					iterator iter = innerList.insert(_Where, _Val);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		void popback()
		{
			if (innerList.size() == 0)return;
			_Ty _Val{};
			if (!multiTask)
			{
				cs.Enter();
				_Val = innerList.back();
				innerList.pop_back();
				cs.Leave();
			}
			else
			{
				_Val = innerList.back();
				innerList.pop_back();
			}
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size(), NotifyCollectionChangedAction::Remove));
		}

		iterator remove(const_iterator _Where)
		{
			try
			{
				if (!multiTask)
				{
					cs.Enter();
					auto _Val = *_Where;
					auto ridx = std::distance(innerList.begin(), _Where);
					iterator iter = innerList.erase(_Where);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
					return iter;
				}
				else
				{
					auto _Val = *_Where;
					auto ridx = std::distance(innerList.begin(), _Where);
					iterator iter = innerList.erase(_Where);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
					return iter;
				}
			}
			catch (...)
			{
				iterator ret = innerList.erase(_Where, _Where);
				cs.Leave();
				return ret;
			}
		}

		iterator remove(std::function<bool(_Ty)> _Pred)
		{
			if (!multiTask)
			{
				bool found = false;
				_Ty _Val{};
				cs.Enter();
				iterator iter = std::find_if<iterator, std::function<bool(_Ty)>>(innerList.begin(), innerList.end(), _Pred);
				if (iter != innerList.end())
				{
					_Val = *iter;
					found = true;
				}
				cs.Leave();
				if (found)
				{
					auto ridx = std::distance(innerList.begin(), iter);
					iter = innerList.erase(iter);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
				}
				return iter;
			}
			else
			{
				bool found = false;
				_Ty _Val{};
				iterator iter = std::find_if<iterator, std::function<bool(_Ty)>>(innerList.begin(), innerList.end(), _Pred);
				if (iter != innerList.end())
				{
					_Val = *iter;
					found = true;
				}
				if (found)
				{
					auto ridx = std::distance(innerList.begin(), iter);
					iter = innerList.erase(iter);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
				}
				return iter;
			}
		}

		bool replace(value_type&& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			if (!multiTask)
			{
				cs.Enter();
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}
				cs.Leave();
			}
			else
			{
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}
			}
			return false;
		}

		bool replace(const value_type& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			if (!multiTask)
			{
				cs.Enter();
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}
				cs.Leave();
			}
			else
			{
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}

			}
			return false;
		}

		void clear()
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.clear();
				cs.Leave();
			}
			else
				innerList.clear();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		void reset(std::vector<_Ty> _Vector)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList = std::vector<_Ty>(_Vector);
				cs.Leave();
			}
			else
				innerList = std::vector<_Ty>(_Vector);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());

		}

		auto getAddress()->const _Ty*
		{
			return &innerList[0];
		}

		auto getSize()->const unsigned int
		{
			return innerList.size();
		}

		void set(const_iterator _Where, _Ty&& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
				cs.Leave();
			}
			else
			{
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
				cs.Leave();
			}
		}

		void set(const_iterator _Where, const _Ty& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
				cs.Leave();
			}
			else
			{
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
			}
		}

		const _Ty& get(const_iterator _Where) const
		{
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				int index = std::distance(innerList.begin(), _Where);
				return innerList[index];
			}
			return NULL;
		}

		const _Ty& get(unsigned int index) const
		{
			return innerList[index];
		}

		const _Ty& operator[](int index) const
		{
			return innerList[index];
		}

		_declspec(property(get = getSize)) unsigned int size;
		_declspec(property(get = getAddress)) const _Ty* address;

	};
#pragma endregion

#pragma region ObservableCollection_Ty_false_true
	template<class _Ty>
	/// <summary>
	/// 可监视集合
	/// </summary>	
	class ObservableCollection<_Ty, false, true>
	{
	public:
		typedef typename std::vector<_Ty>::value_type value_type;
		typedef typename std::vector<_Ty>::iterator iterator;
		typedef typename std::vector<_Ty>::const_iterator const_iterator;
	protected:
		bool multiTask;
		CriticalSection cs;
		std::vector<_Ty> innerList;
		std::shared_ptr<ObservableCollection<_Ty, false, true>> sharedthis;
	public:
		NotifyCollectionChangedEvent<_Ty> CollectionChanged;
		ObservableCollection() : multiTask(false)
		{
			sharedthis = std::make_shared<ObservableCollection<_Ty, false, true>>(*this);
		}

		~ObservableCollection()
		{
		}

		ObservableCollection(const ObservableCollection& other) : multiTask(false)
		{
			innerList = std::vector<_Ty>(other.innerList);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection(ObservableCollection&& other) : multiTask(false)
		{
			innerList = std::vector<_Ty>(other.innerList);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection& operator=(ObservableCollection&& _Right)
		{
			if (innerList != _Right.innerList)
			{
				if (!multiTask)
				{
					cs.Enter();
					innerList = _Right.innerList;
					cs.Leave();
				}
				else
					innerList = _Right.innerList;
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
			}
			return (*this);
		}

		ObservableCollection& operator=(const ObservableCollection& _Right)
		{
			if (innerList != _Right.innerList)
			{
				if (!multiTask)
				{
					cs.Enter();
					innerList = _Right.innerList;
					cs.Leave();
				}
				else
					innerList = _Right.innerList;
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
			}
			return (*this);
		}

		iterator begin() noexcept
		{
			return innerList.begin();
		}

		const_iterator begin() const noexcept
		{
			return innerList.begin();
		}

		iterator end() noexcept
		{
			return innerList.end();
		}

		const_iterator end() const noexcept
		{
			return innerList.end();
		}

		void enterMultiTasks(int reserveCount = -1)
		{
			if (!multiTask)
			{
				cs.Enter();
				multiTask = true;
				if (reserveCount > 0)
					if (reserveCount > innerList.size())
						innerList.reserve(reserveCount);
			}
		}

		void leaveMultiTasks()
		{
			if (multiTask)
			{
				cs.Leave();
				multiTask = false;
			}
		}

		void add(value_type&& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.push_back(_Val);
				cs.Leave();
			}
			else
				innerList.push_back(_Val);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		void add(const value_type& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.push_back(_Val);
				cs.Leave();
			}
			else
				innerList.push_back(_Val);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		iterator insert(iterator _Where, _Ty&& _Val)
		{
			try
			{
				if (!multiTask)
				{
					cs.Enter();
					iterator iter = innerList.insert(_Where, _Val);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
				else
				{
					iterator iter = innerList.insert(_Where, _Val);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		iterator insert(iterator _Where, const _Ty& _Val)
		{
			try
			{
				if (!multiTask)
				{
					cs.Enter();
					iterator iter = innerList.insert(_Where, _Val);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
				else
				{
					iterator iter = innerList.insert(_Where, _Val);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
					return iter;
				}
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		void popback()
		{
			if (innerList.size() == 0)return;
			_Ty _Val{};
			if (!multiTask)
			{
				cs.Enter();
				_Val = innerList.back();
				innerList.pop_back();
				cs.Leave();
			}
			else
			{
				_Val = innerList.back();
				innerList.pop_back();
			}
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size(), NotifyCollectionChangedAction::Remove));
		}

		iterator remove(const_iterator _Where)
		{
			try
			{
				if (!multiTask)
				{
					cs.Enter();
					auto _Val = *_Where;
					auto ridx = std::distance(innerList.begin(), _Where);
					iterator iter = innerList.erase(_Where);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
					return iter;
				}
				else
				{
					auto _Val = *_Where;
					auto ridx = std::distance(innerList.begin(), _Where);
					iterator iter = innerList.erase(_Where);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
					return iter;
				}
			}
			catch (...)
			{
				iterator ret = innerList.erase(_Where, _Where);
				cs.Leave();
				return ret;
			}
		}

		iterator remove(std::function<bool(_Ty)> _Pred)
		{
			if (!multiTask)
			{
				bool found = false;
				_Ty _Val{};
				cs.Enter();
				iterator iter = std::find_if<iterator, std::function<bool(_Ty)>>(innerList.begin(), innerList.end(), _Pred);
				if (iter != innerList.end())
				{
					_Val = *iter;
					found = true;
				}
				cs.Leave();
				if (found)
				{
					auto ridx = std::distance(innerList.begin(), iter);
					iter = innerList.erase(iter);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
				}
				return iter;
			}
			else
			{
				bool found = false;
				_Ty _Val{};
				iterator iter = std::find_if<iterator, std::function<bool(_Ty)>>(innerList.begin(), innerList.end(), _Pred);
				if (iter != innerList.end())
				{
					_Val = *iter;
					found = true;
				}
				if (found)
				{
					auto ridx = std::distance(innerList.begin(), iter);
					iter = innerList.erase(iter);
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, ridx, NotifyCollectionChangedAction::Remove));
				}
				return iter;
			}
		}

		bool replace(value_type&& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			if (!multiTask)
			{
				cs.Enter();
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}
				cs.Leave();
			}
			else
			{
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}
			}
			return false;
		}

		bool replace(const value_type& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			if (!multiTask)
			{
				cs.Enter();
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}
				cs.Leave();
			}
			else
			{
				for (; _First != _Last; ++_First)
				{
					if (_Pred(*_First))
					{
						_Ty tmp = *_First;
						*_First = _Val;
						int index = std::distance(innerList.begin(), _First);
						cs.Leave();
						CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
						return true;
					}
				}

			}
			return false;
		}

		void clear()
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.clear();
				cs.Leave();
			}
			else
				innerList.clear();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		void reset(std::vector<_Ty> _Vector)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList = std::vector<_Ty>(_Vector);
				cs.Leave();
			}
			else
				innerList = std::vector<_Ty>(_Vector);
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());

		}

		auto getAddress()->const _Ty*
		{
			return &innerList[0];
		}

		auto getSize()->const unsigned int
		{
			return innerList.size();
		}

		void set(const_iterator _Where, _Ty&& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
				cs.Leave();
			}
			else
			{
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
				cs.Leave();
			}
		}

		void set(const_iterator _Where, const _Ty& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
				cs.Leave();
			}
			else
			{
				if (_Where >= innerList.begin() && _Where < innerList.end())
				{
					int index = std::distance(innerList.begin(), _Where);
					_Ty tmp = innerList[index];
					innerList[index] = _Val;
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return;
				}
			}
		}

		const _Ty& get(const_iterator _Where) const
		{
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				int index = std::distance(innerList.begin(), _Where);
				return innerList[index];
			}
			return NULL;
		}

		const _Ty& get(unsigned int index) const
		{
			return innerList[index];
		}

		const _Ty& operator[](int index) const
		{
			return innerList[index];
		}

		_declspec(property(get = getSize)) unsigned int size;
		_declspec(property(get = getAddress)) const _Ty* address;

	};
#pragma endregion

#pragma region ObservableCollection_Ty_true_false
	template<class _Ty>
	/// <summary>
	/// 可监视集合
	/// </summary>
	class ObservableCollection<_Ty, true, false> : public IObservableOwner<_Ty, typename _Ty::basetype>
	{
	public:
		typedef typename std::vector<_Ty>::iterator iterator;
		typedef typename std::vector<_Ty>::const_iterator const_iterator;
		using basetype = typename _Ty::basetype;
	protected:
		bool multiTask;
		CriticalSection cs;
		std::vector<_Ty> innerList;
		std::shared_ptr<ObservableCollection<_Ty, true, false>> sharedthis;
		void Notify(_Ty* sender) override
		{
			iterator iter = std::find<iterator>(begin(), end(), *sender);

			unsigned int index = std::distance(innerList.begin(), iter);
			if (index >= 0 && index < innerList.size())
			{
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(innerList[index], index));
			}
		}

	public:
		NotifyCollectionChangedEvent<_Ty> CollectionChanged;
		ObservableCollection() : multiTask(false)
		{
			sharedthis = std::make_shared<ObservableCollection<_Ty, true, false>>(*this);
		}
		~ObservableCollection()
		{
		}

		ObservableCollection(const ObservableCollection& other) : multiTask(false)
		{
			innerList = std::vector<_Ty>(other.innerList);

			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
				if (resource) resource->RegisterOwner(this);
			}

			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection(ObservableCollection&& other) : multiTask(false)
		{
			innerList = std::vector<_Ty>(other.innerList);

			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
				if (resource) resource->RegisterOwner(this);
			}
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection& operator=(ObservableCollection&& _Right)
		{
			if (!multiTask)
			{
				cs.Enter();
				if (innerList != _Right.innerList)
				{
					innerList = _Right.innerList;
				}
				for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
				{
					IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
					if (resource) resource->RegisterOwner(this);
				}
				cs.Leave();
			}
			else
			{
				if (innerList != _Right.innerList)
				{
					innerList = _Right.innerList;
				}
				for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
				{
					IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
					if (resource) resource->RegisterOwner(this);
				}
			}
			return (*this);
		}

		ObservableCollection& operator=(const ObservableCollection& _Right)
		{
			if (!multiTask)
			{
				cs.Enter();
				if (innerList != _Right.innerList)
				{
					innerList = _Right.innerList;
				}
				for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
				{
					IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
					if (resource) resource->RegisterOwner(this);
				}
				cs.Leave();
			}
			else
			{
				if (innerList != _Right.innerList)
				{
					innerList = _Right.innerList;
				}
				for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
				{
					IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
					if (resource) resource->RegisterOwner(this);
				}
			}
			return (*this);
		}

		iterator begin() noexcept
		{
			return innerList.begin();
		}

		const_iterator begin() const noexcept
		{
			return innerList.begin();
		}

		iterator end() noexcept
		{
			return innerList.end();
		}

		const_iterator end() const noexcept
		{
			return innerList.end();
		}

		void add(_Ty&& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.push_back(_Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&_Val);
				if (resource) resource->RegisterOwner(this);
				cs.Leave();
			}
			else
			{
				innerList.push_back(_Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&_Val);
				if (resource) resource->RegisterOwner(this);
			}
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		void add(const _Ty& _Val)
		{
			if (!multiTask)
			{
				cs.Enter();
				innerList.push_back(_Val);
				_Ty* _refVal = const_cast<_Ty*>(&_Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);
				if (resource) resource->RegisterOwner(this);
				cs.Leave();
			}
			else
			{
				innerList.push_back(_Val);
				_Ty* _refVal = const_cast<_Ty*>(&_Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);
				if (resource) resource->RegisterOwner(this);
			}
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		void add(ObservableCollection& other)
		{
			if (other.size == 0)return;
			bool same = ((IObservableOwner<_Ty>)other).id == this->id;
			NotifyCollectionChangedEventArgs<_Ty> args = NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Add);
			unsigned int index = innerList.size() - 1;
			if (!multiTask)
			{
				cs.Enter();
				if (!same) other.cs.Enter();
				for (typename std::vector<_Ty>::iterator iter = other.innerList.begin(); iter != other.innerList.end(); ++iter)
				{
					_Ty item = *iter;
					innerList.push_back(item);
					_Ty* _refVal = const_cast<_Ty*>(&item);
					IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);
					if (resource) resource->RegisterOwner(this);
					args.AddNewItem(item, index);
				}
				if (!same) other.cs.Leave();
				cs.Leave();
			}
			CollectionChanged.Invoke(sharedthis, args);
		}

		iterator insert(iterator _Where, ObservableCollection& other)
		{
			if (other.size == 0)return _Where;
			bool same = ((IObservableOwner<_Ty>)other).id == this->id;
			NotifyCollectionChangedEventArgs<_Ty> args = NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Insert);
			ObservableCollection* _refthis = const_cast<ObservableCollection*>(this);
			unsigned int index = std::distance<const_iterator>(begin(), _Where);
			try
			{
				cs.Enter();
				if (same) other.cs.Enter();
				innerList.insert<const_iterator>(_Where, other.innerList.begin(), other.innerList.end());
				unsigned int eindex = index + other.size;
				for (typename std::vector<_Ty>::iterator iter = innerList.begin() + index; iter != innerList.begin() + eindex; ++iter)
				{
					_Ty item = *iter;
					_Ty* _refVal = const_cast<_Ty*>(&item);
					IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);
					if (resource) resource->RegisterOwner(_refthis);
					args.AddNewItem(item, index);
					index++;
				}
				if (same) other.cs.Leave();
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, args);

				return innerList.begin() + eindex;
			}
			catch (...)
			{
				if (same) other.cs.Leave();
				cs.Leave();
				return _Where;
			}
		}

		iterator insert(iterator _Where, _Ty&& _Val)
		{
			try
			{
				cs.Enter();
				iterator iter = innerList.insert(_Where, _Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&_Val);
				if (resource) resource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
				return iter;
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		iterator insert(iterator _Where, const _Ty& _Val)
		{
			try
			{
				cs.Enter();
				iterator iter = innerList.insert(_Where, _Val);
				_Ty* _refVal = const_cast<_Ty*>(&_Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);
				if (resource) resource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
				return iter;
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		void popback()
		{
			if (innerList.size() == 0)return;
			_Ty _Val{};
			cs.Enter();
			IObservableObject* resource = dynamic_cast<IObservableObject*>(&innerList[innerList.size() - 1]);
			if (resource) resource->UnRegisterOwner(this);
			_Val = innerList.back();
			innerList.pop_back();
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size(), NotifyCollectionChangedAction::Remove));
		}

		iterator remove(const_iterator _Where)
		{
			cs.Enter();
			int index = std::distance<const_iterator>(begin(), _Where);
			if (index >= 0 && index < innerList.size())
			{
				_Ty _Val = *_Where;
				_Ty* obj = &innerList[index];
				IObservableObject* resource = dynamic_cast<IObservableObject*>(obj);
				if (resource) resource->UnRegisterOwner(this);
				auto ridx = std::distance(innerList.begin(), _Where);
				iterator iter = innerList.erase(_Where);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(*obj, ridx, NotifyCollectionChangedAction::Remove));
				return iter;
			}
			else
			{
				cs.Leave();
				return index < 0 ? begin() : end();
			}
		}

		iterator remove(std::function<bool(_Ty)> _Pred)
		{
			cs.Enter();
			iterator iter = std::find_if<iterator, std::function<bool(_Ty)>>(innerList.begin(), innerList.end(), _Pred);
			if (iter != innerList.end())
			{
				_Ty* obj = &*iter;
				IObservableObject* resource = dynamic_cast<IObservableObject*>(obj);
				if (resource) resource->UnRegisterOwner(this);
				auto ridx = std::distance(innerList.begin(), iter);
				innerList.erase(iter);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(*obj, ridx, NotifyCollectionChangedAction::Remove));
				return iter;
			}
			cs.Leave();
			return iter;
		}

		bool replace(_Ty&& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			cs.Enter();
			for (; _First != _Last; ++_First)
			{
				if (_Pred(*_First))
				{
					_Ty tmp = *_First;

					int index = std::distance(innerList.begin(), _First);
					IObservableObject* oldResource = dynamic_cast<IObservableObject*>(&innerList[index]);
					if (oldResource) oldResource->UnRegisterOwner(this);

					*_First = _Val;

					IObservableObject* newResource = dynamic_cast<IObservableObject*>(&innerList[index]);
					if (newResource) newResource->RegisterOwner(this);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return true;
				}
			}
			cs.Leave();
			return false;
		}

		bool replace(const _Ty& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			cs.Enter();
			for (; _First != _Last; ++_First)
			{
				if (_Pred(*_First))
				{
					_Ty tmp = *_First;

					int index = std::distance(innerList.begin(), _First);
					IObservableObject* oldResource = dynamic_cast<IObservableObject*>(&innerList[index]);
					if (oldResource) oldResource->UnRegisterOwner(this);

					*_First = _Val;

					IObservableObject* newResource = dynamic_cast<IObservableObject*>(&innerList[index]);
					if (newResource) newResource->RegisterOwner(this);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return true;
				}
			}
			cs.Leave();
			return false;
		}

		bool replaceAll(const _Ty& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			NotifyCollectionChangedEventArgs<_Ty> args = NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace);
			bool find = false;
			cs.Enter();
			for (; _First != _Last; ++_First)
			{
				if (_Pred(*_First))
				{
					_Ty tmp = *_First;

					int index = std::distance(innerList.begin(), _First);
					IObservableObject* resource = dynamic_cast<IObservableObject*>(&innerList[index]);
					if (resource) resource->UnRegisterOwner(this);

					*_First = _Val;

					resource = dynamic_cast<IObservableObject*>(&innerList[index]);
					if (resource) resource->UnRegisterOwner(this);

					args.AddOldItem(index, tmp);
					args.AddNewItem(index, _Val);
					find = true;
				}
			}
			cs.Leave();
			if (find)
			{
				CollectionChanged.Invoke(sharedthis, args);
			}
			return false;
		}

		void clear()
		{
			cs.Enter();
			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
				if (resource) resource->UnRegisterOwner(this);
			}

			innerList.clear();
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		void reset(std::vector<_Ty> _Vector)
		{
			cs.Enter();
			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
				if (resource) resource->UnRegisterOwner(this);
			}

			innerList = std::vector<_Ty>(_Vector);

			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>(&(*iter));
				if (resource) resource->RegisterOwner(this);
			}
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		auto getAddress()->const _Ty*
		{
			return &innerList[0];
		}

		auto getSize()->const unsigned int
		{
			return innerList.size();
		}

		void set(const_iterator _Where, _Ty&& _Val)
		{
			cs.Enter();
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				int index = std::distance(innerList.begin(), _Where);
				_Ty tmp = innerList[index];

				IObservableObject* oldResource = dynamic_cast<IObservableObject*>(&tmp);

				if (oldResource) oldResource->UnRegisterOwner(this);

				innerList[index] = _Val;

				IObservableObject* newResource = dynamic_cast<IObservableObject*>(&innerList[index]);

				if (newResource) newResource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
				return;
			}
			cs.Leave();
		}

		void set(const_iterator _Where, const _Ty& _Val)
		{
			cs.Enter();
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				int index = std::distance<const_iterator>(begin(), _Where);
				_Ty tmp = innerList[index];


				IObservableObject* oldResource = dynamic_cast<IObservableObject*>(&tmp);

				if (oldResource) oldResource->UnRegisterOwner(this);

				innerList[index] = _Val;

				IObservableObject* newResource = dynamic_cast<IObservableObject*>(&innerList[index]);

				if (newResource) newResource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
				return;
			}
			cs.Leave();
		}

		const _Ty& get(const_iterator _Where) const
		{
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				int index = std::distance(innerList.begin(), _Where);
				return innerList[index];
			}
			return NULL;
		}

		const _Ty& get(unsigned int index) const
		{
			return innerList[index];
		}

		const _Ty& operator[](int index) const
		{
			return innerList[index];
		}

		_declspec(property(get = getSize)) unsigned int size;
		_declspec(property(get = getAddress)) const _Ty* address;


	};
#pragma endregion

#pragma region ObservableCollection_Ty_true_true 
	template<class _Ty>
	/// <summary>
	/// 可监视集合
	/// </summary>	
	class ObservableCollection<_Ty, true, true> : public IObservableOwner<_Ty, typename remove_shared_ptr<_Ty>::type::basetype>
	{
	public:
		typedef typename std::vector<_Ty>::value_type value_type;
		typedef typename std::vector<_Ty>::iterator iterator;
		typedef typename std::vector<_Ty>::const_iterator const_iterator;

	protected:
		CriticalSection cs;
		std::vector<_Ty> innerList;
		std::shared_ptr<ObservableCollection<_Ty, true, true>> sharedthis;

		void Notify(_Ty* sender) override
		{
			iterator iter = std::find_if<iterator>(begin(), end(), [sender](const _Ty& other) {return other.get() == sender->get(); });

			unsigned int index = std::distance(innerList.begin(), iter);
			if (index >= 0 && index < innerList.size())
			{
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(innerList[index], index));
			}
		}

	public:
		NotifyCollectionChangedEvent<_Ty> CollectionChanged;
		ObservableCollection()
		{
			sharedthis = std::make_shared<ObservableCollection<_Ty, true, true>>(*this);
		}

		~ObservableCollection()
		{
		}

		ObservableCollection(const ObservableCollection& other)
		{
			cs.Enter();
			innerList = std::vector<_Ty>(other.innerList);

			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->RegisterOwner(this);
			}
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection(ObservableCollection&& other)
		{
			cs.Enter();
			innerList = std::vector<_Ty>(other.innerList);

			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->RegisterOwner(this);
			}
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		ObservableCollection& operator=(ObservableCollection&& _Right)
		{
			cs.Enter();
			if (innerList != _Right.innerList)
			{
				innerList = _Right.innerList;
			}
			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->RegisterOwner(this);
			}
			cs.Leave();
			return (*this);
		}

		ObservableCollection& operator=(const ObservableCollection& _Right)
		{
			cs.Enter();
			if (innerList != _Right.innerList)
			{
				innerList = _Right.innerList;
			}
			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->RegisterOwner(this);
			}
			cs.Leave();
			return (*this);
		}

		iterator begin() noexcept
		{
			return innerList.begin();
		}

		const_iterator begin() const noexcept
		{
			return innerList.begin();
		}

		iterator end() noexcept
		{
			return innerList.end();
		}

		const_iterator end() const noexcept
		{
			return innerList.end();
		}

		void add(value_type&& _Val)
		{
			cs.Enter();
			innerList.push_back(_Val);
			IObservableObject* resource = dynamic_cast<IObservableObject*>(_Val.get());
			ObservableCollection* _refthis = const_cast<ObservableCollection*>(this);

			if (resource) resource->RegisterOwner(_refthis);
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		void add(const value_type& _Val)
		{
			cs.Enter();
			innerList.push_back(_Val);
			typename remove_shared_ptr<_Ty>::type* _refVal = const_cast<typename remove_shared_ptr<_Ty>::type*>(_Val.get());
			IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);

			ObservableCollection* _refthis = const_cast<ObservableCollection*>(this);

			if (resource) resource->RegisterOwner(_refthis);
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size() - 1, NotifyCollectionChangedAction::Add));
		}

		void add(ObservableCollection& other)
		{
			if (other.size == 0)return;
			bool same = ((IObservableOwner<_Ty>)other).id == this->id;
			NotifyCollectionChangedEventArgs<_Ty> args = NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Insert);
			ObservableCollection* _refthis = const_cast<ObservableCollection*>(this);
			cs.Enter();
			if (same) other.cs.Enter();
			unsigned int index = innerList.size() - 1;

			for (typename std::vector<_Ty>::iterator iter = other.innerList.begin(); iter != other.innerList.end(); ++iter)
			{
				_Ty item = *iter;
				innerList.push_back(item);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(item.get());
				if (resource) resource->RegisterOwner(_refthis);
				args.AddNewItem(item, index);
			}

			if (same) other.cs.Leave();
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, args);
		}

		iterator insert(iterator _Where, ObservableCollection& other)
		{
			if (other.size == 0)return _Where;
			bool same = ((IObservableOwner<_Ty>)other).id == this->id;
			NotifyCollectionChangedEventArgs<_Ty> args = NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Insert);
			ObservableCollection* _refthis = const_cast<ObservableCollection*>(this);

			cs.Enter();
			if (same) other.cs.Enter();

			unsigned int index = std::distance<const_iterator>(begin(), _Where);
			innerList.insert<const_iterator>(_Where, other.innerList.begin(), other.innerList.end());
			unsigned int eindex = index + other.size;
			for (typename std::vector<_Ty>::iterator iter = innerList.begin() + index; iter != innerList.begin() + eindex; ++iter)
			{
				_Ty item = *iter;
				IObservableObject* resource = dynamic_cast<IObservableObject*>(item.get());
				if (resource) resource->RegisterOwner(_refthis);
				args.AddNewItem(item, index);
				index++;
			}
			if (same) other.cs.Leave();
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, args);
			return innerList.begin() + eindex;
		}

		iterator insert(iterator _Where, _Ty&& _Val)
		{
			try
			{
				cs.Enter();
				iterator iter = innerList.insert(_Where, _Val);
				IObservableObject* resource = dynamic_cast<IObservableObject*>(_Val.get());
				if (resource) resource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
				return iter;
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		iterator insert(iterator _Where, const _Ty& _Val)
		{
			try
			{
				cs.Enter();
				iterator iter = innerList.insert(_Where, _Val);
				_Ty* _refVal = const_cast<_Ty*>(_Val.get());
				IObservableObject* resource = dynamic_cast<IObservableObject*>(_refVal);
				if (resource) resource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, std::distance(innerList.begin(), iter), NotifyCollectionChangedAction::Insert));
				return iter;
			}
			catch (...)
			{
				cs.Leave();
				return _Where;
			}
		}

		void popback()
		{
			if (innerList.size() == 0)return;
			_Ty _Val{};
			cs.Enter();
			IObservableObject* resource = dynamic_cast<IObservableObject*>(innerList[innerList.size() - 1].get());
			if (resource) resource->RegisterOwner(this);
			_Val = innerList.back();
			innerList.pop_back();
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(_Val, innerList.size(), NotifyCollectionChangedAction::Remove));
		}

		iterator remove(const_iterator _Where)
		{
			cs.Enter();
			unsigned  int index = std::distance<const_iterator>(begin(), _Where);
			if (index >= 0 && index < innerList.size())
			{
				_Ty obj = innerList[index];
				IObservableObject* resource = dynamic_cast<IObservableObject*>(obj.get());
				if (resource) resource->UnRegisterOwner(this);
				auto ridx = std::distance(innerList.begin(), _Where);
				iterator iter = innerList.erase(_Where);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(obj, ridx, NotifyCollectionChangedAction::Remove));
				return iter;
			}
			else
			{
				cs.Leave();
				return index < 0 ? begin() : end();
			}

		}

		iterator remove(std::function<bool(_Ty)> _Pred)
		{
			cs.Enter();
			iterator iter = std::find_if<iterator, std::function<bool(_Ty)>>(innerList.begin(), innerList.end(), _Pred);
			unsigned int index = std::distance<const_iterator>(begin(), iter);
			if (index >= 0 && index < innerList.size())
			{
				_Ty obj = innerList[index];
				IObservableObject* resource = dynamic_cast<IObservableObject*>(obj.get());
				if (resource) resource->UnRegisterOwner(this);
				auto ridx = std::distance(innerList.begin(), iter);
				iter = innerList.erase(iter);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(obj, ridx, NotifyCollectionChangedAction::Remove));
				return iter;
			}
			cs.Leave();
			return iter;
		}

		bool replace(value_type&& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			cs.Enter();
			for (; _First != _Last; ++_First)
			{
				if (_Pred(*_First))
				{
					_Ty tmp = *_First;

					int index = std::distance(innerList.begin(), _First);
					IObservableObject* resource = dynamic_cast<IObservableObject*>(innerList[index].get());
					if (resource) resource->UnRegisterOwner(this);

					*_First = _Val;

					resource = dynamic_cast<IObservableObject*>(innerList[index].get());
					if (resource) resource->UnRegisterOwner(this);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return true;
				}
			}
			cs.Leave();
			return false;
		}

		bool replace(const value_type& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			cs.Enter();
			for (; _First != _Last; ++_First)
			{
				if (_Pred(*_First))
				{
					_Ty tmp = *_First;

					int index = std::distance(innerList.begin(), _First);
					IObservableObject* resource = dynamic_cast<IObservableObject*>(innerList[index].get());
					if (resource) resource->UnRegisterOwner(this);

					*_First = _Val;

					resource = dynamic_cast<IObservableObject*>(innerList[index].get());
					if (resource) resource->UnRegisterOwner(this);
					cs.Leave();
					CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
					return true;
				}
			}
			cs.Leave();
			return false;
		}

		bool replaceAll(const value_type& _Val, std::function<bool(_Ty)> _Pred)
		{
			iterator _First = innerList.begin();
			iterator _Last = innerList.end();
			NotifyCollectionChangedEventArgs<_Ty> args = NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace);
			bool find = false;
			cs.Enter();
			for (; _First != _Last; ++_First)
			{
				if (_Pred(*_First))
				{
					_Ty tmp = *_First;

					int index = std::distance(innerList.begin(), _First);
					IObservableObject* resource = dynamic_cast<IObservableObject*>(innerList[index].get());
					if (resource) resource->UnRegisterOwner(this);

					*_First = _Val;

					resource = dynamic_cast<IObservableObject*>(innerList[index].get());
					if (resource) resource->UnRegisterOwner(this);

					args.AddOldItem(index, tmp);
					args.AddNewItem(index, _Val);
					find = true;
				}
			}
			cs.Leave();
			if (find)
			{
				CollectionChanged.Invoke(sharedthis, args);
			}
			return false;
		}

		void clear()
		{
			cs.Enter();
			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->UnRegisterOwner(this);
			}

			innerList.clear();
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		void reset(std::vector<_Ty> _Vector)
		{
			cs.Enter();
			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->UnRegisterOwner(this);
			}

			innerList = std::vector<_Ty>(_Vector);

			for (typename std::vector<_Ty>::iterator iter = innerList.begin(); iter != innerList.end(); ++iter)
			{
				IObservableObject* resource = dynamic_cast<IObservableObject*>((*iter).get());
				if (resource) resource->RegisterOwner(this);
			}
			cs.Leave();
			CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>());
		}

		auto getAddress()->const _Ty*
		{
			return &innerList[0];
		}

		auto getSize()->const unsigned int
		{
			return innerList.size();
		}

		void set(const_iterator _Where, _Ty&& _Val)
		{
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				cs.Enter();
				int index = std::distance(innerList.begin(), _Where);
				_Ty tmp = innerList[index];

				IObservableObject* resource = dynamic_cast<IObservableObject*>(tmp.get());

				if (resource) resource->UnRegisterOwner(this);

				resource = NULL;

				innerList[index] = _Val;

				resource = dynamic_cast<IObservableObject*>(innerList[index].get());

				if (resource) resource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
			}
		}

		void set(const_iterator _Where, const _Ty& _Val)
		{
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				cs.Enter();
				int index = std::distance<const_iterator>(begin(), _Where);
				_Ty tmp = innerList[index];

				IObservableObject* resource = dynamic_cast<IObservableObject*>(tmp.get());

				if (resource) resource->UnRegisterOwner(this);

				resource = NULL;

				innerList[index] = _Val;

				resource = dynamic_cast<IObservableObject*>(innerList[index].get());

				if (resource) resource->RegisterOwner(this);
				cs.Leave();
				CollectionChanged.Invoke(sharedthis, NotifyCollectionChangedEventArgs<_Ty>(NotifyCollectionChangedAction::Replace, tmp, index, _Val, index));
			}
		}

		const _Ty& get(const_iterator _Where) const
		{
			if (_Where >= innerList.begin() && _Where < innerList.end())
			{
				int index = std::distance(innerList.begin(), _Where);
				return innerList[index];
			}
			return NULL;
		}

		const _Ty& get(unsigned int index) const
		{
			return innerList[index];
		}

		const _Ty& operator[](int index) const
		{
			return innerList[index];
		}

		_declspec(property(get = getSize)) unsigned int size;
		_declspec(property(get = getAddress)) const _Ty* address;


	};

#pragma endregion
}