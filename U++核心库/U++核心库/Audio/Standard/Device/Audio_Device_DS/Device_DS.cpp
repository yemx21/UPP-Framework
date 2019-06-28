#include "Device_DS.h"
#include "DS.h"
#include "WindowsEnvironment.h"
using namespace System;
using namespace System::OS;
using namespace System::Audio;

std::mutex AudioDevice_DS::locker;
AudioDevice_DS* AudioDevice_DS::instance = nullptr;
const wchar_t* AudioDevice_DS::_author = L"powered by DirectSound";
const wchar_t* AudioDevice_DS::_description = L"DirectSound播放设备";
const wchar_t* AudioDevice_DS::_supports = L"";


AudioDevice_DS::AudioDevice_DS() :m_DS(nullptr)
{
	
}

AudioDevice_DS::~AudioDevice_DS()
{
	Microsoft_DS::dsapi_ReleaseDirectSound(m_DS);
}

AudioDevice_DS* AudioDevice_DS::Create()
{
	if (!instance)
	{
		std::lock_guard<std::mutex> lock(locker);
		if (!instance)
		{
			instance = new AudioDevice_DS();
		}
	}
	return instance;
}

Version AudioDevice_DS::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDevice_DS::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDevice_DS::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDevice_DS::GetSupportFormat() const
{
	return _supports;
}

__forceinline void FreeDSBuffer(DSBuffer*& buf)
{
	if (!buf) return;
	if (buf->DSB)
	{
		Microsoft_DS::dsapi_Stop(buf->DSB);
	}
	if (buf->NotifyEvent != NULL)
	{
		Microsoft::winapi_CloseHandle(buf->NotifyEvent);
		buf->NotifyEvent = NULL;
	}
	if (buf->DsNotify != NULL)
	{
		Microsoft_DS::dsapi_ReleaseNotify(buf->DsNotify);
	}
	if (buf->Mutex)
	{
		Microsoft::winapi_ReleaseMutex(buf->Mutex);
		Microsoft::winapi_CloseHandle(buf->Mutex);
		buf->Mutex = NULL;
	}
	if (buf->DSB)
	{
		Microsoft_DS::dsapi_ReleaseBuffer(buf->DSB);
	}
	SAFE_DELETE(buf);
}

Result<AudioDevicePipe, AudioDeviceError> AudioDevice_DS::Open(const UInt128& owner, UInt64 bufferTimeMs, const SampleInfo& Info, SampleInfo& actualInfo, UInt64& actualBufferMs)
{
	if (bufferTimeMs < 50u) bufferTimeMs = 50u;
	Result<AudioDevicePipe, AudioDeviceError> result(nullptr, AudioDeviceError::Failed);
	if (!m_DS)
	{
		m_DS = Microsoft_DS::dsapi_CreateDirectSound(NULL);
		if (!m_DS)
		{
			result.State = AudioDeviceError::DeviceError;
			return result;
		}
		if (!Microsoft_DS::dsapi_SetCooperativeLevel(m_DS, *GetHost(), DS_DSSCL_PRIORITY))
		{
			result.State = AudioDeviceError::Failed;
			result.WriteMessage(L"Error directsound priority");
			return result;
		}
	}

	DSBuffer* Buffer = new DSBuffer();
	Buffer->Info = Info;
	Buffer->Mutex = Microsoft::winapi_CreateMutex(NULL, false, NULL);
	Buffer->DestroyDS = false;
	Buffer->ID = owner;

	DS_WAVEFORMATEXTENSIBLE wfe;

	memset(&wfe, 0, sizeof(DS_WAVEFORMATEXTENSIBLE));
	wfe.Format.wFormatTag = DS_WAVE_FORMAT_EXTENSIBLE;
	wfe.Format.nChannels = Buffer->Info.Channels;
	wfe.Format.nSamplesPerSec = Buffer->Info.SampleRate;
	wfe.Format.nAvgBytesPerSec = Buffer->Info.AvgBytesPerSec;
	wfe.Format.wBitsPerSample = Buffer->Info.BitsPerSample.BPS;
	wfe.Format.cbSize = sizeof(DS_WAVEFORMATEXTENSIBLE) - sizeof(DS_WAVEFORMATEX);
	wfe.Samples.wValidBitsPerSample = wfe.Format.wBitsPerSample;
	wfe.Format.nBlockAlign = Buffer->Info.BlockAlign;
	wfe.SubFormat = Microsoft_DS::GetSubFormat(Buffer->Info.BitsPerSample.IsFloating);
	wfe.dwChannelMask = Microsoft_DS::GetChannelMask(wfe.Format.nChannels);

	Buffer->SampleTime = bufferTimeMs;
	Buffer->BufferBytes = (size_t)((double)bufferTimeMs / 1000.0*(double)wfe.Format.nAvgBytesPerSec);

	void* PDSB = Microsoft_DS::dsapi_CreatePrimaryBuffer(m_DS, wfe);
	if (!PDSB)
	{
		result.State = AudioDeviceError::Failed;
		FreeDSBuffer(Buffer);
		return result;
	}
	Microsoft_DS::dsapi_ReleaseBuffer(PDSB);

	Buffer->DSB = Microsoft_DS::dsapi_CreateSecondaryBuffer(m_DS, Buffer->BufferBytes, wfe);
	if (!Buffer->DSB)
	{
		result.WriteMessage(L"can not create sound buffer");
		FreeDSBuffer(Buffer);
		return result;
	}

	Buffer->NotifyEvent = Microsoft::winapi_CreateEvent(NULL, false, false, NULL);
	Buffer->DsNotify = Microsoft_DS::dsapi_QueryNotify(Buffer->DSB);
	if (!Buffer->DsNotify)
	{
		result.WriteMessage(L"can not query notify interface");
		FreeDSBuffer(Buffer);
		return result;
	}

	if (!Microsoft_DS::dsapi_SetNotificationPositions(Buffer->DsNotify, Buffer->BufferBytes - 1, Buffer->NotifyEvent))
	{
		result.WriteMessage(L"can not set notification position");
		FreeDSBuffer(Buffer);
		return result;
	}

	Buffer->Vol = 1.0;
	Buffer->Pan = 0.0;
	SetVolume(Buffer, 1.0);
	SetPan(Buffer, 0.0);
	Buffer->OutputWrittenPointer = 0;
	Buffer->WriteOffset = 0;
	Buffer->WaitEndFlag = 0;
	Buffer->WritePlay = false;
	
	cs.Enter();
	Pipes.push_back(Buffer);
	cs.Leave();
	result.Value = Buffer;
	result.State = AudioDeviceError::OK;
	actualInfo = Buffer->Info;
	actualBufferMs = bufferTimeMs;
	return result;
}

bool AudioDevice_DS::GetIsMultiPipesSupported() const
{
	return true;
}

bool AudioDevice_DS::GetIsCurrentExclusive() const
{
	return false;
}

UInt64 AudioDevice_DS::GetNumberOfPipes()
{
	return Pipes.size();
}

SampleInfo AudioDevice_DS::GetPipeInfo(const AudioDevicePipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		DSBuffer* Buffer = static_cast<DSBuffer*>(pipePtr);
		if (Buffer) return Buffer->Info;
	}
	catch (...){}
	return SampleInfo();
}

Result<bool, UInt64> AudioDevice_DS::Write(const AudioDevicePipe& pipe, const void* buffer, UInt64 bytes)
{
	Result<bool, UInt64> result(false, 0ull);
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		 Buffer = static_cast<DSBuffer*>(pipePtr);
		 if (!Buffer) return result;
		 if (Buffer->DestroyDS) return result;
		 if (!buffer) return result;
	}
	catch (...){ return result; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return result;
	}

	void* lpvPtr1 = nullptr;
	unsigned int dwBytes1 = 0u;
	void* lpvPtr2 = nullptr;
	unsigned int dwBytes2 = 0u;
	unsigned int status = 0u;
	Microsoft_DS::dsapi_GetStatus(Buffer->DSB, status);
	if (status > DS_DSBSTATUS_TERMINATED)
	{
		Buffer->DestroyDS = true;
		RaiseEvent(CoreEventFlags::Error, CoreEvents::Crash, CoreObjects::None, CoreSubObjects::AudioDevice, Buffer->ID, L"current ds buffer crash", VariadicAny(_missing));
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return result;
	}

	long res = Microsoft_DS::dsapi_BufferLock(Buffer->DSB, Buffer->WriteOffset, bytes, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2, 0);
	if (DS_DSERR_BUFFERLOST == res)
	{
		if (!Microsoft_DS::dsapi_Restore(Buffer->DSB)) ClearBuffer(Buffer);
		res = Microsoft_DS::dsapi_BufferLock(Buffer->DSB, Buffer->WriteOffset, bytes, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2, 0);
	}

	if (res >= 0)
	{
		memcpy(lpvPtr1, buffer, dwBytes1);
		if (NULL != lpvPtr2) memcpy(lpvPtr2, (char*)buffer + dwBytes1, dwBytes2);
		Buffer->WriteOffset += (dwBytes1 + dwBytes2);
		if (Buffer->WriteOffset >= Buffer->BufferBytes) Buffer->WriteOffset = dwBytes2;
		res = Microsoft_DS::dsapi_BufferUnlock(Buffer->DSB, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
		if (res >= 0)
		{
			Buffer->OutputWrittenPointer += (dwBytes1 + dwBytes2);
			if (Buffer->WritePlay)
			{
				Buffer->WritePlay = false;
				Microsoft_DS::dsapi_Play(Buffer->DSB, 0, 0, DS_DSBPLAY_LOOPING);
			}
			Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
			result.Value = true;
			result.State = dwBytes1 + dwBytes2;
		}
	}
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	return result;
}

UInt64 AudioDevice_DS::CanWrite(const AudioDevicePipe& pipe)
{
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return 0;
		if (Buffer->DestroyDS) return 0;
	}
	catch (...){ return 0; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return 0;
	}
	unsigned int PlayCursor = 0u;
	unsigned int WriteCursor = 0u;
	unsigned int status = 0u;
	Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
	
	Microsoft_DS::dsapi_GetStatus(Buffer->DSB, status);
	if (status > DS_DSBSTATUS_TERMINATED)
	{
		Buffer->DestroyDS = true;
		RaiseEvent(CoreEventFlags::Error, CoreEvents::Crash, CoreObjects::None, CoreSubObjects::AudioDevice, Buffer->ID, L"current ds buffer crash", VariadicAny(_missing));
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return 0;
	}
	UInt64 Space = Buffer->BufferBytes - (Buffer->WriteOffset - PlayCursor);
	if (Space > Buffer->BufferBytes)Space -= Buffer->BufferBytes;
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	if (Space < Buffer->Info.BlockAlign)
		return 0;
	else
		return Space - Buffer->Info.BlockAlign;
}

bool AudioDevice_DS::Drain(const AudioDevicePipe& pipe)
{
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return false;
		if (Buffer->DestroyDS) return false;	
		if (!Buffer->DSB)return false;
	}
	catch (...){ return false; }
	Buffer->WaitEndTime = 0;
	unsigned int PlayCursor;
	unsigned int WriteCursor;
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	auto MaxWaitTime = (Buffer->BufferBytes - PlayCursor + Buffer->WriteOffset) / (double)Buffer->Info.AvgBytesPerSec*1000.0;
	if (MaxWaitTime<0) MaxWaitTime = 0;
	if (PlayCursor>Buffer->WriteOffset)
	{
		Microsoft::winapi_ResetEvent(Buffer->NotifyEvent);
		auto ret = Microsoft::winapi_WaitForSingleObject(Buffer->NotifyEvent, MaxWaitTime);//WaitForSingleObject(Buffer->NotifyEvent[0],INFINITE);
		if (ret == API_WAIT_OBJECT_0)
		{
			Buffer->WaitEndFlag = 1;
			Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
			Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
			Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
			if (PlayCursor<Buffer->WriteOffset)
			{
				void* lpvPtr1 = nullptr;
				unsigned int dwBytes1 = 0u;
				void* lpvPtr2 = nullptr;
				unsigned int dwBytes2 = 0u;
				Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
				auto res = Microsoft_DS::dsapi_BufferLock(Buffer->DSB, Buffer->WriteOffset, Buffer->BufferBytes - Buffer->WriteOffset, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2, 0);
				if (DS_DSERR_BUFFERLOST == res)
				{
					Microsoft_DS::dsapi_Restore(Buffer->DSB);
					res = Microsoft_DS::dsapi_BufferLock(Buffer->DSB, Buffer->WriteOffset, Buffer->BufferBytes - Buffer->WriteOffset, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2, 0);
				}
				if (res >= 0)
				{
					memset(lpvPtr1, 0, dwBytes1);
					if (NULL != lpvPtr2) memset(lpvPtr2, 0, dwBytes2);
					res = Microsoft_DS::dsapi_BufferUnlock(Buffer->DSB, lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
				}
				Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
				Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
				while (PlayCursor<Buffer->WriteOffset&&Buffer->WaitEndTime<Buffer->SampleTime)
				{
					Microsoft::winapi_Sleep(2);
					Buffer->WaitEndTime += 2;
					Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
					Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
					Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
				}
			}
		}
	}
	else
	{
		Buffer->WaitEndFlag = 1;
		while (PlayCursor<Buffer->WriteOffset&&Buffer->WaitEndTime<Buffer->SampleTime)
		{
			Microsoft::winapi_Sleep(2);
			Buffer->WaitEndTime += 2;
			Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
			Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
			Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		}
	}
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	Microsoft_DS::dsapi_Stop(Buffer->DSB);
	Microsoft_DS::dsapi_SetCurrentPosition(Buffer->DSB, 0);
	Buffer->WriteOffset = 0;
	Buffer->OutputWrittenPointer = 0;
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	return false;
}

bool AudioDevice_DS::Play(const AudioDevicePipe& pipe)
{
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return false;
		if (Buffer->DestroyDS) return false;
		if (!Buffer->DSB) return false;
	}
	catch (...){ return false; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return false;
	}
	bool temp = Microsoft_DS::dsapi_Play(Buffer->DSB, 0, 0, DS_DSBPLAY_LOOPING);
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	return temp;
}

bool AudioDevice_DS::Pause(const AudioDevicePipe& pipe)
{
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return false;
		if (Buffer->DestroyDS) return false;
		if (!Buffer->DSB) return false;
	}
	catch (...){ return false; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return false;
	}
	bool temp = Microsoft_DS::dsapi_Stop(Buffer->DSB);
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	return temp;
}

bool AudioDevice_DS::Stop(const AudioDevicePipe& pipe)
{
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return true;
		if (Buffer->DestroyDS) return true;
		if (!Buffer->DSB) return true;
	}
	catch (...){ return false; }

	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (Buffer->DSB)
	{
		Microsoft_DS::dsapi_Stop(Buffer->DSB);
		Buffer->WriteOffset = 0;
		if (Buffer->NotifyEvent != NULL)
		{
			Microsoft::winapi_CloseHandle(Buffer->NotifyEvent);
			Buffer->NotifyEvent = NULL;
		}
		if (Buffer->DsNotify != NULL)
		{
			Microsoft_DS::dsapi_ReleaseNotify(Buffer->DsNotify);
		}
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		Microsoft::winapi_CloseHandle(Buffer->Mutex);
		Buffer->Mutex = NULL;
		Microsoft_DS::dsapi_ReleaseBuffer(Buffer->DSB);
	}

	if (Buffer->DestroyDS) Microsoft_DS::dsapi_ReleaseDirectSound(m_DS);
	if (Buffer)
	{
		cs.Enter();
		std::vector<DSBuffer*>::iterator pend = std::remove_if(Pipes.begin(), Pipes.end(), [&Buffer](DSBuffer* x) {return x == Buffer; });
		Pipes.erase(pend);
		delete Buffer;
		Buffer = NULL;
		cs.Leave();
	}
	return true;

}

void AudioDevice_DS::Flush(const AudioDevicePipe& pipe, UInt64 timeMS)
{
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return;
		if (Buffer->DestroyDS) return;
		if (!Buffer->DSB) return;
	}
	catch (...){ return; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return;
	}
	ClearBuffer(Buffer);
	if (timeMS >= 0) Buffer->OutputWrittenPointer = (double)timeMS / 1000.0*Buffer->Info.AvgBytesPerSec;
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
}

Result<bool, double> AudioDevice_DS::SetVolume(const AudioDevicePipe& pipe, double volume)
{
	Result<bool, double> result(false, volume);
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDS) return result;
		if (!Buffer->DSB) return result;
	}
	catch (...){ return result; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
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

	Microsoft_DS::dsapi_SetVolume(Buffer->DSB, (int)Buffer->Vol);
	
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	
	result.Value = true;
	result.State = volume;
	return result;
}

Result<bool, double> AudioDevice_DS::SetPan(const AudioDevicePipe& pipe, double pan)
{
	Result<bool, double> result(false, pan);
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDS) return result;
		if (!Buffer->DSB) return result;
	}
	catch (...){ return result; }
	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
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

	Microsoft_DS::dsapi_SetPan(Buffer->DSB, (int)Buffer->Pan);
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	result.Value = true; 
	result.State = pan;
	return result;
}

Result<bool, double> AudioDevice_DS::GetOutputTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDS) return result;
		if (!Buffer->DSB) return result;
	}
	catch (...){ return result; }

	Microsoft::winapi_WaitForSingleObject(Buffer->Mutex, API_INFINITE);
	if (!Buffer->DSB)
	{
		Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
		return result;
	}
	unsigned int PlayCursor;
	unsigned int WriteCursor;
	Microsoft_DS::dsapi_GetCurrentPosition(Buffer->DSB, PlayCursor, WriteCursor);
	if (Buffer->WaitEndFlag == 1)
	{
		if (PlayCursor>Buffer->WriteOffset) PlayCursor = Buffer->WriteOffset;
	}
	int m_WO = Buffer->WriteOffset<PlayCursor ? Buffer->WriteOffset + Buffer->BufferBytes : Buffer->WriteOffset;
	if (Buffer->OutputWrittenPointer == Buffer->WriteOffset)m_WO = Buffer->WriteOffset;
	result.Value = true;
	result.State = (double)((double)(Buffer->OutputWrittenPointer - (long)(m_WO - PlayCursor)) / (double)Buffer->Info.AvgBytesPerSec*1000.0);
	Microsoft::winapi_ReleaseMutex(Buffer->Mutex);
	return result;

}

Result<bool, double> AudioDevice_DS::GetWrittenTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDS) return result;
		if (!Buffer->DSB) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = (double)(Buffer->OutputWrittenPointer) / (double)Buffer->Info.AvgBytesPerSec*1000.0;
	return result;
}

Result<bool, UInt64> AudioDevice_DS::GetWrittenPosition(const AudioDevicePipe& pipe)
{
	Result<bool, UInt64> result(false, 0ull);
	DSBuffer* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(DSBuffer));
		Buffer = static_cast<DSBuffer*>(pipePtr);
		if (!Buffer) return result;
		if (Buffer->DestroyDS) return result;
		if (!Buffer->DSB) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State= Buffer->OutputWrittenPointer;
	return result;
}

bool AudioDevice_DS::ClearBuffer(DSBuffer* Buffer)
{
	if (!Buffer)return false;
	if (!Buffer->DSB)return false;
	///清空音频数据
	unsigned int status = 0u;

	Buffer->WritePlay = false;
	if (Microsoft_DS::dsapi_GetStatus(Buffer->DSB, status))
		if ((status & DS_DSBPLAY_LOOPING) != 0)
			if (Microsoft_DS::dsapi_Stop(Buffer->DSB))
				Buffer->WritePlay = true;

	Microsoft_DS::dsapi_ReleaseBuffer(Buffer->DSB);
	Microsoft_DS::dsapi_ReleaseNotify(Buffer->DsNotify);

	DS_WAVEFORMATEXTENSIBLE wfe;

	memset(&wfe, 0, sizeof(DS_WAVEFORMATEXTENSIBLE));
	wfe.Format.wFormatTag = DS_WAVE_FORMAT_EXTENSIBLE;
	wfe.Format.nChannels = Buffer->Info.Channels;
	wfe.Format.nSamplesPerSec = Buffer->Info.SampleRate;
	wfe.Format.nAvgBytesPerSec = Buffer->Info.AvgBytesPerSec;
	wfe.Format.wBitsPerSample = Buffer->Info.BitsPerSample.BPS;
	wfe.Format.cbSize = sizeof(DS_WAVEFORMATEXTENSIBLE) - sizeof(DS_WAVEFORMATEX);
	wfe.Samples.wValidBitsPerSample = wfe.Format.wBitsPerSample;
	wfe.Format.nBlockAlign = Buffer->Info.BlockAlign;
	wfe.SubFormat = Microsoft_DS::GetSubFormat(Buffer->Info.BitsPerSample.IsFloating);
	wfe.dwChannelMask = Microsoft_DS::GetChannelMask(wfe.Format.nChannels);

	void* PDSB = Microsoft_DS::dsapi_CreatePrimaryBuffer(m_DS, wfe);
	if (!PDSB)
	{
		FreeDSBuffer(Buffer);
		goto Error;
	}
	Microsoft_DS::dsapi_ReleaseBuffer(PDSB);

	Buffer->DSB = Microsoft_DS::dsapi_CreateSecondaryBuffer(m_DS, Buffer->BufferBytes, wfe);
	if (!Buffer->DSB)
	{
		FreeDSBuffer(Buffer);
		goto Error;
	}

	Buffer->NotifyEvent = Microsoft::winapi_CreateEvent(NULL, false, false, NULL);
	Buffer->DsNotify = Microsoft_DS::dsapi_QueryNotify(Buffer->DSB);
	if (!Buffer->DsNotify)
	{
		FreeDSBuffer(Buffer);
		goto Error;
	}

	if (!Microsoft_DS::dsapi_SetNotificationPositions(Buffer->DsNotify, Buffer->BufferBytes - 1, Buffer->NotifyEvent))
	{
		FreeDSBuffer(Buffer);
		goto Error;
	}

	SetVolume(Buffer, Buffer->Vol);
	SetPan(Buffer, Buffer->Pan);
	Buffer->WriteOffset = 0;
	Buffer->WaitEndFlag = 0;

	return true;

Error:
	RaiseEvent(CoreEventFlags::Error, CoreEvents::Crash, CoreObjects::None, CoreSubObjects::AudioDevice, Buffer->ID, L"can not create new buffer", VariadicAny(_missing));
	return false;
}
