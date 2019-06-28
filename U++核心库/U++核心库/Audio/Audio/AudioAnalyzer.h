#ifndef AudioAnalyzer_H
#define AudioAnalyzer_H
#include "AudioPool.h"
#include "FFT.h"

namespace System
{
	namespace Audio
	{
#define FFTSIZE_MIN 32
#define FFTSIZE_MAX 8192

#define VUSIZE_MIN 32
#define VUSIZE_MAX 8192

		typedef void (*XWALVisualDataCallBack)(double TimeMs, float* VUData, float** FFTData, float** PCMData, unsigned short VUOutChannel, unsigned int VUOutMeasureLength, unsigned short FFTOutChannel, unsigned int FFTOutMeasureLength, unsigned short PCMChannel, unsigned int PCMLength);
		typedef double (*XWInternalQueryTimeCallBack)();


		class AudioAnalyzer
		{
		private:
			float* Maxpool[CHANNELS_MAX];
			UShort Maxpoolchannel;
			UInt Maxpoolsize;

			XWALVisualDataCallBack VisualCallBack;
			XWInternalQueryTimeCallBack QueryTimeCallBack;
			AudioPool* Pool;
			FastFourier* FFT;
			safe_void_ptr hMutex;
			int KillDecodeThread;
			safe_void_ptr AnalyzerHandle;
			safe_void_ptr Timer;
			static ULong _stdcall AnalyzerThread(void* Obj);

			float* VUPool;

			float* FFTPool_Re[CHANNELS_MAX];
			UShort FFTPool_Re_Ch;
			UInt FFTPool_Re_Size;
			float* FFTPool_Im[CHANNELS_MAX];
			UShort FFTPool_Im_Ch;
			UInt FFTPool_Im_Size;
			float* FFTPool_Power[CHANNELS_MAX];
			UShort FFTPool_Power_Ch;
			UInt FFTPool_Power_Size;

			bool EnsureChannelMaxpool(UShort channel, UInt size);

			bool EnsureFFTChannelMaxpool(UShort channel, UInt size);

			bool EnsureThreadChannelReadMaxpool(UShort channel, UInt size);

			bool EnsureThreadChannelMaxpool(UShort channel, UInt size);

			UInt FPS;
			int NewFPS;

			float* Thread_Maxpool[CHANNELS_MAX];
			UShort Thread_Maxpoolchannel;
			UInt Thread_Maxpoolsize;

			UShort Thread_VUDesiredChannel;
			UInt Thread_VUDesiredMeasureLength;
			UShort Thread_FFTDesiredChannel;
			UInt Thread_FFTDesiredMeasureLength;
			float* Thread_VUPool;

			float* Thread_FFTPool_Re[CHANNELS_MAX];
			UShort Thread_FFTPool_Re_Ch;
			UInt Thread_FFTPool_Re_Size;
			float* Thread_FFTPool_Im[CHANNELS_MAX];
			UShort Thread_FFTPool_Im_Ch;
			UInt Thread_FFTPool_Im_Size;
			float* Thread_FFTPool_Power[CHANNELS_MAX];
			UShort Thread_FFTPool_Power_Ch;
			UInt Thread_FFTPool_Power_Size;
			UInt Thread_FFTWindow;
			bool	Thread_FFTMono;
		public:
			AudioAnalyzer();

			~AudioAnalyzer();

			void Start();

			void SetFPS(unsigned int fps);

			void SetVUDesiredParameter(UInt DesiredChannel, UInt DesiredMeasureLength);

			void SetFFTDesiredParameter(UInt DesiredChannel, UInt DesiredMeasureLength, UInt FFTWindow, bool Mono);

			void SetXWALVisualDataCallBack(XWALVisualDataCallBack callBack);

			void SetQueryTimeCallBack(XWInternalQueryTimeCallBack callBack);

			void Stop();

			void Flush();

			void CollectBins(double timeMs);

			void Write(AudioBuffer* buffer);

			float** GetFFTData(UShort DesiredChannel, UInt DesiredMeasureLength, double TimeMS, UInt FFTWindow, bool Mono, unsigned short& OutChannel, unsigned int& OutMeasureLength);
			float* GetVUData(UShort DesiredChannel, UInt DesiredMeasureLength, double TimeMS, unsigned short& OutChannel, unsigned int& OutMeasureLength);
		};
	}
}



#endif