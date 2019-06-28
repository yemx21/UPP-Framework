#pragma once
#include "Delegate.h"
#include <functional>

namespace System
{
	template<class ...T>
	class Action;

	template<>
	class Action<> : public Delegate, public std::function<void()>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke() const
		{
			this->operator()();
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				this->operator()();
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				this->operator()();
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __P0>
	class Action<__P0> : public Delegate, public std::function<void(__P0)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0) const
		{
			this->operator()(p0);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 1) return false;
				__P0 param = VariadicAnyVisitor<__P0>::Get(args, 0);
				Invoke(param);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 1) return false;
				__P0 param = VariadicAnyVisitor<__P0>::Get(args, 0);
				Invoke(param);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1>
	class Action<__P0, __P1> : public Delegate, public std::function<void(__P0, __P1)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1) const
		{
			this->operator()(p0, p1);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 2) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				Invoke(param1, param2);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 2) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				Invoke(param1, param2);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2>
	class Action<__P0, __P1, __P2> : public Delegate, public std::function<void(__P0, __P1, __P2)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2) const
		{
			this->operator()(p0, p1, p2);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 3) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				Invoke(param1, param2, param3);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 3) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				Invoke(param1, param2, param3);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3>
	class Action<__P0, __P1, __P2, __P3> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3) const
		{
			this->operator()(p0, p1, p2, p3);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 4) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				Invoke(param1, param2, param3, param4);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 4) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				Invoke(param1, param2, param3, param4);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3, typename __P4>
	class Action<__P0, __P1, __P2, __P3, __P4> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3, __P4)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4) const
		{
			this->operator()(p0, p1, p2, p3, p4);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 5) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				Invoke(param1, param2, param3, param4, param5);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 5) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				Invoke(param1, param2, param3, param4, param5);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
	class Action<__P0, __P1, __P2, __P3, __P4, __P5> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3, __P4, __P5)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5) const
		{
			this->operator()(p0, p1, p2, p3, p4, p5);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 6) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				Invoke(param1, param2, param3, param4, param5, param6);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 6) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				Invoke(param1, param2, param3, param4, param5, param6);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
	class Action<__P0, __P1, __P2, __P3, __P4, __P5, __P6> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3, __P4, __P5, __P6)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6) const
		{
			this->operator()(p0, p1, p2, p3, p4, p5, p6);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 7) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5); 
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				Invoke(param1, param2, param3, param4, param5, param6, param7);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 7) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				Invoke(param1, param2, param3, param4, param5, param6, param7);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
	class Action<__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6, __P7 p7) const
		{
			this->operator()(p0, p1, p2, p3, p4, p5, p6, p7);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 8) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				__P7 param8 = VariadicAnyVisitor<__P7>::Get(args, 7);
				Invoke(param1, param2, param3, param4, param5, param6, param7, param8);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 8) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				__P7 param8 = VariadicAnyVisitor<__P7>::Get(args, 7);
				Invoke(param1, param2, param3, param4, param5, param6, param7, param8);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
	class Action<__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6, __P7 p7, __P8 p8) const
		{
			this->operator()(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 9) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				__P7 param8 = VariadicAnyVisitor<__P7>::Get(args, 7);
				__P8 param9 = VariadicAnyVisitor<__P8>::Get(args, 8);
				Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 9) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				__P7 param8 = VariadicAnyVisitor<__P7>::Get(args, 7);
				__P8 param9 = VariadicAnyVisitor<__P8>::Get(args, 8);
				Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
	};

	template<typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
	class Action<__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9> : public Delegate, public std::function<void(__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		void Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6, __P7 p7, __P8 p8, __P9 p9) const
		{
			this->operator()(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		/// <summary>
		/// 判断是否有返回值
		/// </summary>
		bool HasReturn() const override
		{
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 10) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				__P7 param8 = VariadicAnyVisitor<__P7>::Get(args, 7);
				__P8 param9 = VariadicAnyVisitor<__P8>::Get(args, 8);
				__P9 param10 = VariadicAnyVisitor<__P9>::Get(args, 9);
				Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

		/// <summary>
		/// 通用调用
		/// </summary>
		/// <param name="result">返回值</param>
		/// <param name="sender">发送者</param>
		/// <param name="args">事件参数</param>
		/// <param name="nothrow">无错误捕捉</param>
		/// <returns></returns>
		bool GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow = true) const override
		{
			try
			{
				if (args.Size < 10) return false;
				__P0 param1 = VariadicAnyVisitor<__P0>::Get(args, 0);
				__P1 param2 = VariadicAnyVisitor<__P1>::Get(args, 1);
				__P2 param3 = VariadicAnyVisitor<__P2>::Get(args, 2);
				__P3 param4 = VariadicAnyVisitor<__P3>::Get(args, 3);
				__P4 param5 = VariadicAnyVisitor<__P4>::Get(args, 4);
				__P5 param6 = VariadicAnyVisitor<__P5>::Get(args, 5);
				__P6 param7 = VariadicAnyVisitor<__P6>::Get(args, 6);
				__P7 param8 = VariadicAnyVisitor<__P7>::Get(args, 7);
				__P8 param9 = VariadicAnyVisitor<__P8>::Get(args, 8);
				__P9 param10 = VariadicAnyVisitor<__P9>::Get(args, 9);
				Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	public:
#pragma region Action

		Action() noexcept
		{
		}

		Action(nullptr_t) noexcept
		{
		}

		Action(const Action& _Right)
		{
			this->_Reset_copy(_Right);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(_Fx _Func)
		{
			this->_Reset(std::move(_Func));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc&, nullptr_t) noexcept
		{
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Ax, const Action& _Right)
		{
			this->_Reset_alloc(_Right, _Ax);
		}

		template<class _Fx,
			class _Alloc,
			class = typename _Mybase::template std::_Enable_if_callable_t<_Fx&, std::function>>
			Action(std::allocator_arg_t, const _Alloc& _Ax, _Fx _Func)
		{
			this->_Reset_alloc(std::move(_Func), _Ax);
		}

		~Action() noexcept
		{
		}

		Action& operator=(const Action& _Right)
		{
			Action(_Right).swap(*this);
			return (*this);
		}

		Action(Action&& _Right)
		{
			this->_Reset_move(std::move(_Right));
		}

		template<class _Alloc>
		Action(std::allocator_arg_t, const _Alloc& _Al, Action&& _Right)
		{
			this->_Reset_alloc(std::move(_Right), _Al);
		}

		Action& operator=(Action&& _Right)
		{
			if (this != &_Right)
			{
				this->_Tidy();
				this->_Reset_move(std::move(_Right));
			}
			return (*this);
		}

		template<class _Fx,
			class = typename _Mybase::template std::_Enable_if_callable_t<std::decay_t<_Fx>&, std::function>>
			Action& operator=(_Fx&& _Func)
		{
			Action(std::forward<_Fx>(_Func)).swap(*this);
			return (*this);
		}

#if _HAS_FUNCTION_ALLOCATOR_SUPPORT
		template<class _Fx,
			class _Alloc>
			void assign(_Fx&& _Func, const _Alloc& _Ax)
		{
			Action(allocator_arg, _Ax, std::forward<_Fx>(_Func)).swap(*this);
		}
#endif

		Action& operator=(nullptr_t) noexcept
		{
			this->_Tidy();
			return (*this);
		}

		template<class _Fx>
		Action& operator=(std::reference_wrapper<_Fx> _Func) noexcept
		{
			this->_Tidy();
			this->_Reset(_Func);
			return (*this);
		}

		void swap(Action& _Right) noexcept
		{
			this->_Swap(_Right);
		}

		explicit operator bool() const noexcept
		{
			return (!this->_Empty());
		}

#if _HAS_STATIC_RTTI
		_NODISCARD const std::type_info& target_type() const noexcept
		{
			return (this->_Target_type());
		}

		template<class _Fx>
		_NODISCARD _Fx * target() noexcept
		{
			return (reinterpret_cast<_Fx *>(const_cast<void *>(this->_Target(typeid(_Fx)))));
		}

		template<class _Fx>
		_NODISCARD const _Fx * target() const noexcept
		{
			return (reinterpret_cast<const _Fx *>(this->_Target(typeid(_Fx))));
		}
#else
		const std::type_info& target_type() const noexcept = delete;

		template<class _Fx>
		_Fx *target() noexcept = delete;

		template<class _Fx>
		const _Fx *target() const noexcept = delete;
#endif
#pragma endregion
	};

}