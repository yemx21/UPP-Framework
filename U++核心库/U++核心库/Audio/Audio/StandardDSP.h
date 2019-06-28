#pragma once
#include "Config.h"
#include <math.h>
#include "Extras\SoundTouchDLL.h"
#ifndef M_LN2
#define M_LN2         0.69314718055994530942
#endif
#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

namespace System
{
	namespace Audio
	{
		class AudioSTDDSP
		{
		private:
			static bool APILoaded;

			HANDLE SoundTouch;
			UInt InternAudioBuffer_AvailableSize;
			UInt m_Channels;
			UInt m_SampleRate;
			float m_Pitch;
			float m_Tempo;
			float m_Rate;
		public:
			AudioSTDDSP();
			~AudioSTDDSP();
			void Clear();
			void SampleDSP(AudioBuffer* Buffer);
			void SetPitch(float Gain);
			void SetTempo(float Gain);
			void SetRate(float Gain);
		};
	}
}
