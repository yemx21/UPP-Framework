#pragma once
#include "Config.h"

namespace System
{
	namespace Audio
	{
		typedef struct DitherGenerator
		{
			UInt previous;
			UInt randSeed1;
			UInt randSeed2;
		} DitherGenerator;

		void InitializeDitherState(DitherGenerator *ditherState);
		int Generate16BitDither(DitherGenerator *ditherState);
		float GenerateFloatDither(DitherGenerator *ditherState);
		double GenerateDoubleDither(DitherGenerator *ditherState);

		void Int32_To_Int24(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int32_To_Int16_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);
		void Int32_To_Int8_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);
		void Int32_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int32_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);

		void Int24_To_Int32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int24_To_Int16_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);
		void Int24_To_Int8_Dither(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);
		void Int24_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);

		void Int24_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);

		void Int16_To_Int24(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int16_To_Int8(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int16_To_Int32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int16_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int16_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);

		void Int8_To_Int16(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int8_To_Int24(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int8_To_Int32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int8_To_Float32(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
		void Int8_To_Float64(
			void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);

		void Float32_To_Int8_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float32_To_Int16_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float32_To_Int24_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float32_To_Int32_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int8_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int16_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int24_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int32_Dither(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Float32(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);

		void Float32_To_Float64(void *dest, signed int deststride,
			const void *src, signed int srcstride,
			unsigned int count);
	}
}