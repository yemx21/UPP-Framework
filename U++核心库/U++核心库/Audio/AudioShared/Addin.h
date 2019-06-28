#pragma once
#include "AudioTypes.h"
#include "Core.h"
#include "AudioBuffer.h"

namespace System
{
	namespace Audio
	{
		__uclass  __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API __uref Addin_base : public Core_base
		{
		public:
			__uref virtual Version GetVersion() const = 0;
			__uref virtual const wchar_t* GetAuthor() const = 0;
			__uref virtual const wchar_t* GetDescription() const = 0;
			__uref virtual const wchar_t* GetSupportFormat() const = 0;

			REFLECT_CLASS(Addin_base)
		};
	}
}