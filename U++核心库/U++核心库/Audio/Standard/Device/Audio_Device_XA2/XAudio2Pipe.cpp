#include "XAudio2Pipe.h"
#include <process.h>
using namespace System::Audio;

IXAudio2* XAudio2Pipe::pXAudio2 = nullptr;
IXAudio2MasteringVoice* XAudio2Pipe::pMasteringVoice = nullptr;
bool XAudio2Pipe::isinited = false;

XAudio2Pipe::XAudio2Pipe() :
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA) 
hBufferEndEvent(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))
#else 
hBufferEndEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
#endif 
{
}

XAudio2Pipe::~XAudio2Pipe()
{
	Stop();
	if (pSourceVoice)
	{
		pSourceVoice->DestroyVoice();
		pSourceVoice = nullptr;
	}
	if (pool)
	{
		free(pool);
		pool = nullptr;
	}
	if (locker)
	{
		CloseHandle(locker);
		locker = NULL;
	}
	if (hBufferEndEvent)
	{
		CloseHandle(hBufferEndEvent);
		hBufferEndEvent = NULL;
	}
}

XAudio2Pipe* XAudio2Pipe::Create(UInt64 desiredBufferMs, const SampleInfo& info, SampleInfo& actualInfo, UInt64& actualBufferMs)
{
	XAudio2Pipe* pipe = new XAudio2Pipe();
	memset(&pipe->wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	pipe->wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	pipe->wfx.Format.nChannels = info.Channels;
	pipe->wfx.Format.nSamplesPerSec = info.SampleRate;
	pipe->wfx.Format.wBitsPerSample = info.BitsPerSample.BPS;
	pipe->wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE)-sizeof(WAVEFORMATEX);
	pipe->wfx.Samples.wValidBitsPerSample = pipe->wfx.Format.wBitsPerSample;
	pipe->wfx.Format.nBlockAlign = info.BlockAlign;
	pipe->wfx.Format.nAvgBytesPerSec = info.AvgBytesPerSec;

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
	
	hr = pXAudio2->CreateSourceVoice(&pipe->pSourceVoice, (LPCWAVEFORMATEX)&pipe->wfx, XAUDIO2_VOICE_NOPITCH, 1.0f, pipe);
	if (FAILED(hr))
	{
		SAFE_DELETE(pipe->pSourceVoice); 
		SAFE_DELETE(pipe);
		return nullptr;
	}

	pipe->poolsize = (size_t)((double)desiredBufferMs / 1000.0*(double)pipe->wfx.Format.nAvgBytesPerSec);
	pipe->blocksize = pipe->poolsize / BUFFERCOUNT;
	pipe->blocksize = pipe->blocksize + pipe->blocksize % pipe->wfx.Format.nBlockAlign;
	pipe->poolsize = pipe->blocksize * BUFFERCOUNT;
	pipe->pool = (BYTE*)calloc(pipe->poolsize, 1u);
	pipe->poolcur = 0u;
	pipe->writeroffset = 0u;
	pipe->start = 0u;
	pipe->sinfo = info;
	pipe->vol = 1.0;
	pipe->pan = 0.0;
	pipe->KillFillThread = false;
	pipe->locker = CreateMutexA(NULL, false, NULL);
	pipe->fillcur = 0u;

	pipe->FillThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &FillThread, pipe, CREATE_SUSPENDED, 0);
	SetThreadPriority(pipe->FillThreadHandle, THREAD_PRIORITY_HIGHEST);
	ResumeThread(pipe->FillThreadHandle);

	actualInfo = info;
	actualBufferMs = desiredBufferMs;

	return pipe;
}

size_t XAudio2Pipe::Write(const BYTE* buf, size_t bytes)
{
	if (!buf || !bytes) return 0ull;
	WaitForSingleObject(locker, INFINITE);
	size_t tail = poolcur + bytes;
	if (tail > poolsize)
	{
		size_t fill = poolsize - poolcur;
		memcpy(pool + poolcur, buf, fill);
		size_t left = bytes - fill;
		memcpy(pool, buf + fill, left);	
		poolcur = left;
	}
	else
	{
		memcpy(pool + poolcur, buf, bytes);
		poolcur = tail == poolsize ? 0u : tail;
	}
	ReleaseMutex(locker);
	return bytes;
}

unsigned int _stdcall XAudio2Pipe::FillThread(void* Obj)
{
	XAudio2Pipe* pipe = (XAudio2Pipe*)Obj;
	while (!pipe->KillFillThread)
	{
		bool maybeend = false;
		XAUDIO2_VOICE_STATE state;
		while (true)
		{
			if (pipe->KillFillThread)
			{
				_endthreadex(1);
				return 1;
			}
			pipe->pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			maybeend = state.BuffersQueued <= 1u;
			if (state.BuffersQueued < BUFFERWAIT + 1)
				break;
			WaitForSingleObject(pipe->hBufferEndEvent, INFINITE);
		}
		if (pipe->KillFillThread)
		{
			_endthreadex(1);
			return 1;
		}
		WaitForSingleObject(pipe->locker, INFINITE);
		size_t len = 0u;
		if (pipe->fillcur < pipe->poolcur) len = pipe->poolcur - pipe->fillcur;
		else if (pipe->fillcur > pipe->poolcur) len = pipe->poolsize - pipe->fillcur + pipe->poolcur;

		if (len> pipe->blocksize)
		{
			if (pipe->fillcur + pipe->blocksize <= pipe->poolsize)
			{
				XAUDIO2_BUFFER buf = { 0 };
				buf.AudioBytes = pipe->blocksize;
				buf.pAudioData = pipe->pool + pipe->fillcur;
				pipe->xlens.push_back(buf.AudioBytes);
				pipe->pSourceVoice->SubmitSourceBuffer(&buf);
				pipe->fillcur += pipe->blocksize;
				if (pipe->fillcur >= pipe->poolsize) pipe->fillcur = 0u;
			}
			else
			{
				size_t nest = pipe->poolsize - pipe->fillcur;
				size_t left = pipe->blocksize - nest;

				XAUDIO2_BUFFER buf1 = { 0 };
				buf1.AudioBytes = nest;
				buf1.pAudioData = pipe->pool + pipe->fillcur;
				pipe->xlens.push_back(buf1.AudioBytes);
				pipe->pSourceVoice->SubmitSourceBuffer(&buf1);

				XAUDIO2_BUFFER buf2 = { 0 };
				buf2.AudioBytes = left;
				buf2.pAudioData = pipe->pool;
				pipe->xlens.push_back(buf2.AudioBytes);
				pipe->pSourceVoice->SubmitSourceBuffer(&buf2);

				pipe->fillcur = left;
			}
		}
		else
		{
			if (maybeend && len > 0u)
			{
				if (pipe->fillcur + len <= pipe->poolsize)
				{
					XAUDIO2_BUFFER buf = { 0 };
					buf.AudioBytes = len;
					buf.pAudioData = pipe->pool + pipe->fillcur;
					pipe->xlens.push_back(buf.AudioBytes);
					pipe->pSourceVoice->SubmitSourceBuffer(&buf);
					pipe->fillcur += len;
					if (pipe->fillcur >= pipe->poolsize) pipe->fillcur = 0u;
				}
				else
				{
					size_t nest = pipe->poolsize - pipe->fillcur;
					size_t left = len - nest;

					XAUDIO2_BUFFER buf1 = { 0 };
					buf1.AudioBytes = nest;
					buf1.pAudioData = pipe->pool + pipe->fillcur;
					pipe->xlens.push_back(buf1.AudioBytes);
					pipe->pSourceVoice->SubmitSourceBuffer(&buf1);

					XAUDIO2_BUFFER buf2 = { 0 };
					buf2.AudioBytes = left;
					buf2.pAudioData = pipe->pool;
					pipe->xlens.push_back(buf2.AudioBytes);
					pipe->pSourceVoice->SubmitSourceBuffer(&buf2);
					pipe->fillcur = left;

				}
			}
		}

		ReleaseMutex(pipe->locker);
	}
	_endthreadex(0);
	return 0;
}


size_t XAudio2Pipe::CanWrite()
{
	WaitForSingleObject(locker, INFINITE);
	poolplaycur = (size_t)(fillcur % (UINT64)poolsize);
	if (poolplaycur < poolcur)
	{
		ReleaseMutex(locker);
		return poolsize - poolcur + poolplaycur;
	}
	if (poolplaycur == poolcur)
	{
		ReleaseMutex(locker);
		return poolsize;
	}
	ReleaseMutex(locker);
	return poolplaycur - poolcur;
}

bool XAudio2Pipe::Play()
{
	return pSourceVoice!=nullptr ? pSourceVoice->Start(0, 0) == S_OK : false;
}

bool XAudio2Pipe::Pause()
{
	return pSourceVoice != nullptr ? pSourceVoice->Stop(0, 0) == S_OK : false;
}

bool XAudio2Pipe::Stop()
{
	if (pSourceVoice == nullptr) return false;
	if (pSourceVoice->Stop(0, 0) == S_OK)
	{
		WaitForSingleObject(locker, INFINITE);
		KillFillThread = true;
		SetEvent(hBufferEndEvent);
		if (pSourceVoice->FlushSourceBuffers() == S_OK)
		{
			xlens.clear();
			ReleaseMutex(locker);
			if (WaitForSingleObject(FillThreadHandle, INFINITE) == WAIT_TIMEOUT)
			{
				TerminateThread(FillThreadHandle, 0);
			}
			if (!CloseHandle(FillThreadHandle))
				TerminateThread(FillThreadHandle, 0);
			FillThreadHandle = INVALID_HANDLE_VALUE;
			if (pSourceVoice)
			{
				pSourceVoice->DestroyVoice();
				pSourceVoice = nullptr;
			}
			if (pool)
			{
				free(pool);
				pool = nullptr;
			}
			poolsize = 0u;
			poolcur = 0u;
			poolplaycur = 0u;
			writeroffset = 0u;
			start = 0u;
			return true;
		}
	}
	return false;
}

bool XAudio2Pipe::Flush(UInt64 timeMS)
{
	if (pSourceVoice == nullptr) return false;
	WaitForSingleObject(locker, INFINITE);
	if (pSourceVoice->FlushSourceBuffers() == S_OK)
	{
		xlens.clear();
		pool = (BYTE*)memset(pool, 0, poolsize);
		poolcur = 0u;
		poolplaycur = 0u;
		fillcur = 0u;
		writeroffset = (double)timeMS / 1000.0 * wfx.Format.nAvgBytesPerSec;
		start = writeroffset;
		ReleaseMutex(locker);
		return true;
	}
	ReleaseMutex(locker);
	return false;
}

double XAudio2Pipe::SetPan(double v)
{
	if (pSourceVoice == nullptr)
	{
		vol = CLAMP<double>(v, -1.0, 1.0);
		return vol;
	}
	XAUDIO2_VOICE_DETAILS vd;
	pSourceVoice->GetVoiceDetails(&vd);
	if (vd.InputChannels==2u)
	{
		float vols[2] = { (pan >= 0) ? (1.f - pan) : 1.f, (pan <= 0) ? (-pan - 1.f) : 1.f };
		pSourceVoice->SetChannelVolumes(2u, vols, 0);
		return pan;
	}
	return pan;
}

double XAudio2Pipe::SetVolume(double v)
{
	if (pSourceVoice == nullptr)
	{
		vol = CLAMP<double>(v, 0.0, 1.0);
		return vol;
	}
	pSourceVoice->SetVolume(vol, 0);
	return vol;
}

double XAudio2Pipe::GetOutputTime()
{
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);
	UINT64 played = state.SamplesPlayed;
	double playoffset = played + start;
	return (double)(playoffset) / (double)wfx.Format.nAvgBytesPerSec*1000.0;
}

double XAudio2Pipe::GetWrittenTime()
{
	return (double)(writeroffset) / (double)wfx.Format.nAvgBytesPerSec*1000.0;
}

UInt64 XAudio2Pipe::GetWrittenPosition()
{
	return writeroffset;
}

bool XAudio2Pipe::WaitEnd()
{
	if (pSourceVoice == nullptr) return false;
	XAUDIO2_VOICE_STATE state;
	while (true)
	{	
		pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
		if (state.BuffersQueued == 0u)
		{
			break;
		}
		WaitForSingleObject(hBufferEndEvent, INFINITE);
	}
	return false;
}

Result<bool> XAudio2Pipe::InitEngine()
{
	Result<bool> result(false);
	if (isinited)
	{
		result.Value = true;
		return result;
	}
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	UINT32 flags = 0;
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG) 
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif 
	HRESULT hr = XAudio2Create(&pXAudio2, flags);
	if (FAILED(hr))
	{
		CoUninitialize();
		result.WriteMessage(L"can not init XAudio2 engine");
		return result;
	}
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG) 
	XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	pXAudio2->SetDebugConfiguration(&debug, 0);
#endif 

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
	{
		result.WriteMessage(L"Failed creating mastering voice");
		SAFE_RELEASE(pXAudio2);
		CoUninitialize();
		return result;
	}

	result.Value = true;
	return result;
}

void XAudio2Pipe::ExitEngine()
{
	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	isinited = false;
}