#pragma once
#include "Config.h"
#include <Windows.h>
#include <Ks.h>
#include <Ksmedia.h>
#include <xaudio2.h>
#include <vector>
#define MAX_WAVEBLOCKS    256

namespace System
{
	namespace Audio
	{
		class XAudio2Pipe;

		__uclass __uobjdef(asm = Audio_Device_XA2, ns = System::Audio, ver = 1.0) AudioDevice_XA2 : public AudioDevice_base
		{
		private:
			static const wchar_t* _author;
			static const wchar_t* _description;
			static const wchar_t* _supports;
			static mutex locker;
			static CRITICAL_SECTION cs;
			static AudioDevice_XA2 * instance;
			std::vector<XAudio2Pipe*> Pipes;

		protected:
			AudioDevice_XA2();

			AudioDevice_XA2(const AudioDevice_XA2&) = delete;

		public:
			__udefctor static AudioDevice_XA2* Create();

			~AudioDevice_XA2();

			__uref Version GetVersion() const  override;
			__uref const wchar_t* GetAuthor() const  override;
			__uref const wchar_t* GetDescription() const  override;
			__uref const wchar_t* GetSupportFormat() const  override;

		public:
			__uref Result<AudioDevicePipe, AudioDeviceError> Open(const UInt128& owner, UInt64 desiredBufferMs, const SampleInfo& desiredInfo, SampleInfo& actualInfo, UInt64& actualBufferMs) override;///设置音频数据信息

			__uref bool GetIsMultiPipesSupported() const override; ///是否支持多管线

			__uref bool GetIsCurrentExclusive() const override; ///获取当前是否是独占模式

			__uref UInt64 GetNumberOfPipes() override;///获取当前有效管线数量

			__uref SampleInfo GetPipeInfo(const AudioDevicePipe& pipe) override; ///读取播放设备的格式信息

			__uref Result<bool, UInt64> Write(const AudioDevicePipe& pipe, const void* buffer, UInt64 bytes) override;	///写入数据，返回写入数据大小

			__uref UInt64 CanWrite(const AudioDevicePipe& pipe) override;	///返回能写入的数据bytes长度

			__uref bool Drain(const AudioDevicePipe& pipe) override;///等待缓冲区播放完毕并返回是否正在播放

			__uref bool Play(const AudioDevicePipe& pipe) override;///播放

			__uref bool Pause(const AudioDevicePipe& pipe) override;///暂停

			__uref bool Stop(const AudioDevicePipe& pipe) override;///停止

			__uref void Flush(const AudioDevicePipe& pipe, UInt64 timeMS) override;/// 清空缓冲数据并重置输出时间

			__uref Result<bool, double> SetVolume(const AudioDevicePipe& pipe, double volume) override; /// 0-1.0

			__uref Result<bool, double> SetPan(const AudioDevicePipe& pipe, double pan) override; ///  -1.0 to 1.0

			__uref Result<bool, double> GetOutputTime(const AudioDevicePipe& pipe) override;		///返回已播放时间 

			__uref Result<bool, double> GetWrittenTime(const AudioDevicePipe& pipe) override;		///返回已写入时间 

			__uref Result<bool, UInt64> GetWrittenPosition(const AudioDevicePipe& pipe) override;   ///返回已写入的数据量

			REFLECT_CLASS(AudioDevice_XA2)

		};
	}
}  