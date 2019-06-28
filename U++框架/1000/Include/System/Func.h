#pragma once
#include "Delegate.h"
#include <functional>

namespace System
{
	template<class __R, class ...T>
	class Func;

	template<typename __R>
	class Func<__R> : public Delegate, public std::function<__R()>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke() const
		{
			return this->operator()();
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
				result = this->operator()();
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0>
	class Func<__R, __P0> : public Delegate, public std::function<__R(__P0)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0) const
		{
			return this->operator()(p0);
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

	};

	template<typename __R, typename __P0, typename __P1>
	class Func<__R, __P0, __P1> : public Delegate, public std::function<__R(__P0, __P1)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1) const
		{
			return this->operator()(p0, p1);
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
				result = Invoke(param1, param2);
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
				result = Invoke(param1, param2);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2>
	class Func<__R, __P0, __P1, __P2> : public Delegate, public std::function<__R(__P0, __P1, __P2)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2) const
		{
			return this->operator()(p0, p1, p2);
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
				result = Invoke(param1, param2, param3);
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
				result = Invoke(param1, param2, param3);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3>
	class Func<__R, __P0, __P1, __P2, __P3> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3) const
		{
			return this->operator()(p0, p1, p2, p3);
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
				result = Invoke(param1, param2, param3, param4);
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
				result = Invoke(param1, param2, param3, param4);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3, typename __P4>
	class Func<__R, __P0, __P1, __P2, __P3, __P4> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3, __P4)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4) const
		{
			return this->operator()(p0, p1, p2, p3, p4);
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
				result = Invoke(param1, param2, param3, param4, param5);
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
				result = Invoke(param1, param2, param3, param4, param5);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5>
	class Func<__R, __P0, __P1, __P2, __P3, __P4, __P5> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3, __P4, __P5)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5) const
		{
			return this->operator()(p0, p1, p2, p3, p4, p5);
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
				result = Invoke(param1, param2, param3, param4, param5, param6);
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
				result = Invoke(param1, param2, param3, param4, param5, param6);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6>
	class Func<__R, __P0, __P1, __P2, __P3, __P4, __P5, __P6> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3, __P4, __P5, __P6)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6) const
		{
			return this->operator()(p0, p1, p2, p3, p4, p5, p6);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7>
	class Func<__R, __P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6, __P7 p7) const
		{
			return this->operator()(p0, p1, p2, p3, p4, p5, p6, p7);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7, param8);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7, param8);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8>
	class Func<__R, __P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6, __P7 p7, __P8 p8) const
		{
			return this->operator()(p0, p1, p2, p3, p4, p5, p6, p7, p8);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

	template<typename __R, typename __P0, typename __P1, typename __P2, typename __P3, typename __P4, typename __P5, typename __P6, typename __P7, typename __P8, typename __P9>
	class Func<__R, __P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9> : public Delegate, public std::function<__R(__P0, __P1, __P2, __P3, __P4, __P5, __P6, __P7, __P8, __P9)>
	{
	public:
		/// <summary>
		/// 调用
		/// </summary>
		__R Invoke(__P0 p0, __P1 p1, __P2 p2, __P3 p3, __P4 p4, __P5 p5, __P6 p6, __P7 p7, __P8 p8, __P9 p9) const
		{
			return this->operator()(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10);
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
				result = Invoke(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10);
				return true;
			}
			catch (...)
			{
				if (!nothrow) throw;
			}
			return false;
		}

	};

}