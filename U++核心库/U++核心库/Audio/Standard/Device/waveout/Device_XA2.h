#pragma once
#include "Config.h"
#include <MMReg.h>
#include <Ks.h>
#include <Ksmedia.h>

#define MAX_WAVEBLOCKS    256

namespace System
{
	namespace Audio
	{
		struct WOBuffer : ZeroStruct<WOBuffer>
		{
			ulong WaitEndTime; ///动态等待播放超时时间
			ulong SampleTime;///缓冲样本长度
			ulong BufferBytes;    ///缓冲样本数据大小
			ulong WriteOffset;
			ulong OutputWrittenPointer;
			WAVEFORMATEXTENSIBLE WFE;///波形格式扩展
			SampleInfo Info;
			HANDLE Mutex;
			uint128 ID;
			int WaitEndFlag;
			bool WritePlay;
			double Vol;
			double Pan;
			bool DestroyDEV;

			HWAVEOUT dev = NULL;
			unsigned int  ScheduledBlocks;
			int   PlayedWaveHeadersCount;
			WAVEHDR*  PlayedWaveHeaders[MAX_WAVEBLOCKS];

			WOBuffer()
			{

			}

			~WOBuffer()
			{

			}

		};


		__uclass __uobjdef(asm = Audio_Device_WaveOut, ns = System::Audio, ver = 1.0)  AudioDevice_WaveOut : public AudioDevice_base
		{
		private:
			static const wchar_t* _author;
			static const wchar_t* _description;
			static const wchar_t* _supports;
			static mutex locker;
			static CRITICAL_SECTION cs;
			static AudioDevice_WaveOut * instance;
			std::vector<WOBuffer*> Pipes;

			static void CALLBACK wave_callback(HWAVE hWave, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

			void Free(WOBuffer* bufffer);

		protected:
			AudioDevice_WaveOut();

			AudioDevice_WaveOut(const AudioDevice_WaveOut&) = delete;

		public:
			__udefctor static AudioDevice_WaveOut* Create();

			~AudioDevice_WaveOut();

			__uref Version GetVersion() const  override;
			__uref const wchar_t* GetAuthor() const  override;
			__uref const wchar_t* GetDescription() const  override;
			__uref const wchar_t* GetSupportFormat() const  override;

		public:
			__uref Result<AudioDevicePipe, AudioDeviceError> Open(const uint128& owner, ulong bufferTimeMs, const SampleInfo& info) override;///设置音频数据信息

			__uref bool GetIsMultiPipesSupported() const override; ///是否支持多管线

			__uref bool GetIsCurrentExclusive() const override; ///获取当前是否是独占模式

			__uref unsigned int GetNumberOfPipes() override;///获取当前有效管线数量

			__uref SampleInfo GetPipeInfo(const AudioDevicePipe& pipe) override; ///读取播放设备的格式信息

			__uref void Write(const AudioDevicePipe& pipe, const void* buffer, uint bytes) override;	///写入数据，返回写入数据大小

			__uref int CanWrite(const AudioDevicePipe& pipe) override;	///返回能写入的数据bytes长度

			__uref bool WaitEnd(const AudioDevicePipe& pipe) override;///等待缓冲区播放完毕并返回是否正在播放

			__uref bool Play(const AudioDevicePipe& pipe) override;///播放

			__uref bool Pause(const AudioDevicePipe& pipe) override;///暂停

			__uref bool Stop(const AudioDevicePipe& pipe) override;///停止

			__uref void Flush(const AudioDevicePipe& pipe, int timeMS) override;/// 清空缓冲数据并重置输出时间

			__uref Result<bool, double> SetVolume(const AudioDevicePipe& pipe, double volume) override; /// 0-1.0

			__uref Result<bool, double> SetPan(const AudioDevicePipe& pipe, double pan) override; ///  -1.0 to 1.0

			__uref Result<bool, double> GetOutputTime(const AudioDevicePipe& pipe) override;		///返回已播放时间 

			__uref Result<bool, double> GetWrittenTime(const AudioDevicePipe& pipe) override;		///返回已写入时间 

			__uref Result<bool, unsigned long> GetWrittenPosition(const AudioDevicePipe& pipe) override;   ///返回已写入的数据量

			REFLECT_CLASS(AudioDevice_WaveOut)

		};
	}
}