#pragma once
#include "..\Core.h"
namespace System
{
	namespace Audio
	{
		class PluginCenter;

		typedef void(*InternalPostCallBack)(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args);
		class Core_base_impl
		{
		protected:
			friend class Core_base;
			friend class PluginCenter;
			InternalPostCallBack _post;
			safe_void_ptr _owner;
			PluginCenter* _center;
			Core_base_impl(): _owner(0), _post(nullptr), _center(nullptr) {}
			~Core_base_impl() {}
		};

	}
}