#include "StandardDSP.h"
#include "WindowsEnvironment.h"
using namespace System::OS;

using namespace System::Audio;

#ifndef SETTING_USE_QUICKSEEK
#define SETTING_USE_QUICKSEEK 2
#endif

#ifndef SETTING_USE_AA_FILTER
#define SETTING_USE_AA_FILTER 0
#endif


namespace System
{
	namespace Audio
	{
		class SoundTouchAPI
		{
		public:
			static HANDLE (*soundtouch_createInstance)();
			static int (*soundtouch_setSetting)(HANDLE h,int settingId, int value );
			static void(*soundtouch_destroyInstance)(HANDLE h);
			static unsigned int(*soundtouch_receiveSamples)(HANDLE h,float *outBuffer, unsigned int maxSamples );
			static void(*soundtouch_clear)(HANDLE h);
			static void(*soundtouch_putSamples)(HANDLE h, const float *samples,  unsigned int numSamples     );
			static void(*soundtouch_setChannels)(HANDLE h, unsigned int numChannels);
			static void(*soundtouch_setSampleRate)(HANDLE h, unsigned int srate);
			static void(*soundtouch_setTempo)(HANDLE h, float newTempo);
			static void(*soundtouch_setRateChange)(HANDLE h, float newRate);
			static void(*soundtouch_setTempoChange)(HANDLE h, float newTempo);
			static void(*soundtouch_setPitchSemiTones)(HANDLE h, float newPitch);

			static HANDLE Create()
			{
				if (soundtouch_createInstance) return soundtouch_createInstance(); 
				return NULL;
			}

			#define CHECK_API(test) if((test)==nullptr) goto ERR

			static API_PTR module;
			static bool DynmaicInit()
			{
				module = Microsoft::winapi_LoadLibrary(L"SoundTouch.dll");
				if (module)
				{
					CHECK_API(soundtouch_createInstance = (HANDLE(*)()) Microsoft::winapi_GetProcAddress(module, "soundtouch_createInstance"));
					CHECK_API(soundtouch_setSetting = (int(*)(HANDLE h, int settingId, int value)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setSetting"));
					CHECK_API(soundtouch_destroyInstance = (void(*)(HANDLE h)) Microsoft::winapi_GetProcAddress(module, "soundtouch_destroyInstance"));
					CHECK_API(soundtouch_receiveSamples = (unsigned int (*)(HANDLE h, float *outBuffer, unsigned int maxSamples)) Microsoft::winapi_GetProcAddress(module, "soundtouch_receiveSamples"));
					CHECK_API(soundtouch_clear = (void(*)(HANDLE h)) Microsoft::winapi_GetProcAddress(module, "soundtouch_clear"));
					CHECK_API(soundtouch_putSamples = (void(*)(HANDLE h, const float *samples, unsigned int numSamples)) Microsoft::winapi_GetProcAddress(module, "soundtouch_putSamples"));
					CHECK_API(soundtouch_setChannels = (void(*)(HANDLE h, unsigned int numChannels)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setChannels"));
					CHECK_API(soundtouch_setSampleRate = (void(*)(HANDLE h, unsigned int srate)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setSampleRate"));

					CHECK_API(soundtouch_setTempo = (void(*)(HANDLE h, float newTempo)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setTempo"));
					CHECK_API(soundtouch_setRateChange = (void(*)(HANDLE h, float newRate)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setRateChange"));
					CHECK_API(soundtouch_setTempoChange = (void(*)(HANDLE h, float newTempo)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setTempoChange"));

					CHECK_API(soundtouch_setPitchSemiTones = (void(*)(HANDLE h, float newPitch)) Microsoft::winapi_GetProcAddress(module, "soundtouch_setPitchSemiTones"));

					return true;
				ERR:
					Microsoft::winapi_FreeLibrary(module);
					module = NULL;
					return false;
				}
				return false;
			}

			static void DynamicExit()
			{
				if (module) Microsoft::winapi_FreeLibrary(module);
			}

		};

		API_PTR SoundTouchAPI::module = nullptr;

		decltype(SoundTouchAPI::soundtouch_createInstance) SoundTouchAPI::soundtouch_createInstance = nullptr;
		decltype(SoundTouchAPI::soundtouch_setSetting) SoundTouchAPI::soundtouch_setSetting = nullptr;
		decltype(SoundTouchAPI::soundtouch_destroyInstance) SoundTouchAPI::soundtouch_destroyInstance = nullptr;
		decltype(SoundTouchAPI::soundtouch_receiveSamples) SoundTouchAPI::soundtouch_receiveSamples = nullptr;
		decltype(SoundTouchAPI::soundtouch_clear) SoundTouchAPI::soundtouch_clear = nullptr;
		decltype(SoundTouchAPI::soundtouch_putSamples) SoundTouchAPI::soundtouch_putSamples = nullptr;
		decltype(SoundTouchAPI::soundtouch_setChannels) SoundTouchAPI::soundtouch_setChannels = nullptr;
		decltype(SoundTouchAPI::soundtouch_setSampleRate) SoundTouchAPI::soundtouch_setSampleRate = nullptr;
		decltype(SoundTouchAPI::soundtouch_setTempo) SoundTouchAPI::soundtouch_setTempo = nullptr;
		decltype(SoundTouchAPI::soundtouch_setRateChange) SoundTouchAPI::soundtouch_setRateChange = nullptr;
		decltype(SoundTouchAPI::soundtouch_setTempoChange) SoundTouchAPI::soundtouch_setTempoChange = nullptr;
		decltype(SoundTouchAPI::soundtouch_setPitchSemiTones) SoundTouchAPI::soundtouch_setPitchSemiTones = nullptr;
	}
}

bool AudioSTDDSP::APILoaded = SoundTouchAPI::DynmaicInit();


AudioSTDDSP::AudioSTDDSP():InternAudioBuffer_AvailableSize(0),m_Pitch(0),m_Tempo(0),m_Rate(0),m_Channels(0),m_SampleRate(0)
{
	SoundTouch = (AudioSTDDSP::APILoaded) ? SoundTouchAPI::Create() : nullptr;
	if (SoundTouch)
	{
		SoundTouchAPI::soundtouch_setSetting(SoundTouch, SETTING_USE_QUICKSEEK, 1);
		SoundTouchAPI::soundtouch_setSetting(SoundTouch, SETTING_USE_AA_FILTER, 1);
	}
}

AudioSTDDSP::~AudioSTDDSP()
{
	if (SoundTouch)
	{
		SoundTouchAPI::soundtouch_destroyInstance(SoundTouch);
		SoundTouch = NULL;
	}
}

void AudioSTDDSP::Clear()
{
	if(SoundTouch) SoundTouchAPI::soundtouch_clear(SoundTouch);
}

void AudioSTDDSP::SampleDSP(AudioBuffer* Buffer)
{	
	if (!Buffer || !SoundTouch) return;
	try
	{
		UShort ch=Buffer->Channels;
		UInt sr=Buffer->SampleRate;
		UInt sn=Buffer->SampleNum;
		UInt samples=sn;
		if(m_Pitch!=0||m_Rate!=0||m_Tempo!=0)
		{
			if(m_Channels!=ch)
			{
				try
				{
					SoundTouchAPI::soundtouch_setChannels(SoundTouch, ch);
					m_Channels=ch;
				}
				catch(...)
				{
				}
			}
			if(m_SampleRate!= sr)
			{
				SoundTouchAPI::soundtouch_setSampleRate(SoundTouch, sr);
				m_SampleRate=sr;
			}
			auto res = Buffer->BeginChunk(32, true, false);
			auto chunk = res.Value;
			if (chunk)
			{
				chunk->SetInterleaved(true);	
				float* floatmem = (float*)chunk->Interleaved();	
				if(floatmem)
				{
					SoundTouchAPI::soundtouch_putSamples(SoundTouch, floatmem, sn);
					size_t done = 0;
					UInt donesamples = 0;
					UInt needsamples = 0;
					do 
					{
						needsamples = samples + donesamples;
						chunk->EnsureSampleCount(needsamples, true);
						InternAudioBuffer_AvailableSize = SoundTouchAPI::soundtouch_receiveSamples(SoundTouch, ((float*)chunk->Interleaved())+ done, sn);
						donesamples += InternAudioBuffer_AvailableSize;
						if(InternAudioBuffer_AvailableSize!=0)
						{
							int debug = 0;
							debug++;
						}
						done += InternAudioBuffer_AvailableSize * ch;
						chunk->SetSampleCount(donesamples);
					}while (InternAudioBuffer_AvailableSize != 0);
				}
				Buffer->EndChunk(chunk, false);
			}
		}
	}
	catch(...)
	{
		if(Buffer)
		{
			Buffer->Clear();
		}
	}
}

void AudioSTDDSP::SetPitch(float Gain)
{
	if(Gain>=-12&&Gain<=12)
	{
		m_Pitch=Gain;
		if (SoundTouch) SoundTouchAPI::soundtouch_setPitchSemiTones(SoundTouch, m_Pitch);
	}
}
void AudioSTDDSP::SetTempo(float Gain)
{
	if(Gain>=-50&&Gain<=100)
	{
		m_Tempo=Gain;
		if (SoundTouch) SoundTouchAPI::soundtouch_setTempoChange(SoundTouch, m_Tempo);
	}
}
void AudioSTDDSP::SetRate(float Gain)
{
	if(Gain>=-50&&Gain<=100)
	{
		m_Rate=Gain;
		if (SoundTouch) SoundTouchAPI::soundtouch_setRateChange(SoundTouch, m_Rate);
	}
}
