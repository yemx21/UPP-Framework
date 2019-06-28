#include "Assembly_impl.h"
#include "WindowsEnvironment.h"
using namespace System;
using namespace System::OS;

namespace System
{
	class yield_iterator_warpper
	{
	public:
		void* m_arg;
		void* m_fiber;
		void* m_iteratorFiber;
		bool m_initialized;
		bool m_done;
		bool m_throws;
		std::exception m_ex;
		yield_iterator_impl* m_owner;

		yield_iterator_warpper(yield_iterator_impl* owner, void* arg) : m_owner(owner), m_arg(arg), m_fiber(0), m_iteratorFiber(0), m_initialized(false), m_done(false), m_throws(false)
		{

		}

		~yield_iterator_warpper()
		{
			if (m_initialized && !m_done)
			{
				m_done = true;
				Microsoft::winapi_SwitchToFiber(m_iteratorFiber);
				Microsoft::winapi_DeleteFiber(m_iteratorFiber);
			}
		}

		void setexception(const std::exception& ex)
		{
			m_ex = ex;
			m_throws = true;
		}

		void iterateImpl();

		void ensureiterateImpl()
		{
			if (!m_initialized) iterateImpl();
		}

		static void API_STDCALL iteratorFiberProc(void *pv)
		{
			yield_iterator_warpper *yit = static_cast<yield_iterator_warpper*>(pv);

			try
			{
				yit->m_owner->oniterate(yit->m_arg);
			}
			catch (std::exception& e)
			{
				yit->setexception(e);
			}
			catch (const char *str)
			{
				yit->setexception(std::exception(str));
			}
			catch (...)
			{
				yit->setexception(std::exception(
					"Unknown std::exception thrown in yield_iterator::iterate."));
			}

			yit->m_done = true;

			Microsoft::winapi_SwitchToFiber(yit->m_fiber);
		}


		void onyieldImpl()
		{
			Microsoft::winapi_SwitchToFiber(m_fiber);
		}

		__forceinline bool done()
		{
			return m_done;
		}

	};

	void yield_iterator_warpper::iterateImpl()
	{
		if (!m_initialized)
		{
			m_initialized = true;

			m_fiber = Microsoft::winapi_GetCurrentFiber();

			if (m_fiber == (void*)0x1E00)
				m_fiber = Microsoft::winapi_ConvertThreadToFiber(this);
			else
			{
				if (Microsoft::winapi_IsBadReadPtr(m_fiber, sizeof(m_fiber)))
					m_fiber = Microsoft::winapi_ConvertThreadToFiber(this);
			}
			m_iteratorFiber = Microsoft::winapi_CreateFiber(4096, iteratorFiberProc, this);
		}

		Microsoft::winapi_SwitchToFiber(m_iteratorFiber);

		if (m_done)
			Microsoft::winapi_DeleteFiber(m_iteratorFiber);

		if (m_throws)
			throw m_ex;
	}

	yield_iterator_impl::yield_iterator_impl(void* arg)
	{
		warpper = new yield_iterator_warpper(this, arg);
	}

	yield_iterator_impl::~yield_iterator_impl()
	{
		yield_iterator_warpper* yiw = (yield_iterator_warpper*)warpper;
		if (yiw) { delete yiw; yiw = nullptr; warpper = nullptr; }
	}

	void yield_iterator_impl::oniterate(void* p)
	{

	}

	void yield_iterator_impl::ensureiterateImpl()
	{
		yield_iterator_warpper* yiw = (yield_iterator_warpper*)warpper;
		if (yiw) yiw->ensureiterateImpl();
	}

	void yield_iterator_impl::iterateImpl()
	{
		yield_iterator_warpper* yiw = (yield_iterator_warpper*)warpper;
		if (yiw) yiw->iterateImpl();
	}

	void yield_iterator_impl::onyieldImpl()
	{
		yield_iterator_warpper* yiw = (yield_iterator_warpper*)warpper;
		if (yiw) yiw->onyieldImpl();
	}

	bool yield_iterator_impl::done()
	{
		yield_iterator_warpper* yiw = (yield_iterator_warpper*)warpper;
		if (yiw) return yiw->done();
		return true;
	}
}