#pragma once
#include "Config.h"
#include <Windows.h>
#include <Ks.h>
#include <Ksmedia.h>
#include <xaudio2.h> 

#define BUFFERCOUNT 4
#define BUFFERWAIT 2

namespace System
{
	namespace Audio
	{
		class AudioDevice_XA2;

		class XAudio2Pipe : IXAudio2VoiceCallback
		{
		private:
			friend class AudioDevice_XA2;
			
			static IXAudio2* pXAudio2;
			static IXAudio2MasteringVoice* pMasteringVoice;
			static bool isinited;
			bool KillFillThread;
			
			SampleInfo sinfo;
			size_t poolsize;
			size_t blocksize;			
			size_t poolcur;
			size_t poolplaycur;
			BYTE* pool;
			IXAudio2SourceVoice* pSourceVoice;
			UInt64 writeroffset;
			UINT64 start;
			HANDLE locker;
			HANDLE FillThreadHandle;
			HANDLE hBufferEndEvent;
			UInt64 fillcur;
			double vol;
			double pan;
			std::deque<size_t> xlens;
			WAVEFORMATEXTENSIBLE wfx;
			static unsigned int _stdcall FillThread(void* Obj);
			XAudio2Pipe();
		public:
			static Result<bool> InitEngine();

			static void ExitEngine();

			static XAudio2Pipe* Create(UInt64 desiredBufferMs, const SampleInfo& desiredInfo, SampleInfo& actualInfo, UInt64& actualBufferMs);
			virtual ~XAudio2Pipe();

			size_t Write(const BYTE* buf, size_t bytes);
			size_t CanWrite();

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

			STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) override
			{
			}
			STDMETHOD_(void, OnVoiceProcessingPassEnd)() override
			{
			}
			STDMETHOD_(void, OnStreamEnd)() override
			{
			}
			STDMETHOD_(void, OnBufferStart)(void*) override
			{
			}
			STDMETHOD_(void, OnBufferEnd)(void* context) override
			{
				WaitForSingleObject(locker, INFINITE);
				if (!xlens.empty())
				{
					size_t len = xlens.front();
					xlens.pop_front();
					poolplaycur += len;
					if (poolplaycur >= poolsize) poolplaycur = poolplaycur - poolsize;
				}
				ReleaseMutex(locker);
				SetEvent(hBufferEndEvent);
			}
			STDMETHOD_(void, OnLoopEnd)(void*) override
			{
			}
			STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override
			{
			}
			
		};

	}
}

