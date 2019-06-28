#pragma once
#include "Addin.h"
using namespace System;

namespace System
{
	namespace Audio
	{
		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioDeviceError : __uobject
		{
			__uvalue_begin(AudioDeviceError, UShort)
			__uvalue(OK)
			__uvalue(Failed)
			__uvalue(UnSupportedInfo)
			__uvalue(DeviceError)
			__uvalue_end(AudioDeviceError, OK)
		};

		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioDecoderError : __uobject
		{
			__uvalue_begin(AudioDecoderError, UShort)
			__uvalue(OK)
			__uvalue(Failed)
			__uvalue(InvalidPath)
			__uvalue(UnSupported)
			__uvalue(End)
			__uvalue_end(AudioDecoderError, OK)
		};

		typedef safe_void_ptr AudioDevicePipe;
		typedef safe_void_ptr AudioDecoderPipe;
		typedef safe_void_ptr AudioDSPPipe;
		typedef safe_void_ptr AudioEncoderPipe;
		typedef safe_void_ptr AudioVisualPipe;

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioDevice_base : public Addin_base
		{
		protected:
			AudioDevice_base();

			AudioDevice_base(const AudioDevice_base&) = delete;

		public:

			virtual ~AudioDevice_base();

			__uref virtual Result<AudioDevicePipe, AudioDeviceError> Open(const UInt128& owner, UInt64 desiredBufferMs, const SampleInfo& desiredInfo, SampleInfo& actualInfo, UInt64& actualBufferMs);///设置音频数据信息

			__uref virtual bool GetIsMultiPipesSupported() const; ///是否支持多管线

			__uref virtual bool GetIsCurrentExclusive() const; ///获取当前是否是独占模式

			__uref virtual UInt64 GetNumberOfPipes();///获取当前有效管线数量

			__uref virtual SampleInfo GetPipeInfo(const AudioDevicePipe& pipe); ///读取播放设备的格式信息

			__uref virtual Result<bool, UInt64> Write(const AudioDevicePipe& pipe, const void* buffer, UInt64 bytes);	///写入数据，返回写入数据大小

			__uref virtual UInt64 CanWrite(const AudioDevicePipe& pipe);	///返回能写入的数据bytes长度

			__uref virtual bool Drain(const AudioDevicePipe& pipe);///等待缓冲区播放完毕并返回是否正在播放

			__uref virtual bool Play(const AudioDevicePipe& pipe);///播放

			__uref virtual bool Pause(const AudioDevicePipe& pipe);///暂停

			__uref virtual bool Stop(const AudioDevicePipe& pipe);///停止

			__uref virtual void Flush(const AudioDevicePipe& pipe, UInt64 timeMS);/// 清空缓冲数据并重置输出时间

			__uref virtual Result<bool, double> SetVolume(const AudioDevicePipe& pipe, double volume); /// 0-1.0

			__uref virtual Result<bool, double> SetPan(const AudioDevicePipe& pipe, double pan); ///  -1.0 to 1.0

			__uref virtual Result<bool, double> GetOutputTime(const AudioDevicePipe& pipe);		///返回已播放时间 

			__uref virtual Result<bool, double> GetWrittenTime(const AudioDevicePipe& pipe);		///返回已写入时间 

			__uref virtual Result<bool, UInt64> GetWrittenPosition(const AudioDevicePipe& pipe);   ///返回已写入的数据量

			REFLECT_CLASS(AudioDevice_base)

		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioDecoder_base : public Addin_base
		{
		protected:
			AudioDecoder_base();

			AudioDecoder_base(const AudioDecoder_base&) = delete;

		public:

			virtual ~AudioDecoder_base();

			__uref virtual Result<AudioDecoderPipe, AudioDecoderError> OpenLocal(const wchar_t* localPath);///加载读取本地音频文件

			__uref virtual Result<AudioDecoderPipe, AudioDecoderError> CreateStream();///创建流式解码管线

			__uref virtual bool IsStream(const AudioDecoderPipe& pipe); ///获取是否为流式解码管线

			__uref virtual UInt64 GetNumberOfPipes();///获取当前有效管线数量

			__uref virtual bool TryGetPipeInfo(const AudioDevicePipe& pipe, AudioInfo& info); ///尝试读取音频信息

			__uref virtual AudioDecoderError TryGetLocalAudioInfo(const wchar_t *path, AudioInfo& info);  ///尝试直接获取本地音频信息

			__uref virtual Result<UInt64, AudioDecoderError> Read(const AudioDevicePipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples);///读取音频PCM数据,返回实际读取的样本数

			__uref virtual bool TrySeek(const AudioDevicePipe& pipe, double posMS);///设置读取进度

			__uref virtual bool TryClose(const AudioDevicePipe&pipe);///关闭读取

			__uref virtual double GetCurrentMS(const AudioDevicePipe& pipe);///返回读取进度

			REFLECT_CLASS(AudioDecoder_base)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioDSP_base : public Addin_base
		{
		protected:
			AudioDSP_base();

			AudioDSP_base(const AudioDSP_base&) = delete;

		public:

			virtual ~AudioDSP_base();

			__uref virtual AudioDSPPipe Create(); ///创建音效管线

			__uref virtual UInt64 GetNumberOfPipes();///获取当前有效管线数量

			__uref virtual bool Destory(const AudioDSPPipe& pipe);///销毁效果器

			__uref virtual int Run(const AudioDSPPipe& pipe, AudioBuffer& buffer);///单管线效果变换

			REFLECT_CLASS(AudioDSP_base)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioEncoder_base : public Addin_base
		{
		protected:
			AudioEncoder_base();

			AudioEncoder_base(const AudioEncoder_base&) = delete;

		public:

			virtual ~AudioEncoder_base();

			__uref virtual AudioEncoderPipe Create(const wchar_t* path, const SampleInfo& info, const SampleInfo& encCfg);///创建音频文件

			__uref virtual UInt64 GetNumberOfPipes();///获取当前有效管线数量

			__uref virtual void AddData(const AudioEncoderPipe& pipe, const AudioBuffer& buffer);///增加音频PCM数据

			__uref virtual void FinshData(const AudioEncoderPipe& pipe);

			REFLECT_CLASS(AudioEncoder_base)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioVisual_base : public Addin_base
		{
		protected:
			AudioVisual_base();

			AudioVisual_base(const AudioVisual_base&) = delete;

		public:

			virtual ~AudioVisual_base();

			__uref virtual Result<AudioVisualPipe> Create(); ///创建可视化管线

			__uref virtual UInt64 GetNumberOfPipes();///获取当前有效管线数量

			__uref virtual bool Destory(const AudioVisualPipe& pipe);///销毁可视化管线

			__uref virtual void Run(const AudioVisualPipe& pipe, const AudioBuffer& buffer); ///输入可视化数据


			REFLECT_CLASS(AudioVisual_base)
		};
	}
}


