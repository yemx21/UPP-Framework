#include "AudioGainAnalyzer.h"
#include "AudioResampler.h"
#include "WindowsEnvironment.h"
using namespace System::OS;

static const float ABYule[9][2 * YULE_ORDER + 1] = {
    {0.03857599435200f, -3.84664617118067f, -0.02160367184185f, 7.81501653005538f, -0.00123395316851f,
     -11.34170355132042f, -0.00009291677959f, 13.05504219327545f, -0.01655260341619f,
     -12.28759895145294f, 0.02161526843274f, 9.48293806319790f, -0.02074045215285f, -5.87257861775999f,
     0.00594298065125f, 2.75465861874613f, 0.00306428023191f, -0.86984376593551f, 0.00012025322027f,
     0.13919314567432f, 0.00288463683916f},
    {0.05418656406430f, -3.47845948550071f, -0.02911007808948f, 6.36317777566148f, -0.00848709379851f,
     -8.54751527471874f, -0.00851165645469f, 9.47693607801280f, -0.00834990904936f, -8.81498681370155f,
     0.02245293253339f, 6.85401540936998f, -0.02596338512915f, -4.39470996079559f, 0.01624864962975f,
     2.19611684890774f, -0.00240879051584f, -0.75104302451432f, 0.00674613682247f, 0.13149317958808f,
     -0.00187763777362f},
    {0.15457299681924f, -2.37898834973084f, -0.09331049056315f, 2.84868151156327f, -0.06247880153653f,
     -2.64577170229825f, 0.02163541888798f, 2.23697657451713f, -0.05588393329856f, -1.67148153367602f,
     0.04781476674921f, 1.00595954808547f, 0.00222312597743f, -0.45953458054983f, 0.03174092540049f,
     0.16378164858596f, -0.01390589421898f, -0.05032077717131f, 0.00651420667831f, 0.02347897407020f,
     -0.00881362733839f},
    {0.30296907319327f, -1.61273165137247f, -0.22613988682123f, 1.07977492259970f, -0.08587323730772f,
     -0.25656257754070f, 0.03282930172664f, -0.16276719120440f, -0.00915702933434f, -0.22638893773906f,
     -0.02364141202522f, 0.39120800788284f, -0.00584456039913f, -0.22138138954925f, 0.06276101321749f,
     0.04500235387352f, -0.00000828086748f, 0.02005851806501f, 0.00205861885564f, 0.00302439095741f,
     -0.02950134983287f},
    {0.33642304856132f, -1.49858979367799f, -0.25572241425570f, 0.87350271418188f, -0.11828570177555f,
     0.12205022308084f, 0.11921148675203f, -0.80774944671438f, -0.07834489609479f, 0.47854794562326f,
     -0.00469977914380f, -0.12453458140019f, -0.00589500224440f, -0.04067510197014f, 0.05724228140351f,
     0.08333755284107f, 0.00832043980773f, -0.04237348025746f, -0.01635381384540f, 0.02977207319925f,
     -0.01760176568150f},
    {0.44915256608450f, -0.62820619233671f, -0.14351757464547f, 0.29661783706366f, -0.22784394429749f,
     -0.37256372942400f, -0.01419140100551f, 0.00213767857124f, 0.04078262797139f, -0.42029820170918f,
     -0.12398163381748f, 0.22199650564824f, 0.04097565135648f, 0.00613424350682f, 0.10478503600251f,
     0.06747620744683f, -0.01863887810927f, 0.05784820375801f, -0.03193428438915f, 0.03222754072173f,
     0.00541907748707f},
    {0.56619470757641f, -1.04800335126349f, -0.75464456939302f, 0.29156311971249f, 0.16242137742230f,
     -0.26806001042947f, 0.16744243493672f, 0.00819999645858f, -0.18901604199609f, 0.45054734505008f,
     0.30931782841830f, -0.33032403314006f, -0.27562961986224f, 0.06739368333110f, 0.00647310677246f,
     -0.04784254229033f, 0.08647503780351f, 0.01639907836189f, -0.03788984554840f, 0.01807364323573f,
     -0.00588215443421f},
    {0.58100494960553f, -0.51035327095184f, -0.53174909058578f, -0.31863563325245f, -0.14289799034253f,
     -0.20256413484477f, 0.17520704835522f, 0.14728154134330f, 0.02377945217615f, 0.38952639978999f,
     0.15558449135573f, -0.23313271880868f, -0.25344790059353f, -0.05246019024463f, 0.01628462406333f,
     -0.02505961724053f, 0.06920467763959f, 0.02442357316099f, -0.03721611395801f, 0.01818801111503f,
     -0.00749618797172f},
    {0.53648789255105f, -0.25049871956020f, -0.42163034350696f, -0.43193942311114f, -0.00275953611929f,
     -0.03424681017675f, 0.04267842219415f, -0.04678328784242f, -0.10214864179676f, 0.26408300200955f,
     0.14590772289388f, 0.15113130533216f, -0.02459864859345f, -0.17556493366449f, -0.11202315195388f,
     -0.18823009262115f, -0.04060034127000f, 0.05477720428674f, 0.04788665548180f, 0.04704409688120f,
     -0.02217936801134f}
};
static const float ABButter[9][2 * BUTTER_ORDER + 1] = {
    {0.98621192462708f, -1.97223372919527f, -1.97242384925416f, 0.97261396931306f, 0.98621192462708f},
    {0.98500175787242f, -1.96977855582618f, -1.97000351574484f, 0.97022847566350f, 0.98500175787242f},
    {0.97938932735214f, -1.95835380975398f, -1.95877865470428f, 0.95920349965459f, 0.97938932735214f},
    {0.97531843204928f, -1.95002759149878f, -1.95063686409857f, 0.95124613669835f, 0.97531843204928f},
    {0.97316523498161f, -1.94561023566527f, -1.94633046996323f, 0.94705070426118f, 0.97316523498161f},
    {0.96454515552826f, -1.92783286977036f, -1.92909031105652f, 0.93034775234268f, 0.96454515552826f},
    {0.96009142950541f, -1.91858953033784f, -1.92018285901082f, 0.92177618768381f, 0.96009142950541f},
    {0.95856916599601f, -1.91542108074780f, -1.91713833199203f, 0.91885558323625f, 0.95856916599601f},
    {0.94597685600279f, -1.88903307939452f, -1.89195371200558f, 0.89487434461664f, 0.94597685600279f}
};

static void filterYule(const float * input, float * output, size_t nSamples, const float * const kernel)
{
    /**register double  y; */

    while (nSamples--) {
        *output = 1e-10 /** 1e-10 is a hack to avoid slowdown because of denormals */
            + input[0] * kernel[0]
            - output[-1] * kernel[1]
            + input[-1] * kernel[2]
            - output[-2] * kernel[3]
            + input[-2] * kernel[4]
            - output[-3] * kernel[5]
            + input[-3] * kernel[6]
            - output[-4] * kernel[7]
            + input[-4] * kernel[8]
            - output[-5] * kernel[9]
            + input[-5] * kernel[10]
            - output[-6] * kernel[11]
            + input[-6] * kernel[12]
            - output[-7] * kernel[13]
            + input[-7] * kernel[14]
            - output[-8] * kernel[15]
            + input[-8] * kernel[16]
            - output[-9] * kernel[17]
            + input[-9] * kernel[18]
            - output[-10] * kernel[19]
            + input[-10] * kernel[20];
        ++output;
        ++input;
        /** *output++ = (float)y; */
    }
}

static void filterButter(const float * input, float * output, size_t nSamples, const float * const kernel)
{                       /**register double  y; */

    while (nSamples--) {
        *output = input[0] * kernel[0]
            - output[-1] * kernel[1]
            + input[-1] * kernel[2]
            - output[-2] * kernel[3]
            + input[-2] * kernel[4];
        ++output;
        ++input;
        /** *output++ = (float)y; */
    }
}


/** returns a INIT_GAIN_ANALYSIS_OK if successful, INIT_GAIN_ANALYSIS_ERROR if not */

int ResetSampleFrequency(ALGainData* Data, long samplefreq)
{
    int     i;

    /** zero out initial values */
    for (i = 0; i < MAX_ORDER; i++)
        Data->linprebuf[i] = Data->lstepbuf[i]
            = Data->loutbuf[i]
            = Data->rinprebuf[i]
            = Data->rstepbuf[i]
            = Data->routbuf[i] = 0.;

    switch ((int) (samplefreq)) {
    case 48000:
        Data->freqindex = 0;
        break;
    case 44100:
        Data->freqindex = 1;
        break;
    case 32000:
        Data->freqindex = 2;
        break;
    case 24000:
        Data->freqindex = 3;
        break;
    case 22050:
        Data->freqindex = 4;
        break;
    case 16000:
        Data->freqindex = 5;
        break;
    case 12000:
        Data->freqindex = 6;
        break;
    case 11025:
        Data->freqindex = 7;
        break;
    case 8000:
        Data->freqindex = 8;
        break;
    default:
        return INIT_GAIN_ANALYSIS_ERROR;
    }

    Data->sampleWindow =
        (samplefreq * RMS_WINDOW_TIME_NUMERATOR + RMS_WINDOW_TIME_DENOMINATOR -
         1) / RMS_WINDOW_TIME_DENOMINATOR;

    Data->lsum = 0.;
    Data->rsum = 0.;
    Data->totsamp = 0;

    memset(Data->A, 0, sizeof(Data->A));

    return INIT_GAIN_ANALYSIS_OK;
}

int InitGainAnalysis(ALGainData* Data, long samplefreq)
{
    if (ResetSampleFrequency(Data, samplefreq) != INIT_GAIN_ANALYSIS_OK) {
        return INIT_GAIN_ANALYSIS_ERROR;
    }

    Data->linpre = Data->linprebuf + MAX_ORDER;
    Data->rinpre = Data->rinprebuf + MAX_ORDER;
    Data->lstep = Data->lstepbuf + MAX_ORDER;
    Data->rstep = Data->rstepbuf + MAX_ORDER;
    Data->lout = Data->loutbuf + MAX_ORDER;
    Data->rout = Data->routbuf + MAX_ORDER;

    memset(Data->B, 0, sizeof(Data->B));

    return INIT_GAIN_ANALYSIS_OK;
}

/** returns GAIN_ANALYSIS_OK if successful, GAIN_ANALYSIS_ERROR if not */

static inline double fsqr(const double d)
{
    return d * d;
}

int AnalyzeSamples(ALGainData* Data, const float * left_samples, const float * right_samples, size_t num_samples, int num_channels)
{
    const float *curleft;
    const float *curright;
    long    batchsamples;
    long    cursamples;
    long    cursamplepos;
    int     i;

    if (num_samples == 0)
        return GAIN_ANALYSIS_OK;

    cursamplepos = 0;
    batchsamples = (long) num_samples;

    switch (num_channels) {
    case 1:
        right_samples = left_samples;
        break;
    case 2:
        break;
    default:
        return GAIN_ANALYSIS_ERROR;
    }

    if (num_samples < MAX_ORDER) {
        memcpy(Data->linprebuf + MAX_ORDER, left_samples, num_samples * sizeof(float));
        memcpy(Data->rinprebuf + MAX_ORDER, right_samples, num_samples * sizeof(float));
    }
    else {
        memcpy(Data->linprebuf + MAX_ORDER, left_samples, MAX_ORDER * sizeof(float));
        memcpy(Data->rinprebuf + MAX_ORDER, right_samples, MAX_ORDER * sizeof(float));
    }

    while (batchsamples > 0) {
        cursamples = batchsamples > Data->sampleWindow - Data->totsamp ?
            Data->sampleWindow - Data->totsamp : batchsamples;
        if (cursamplepos < MAX_ORDER) {
            curleft = Data->linpre + cursamplepos;
            curright = Data->rinpre + cursamplepos;
            if (cursamples > MAX_ORDER - cursamplepos)
                cursamples = MAX_ORDER - cursamplepos;
        }
        else {
            curleft = left_samples + cursamplepos;
            curright = right_samples + cursamplepos;
        }

        YULE_FILTER(curleft, Data->lstep + Data->totsamp, cursamples,
                    ABYule[Data->freqindex]);
        YULE_FILTER(curright, Data->rstep + Data->totsamp, cursamples,
                    ABYule[Data->freqindex]);

        BUTTER_FILTER(Data->lstep + Data->totsamp, Data->lout + Data->totsamp, cursamples,
                      ABButter[Data->freqindex]);
        BUTTER_FILTER(Data->rstep + Data->totsamp, Data->rout + Data->totsamp, cursamples,
                      ABButter[Data->freqindex]);

        curleft = Data->lout + Data->totsamp; /** Get the squared values */
        curright = Data->rout + Data->totsamp;

        i = cursamples % 8;
        while (i--) {
            Data->lsum += fsqr(*curleft++);
            Data->rsum += fsqr(*curright++);
        }
        i = cursamples / 8;
        while (i--) {
            Data->lsum += fsqr(curleft[0])
                + fsqr(curleft[1])
                + fsqr(curleft[2])
                + fsqr(curleft[3])
                + fsqr(curleft[4])
                + fsqr(curleft[5])
                + fsqr(curleft[6])
                + fsqr(curleft[7]);
            curleft += 8;
            Data->rsum += fsqr(curright[0])
                + fsqr(curright[1])
                + fsqr(curright[2])
                + fsqr(curright[3])
                + fsqr(curright[4])
                + fsqr(curright[5])
                + fsqr(curright[6])
                + fsqr(curright[7]);
            curright += 8;
        }

        batchsamples -= cursamples;
        cursamplepos += cursamples;
        Data->totsamp += cursamples;
        if (Data->totsamp == Data->sampleWindow) { /** Get the Root Mean Square (RMS) for this set of samples */
            double const val =
                STEPS_per_dB * 10. * log10((Data->lsum + Data->rsum) / Data->totsamp * 0.5 +
                                           1.e-37);
            size_t  ival = (val <= 0) ? 0 : (size_t) val;
            if (ival >= sizeof(Data->A) / sizeof(*(Data->A)))
                ival = sizeof(Data->A) / sizeof(*(Data->A)) - 1;
            Data->A[ival]++;
            Data->lsum = Data->rsum = 0.;
            memmove(Data->loutbuf, Data->loutbuf + Data->totsamp,
                    MAX_ORDER * sizeof(float));
            memmove(Data->routbuf, Data->routbuf + Data->totsamp,
                    MAX_ORDER * sizeof(float));
            memmove(Data->lstepbuf, Data->lstepbuf + Data->totsamp,
                    MAX_ORDER * sizeof(float));
            memmove(Data->rstepbuf, Data->rstepbuf + Data->totsamp,
                    MAX_ORDER * sizeof(float));
            Data->totsamp = 0;
        }
        if (Data->totsamp > Data->sampleWindow) /** somehow I really screwed up: Error in programming! Contact author about totsamp > sampleWindow */
            return GAIN_ANALYSIS_ERROR;
    }
    if (num_samples < MAX_ORDER) {
        memmove(Data->linprebuf, Data->linprebuf + num_samples,
                (MAX_ORDER - num_samples) * sizeof(float));
        memmove(Data->rinprebuf, Data->rinprebuf + num_samples,
                (MAX_ORDER - num_samples) * sizeof(float));
        memcpy(Data->linprebuf + MAX_ORDER - num_samples, left_samples,
               num_samples * sizeof(float));
        memcpy(Data->rinprebuf + MAX_ORDER - num_samples, right_samples,
               num_samples * sizeof(float));
    }
    else {
        memcpy(Data->linprebuf, left_samples + num_samples - MAX_ORDER,
               MAX_ORDER * sizeof(float));
        memcpy(Data->rinprebuf, right_samples + num_samples - MAX_ORDER,
               MAX_ORDER * sizeof(float));
    }

    return GAIN_ANALYSIS_OK;
}


static  float analyzeResult(UInt const *Array, size_t len)
{
    UInt elems;
    int upper;
    size_t  i;

    elems = 0;
    for (i = 0; i < len; i++)
        elems += Array[i];
    if (elems == 0)
        return GAIN_NOT_ENOUGH_SAMPLES;

    upper = (int) ceil(elems * (1. - RMS_PERCENTILE));
    for (i = len; i-- > 0;) {
        if ((upper -= Array[i]) <= 0)
            break;
    }

    return (float) ((float) PINK_REF - (float) i / (float) STEPS_per_dB);
}


float GetTrackGain(ALGainData* Data)
{
    float retval;
    unsigned int i;

    retval = analyzeResult(Data->A, sizeof(Data->A) / sizeof(*(Data->A)));

    for (i = 0; i < sizeof(Data->A) / sizeof(*(Data->A)); i++) {
        Data->B[i] += Data->A[i];
        Data->A[i] = 0;
    }

    for (i = 0; i < MAX_ORDER; i++)
        Data->linprebuf[i] = Data->lstepbuf[i]
            = Data->loutbuf[i]
            = Data->rinprebuf[i]
            = Data->rstepbuf[i]
            = Data->routbuf[i] = 0.f;

    Data->totsamp = 0;
    Data->lsum = Data->rsum = 0.;
    return retval;
}


float GetAlbumGain(ALGainData* Data)
{
    return analyzeResult(Data->B, sizeof(Data->B) / sizeof(*(Data->B)));
}

using namespace System::Audio;

Result<ReplayGainTask*> ReplayGainTask::Create(const SampleInfo& Info, AudioDecoder_base* Decoder, AudioDecoderPipe DecoderHandle, TaskCompletedEvent Event)
{
	Result<ReplayGainTask*> result = Result<ReplayGainTask*>(nullptr);
	try
	{
		result.Value = new ReplayGainTask(Info, Decoder, DecoderHandle, Event);
		return result;
	}
	catch (std::exception err)
	{
		wstring tmp;
		if(Converter::Convert(err.what(), tmp, CultureInfo::Current().CLocale))
			result.WriteMessage(tmp.c_str());
		return result;
	}
	catch (...)
	{
		result.WriteMessage(L"unknown error");
		return result;
	}
}

ReplayGainTask::ReplayGainTask(const SampleInfo& Info, AudioDecoder_base* Decoder, AudioDevicePipe DecoderHandle, TaskCompletedEvent Event) : Left(NULL), Right(NULL), LeftSize(0), RightSize(0)
{
	m_Info=Info;
	if(m_Info.SampleRate>48000) m_Info.SampleRate=48000;
	if (Decoder->IsStream(DecoderHandle)) throw std::exception("stream pipe is not supported");

	AudioInfo ai;

	if (!Decoder->TryGetPipeInfo(DecoderHandle, ai)) throw std::exception("can not get audio info");

	if (!ai.IsValid) throw std::exception("audio info is invalid");

	TimeMS = ai.TimeMs;
	
	resampler = new AudioResampler();

	ResampleConfig SampleConfig;

	SampleConfig.BitsPerSample=BitsPerSample::Float32;
	SampleConfig.ChannelLayout=m_Info.ChannelLayout;
	SampleConfig.EnableBitsPerSample=true;
	SampleConfig.EnableChannels=false;
	SampleConfig.EnableSampleRate=true;
	SampleConfig.SampleRate=m_Info.SampleRate;

	if (!resampler->Config(SampleConfig))
	{
		throw std::exception("resample unsupported");
	}

	m_Decoder=Decoder;
	m_DecoderHandle=DecoderHandle;
	m_Event=Event;

	
	KillDecodeThread=false;
	Data=new ALGainData();

	InitGainAnalysis(Data,m_Info.SampleRate);
	Buffer=new AudioBuffer(m_Info.SampleRate,m_Info.BitsPerSample, m_Info.ChannelLayout);

	ThreadHandle = Microsoft::winapi_CreateThread (NULL, 0, AnalysisThread, this, 0, 0);
	Microsoft::winapi_SuspendThread(*ThreadHandle);
	Microsoft::winapi_SetThreadPriority(*ThreadHandle, API_THREAD_PRIORITY_HIGHEST);
	Microsoft::winapi_ResumeThread(*ThreadHandle);
}
ReplayGainTask::~ReplayGainTask()
{
	if (*ThreadHandle != INVALID_API_PTR)
	{
		KillDecodeThread = true;
		if (Microsoft::winapi_WaitForSingleObject(*ThreadHandle, API_INFINITE) == API_WAIT_TIMEOUT)
			Microsoft::winapi_TerminateThread(*ThreadHandle,0);
		Microsoft::winapi_CloseHandle(*ThreadHandle);
		ThreadHandle = INVALID_API_PTR;
	}
	if(Buffer){delete Buffer; Buffer=NULL;}
	if(Left) {delete Left; Left=NULL;}
	if(Right) {delete Right; Right=NULL;}
	if (resampler){ delete resampler; resampler = nullptr; }
}

ULong ReplayGainTask::AnalysisThread(void* Obj)
{
	ReplayGainTask* Task=(ReplayGainTask*)Obj;
	bool DecodeEnd=false;
	while(!Task->KillDecodeThread)
	{
		if(DecodeEnd)
		{
			if(Task->m_Event)Task->m_Event((intptr_t)Task);
		}
		else
		{
			Result<UInt64, AudioDecoderError> result=Task->m_Decoder->Read(Task->m_DecoderHandle, *Task->Buffer,512);
			size_t bytes=0;
			if (result.Value && (result.State == AudioDecoderError::OK || (result.State == AudioDecoderError::End && bytes)))
			{
				bool direct = false;
				const void* rmem = Task->resampler->Write(Task->Buffer, bytes, direct, Task->CurrentInfo);
				bool needmore = false;
				if (!direct)
				{
					rmem = Task->resampler->Read(bytes, Task->CurrentInfo, needmore);
				}
				if (needmore)
				{
					Task->Buffer->Clear();
					continue;
				}
				if (rmem)
				{
					float * mem = (float*)rmem;
					if (mem && bytes)
					{
						UInt count = bytes / Task->CurrentInfo.BlockAlign;
						if (Task->Left)
						{
							if (Task->LeftSize < bytes)
							{
								float* oldmem = Task->Left;
								if ((Task->Left = (float*)realloc(Task->Left, bytes)) == NULL)
								{
									Task->Left = (float*)malloc(bytes);
									if (!Task->Left)
									{
										free(oldmem);
										throw 32;
									}
									free(oldmem);
								}
							}
						}
						if (Task->Right)
						{
							if (Task->RightSize < bytes)
							{
								float* oldmem = Task->Right;
								if ((Task->Right = (float*)realloc(Task->Right, bytes)) == NULL)
								{
									Task->Right = (float*)malloc(bytes);
									if (!Task->Right)
									{
										free(oldmem);
										throw 32;
									}
									free(oldmem);
								}
							}
						}
						UShort ch = Task->CurrentInfo.Channels;
						if (ch == 1)
						{
							for (int i = 0; i < count; i++)
							{
								Task->Left[i] = mem[i*ch];
							}
							AnalyzeSamples(Task->Data, Task->Left, Task->Right, count, 1);
						}
						else
						{
							for (int i = 0; i < count; i++)
							{
								UInt index = i*ch;
								Task->Left[i] = mem[index];
								Task->Right[i] = mem[index + 1];
							}
							AnalyzeSamples(Task->Data, Task->Left, Task->Right, count, 2);
						}
					}
				}
			}
			else
			{
				DecodeEnd=1;
			}				
			Task->m_Progress=Task->m_Decoder->GetCurrentMS(Task->m_DecoderHandle)/Task->TimeMS;
		}
	}
	return 0;
}