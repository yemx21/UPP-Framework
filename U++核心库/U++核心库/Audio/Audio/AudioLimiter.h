#ifndef AudioLimiter_H
#define AudioLimiter_H
#include <math.h>
#include "Config.h"

namespace System
{
	namespace Audio
	{
		class AudioLimiter
		{
		private:
			float thr, rat, env, env2, att, rel, trim, lthr, xthr, xrat, dry;
			float genv, gatt, irel;
		public:
			AudioLimiter();
			~AudioLimiter();
			void Flush();
			void Process(AudioBuffer* Buffer);
		};
	}
}
#endif