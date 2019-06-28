#pragma once
#include "Config.h"
#include <map>
#include <math.h>
#include <queue>
#include <functional>
#include <algorithm>
#include "AudioLimiter.h"

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

#ifndef M_LN2
#define M_LN2	   0.69314718055994530942
#endif

static const float band_f010[] ={ 31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 16000 };
static const float band_f015[] ={ 25,40,63,100,160,250,400,630,1000,1600,2500,4000,6300,10000,16000};
static const float band_f018[] ={ 55, 77, 110, 156, 220, 311, 440, 662, 880, 1200, 1800, 2500, 3500, 5000, 7000, 10000, 14000, 20000 };
static const float band_f025[] =
{ 20,31.5,40,50,80,100,125,160,250,315,400,500,800,
  1000,1250,1600,2500,3150,4000,5000,8000,10000,12500,16000,20000
};
static const float band_f031[] =
{ 20,25,31.5,40,50,63,80,100,125,160,200,250,315,400,500,630,800,
  1000,1250,1600,2000,2500,3150,4000,5000,6300,8000,10000,12500,16000,20000
};

#define CLAMP_fGain(value)(CLAMP<float>(value,-24.0f,24.0f))
#define IS_DENORMAL(f) (((*(unsigned int *)&(f))&0x7f800000) == 0)

namespace System
{
	namespace Audio
	{
		struct AudioEQConfig :ZeroStruct<AudioEQConfig>
		{
			int Bands;
			int Channels;
			bool Active;
			float Preamp;
			std::vector<float> Gains;
		};

		class AudioTimeEQ;

		enum class AudioIIRFilterMode
		{
			PeakingEQ,
			HighPass,
			LowPass,
			LowShelving,
			HighShelving
		};

		class AudioIIRFilter
		{
		private:
			friend class AudioTimeEQ;
			float omega, alpha, beta;
			float ff1, ff2, bww;
			float GaindB;
			friend class AudioTimeEQ;
		public:
			float a0, a1, a2, a3, a4;
			AudioIIRFilterMode Mode;
			bool GetRealActive() { return  GaindB != 0; }
			float SetGain(float g)
			{
				g = CLAMP_fGain(g);
				if (g != GaindB)
				{
					GaindB = g;
					float A = pow(10.0f, GaindB / 40.0f);
					omega = 2 * M_PI * ff1 / ff2;
					float sn = sin(omega);
					float cs = cos(omega);
					alpha = sn * sinh(M_LN2 / 2 * bww * omega / sn);
					beta = sqrt(A + A);

					float b0, b1, b2, c, c1, c2;
					if (Mode == AudioIIRFilterMode::PeakingEQ)
					{
						b0 = 1 + (alpha * A);
						b1 = -2 * cs;
						b2 = 1 - (alpha * A);
						c = 1 + (alpha / A);
						c1 = -2 * cs;
						c2 = 1 - (alpha / A);
					}
					else if (Mode == AudioIIRFilterMode::HighPass)
					{
						b0 = (1 + cs) / 2;
						b1 = -(1 + cs);
						b2 = (1 + cs) / 2;
						c = 1 + alpha;
						c1 = -2 * cs;
						c2 = 1 - alpha;
					}
					else if (Mode == AudioIIRFilterMode::LowPass)
					{
						b0 = (1 - cs) / 2;
						b1 = 1 - cs;
						b2 = (1 - cs) / 2;
						c = 1 + alpha;
						c1 = -2 * cs;
						c2 = 1 - alpha;
					}
					else if (Mode == AudioIIRFilterMode::LowShelving)
					{
						b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
						b1 = 2 * A * ((A - 1) - (A + 1) * cs);
						b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
						c = (A + 1) + (A - 1) * cs + beta * sn;
						c1 = -2 * ((A - 1) + (A + 1) * cs);
						c2 = (A + 1) + (A - 1) * cs - beta * sn;
					}
					else if (Mode == AudioIIRFilterMode::HighShelving)
					{
						b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
						b1 = -2 * A * ((A - 1) + (A + 1) * cs);
						b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
						c = (A + 1) - (A - 1) * cs + beta * sn;
						c1 = 2 * ((A - 1) - (A + 1) * cs);
						c2 = (A + 1) - (A - 1) * cs - beta * sn;
					}

					a0 = b0 / c;
					a1 = b1 / c;
					a2 = b2 / c;
					a3 = c1 / c;
					a4 = c2 / c;
				}
				return g;
			}
			AudioIIRFilter(float fq, float sr, float g, float bw, AudioIIRFilterMode mode = AudioIIRFilterMode::PeakingEQ) :GaindB(0), a0(0), a1(0), a2(0), a3(0), a4(0)
			{
				g = CLAMP_fGain(g);
				GaindB = g;
				bww = bw;
				ff1 = fq;
				ff2 = sr;
				float A = pow(10.0f, GaindB / 40.0f);
				omega = 2 * M_PI * fq / sr;
				float sn = sin(omega);
				float cs = cos(omega);
				alpha = sn * sinh(M_LN2 / 2 * bw * omega / sn);
				beta = sqrt(A + A);
				Mode = mode;
				float b0, b1, b2, c, c1, c2;
				if (Mode == AudioIIRFilterMode::PeakingEQ)
				{
					b0 = 1 + (alpha * A);
					b1 = -2 * cs;
					b2 = 1 - (alpha * A);
					c = 1 + (alpha / A);
					c1 = -2 * cs;
					c2 = 1 - (alpha / A);
				}
				else if (Mode == AudioIIRFilterMode::HighPass)
				{
					b0 = (1 + cs) / 2;
					b1 = -(1 + cs);
					b2 = (1 + cs) / 2;
					c = 1 + alpha;
					c1 = -2 * cs;
					c2 = 1 - alpha;
				}
				else if (Mode == AudioIIRFilterMode::LowPass)
				{
					b0 = (1 - cs) / 2;
					b1 = 1 - cs;
					b2 = (1 - cs) / 2;
					c = 1 + alpha;
					c1 = -2 * cs;
					c2 = 1 - alpha;
				}
				else if (Mode == AudioIIRFilterMode::LowShelving)
				{
					b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
					b1 = 2 * A * ((A - 1) - (A + 1) * cs);
					b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
					c = (A + 1) + (A - 1) * cs + beta * sn;
					c1 = -2 * ((A - 1) + (A + 1) * cs);
					c2 = (A + 1) + (A - 1) * cs - beta * sn;
				}
				else if (Mode == AudioIIRFilterMode::HighShelving)
				{
					b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
					b1 = -2 * A * ((A - 1) + (A + 1) * cs);
					b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
					c = (A + 1) - (A - 1) * cs + beta * sn;
					c1 = 2 * ((A - 1) - (A + 1) * cs);
					c2 = (A + 1) - (A - 1) * cs - beta * sn;
				}

				a0 = b0 / c;
				a1 = b1 / c;
				a2 = b2 / c;
				a3 = c1 / c;
				a4 = c2 / c;
			}
		};

		class IIRHistory
		{
		private:
			int Channels;
		public:
			float* X1;
			float* X2;
			float* Y1;
			float* Y2;
			IIRHistory(int channels)
			{
				Channels = channels;
				X1 = new float[Channels];
				X2 = new float[Channels];
				Y1 = new float[Channels];
				Y2 = new float[Channels];
				Zero();
			}
			~IIRHistory()
			{
				delete X1; X1 = NULL;
				delete X2; X2 = NULL;
				delete Y1; Y1 = NULL;
				delete Y2; Y2 = NULL;
			}
			void Zero()
			{
				memset(X1, 0, sizeof(float)*Channels);
				memset(X2, 0, sizeof(float)*Channels);
				memset(Y1, 0, sizeof(float)*Channels);
				memset(Y2, 0, sizeof(float)*Channels);
			}
		};

		class AudioTimeEQ
		{
		private:
			safe_void_ptr hMutex;
			float Preamp;
			int _Bands;
			int _Channels;
			float _SampleRate;
			bool Active;
			std::vector<IIRHistory*> History;
			std::vector<AudioIIRFilter*> Filters;
			std::vector<float> _Gains;
			std::vector<int> UpdateGain;
			AudioEQConfig* UpdateConfig;
			float UpdateSampleRate;
			bool NeedBuild;
		public:
			AudioTimeEQ();
			~AudioTimeEQ();
			bool Build(const AudioEQConfig& Config, float SampleRate);

			void Flush();

			void ClearHistory();
			void SetActive(bool v);
			float SetParam(int index, float gain);  ///当Index=-1时指派Preamp dB数值
			void DoEQ(AudioBuffer* Buffer);
		};
	}
}