#pragma once
#include "PtrType_impl.h"
#include "RefType_impl.h"
#include "ModifiedType_impl.h"

namespace System
{
	class TypeRecycle
	{
	public :
		static void Recycle()
		{
			PtrType_impl::Recycle();
			RefType_impl::Recycle();
			ModifiedType_impl::Recycle();
		}
	};
}