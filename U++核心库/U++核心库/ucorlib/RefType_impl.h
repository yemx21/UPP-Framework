#pragma once
#include "RefType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"

namespace System
{
	class RefType_impl
	{
	private:
		static std::mutex& getlocker();
		static std::unordered_map<size_t, RefType*>& getcaches();
		static std::hash<std::wstring> hash_fn;
		RefType_impl(_type type, RefKinds pos) = delete;
	public:
		static void Recycle();

		static RefType* Create(_type type, RefKinds pos);

		~RefType_impl() {}
	};
}