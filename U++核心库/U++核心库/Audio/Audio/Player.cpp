#include "Player.h"

using namespace System::Audio;

Player::Player(PluginCenter* Center, UInt64 bufferTime) :NowTree(NULL), m_Pitch(0), m_Tempo(0), m_Rate(0), m_Volume(1), m_Pan(0), BufferTime(bufferTime)
{	
	if(!Center)throw 2;
	Plugins=Center;
	_id = Center->GenerateHashId128();
	EqConfig=new AudioEQConfig();
	EqConfig->Active=false;
	EqConfig->Bands=0;
	EqConfig->Channels=0;
	EqConfig->Preamp=.0f;
	m_ReSampleConfig.EnableBitsPerSample=false;
	m_ReSampleConfig.EnableChannels=false;
	m_ReSampleConfig.EnableSampleRate=false;
	StdSA=new AudioAnalyzer(); 
	/*ReplayGain.Album=true;
	ReplayGain.Track=true;
	ReplayGain.PreventClip=true;
	ReplayGain.SoftClip=false;*/
	//ID=m_Manager->Create();
	Plugins->Players.push_back(this);

	DSPMgr=new DSPManager(Plugins);
}
Player::~Player()
{		
	for(int i=0;i<Trees.size();i++)
		if(Trees[i]->Stop(false))
		{
			bool IsNow=NowTree==Trees[i];
			delete Trees[i];
			Trees[i]=NULL;
			if(IsNow)NowTree=NULL;
		}
	Trees.clear();
	if(StdSA){delete StdSA;StdSA=NULL;}
	if(EqConfig)
	{
		EqConfig->Gains.clear();
		delete EqConfig;
		EqConfig=NULL;
	}	
	vector<Player*>::iterator pos=find_if(Plugins->Players.begin(),Plugins->Players.end(),Player_ID_Equ(Id));
	if(pos != Plugins->Players.end()) Plugins->Players.erase(pos);

	if(DSPMgr)
	{
		delete DSPMgr;
		DSPMgr=NULL;
	}
}

float** Player::GetFFTData(UShort DesiredChannel, UInt DesiredMeasureLength, UInt FFTWindow, bool Mono, unsigned short& OutChannel, unsigned int& OutMeasureLength)
{
	lock_guard<std::mutex> lock(locker);
	if(StdSA&&NowTree)
	{
		return StdSA->GetFFTData(DesiredChannel, DesiredMeasureLength, NowTree->GetCurrent(), FFTWindow, Mono, OutChannel, OutMeasureLength);
	}
	return NULL;
}

float* Player::GetVUData(UShort DesiredChannel, UInt DesiredMeasureLength, unsigned short& OutChannel, unsigned int& OutMeasureLength)
{
	lock_guard<std::mutex> lock(locker);
	if(StdSA&&NowTree)
	{
		return StdSA->GetVUData(DesiredChannel, DesiredMeasureLength, NowTree->GetCurrent(), OutChannel, OutMeasureLength);
	}
	return NULL;
}

bool Player::ReorderDSP(int OldIndex,int NewIndex, int Mode)
{
	lock_guard<std::mutex> lock(locker);
	return DSPMgr->MoveDSP(OldIndex,NewIndex,Mode);
}

Result<UInt128,  AudioTreeError, CoreSubObjects, int> Player::Open(const wchar_t* Path, const UInt128& Decoder, const UInt128& Device, int Timeout)
{
	lock_guard<std::mutex> lock(locker);
	Result<UInt128, AudioTreeError, CoreSubObjects, int> error(0U, AudioTreeError::Unknown, CoreSubObjects::None, 0);

	AudioTree* AT = new AudioTree(this, Decoder, Device, Plugins, DSPMgr, StdSA, BufferTime);

	if(m_Pitch!=0)AT->StdDSP->SetPitch(m_Pitch);
	if(m_Tempo!=0)AT->StdDSP->SetTempo(m_Tempo);
	if(m_Rate!=0)AT->StdDSP->SetRate(m_Rate);
	AT->SetReSampleConfig(m_ReSampleConfig);
	Result<bool, AudioTreeError, CoreSubObjects, int> result = AT->Open(Path, Timeout);
	if(result.Value1)
	{
		AT->SetVolume(m_Volume);
		AT->SetPan(m_Pan);
		AT->StdTimeEQ->Build(*EqConfig,AT->Info.SampleRate);
		//StdSA->SetConfig(BufferTime*AT->Info.Resolution.SampleRate/1000,AT->Info.Resolution.Channels);
		NowTree=AT;
		AT->Active=true;
		Trees.push_back(AT);
		StdSA->Flush();

		error.Value1 = AT->Id;
		error.Value2 = AudioTreeError::None;

		/*if(AT->StartDecodeThread())
		{
			while (!AT->IsThreadActive)
			{
				Sleep(2);
			}*/
			error.Value1= AT->Id;
			error.Value2 = AudioTreeError::None;
		/*}
		else
		{
			delete AT;
			AT=NULL;
		}*/
	}
	else
	{
		delete AT;
		AT=NULL;
		error.State=result.State;
		error.WriteMessage(result.Message);
	}
	return error;
}
bool Player::Play(const UInt128& ID)
{
	lock_guard<std::mutex> lock(locker);
	for(int i=0;i<Trees.size();i++)
		if(Trees[i]->Active&&Trees[i]->Id==ID)
			if(Trees[i]->Play())return true;
	return false;
}

bool Player::Pause(const UInt128& ID)
{
	lock_guard<std::mutex> lock(locker);
	for(int i=0;i<Trees.size();i++)
	if (Trees[i]->Active&&Trees[i]->Id == ID)
			if(Trees[i]->Pause())return true;
	return false;
}

bool Player::Stop(const UInt128& ID, int Message)
{
	lock_guard<std::mutex> lock(locker);
	for(int i=0;i<Trees.size();i++)
	{
		AudioTree* Tree=Trees[i];
		if (Tree->Active&&Trees[i]->Id == ID)
		{
			if(Tree->Stop(Message))
			{			
				bool IsNow=NowTree==Tree;
				delete Tree;
				Tree=NULL;
				Trees.erase(Trees.begin()+i);
				if(IsNow)NowTree=NULL;
				return true;
			}
			else
				return false;
		}
	}
	return true;
}
double Player::GetCurrent(const UInt128& ID)
{
	lock_guard<std::mutex> lock(locker);
	for(int i=0;i<Trees.size();i++)
	if (Trees[i]->Active&&Trees[i]->Id == ID)
			return Trees[i]->GetCurrent();
	return 0;
}
bool Player::SetCurrent(const UInt128& ID, double PosMS)
{
	lock_guard<std::mutex> lock(locker);
	for(int i=0;i<Trees.size();i++)
	if (Trees[i]->Active&&Trees[i]->Id == ID)
			return Trees[i]->SetCurrent(PosMS);
	return false;
}
AudioInfo Player::GetInfo(const UInt128& ID)
{
	lock_guard<std::mutex> lock(locker);
	for(int i=0;i<Trees.size();i++)
	if (Trees[i]->Active&&Trees[i]->Id == ID)
			return Trees[i]->GetInfo();
	return Empty;
}

int Player::SetEqConfig(int Bands, int Channels)
{
	lock_guard<std::mutex> lock(locker);
	if(Trees.size()==0)
	{
		if((Bands==10 || Bands==15 || Bands==18 || Bands==25 || Bands==31) && Channels>0 && Channels<=8)
		{
			EqConfig->Bands=Bands;
			EqConfig->Channels=Channels;
			EqConfig->Gains.clear();
			for(int i=0;i<Bands;i++)
				EqConfig->Gains.push_back(0);
			return Bands;
		}
		else
			return 0;
	}
	AudioEQConfig Config;
	memcpy(&Config,&EqConfig,sizeof(EqConfig));
	EqConfig->Bands=Bands;
	EqConfig->Channels=Channels;
	EqConfig->Gains.clear();
	for(int i=0;i<EqConfig->Bands;i++)
		EqConfig->Gains.push_back(0);
	bool re=true;
	for(int i=0;i<Trees.size();i++)
	{
		re=Trees[i]->StdTimeEQ->Build(*EqConfig,Trees[i]->Info.SampleRate);
	}
	if(!re)
	{
		memcpy(&EqConfig,&Config,sizeof(Config));
		return EqConfig->Bands;
	}
	return EqConfig->Bands;
}

void Player::SetEqActive(bool Active)
{
	lock_guard<std::mutex> lock(locker);
	EqConfig->Active=Active;
	for(int i=0;i<Trees.size();i++)Trees[i]->StdTimeEQ->SetActive(EqConfig->Active);
}

double Player::SetEqGain(int Index, double Gain)
{
	lock_guard<std::mutex> lock(locker);
	if(Index==-1)
		EqConfig->Preamp=Gain;
	else if(Index>-1 && Index<EqConfig->Gains.size())
		EqConfig->Gains[Index]=Gain;
	float result=Gain;
	for(int i=0;i<Trees.size();i++) result=Trees[i]->StdTimeEQ->SetParam(Index,Gain);
	return result;
}

void Player::SetPitch(double Gain)
{
	lock_guard<std::mutex> lock(locker);
	m_Pitch=Gain;
	for(int i=0;i<Trees.size();i++)Trees[i]->StdDSP->SetPitch(Gain);
}

void Player::SetTempo(double Gain)
{
	lock_guard<std::mutex> lock(locker);
	m_Tempo=Gain;
	for(int i=0;i<Trees.size();i++)Trees[i]->StdDSP->SetTempo(Gain);
}

void Player::SetRate(double Gain)
{
	lock_guard<std::mutex> lock(locker);
	m_Rate=Gain;
	for(int i=0;i<Trees.size();i++)Trees[i]->StdDSP->SetRate(Gain);
}

void Player::SetVolume(double Volume)
{
	lock_guard<std::mutex> lock(locker);
	m_Volume=Volume;
	for(int i=0;i<Trees.size();i++)Trees[i]->SetVolume(Volume);
}

void Player::SetPan(double Pan)
{
	lock_guard<std::mutex> lock(locker);
	m_Pan=Pan;
	for(int i=0;i<Trees.size();i++)Trees[i]->SetPan(Pan);
}


