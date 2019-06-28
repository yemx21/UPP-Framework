#pragma once
#include "Config.h"
#include "PluginCenter.h"
#include "StandardDSP.h"
#include "AudioAnalyzer.h"
#include "AudioLimiter.h"
#include "DSPManager.h"
#include "AudioTimeEQ.h"
#include "AudioResampler.h"
#include "ID128.h"
#include <process.h>
enum class AudioTreeError
{
	None,
	Failed,
	InvalidPath,
	InvalidCenter,
	Invalid,
	AlreadyOnThread,
	Unsupported,
	UnsupportedSampleRate,
	UnsupportedBitsPerSample,
	UnsupportedChannel,
	UnsupportedStream,
	UnsupportedInfo,
	Unknown
};

namespace System
{
	namespace Audio
	{
		class AudioTree: public ObjectManager::ID128
		{
		private:

			friend class PluginCenter;
			int m_StopParam;

			AudioBuffer* Buffer;
			AudioDecoder_base* Decoder;
			AudioDevice_base* Device;
			AudioVisual_base* Visual;

			UInt64 m_BufferTime;
			PluginCenter* Center;
			UInt128 DecoderID;
			UInt128 DeviceID;
			UInt128 VisualID;

			AudioDecoderPipe Decoder_ptr;
			AudioDevicePipe Device_ptr;
			AudioVisualPipe Visual_ptr;
			intptr_t DSPMgr_ptr;
			Player* Host;

			int KillDecodeThread;
			safe_void_ptr DecodeHandle;
			static unsigned int _stdcall DecodeThread(void* Obj);
			double SeekNeeded;
			AudioAnalyzer* m_StdSA;
			DSPManager* m_DSPMg;
			AudioResampler* BackResampler;
			AudioResampler* RenderResampler;
			SampleInfo CurrentInfo;
			SampleInfo BeforeRenderInfo;
			SampleInfo LastInfo;

			safe_void_ptr Operation_Decoder;
			safe_void_ptr Operation_Device;

			const wchar_t* Operation_Path;

			Result<bool, AudioTreeError, CoreSubObjects, int> Operation_OpenResult;

			//safe_void_ptr Operation_Visual;

			std::mutex Disposing_Pipes;

			bool IsPlaying;
		public:
			bool IsThreadActive;
			bool Active;
			AudioInfo Info;

			ResampleConfig RenderReSampleSetting;
			ResampleConfig BackResampleSetting;
			bool BackResampleRefresh;
			SampleInfo RenderInfo;
			bool RenderReSample;
			UInt64 RenderBufferTime;

			AudioTimeEQ* StdTimeEQ;
			AudioSTDDSP* StdDSP;
			AudioLimiter* StdLimiter;
			AudioTree(Player* PlayerHost, const UInt128& D, const UInt128& R, PluginCenter* Plugins, DSPManager* DSPMgr, AudioAnalyzer* StdSA, UInt64 BufferTime);
			~AudioTree();
			Result<bool, AudioTreeError, CoreSubObjects, int> Open(const wchar_t* Path, int Timeout);
			Result<bool, AudioTreeError, CoreSubObjects, int> BindVisual(const UInt128& V, safe_void_ptr Owner);

			bool SetReSampleConfig(const ResampleConfig& reSampleSetting);
			bool Play();
			bool Pause();
			bool Stop(int Message);
			bool SetCurrent(double PosMS);
			double GetCurrent();
			AudioInfo GetInfo() { return Info; }
			void SetVolume(double Volume);
			void SetPan(double Pan);
		};

	}
}
