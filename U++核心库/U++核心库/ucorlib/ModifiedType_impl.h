#pragma once
#include "Any.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"


namespace System
{
	class ModifiedType_impl
	{
	private:
		static std::mutex& getlocker();
		static std::unordered_map<size_t, ModifiedType*>&  getcaches();
		static std::hash<std::wstring> hash_fn;

		ModifiedType_impl() = delete;

	public:
		static void Recycle();

		~ModifiedType_impl() {}
		static const ModifiedType* Create(_type hold, Modifiers modifier);

	};
}