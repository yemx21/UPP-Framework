#include "Event.h"

using namespace System;

EventArgs::EventArgs() {}
EventArgs::~EventArgs() {}

EventArgs::EventArgs(const EventArgs&) {}

__forceinline EventArgs EventArgs::Empty()
{
	static EventArgs empty;
	return empty;
}