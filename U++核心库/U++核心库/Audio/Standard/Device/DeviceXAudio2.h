#pragma once
#include "Config.h"
using namespace System;
using namespace System::Audio;

namespace System
{
	namespace Audio
	{
		class AudioDevice_XAudio2 : public AudioDevice_base
		{
		protected:
			AudioDevice_XAudio2();

			AudioDevice_XAudio2(const AudioDevice_XAudio2&) = delete;

		public:

			~AudioDevice_XAudio2();

			Result<AudioDevicePipe, AudioDeviceError> Open(const uint128& owner, ulong bufferTimeMs, const SampleInfo& info) override;///设置音频数据信息

			bool GetIsMultiPipesSupported() const override; ///是否支持多管线

			bool GetIsCurrentExclusive() const override; ///获取当前是否是独占模式

			unsigned int GetNumberOfPipes() override;///获取当前有效管线数量

			SampleInfo GetPipeInfo(const AudioDevicePipe& pipe) override; ///读取播放设备的格式信息

			void Write(const AudioDevicePipe& pipe, const void* buffer, uint bytes) override;	///写入数据，返回写入数据大小

			int CanWrite(const AudioDevicePipe& pipe) override;	///返回能写入的数据bytes长度

			bool WaitEnd(const AudioDevicePipe& pipe) override;///等待缓冲区播放完毕并返回是否正在播放

			bool Play(const AudioDevicePipe& pipe) override;///播放

			bool Pause(const AudioDevicePipe& pipe) override;///暂停

			bool Stop(const AudioDevicePipe& pipe) override;///停止

			void Flush(const AudioDevicePipe& pipe, int timeMS) override;/// 清空缓冲数据并重置输出时间

			void SetVolume(const AudioDevicePipe& pipe, double volume) override; /// 0-1.0

			void SetPan(const AudioDevicePipe& pipe, double pan) override; ///  -1.0 to 1.0

			double GetOutputTime(const AudioDevicePipe& pipe) override;		///返回已播放时间 

			double GetWrittenTime(const AudioDevicePipe& pipe) override;		///返回已写入时间 

			unsigned long GetWrittenPosition(const AudioDevicePipe& pipe) override;   ///返回已写入的数据量

			REFLECT_CLASS(AudioDevice_XAudio2)

		};
	}
}