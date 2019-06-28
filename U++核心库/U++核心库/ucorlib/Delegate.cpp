#include "Delegate.h"

using namespace System;

bool Delegate::GenericInvoke(const Any& sender, const OptionalVariadicAny& args, bool nothrow) const
{
	return false;
}

bool Delegate::GenericInvoke(Any& result, const Any& sender, const OptionalVariadicAny& args, bool nothrow) const
{
	return false;
}

bool Delegate::HasReturn() const
{
	return false;
}