#pragma once
#include "Config.h"
#include <Windows.h>
#include <Ks.h>
#include <Ksmedia.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>

namespace System
{
	namespace Audio
	{
		class AudioDevice_Wasapi;

		class WasapiPipe
		{
		private:
			friend class AudioDevice_Wasapi;
			HANDLE hBufferEvent;
		    IAudioClient* pAudioClient;
			static IMMDevice *pDevice;
			static bool isinited;
			WAVEFORMATEXTENSIBLE wfx;
			SampleInfo sinfo;
			SampleInfo oinfo;

			IAudioRenderClient* pRenderClient;
			IAudioStreamVolume* volctr;
			IAudioClock *clock;
			UINT32 framebuffercount;

			double vol;
			double pan;
			UInt64 written;

			size_t poolsize;
			BYTE* pool;

			DWORD tid;

			HANDLE FillThreadHandle;
			bool KillFillThread;

			HANDLE Operation_RenderClient;
			static unsigned int _stdcall FillThread(void* Obj);

			WasapiPipe();
		public:
			static Result<bool> InitEngine();

			static void ExitEngine();

			static WasapiPipe* Create(const System::Audio::SampleInfo&, UInt64 bufferTimeMs, System::Audio::SampleInfo& actualinfo, UInt64& actualbufferTimeMs);
			virtual ~WasapiPipe();

			UInt64 Write(const BYTE* buf, UInt64 bytes, bool& result);
			int CanWrite();

			bool Play();
			bool Pause();
			bool Stop();
			bool Flush(UInt64 timeMs);
			double SetVolume(double);
			double SetPan(double);

			double GetOutputTime();	
			double GetWrittenTime();
			UInt64 GetWrittenPosition();
			bool WaitEnd();

		};

	}
}

