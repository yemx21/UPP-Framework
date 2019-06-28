#ifndef AudioResampler_H
#define  AudioResampler_H
#include <math.h>
#include "Config.h"
#include "Extras\soxr.h"
namespace System
{
	namespace Audio
	{
		struct DitherGenerator;
		class AudioResampler
		{
			static bool APILoaded;
			DitherGenerator* Dither;

			soxr* src;
			size_t srcsize;
			float ratio;

			float* outfloat;
			size_t outfloatsize;
			size_t outfloatvalid;

			SampleInfo srcinfo;
			ResampleConfig dstcfg;
			SampleInfo finalinfo;

			void* outpool;
			size_t outpoolsize;
			size_t outpoolvalid;

			float* floatpool;
			size_t floatpoolsize;
			size_t floatpoolvalid;

			float* EnsureFloatPool(const void* buf, UShort ch, size_t samplecount, BitsPerSample bps, size_t& bytes);
		public:
			AudioResampler();
			~AudioResampler();

			void unsafe_SetInputInfo(const SampleInfo& info);

			bool Config(const ResampleConfig& cfg);

			const void* Write(AudioBuffer* buf, size_t& bytes, bool& direct);

			const void* Write(AudioBuffer* buf, size_t& bytes, bool& direct, Audio::SampleInfo& FinalInfo);

			const void* Write(const void* buf, size_t bufbytes, const SampleInfo& info, size_t& bytes, bool& direct, Audio::SampleInfo& FinalInfo);

			const void* Read(size_t& bytes, Audio::SampleInfo& FinalInfo, bool& needmore);

			void Flush();
		};
	}
}

#endif