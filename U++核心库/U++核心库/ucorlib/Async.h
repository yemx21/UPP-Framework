#pragma once
#include "TimeSpan.h"
#include <Future>

#define _Init_atomic_counter(_Counter, _Value)	\
	_Counter = _Value

namespace System
{
	namespace Threading
	{
		enum class AsyncOperationStatus
		{
			Pending,
			Aborted,
			Completed,
			Executing
		};

		class AsyncOperation;

		template<class _Ty>
		class Associated_state;

		template<class _Ty>
		class State_manager;

		class RUNTIME_API IAsyncState
		{
		public:
			virtual void Wait_impl();
			virtual void WaitFor_impl(const TimeSpan& timeout);
			virtual AsyncOperationStatus GetStatus_impl() const;
		};

		class RUNTIME_API AsyncOperation
		{
		public:
			virtual void Wait();
			virtual void WaitFor(const TimeSpan& timeout);
			virtual AsyncOperationStatus GetStatus() const;
			virtual Any GetResult();
		};

		enum class FutureStatus
		{
			Ready,
			Timeout,
			Deferred
		};


		template<class _Ty>
		struct AssociatedDeleter_base
		{
			virtual void _Delete(Associated_state<_Ty> *) = 0;
			virtual ~AssociatedDeleter_base() noexcept
			{
			}
		};

		template<class _Ty,
			class _Derived,
			class _Alloc>
			struct AssociatedDeleter
			: AssociatedDeleter_base<_Ty>
		{
			AssociatedDeleter(const _Alloc& _Al)
				: _My_alloc(_Al)
			{
			}

			AssociatedDeleter(const AssociatedDeleter&) = delete;
			AssociatedDeleter& operator=(const AssociatedDeleter&) = delete;

			inline void _Delete(Associated_state<_Ty> *_State);

			_Alloc _My_alloc;
		};

		template<class _Ty>
		class Associated_state: public IAsyncState
		{
		public:
			typedef _Ty _State_type;
			typedef AssociatedDeleter_base<_Ty> _Mydel;

			Associated_state(_Mydel *_Dp = 0)
				: _exception(),
				_Retrieved(false),
				_Running(false),
				_Ready(false),
				_Ready_at_thread_exit(false),
				_Has_stored_result(false),
				_Deleter(_Dp)
			{	
				_Init_refs();
			}

			virtual ~Associated_state() noexcept
			{	
				if (_Has_stored_result && !_Ready)
				{	
					_Cond._Unregister(_Mtx);
				}
				_Destroy_refs();
			}

			void _Retain()
			{	
				_MT_INCR(_Refs);
			}

			void _Release()
			{	
				if (_MT_DECR(_Refs) == 0)
					_Delete_this();
			}

		private:
			std::_Atomic_counter_t _Refs;

		public:
			void _Init_refs()
			{	
				_Init_atomic_counter(_Refs, 1);
			}

			void _Destroy_refs()
			{	
			}

			virtual void _Wait()
			{	
				std::unique_lock<std::mutex> _Lock(_Mtx);
				_Maybe_run_deferred_function(_Lock);
				while (!_Ready)
					_Cond.wait(_Lock);
			}

			struct _Test_ready
			{	
				_Test_ready(const Associated_state *_St)
					: _State(_St)
				{
				}

				bool operator()() const
				{
					return (_State->_Ready != 0);
				}
				const Associated_state *_State;
			};

			template<class _Rep,
				class _Per>
				std::future_status _Wait_for(
					const std::chrono::duration<_Rep, _Per>& _Rel_time)
			{
				std::unique_lock<std::mutex> _Lock(_Mtx);
				if (_Has_deferred_function())
					return (std::future_status::deferred);
				if (_Cond.wait_for(_Lock, _Rel_time, _Test_ready(this)))
					return (std::future_status::ready);
				return (std::future_status::timeout);
			}

			template<class _Clock,
				class _Dur>
				std::future_status _Wait_until(
					const std::chrono::time_point<_Clock, _Dur>& _Abs_time)
			{	// wait until time point
				std::unique_lock<std::mutex> _Lock(_Mtx);
				if (_Has_deferred_function())
					return (std::future_status::deferred);
				if (_Cond.wait_until(_Lock, _Abs_time, _Test_ready(this)))
					return (std::future_status::ready);
				return (std::future_status::timeout);
			}

			virtual _Ty& _Get_value(bool _Get_only_once)
			{	// return the stored result or throw stored std::exception
				std::unique_lock<std::mutex> _Lock(_Mtx);
				if (_Get_only_once && _Retrieved)
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::future_already_retrieved));
				if (_exception)
					_Rethrow_future_exception(_exception);
				_Retrieved = true;
				_Maybe_run_deferred_function(_Lock);
				while (!_Ready)
					_Cond.wait(_Lock);
				if (_exception)
					_Rethrow_future_exception(_exception);
				return (_Result);
			}

			void _Set_value(const _Ty& _Val, bool _At_thread_exit)
			{	// store a result
				std::unique_lock<std::mutex> _Lock(_Mtx);
				_Set_value_raw(_Val, &_Lock, _At_thread_exit);
			}

			void _Set_value_raw(const _Ty& _Val, std::unique_lock<std::mutex> *_Lock,
				bool _At_thread_exit)
			{	// store a result while inside a locked block
				if (_Has_stored_result)
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_Result = _Val;
				_Do_notify(_Lock, _At_thread_exit);
			}

			void _Set_value(_Ty&& _Val, bool _At_thread_exit)
			{	// store a result
				std::unique_lock<std::mutex> _Lock(_Mtx);
				_Set_value_raw(std::forward<_Ty>(_Val), &_Lock, _At_thread_exit);
			}

			void _Set_value_raw(_Ty&& _Val, std::unique_lock<std::mutex> *_Lock,
				bool _At_thread_exit)
			{	// store a result while inside a locked block
				if (_Has_stored_result)
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_Result = std::forward<_Ty>(_Val);
				_Do_notify(_Lock, _At_thread_exit);
			}

			void _Set_value(bool _At_thread_exit)
			{	// store a (void) result
				std::unique_lock<std::mutex> _Lock(_Mtx);
				_Set_value_raw(&_Lock, _At_thread_exit);
			}

			void _Set_value_raw(std::unique_lock<std::mutex> *_Lock, bool _At_thread_exit)
			{	// store a (void) result while inside a locked block
				if (_Has_stored_result)
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_Do_notify(_Lock, _At_thread_exit);
			}

			void _set_exception(std::exception_ptr _Exc, bool _At_thread_exit)
			{	// store a result
				std::unique_lock<std::mutex> _Lock(_Mtx);
				_set_exception_raw(_Exc, &_Lock, _At_thread_exit);
			}

			struct _Anon
			{	// anonymous type
			};

			void _set_exception_raw(std::exception_ptr _Exc,
				std::unique_lock<std::mutex> *_Lock, bool _At_thread_exit)
			{	// store a result while inside a locked block
				if (_Has_stored_result)
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_exception = _Exc;
				if (!_Exc)	// make a non-null std::exception_ptr

					_exception = std::make_exception_ptr(_Anon());

				_Do_notify(_Lock, _At_thread_exit);
			}

			bool _Is_ready() const
			{	// return ready status
				return (_Ready != 0);
			}

			bool _Already_has_stored_result() const
			{	// return presence of stored result
				return (_Has_stored_result);
			}

			bool _Already_retrieved() const
			{	// return retrieved status
				return (_Retrieved);
			}

			void _Abandon()
			{	// abandon shared state
				std::unique_lock<std::mutex> _Lock(_Mtx);
				if (!_Has_stored_result)
				{	// queue std::exception
					std::future_error _Fut(std::make_error_code(std::future_errc::broken_Promise));
					_set_exception_raw(std::make_exception_ptr(_Fut), &_Lock, false);
				}
			}

		protected:
			void _Make_ready_at_thread_exit()
			{	// set ready status at thread exit
				if (_Ready_at_thread_exit)
					_Ready = true;
			}

			void _Maybe_run_deferred_function(std::unique_lock<std::mutex>& _Lock)
			{	// run a deferred function if not already done
				if (!_Running)
				{	// run the function
					_Running = true;
					_Run_deferred_function(_Lock);
				}
			}

		public:
			_Ty _Result;
			std::exception_ptr _exception;
			std::mutex _Mtx;
			std::condition_variable _Cond;
			bool _Retrieved;
			int _Ready;
			bool _Ready_at_thread_exit;
			bool _Has_stored_result;
			bool _Running;

		private:
			virtual bool _Has_deferred_function() const noexcept
			{
				return (false);
			}

			virtual void _Run_deferred_function(std::unique_lock<std::mutex>&)
			{	// do nothing
			}

			virtual void _Do_notify(std::unique_lock<std::mutex> *_Lock, bool _At_thread_exit)
			{	// notify waiting threads
				_Has_stored_result = true;
				if (_At_thread_exit)
				{	// notify at thread exit
					_Cond._Register(*_Lock, &_Ready);
				}
				else
				{	// notify immediately
					_Ready = true;
					_Cond.notify_all();
				}
			}

			void _Delete_this()
			{	// delete this object
				if (_Deleter)
					_Deleter->_Delete(this);
				else
					delete this;
			}

			_Mydel *_Deleter;

		public:
			Associated_state(const Associated_state&) = delete;
			Associated_state& operator=(const Associated_state&) = delete;

		public:
			void Wait_impl() override
			{
				_Wait();
			}

			void WaitFor_impl(const TimeSpan& timeout) override
			{
				_Wait_for(std::chrono::milliseconds((long long)timeout.TotalMilliseconds));
			}

			AsyncOperationStatus GetStatus_impl() const override
			{
				if (_Running)
				{
					return AsyncOperationStatus::Executing;
				}
				else if (_Has_stored_result)
				{
					return AsyncOperationStatus::Completed;
				}
				else if (_Ready!=0)
				{
					return AsyncOperationStatus::Pending;
				}
				return AsyncOperationStatus::Aborted;
			}
		};

		template<class _Ty,
			class _Derived,
			class _Alloc> inline
			void AssociatedDeleter<_Ty, _Derived, _Alloc>::_Delete(
				Associated_state<_Ty> *_State)
		{
			typedef std::_Wrap_alloc<_Alloc> _Alty0;

			typedef typename _Alty0::template rebind<_Derived>
				::other _State_allocator;
			_State_allocator _St_alloc(_My_alloc);

			typedef typename _Alty0::template rebind<AssociatedDeleter>
				::other _Deleter_allocator;
			_Deleter_allocator _Del_alloc(_My_alloc);

			_Derived *_Ptr = static_cast<_Derived *>(_State);

			_St_alloc.destroy(_Ptr);
			_St_alloc.deallocate(_Ptr, 1);

			_Del_alloc.destroy(this);
			_Del_alloc.deallocate(this, 1);
		}


		template<class>
		class Packaged_state;

		template<class _Ret,
			class... _ArgTypes>
			class Packaged_state<_Ret(_ArgTypes...)>
			: public Associated_state<_Ret>
		{
		public:
			typedef Associated_state<_Ret> _Mybase;
			typedef typename _Mybase::_Mydel _Mydel;

			template<class _Fty2>
			Packaged_state(const _Fty2& _Fnarg)
				: _Fn(_Fnarg)
			{	// construct from function object
			}

			template<class _Fty2,
				class _Alloc>
				Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
				: _Mybase(_Dp), _Fn(std::allocator_arg, _Al, _Fnarg)
			{	// construct from function object and allocator
			}

			template<class _Fty2>
			Packaged_state(_Fty2&& _Fnarg)
				: _Fn(std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object
			}

			template<class _Fty2,
				class _Alloc>
				Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
				: _Mybase(_Dp), _Fn(std::allocator_arg, _Al,
					std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object and allocator
			}

			void _Call_deferred(_ArgTypes... _Args)
			{	// set deferred call
				_TRY_BEGIN
					// call function object and catch std::exceptions
					this->_Set_value(_Fn(std::forward<_ArgTypes>(_Args)...),
						true);
				_CATCH_ALL
					// function object threw std::exception; record result
					this->_set_exception(std::current_exception(), true);
				_CATCH_END
			}

			void _Call_immediate(_ArgTypes... _Args)
			{	// call function object
				_TRY_BEGIN
					// call function object and catch std::exceptions
					this->_Set_value(_Fn(std::forward<_ArgTypes>(_Args)...),
						false);
				_CATCH_ALL
					// function object threw std::exception; record result
					this->_set_exception(std::current_exception(), false);
				_CATCH_END
			}

			const std::function<_Ret(_ArgTypes...)>& _Get_fn()
			{	// return stored function object
				return (_Fn);
			}

		private:
			std::function<_Ret(_ArgTypes...)> _Fn;
		};

		template<class _Ret,
			class... _ArgTypes>
			class Packaged_state<_Ret&(_ArgTypes...)>
			: public Associated_state<_Ret *>
		{
		public:
			typedef Associated_state<_Ret *> _Mybase;
			typedef typename _Mybase::_Mydel _Mydel;

			template<class _Fty2>
			Packaged_state(const _Fty2& _Fnarg)
				: _Fn(_Fnarg)
			{	// construct from function object
			}

			template<class _Fty2,
				class _Alloc>
				Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
				: _Mybase(_Dp), _Fn(std::allocator_arg, _Al, _Fnarg)
			{	// construct from function object and allocator
			}

			template<class _Fty2>
			Packaged_state(_Fty2&& _Fnarg)
				: _Fn(std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object
			}

			template<class _Fty2,
				class _Alloc>
				Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
				: _Mybase(_Dp), _Fn(std::allocator_arg, _Al,
					std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object and allocator
			}

			void _Call_deferred(_ArgTypes... _Args)
			{	// set deferred call
				_TRY_BEGIN
					// call function object and catch std::exceptions
					this->_Set_value(
						std::addressof(_Fn(std::forward<_ArgTypes>(_Args)...)),
						true);
				_CATCH_ALL
					// function object threw std::exception; record result
					this->_set_exception(std::current_exception(), true);
				_CATCH_END
			}

			void _Call_immediate(_ArgTypes... _Args)
			{	// call function object
				_TRY_BEGIN
					// call function object and catch std::exceptions
					this->_Set_value(
						std::addressof(_Fn(std::forward<_ArgTypes>(_Args)...)),
						false);
				_CATCH_ALL
					// function object threw std::exception; record result
					this->_set_exception(std::current_exception(), false);
				_CATCH_END
			}

			const std::function<_Ret&(_ArgTypes...)>& _Get_fn()
			{	// return stored function object
				return (_Fn);
			}

		private:
			std::function<_Ret&(_ArgTypes...)> _Fn;
		};

		template<class... _ArgTypes>
		class Packaged_state<void(_ArgTypes...)>
			: public Associated_state<int>
		{
		public:
			typedef Associated_state<int> _Mybase;
			typedef typename _Mybase::_Mydel _Mydel;

			template<class _Fty2>
			Packaged_state(const _Fty2& _Fnarg)
				: _Fn(_Fnarg)
			{	// construct from function object
			}

			template<class _Fty2,
				class _Alloc>
				Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
				: _Mybase(_Dp), _Fn(std::allocator_arg, _Al, _Fnarg)
			{	// construct from function object and allocator
			}

			template<class _Fty2>
			Packaged_state(_Fty2&& _Fnarg)
				: _Fn(std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object
			}

			template<class _Fty2,
				class _Alloc>
				Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
				: _Mybase(_Dp), _Fn(std::allocator_arg, _Al,
					std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object and allocator
			}

			void _Call_deferred(_ArgTypes... _Args)
			{	// set deferred call
				_TRY_BEGIN
					// call function object and catch std::exceptions
					_Fn(std::forward<_ArgTypes>(_Args)...);
				this->_Set_value(1, true);
				_CATCH_ALL
					// function object threw std::exception; record result
					this->_set_exception(std::current_exception(), true);
				_CATCH_END
			}

			void _Call_immediate(_ArgTypes... _Args)
			{	// call function object
				_TRY_BEGIN
					// call function object and catch std::exceptions
					_Fn(std::forward<_ArgTypes>(_Args)...);
				this->_Set_value(1, false);
				_CATCH_ALL
					// function object threw std::exception; record result
					this->_set_exception(std::current_exception(), false);
				_CATCH_END
			}

			const std::function<void(_ArgTypes...)>& _Get_fn()
			{	// return stored function object
				return (_Fn);
			}

		private:
			std::function<void(_ArgTypes...)> _Fn;
		};


		template<class _Ty,
			class _Alloc> inline
			Associated_state<_Ty> *_Make_associated_state(const _Alloc& _Al)
		{	// construct an Associated_state object with an allocator
			typedef AssociatedDeleter<_Ty, Associated_state<_Ty>, _Alloc> _Delty;
			typedef std::_Wrap_alloc<_Alloc> _Alty0;

			typedef typename _Alty0::template rebind<_Delty>
				::other _Deleter_allocator;
			typedef typename _Alty0::template rebind<Associated_state<_Ty> >
				::other _State_allocator;

			_Deleter_allocator _Del_alloc(_Al);
			_State_allocator _St_alloc(_Al);

			_Delty *_Del = 0;
			Associated_state<_Ty> *_Res = 0;

			_Del = _Del_alloc.allocate(1);

			bool _Del_constructed = false;

			_TRY_BEGIN
				_Del_alloc.construct(_Del, _Al);
			_Del_constructed = true;

			_Res = _St_alloc.allocate(1);

			_St_alloc.construct(_Res, _Del);
			_CATCH_ALL
				if (_Res)
					_St_alloc.deallocate(_Res, 1);

			if (_Del_constructed)
				_Del_alloc.destroy(_Del);

			_Del_alloc.deallocate(_Del, 1);
			_RERAISE;
			_CATCH_END

				return (_Res);
		}

		template<class _Pack_state,
			class _Fty2,
			class _Alloc> inline
			_Pack_state *_Make_packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al)
		{	
			typedef AssociatedDeleter<typename _Pack_state::_Mybase::_State_type,
				_Pack_state, _Alloc> _Delty;
			typedef std::_Wrap_alloc<_Alloc> _Alty0;

			typedef typename _Alty0::template rebind<_Delty>
				::other _Deleter_allocator;
			typedef typename _Alty0::template rebind<_Pack_state>
				::other _State_allocator;

			_Deleter_allocator _Del_alloc(_Al);
			_State_allocator _St_alloc(_Al);

			_Delty *_Del = 0;
			_Pack_state *_Res = 0;

			_Del = _Del_alloc.allocate(1);

			bool _Del_constructed = false;

			_TRY_BEGIN
				_Del_alloc.construct(_Del, _Al);
			_Del_constructed = true;

			_Res = _St_alloc.allocate(1);

			_St_alloc.construct(_Res, std::forward<_Fty2>(_Fnarg), _Al, _Del);
			_CATCH_ALL
				if (_Res)
					_St_alloc.deallocate(_Res, 1);

			if (_Del_constructed)
				_Del_alloc.destroy(_Del);

			_Del_alloc.deallocate(_Del, 1);
			_RERAISE;
			_CATCH_END

				return (_Res);
		}

		template<class _Rx>
		class Deferred_async_state
			: public Packaged_state<_Rx()>
		{	
		public:
			template<class _Fty2>
			Deferred_async_state(const _Fty2& _Fnarg)
				: Packaged_state<_Rx()>(_Fnarg)
			{	// construct from function object
			}

			template<class _Fty2>
			Deferred_async_state(_Fty2&& _Fnarg)
				: Packaged_state<_Rx()>(std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object
			}

		private:
			virtual bool _Has_deferred_function() const noexcept
			{	// this function is considered to be deferred until it's invoked
				return (!this->_Running);
			}

			virtual void _Run_deferred_function(std::unique_lock<std::mutex>& _Lock)
			{	// run the deferred function
				_Lock.unlock();
				Packaged_state<_Rx()>::_Call_immediate();
				_Lock.lock();
			}
		};

		template<class _Rx, bool _Inline>
		class Task_async_state
			: public Packaged_state<_Rx()>
		{	
		public:
			typedef Packaged_state<_Rx()> _Mybase;
			typedef typename _Mybase::_State_type _State_type;

			template<class _Fty2>
			Task_async_state(_Fty2&& _Fnarg)
				: _Mybase(std::forward<_Fty2>(_Fnarg))
			{	// construct from rvalue function object
				_Task = ::Concurrency::create_task([this]()
				{	// do it now
					this->_Call_immediate();
				});

				this->_Running = true;
			}

			virtual ~Task_async_state() noexcept
			{	// destroy
				if (!this->_Ready)
					_Wait();
			}

			virtual void _Wait()
			{	// wait for completion
				if (_Inline)
					_Task.wait();
				else
					_Mybase::_Wait();
			}

			virtual _State_type& _Get_value(bool _Get_only_once)
			{	// return the stored result or throw stored std::exception
				if (_Inline)
					_Task.wait();
				return (_Mybase::_Get_value(_Get_only_once));
			}

		private:
			::Concurrency::task<void> _Task;
		};

		template<class _Ty>
		class State_manager: public IAsyncState
		{
		public:
			State_manager()
				: _Assoc_state(0)
			{
				_Get_only_once = false;
			}

			State_manager(Associated_state<_Ty> *_New_state, bool _Get_once)
				: _Assoc_state(_New_state)
			{
				_Get_only_once = _Get_once;
			}

			State_manager(const State_manager& _Other, bool _Get_once = false)
				: _Assoc_state(0)
			{
				_Copy_from(_Other);
				_Get_only_once = _Get_once;
			}

			State_manager(State_manager&& _Other, bool _Get_once = false)
				: _Assoc_state(0)
			{
				_Move_from(_Other);
				_Get_only_once = _Get_once;
			}

			~State_manager() noexcept
			{	// destroy
				if (_Assoc_state != 0)
					_Assoc_state->_Release();
			}

			State_manager& operator=(const State_manager& _Other)
			{	// assign from _Other
				_Copy_from(_Other);
				return (*this);
			}

			State_manager& operator=(State_manager&& _Other)
			{	// assign from rvalue _Other
				_Move_from(_Other);
				return (*this);
			}

			bool valid() const noexcept
			{	// return status
				return (_Assoc_state != 0
					&& !(_Get_only_once && _Assoc_state->_Already_retrieved()));
			}

			void wait() const
			{	// wait for signal
				if (!valid())
					std::_Throw_future_error(std::make_error_code(std::future_errc::no_state));
				_Assoc_state->_Wait();
			}

			template<class _Rep,
				class _Per>
				std::future_status wait_for(
					const std::chrono::duration<_Rep, _Per>& _Rel_time) const
			{	// wait for duration
				if (!valid())
					std::_Throw_future_error(std::make_error_code(std::future_errc::no_state));
				return (_Assoc_state->_Wait_for(_Rel_time));
			}

			template<class _Clock,
				class _Dur>
				std::future_status wait_until(
					const std::chrono::time_point<_Clock, _Dur>& _Abs_time) const
			{	// wait until time point
				if (!valid())
					std::_Throw_future_error(std::make_error_code(std::future_errc::no_state));
				return (_Assoc_state->_Wait_until(_Abs_time));
			}

			_Ty& _Get_value() const
			{	// return the stored result or throw stored std::exception
				if (!valid())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::no_state));
				return (_Assoc_state->_Get_value(_Get_only_once));
			}

			void _Set_value(const _Ty& _Val, bool _Defer)
			{	// store a result
				if (!valid())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::no_state));
				_Assoc_state->_Set_value(_Val, _Defer);
			}

			void _Set_value(_Ty&& _Val, bool _Defer)
			{	// store a result
				if (!valid())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::no_state));
				_Assoc_state->_Set_value(std::forward<_Ty>(_Val), _Defer);
			}

			void _Abandon()
			{	// abandon shared state
				if (_Assoc_state)
					_Assoc_state->_Abandon();
			}

			void _set_exception(std::exception_ptr _Exc, bool _Defer)
			{	// store a result
				if (!valid())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::no_state));
				_Assoc_state->_set_exception(_Exc, _Defer);
			}

			void _Swap(State_manager& _Other)
			{	// exchange with _Other
				std::swap(_Assoc_state, _Other._Assoc_state);
			}

			Associated_state<_Ty> *_Ptr() const
			{
				return (_Assoc_state);
			}

			void _Copy_from(const State_manager& _Other)
			{
				if (this != &_Other)
				{	// different, copy
					if (_Assoc_state)
						_Assoc_state->_Release();
					if (_Other._Assoc_state == 0)
						_Assoc_state = 0;
					else
					{	// do the copy
						_Other._Assoc_state->_Retain();
						_Assoc_state = _Other._Assoc_state;
						_Get_only_once = _Other._Get_only_once;
					}
				}
			}

			void _Move_from(State_manager& _Other)
			{
				if (this != &_Other)
				{	// different, move
					if (_Assoc_state)
						_Assoc_state->_Release();
					_Assoc_state = _Other._Assoc_state;
					_Other._Assoc_state = 0;
					_Get_only_once = _Other._Get_only_once;
				}
			}

			bool _Is_ready() const
			{	// return status
				return (_Assoc_state && _Assoc_state->_Is_ready());
			}

		private:
			Associated_state<_Ty> *_Assoc_state;
			bool _Get_only_once;
		public:
			void Wait_impl() override
			{
				wait();
			}

			void WaitFor_impl(const TimeSpan& timeout) override
			{
				wait_for(std::chrono::milliseconds((long long)timeout.TotalMilliseconds));
			}

			AsyncOperationStatus GetStatus_impl() const override
			{
				return _Assoc_state->GetStatus_impl();
			}
		};

		template<class _Ty>
		class SharedFuture;

		template<class _Ty>
		class Future
			: public State_manager<_Ty>, public AsyncOperation
		{
			typedef State_manager<_Ty> _Mybase;
		public:
			Future() noexcept
			{	// construct
			}

			Future(Future&& _Other) noexcept
				: _Mybase(std::forward<Future>(_Other), true)
			{	// construct from rvalue Future object
			}

			Future& operator=(Future&& _Right) noexcept
			{	// assign from rvalue Future object
				_Mybase::operator=(std::forward<Future>(_Right));
				return (*this);
			}

			Future(const _Mybase& _State, std::_Nil)
				: _Mybase(_State, true)
			{
			}

			~Future() noexcept
			{	// destroy
			}

			_Ty get()
			{	// block until ready then return the stored result or
				// throw the stored std::exception
				return (std::move(this->_Get_value()));
			}

			SharedFuture<_Ty> share()
			{	// return state as SharedFuture
				return (SharedFuture<_Ty>(std::move(*this)));
			}

			Future(const Future&) = delete;
			Future& operator=(const Future&) = delete;

		public:
			void Wait() override
			{
				this->Wait_impl();
			}

			void WaitFor(const TimeSpan& timeout) override
			{
				this->WaitFor_impl(timeout);
			}

			AsyncOperationStatus GetStatus() const override
			{
				return this->GetStatus_impl();
			}

			Any GetResult() override
			{
				return Any<_Ty>(get());
			}
		};

		template<class _Ty>
		class Future<_Ty&>
			: public State_manager<_Ty *>, public AsyncOperation
		{
			typedef State_manager<_Ty *> _Mybase;
		public:
			Future() noexcept
			{	// construct
			}

			Future(Future&& _Other) noexcept
				: _Mybase(std::forward<Future>(_Other), true)
			{	// construct from rvalue Future object
			}

			Future& operator=(Future&& _Right) noexcept
			{	// assign from rvalue Future object
				_Mybase::operator=(std::forward<Future>(_Right));
				return (*this);
			}

			Future(const _Mybase& _State, std::_Nil)
				: _Mybase(_State, true)
			{
			}

			~Future() noexcept
			{	// destroy
			}

			_Ty& get()
			{	// block until ready then return the stored result or
				// throw the stored std::exception
				return (*this->_Get_value());
			}

			SharedFuture<_Ty&> share()
			{	// return state as SharedFuture
				return (SharedFuture<_Ty&>(std::move(*this)));
			}

			Future(const Future&) = delete;
			Future& operator=(const Future&) = delete;

		public:
			void Wait() override
			{
				this->Wait_impl();
			}

			void WaitFor(const TimeSpan& timeout) override
			{
				this->WaitFor_impl(timeout);
			}

			AsyncOperationStatus GetStatus() const override
			{
				return this->GetStatus_impl();
			}

			Any GetResult() override
			{
				return Any<_Ty&>(get());
			}
		};

		template<>
		class Future<void>
			: public State_manager<int>, public AsyncOperation
		{
			typedef State_manager<int> _Mybase;
		public:
			Future() noexcept
			{	// construct
			}

			Future(Future&& _Other) noexcept
				: _Mybase(std::forward<Future>(_Other), true)
			{	// construct from rvalue Future object
			}

			Future& operator=(Future&& _Right) noexcept
			{	// assign from rvalue Future object
				_Mybase::operator=(std::forward<Future>(_Right));
				return (*this);
			}

			Future(const _Mybase& _State, std::_Nil)
				: _Mybase(_State, true)
			{
			}

			~Future() noexcept
			{	// destroy
			}

			void get()
			{	// block until ready then return or
				// throw the stored std::exception
				this->_Get_value();
			}

			SharedFuture<void> share();

			Future(const Future&) = delete;
			Future& operator=(const Future&) = delete;

		public:
			void Wait() override
			{
				this->Wait_impl();
			}

			void WaitFor(const TimeSpan& timeout) override
			{
				this->WaitFor_impl(timeout);
			}

			AsyncOperationStatus GetStatus() const override
			{
				return this->GetStatus_impl();
			}

		};

		template<class _Ty>
		class SharedFuture
			: public State_manager<_Ty>, public AsyncOperation
		{
			typedef State_manager<_Ty> _Mybase;

		public:
			SharedFuture() noexcept
			{	// construct
			}

			SharedFuture(const SharedFuture& _Other)
				: _Mybase(_Other)
			{	// construct from SharedFuture object
			}

			SharedFuture& operator=(const SharedFuture& _Right)
			{	// assign from SharedFuture object
				_Mybase::operator=(_Right);
				return (*this);
			}

			SharedFuture(Future<_Ty>&& _Other) noexcept
				: _Mybase(std::forward<_Mybase>(_Other))
			{	// construct from rvalue Future object
			}

			SharedFuture(SharedFuture&& _Other) noexcept
				: _Mybase(std::forward<SharedFuture>(_Other))
			{	// construct from rvalue SharedFuture object
			}

			SharedFuture& operator=(SharedFuture&& _Right) noexcept
			{	// assign from SharedFuture rvalue object
				_Mybase::operator=(std::forward<SharedFuture>(_Right));
				return (*this);
			}

			~SharedFuture() noexcept
			{	// destroy
			}

			const _Ty& get() const
			{	// block until ready then return the stored result or
				// throw the stored std::exception
				return (this->_Get_value());
			}

		public:
			void Wait() override
			{
				this->Wait_impl();
			}

			void WaitFor(const TimeSpan& timeout) override
			{
				this->WaitFor_impl(timeout);
			}

			AsyncOperationStatus GetStatus() const override
			{
				return this->GetStatus_impl();
			}

			Any GetResult() override
			{
				return  Any<_Ty>(get());
			}
		};

		template<class _Ty>
		class SharedFuture<_Ty&>
			: public State_manager<_Ty *>, public AsyncOperation
		{
			typedef State_manager<_Ty *> _Mybase;

		public:
			SharedFuture() noexcept
			{	// construct
			}

			SharedFuture(const SharedFuture& _Other)
				: _Mybase(_Other)
			{	// construct from SharedFuture object
			}

			SharedFuture& operator=(const SharedFuture& _Right)
			{	// assign from SharedFuture object
				_Mybase::operator=(_Right);
				return (*this);
			}

			SharedFuture(Future<_Ty&>&& _Other) noexcept
				: _Mybase(std::forward<_Mybase>(_Other))
			{	// construct from rvalue Future object
			}

			SharedFuture(SharedFuture&& _Other) noexcept
				: _Mybase(std::forward<SharedFuture>(_Other))
			{	// construct from rvalue SharedFuture object
			}

			SharedFuture& operator=(SharedFuture&& _Right) noexcept
			{	// assign from rvalue SharedFuture object
				_Mybase::operator=(std::forward<SharedFuture>(_Right));
				return (*this);
			}

			~SharedFuture() noexcept
			{	// destroy
			}

			_Ty& get() const
			{	// block until ready then return the stored result or
				// throw the stored std::exception
				return (*this->_Get_value());
			}

		public:
			void Wait() override
			{
				this->Wait_impl();
			}

			void WaitFor(const TimeSpan& timeout) override
			{
				this->WaitFor_impl(timeout);
			}

			AsyncOperationStatus GetStatus() const override
			{
				return this->GetStatus_impl();
			}

			Any GetResult() override
			{
				return Any<_Ty&>(get());
			}
		};

		template<>
		class SharedFuture<void>
			: public State_manager<int>, public AsyncOperation
		{
			typedef State_manager<int> _Mybase;

		public:
			SharedFuture() noexcept
			{	// construct
			}

			SharedFuture(const SharedFuture& _Other)
				: _Mybase(_Other)
			{	// construct from SharedFuture object
			}

			SharedFuture& operator=(const SharedFuture& _Right)
			{	// assign from SharedFuture object
				_Mybase::operator=(_Right);
				return (*this);
			}

			SharedFuture(SharedFuture&& _Other) noexcept
				: _Mybase(std::forward<SharedFuture>(_Other))
			{	// construct from rvalue SharedFuture object
			}

			SharedFuture(Future<void>&& _Other) noexcept
				: _Mybase(std::forward<_Mybase>(_Other))
			{	// construct from rvalue Future object
			}

			SharedFuture& operator=(SharedFuture&& _Right)
			{	// assign from rvalue SharedFuture object
				_Mybase::operator=(std::forward<SharedFuture>(_Right));
				return (*this);
			}

			~SharedFuture() noexcept
			{	// destroy
			}

			void get() const
			{	// block until ready then return or
				// throw the stored std::exception
				this->_Get_value();
			}

		public:
			void Wait() override
			{
				this->Wait_impl();
			}

			void WaitFor(const TimeSpan& timeout) override
			{
				this->WaitFor_impl(timeout);
			}

			AsyncOperationStatus GetStatus() const override
			{
				return this->GetStatus_impl();
			}

		};

		inline SharedFuture<void> Future<void>::share()
		{	// return state as SharedFuture
			return (SharedFuture<void>(std::move(*this)));
		}

		template<class _Ty>
		class Promiseimpl
		{
		public:
			Promiseimpl(Associated_state<_Ty> *_State_ptr)
				: _State(_State_ptr, false),
				_Future_retrieved(false)
			{
			}

			Promiseimpl(Promiseimpl&& _Other)
				: _State(std::forward<State_manager<_Ty> >(_Other._State)),
				_Future_retrieved(_Other._Future_retrieved)
			{	// construct from rvalue Promiseimpl object
			}

			Promiseimpl& operator=(Promiseimpl&& _Other)
			{	// assign from rvalue Promiseimpl object
				_State = std::move(_Other._State);
				_Future_retrieved = _Other._Future_retrieved;
				return (*this);
			}

			~Promiseimpl() noexcept
			{	// destroy
			}

			void _Swap(Promiseimpl& _Other)
			{	// exchange with _Other
				_State._Swap(_Other._State);
				std::swap(_Future_retrieved, _Other._Future_retrieved);
			}

			const State_manager<_Ty>& _Get_state() const
			{
				return (_State);
			}
			State_manager<_Ty>& _Get_state()
			{
				return (_State);
			}

			State_manager<_Ty>& _Get_state_for_set()
			{
				if (!_State.valid())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::no_state));
				return (_State);
			}

			State_manager<_Ty>& _Get_state_for_Future()
			{
				if (!_State.valid())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::no_state));
				if (_Future_retrieved)
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::future_already_retrieved));
				_Future_retrieved = true;
				return (_State);
			}

			bool _Is_valid() const noexcept
			{	// return status
				return (_State.valid());
			}

			bool _Is_ready() const
			{	// return status
				return (_State._Is_ready());
			}

			Promiseimpl(const Promiseimpl&) = delete;
			Promiseimpl& operator=(const Promiseimpl&) = delete;

		private:
			State_manager<_Ty> _State;
			bool _Future_retrieved;
		};

		template<class _Ty>
		class Promise
		{
		public:
			Promise()
				: _MyPromise(new Associated_state<_Ty>)
			{	// construct
			}

			template<class _Alloc>
			Promise(std::allocator_arg_t, const _Alloc& _Al)
				: _MyPromise(_Make_associated_state<_Ty>(_Al))
			{	// construct with allocator
			}

			Promise(Promise&& _Other) noexcept
				: _MyPromise(std::forward<Promiseimpl<_Ty> >(_Other._MyPromise))
			{	// construct from rvalue Promise object
			}

			Promise& operator=(Promise&& _Other) noexcept
			{	// assign from rvalue Promise object
				_MyPromise = std::forward<Promiseimpl<_Ty> >(_Other._MyPromise);
				return (*this);
			}

			~Promise() noexcept
			{	// destroy
				if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
				{	// std::exception if destroyed before function object returns
					std::future_error _Fut(std::make_error_code(std::future_errc::broken_Promise));
					_MyPromise._Get_state()
						._set_exception(std::make_exception_ptr(_Fut), false);
				}
			}

			void swap(Promise& _Other) noexcept
			{	// exchange with _Other
				_MyPromise._Swap(_Other._MyPromise);
			}

			Future<_Ty> get_Future()
			{	// return a Future object that shares the associated
				// asynchronous state
				return (Future<_Ty>(_MyPromise._Get_state_for_Future(), std::_Nil()));
			}

			void set_value(const _Ty& _Val)
			{	// store result
				_MyPromise._Get_state_for_set()._Set_value(_Val, false);
			}

			void set_value_at_thread_exit(const _Ty& _Val)
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._Set_value(_Val, true);
			}

			void set_value(_Ty&& _Val)
			{	// store result
				_MyPromise._Get_state_for_set()._Set_value(
					std::forward<_Ty>(_Val), false);
			}

			void set_value_at_thread_exit(_Ty&& _Val)
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._Set_value(
					std::forward<_Ty>(_Val), true);
			}

			void set_exception(std::exception_ptr _Exc)
			{	// store result
				_MyPromise._Get_state_for_set()._set_exception(_Exc, false);
			}

			void set_exception_at_thread_exit(std::exception_ptr _Exc)
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._set_exception(_Exc, true);
			}

			Promise(const Promise&) = delete;
			Promise& operator=(const Promise&) = delete;

		private:
			Promiseimpl<_Ty> _MyPromise;
		};

		template<class _Ty>
		class Promise<_Ty&>
		{	// class that defines an asynchronous provider that holds a reference
		public:
			Promise()
				: _MyPromise(new Associated_state<_Ty *>)
			{	// construct
			}

			template<class _Alloc>
			Promise(std::allocator_arg_t, const _Alloc& _Al)
				: _MyPromise(_Make_associated_state<_Ty *>(_Al))
			{	// construct with allocator
			}

			Promise(Promise&& _Other) noexcept
				: _MyPromise(std::forward<Promiseimpl<_Ty *> >(_Other._MyPromise))
			{	// construct from rvalue Promise object
			}

			Promise& operator=(Promise&& _Other) noexcept
			{	// assign from rvalue Promise object
				_MyPromise = std::forward<Promiseimpl<_Ty *> >(_Other._MyPromise);
				return (*this);
			}

			~Promise() noexcept
			{	// destroy
				if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
				{	// std::exception if destroyed before function object returns
					std::future_error _Fut(std::make_error_code(std::future_errc::broken_Promise));
					_MyPromise._Get_state()
						._set_exception(std::make_exception_ptr(_Fut), false);
				}
			}

			void swap(Promise& _Other) noexcept
			{	// exchange with _Other
				_MyPromise._Swap(_Other._MyPromise);
			}

			Future<_Ty&> get_Future()
			{	// return a Future object that shares the associated
				// asynchronous state
				return (Future<_Ty&>(_MyPromise._Get_state_for_Future(), std::_Nil()));
			}

			void set_value(_Ty& _Val)
			{	// store result
				_MyPromise._Get_state_for_set()._Set_value(&_Val, false);
			}

			void set_value_at_thread_exit(_Ty& _Val)
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._Set_value(&_Val, true);
			}

			void set_exception(std::exception_ptr _Exc)
			{	// store result
				_MyPromise._Get_state_for_set()._set_exception(_Exc, false);
			}

			void set_exception_at_thread_exit(std::exception_ptr _Exc)
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._set_exception(_Exc, true);
			}

			Promise(const Promise&) = delete;
			Promise& operator=(const Promise&) = delete;

		private:
			Promiseimpl<_Ty *> _MyPromise;
		};

		template<>
		class Promise<void>
		{	// defines an asynchronous provider that does not hold a value
		public:
			Promise()
				: _MyPromise(new Associated_state<int>)
			{	// construct
			}

			template<class _Alloc>
			Promise(std::allocator_arg_t, const _Alloc& _Al)
				: _MyPromise(_Make_associated_state<int>(_Al))
			{	// construct with allocator
			}

			Promise(Promise&& _Other) noexcept
				: _MyPromise(std::forward<Promiseimpl<int> >(_Other._MyPromise))
			{	// construct from rvalue Promise object
			}

			Promise& operator=(Promise&& _Other) noexcept
			{	// assign from rvalue Promise object
				_MyPromise = std::forward<Promiseimpl<int> >(_Other._MyPromise);
				return (*this);
			}

			~Promise() noexcept
			{	// destroy
				if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
				{	// std::exception if destroyed before function object returns
					std::future_error _Fut(std::make_error_code(std::future_errc::broken_promise));
					_MyPromise._Get_state()
						._set_exception(std::make_exception_ptr(_Fut), false);
				}
			}

			void swap(Promise& _Other) noexcept
			{	// exchange with _Other
				_MyPromise._Swap(_Other._MyPromise);
			}

			Future<void> get_Future()
			{	// return a Future object that shares the associated
				// asynchronous state
				return (Future<void>(_MyPromise._Get_state_for_Future(), std::_Nil()));
			}

			void set_value()
			{	// store a (void) result
				_MyPromise._Get_state_for_set()._Set_value(1, false);
			}

			void set_value_at_thread_exit()
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._Set_value(1, true);
			}

			void set_exception(std::exception_ptr _Exc)
			{	// store a result
				_MyPromise._Get_state_for_set()._set_exception(_Exc, false);
			}

			void set_exception_at_thread_exit(std::exception_ptr _Exc)
			{	// store result and block until thread exit
				_MyPromise._Get_state_for_set()._set_exception(_Exc, true);
			}

			Promise(const Promise&) = delete;
			Promise& operator=(const Promise&) = delete;

		private:
			Promiseimpl<int> _MyPromise;
		};

		template<class>
		class PackagedTask;

		template<class _Ret,
			class... _ArgTypes>
			class PackagedTask<_Ret(_ArgTypes...)>
		{
		public:
			typedef PackagedTask<_Ret(_ArgTypes...)> _Myt;
			typedef typename std::_P_arg_type<_Ret>::type _Ptype;
			typedef Promiseimpl<_Ptype> _MyPromiseType;
			typedef State_manager<_Ptype> _MyStateManagerType;
			typedef Packaged_state<_Ret(_ArgTypes...)> _MyStateType;

			PackagedTask() noexcept
				: _MyPromise(0)
			{	// construct
			}

			template<class _Fty2,
				class = typename std::enable_if<
				!is_same<typename std::decay<_Fty2>::type, _Myt>::value>::type>
				explicit PackagedTask(_Fty2&& _Fnarg)
				: _MyPromise(new _MyStateType(std::forward<_Fty2>(_Fnarg)))
			{	// construct from rvalue function object
			}

			PackagedTask(PackagedTask&& _Other) noexcept
				: _MyPromise(std::move(_Other._MyPromise))
			{	// construct from rvalue PackagedTask object
			}

			PackagedTask& operator=(PackagedTask&& _Other) noexcept
			{	// assign from rvalue PackagedTask object
				_MyPromise = std::move(_Other._MyPromise);
				return (*this);
			}

			template<class _Fty2,
				class _Alloc,
				class = typename std::enable_if<
				!is_same<typename std::decay<_Fty2>::type, _Myt>::value>::type>
				PackagedTask(std::allocator_arg_t, const _Alloc& _Al, _Fty2&& _Fnarg)
				: _MyPromise(_Make_packaged_state<_MyStateType>(
					std::forward<_Fty2>(_Fnarg), _Al))
			{	// construct from rvalue function object and allocator
			}

			~PackagedTask() noexcept
			{	// destroy
				_MyPromise._Get_state()._Abandon();
			}

			void swap(PackagedTask& _Other) noexcept
			{	// exchange with _Other
				std::swap(_MyPromise, _Other._MyPromise);
			}

			bool valid() const noexcept
			{	// return status
				return (_MyPromise._Is_valid());
			}

			Future<_Ret> get_Future()
			{	// return a Future object that shares the associated
				// asynchronous state
				return (Future<_Ret>(_MyPromise._Get_state_for_Future(), std::_Nil()));
			}

			void operator()(_ArgTypes... _Args)
			{	// call the function object
				if (_MyPromise._Is_ready())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
				_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr());
				_Ptr->_Call_immediate(std::forward<_ArgTypes>(_Args)...);
			}

			void make_ready_at_thread_exit(_ArgTypes... _Args)
			{	// call the function object and block until thread exit
				if (_MyPromise._Is_ready())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
				if (_State._Ptr()->_Already_has_stored_result())
					std::_Throw_future_error(
						std::make_error_code(std::future_errc::promise_already_satisfied));
				_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr());
				_Ptr->_Call_deferred(std::forward<_ArgTypes>(_Args)...);
			}

			void reset()
			{	// reset to newly constructed state
				_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
				_MyStateType *_MyState = static_cast<_MyStateType *>(_State._Ptr());
				function<_Ret(_ArgTypes...)> _Fnarg = _MyState->_Get_fn();
				_MyPromiseType _New_Promise(new _MyStateType(_Fnarg));
				_MyPromise._Get_state()._Abandon();
				_MyPromise._Swap(_New_Promise);
			}

			PackagedTask(const PackagedTask&) = delete;
			PackagedTask& operator=(const PackagedTask&) = delete;

		private:
			_MyPromiseType _MyPromise;
		};

		template<class _Ret,
			class _Fty> inline
			Associated_state<typename std::_P_arg_type<_Ret>::type>

			*_Get_associated_state(std::launch _Psync, _Fty&& _Fnarg)
		{	// construct associated asynchronous state object for the std::launch type
			switch (_Psync)
			{	// select std::launch type
			case std::launch::async:
				return (new Task_async_state<_Ret, false>(
					std::forward<_Fty>(_Fnarg)));
			case std::launch::deferred:
				return (new Deferred_async_state<_Ret>(
					std::forward<_Fty>(_Fnarg)));
			default:
				return (new Task_async_state<_Ret, true>(
					std::forward<_Fty>(_Fnarg)));
			}
		}

		template<class _Fty,
			class... _ArgTypes> inline
			Future<std::result_of_t<std::decay_t<_Fty>(std::decay_t<_ArgTypes>...)>>
			Async(std::launch _Policy, _Fty&& _Fnarg, _ArgTypes&&... _Args)
		{	// return a Future object whose associated asynchronous state
			// manages a callable object launched with supplied policy
			typedef std::result_of_t<std::decay_t<_Fty>(std::decay_t<_ArgTypes>...)> _Ret;
			typedef typename std::_P_arg_type<_Ret>::type _Ptype;
			Promiseimpl<_Ptype> _Pr(_Get_associated_state<_Ret>(_Policy,
				_Fake_no_copy_callable_adapter<_Fty, _ArgTypes...>(
					std::forward<_Fty>(_Fnarg),
					std::forward<_ArgTypes>(_Args)...
					)));

			return (Future<_Ret>(_Pr._Get_state_for_Future(), std::_Nil()));
		}

		template<class _Fty,
			class... _ArgTypes> inline
			Future<std::result_of_t<std::decay_t<_Fty>(std::decay_t<_ArgTypes>...)>>
			Async(_Fty&& _Fnarg, _ArgTypes&&... _Args)
		{	
			return (std::Async(std::launch::async | std::launch::deferred,
				std::forward<_Fty>(_Fnarg),
				std::forward<_ArgTypes>(_Args)...
			));
		}
	}
}

namespace std 
{
	template<class _Ty> inline
		void swap(System::Threading::Promise<_Ty>& _Left, System::Threading::Promise<_Ty>& _Right) noexcept
	{
		_Left.swap(_Right);
	}

	template<class _Ty> inline
		void swap(System::Threading::PackagedTask<_Ty>& _Left,
			System::Threading::PackagedTask<_Ty>& _Right) noexcept
	{
		_Left.swap(_Right);
	}

	template<class _Ty,
		class _Alloc>
		struct uses_allocator<System::Threading::Promise<_Ty>, _Alloc>
		: true_type
	{
	};

	template<class _Ty,
		class _Alloc>
		struct uses_allocator<System::Threading::PackagedTask<_Ty>, _Alloc>
		: true_type
	{
	};
}