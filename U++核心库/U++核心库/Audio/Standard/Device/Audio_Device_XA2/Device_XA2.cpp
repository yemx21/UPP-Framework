#include "Device_XA2.h"
#include "XAudio2Pipe.h"
using namespace System::Audio;

mutex AudioDevice_XA2::locker;
AudioDevice_XA2* AudioDevice_XA2::instance = nullptr;
const wchar_t* AudioDevice_XA2::_author = L"xworkteam powered by XAudio2";
const wchar_t* AudioDevice_XA2::_description = L"XAudio2≤•∑≈…Ë±∏";
const wchar_t* AudioDevice_XA2::_supports = L"";
CRITICAL_SECTION AudioDevice_XA2::cs = {};

AudioDevice_XA2::AudioDevice_XA2()
{
	InitializeCriticalSection(&cs);
}

AudioDevice_XA2::~AudioDevice_XA2()
{
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	XAudio2Pipe::ExitEngine();
}

AudioDevice_XA2* AudioDevice_XA2::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			auto res = XAudio2Pipe::InitEngine();
			if (res.Value)
				instance = new AudioDevice_XA2();
		}
	}
	return instance;
}

Version AudioDevice_XA2::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDevice_XA2::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDevice_XA2::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDevice_XA2::GetSupportFormat() const
{
	return _supports;
}

Result<AudioDevicePipe, AudioDeviceError> AudioDevice_XA2::Open(const UInt128& owner, UInt64 desiredBufferMs, const SampleInfo& desiredInfo, SampleInfo& actualInfo, UInt64& actualBufferMs)
{
	Result<AudioDevicePipe, AudioDeviceError> result(nullptr, AudioDeviceError::Failed);

	if (!waveOutGetNumDevs())
	{
		result.WriteMessage(L"no audio device found using waveout");
		return result;
	}
	if (desiredBufferMs < 50u) desiredBufferMs = 50u;
	XAudio2Pipe* Buffer = XAudio2Pipe::Create(desiredBufferMs, desiredInfo, actualInfo, actualBufferMs);
	if (Buffer)
	{
		EnterCriticalSection(&cs);
		Pipes.push_back(Buffer);
		LeaveCriticalSection(&cs);
		result.Value = Buffer;
		result.State = AudioDeviceError::OK;
		return result;
	}
	result.WriteMessage(L"can not create xaudio pipe");
	return result;
}

bool AudioDevice_XA2::GetIsMultiPipesSupported() const
{
	return true;
}

bool AudioDevice_XA2::GetIsCurrentExclusive() const
{
	return false;
}

UInt64 AudioDevice_XA2::GetNumberOfPipes()
{
	return Pipes.size();
}

SampleInfo AudioDevice_XA2::GetPipeInfo(const AudioDevicePipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		XAudio2Pipe* Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (Buffer) return Buffer->sinfo;
	}
	catch (...){}
	return SampleInfo();
}
Result<bool, UInt64> AudioDevice_XA2::Write(const AudioDevicePipe& pipe, const void* buffer, UInt64 bytes)
{
	Result<bool, UInt64> result(false, 0ull);
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return result;
		if (!buffer) return result;
	}
	catch (...){ return result; }
	result.State=Buffer->Write((const BYTE*)buffer, bytes);
	result.Value = result.State != 0ull;
	return result;
}

UInt64 AudioDevice_XA2::CanWrite(const AudioDevicePipe& pipe)
{
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return 0;
	}
	catch (...){ return 0; }

	UInt64 space=Buffer->CanWrite();
	if (space < Buffer->wfx.Format.nBlockAlign)
		return 0;
	else
		return space - Buffer->wfx.Format.nBlockAlign;
}

bool AudioDevice_XA2::Drain(const AudioDevicePipe& pipe)
{
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return false;
	}
	catch (...){ return false; }
	return Buffer->WaitEnd();
}

bool AudioDevice_XA2::Play(const AudioDevicePipe& pipe)
{
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return false;
	}
	catch (...){ return false; }
	return Buffer->Play();
}

bool AudioDevice_XA2::Pause(const AudioDevicePipe& pipe)
{
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return false;
	}
	catch (...){ return false; }
	return Buffer->Pause();
}

bool AudioDevice_XA2::Stop(const AudioDevicePipe& pipe)
{
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return true;
	}
	catch (...){ return false; }

	if (Buffer->Stop())
	{
		EnterCriticalSection(&cs);
		std::vector<XAudio2Pipe*>::iterator pend = std::remove_if(Pipes.begin(), Pipes.end(), std::bind1st(std::equal_to<XAudio2Pipe*>(), Buffer));
		Pipes.erase(pend);
		delete Buffer;
		Buffer = NULL;
		LeaveCriticalSection(&cs);
		return true;
	}
	return false;
}

void AudioDevice_XA2::Flush(const AudioDevicePipe& pipe, UInt64 timeMS)
{
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return;
	}
	catch (...){ return; }
	Buffer->Flush(timeMS);
}

Result<bool, double> AudioDevice_XA2::SetVolume(const AudioDevicePipe& pipe, double volume)
{
	Result<bool, double> result(false, volume);
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->SetVolume(volume);
	return result;
}

Result<bool, double> AudioDevice_XA2::SetPan(const AudioDevicePipe& pipe, double pan)
{
	Result<bool, double> result(false, pan);
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->SetPan(pan);
	return result;
}

Result<bool, double> AudioDevice_XA2::GetOutputTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State=Buffer->GetOutputTime();
	return result;
}

Result<bool, double> AudioDevice_XA2::GetWrittenTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->GetWrittenTime();
	return result;
}

Result<bool, UInt64> AudioDevice_XA2::GetWrittenPosition(const AudioDevicePipe& pipe)
{
	Result<bool, UInt64> result(false, 0U);
	XAudio2Pipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(XAudio2Pipe));
		Buffer = static_cast<XAudio2Pipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->GetWrittenPosition();
	return result;
}

