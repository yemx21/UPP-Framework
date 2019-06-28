#pragma once
#include "Config.h"
namespace System
{
	namespace Audio
	{
		class Core_base_impl;
		class Core_base;
		class PluginCenter;

		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API CoreEventFlags : __uobject
		{
			__uvalue_begin(CoreEventFlags, UShort)
			__uvalue(None)
			__uvalue(Tip)///提示
			__uvalue(Info)///信息（一般含有显示给用户的信息）
			__uvalue(Error)///错误
			__uvalue_end(CoreEventFlags, None)
		};


		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API CoreEvents : __uobject
		{
			__uvalue_begin(CoreEvents, UShort)
			__uvalue(None)
			__uvalue(Exception)
			__uvalue(MemoryError)
			__uvalue(AddinError)
			__uvalue(Crash)

			__uvalue(PlayCompleted, 100)
			__uvalue(PlayAutoChangeStart)
			__uvalue(SeekCompleted)
			__uvalue(StateChanged)
			__uvalue(ThreadExit)

			__uvalue(User, 1000)
			__uvalue_end(CoreEvents, None)
		};

		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API CoreObjects : __uobject
		{
			__uvalue_begin(CoreObjects, UShort)
			__uvalue(None)
			__uvalue(Player)
			__uvalue(Pipe)
			__uvalue(Lyric)
			__uvalue_end(CoreObjects, None)
		};


		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API CoreSubObjects : __uobject
		{
			__uvalue_begin(CoreSubObjects, UShort)
			__uvalue(None)
			__uvalue(AudioDecoder)
			__uvalue(AudioDSP)
			__uvalue(AudioDevice)
			__uvalue(AudioVisual)
			__uvalue(AudioEncoder)

			__uvalue(LryicDecoder, 100)
			__uvalue(LryicEncoder, 101)
			__uvalue(LryicVisual, 102)

			__uvalue_end(CoreSubObjects, None)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API Core_base : __uobject
		{
		private:
			friend class PluginCenter;
			Core_base_impl* impl;
			void OnCapture(Core_base* sender, CoreEventFlags flag, CoreEvents eventID, const Any& UserIdentity, CoreObjects obj, CoreSubObjects subObj, double fireTimeMs, double raiseTimeMs, const UInt128& objectID, const wstring& msg, const VariadicAny& args);
		protected:
			safe_void_ptr GetHost() const;

			__uproperty(get = GetHost) safe_void_ptr Host;

			virtual void OnEvent(Core_base* sender, CoreEventFlags flag, CoreEvents eventID, const Any& UserIdentity, CoreObjects obj, CoreSubObjects subObj, double fireTimeMs, double raiseTimeMs, const UInt128& objectID, const wstring& msg, const VariadicAny& args);
			__uref void RaiseEvent(CoreEventFlags flag, CoreEvents eventID, CoreObjects obj, CoreSubObjects subObj, const UInt128& objectID, const wstring& msg, const VariadicAny& args);
			__uref void RaiseEvent(CoreEventFlags flag, CoreEvents eventID, const Any& UserIdentity, CoreObjects obj, CoreSubObjects subObj, const UInt128& objectID, const wstring& msg, const VariadicAny& args);
			Core_base();
		public:
			virtual ~Core_base();
			REFLECT_CLASS(Core_base)
		};
	}
}