#include "Core.h"
#include "Internal\Core_base_impl.h"

using namespace System::Audio;


void Core_base::OnCapture(Core_base* sender, CoreEventFlags flag, CoreEvents eventID, const Any& userIdentity, CoreObjects obj, CoreSubObjects subObj, double fireTimeMs, double raiseTimeMs, const UInt128& objectID, const wstring& msg, const VariadicAny& args)
{
	if (sender) OnEvent(sender, flag, eventID, userIdentity, obj, subObj, fireTimeMs, raiseTimeMs, objectID, msg, args);
}

safe_void_ptr Core_base::GetHost() const
{
	return impl != nullptr ? impl->_owner : 0;
}

void Core_base::OnEvent(Core_base* sender, CoreEventFlags flag, CoreEvents eventID, const Any& userIdentity, CoreObjects obj, CoreSubObjects subObj, double fireTimeMs, double raiseTimeMs, const UInt128& objectID, const wstring& msg, const VariadicAny& args)
{

}

void Core_base::RaiseEvent(CoreEventFlags flag, CoreEvents eventID, CoreObjects obj, CoreSubObjects subObj, const UInt128& objectID, const wstring& msg, const VariadicAny& args)
{
	if (impl)
	if (impl->_post)
		impl->_post(impl->_center, flag, eventID, Any(_missing), obj, subObj, objectID, msg, args);
}

void Core_base::RaiseEvent(CoreEventFlags flag, CoreEvents eventID, const Any& userIdentity, CoreObjects obj, CoreSubObjects subObj, const UInt128& objectID, const wstring& msg, const VariadicAny& args)
{
	if (impl) 
		if(impl->_post)
			impl->_post(impl->_center, flag, eventID, userIdentity, obj, subObj, objectID, msg, args);
}

Core_base::Core_base()
{
	impl = new Core_base_impl();
}

Core_base::~Core_base()
{
	if (impl){ delete impl; impl = nullptr; }
}


