#ifndef CORE_H
#define CORE_H
#include "Config.h"
#include "PluginCenter.h"
#include "StandardDSP.h"
#include "AudioAnalyzer.h"
#include "AudioTree.h"
#include "AudioGainAnalyzer.h"
#include "ID128.h"
#include <mutex>
namespace System
{
	namespace Audio
	{
		//enum class PlayerState
		//{
		//	Stopped,
		//	Stopping,
		//	Opening,
		//	Opened,
		//	Buffering,
		//	Ready,
		//	Playing,
		//	Paused,
		//};

		class Player : public ObjectManager::ID128
		{
		private:
			friend class Player_ID_Equ;
			AudioTree* NowTree;
			AudioInfo Empty;
			PluginCenter* Plugins;
			mutex locker;
		public:
			double m_Pitch;
			double m_Tempo;
			double m_Rate;
			double m_Volume;
			double m_Pan;
			AudioEQConfig* EqConfig;
			DSPManager* DSPMgr;
			AudioAnalyzer* StdSA;
			ResampleConfig m_ReSampleConfig;
			UInt64 BufferTime;
			vector<AudioTree*> Trees;

			Player(PluginCenter* Center, UInt64 bufferTime);
			~Player();
			//Replaygainconfig ReplayGain;

			bool ReorderDSP(int OldIndex, int NewIndex, int Mode);

			Result<UInt128, AudioTreeError, CoreSubObjects, int> Open(const wchar_t* Path, const UInt128& Decoder, const UInt128& Device, int Timeout);

			AudioInfo GetInfo(const UInt128& id);
			bool Play(const UInt128& id);
			bool Pause(const UInt128& id);
			bool Stop(const UInt128& id, int Message);
			double GetCurrent(const UInt128& id);
			bool SetCurrent(const UInt128& id, double PosMS);

			int SetEqConfig(int Bands, int Channels);
			void SetEqActive(bool Active);
			double SetEqGain(int Index, double Gain);

			void SetPitch(double Gain);
			void SetTempo(double Gain);
			void SetRate(double Gain);
			void SetVolume(double Volume);
			void SetPan(double Pan);

			float** GetFFTData(UShort DesiredChannel, UInt DesiredMeasureLength, UInt FFTWindow, bool Mono, unsigned short& OutChannel, unsigned int& OutMeasureLength);
			float* GetVUData(UShort DesiredChannel, UInt DesiredMeasureLength, unsigned short& OutChannel, unsigned int& OutMeasureLength);

		};

		class Player_ID_Equ
		{
		public:
			UInt128 i;
			explicit Player_ID_Equ(const UInt128& s) : i(s) {}
			bool operator ()(const Player* e) const { return e->Id == i; }
		};

	}
}



#endif