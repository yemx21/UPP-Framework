#pragma once
#include "Config.h"
namespace System
{
	/// <summary>
	/// 临界区类
	/// </summary>
	class RUNTIME_API CriticalSection
	{
	private:
		void* impl;
		CriticalSection(const CriticalSection&) = delete;
		CriticalSection& operator=(const CriticalSection&) = delete;
	public:
		/// <summary>
		/// 创建<see cref="CriticalSection"/>实例.
		/// </summary>
		CriticalSection();
		~CriticalSection();

		/// <summary>
		/// 尝试进入临界区
		/// </summary>
		/// <returns></returns>
		bool TryEnter();

		/// <summary>
		/// 进入临界区
		/// </summary>
		void Enter();

		/// <summary>
		/// 离开临界区
		/// </summary>
		void Leave();
	};

}

#include <mutex>

namespace std
{
	template<>
	class lock_guard<System::CriticalSection>
	{	
	public:
		typedef System::CriticalSection mutex_type;

		explicit lock_guard(System::CriticalSection& _Mtx)
			: _Mutex(_Mtx)
		{	
			_Mutex.Enter();
		}

		lock_guard(System::CriticalSection& _Mtx, adopt_lock_t)
			: _Mutex(_Mtx)
		{
		}

		~lock_guard() noexcept
		{	
			_Mutex.Leave();
		}

		lock_guard(const lock_guard&) = delete;
		lock_guard& operator=(const lock_guard&) = delete;

	private:
		System::CriticalSection& _Mutex;
	};

}