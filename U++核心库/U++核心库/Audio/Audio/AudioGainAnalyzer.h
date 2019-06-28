#ifndef ALGAINANALYZER_H
#define ALGAINANALYZER_H
#include <math.h>
#include "Config.h"
#define PINK_REF                64.82       /** 298640883795 */ /** calibration value for 89dB */
#define YULE_ORDER         10
#define BUTTER_ORDER        2
#define YULE_FILTER     filterYule
#define BUTTER_FILTER   filterButter
#define RMS_PERCENTILE      0.95 /** percentile which is louder than the proposed level */
#define MAX_SAMP_FREQ   48000L /** maximum allowed sample frequency [Hz] */
#define RMS_WINDOW_TIME_NUMERATOR    1L
#define RMS_WINDOW_TIME_DENOMINATOR 20L /** numerator / denominator = time slice size [s] */
#define STEPS_per_dB      100. /** Table entries per dB */
#define MAX_dB             120. /** Table entries for 0...MAX_dB (normal max. values are 70...80 dB) */
enum { GAIN_NOT_ENOUGH_SAMPLES = -24601, GAIN_ANALYSIS_ERROR = 0, GAIN_ANALYSIS_OK =
            1, INIT_GAIN_ANALYSIS_ERROR = 0, INIT_GAIN_ANALYSIS_OK = 1
    };
    enum { MAX_ORDER = (BUTTER_ORDER > YULE_ORDER ? BUTTER_ORDER : YULE_ORDER)
            , MAX_SAMPLES_PER_WINDOW = ((MAX_SAMP_FREQ * RMS_WINDOW_TIME_NUMERATOR) / RMS_WINDOW_TIME_DENOMINATOR + 1) /** max. Samples per Time slice */
    };

struct ALGainData
{
    float linprebuf[MAX_ORDER * 2];
    float *linpre;     /** left input samples, with pre-buffer */
    float lstepbuf[MAX_SAMPLES_PER_WINDOW + MAX_ORDER];
    float *lstep;      /** left "first step" (i.e. post first filter) samples */
    float loutbuf[MAX_SAMPLES_PER_WINDOW + MAX_ORDER];
    float *lout;       /** left "out" (i.e. post second filter) samples */
    float rinprebuf[MAX_ORDER * 2];
    float *rinpre;     /** right input samples ... */
    float rstepbuf[MAX_SAMPLES_PER_WINDOW + MAX_ORDER];
    float *rstep;
    float routbuf[MAX_SAMPLES_PER_WINDOW + MAX_ORDER];
    float *rout;
    long    sampleWindow; /** number of samples required to reach number of milliseconds required for RMS window */
    long    totsamp;
    double  lsum;
    double  rsum;
    int     freqindex;
    int     first;
    UInt A[(size_t) (STEPS_per_dB * MAX_dB)];
    UInt B[(size_t) (STEPS_per_dB * MAX_dB)];
};

int InitGainAnalysis(ALGainData* Data, long samplefreq);
int AnalyzeSamples(ALGainData* Data, const float * left_samples,const float * right_samples, size_t num_samples, int num_channels);
int ResetSampleFrequency(ALGainData* Data, long samplefreq);
float GetTrackGain(ALGainData* Data);
float GetAlbumGain(ALGainData* Data);

namespace System
{
	namespace Audio
	{
		class AudioResampler;

		typedef void (* TaskCompletedEvent)(intptr_t TaskHandle);
		class ReplayGainTask
		{
		private:
			ALGainData* Data;
			bool KillDecodeThread;
			AudioBuffer* Buffer;
			float* Left;
			float* Right;
			UInt LeftSize;
			UInt RightSize;
			float m_Progress;
			AudioDecoder_base* m_Decoder;
			double TimeMS;
			AudioDecoderPipe m_DecoderHandle;
			SampleInfo m_Info;
			AudioResampler* resampler;

			SampleInfo CurrentInfo;
			TaskCompletedEvent m_Event;
			safe_void_ptr ThreadHandle;
			static ULong _stdcall AnalysisThread(void* Obj);
			ReplayGainTask(const SampleInfo& Info, AudioDecoder_base* Decoder, AudioDecoderPipe DecoderHandle, TaskCompletedEvent Event);
		public:
			Result<ReplayGainTask*> Create(const SampleInfo& Info, AudioDecoder_base* Decoder, AudioDecoderPipe DecoderHandle, TaskCompletedEvent Event);
			~ReplayGainTask();
			float GetProgress() { return m_Progress; }
		};

	}
}

#endif