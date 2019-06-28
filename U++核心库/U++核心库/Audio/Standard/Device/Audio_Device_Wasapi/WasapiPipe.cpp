#include "WasapiPipe.h"
#include <process.h>
using namespace System::Audio;

IMMDevice* WasapiPipe::pDevice = nullptr;
bool WasapiPipe::isinited = false;

WasapiPipe::WasapiPipe() : pRenderClient(nullptr), pAudioClient(false), volctr(nullptr), clock(nullptr)
{
	#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	hBufferEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
	#else 
	hBufferEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	#endif 
	Operation_RenderClient = CreateEvent(NULL, TRUE, FALSE, NULL);
}

WasapiPipe::~WasapiPipe()
{
	Stop();
	CloseHandle(Operation_RenderClient);
	CloseHandle(hBufferEvent);
	hBufferEvent = NULL;
}

WasapiPipe* WasapiPipe::Create(const System::Audio::SampleInfo& info, UInt64 bufferTimeMs, System::Audio::SampleInfo& actualinfo, UInt64& actualbufferTimeMs)
{
	WasapiPipe* pipe = new WasapiPipe();
	memset(&pipe->wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	pipe->wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	pipe->wfx.Format.nChannels = info.Channels;
	pipe->wfx.Format.nSamplesPerSec = info.SampleRate;
	pipe->wfx.Format.nAvgBytesPerSec = info.AvgBytesPerSec;
	pipe->wfx.Format.wBitsPerSample = info.BitsPerSample.BPS;
	pipe->wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	pipe->wfx.Samples.wValidBitsPerSample = pipe->wfx.Format.wBitsPerSample;
	pipe->wfx.Format.nBlockAlign = info.BlockAlign;
	pipe->wfx.SubFormat = info.BitsPerSample.IsFloating ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM;

	pipe->wfx.dwChannelMask = info.ChannelLayout.IsValid ? info.ChannelLayout : KSAUDIO_SPEAKER_DIRECTOUT;

	HRESULT hr=S_OK;
	
	hr = pDevice->Activate(
		__uuidof(IAudioClient), CLSCTX_ALL,
		NULL, (void**)&pipe->pAudioClient);
	if (FAILED(hr))
	{
		SAFE_DELETE(pipe);
		return nullptr;
	}

	WAVEFORMATEX* ewfx;

	hr = pipe->pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, &pipe->wfx.Format, &ewfx);

	if (hr == S_OK)
	{
		hr = pipe->pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			bufferTimeMs * 10000,
			0,
			&pipe->wfx.Format,
			NULL);
		if (FAILED(hr))
		{
			SAFE_RELEASE(pipe->pAudioClient);
			SAFE_DELETE(pipe);
			return nullptr;
		}
		actualinfo = info;
	}
	else if (hr==S_FALSE)
	{
		//if (ewfx->nChannels != pipe->wfx.Format.nChannels)
		//{
		//	actualinfo.ChannelLayout = ChannelLayout::Unknown;
		//}
		//else
		//{
			actualinfo.ChannelLayout = info.ChannelLayout;
		//}
		bool floating = false;
		if (ewfx->cbSize == sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) && ewfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
		{
			WAVEFORMATEXTENSIBLE* wwfx = (WAVEFORMATEXTENSIBLE*)ewfx;
			floating = IsEqualGUID(wwfx->SubFormat, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) == TRUE;
		}
		if (ewfx->wBitsPerSample != pipe->wfx.Format.wBitsPerSample)
		{
			actualinfo.BitsPerSample = BitsPerSample::GetOSBitsPerSample(ewfx->wBitsPerSample, true, floating);
		}
		else
		{
			if (floating)
			{
				switch (ewfx->wBitsPerSample)
				{
				case 32u:
					actualinfo.BitsPerSample = BitsPerSample::GetOSBitsPerSample(32u, true, true);
					break;
				case 64u:
					actualinfo.BitsPerSample = BitsPerSample::GetOSBitsPerSample(64u, true, true);
					break;
				}
			}
			else
				actualinfo.BitsPerSample =   info.BitsPerSample;
		}
		memcpy(&pipe->wfx.Format, ewfx, sizeof(WAVEFORMATEX) + ewfx->cbSize);
		pipe->wfx.Samples.wValidBitsPerSample = pipe->wfx.Format.wBitsPerSample;

		actualinfo.SampleRate = pipe->wfx.Format.nSamplesPerSec;


		hr = pipe->pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			bufferTimeMs * 10000,
			0,
			&pipe->wfx.Format,
			NULL);
		if (FAILED(hr))
		{
			SAFE_RELEASE(pipe->pAudioClient);
			SAFE_DELETE(pipe);
			return nullptr;
		}
	}
	else
	{
		SAFE_RELEASE(pipe->pAudioClient);
		SAFE_DELETE(pipe);
		return nullptr;
	}

	hr = pipe->pAudioClient->GetBufferSize(&pipe->framebuffercount);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pipe->pAudioClient);
		SAFE_DELETE(pipe);
		return nullptr;
	}
	
	actualbufferTimeMs = (double)pipe->framebuffercount *1000.0 / (double)pipe->wfx.Format.nSamplesPerSec;


	pipe->KillFillThread = false;
	pipe->FillThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &FillThread, pipe, CREATE_SUSPENDED, 0);
	SetThreadPriority(pipe->FillThreadHandle, THREAD_PRIORITY_HIGHEST);
	ResumeThread(pipe->FillThreadHandle);
	if (WaitForSingleObject(pipe->Operation_RenderClient, INFINITE) == WAIT_OBJECT_0)
	{
		if (pipe->pRenderClient && pipe->volctr && pipe->clock)
		{
			pipe->sinfo = actualinfo;
			pipe->oinfo = info;
			pipe->vol = 1.0;
			pipe->pan = 0.0;
			pipe->written = 0u;
			return pipe;
		}
	}
	SAFE_RELEASE(pipe->pAudioClient);
	SAFE_DELETE(pipe);
	return nullptr;
}

unsigned int _stdcall WasapiPipe::FillThread(void* Obj)
{
	WasapiPipe* pipe = (WasapiPipe*)Obj;

	HRESULT hr = pipe->pAudioClient->GetService(
		__uuidof(IAudioRenderClient),
		(void**)& pipe->pRenderClient);
	if (FAILED(hr))
	{
		pipe->pRenderClient = nullptr;
	}
	else
	{
		hr = pipe->pAudioClient->GetService(__uuidof(IAudioStreamVolume), (void**)&pipe->volctr);
		if (FAILED(hr))
		{
			pipe->volctr = nullptr;
		}
		else
		{
			hr = pipe->pAudioClient->GetService(__uuidof(IAudioClock), (void**)&pipe->clock);
			if (FAILED(hr))
			{
				pipe->clock = nullptr;
			}
		}
	}
	
	SetEvent(pipe->Operation_RenderClient);
	while (!pipe->KillFillThread)
	{
		Sleep(20);
	}

	SAFE_RELEASE(pipe->volctr);
	SAFE_RELEASE(pipe->clock);
	SAFE_RELEASE(pipe->pRenderClient);
	_endthreadex(0);
	return 0;
}


UInt64 WasapiPipe::Write(const BYTE* buf, UInt64 bytes, bool& result)
{
	if (!buf || !bytes)
	{
		result = false;
		return 0ull;
	}
	UINT32 padding = 0u;
	if (pAudioClient->GetCurrentPadding(&padding) == S_OK)
	{
		UINT32 available = framebuffercount - padding;
		UINT32 abytes = available*sinfo.Channels* sinfo.BitsPerSample.BPS >> 3;
		UINT32 bcount = bytes / (sinfo.Channels* sinfo.BitsPerSample.BPS >> 3);
		if (abytes >= bytes)
		{
			BYTE *pData;
			if (pRenderClient->GetBuffer(bcount, &pData) == S_OK)
			{
				memcpy(pData, buf, bytes);
				pRenderClient->ReleaseBuffer(bcount, 0);
				written += bytes;
			}
			result = true;
			return bytes;
		}
		else
		{
			BYTE *pData;
			if (pRenderClient->GetBuffer(available, &pData) == S_OK)
			{
				memcpy(pData, buf, abytes);
				pRenderClient->ReleaseBuffer(available, 0);
				written += available;
			}
			result = true;
			return abytes;
		}
	}
	result = false;
	return 0ull;
}

int WasapiPipe::CanWrite()
{
	UINT32 padding = 0u;
	if (pAudioClient->GetCurrentPadding(&padding) == S_OK)
	{
		return framebuffercount - padding;
	}
	return 0;
}

bool WasapiPipe::Play()
{
	return pAudioClient != nullptr ? pAudioClient->Start() == S_OK : false;
}

bool WasapiPipe::Pause()
{
	return pAudioClient != nullptr ? pAudioClient->Stop() == S_OK : false;
}

bool WasapiPipe::Stop()
{
	if (pAudioClient == nullptr) return false;
	HRESULT hr = pAudioClient->Stop();
	if (hr == S_OK || hr == S_FALSE)
	{
		hr = pAudioClient->Reset();
		if (hr == S_OK || hr == S_FALSE)
		{
			written = 0u;
			KillFillThread = true;

			if (WaitForSingleObject(FillThreadHandle, INFINITE) == WAIT_TIMEOUT)
			{
				TerminateThread(FillThreadHandle, 0);
			}
			if (!CloseHandle(FillThreadHandle))
				TerminateThread(FillThreadHandle, 0);
			FillThreadHandle = INVALID_HANDLE_VALUE;

			SAFE_RELEASE(pAudioClient);

			return true;
		}
	}
	return false;
}

bool WasapiPipe::Flush(UInt64 timeMS)
{
	if (pAudioClient == nullptr) return false;

	pAudioClient->Stop();
	if (SUCCEEDED(pAudioClient->Reset()))
	{
		written = (double)timeMS / 1000.0 * wfx.Format.nAvgBytesPerSec;
		pAudioClient->Start();
	}
	return false;
}

double WasapiPipe::SetPan(double v)
{
	double lastpan = pan;
	if (pAudioClient == nullptr)
	{
		pan = CLAMP<double>(v, -1.0, 1.0);
		return pan;
	}
	float vols[2] = { vol*((pan >= 0) ? (1.f - pan) : 1.f), vol*((pan <= 0) ? (-pan - 1.f) : 1.f) };
	if (volctr)
	{
		if (volctr->SetAllVolumes(2, vols) != S_OK)
		{
			pan = lastpan;
		}
	}
	else
	{
		pan = lastpan;
	}
	return pan;
}

double WasapiPipe::SetVolume(double v)
{
	double lastvol = vol;
	if (pAudioClient == nullptr)
	{
		vol = CLAMP<double>(v, 0.0, 1.0);
		return vol;
	}
	float vols[2] = { vol*((pan >= 0) ? (1.f - pan) : 1.f), vol*((pan <= 0) ? (-pan - 1.f) : 1.f) };
	if (volctr)
	{
		if (volctr->SetAllVolumes(2, vols) != S_OK)
		{
			vol = lastvol;
		}
	}
	else
		vol = lastvol;
	return vol;
}

double WasapiPipe::GetOutputTime()
{
	if (pAudioClient == nullptr) return 0.0;

	UINT64 pos = 0ull;
	if (clock)
	{
		if (clock->GetPosition(&pos, NULL) == S_OK)
		{
			UINT64 freq = 0ull;
			if (clock->GetFrequency(&freq) == S_OK)
			{
				double tpos = (double)pos / (double)freq;
				return tpos - (double)(written) / (double)wfx.Format.nAvgBytesPerSec*1000.0;
			}
		}
	}
	return 0.0;
}

double WasapiPipe::GetWrittenTime()
{
	return (double)(written) / (double)wfx.Format.nAvgBytesPerSec*1000.0;
}

UInt64 WasapiPipe::GetWrittenPosition()
{
	return written;
}

bool WasapiPipe::WaitEnd()
{
	if (pAudioClient == nullptr) return false;
	while (pAudioClient!=nullptr)
	{	
		UINT32 padding = 0u;
		if(pAudioClient->GetCurrentPadding(&padding)==S_OK)
		{
			if (padding == 0)
			{
				break;
			}
		}
		Sleep(2);
	}
	return false;
}

Result<bool> WasapiPipe::InitEngine()
{
	Result<bool> result(false);
	if (isinited)
	{
		result.Value = true;
		return result;
	}
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	IMMDeviceEnumerator *pEnumerator = NULL;
	HRESULT hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pEnumerator);
		CoUninitialize();
		result.WriteMessage(L"can not query device enumerator");
		return result;
	}
	hr = pEnumerator->GetDefaultAudioEndpoint(
		eRender, eConsole, &pDevice);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pEnumerator);
		CoUninitialize();
		result.WriteMessage(L"can not get default wasapi device");
		return result;
	}
	SAFE_RELEASE(pEnumerator);
	result.Value = true;
	return result;
}

void WasapiPipe::ExitEngine()
{
	SAFE_RELEASE(pDevice);
	CoUninitialize();
	isinited = false;
}