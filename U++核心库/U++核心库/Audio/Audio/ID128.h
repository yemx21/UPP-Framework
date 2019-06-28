#pragma once
#include "Config.h"
namespace System
{
	namespace ObjectManager
	{
		class ID128
		{
		protected:
			UInt128 _id; 
			ID128(): _id(0ULL) {}
			ID128(UInt128 id) : _id(id) {}
		public:
			virtual ~ID128() {};
			UInt128 GetId() const { return _id; }
			_declspec(property(get = GetId)) UInt128 Id;
		};

	}
}