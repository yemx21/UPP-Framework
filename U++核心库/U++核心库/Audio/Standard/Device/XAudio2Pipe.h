#pragma once
#include "Config.h"
#include <Ks.h>
#include <Ksmedia.h>
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) 
#include <xaudio2.h> 
#pragma comment(lib,"xaudio2.lib") 
#else 
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\comdecl.h> 
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h> 
#endif 

#define STREAMING_BUFFER_SIZE 65536 
#define MAX_BUFFER_COUNT 3 

class XAudio2Pipe : IXAudio2VoiceCallback
{
private:
	HANDLE hBufferEndEvent;
	static IXAudio2* pXAudio2;
	static IXAudio2MasteringVoice* pMasteringVoice;
	WAVEFORMATEXTENSIBLE wfx;
	IXAudio2SourceVoice* pSourceVoice; 
	CRITICAL_SECTION cs; 
	OVERLAPPED ovlCurrentRequest; 
	std::unique_ptr<uint8_t[]> buffers[MAX_BUFFER_COUNT];
	XAudio2Pipe();
public:
	XAudio2Pipe* Create(const System::Audio::SampleInfo&);
	virtual ~XAudio2Pipe();

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
	STDMETHOD_(void, OnBufferEnd)(void*) override
	{
		SetEvent(hBufferEndEvent);
	}
	STDMETHOD_(void, OnLoopEnd)(void*) override
	{
	}
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override
	{
	}
	static bool InitEngine();



};
