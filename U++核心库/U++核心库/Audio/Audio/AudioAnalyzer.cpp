#include "AudioAnalyzer.h"
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;


bool AudioAnalyzer::EnsureChannelMaxpool(UShort channel, UInt size)
{
	UShort ch = 0;
	if (channel > Maxpoolchannel)
	{
		if (size > Maxpoolsize)
		{
			if (Maxpoolchannel)
			{
				for (ch = 0; ch < Maxpoolchannel; ch++)
				{
					float* oldpool = Maxpool[ch];
					if ((Maxpool[ch] = (float*)realloc(Maxpool[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Maxpool[ch] = (float*)malloc(size * sizeof(float));
						if (!Maxpool[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Maxpool[i]) { free(Maxpool[i]); Maxpool[i] = NULL; }
							}
							Maxpoolsize = 0;
							Maxpoolchannel = 0;
							return false;
						}
					}
				}
			}
			for (ch = Maxpoolchannel; ch < channel; ch++)
			{
				Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Maxpoolchannel; ch < channel; ch++)
			{
				Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Maxpoolsize = size;
		Maxpoolchannel = channel;
	}
	return true;
}

bool AudioAnalyzer::EnsureFFTChannelMaxpool(UShort channel, UInt size)
{
	UShort ch = 0;
	if (channel > Maxpoolchannel)
	{
		if (size > Maxpoolsize)
		{
			if (Maxpoolchannel)
			{
				for (ch = 0; ch < Maxpoolchannel; ch++)
				{
					float* oldpool = Maxpool[ch];
					if ((Maxpool[ch] = (float*)realloc(Maxpool[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Maxpool[ch] = (float*)malloc(size * sizeof(float));
						if (!Maxpool[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Maxpool[i]) { free(Maxpool[i]); Maxpool[i] = NULL; }
							}
							Maxpoolsize = 0;
							Maxpoolchannel = 0;
							return false;
						}
					}
				}
			}
			for (ch = Maxpoolchannel; ch < channel; ch++)
			{
				Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Maxpoolchannel; ch < channel; ch++)
			{
				Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Maxpoolsize = size;
		Maxpoolchannel = channel;
	}

	if (channel > FFTPool_Re_Ch)
	{
		if (size > FFTPool_Re_Size)
		{
			if (FFTPool_Re_Ch)
			{
				for (ch = 0; ch < FFTPool_Re_Ch; ch++)
				{
					float* oldpool = FFTPool_Re[ch];
					if ((FFTPool_Re[ch] = (float*)realloc(FFTPool_Re[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						FFTPool_Re[ch] = (float*)malloc(size * sizeof(float));
						if (!FFTPool_Re[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (FFTPool_Re[i]) { free(FFTPool_Re[i]); FFTPool_Re[i] = NULL; }
							}
							FFTPool_Re_Size = 0;
							FFTPool_Re_Ch = 0;
							return false;
						}
					}
				}
			}
			for (ch = FFTPool_Re_Ch; ch < channel; ch++)
			{
				FFTPool_Re[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = FFTPool_Re_Ch; ch < channel; ch++)
			{
				FFTPool_Re[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		FFTPool_Re_Size = size;
		FFTPool_Re_Ch = channel;
	}

	if (channel > FFTPool_Im_Ch)
	{
		if (size > FFTPool_Im_Size)
		{
			if (FFTPool_Im_Ch)
			{
				for (ch = 0; ch < FFTPool_Im_Ch; ch++)
				{
					float* oldpool = FFTPool_Im[ch];
					if ((FFTPool_Im[ch] = (float*)realloc(FFTPool_Im[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						FFTPool_Im[ch] = (float*)malloc(size * sizeof(float));
						if (!FFTPool_Im[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (FFTPool_Im[i]) { free(FFTPool_Im[i]); FFTPool_Im[i] = NULL; }
							}
							FFTPool_Im_Size = 0;
							FFTPool_Im_Ch = 0;
							return false;
						}
					}
				}
			}
			for (ch = FFTPool_Im_Ch; ch < channel; ch++)
			{
				FFTPool_Im[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = FFTPool_Im_Ch; ch < channel; ch++)
			{
				FFTPool_Im[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		FFTPool_Im_Size = size;
		FFTPool_Im_Ch = channel;
	}

	if (channel > FFTPool_Power_Ch)
	{
		if (size > FFTPool_Power_Size)
		{
			if (FFTPool_Power_Ch)
			{
				for (ch = 0; ch < FFTPool_Power_Ch; ch++)
				{
					float* oldpool = FFTPool_Power[ch];
					if ((FFTPool_Power[ch] = (float*)realloc(FFTPool_Power[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						FFTPool_Power[ch] = (float*)malloc(size * sizeof(float));
						if (!FFTPool_Power[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (FFTPool_Power[i]) { free(FFTPool_Power[i]); FFTPool_Power[i] = NULL; }
							}
							FFTPool_Power_Size = 0;
							FFTPool_Power_Ch = 0;
							return false;
						}
					}
				}
			}
			for (ch = FFTPool_Power_Ch; ch < channel; ch++)
			{
				FFTPool_Power[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = FFTPool_Power_Ch; ch < channel; ch++)
			{
				FFTPool_Power[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		FFTPool_Power_Size = size;
		FFTPool_Power_Ch = channel;
	}
	return true;
}

bool AudioAnalyzer::EnsureThreadChannelReadMaxpool(UShort channel, UInt size)
{
	UShort ch = 0;
	if (channel > Thread_Maxpoolchannel)
	{
		if (size > Thread_Maxpoolsize)
		{
			if (Thread_Maxpoolchannel)
			{
				for (ch = 0; ch < Thread_Maxpoolchannel; ch++)
				{
					float* oldpool = Thread_Maxpool[ch];
					if ((Thread_Maxpool[ch] = (float*)realloc(Thread_Maxpool[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Thread_Maxpool[ch] = (float*)malloc(size * sizeof(float));
						if (!Thread_Maxpool[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Thread_Maxpool[i]) { free(Thread_Maxpool[i]); Thread_Maxpool[i] = NULL; }
							}
							Thread_Maxpoolsize = 0;
							Thread_Maxpoolchannel = 0;
							return false;
						}
					}
				}
			}
			for (ch = Thread_Maxpoolchannel; ch < channel; ch++)
			{
				Thread_Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Thread_Maxpoolchannel; ch < channel; ch++)
			{
				Thread_Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Thread_Maxpoolsize = size;
		Thread_Maxpoolchannel = channel;
	}
	return true;
}

bool AudioAnalyzer::EnsureThreadChannelMaxpool(UShort channel, UInt size)
{
	UShort ch = 0;
	if (channel > Thread_Maxpoolchannel)
	{
		if (size > Thread_Maxpoolsize)
		{
			if (Thread_Maxpoolchannel)
			{
				for (ch = 0; ch < Thread_Maxpoolchannel; ch++)
				{
					float* oldpool = Thread_Maxpool[ch];
					if ((Thread_Maxpool[ch] = (float*)realloc(Thread_Maxpool[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Thread_Maxpool[ch] = (float*)malloc(size * sizeof(float));
						if (!Thread_Maxpool[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Thread_Maxpool[i]) { free(Thread_Maxpool[i]); Thread_Maxpool[i] = NULL; }
							}
							Thread_Maxpoolsize = 0;
							Thread_Maxpoolchannel = 0;
							return false;
						}
					}
				}
			}
			for (ch = Thread_Maxpoolchannel; ch < channel; ch++)
			{
				Thread_Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Thread_Maxpoolchannel; ch < channel; ch++)
			{
				Thread_Maxpool[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Thread_Maxpoolsize = size;
		Thread_Maxpoolchannel = channel;
	}

	if (channel > Thread_FFTPool_Re_Ch)
	{
		if (size > Thread_FFTPool_Re_Size)
		{
			if (Thread_FFTPool_Re_Ch)
			{
				for (ch = 0; ch < Thread_FFTPool_Re_Ch; ch++)
				{
					float* oldpool = Thread_FFTPool_Re[ch];
					if ((Thread_FFTPool_Re[ch] = (float*)realloc(Thread_FFTPool_Re[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Thread_FFTPool_Re[ch] = (float*)malloc(size * sizeof(float));
						if (!Thread_FFTPool_Re[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Thread_FFTPool_Re[i]) { free(Thread_FFTPool_Re[i]); Thread_FFTPool_Re[i] = NULL; }
							}
							Thread_FFTPool_Re_Size = 0;
							Thread_FFTPool_Re_Ch = 0;
							return false;
						}
					}
				}
			}
			for (ch = Thread_FFTPool_Re_Ch; ch < channel; ch++)
			{
				Thread_FFTPool_Re[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Thread_FFTPool_Re_Ch; ch < channel; ch++)
			{
				Thread_FFTPool_Re[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Thread_FFTPool_Re_Size = size;
		Thread_FFTPool_Re_Ch = channel;
	}

	if (channel > Thread_FFTPool_Im_Ch)
	{
		if (size > Thread_FFTPool_Im_Size)
		{
			if (Thread_FFTPool_Im_Ch)
			{
				for (ch = 0; ch < Thread_FFTPool_Im_Ch; ch++)
				{
					float* oldpool = Thread_FFTPool_Im[ch];
					if ((Thread_FFTPool_Im[ch] = (float*)realloc(Thread_FFTPool_Im[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Thread_FFTPool_Im[ch] = (float*)malloc(size * sizeof(float));
						if (!Thread_FFTPool_Im[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Thread_FFTPool_Im[i]) { free(Thread_FFTPool_Im[i]); Thread_FFTPool_Im[i] = NULL; }
							}
							Thread_FFTPool_Im_Size = 0;
							Thread_FFTPool_Im_Ch = 0;
							return false;
						}
					}
				}
			}
			for (ch = Thread_FFTPool_Im_Ch; ch < channel; ch++)
			{
				Thread_FFTPool_Im[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Thread_FFTPool_Im_Ch; ch < channel; ch++)
			{
				Thread_FFTPool_Im[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Thread_FFTPool_Im_Size = size;
		Thread_FFTPool_Im_Ch = channel;
	}

	if (channel > Thread_FFTPool_Power_Ch)
	{
		if (size > Thread_FFTPool_Power_Size)
		{
			if (Thread_FFTPool_Power_Ch)
			{
				for (ch = 0; ch < Thread_FFTPool_Power_Ch; ch++)
				{
					float* oldpool = Thread_FFTPool_Power[ch];
					if ((Thread_FFTPool_Power[ch] = (float*)realloc(Thread_FFTPool_Power[ch], size * sizeof(float))) == 0)
					{
						free(oldpool);
						Thread_FFTPool_Power[ch] = (float*)malloc(size * sizeof(float));
						if (!Thread_FFTPool_Power[ch])
						{
							for (UShort i = 0; i < CHANNELS_MAX; i++)
							{
								if (Thread_FFTPool_Power[i]) { free(Thread_FFTPool_Power[i]); Thread_FFTPool_Power[i] = NULL; }
							}
							Thread_FFTPool_Power_Size = 0;
							Thread_FFTPool_Power_Ch = 0;
							return false;
						}
					}
				}
			}
			for (ch = Thread_FFTPool_Power_Ch; ch < channel; ch++)
			{
				Thread_FFTPool_Power[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		else
		{
			for (ch = Thread_FFTPool_Power_Ch; ch < channel; ch++)
			{
				Thread_FFTPool_Power[ch] = (float*)malloc(size * sizeof(float));
			}
		}
		Thread_FFTPool_Power_Size = size;
		Thread_FFTPool_Power_Ch = channel;
	}
	return true;
}

AudioAnalyzer::AudioAnalyzer() : FFTPool_Re_Size(0), FFTPool_Im_Size(0), FFTPool_Power_Size(0), Maxpoolchannel(0), Maxpoolsize(0), Timer(NULL), FPS(40), Thread_FFTPool_Re_Size(0), Thread_FFTPool_Im_Size(0), Thread_FFTPool_Power_Size(0), Thread_Maxpoolchannel(0), Thread_Maxpoolsize(0), NewFPS(-1), Thread_FFTWindow(0), Thread_FFTMono(true), Thread_FFTPool_Re_Ch(0), Thread_FFTPool_Im_Ch(0), Thread_FFTPool_Power_Ch(0), FFTPool_Re_Ch(0), FFTPool_Im_Ch(0), FFTPool_Power_Ch(0), VisualCallBack(0), QueryTimeCallBack(0), KillDecodeThread(0)
{
	FFT = new FastFourier();
	Pool = new AudioPool();

	Maxpool[0] = NULL;
	Maxpool[1] = NULL;
	Maxpool[2] = NULL;
	Maxpool[3] = NULL;
	Maxpool[4] = NULL;
	Maxpool[5] = NULL;
	Maxpool[6] = NULL;
	Maxpool[7] = NULL;
	Maxpool[8] = NULL;
	Maxpool[9] = NULL;
	Maxpool[10] = NULL;
	Maxpool[11] = NULL;
	Maxpool[12] = NULL;
	Maxpool[13] = NULL;
	Maxpool[14] = NULL;
	Maxpool[15] = NULL;
	Maxpool[16] = NULL;
	Maxpool[17] = NULL;

	VUPool = new float[CHANNELS_MAX];
	VUPool[0] = 0.0f;
	VUPool[1] = 0.0f;
	VUPool[2] = 0.0f;
	VUPool[3] = 0.0f;
	VUPool[4] = 0.0f;
	VUPool[5] = 0.0f;
	VUPool[6] = 0.0f;
	VUPool[7] = 0.0f;
	VUPool[8] = 0.0f;
	VUPool[9] = 0.0f;
	VUPool[10] = 0.0f;
	VUPool[11] = 0.0f;
	VUPool[12] = 0.0f;
	VUPool[13] = 0.0f;
	VUPool[14] = 0.0f;
	VUPool[15] = 0.0f;
	VUPool[16] = 0.0f;
	VUPool[17] = 0.0f;

	FFTPool_Re[0] = NULL;
	FFTPool_Re[1] = NULL;
	FFTPool_Re[2] = NULL;
	FFTPool_Re[3] = NULL;
	FFTPool_Re[4] = NULL;
	FFTPool_Re[5] = NULL;
	FFTPool_Re[6] = NULL;
	FFTPool_Re[7] = NULL;
	FFTPool_Re[8] = NULL;
	FFTPool_Re[9] = NULL;
	FFTPool_Re[10] = NULL;
	FFTPool_Re[11] = NULL;
	FFTPool_Re[12] = NULL;
	FFTPool_Re[13] = NULL;
	FFTPool_Re[14] = NULL;
	FFTPool_Re[15] = NULL;
	FFTPool_Re[16] = NULL;
	FFTPool_Re[17] = NULL;

	FFTPool_Im[0] = NULL;
	FFTPool_Im[1] = NULL;
	FFTPool_Im[2] = NULL;
	FFTPool_Im[3] = NULL;
	FFTPool_Im[4] = NULL;
	FFTPool_Im[5] = NULL;
	FFTPool_Im[6] = NULL;
	FFTPool_Im[7] = NULL;
	FFTPool_Im[8] = NULL;
	FFTPool_Im[9] = NULL;
	FFTPool_Im[10] = NULL;
	FFTPool_Im[11] = NULL;
	FFTPool_Im[12] = NULL;
	FFTPool_Im[13] = NULL;
	FFTPool_Im[14] = NULL;
	FFTPool_Im[15] = NULL;
	FFTPool_Im[16] = NULL;
	FFTPool_Im[17] = NULL;

	FFTPool_Power[0] = NULL;
	FFTPool_Power[1] = NULL;
	FFTPool_Power[2] = NULL;
	FFTPool_Power[3] = NULL;
	FFTPool_Power[4] = NULL;
	FFTPool_Power[5] = NULL;
	FFTPool_Power[6] = NULL;
	FFTPool_Power[7] = NULL;
	FFTPool_Power[8] = NULL;
	FFTPool_Power[9] = NULL;
	FFTPool_Power[10] = NULL;
	FFTPool_Power[11] = NULL;
	FFTPool_Power[12] = NULL;
	FFTPool_Power[13] = NULL;
	FFTPool_Power[14] = NULL;
	FFTPool_Power[15] = NULL;
	FFTPool_Power[16] = NULL;
	FFTPool_Power[17] = NULL;

	Thread_Maxpool[0] = NULL;
	Thread_Maxpool[1] = NULL;
	Thread_Maxpool[2] = NULL;
	Thread_Maxpool[3] = NULL;
	Thread_Maxpool[4] = NULL;
	Thread_Maxpool[5] = NULL;
	Thread_Maxpool[6] = NULL;
	Thread_Maxpool[7] = NULL;
	Thread_Maxpool[8] = NULL;
	Thread_Maxpool[9] = NULL;
	Thread_Maxpool[10] = NULL;
	Thread_Maxpool[11] = NULL;
	Thread_Maxpool[12] = NULL;
	Thread_Maxpool[13] = NULL;
	Thread_Maxpool[14] = NULL;
	Thread_Maxpool[15] = NULL;
	Thread_Maxpool[16] = NULL;
	Thread_Maxpool[17] = NULL;

	Thread_VUPool = new float[CHANNELS_MAX];
	Thread_VUPool[0] = 0.0f;
	Thread_VUPool[1] = 0.0f;
	Thread_VUPool[2] = 0.0f;
	Thread_VUPool[3] = 0.0f;
	Thread_VUPool[4] = 0.0f;
	Thread_VUPool[5] = 0.0f;
	Thread_VUPool[6] = 0.0f;
	Thread_VUPool[7] = 0.0f;
	Thread_VUPool[8] = 0.0f;
	Thread_VUPool[9] = 0.0f;
	Thread_VUPool[10] = 0.0f;
	Thread_VUPool[11] = 0.0f;
	Thread_VUPool[12] = 0.0f;
	Thread_VUPool[13] = 0.0f;
	Thread_VUPool[14] = 0.0f;
	Thread_VUPool[15] = 0.0f;
	Thread_VUPool[16] = 0.0f;
	Thread_VUPool[17] = 0.0f;

	Thread_FFTPool_Re[0] = NULL;
	Thread_FFTPool_Re[1] = NULL;
	Thread_FFTPool_Re[2] = NULL;
	Thread_FFTPool_Re[3] = NULL;
	Thread_FFTPool_Re[4] = NULL;
	Thread_FFTPool_Re[5] = NULL;
	Thread_FFTPool_Re[6] = NULL;
	Thread_FFTPool_Re[7] = NULL;
	Thread_FFTPool_Re[8] = NULL;
	Thread_FFTPool_Re[9] = NULL;
	Thread_FFTPool_Re[10] = NULL;
	Thread_FFTPool_Re[11] = NULL;
	Thread_FFTPool_Re[12] = NULL;
	Thread_FFTPool_Re[13] = NULL;
	Thread_FFTPool_Re[14] = NULL;
	Thread_FFTPool_Re[15] = NULL;
	Thread_FFTPool_Re[16] = NULL;
	Thread_FFTPool_Re[17] = NULL;

	Thread_FFTPool_Im[0] = NULL;
	Thread_FFTPool_Im[1] = NULL;
	Thread_FFTPool_Im[2] = NULL;
	Thread_FFTPool_Im[3] = NULL;
	Thread_FFTPool_Im[4] = NULL;
	Thread_FFTPool_Im[5] = NULL;
	Thread_FFTPool_Im[6] = NULL;
	Thread_FFTPool_Im[7] = NULL;
	Thread_FFTPool_Im[8] = NULL;
	Thread_FFTPool_Im[9] = NULL;
	Thread_FFTPool_Im[10] = NULL;
	Thread_FFTPool_Im[11] = NULL;
	Thread_FFTPool_Im[12] = NULL;
	Thread_FFTPool_Im[13] = NULL;
	Thread_FFTPool_Im[14] = NULL;
	Thread_FFTPool_Im[15] = NULL;
	Thread_FFTPool_Im[16] = NULL;
	Thread_FFTPool_Im[17] = NULL;

	Thread_FFTPool_Power[0] = NULL;
	Thread_FFTPool_Power[1] = NULL;
	Thread_FFTPool_Power[2] = NULL;
	Thread_FFTPool_Power[3] = NULL;
	Thread_FFTPool_Power[4] = NULL;
	Thread_FFTPool_Power[5] = NULL;
	Thread_FFTPool_Power[6] = NULL;
	Thread_FFTPool_Power[7] = NULL;
	Thread_FFTPool_Power[8] = NULL;
	Thread_FFTPool_Power[9] = NULL;
	Thread_FFTPool_Power[10] = NULL;
	Thread_FFTPool_Power[11] = NULL;
	Thread_FFTPool_Power[12] = NULL;
	Thread_FFTPool_Power[13] = NULL;
	Thread_FFTPool_Power[14] = NULL;
	Thread_FFTPool_Power[15] = NULL;
	Thread_FFTPool_Power[16] = NULL;
	Thread_FFTPool_Power[17] = NULL;

	Thread_FFTDesiredChannel = 2;
	Thread_FFTDesiredMeasureLength = 4096;

	Thread_VUDesiredChannel = 2;
	Thread_VUDesiredMeasureLength = 4096;

	hMutex = Microsoft::winapi_CreateMutex(NULL, false, NULL);
	AnalyzerHandle = Microsoft::winapi_CreateThread(NULL, 0, AnalyzerThread, (void *)this, 0, 0);
	Microsoft::winapi_SuspendThread(*AnalyzerHandle);
	Microsoft::winapi_SetThreadPriority(*AnalyzerHandle, API_THREAD_PRIORITY_HIGHEST);
}

AudioAnalyzer::~AudioAnalyzer()
{
	delete Pool; Pool = NULL;
	delete VUPool; VUPool = NULL;
	delete FFT; FFT = NULL;
	Microsoft::winapi_ReleaseMutex(*hMutex);
	Microsoft::winapi_CloseHandle(*hMutex);
}

void AudioAnalyzer::Start()
{

}

void AudioAnalyzer::SetFPS(unsigned int fps)
{
	if (fps > 1000) fps = 1000;
	NewFPS = fps;
}

void AudioAnalyzer::SetVUDesiredParameter(UInt DesiredChannel, UInt DesiredMeasureLength)
{
	Thread_VUDesiredChannel = DesiredChannel;
	Thread_VUDesiredMeasureLength = DesiredMeasureLength;
}

void AudioAnalyzer::SetFFTDesiredParameter(UInt DesiredChannel, UInt DesiredMeasureLength, UInt FFTWindow, bool Mono)
{
	Thread_FFTDesiredChannel = DesiredChannel;
	Thread_FFTDesiredMeasureLength = DesiredMeasureLength;
	Thread_FFTWindow = FFTWindow;
	Thread_FFTMono = Mono;
}

void AudioAnalyzer::SetXWALVisualDataCallBack(XWALVisualDataCallBack callBack)
{
	VisualCallBack = callBack;
}

void AudioAnalyzer::SetQueryTimeCallBack(XWInternalQueryTimeCallBack callBack)
{
	QueryTimeCallBack = callBack;
}

void AudioAnalyzer::Stop()
{

}

void AudioAnalyzer::Flush()
{
	Pool->Flush();
}

void AudioAnalyzer::CollectBins(double timeMs)
{
	Pool->CollectBins(timeMs);
}

void AudioAnalyzer::Write(AudioBuffer* buffer)
{
	//Pool->Write(buffer);
}




float* AudioAnalyzer::GetVUData(UShort DesiredChannel, UInt DesiredMeasureLength, double TimeMS, unsigned short& OutChannel, unsigned int& OutMeasureLength)
{
	OutChannel=0;
	OutMeasureLength=0;
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	if(Pool->MaxChannel) DesiredChannel=Pool->MaxChannel;
	if(!DesiredChannel) return NULL;
	if(!EnsureChannelMaxpool(DesiredChannel, DesiredMeasureLength)) return NULL;
	float maxsr=0.0f;
	UInt count=Pool->Read(Maxpool, DesiredChannel, DesiredMeasureLength, TimeMS, &maxsr);
	if(count==0 || maxsr==0.0f) 
	{
		OutMeasureLength=0;
		OutChannel=0;
		return NULL;
	}
	OutMeasureLength=count;
	OutChannel=DesiredChannel;	
	for(UShort ch=0; ch<DesiredChannel; ch++)
	{
		float* cch=Maxpool[ch];
		float csum=0.0f;
		UInt tc=count;
		while(tc--)
		{
			float input=fabs(*cch);
			csum+=input;
			cch++;
		}		
		VUPool[ch] = csum / (float)count;
	}
	Microsoft::winapi_ReleaseMutex(*hMutex);
	return VUPool;
}

float** AudioAnalyzer::GetFFTData(UShort DesiredChannel, UInt DesiredMeasureLength, double TimeMS, UInt FFTWindow, bool Mono, unsigned short& OutChannel, unsigned int& OutMeasureLength)
{
	OutChannel=0;
	OutMeasureLength=0;
	if(DesiredMeasureLength<FFTSIZE_MIN || DesiredMeasureLength >FFTSIZE_MAX) return NULL;
	if((DesiredMeasureLength& (DesiredMeasureLength-1))!=0) return NULL; //判断是否为2的倍数
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	if(Pool->MaxChannel) DesiredChannel=Pool->MaxChannel;
	if(!DesiredChannel) return NULL;
	if(!EnsureFFTChannelMaxpool(DesiredChannel, DesiredMeasureLength)) return NULL;
	UInt count=Pool->Read(Maxpool, DesiredChannel, DesiredMeasureLength, TimeMS, NULL);
	if(count==0)
	{
		return NULL;
	}
	OutMeasureLength=count /2 ;
	OutChannel= Mono? 1: DesiredChannel;	

	for(UInt ch=0;ch<DesiredChannel;ch++)
	{
		memset(FFTPool_Re[ch], 0, FFTPool_Re_Size* sizeof(float));
		memset(FFTPool_Im[ch], 0, FFTPool_Im_Size* sizeof(float));
		memset(FFTPool_Power[ch], 0, FFTPool_Power_Size* sizeof(float));
	}
	
	FFT->do_fft_direct(FFTWindow,count, Mono, DesiredChannel, Maxpool, FFTPool_Re, FFTPool_Im, FFTPool_Power);
	Microsoft::winapi_ReleaseMutex(*hMutex);

	return FFTPool_Power;
}

ULong AudioAnalyzer::AnalyzerThread(void* Obj)
{
	AudioAnalyzer* me=static_cast<AudioAnalyzer*>(Obj);
	if(me)
	{
		API_INT64 li = 0;
		me->Timer = Microsoft::winapi_CreateWaitableTimer(NULL, false, NULL);
		Microsoft::winapi_SetWaitableTimer(*me->Timer, li, 1000/me->FPS, NULL, NULL, false);
		while(me->KillDecodeThread==0)
		{
			if(me->FPS)
			{
				Microsoft::winapi_WaitForSingleObject(*me->Timer, API_INFINITE);
				if(me->NewFPS>-1)
				{
					if(me->NewFPS>0) Microsoft::winapi_SetWaitableTimer(*me->Timer, li, 1000/me->NewFPS,NULL, NULL, false);
					me->FPS=me->NewFPS;
					me->NewFPS=-1;
				}
				if(me->VisualCallBack && me->QueryTimeCallBack)
				{
					double current=me->QueryTimeCallBack();
					if(current<0.0) current=0.0;
					UShort VUDesiredChannel=me->Thread_VUDesiredChannel;
					UInt VUDesiredMeasureLength =me ->Thread_VUDesiredMeasureLength ;
					if(me->Pool->MaxChannel) VUDesiredChannel=me->Pool->MaxChannel;
					if(!VUDesiredChannel) continue;
					if(!me->EnsureThreadChannelMaxpool(VUDesiredChannel, VUDesiredMeasureLength)) continue;
					UInt count=me->Pool->Read(me->Thread_Maxpool, VUDesiredChannel, VUDesiredMeasureLength, current, NULL);
					UInt OutVUMeasureLength=count;
					UShort OutVUChannel=VUDesiredChannel;	
					for(UShort ch=0; ch<VUDesiredChannel; ch++)
					{
						float* cch=me->Thread_Maxpool[ch];
						float cmax=0.0f;
						while(count--)
						{
							float cdata=fabsf(*cch);
							if(cdata> cmax) cmax=cdata;
							cch++;
						}
						me->Thread_VUPool[ch]=cmax;
					}
					UShort FFTDesiredChannel=me->Thread_FFTDesiredChannel;
					UInt FFTDesiredMeasureLength =me ->Thread_FFTDesiredMeasureLength ;
					if(FFTDesiredMeasureLength<FFTSIZE_MIN || FFTDesiredMeasureLength >FFTSIZE_MAX) continue;
					if((FFTDesiredMeasureLength& (FFTDesiredMeasureLength-1))!=0) continue;//判断是否为2的倍数
					if(me->Pool->MaxChannel) FFTDesiredChannel=me->Pool->MaxChannel;
					if(!FFTDesiredChannel) continue;
					if(!me->EnsureThreadChannelMaxpool(FFTDesiredChannel, FFTDesiredMeasureLength)) continue;
					count=me->Pool->Read(me->Thread_Maxpool, FFTDesiredChannel, FFTDesiredMeasureLength, current, NULL);
					UInt OutFFTMeasureLength=count /2 ;
					bool mono=me->Thread_FFTMono;
					UShort OutFFTChannel= mono? 1: FFTDesiredChannel;	
					me->FFT->do_fft_direct(me->Thread_FFTWindow,count, mono, FFTDesiredChannel, me->Thread_Maxpool, me->Thread_FFTPool_Re, me->Thread_FFTPool_Im, me->Thread_FFTPool_Power);
					
					UInt maxchannel=me->Pool->MaxChannel;
					UInt maxlength=OutVUMeasureLength< OutFFTMeasureLength? OutFFTMeasureLength : OutVUMeasureLength;
					
					if(!me->EnsureThreadChannelReadMaxpool(maxchannel, maxlength)) continue;
					maxlength=me->Pool->Read(me->Thread_Maxpool, maxchannel, maxlength, current, NULL);
					me->VisualCallBack(current, me->Thread_VUPool, me->Thread_FFTPool_Power, me->Thread_Maxpool, OutVUChannel, OutVUMeasureLength, OutFFTChannel, OutFFTMeasureLength, maxchannel, maxlength);
				}
			}
			else
			{
				Microsoft::winapi_Sleep(10);
			}
		}
	}
	return 0;
}