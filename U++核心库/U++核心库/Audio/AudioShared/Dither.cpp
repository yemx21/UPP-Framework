#include "Dither.h"


using namespace System::Audio;

#define DITHER_BITS_   (15)

#define DITHER_BITS_1   (31)

#define DITHER_BITS_2   (63)


static const float const_1_div_128_f = 1.0f / 128.0f;  /* 8 bit multiplier */

static const float const_1_div_32768_f = 1.0f / 32768.0f; /* 16 bit multiplier */

static const float const_1_div_8388608_f = 1.0f / 8388608.0f; /* 24 bit multiplier */

static const float const_1_div_2147483648_f= 1.0f / 2147483648.0f; /* 32 bit multiplier */

static const double const_1_div_128_d= 1.0 / 128.0;  /* 8 bit multiplier */

static const double const_1_div_32768_d = 1.0 / 32768.0; /* 16 bit multiplier */

static const double const_1_div_8388608_d = 1.0 / 8388608.0; /* 24 bit multiplier */

static const double const_1_div_2147483648_d= 1.0 / 2147483648.0; /* 32 bit multiplier */

namespace System
{
	namespace Audio
	{
		void InitializeDitherState(DitherGenerator *state)
		{
			state->previous = 0;
			state->randSeed1 = 22222;
			state->randSeed2 = 5555555;
		}

		__forceinline int Generate16BitDither(DitherGenerator *state)
		{
			int current, highPass;

			state->randSeed1 = (state->randSeed1 * 196314165) + 907633515;
			state->randSeed2 = (state->randSeed2 * 196314165) + 907633515;

#define DITHER_SHIFT_  ((sizeof(int)*8 - DITHER_BITS_) + 1)

			current = (((int)state->randSeed1) >> DITHER_SHIFT_) +
				(((int)state->randSeed2) >> DITHER_SHIFT_);

			highPass = current - state->previous;
			state->previous = current;
			return highPass;
		}

#define FLOAT_DITHER_SCALE_  (1.0f / ((1<<DITHER_BITS_1)-1))
		static const float const_float_dither_scale_ = FLOAT_DITHER_SCALE_;
		__forceinline float GenerateFloatDither(DitherGenerator *state)
		{
			int current, highPass;

			state->randSeed1 = (state->randSeed1 * 196314165) + 907633515;
			state->randSeed2 = (state->randSeed2 * 196314165) + 907633515;

			current = (((int)state->randSeed1) >> DITHER_SHIFT_) +
				(((int)state->randSeed2) >> DITHER_SHIFT_);

			highPass = current - state->previous;
			state->previous = current;
			return ((float)highPass) * const_float_dither_scale_;
		}

		__forceinline double GenerateDoubleDither(DitherGenerator *state)
		{
			int current, highPass;

			state->randSeed1 = (state->randSeed1 * 196314165) + 907633515;
			state->randSeed2 = (state->randSeed2 * 196314165) + 907633515;

			current = (((int)state->randSeed1) >> DITHER_SHIFT_) +
				(((int)state->randSeed2) >> DITHER_SHIFT_);

			highPass = current - state->previous;
			state->previous = current;
			return ((double)highPass) * const_float_dither_scale_;
		}

#pragma region Interleaved to Interleaved
		void Int32_To_Int24(
			void *dest,
			const void *src,
			unsigned int count)
		{
			int *srcB = (int*)src;
			unsigned char *destB = (unsigned char*)dest;

			while (count--)
			{
				destB[0] = (unsigned char)(*srcB >> 8);
				destB[1] = (unsigned char)(*srcB >> 16);
				destB[2] = (unsigned char)(*srcB >> 24);

				srcB ++;
				destB += 3;
			}
		}

		void Int32_To_Int16_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			int *srcB = (int*)src;
			short *destB = (short*)dest;
			int dither;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB = (short)CLAMP_i16(((((*srcB) >> 1) + dither) >> 15));

				srcB ++;
				destB ++;
			}
		}

		void Int32_To_Int8_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			int *srcB = (int*)src;
			signed char *destB = (signed char*)dest;
			int dither;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB = (signed char)CLAMP_i8(((((*srcB) >> 1) + dither) >> 23));

				srcB ++;
				destB ++;
			}
		}

		void Int24_To_Int16_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			unsigned char *srcB = (unsigned char*)src;
			short *destB = (short*)dest;

			int temp, dither;

			while (count--)
			{
				temp = (((int)srcB[0]) << 8);
				temp = temp | (((int)srcB[1]) << 16);
				temp = temp | (((int)srcB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				int Res = ((temp >> 1) + dither) >> 15;
				Res = CLAMP_i16(Res);
				*destB = (short)(Res);

				srcB += 3;
				destB ++;
			}
		}

		void Int16_To_Int24(
			void *dest,
			const void *src,
			unsigned int count)
		{
			short *srcB = (short*)src;
			unsigned char *destB = (unsigned char*)dest;
			short temp;

			while (count--)
			{
				temp = *srcB;

				destB[0] = 0;
				destB[1] = (unsigned char)(temp);
				destB[2] = (unsigned char)(temp >> 8);
				srcB ++;
				destB += 3;
			}
		}

		void Int16_To_Int32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			short *srcB = (short*)src;
			Int32 *destB = (Int32*)dest;

			while (count--)
			{
				*destB = *srcB << 16;
				srcB ++;
				destB ++;
			}
		}


		void Int24_To_Int8_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			unsigned char *srcB = (unsigned char*)src;
			signed char  *destB = (signed char*)dest;

			int temp, dither;

			while (count--)
			{
				temp = (((int)srcB[0]) << 8);
				temp = temp | (((int)srcB[1]) << 16);
				temp = temp | (((int)srcB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				*destB = (signed char)CLAMP_i8((((temp >> 1) + dither) >> 23));

				srcB += 3;
				destB ++;
			}
		}
		void Int24_To_Int32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			unsigned char *srcB = (unsigned char*)src;
			Int32 *destB = (Int32*)dest;
			Int32 temp;
			while (count--)
			{

				temp = (((Int32)srcB[0]) << 8);
				temp = temp | (((Int32)srcB[1]) << 16);
				temp = temp | (((Int32)srcB[2]) << 24);

				*destB = temp;

				srcB += 3;
				destB ++;
			}

		}

		void Int16_To_Int8(
			void *dest,
			const void *src,
			unsigned int count)
		{
			short *srcB = (short*)src;
			signed char *destB = (signed char*)dest;
			while (count--)
			{
				*destB = (signed char)((*srcB) >> 8);

				srcB ++;
				destB ++;
			}
		}

		void Int8_To_Int16(
			void *dest,
			const void *src,
			unsigned int count)
		{
			signed char *srcB = (signed char*)src;
			short *destB = (short*)dest;

			while (count--)
			{
				*destB = (short)((*srcB) << 8);

				srcB ++;
				destB ++;
			}
		}

		void Int8_To_Int24(
			void *dest,
			const void *src,
			unsigned int count)
		{
			signed char *srcB = (signed char*)src;
			unsigned char *destB = (unsigned char*)dest;
			while (count--)
			{
				destB[0] = 0;
				destB[1] = 0;
				destB[2] = (*srcB);

				srcB ++;
				destB += 3;
			}
		}

		void Int8_To_Int32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			signed char *srcB = (signed char*)src;
			Int32 *destB = (Int32*)dest;
			while (count--)
			{
				(*destB) = (*srcB) << 24;

				srcB ++;
				destB ++;
			}

		}

		void Int8_To_Float32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int8 *srcB = (Int8*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_128_f;

				srcB ++;
				destB ++;
			}
		}

		void Int16_To_Float32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int16 *srcB = (Int16*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_32768_f;

				srcB ++;
				destB ++;
			}
		}

		void Int24_To_Float32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int24 *srcB = (Int24*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB).operator float() * const_1_div_8388608_f;

				srcB ++;
				destB ++;
			}
		}

		void Int32_To_Float32(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int32 *srcB = (Int32*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_2147483648_f;

				srcB ++;
				destB ++;
			}
		}

		void Int8_To_Float64(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int8 *srcB = (Int8*)src;
			double *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_128_d;

				srcB ++;
				destB ++;
			}
		}

		void Int16_To_Float64(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int16 *srcB = (Int16*)src;
			double  *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_32768_d;

				srcB ++;
				destB ++;
			}
		}

		void Int24_To_Float64(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int24 *srcB = (Int24*)src;
			double  *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB).operator float() * const_1_div_8388608_d;

				srcB ++;
				destB ++;
			}
		}

		void Int32_To_Float64(
			void *dest,
			const void *src,
			unsigned int count)
		{
			Int32 *srcB = (Int32*)src;
			double  *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_2147483648_d;

				srcB ++;
				destB ++;
			}
		}

		void Float32_To_Int8_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			signed char *destB = (signed char*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (126.0f)) + dither;
				*destB = (signed char)CLAMP_f8(dithered);

				srcB ++;
				destB ++;
			}
		}

		void Float64_To_Int8_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			signed char *destB = (signed char*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (126.0)) + dither;
				*destB = (signed char)CLAMP_d8(dithered);

				srcB ++;
				destB ++;
			}
		}


		void Float32_To_Int16_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			Int16 *destB = (Int16*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (32766.0f)) + dither;
				*destB = (Int16)CLAMP_f16(dithered);

				srcB ++;
				destB ++;
			}
		}

		void Float64_To_Int16_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			Int16 *destB = (Int16*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (32766.0f)) + dither;
				*destB = (Int16)CLAMP_d16(dithered);

				srcB ++;
				destB ++;
			}
		}

		void Float32_To_Int24_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			Int24 *destB = (Int24*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (8388606.0f)) + dither;
				(*destB) = (int)CLAMP_f24(dithered);

				srcB ++;
				destB ++;
			}
		}

		void Float64_To_Int24_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			Int24 *destB = (Int24*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (8388606.0f)) + dither;

				(*destB) = (int)CLAMP_d24(dithered);

				srcB ++;
				destB ++;
			}
		}

		void Float32_To_Int32_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			Int32 *destB = (Int32*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (2147483646.0f)) + dither;
				*destB = (Int32)CLAMP_f32(dithered);

				srcB ++;
				destB ++;
			}

		}

		void Float64_To_Int32_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			Int32 *destB = (Int32*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (2147483646.0f)) + dither;
				*destB = (Int32)CLAMP_d32(dithered);

				srcB ++;
				destB ++;
			}
		}

		void Float32_To_Float64(void *dest,
			const void *src,
			unsigned int count)
		{
			float *srcB = (float*)src;
			double *destB = (double*)dest;

			while (count--)
			{
				*destB = *srcB;

				srcB ++;
				destB ++;
			}

		}

		void Float64_To_Float32(void *dest,
			const void *src,
			unsigned int count)
		{
			double *srcB = (double*)src;
			float *destB = (float*)dest;

			while (count--)
			{
				*destB = *srcB;

				srcB ++;
				destB ++;
			}

		}

#pragma endregion

#pragma region Interleaved to Splitted
		void Int8_To_SplittedInt8(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int8 *srcB = (Int8*)src;
			Int8** destB = (Int8**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB[ct] = *srcB;

				srcB++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void Int16_To_SplittedInt16(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			short *srcB = (short*)src;
			short** destB = (short**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB[ct] = *srcB;

				srcB++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int24_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			unsigned char *srcB = (unsigned char*)src;
			unsigned char** destB = (unsigned char**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				auto destCB = destB[ct];

				destCB[0] = srcB[0];
				destCB[1] = srcB[1];
				destCB[2] = srcB[2];

				srcB+=3;
				destB[ct]+=3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int32_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			int *srcB = (int*)src;
			int** destB = (int**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB[ct] = *srcB;

				srcB++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float32_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			float *srcB = (float*)src;
			float** destB = (float**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB[ct] = *srcB;

				srcB++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float64_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			double *srcB = (double*)src;
			double** destB = (double**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB[ct] = *srcB;

				srcB++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int32_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			int *srcB = (int*)src;
			unsigned char** destB = (unsigned char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				auto destCB = destB[ct];
				destCB[0] = (unsigned char)(*srcB >> 8);
				destCB[1] = (unsigned char)(*srcB >> 16);
				destCB[2] = (unsigned char)(*srcB >> 24);

				srcB ++;
				destB[ct] += 3;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int32_To_SplittedInt16_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			int *srcB = (int*)src;
			short**destB = (short**)dest;
			int dither;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB[ct] = (short)CLAMP_i16(((((*srcB) >> 1) + dither) >> 15));

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int32_To_SplittedInt8_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			int *srcB = (int*)src;
			signed char**destB = (signed char**)dest;
			int dither;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB[ct] = (signed char)CLAMP_i8(((((*srcB) >> 1) + dither) >> 23));

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int24_To_SplittedInt16_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			unsigned char *srcB = (unsigned char*)src;
			short**destB = (short**)dest;

			int temp, dither;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				temp = (((int)srcB[0]) << 8);
				temp = temp | (((int)srcB[1]) << 16);
				temp = temp | (((int)srcB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				int Res = ((temp >> 1) + dither) >> 15;
				Res = CLAMP_i16(Res);
				*destB[ct] = (short)(Res);

				srcB += 3;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int16_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			short *srcB = (short*)src;
			unsigned char**destB = (unsigned char**)dest;
			short temp;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				temp = *srcB;
				auto destCB = destB[ct];

				destCB[0] = 0;
				destCB[1] = (unsigned char)(temp);
				destCB[2] = (unsigned char)(temp >> 8);

				srcB ++;
				destB[ct] += 3;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int16_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			short *srcB = (short*)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = *srcB << 16;
				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void Int24_To_SplittedInt8_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			unsigned char *srcB = (unsigned char*)src;
			signed char**destB = (signed char**)dest;

			int temp, dither;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				temp = (((int)srcB[0]) << 8);
				temp = temp | (((int)srcB[1]) << 16);
				temp = temp | (((int)srcB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				*destB[ct] = (signed char)CLAMP_i8((((temp >> 1) + dither) >> 23));

				srcB += 3;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}
		void Int24_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			unsigned char *srcB = (unsigned char*)src;
			Int32** destB = (Int32**)dest;
			Int32 temp;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{

				temp = (((Int32)srcB[0]) << 8);
				temp = temp | (((Int32)srcB[1]) << 16);
				temp = temp | (((Int32)srcB[2]) << 24);

				*destB[ct] = temp;

				srcB += 3;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void Int16_To_SplittedInt8(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			short *srcB = (short*)src;
			signed char** destB = (signed char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (signed char)((*srcB) >> 8);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int8_To_SplittedInt16(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			signed char *srcB = (signed char*)src;
			short**destB = (short**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (short)((*srcB) << 8);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int8_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			signed char *srcB = (signed char*)src;
			unsigned char**destB = (unsigned char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				auto destCB = destB[ct];

				destCB[0] = 0;
				destCB[1] = 0;
				destCB[2] = (*srcB);

				srcB ++;
				destB[ct] += 3;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int8_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			signed char *srcB = (signed char*)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) << 24;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void Int8_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int8 *srcB = (Int8*)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) * const_1_div_128_f;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int16_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int16 *srcB = (Int16*)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) * const_1_div_32768_f;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int24_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int24 *srcB = (Int24*)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB).operator float() * const_1_div_8388608_f;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int32_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int32 *srcB = (Int32*)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) * const_1_div_2147483648_f;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int8_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int8 *srcB = (Int8*)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) * const_1_div_128_d;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int16_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int16 *srcB = (Int16*)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) * const_1_div_32768_d;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int24_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int24 *srcB = (Int24*)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB).operator float() * const_1_div_8388608_d;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Int32_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			Int32 *srcB = (Int32*)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB) * const_1_div_2147483648_d;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float32_To_SplittedInt8_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float *srcB = (float*)src;
			signed char**destB = (signed char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (126.0f)) + dither;
				*destB[ct] = (signed char)CLAMP_f8(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float64_To_SplittedInt8_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double *srcB = (double*)src;
			signed char**destB = (signed char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (126.0)) + dither;
				*destB[ct] = (signed char)CLAMP_d8(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void Float32_To_SplittedInt16_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float *srcB = (float*)src;
			Int16** destB = (Int16**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (32766.0f)) + dither;
				*destB[ct] = (Int16)CLAMP_f16(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float64_To_SplittedInt16_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double *srcB = (double*)src;
			Int16**destB = (Int16**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (32766.0f)) + dither;
				*destB[ct] = (Int16)CLAMP_d16(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float32_To_SplittedInt24_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float *srcB = (float*)src;
			Int24**destB = (Int24**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (8388606.0f)) + dither;
				*destB[ct] = (int)CLAMP_f24(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float64_To_SplittedInt24_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double *srcB = (double*)src;
			Int24**destB = (Int24**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (8388606.0f)) + dither;

				*destB[ct] = (int)CLAMP_d24(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float32_To_SplittedInt32_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float *srcB = (float*)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (2147483646.0f)) + dither;
				*destB[ct] = (Int32)CLAMP_f32(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void Float64_To_SplittedInt32_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double *srcB = (double*)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (2147483646.0f)) + dither;
				*destB[ct] = (Int32)CLAMP_d32(dithered);

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void Float32_To_SplittedFloat64(void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			float *srcB = (float*)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = *srcB;

				srcB ++;
				destB[ct] ++;

				ct ++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void Float64_To_SplittedFloat32(void** dest,
			const void *src,
			unsigned int count, unsigned short ch)
		{
			double *srcB = (double*)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = *srcB;

				srcB ++;
				destB[ct] ++;
				ct ++;
				if (ct > ch_upper) ct = 0;
			}

		}

#pragma endregion

#pragma region Splitted to Splitted
		void SplittedInt32_To_SplittedInt24(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			int**srcB = (int**)src;
			unsigned char** destB = (unsigned char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				auto destCB = destB[ct];
				auto srcCB = srcB[ct];

				destCB[0] = (unsigned char)(*srcCB >> 8);
				destCB[1] = (unsigned char)(*srcCB >> 16);
				destCB[2] = (unsigned char)(*srcCB >> 24);

				srcB[ct] ++;
				destB[ct] += 3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_SplittedInt16_Dither(
			void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			int**srcB = (int**)src;
			short**destB = (short**)dest;
			int dither;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB[ct] = (short)CLAMP_i16(((((*srcB[ct]) >> 1) + dither) >> 15));

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_SplittedInt8_Dither(
			void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			int**srcB = (int**)src;
			signed char**destB = (signed char**)dest;
			int dither;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB[ct] = (signed char)CLAMP_i8(((((*srcB[ct]) >> 1) + dither) >> 23));

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_SplittedInt16_Dither(
			void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			short**destB = (short**)dest;

			int temp, dither;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				auto srcCB = srcB[ct];

				temp = (((int)srcCB[0]) << 8);
				temp = temp | (((int)srcCB[1]) << 16);
				temp = temp | (((int)srcCB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				int Res = ((temp >> 1) + dither) >> 15;
				Res = CLAMP_i16(Res);
				*destB[ct] = (short)(Res);

				srcB[ct] += 3;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_SplittedInt24(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			unsigned char**destB = (unsigned char**)dest;
			short temp;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				temp = *srcB[ct];
				auto destCB = destB[ct];

				destCB[0] = 0;
				destCB[1] = (unsigned char)(temp);
				destCB[2] = (unsigned char)(temp >> 8);

				srcB[ct] ++;
				destB[ct] += 3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_SplittedInt32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = *srcB[ct] << 16;
				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void SplittedInt24_To_SplittedInt8_Dither(
			void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			signed char**destB = (signed char**)dest;

			int temp, dither;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				auto srcCB = srcB[ct];

				temp = (((int)srcCB[0]) << 8);
				temp = temp | (((int)srcCB[1]) << 16);
				temp = temp | (((int)srcCB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				*destB[ct] = (signed char)CLAMP_i8((((temp >> 1) + dither) >> 23));

				srcB[ct] += 3;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}
		void SplittedInt24_To_SplittedInt32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			Int32** destB = (Int32**)dest;
			Int32 temp;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				auto srcCB = srcB[ct];

				temp = (((Int32)srcCB[0]) << 8);
				temp = temp | (((Int32)srcCB[1]) << 16);
				temp = temp | (((Int32)srcCB[2]) << 24);

				*destB[ct] = temp;

				srcB[ct] += 3;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedInt16_To_SplittedInt8(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			signed char** destB = (signed char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (signed char)((*srcB[ct]) >> 8);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_SplittedInt16(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			signed char**srcB = (signed char**)src;
			short**destB = (short**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (short)((*srcB[ct]) << 8);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_SplittedInt24(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			signed char**srcB = (signed char**)src;
			unsigned char**destB = (unsigned char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				auto destCB = destB[ct];

				destCB[0] = 0;
				destCB[1] = 0;
				destCB[2] = (*srcB[ct]);

				srcB[ct] ++;
				destB[ct] += 3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_SplittedInt32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			signed char**srcB = (signed char**)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) << 24;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedInt8_To_SplittedFloat32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int8**srcB = (Int8**)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) * const_1_div_128_f;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_SplittedFloat32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int16**srcB = (Int16**)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) * const_1_div_32768_f;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_SplittedFloat32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int24**srcB = (Int24**)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]).operator float() * const_1_div_8388608_f;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_SplittedFloat32(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int32**srcB = (Int32**)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) * const_1_div_2147483648_f;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_SplittedFloat64(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int8**srcB = (Int8**)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) * const_1_div_128_d;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_SplittedFloat64(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int16**srcB = (Int16**)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) * const_1_div_32768_d;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_SplittedFloat64(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int24**srcB = (Int24**)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]).operator float() * const_1_div_8388608_d;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_SplittedFloat64(
			void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int32**srcB = (Int32**)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = (*srcB[ct]) * const_1_div_2147483648_d;

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_SplittedInt8_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			signed char**destB = (signed char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (126.0f)) + dither;
				*destB[ct] = (signed char)CLAMP_f8(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_SplittedInt8_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			signed char**destB = (signed char**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (126.0)) + dither;
				*destB[ct] = (signed char)CLAMP_d8(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void SplittedFloat32_To_SplittedInt16_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			Int16** destB = (Int16**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (32766.0f)) + dither;
				*destB[ct] = (Int16)CLAMP_f16(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_SplittedInt16_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			Int16**destB = (Int16**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (32766.0f)) + dither;
				*destB[ct] = (Int16)CLAMP_d16(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_SplittedInt24_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			Int24**destB = (Int24**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (8388606.0f)) + dither;
				*destB[ct] = (int)CLAMP_f24(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_SplittedInt24_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			Int24**destB = (Int24**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (8388606.0f)) + dither;

				*destB[ct] = (int)CLAMP_d24(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_SplittedInt32_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (2147483646.0f)) + dither;
				*destB[ct] = (Int32)CLAMP_f32(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedFloat64_To_SplittedInt32_Dither(void** dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			Int32**destB = (Int32**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (2147483646.0f)) + dither;
				*destB[ct] = (Int32)CLAMP_d32(dithered);

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_SplittedFloat64(void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			float**srcB = (float**)src;
			double**destB = (double**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = *srcB[ct];

				srcB[ct] ++;
				destB[ct] ++;

				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedFloat64_To_SplittedFloat32(void** dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			double**srcB = (double**)src;
			float**destB = (float**)dest;
			auto ch_upper = ch- 1;
			int ct = 0;

			while (count--)
			{
				*destB[ct] = *srcB[ct];

				srcB[ct] ++;
				destB[ct] ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

#pragma endregion

#pragma region Splitted to Interleaved
		void SplittedInt8_To_Int8(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch)
		{
			Int8**srcB = (Int8**)src;
			Int8* destB = (Int8*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct]++;
				destB++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void SplittedInt16_To_Int16(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			short* destB = (short*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct]++;
				destB++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_Int24(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			unsigned char* destB = (unsigned char*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				auto srcCB = srcB[ct];

				destB[0] = srcCB[0];
				destB[1] = srcCB[1];
				destB[2] = srcCB[2];

				srcB[ct] += 3;
				destB += 3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_Int32(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch)
		{
			Int32**srcB = (Int32**)src;
			Int32* destB = (Int32*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct]++;
				destB++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_Float32(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch)
		{
			float**srcB = (float**)src;
			float* destB = (float*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct]++;
				destB++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_Float64(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch)
		{
			double**srcB = (double**)src;
			double* destB = (double*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct]++;
				destB++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_Int24(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			int**srcB = (int**)src;
			unsigned char* destB = (unsigned char*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;
			auto ch3 = 3 * ch;

			while (count--)
			{
				auto srcCB = srcB[ct];

				destB[0] = (unsigned char)(*srcCB >> 8);
				destB[1] = (unsigned char)(*srcCB >> 16);
				destB[2] = (unsigned char)(*srcCB >> 24);

				srcB[ct] ++;
				destB += 3;
				ct++;
				if (ct > ch_upper)
				{
					ct = 0;
					destB += ch3;
				}
			}
		}

		void SplittedInt32_To_Int16_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			int**srcB = (int**)src;
			short*destB = (short*)dest;
			int dither;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB = (short)CLAMP_i16(((((*srcB[ct]) >> 1) + dither) >> 15));

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_Int8_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			int**srcB = (int**)src;
			signed char*destB = (signed char*)dest;
			int dither;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB = (signed char)CLAMP_i8(((((*srcB[ct]) >> 1) + dither) >> 23));

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_Int16_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			short*destB = (short*)dest;

			int temp, dither;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				auto srcCB = srcB[ct];

				temp = (((int)srcCB[0]) << 8);
				temp = temp | (((int)srcCB[1]) << 16);
				temp = temp | (((int)srcCB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				int Res = ((temp >> 1) + dither) >> 15;
				Res = CLAMP_i16(Res);
				*destB = (short)(Res);

				srcB[ct] += 3;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_Int24(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			unsigned char*destB = (unsigned char*)dest;
			short temp;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				temp = *srcB[ct];

				destB[0] = 0;
				destB[1] = (unsigned char)(temp);
				destB[2] = (unsigned char)(temp >> 8);

				srcB[ct] ++;
				destB += 3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_Int32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			Int32*destB = (Int32*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = *srcB[ct] << 16;
				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void SplittedInt24_To_Int8_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			signed char*destB = (signed char*)dest;

			int temp, dither;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				auto srcCB = srcB[ct];

				temp = (((int)srcCB[0]) << 8);
				temp = temp | (((int)srcCB[1]) << 16);
				temp = temp | (((int)srcCB[2]) << 24);

				dither = Generate16BitDither(dithergen);
				*destB = (signed char)CLAMP_i8((((temp >> 1) + dither) >> 23));

				srcB[ct] += 3;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}
		void SplittedInt24_To_Int32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			unsigned char**srcB = (unsigned char**)src;
			Int32* destB = (Int32*)dest;
			Int32 temp;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				auto srcCB = srcB[ct];

				temp = (((Int32)srcCB[0]) << 8);
				temp = temp | (((Int32)srcCB[1]) << 16);
				temp = temp | (((Int32)srcCB[2]) << 24);

				*destB = temp;

				srcB[ct] += 3;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedInt16_To_Int8(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			short**srcB = (short**)src;
			signed char* destB = (signed char*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (signed char)((*srcB[ct]) >> 8);

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_Int16(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			signed char**srcB = (signed char**)src;
			short*destB = (short*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (short)((*srcB[ct]) << 8);

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_Int24(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			signed char**srcB = (signed char**)src;
			unsigned char*destB = (unsigned char*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				destB[0] = 0;
				destB[1] = 0;
				destB[2] = (*srcB[ct]);

				srcB[ct] ++;
				destB += 3;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_Int32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			signed char**srcB = (signed char**)src;
			Int32*destB = (Int32*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) << 24;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedInt8_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int8**srcB = (Int8**)src;
			float*destB = (float*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) * const_1_div_128_f;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int16**srcB = (Int16**)src;
			float*destB = (float*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) * const_1_div_32768_f;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int24**srcB = (Int24**)src;
			float*destB = (float*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]).operator float() * const_1_div_8388608_f;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int32**srcB = (Int32**)src;
			float*destB = (float*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) * const_1_div_2147483648_f;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt8_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int8**srcB = (Int8**)src;
			double*destB = (double*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) * const_1_div_128_d;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt16_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int16**srcB = (Int16**)src;
			double*destB = (double*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) * const_1_div_32768_d;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt24_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int24**srcB = (Int24**)src;
			double*destB = (double*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]).operator float() * const_1_div_8388608_d;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedInt32_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			Int32**srcB = (Int32**)src;
			double*destB = (double*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = (*srcB[ct]) * const_1_div_2147483648_d;

				srcB[ct] ++;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_Int8_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			signed char*destB = (signed char*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (126.0f)) + dither;
				*destB = (signed char)CLAMP_f8(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_Int8_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			signed char*destB = (signed char*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (126.0)) + dither;
				*destB = (signed char)CLAMP_d8(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}


		void SplittedFloat32_To_Int16_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			Int16* destB = (Int16*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (32766.0f)) + dither;
				*destB = (Int16)CLAMP_f16(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_Int16_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			Int16*destB = (Int16*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (32766.0f)) + dither;
				*destB = (Int16)CLAMP_d16(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_Int24_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			Int24*destB = (Int24*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (8388606.0f)) + dither;
				*destB = (int)CLAMP_f24(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat64_To_Int24_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			Int24*destB = (Int24*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (8388606.0f)) + dither;

				*destB = (int)CLAMP_d24(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_Int32_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			float**srcB = (float**)src;
			Int32*destB = (Int32*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB[ct] * (2147483646.0f)) + dither;
				*destB = (Int32)CLAMP_f32(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedFloat64_To_Int32_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch)
		{
			double**srcB = (double**)src;
			Int32*destB = (Int32*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB[ct] * (2147483646.0f)) + dither;
				*destB = (Int32)CLAMP_d32(dithered);

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}
		}

		void SplittedFloat32_To_Float64(void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			float**srcB = (float**)src;
			double*destB = (double*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct] + 1;
				destB ++;

				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

		void SplittedFloat64_To_Float32(void* dest,
			const void** src,
			unsigned int count, unsigned short ch)
		{
			double**srcB = (double**)src;
			float*destB = (float*)dest;
			auto ch_upper = ch - 1;
			int ct = 0;

			while (count--)
			{
				*destB = *srcB[ct];

				srcB[ct] + 1;
				destB ++;
				ct++;
				if (ct > ch_upper) ct = 0;
			}

		}

#pragma endregion


	}
}