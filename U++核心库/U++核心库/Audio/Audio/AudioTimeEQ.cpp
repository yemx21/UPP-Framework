#include "AudioTimeEQ.h"
#include <stdio.h>
#include <float.h>
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;

AudioTimeEQ::AudioTimeEQ():History(NULL),Filters(NULL),_Bands(0),_Channels(0),_SampleRate(0),Active(false),Preamp(1.0f),NeedBuild(false),UpdateConfig(NULL), UpdateSampleRate(44100)
{
	int n=rand();
	hMutex = Microsoft::winapi_CreateMutex(NULL, false, NULL);
}

AudioTimeEQ::~AudioTimeEQ()
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	Microsoft::winapi_ReleaseMutex(*hMutex);
	Microsoft::winapi_CloseHandle(*hMutex);
	hMutex=NULL;
	int i=0;
	_Gains.clear();
	if(UpdateConfig)
	{
		UpdateConfig->Gains.clear();
		delete UpdateConfig; 
		UpdateConfig=NULL;
	}
	for(i=0;i<Filters.size();i++)
	{
		delete Filters[i];
		Filters[i]=NULL;
	}
	Filters.clear();
	for(i=0;i<History.size();i++)
	{
		delete History[i];
		History[i]=NULL;
	}
	History.clear();
}
bool AudioTimeEQ::Build(const AudioEQConfig& Config, float SampleRate)
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	int i=0;
	UpdateConfig=new AudioEQConfig();
	UpdateConfig->Active=Config.Active;
	UpdateConfig->Bands=Config.Bands;
	UpdateConfig->Channels=Config.Channels;
	for(i=0;i<UpdateConfig->Bands;i++)
		UpdateConfig->Gains.push_back(Config.Gains[i]);
	UpdateConfig->Preamp=Config.Preamp;
	if(_Bands!=Config.Bands || _Channels!=Config.Channels || _SampleRate!=SampleRate)
	{
		if(Config.Channels>0&&Config.Channels<=8)
		{
			if(Config.Bands==10 || Config.Bands==15 || Config.Bands==18 || Config.Bands==25 || Config.Bands==31)
			{
				if(SampleRate>0 && SampleRate<192000)
				{
					UpdateSampleRate=SampleRate;
					_Gains.clear();
					for(i=0;i<UpdateConfig->Bands;i++)
					{
						_Gains.push_back(UpdateConfig->Gains[i]);
					}
					UpdateGain.clear();
					NeedBuild=true;
					Microsoft::winapi_ReleaseMutex(*hMutex);
					return true;
				}
			}
		}
	}
	else if(_Bands==Config.Bands || _Channels==Config.Channels || _SampleRate==SampleRate)
	{
		Microsoft::winapi_ReleaseMutex(*hMutex);
		return true;
	}
	UpdateConfig->Gains.clear();
	delete UpdateConfig;
	UpdateConfig=NULL;
	Microsoft::winapi_ReleaseMutex(*hMutex);
	return false;
}

float AudioTimeEQ::SetParam(int index, float gain)
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	if(index==-1)
	{
		Preamp=pow(10.0f,gain/20.0f); 
		Microsoft::winapi_ReleaseMutex(*hMutex);
		return gain;
	}
	float result=CLAMP_fGain(gain);
	if(index>-1 && index< _Bands)
	{
		_Gains[index]=result;
		UpdateGain.push_back(index);
	}
	else
		result=-1.0f;
	if(History.size()>0)
	{
		for(int i=0;i<History.size();i++)
		{
			History[i]->Zero();
		}
	}
	Microsoft::winapi_ReleaseMutex(*hMutex);
	return result;
}

void AudioTimeEQ::SetActive(bool v)
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	Active=v;
	Microsoft::winapi_ReleaseMutex(*hMutex);
}

void AudioTimeEQ::ClearHistory()
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	int i=0;
	if(History.size()>0)
	{
		for(i=0;i<History.size();i++)
		{
			delete History[i];
			History[i]=NULL;
		}
	}
	History.clear();
	Microsoft::winapi_ReleaseMutex(*hMutex);
}

void AudioTimeEQ::DoEQ(AudioBuffer* Buffer)
{
	if(!Buffer) return;
	if(Microsoft::winapi_WaitForSingleObject(*hMutex, 1000)==API_WAIT_TIMEOUT)
	{
		Microsoft::winapi_ReleaseMutex(*hMutex);
	}
	int i=0,j=0,n=0;
	if(_SampleRate!=Buffer->SampleRate)
	{
		UpdateSampleRate=Buffer->SampleRate;
		NeedBuild=true;
	}
	if(NeedBuild)
	{	
		for(i=0;i<Filters.size();i++)
		{
			delete Filters[i];
			Filters[i]=NULL;
		}
		Filters.clear();
		for(i=0;i<History.size();i++)
		{
			delete History[i];
			History[i]=NULL;
		}
		History.clear();
		if (UpdateConfig)
		{
			_Bands = UpdateConfig->Bands;
			_Channels = UpdateConfig->Channels;
			Active = UpdateConfig->Active;
			_SampleRate = UpdateSampleRate;
			Preamp = pow(10.0f, (UpdateConfig->Preamp / 20.0f));
			UpdateConfig->Gains.clear();
			delete UpdateConfig;
			UpdateConfig = NULL;
		}
		if(_Bands==10)
		{
			for(j=0;j<10;j++)
			{
				Filters.push_back(new AudioIIRFilter(band_f010[j],_SampleRate,_Gains[j],1.0f ));
			}
		}
		else if(_Bands==15)
		{
			for(j=0;j<15;j++)
			{
				Filters.push_back(new AudioIIRFilter(band_f015[j],_SampleRate,_Gains[j],2.0f/3.0f));
			}
		}
		else if(_Bands==18)
		{
			for(j=0;j<18;j++)
			{
				Filters.push_back(new AudioIIRFilter(band_f018[j],_SampleRate,_Gains[j],2.0f/3.0f));
			}
		}
		else if(_Bands==25)
		{
			for(j=0;j<25;j++)
			{
				Filters.push_back(new AudioIIRFilter(band_f025[j],_SampleRate,_Gains[j],1.0f/3.0f));
			}
		}
		else if(_Bands==31)
		{
			for(j=0;j<31;j++)
			{
				Filters.push_back(new AudioIIRFilter(band_f031[j],_SampleRate,_Gains[j],1.0f/3.0f));
			}
		}
		for(j=0;j<_Bands;j++)
		{
			History.push_back(new IIRHistory(_Channels));
		}
		UpdateGain.clear();
		NeedBuild=false;
	}
	bool prelimit=false;
	if(UpdateGain.size()>0)
	{
		int index=0;
		for(i=0;i<UpdateGain.size();i++)
		{
			index=UpdateGain[i];
			Filters[index]->SetGain(_Gains[index]);
		}
		UpdateGain.clear();
		for(i=0;i<History.size();i++)
		{
			History[i]->Zero();
		}
		prelimit=true;
	}
	if(!Active || _Bands==0 || _Channels==0)
	{
		Microsoft::winapi_ReleaseMutex(*hMutex);
		return;
	}
	float Data=0;
	float Out=0;
	float Result=0;
	auto res = Buffer->BeginChunk(32, true, false);
	auto chunk = res.Value;
	if (chunk)
	{
		chunk->SetInterleaved(true);
		float* floatmem = (float*)chunk->Interleaved();
		if (floatmem)
		{
			
			UShort ch= chunk->GetChannels();
			unsigned int Count = chunk->GetSampleCount()*ch;
			for(i=0;i<Count;i+=ch)
			{
				for(j=0;j<ch;j++)
				{
					if(j<_Channels)
					{
 						Out=0;
						Data=floatmem[i+j];
						Data*=Preamp;
						for (n=0; n<_Bands; n++)
						{
							Result = Filters[n]->GaindB==0.0f? Data: Filters[n]->a0 * Data + Filters[n]->a1 * History[n]->X1[j] + Filters[n]->a2 * History[n]->X2[j] -Filters[n]->a3 * History[n]->Y1[j] - Filters[n]->a4 * History[n]->Y2[j];

							if( IS_DENORMAL( Result ) ) Result = 0;
							History[n]->X2[j] = History[n]->X1[j];
							History[n]->X1[j] = Data;

							History[n]->Y2[j] = History[n]->Y1[j];
							History[n]->Y1[j] = Result;

							Out= Result;
							Data=Out;
						}
						floatmem[i+j]=Out;
					}
				}
			}
		}
		Buffer->EndChunk(chunk);
	}
	Microsoft::winapi_ReleaseMutex(*hMutex);
}

void AudioTimeEQ::Flush()
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	for(auto& his: History)
	{
		his->Zero();
	}
	Microsoft::winapi_ReleaseMutex(*hMutex);
}