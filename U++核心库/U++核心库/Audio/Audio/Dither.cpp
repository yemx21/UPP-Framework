#include "Dither.h"


using namespace System::Audio;

#define DITHER_BITS_   (15)

#define DITHER_BITS_1   (31)

#define DITHER_BITS_2   (61)


static const float const_1_div_128_f = 1.0f / 128.0f;  /* 8 bit multiplier */

static const float const_1_div_32768_f = 1.0f / 32768.0f; /* 16 bit multiplier */

static const float const_1_div_8388608_f = 1.0f / 8388608.0f; /* 24 bit multiplier */

static const float const_1_div_2147483648_f = 1.0f / 2147483648.0f; /* 32 bit multiplier */

static const double const_1_div_128_d = 1.0 / 128.0;  /* 8 bit multiplier */

static const double const_1_div_32768_d = 1.0 / 32768.0; /* 16 bit multiplier */

static const double const_1_div_8388608_d = 1.0 / 8388608.0; /* 24 bit multiplier */

static const double const_1_div_2147483648_d = 1.0 / 2147483648.0; /* 32 bit multiplier */

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

		int Generate16BitDither(DitherGenerator *state)
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

#define FLOAT_DITHER_SCALE_  (1.0f / ((1ll<<DITHER_BITS_1)-1ll))
		static const float const_float_dither_scale_ = FLOAT_DITHER_SCALE_;
		float GenerateFloatDither(DitherGenerator *state)
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

		double GenerateDoubleDither(DitherGenerator *state)
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
		void Int32_To_Int24(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			int *srcB = (int*)src;
			unsigned char *destB = (unsigned char*)dest;

			while (count--)
			{
				destB[0] = (unsigned char)(*srcB >> 8);
				destB[1] = (unsigned char)(*srcB >> 16);
				destB[2] = (unsigned char)(*srcB >> 24);

				srcB += srcstride;
				destB += deststride * 3;
			}
		}

		void Int32_To_Int16_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			int *srcB = (int*)src;
			short *destB = (short*)dest;
			int dither;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB = (short)CLAMP_i16(((((*srcB) >> 1) + dither) >> 15));

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int32_To_Int8_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			int *srcB = (int*)src;
			signed char *destB = (signed char*)dest;
			int dither;

			while (count--)
			{
				dither = Generate16BitDither(dithergen);
				*destB = (signed char)CLAMP_i8(((((*srcB) >> 1) + dither) >> 23));

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int24_To_Int16_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
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

				srcB += srcstride * 3;
				destB += deststride;
			}
		}

		void Int16_To_Int24(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
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
				srcB += srcstride;
				destB += deststride * 3;
			}
		}

		void Int16_To_Int32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			short *srcB = (short*)src;
			Int32 *destB = (Int32*)dest;

			while (count--)
			{
				*destB = *srcB << 16;
				srcB += srcstride;
				destB += deststride;
			}
		}


		void Int24_To_Int8_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
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

				srcB += srcstride * 3;
				destB += deststride;
			}
		}
		void Int24_To_Int32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
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

				srcB += srcstride * 3;
				destB += deststride;
			}

		}

		void Int16_To_Int8(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			short *srcB = (short*)src;
			signed char *destB = (signed char*)dest;
			while (count--)
			{
				*destB = (signed char)((*srcB) >> 8);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int8_To_Int16(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			signed char *srcB = (signed char*)src;
			short *destB = (short*)dest;

			while (count--)
			{
				*destB = (short)((*srcB) << 8);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int8_To_Int24(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			signed char *srcB = (signed char*)src;
			unsigned char *destB = (unsigned char*)dest;
			while (count--)
			{
				destB[0] = 0;
				destB[1] = 0;
				destB[2] = (*srcB);

				srcB += srcstride;
				destB += deststride * 3;
			}
		}

		void Int8_To_Int32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			signed char *srcB = (signed char*)src;
			Int32 *destB = (Int32*)dest;
			while (count--)
			{
				(*destB) = (*srcB) << 24;

				srcB += srcstride;
				destB += deststride;
			}

		}

		void Int8_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int8 *srcB = (Int8*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_128_f;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int16_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int16 *srcB = (Int16*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_32768_f;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int24_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int24 *srcB = (Int24*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB).operator float() * const_1_div_8388608_f;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int32_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int32 *srcB = (Int32*)src;
			float *destB = (float*)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_2147483648_f;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int8_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int8 *srcB = (Int8*)src;
			double *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_128_d;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int16_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int16 *srcB = (Int16*)src;
			double  *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_32768_d;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int24_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int24 *srcB = (Int24*)src;
			double  *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB).operator float() * const_1_div_8388608_d;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Int32_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			Int32 *srcB = (Int32*)src;
			double  *destB = (double *)dest;
			while (count--)
			{
				(*destB) = (*srcB) * const_1_div_2147483648_d;

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float32_To_Int8_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			signed char *destB = (signed char*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (126.0f)) + dither;
				*destB = (signed char)CLAMP_f8(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float64_To_Int8_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			signed char *destB = (signed char*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (126.0)) + dither;
				*destB = (signed char)CLAMP_d8(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}


		void Float32_To_Int16_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			Int16 *destB = (Int16*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (32766.0f)) +dither;
				*destB = (Int16)CLAMP_f16(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float64_To_Int16_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			Int16 *destB = (Int16*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (32766.0f)) + dither;
				*destB = (Int16)CLAMP_d16(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float32_To_Int24_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			Int24 *destB = (Int24*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (8388606.0f)) + dither;
				(*destB) = (int)CLAMP_f24(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float64_To_Int24_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			Int24 *destB = (Int24*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (8388606.0f)) + dither;

				(*destB) = (int)CLAMP_d24(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float32_To_Int32_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			float *srcB = (float*)src;
			Int32 *destB = (Int32*)dest;

			while (count--)
			{
				float dither = GenerateFloatDither(dithergen);
				float dithered = (*srcB * (2147483646.0f)) +dither;
				*destB = (Int32)CLAMP_f32(dithered);

				srcB += srcstride;
				destB += deststride;
			}

		}

		void Float64_To_Int32_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen)
		{
			double *srcB = (double*)src;
			Int32 *destB = (Int32*)dest;

			while (count--)
			{
				double dither = GenerateDoubleDither(dithergen);
				double dithered = (*srcB * (2147483646.0f)) + dither;
				*destB = (Int32)CLAMP_d32(dithered);

				srcB += srcstride;
				destB += deststride;
			}
		}

		void Float32_To_Float64(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			float *srcB = (float*)src;
			double *destB = (double*)dest;

			while (count--)
			{
				*destB = *srcB;

				srcB += srcstride;
				destB += deststride;
			}

		}

		void Float64_To_Float32(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count)
		{
			double *srcB = (double*)src;
			float *destB = (float*)dest;

			while (count--)
			{
				*destB = *srcB;

				srcB += srcstride;
				destB += deststride;
			}

		}
	}
}