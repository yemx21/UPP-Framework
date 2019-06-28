#pragma once

#include "PtrType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"

namespace System
{
	class PtrType_impl
	{
	private:
		static std::mutex& getlocker();
		static std::unordered_map<size_t, PtrType*>& getcaches();
		static std::hash<std::wstring> hash_fn;
		PtrType_impl() = delete;
	public:
		static void Recycle();

		static PtrType* Create(_type type, UInt dimension);
		~PtrType_impl() {}

	};
}