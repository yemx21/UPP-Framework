#include "Device_Wasapi.h"
#include "WasapiPipe.h"
using namespace System::Audio;

mutex AudioDevice_Wasapi::locker;
AudioDevice_Wasapi* AudioDevice_Wasapi::instance = nullptr;
const wchar_t* AudioDevice_Wasapi::_author = L"xworkteam powered by Wasapi";
const wchar_t* AudioDevice_Wasapi::_description = L"Wasapi≤•∑≈…Ë±∏";
const wchar_t* AudioDevice_Wasapi::_supports = L"";
CRITICAL_SECTION AudioDevice_Wasapi::cs = {};

AudioDevice_Wasapi::AudioDevice_Wasapi()
{
	InitializeCriticalSection(&cs);
}

AudioDevice_Wasapi::~AudioDevice_Wasapi()
{
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	WasapiPipe::ExitEngine();
}

AudioDevice_Wasapi* AudioDevice_Wasapi::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			auto res = WasapiPipe::InitEngine();
			if (res.Value)
				instance = new AudioDevice_Wasapi();
		}
	}
	return instance;
}

Version AudioDevice_Wasapi::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDevice_Wasapi::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDevice_Wasapi::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDevice_Wasapi::GetSupportFormat() const
{
	return _supports;
}

Result<AudioDevicePipe, AudioDeviceError> AudioDevice_Wasapi::Open(const UInt128& owner, UInt64 desiredBufferMs, const SampleInfo& desiredInfo, SampleInfo& actualInfo, UInt64& actualBufferMs)
{
	Result<AudioDevicePipe, AudioDeviceError> result(nullptr, AudioDeviceError::Failed);

	if (!waveOutGetNumDevs())
	{
		result.WriteMessage(L"no audio device found using waveout");
		return result;
	}
	if (desiredBufferMs < 50ull) desiredBufferMs = 50ull;
	WasapiPipe* Buffer = WasapiPipe::Create(desiredInfo, desiredBufferMs, actualInfo, actualBufferMs);
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

bool AudioDevice_Wasapi::GetIsMultiPipesSupported() const
{
	return true;
}

bool AudioDevice_Wasapi::GetIsCurrentExclusive() const
{
	return false;
}

UInt64 AudioDevice_Wasapi::GetNumberOfPipes()
{
	return Pipes.size();
}

SampleInfo AudioDevice_Wasapi::GetPipeInfo(const AudioDevicePipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		WasapiPipe* Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (Buffer) return Buffer->sinfo;
	}
	catch (...){}
	return SampleInfo();
}

Result<bool, UInt64> AudioDevice_Wasapi::Write(const AudioDevicePipe& pipe, const void* buffer, UInt64 bytes)
{
	Result<bool, UInt64> result(false, 0ull);
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return result;
		if (!buffer) return result;
	}
	catch (...){ return result; }
	result.State = Buffer->Write((const BYTE*)buffer, bytes, result.Value);
	return result;
}

UInt64 AudioDevice_Wasapi::CanWrite(const AudioDevicePipe& pipe)
{
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return 0;
	}
	catch (...){ return 0; }

	int space=Buffer->CanWrite();
	if (space < Buffer->wfx.Format.nBlockAlign)
		return 0;
	else
		return space - Buffer->wfx.Format.nBlockAlign;
}

bool AudioDevice_Wasapi::Drain(const AudioDevicePipe& pipe)
{
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return false;
	}
	catch (...){ return false; }
	return Buffer->WaitEnd();
}

bool AudioDevice_Wasapi::Play(const AudioDevicePipe& pipe)
{
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return false;
	}
	catch (...){ return false; }
	return Buffer->Play();
}

bool AudioDevice_Wasapi::Pause(const AudioDevicePipe& pipe)
{
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return false;
	}
	catch (...){ return false; }
	return Buffer->Pause();
}

bool AudioDevice_Wasapi::Stop(const AudioDevicePipe& pipe)
{
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return true;
	}
	catch (...){ return false; }

	if (Buffer->Stop())
	{
		EnterCriticalSection(&cs);
		std::vector<WasapiPipe*>::iterator pend = std::remove_if(Pipes.begin(), Pipes.end(), std::bind1st(std::equal_to<WasapiPipe*>(), Buffer));
		Pipes.erase(pend);
		delete Buffer;
		Buffer = NULL;
		LeaveCriticalSection(&cs);
		return true;
	}
	return false;
}

void AudioDevice_Wasapi::Flush(const AudioDevicePipe& pipe, UInt64 timeMS)
{
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return;
	}
	catch (...){ return; }
	Buffer->Flush(timeMS);
}

Result<bool, double> AudioDevice_Wasapi::SetVolume(const AudioDevicePipe& pipe, double volume)
{
	Result<bool, double> result(false, volume);
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->SetVolume(volume);
	return result;
}

Result<bool, double> AudioDevice_Wasapi::SetPan(const AudioDevicePipe& pipe, double pan)
{
	Result<bool, double> result(false, pan);
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->SetPan(pan);
	return result;
}

Result<bool, double> AudioDevice_Wasapi::GetOutputTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State=Buffer->GetOutputTime();
	return result;
}

Result<bool, double> AudioDevice_Wasapi::GetWrittenTime(const AudioDevicePipe& pipe)
{
	Result<bool, double> result(false, 0.0);
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->GetWrittenTime();
	return result;
}

Result<bool, UInt64> AudioDevice_Wasapi::GetWrittenPosition(const AudioDevicePipe& pipe)
{
	Result<bool, UInt64> result(false, 0ull);
	WasapiPipe* Buffer = nullptr;
	try
	{
		void* pipePtr = pipe.get(sizeof(WasapiPipe));
		Buffer = static_cast<WasapiPipe*>(pipePtr);
		if (!Buffer) return result;
	}
	catch (...){ return result; }
	result.Value = true;
	result.State = Buffer->GetWrittenPosition();
	return result;
}

