#include "XAudio2Pipe.h"

IXAudio2* XAudio2Pipe::pXAudio2 = nullptr;
IXAudio2MasteringVoice* XAudio2Pipe::pMasteringVoice = nullptr;
XAudio2Pipe::XAudio2Pipe() :
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA) 
hBufferEndEvent(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))
#else 
hBufferEndEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
#endif 
{
	InitializeCriticalSection(&cs); 
	ovlCurrentRequest = { 0 }; 
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	ovlCurrentRequest.hEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE);
#else 
	ovlCurrentRequest.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
#endif 
	for (size_t j = 0; j < MAX_BUFFER_COUNT; ++j)
	{
		buffers[j].reset(new uint8_t[STREAMING_BUFFER_SIZE]);
	}
}

XAudio2Pipe* XAudio2Pipe::Create(const System::Audio::SampleInfo& info)
{
	XAudio2Pipe* pipe = new XAudio2Pipe();
	memset(&pipe->wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	pipe->wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	pipe->wfx.Format.nChannels = info.Channels;
	pipe->wfx.Format.nSamplesPerSec = info.SampleRate;
	pipe->wfx.Format.nAvgBytesPerSec = info.AvgBytesPerSec;
	pipe->wfx.Format.wBitsPerSample = info.BitsPerSample.BPS;
	pipe->wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE)-sizeof(WAVEFORMATEX);
	pipe->wfx.Samples.wValidBitsPerSample = pipe->wfx.Format.wBitsPerSample;
	pipe->wfx.Format.nBlockAlign = info.BlockAlign;
	pipe->wfx.SubFormat = info.BitsPerSample.IsFloating ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM;

	if (pipe->wfx.Format.nChannels == 8)
		pipe->wfx.dwChannelMask = KSAUDIO_SPEAKER_7POINT1;
	else if (pipe->wfx.Format.nChannels == 6)
		pipe->wfx.dwChannelMask = KSAUDIO_SPEAKER_5POINT1;
	else if (pipe->wfx.Format.nChannels == 4)
		pipe->wfx.dwChannelMask = KSAUDIO_SPEAKER_QUAD;
	else if (pipe->wfx.Format.nChannels == 2)
		pipe->wfx.dwChannelMask = KSAUDIO_SPEAKER_STEREO;
	else
		pipe->wfx.dwChannelMask = KSAUDIO_SPEAKER_DIRECTOUT;

	HRESULT hr=S_OK;
	
	hr = pXAudio2->CreateSourceVoice(&pipe->pSourceVoice, (LPCWAVEFORMATEX)&pipe->wfx, 0, 1.0f, pipe);
	if (FAILED(hr))
	{
		SAFE_DELETE(pipe->pSourceVoice); 
		SAFE_DELETE(pipe);
		return nullptr;
	}
	pSourceVoice->Start(0, 0);
	return pipe;
}

XAudio2Pipe::~XAudio2Pipe()
{
	CloseHandle(hBufferEndEvent); 
	DeleteCriticalSection(&cs);
}

bool XAudio2Pipe::InitEngine()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	UINT32 flags = 0;
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG) 
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif 
	HRESULT hr = XAudio2Create(&pXAudio2, flags);
	if (FAILED(hr))
	{
		CoUninitialize();
		return false;
	}
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG) 
	XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	pXAudio2->SetDebugConfiguration(&debug, 0);
#endif 

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
	{
		wprintf(L"Failed creating mastering voice: %#X\n", hr);
		SAFE_RELEASE(pXAudio2);
		CoUninitialize();
		return false;
	}


}