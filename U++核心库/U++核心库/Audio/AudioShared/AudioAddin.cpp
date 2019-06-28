#include "AudioAddin.h"


using namespace System::Audio;


AudioDevice_base::AudioDevice_base() {}
AudioDevice_base::~AudioDevice_base() {}

AudioDecoder_base::AudioDecoder_base() {}
AudioDecoder_base::~AudioDecoder_base() {}

AudioDSP_base::AudioDSP_base() {}
AudioDSP_base::~AudioDSP_base() {}

AudioVisual_base::AudioVisual_base() {}
AudioVisual_base::~AudioVisual_base() {}

AudioEncoder_base::AudioEncoder_base() {}
AudioEncoder_base::~AudioEncoder_base() {}



Result<AudioDevicePipe, AudioDeviceError> AudioDevice_base::Open(const UInt128& owner, UInt64 desiredBufferMs, const SampleInfo& desiredInfo, SampleInfo& actualInfo, UInt64& actualBufferMs)
{
	return Result<AudioDevicePipe, AudioDeviceError>(nullptr, AudioDeviceError::Failed, L"not implemented");
}

bool AudioDevice_base::GetIsMultiPipesSupported() const
{
	return false;
}

bool AudioDevice_base::GetIsCurrentExclusive() const
{
	return false;
}

UInt64 AudioDevice_base::GetNumberOfPipes()
{
	return 0Ul;
}

SampleInfo AudioDevice_base::GetPipeInfo(const AudioDevicePipe& pipe)
{
	return SampleInfo();
}

Result<bool, UInt64> AudioDevice_base::Write(const AudioDevicePipe& pipe, const void* buffer, UInt64 bytes)
{
	return Result<bool, UInt64>(false, 0ull);
}

UInt64 AudioDevice_base::CanWrite(const AudioDevicePipe& pipe)
{
	return 0ul;
}

bool AudioDevice_base::Drain(const AudioDevicePipe& pipe)
{
	return false;
}

bool AudioDevice_base::Play(const AudioDevicePipe& pipe)
{
	return false;
}

bool AudioDevice_base::Pause(const AudioDevicePipe& pipe)
{
	return false;
}

bool AudioDevice_base::Stop(const AudioDevicePipe& pipe)
{
	return false;
}

void AudioDevice_base::Flush(const AudioDevicePipe& pipe, UInt64 timeMS)
{

}

Result<bool, double> AudioDevice_base::SetVolume(const AudioDevicePipe& pipe, double volume)
{
	return Result<bool, double>(false, 0.0, L"not implemented");
}

Result<bool, double> AudioDevice_base::SetPan(const AudioDevicePipe& pipe, double pan)
{
	return Result<bool, double>(false, 0.0, L"not implemented");
}

Result<bool, double> AudioDevice_base::GetOutputTime(const AudioDevicePipe& pipe)
{
	return Result<bool, double>(false, 0.0, L"not implemented");
}

Result<bool, double> AudioDevice_base::GetWrittenTime(const AudioDevicePipe& pipe)
{
	return Result<bool, double>(false, 0.0, L"not implemented");
}

Result<bool, UInt64> AudioDevice_base::GetWrittenPosition(const AudioDevicePipe& pipe)
{
	return Result<bool, UInt64>(false, 0U, L"not implemented");
}





Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_base::OpenLocal(const wchar_t* localPath)
{
	return Result<AudioDecoderPipe, AudioDecoderError>(nullptr, AudioDecoderError::Failed, L"not implemented");
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_base::CreateStream()
{
	return Result<AudioDecoderPipe, AudioDecoderError>(nullptr, AudioDecoderError::Failed, L"not implemented");
}


bool AudioDecoder_base::IsStream(const AudioDecoderPipe& pipe)
{
	return false;
}


UInt64 AudioDecoder_base::GetNumberOfPipes()
{
	return 0Ul;
}


bool AudioDecoder_base::TryGetPipeInfo(const AudioDevicePipe& pipe, AudioInfo& info)
{
	return false;
}


AudioDecoderError AudioDecoder_base::TryGetLocalAudioInfo(const wchar_t *path, AudioInfo& info)
{
	return AudioDecoderError::Failed;
}


Result<UInt64, AudioDecoderError> AudioDecoder_base::Read(const AudioDevicePipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples)
{
	return Result<UInt64, AudioDecoderError>(0U, AudioDecoderError::Failed, L"not implemented");
}


bool AudioDecoder_base::TrySeek(const AudioDevicePipe& pipe, double posMS)
{
	return false;
}


bool AudioDecoder_base::TryClose(const AudioDevicePipe&pipe)
{
	return false;
}


double AudioDecoder_base::GetCurrentMS(const AudioDevicePipe& pipe)
{
	return 0.0;
}

AudioDSPPipe AudioDSP_base::Create()
{
	return nullptr;
}

UInt64 AudioDSP_base::GetNumberOfPipes()
{
	return 0Ul;
}

bool AudioDSP_base::Destory(const AudioDSPPipe& pipe)
{
	return false;
}

int AudioDSP_base::Run(const AudioDSPPipe& pipe, AudioBuffer& buffer)
{
	return 0;
}

AudioEncoderPipe AudioEncoder_base::Create(const wchar_t* path, const SampleInfo& info, const SampleInfo& encCfg)
{
	return nullptr;
}

UInt64 AudioEncoder_base::GetNumberOfPipes()
{
	return 0Ul;
}

void AudioEncoder_base::AddData(const AudioEncoderPipe& pipe, const AudioBuffer& buffer)
{

}

void AudioEncoder_base::FinshData(const AudioEncoderPipe& pipe)
{

}


Result<AudioVisualPipe> AudioVisual_base::Create()
{
	return Result<AudioVisualPipe>(nullptr);
}

UInt64 AudioVisual_base::GetNumberOfPipes()
{
	return 0Ul;
}

bool AudioVisual_base::Destory(const AudioVisualPipe& pipe)
{
	return false;
}

void AudioVisual_base::Run(const AudioVisualPipe& pipe, const AudioBuffer& buffer)
{

}