#include "Device_WaveOut.h"
using namespace System::Audio;

mutex AudioDevice_WaveOut::locker;
AudioDevice_WaveOut* AudioDevice_WaveOut::instance = nullptr;
const wchar_t* AudioDevice_WaveOut::_author = L"xworkteam powered by WaveOut";
const wchar_t* AudioDevice_WaveOut::_description = L"WaveOut≤•∑≈…Ë±∏";
const wchar_t* AudioDevice_WaveOut::_supports = L"";
CRITICAL_SECTION AudioDevice_WaveOut::cs = {};

AudioDevice_WaveOut::AudioDevice_WaveOut()
{
	InitializeCriticalSection(&cs);
}

AudioDevice_WaveOut::~AudioDevice_WaveOut()
{
	DeleteCriticalSection(&cs);
}

AudioDevice_WaveOut* AudioDevice_WaveOut::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			instance = new AudioDevice_WaveOut();
		}
	}
	return instance;
}

Version AudioDevice_WaveOut::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDevice_WaveOut::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDevice_WaveOut::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDevice_WaveOut::GetSupportFormat() const
{
	return _supports;
}

void CALLBACK AudioDevice_WaveOut::wave_callback(HWAVE hWave, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if (uMsg == WOM_DONE)
	{
		EnterCriticalSection(&cs);
		WOBuffer* buf = (WOBuffer*)dwInstance;
		buf->PlayedWaveHeaders[buf->PlayedWaveHeadersCount++] = (WAVEHDR*)dwParam1;
		buf->ScheduledBlocks--;
		LeaveCriticalSection(&cs);
	}
}


Result<AudioDevicePipe, AudioDeviceError> AudioDevice_WaveOut::Open(const uint128& owner, ulong bufferTimeMs, const SampleInfo& Info)
{
	Result<AudioDevicePipe, AudioDeviceError> result(nullptr, AudioDeviceError::Failed);

	if (!waveOutGetNumDevs())
	{
		result.WriteMessage(L"no audio device found using waveout");
		return result;
	}

	WOBuffer* Buffer = new WOBuffer();
	Buffer->Info = Info;
	Buffer->Mutex = CreateMutex(NULL, FALSE, NULL);
	Buffer->DestroyDEV = false;
	Buffer->ID = owner;
	memset(&Buffer->WFE, 0, sizeof(WAVEFORMATEXTENSIBLE));
	Buffer->WFE.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	Buffer->WFE.Format.nChannels = Info.Channels;
	Buffer->WFE.Format.nSamplesPerSec = Info.SampleRate;
	Buffer->WFE.Format.nAvgBytesPerSec = Info.AvgBytesPerSec;
	Buffer->WFE.Format.wBitsPerSample = Info.BitsPerSample.BPS;
	Buffer->WFE.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	Buffer->WFE.Samples.wValidBitsPerSample = Buffer->WFE.Format.wBitsPerSample;
	Buffer->WFE.Format.nBlockAlign = Info.BlockAlign;
	Buffer->WFE.SubFormat = Info.BitsPerSample.IsFloating ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM;

	if (Buffer->WFE.Format.nChannels == 8)
		Buffer->WFE.dwChannelMask = KSAUDIO_SPEAKER_7POINT1;
	else if (Buffer->WFE.Format.nChannels == 6)
		Buffer->WFE.dwChannelMask = KSAUDIO_SPEAKER_5POINT1;
	else if (Buffer->WFE.Format.nChannels == 4)
		Buffer->WFE.dwChannelMask = KSAUDIO_SPEAKER_QUAD;
	else if (Buffer->WFE.Format.nChannels == 2)
		Buffer->WFE.dwChannelMask = KSAUDIO_SPEAKER_STEREO;
	else
		Buffer->WFE.dwChannelMask = KSAUDIO_SPEAKER_DIRECTOUT;

	Buffer->SampleTime = bufferTimeMs;
	Buffer->BufferBytes = (ulong)((double)bufferTimeMs / 1000.0*(double)Buffer->WFE.Format.nAvgBytesPerSec);

	UINT deviceID = WAVE_MAPPER;
	switch (waveOutOpen(&Buffer->dev, deviceID, &Buffer->WFE.Format, (DWORD)wave_callback, (DWORD_PTR)Buffer, CALLBACK_FUNCTION))
	{
	case MMSYSERR_ALLOCATED:
	{
		result.WriteMessage(L"waveout device is already opened");
		SAFE_DELETE(Buffer);
	}
	return result;
	case MMSYSERR_BADDEVICEID:
	{
		result.WriteMessage(L"the specified device is out of range");
		SAFE_DELETE(Buffer);
	}
	return result;
	case MMSYSERR_NODRIVER:
	{
		result.WriteMessage(L"no audio driver in OS");
		SAFE_DELETE(Buffer);
	}
	return result;
	case MMSYSERR_NOMEM:
	{
		result.WriteMessage(L"unable to allocate sound memory");
		SAFE_DELETE(Buffer);
	}
	return result;
	case WAVERR_BADFORMAT:
	{
		result.WriteMessage(L"unsupported audio format");
		SAFE_DELETE(Buffer);
	}
	return result;
	case WAVERR_SYNC:
	{
		result.WriteMessage(L"the device is synchronous");
		SAFE_DELETE(Buffer);
	}
	return result;
	case MMSYSERR_NOERROR:
		break;
	default:
	{
		result.WriteMessage(L"unknown error");
		SAFE_DELETE(Buffer);
	}
	return result;
	}
	waveOutReset(Buffer->dev);

	Buffer->Vol = 1.0;
	Buffer->Pan = 0.0;
	SetVolume(Buffer, 1.0);
	SetPan(Buffer, 0.0);
	Buffer->OutputWrittenPointer = 0;
	Buffer->WriteOffset = 0;
	Buffer->WaitEndFlag = 0;
	Buffer->WritePlay = false;
	if (MulDiv(Buffer->BufferBytes, 1000, Buffer->WFE.Format.nAvgBytesPerSec)>0)
	{
		EnterCriticalSection(&cs);
		Pipes.push_back(Buffer);
		LeaveCriticalSection(&cs);
		result.Value = Buffer;
		result.State = AudioDeviceError::OK;
		return result;
	}
	return result;
}

bool AudioDevice_WaveOut::GetIsMultiPipesSupported() const
{
	return true;
}

bool AudioDevice_WaveOut::GetIsCurrentExclusive() const
{
	return false;
}

unsigned int AudioDevice_WaveOut::GetNumberOfPipes()
{
	return Pipes.size();
}

SampleInfo AudioDevice_WaveOut::GetPipeInfo(const AudioDevicePipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		WOBuffer* Buffer = static_cast<WOBuffer*>(pipePtr);
		if (Buffer) return Buffer->Info;
	}
	catch (...){}
	return SampleInfo();
}

void AudioDevice_WaveOut::Free(WOBuffer* buffer)
{
	WAVEHDR*  wh;
	HGLOBAL   hg;

	EnterCriticalSection(&cs);
	wh = buffer->PlayedWaveHeaders[--buffer->PlayedWaveHeadersCount];
	LeaveCriticalSection(&cs);

	waveOutUnprepareHeader(buffer->dev, wh, sizeof(WAVEHDR));

	hg = GlobalHandle(wh->lpData);
	GlobalUnlock(hg);
	GlobalFree(hg);

	hg = GlobalHandle(wh);
	GlobalUnlock(hg);
	GlobalFree(hg);
}

void AudioDevice_WaveOut::Write(const AudioDevicePipe& pipe, const void* buffer, uint bytes)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return;
		if (Buffer->DestroyDEV) return;
		if (!buffer) return;
	}
	catch (...){ return; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return;
	}

	HGLOBAL    hg;
	HGLOBAL    hg2;
	LPWAVEHDR  wh;
	void*      allocptr;

	while (Buffer->PlayedWaveHeadersCount > 0) Free(Buffer);

	if (Buffer->ScheduledBlocks >= sizeof(Buffer->PlayedWaveHeaders) / sizeof(*Buffer->PlayedWaveHeaders)) // wait for a free block ...
	{
		Sleep(5000);
		ReleaseMutex(Buffer->Mutex);
		return;
	}

	if ((hg2 = GlobalAlloc(GMEM_MOVEABLE, bytes)) == NULL)   // allocate some memory for a copy of the buffer
	{
		ReleaseMutex(Buffer->Mutex);
		return;
	}

	allocptr = GlobalLock(hg2);
	CopyMemory(allocptr, buffer, bytes);

	if ((hg = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(WAVEHDR))) == NULL)
	{
		ReleaseMutex(Buffer->Mutex);
		return;
	}

	wh = (wavehdr_tag*)GlobalLock(hg);
	wh->dwBufferLength = bytes;
	wh->lpData = (CHAR *)allocptr;

	if (waveOutPrepareHeader(Buffer->dev, wh, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		GlobalUnlock(hg);
		GlobalFree(hg);
		ReleaseMutex(Buffer->Mutex);
		return;
	}

	if (waveOutWrite(Buffer->dev, wh, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		GlobalUnlock(hg);
		GlobalFree(hg);
		ReleaseMutex(Buffer->Mutex);
		return;
	}

	EnterCriticalSection(&cs);
	Buffer->ScheduledBlocks++;
	LeaveCriticalSection(&cs);
	Buffer->OutputWrittenPointer += bytes;

	ReleaseMutex(Buffer->Mutex);
}

int AudioDevice_WaveOut::CanWrite(const AudioDevicePipe& pipe)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return 0;
		if (Buffer->DestroyDEV) return 0;
	}
	catch (...){ return 0; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return 0;
	}
	DWORD PlayCursor;
	Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
	DWORD status;
	Buffer->dev->GetStatus(&status);
	if (status>DSBSTATUS_TERMINATED)
	{
		Buffer->DestroyDEV = true;
		RaiseEvent(CoreEventFlags::Error, CoreEvents::Crash, CoreObjects::None, CoreSubObjects::AudioDevice, Buffer->ID, L"current ds buffer crash", VariadicAny(_missing));
		ReleaseMutex(Buffer->Mutex);
		return 0;
	}
	ulong Space = Buffer->BufferBytes - (Buffer->WriteOffset - PlayCursor);
	if (Space > Buffer->BufferBytes)Space -= Buffer->BufferBytes;
	ReleaseMutex(Buffer->Mutex);
	if (Space < Buffer->WFE.Format.nBlockAlign)
		return 0;
	else
		return Space - Buffer->WFE.Format.nBlockAlign;
}

bool AudioDevice_WaveOut::WaitEnd(const AudioDevicePipe& pipe)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return false;
		if (Buffer->DestroyDEV) return false;
		if (!Buffer->dev) return false;
	}
	catch (...){ return false; }
	Buffer->WaitEndTime = 0;
	DWORD PlayCursor;
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
	ReleaseMutex(Buffer->Mutex);
	DWORD MaxWaitTime = (Buffer->BufferBytes - PlayCursor + Buffer->WriteOffset) / (double)Buffer->WFE.Format.nAvgBytesPerSec*1000.0;
	if (MaxWaitTime<0) MaxWaitTime = 0;
	if (PlayCursor>Buffer->WriteOffset)
	{
		ResetEvent(Buffer->NotifyEvent[0]);
		DWORD ret = WaitForSingleObject(Buffer->NotifyEvent[0], MaxWaitTime);//WaitForSingleObject(Buffer->NotifyEvent[0],INFINITE);
		if (ret == WAIT_OBJECT_0)
		{
			Buffer->WaitEndFlag = 1;
			WaitForSingleObject(Buffer->Mutex, INFINITE);
			Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
			ReleaseMutex(Buffer->Mutex);
			if (PlayCursor<Buffer->WriteOffset)
			{
				LPVOID lpvPtr1;
				DWORD dwBytes1;
				LPVOID lpvPtr2;
				DWORD dwBytes2;
				WaitForSingleObject(Buffer->Mutex, INFINITE);
				HRESULT res = Buffer->dev->Lock(Buffer->WriteOffset, Buffer->BufferBytes - Buffer->WriteOffset, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
				if (DSERR_BUFFERLOST == res)
				{
					Buffer->dev->Restore();
					res = Buffer->dev->Lock(Buffer->WriteOffset, Buffer->BufferBytes - Buffer->WriteOffset, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
				}
				if (SUCCEEDED(res))
				{
					memset(lpvPtr1, 0, dwBytes1);
					if (NULL != lpvPtr2)memset(lpvPtr2, 0, dwBytes2);
					res = Buffer->dev->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
				}
				Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
				ReleaseMutex(Buffer->Mutex);
				while (PlayCursor<Buffer->WriteOffset&&Buffer->WaitEndTime<Buffer->SampleTime)
				{
					Sleep(2);
					Buffer->WaitEndTime += 2;
					WaitForSingleObject(Buffer->Mutex, INFINITE);
					Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
					ReleaseMutex(Buffer->Mutex);
				}
			}
		}
	}
	else
	{
		Buffer->WaitEndFlag = 1;
		while (PlayCursor<Buffer->WriteOffset&&Buffer->WaitEndTime<Buffer->SampleTime)
		{
			Sleep(2);
			Buffer->WaitEndTime += 2;
			WaitForSingleObject(Buffer->Mutex, INFINITE);
			Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
			ReleaseMutex(Buffer->Mutex);
		}
	}
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	Buffer->dev->Stop();
	Buffer->dev->SetCurrentPosition(0);
	Buffer->WriteOffset = 0;
	Buffer->OutputWrittenPointer = 0;
	ReleaseMutex(Buffer->Mutex);
	return false;
}

bool AudioDevice_WaveOut::Play(const AudioDevicePipe& pipe)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return false;
		if (Buffer->DestroyDEV) return false;
		if (!Buffer->dev) return false;
	}
	catch (...){ return false; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return false;
	}
	bool temp = waveOutReset(Buffer->dev) == MMSYSERR_NOERROR;
	ReleaseMutex(Buffer->Mutex);
	return temp;
}

bool AudioDevice_WaveOut::Pause(const AudioDevicePipe& pipe)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return false;
		if (Buffer->DestroyDEV) return false;
		if (!Buffer->dev) return false;
	}
	catch (...){ return false; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return false;
	}
	bool temp = waveOutPause(Buffer->dev) == MMSYSERR_NOERROR;
	ReleaseMutex(Buffer->Mutex);
	return temp;
}

bool AudioDevice_WaveOut::Stop(const AudioDevicePipe& pipe)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return true;
		if (Buffer->DestroyDEV) return false;
		if (!Buffer->dev) return false;
	}
	catch (...){ return false; }

	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (Buffer->dev)
	{
		Buffer->dev->Stop();
		Buffer->WriteOffset = 0;
		if (Buffer->NotifyEvent[0] != NULL)
		{
			CloseHandle(Buffer->NotifyEvent[0]);
			Buffer->NotifyEvent[0] = NULL;
		}
		if (Buffer->DsNotify != NULL)
		{
			Buffer->DsNotify->Release();
			Buffer->DsNotify = NULL;
		}
		ReleaseMutex(Buffer->Mutex);
		CloseHandle(Buffer->Mutex);
		Buffer->Mutex = NULL;
		SAFE_RELEASE(Buffer->dev);
	}
	ZeroMemory(&Buffer->WFE, sizeof(Buffer->WFE));
	if (Buffer->DestroyDEV)SAFE_RELEASE(m_DS);
	if (Buffer)
	{
		EnterCriticalSection(&cs);
		std::vector<WOBuffer*>::iterator pend = std::remove_if(Pipes.begin(), Pipes.end(), std::bind1st(std::equal_to<WOBuffer*>(), Buffer));
		Pipes.erase(pend);
		delete Buffer;
		Buffer = NULL;
		LeaveCriticalSection(&cs);
	}
	return true;

}

void AudioDevice_WaveOut::Flush(const AudioDevicePipe& pipe, int timeMS)
{
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return;
		if (Buffer->DestroyDEV) return;
		if (!Buffer->dev) return;
	}
	catch (...){ return; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return;
	}
	while (Buffer->PlayedWaveHeadersCount > 0) Free(Buffer);
	waveOutReset(Buffer->dev);
	Buffer->WaitEndFlag = 0;
	if (timeMS >= 0)Buffer->OutputWrittenPointer = (double)timeMS / 1000.0*Buffer->WFE.Format.nAvgBytesPerSec;
	ReleaseMutex(Buffer->Mutex);
}

Result<bool, double> AudioDevice_WaveOut::SetVolume(const AudioDevicePipe& pipe, double volume)
{
	Result<bool, double> result(false, volume);
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDEV) return result;
		if (!Buffer->dev) return result;
	}
	catch (...){ return result; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return result;
	}

	volume = CLAMP<double>(volume, 0.0, 1.0);
	double v = 0;
	if (volume == 0)
		v = -10000;
	else
	{
		v = log10(volume*100.0) * 2500 - 5000;
		if (v<-10000)v = -10000;
		if (v>0)v = 0;
	}

	Buffer->Vol = volume;

	waveOutSetVolume(Buffer->dev, )
	ReleaseMutex(Buffer->Mutex);

	result.Value = true;
	result.State = volume;
	return result;
}

Result<bool, double> AudioDevice_WaveOut::SetPan(const AudioDevicePipe& pipe, double pan)
{
	Result<bool, double> result(false, pan);
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDEV) return result;
		if (!Buffer->dev) return result;
	}
	catch (...){ return result; }
	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return result;
	}
	pan = CLAMP<double>(pan, -1.0, 1.0);

	double p = 0;
	if (pan<0)
		p = -10000.0 - log10(100.0 - abs(pan)*99.0)*(-5000.0);
	else
		p = log10(100.0 - pan*99.0)*(-5000.0) + 10000.0;

	if (p<-10000)p = -10000;

	if (p>10000)p = 10000;

	Buffer->Pan = pan;

	Buffer->dev->SetPan((long)p);
	ReleaseMutex(Buffer->Mutex);
	result.Value = true;
	result.State = pan;
	return result;
}

Result<bool, double> AudioDevice_WaveOut::GetOutputTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDEV) return result;
		if (!Buffer->dev) return result;
	}
	catch (...){ return result; }

	WaitForSingleObject(Buffer->Mutex, INFINITE);
	if (!Buffer->dev)
	{
		ReleaseMutex(Buffer->Mutex);
		return result;
	}
	DWORD PlayCursor;
	Buffer->dev->GetCurrentPosition(&PlayCursor, NULL);
	if (Buffer->WaitEndFlag == 1)
	{
		if (PlayCursor>Buffer->WriteOffset) PlayCursor = Buffer->WriteOffset;
	}
	int m_WO = Buffer->WriteOffset<PlayCursor ? Buffer->WriteOffset + Buffer->BufferBytes : Buffer->WriteOffset;
	if (Buffer->OutputWrittenPointer == Buffer->WriteOffset)m_WO = Buffer->WriteOffset;
	result.Value = true;
	result.State = (double)((double)(Buffer->OutputWrittenPointer - (long)(m_WO - PlayCursor)) / (double)Buffer->WFE.Format.nAvgBytesPerSec*1000.0);
	ReleaseMutex(Buffer->Mutex);
	return result;

}

Result<bool, double> AudioDevice_WaveOut::GetWrittenTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDEV) return result;
		if (!Buffer->dev) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = (double)(Buffer->OutputWrittenPointer) / (double)Buffer->WFE.Format.nAvgBytesPerSec*1000.0;
	return result;
}

Result<bool, unsigned long> AudioDevice_WaveOut::GetWrittenPosition(const AudioDevicePipe& pipe)
{
	Result<bool, unsigned long> result(false, 0U);
	WOBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WOBuffer));
		Buffer = static_cast<WOBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDEV) return result;
		if (!Buffer->dev) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->OutputWrittenPointer;
	return result;
}

