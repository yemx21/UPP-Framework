#include "AudioLimiter.h"
using namespace System;
using namespace System::Audio;

AudioLimiter::AudioLimiter()
{
	memset(this, 0, sizeof(AudioLimiter));

	float  fParam2 = (float)0.40; //ratio
	float  fParam3 = (float)0.00; //level      ///was 0.6
	float  fParam4 = (float)0.18; //attack
	float  fParam5 = (float)0.55; //release
	float  fParam7 = (float)0.00; //gate thresh
	float  fParam8 = (float)0.10; //gate attack
	float  fParam9 = (float)0.50; //gate decay

	thr = 1.0f;
	rat = 2.5f * fParam2 - 0.5f;
	if (rat > 1.0)  rat = 1.f + 16.f*(rat - 1.f) * (rat - 1.f);
	if (rat < 0.0)  rat = 0.6f*rat;
	trim = (float)pow(10.f, 2.f * fParam3); //was  - 1.f);
	att = (float)pow(10.f, -0.002f - 2.f * fParam4);
	rel = (float)pow(10.f, -2.f - 3.f * fParam5);

	lthr = 1.0f;

	if (fParam7 < 0.02)
		xthr = 0.f;  //expander
	else
		xthr = (float)pow(10.f, 3.f * fParam7 - 3.f);
	xrat = 1.f - (float)pow(10.f, -2.f - 3.3f * fParam9);
	irel = (float)pow(10.0, -2.0 / 44100.0);
	gatt = (float)pow(10.f, -0.002f - 3.f * fParam8);

	if (rat < 0.0f && thr < 0.1f) rat *= thr * 15.f;
}

AudioLimiter::~AudioLimiter()
{

}

void AudioLimiter::Flush()
{
}

void AudioLimiter::Process(AudioBuffer* Buffer)
{
	return;
	if (!Buffer) return;

	auto res = Buffer->BeginChunk(32, true, false);
	auto chunk = res.Value;
	if (chunk)
	{
		chunk->SetInterleaved(true);
		float* mem = (float*)chunk->Interleaved();
		if (!mem) return;
		UInt count = chunk->GetSampleCount();
		UShort channels = chunk->GetChannels();
		UInt ch = 0;
		float bufsr = chunk->GetSampleRate();

		irel = (float)pow(10.0, -2.0 / bufsr);

		float g, e = env, e2 = env2, ra = rat, re = (1.f - rel), at = att, ga = gatt;
		float tr = trim, th = thr, lth = lthr, xth = xthr, ge = genv, y = dry;

		if (lth == 0.f) lth = 1000.f;
		while (count--)
		{
			float cmax = 0.0f;
			for (ch = 0; ch < channels; ch++)
			{
				float v = fabsf(*mem);
				mem++;
				if (v > cmax) cmax = v;
			}
			e = (cmax > e) ? e + at * (cmax - e) : e * re;
			e2 = (cmax > e) ? cmax : e2 * re; //ir;
			g = (e > th) ? tr / (1.f + ra * ((e / th) - 1.f)) : tr;

			if (g < 0.f) g = 0.f;
			if (g*e2 > lth) g = lth / e2; //limit 

			ge = (e > xth) ? ge + ga - ga * ge : ge * xrat; //gate

			mem -= channels;

			float gain = g * ge + y;
			for (ch = 0; ch < channels; ch++)
			{
				*mem = (*mem) * gain;
				if (*mem > 1.0f) *mem = 1.0f;
				if (*mem < -1.0f) *mem = -1.0f;
				mem++;
			}
		}
		if (e < 1.0e-10) env = 0.f; else env = e;
		if (e2 < 1.0e-10) env2 = 0.f; else env2 = e2;
		if (ge < 1.0e-10) genv = 0.f; else genv = ge;

		Buffer->EndChunk(chunk, false);
	}
}