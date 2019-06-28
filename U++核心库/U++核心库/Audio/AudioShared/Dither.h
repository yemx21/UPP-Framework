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

#pragma region Interleaved to Interleaved

		void Int32_To_Int24(
			void *dest,
			const void *src,
			unsigned int count);
		void Int32_To_Int16_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);
		void Int32_To_Int8_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);
		void Int32_To_Float32(
			void *dest,
			const void *src,
			unsigned int count);
		void Int32_To_Float64(
			void *dest,
			const void *src,
			unsigned int count);

		void Int24_To_Int32(
			void *dest,
			const void *src,
			unsigned int count);
		void Int24_To_Int16_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);
		void Int24_To_Int8_Dither(
			void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);
		void Int24_To_Float32(
			void *dest,
			const void *src,
			unsigned int count);

		void Int24_To_Float64(
			void *dest,
			const void *src,
			unsigned int count);

		void Int16_To_Int24(
			void *dest,
			const void *src,
			unsigned int count);
		void Int16_To_Int8(
			void *dest,
			const void *src,
			unsigned int count);
		void Int16_To_Int32(
			void *dest,
			const void *src,
			unsigned int count);
		void Int16_To_Float32(
			void *dest,
			const void *src,
			unsigned int count);
		void Int16_To_Float64(
			void *dest,
			const void *src,
			unsigned int count);

		void Int8_To_Int16(
			void *dest,
			const void *src,
			unsigned int count);
		void Int8_To_Int24(
			void *dest,
			const void *src,
			unsigned int count);
		void Int8_To_Int32(
			void *dest,
			const void *src,
			unsigned int count);
		void Int8_To_Float32(
			void *dest,
			const void *src,
			unsigned int count);
		void Int8_To_Float64(
			void *dest,
			const void *src,
			unsigned int count);

		void Float32_To_Int8_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float32_To_Int16_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float32_To_Int24_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float32_To_Int32_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int8_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int16_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int24_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Int32_Dither(void *dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen);

		void Float64_To_Float32(void *dest,
			const void *src,
			unsigned int count);

		void Float32_To_Float64(void *dest,
			const void *src,
			unsigned int count);

#pragma endregion

#pragma region Interleaved to Splitted
		void Int8_To_SplittedInt8(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int16_To_SplittedInt16(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int24_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int32_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Float32_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Float64_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Int32_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int32_To_SplittedInt16_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void Int32_To_SplittedInt8_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void Int32_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int32_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Int24_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int24_To_SplittedInt16_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void Int24_To_SplittedInt8_Dither(
			void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void Int24_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Int24_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Int16_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int16_To_SplittedInt8(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int16_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int16_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int16_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Int8_To_SplittedInt16(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int8_To_SplittedInt24(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int8_To_SplittedInt32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int8_To_SplittedFloat32(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);
		void Int8_To_SplittedFloat64(
			void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Float32_To_SplittedInt8_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float32_To_SplittedInt16_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float32_To_SplittedInt24_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float32_To_SplittedInt32_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float64_To_SplittedInt8_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float64_To_SplittedInt16_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float64_To_SplittedInt24_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float64_To_SplittedInt32_Dither(void** dest,
			const void *src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void Float64_To_SplittedFloat32(void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

		void Float32_To_SplittedFloat64(void** dest,
			const void *src,
			unsigned int count, unsigned short ch);

#pragma endregion

#pragma region Splitted to Splitted

		void SplittedInt32_To_SplittedInt24(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt32_To_SplittedInt16_Dither(
			void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt32_To_SplittedInt8_Dither(
			void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt32_To_SplittedFloat32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt32_To_SplittedFloat64(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedInt24_To_SplittedInt32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt24_To_SplittedInt16_Dither(
			void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt24_To_SplittedInt8_Dither(
			void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt24_To_SplittedFloat32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedInt24_To_SplittedFloat64(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedInt16_To_SplittedInt24(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_SplittedInt8(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_SplittedInt32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_SplittedFloat32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_SplittedFloat64(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedInt8_To_SplittedInt16(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_SplittedInt24(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_SplittedInt32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_SplittedFloat32(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_SplittedFloat64(
			void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedFloat32_To_SplittedInt8_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat32_To_SplittedInt16_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat32_To_SplittedInt24_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat32_To_SplittedInt32_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_SplittedInt8_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_SplittedInt16_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_SplittedInt24_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_SplittedInt32_Dither(void** dest,
			const void**src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_SplittedFloat32(void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedFloat32_To_SplittedFloat64(void** dest,
			const void**src,
			unsigned int count, unsigned short ch);

#pragma endregion

#pragma region Splitted to Interleaved
		void SplittedInt8_To_Int8(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_Int16(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt24_To_Int24(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedInt32_To_Int32(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedFloat32_To_Float32(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch);
		void SplittedFloat64_To_Float64(
			void* dest,
			const void**src,
			unsigned int count, unsigned short ch);

		void SplittedInt32_To_Int24(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt32_To_Int16_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt32_To_Int8_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt32_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt32_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

		void SplittedInt24_To_Int32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt24_To_Int16_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt24_To_Int8_Dither(
			void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);
		void SplittedInt24_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

		void SplittedInt24_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

		void SplittedInt16_To_Int24(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_Int8(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_Int32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt16_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

		void SplittedInt8_To_Int16(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_Int24(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_Int32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_Float32(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);
		void SplittedInt8_To_Float64(
			void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

		void SplittedFloat32_To_Int8_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat32_To_Int16_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat32_To_Int24_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat32_To_Int32_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_Int8_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_Int16_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_Int24_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_Int32_Dither(void* dest,
			const void** src,
			unsigned int count, DitherGenerator *dithergen, unsigned short ch);

		void SplittedFloat64_To_Float32(void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

		void SplittedFloat32_To_Float64(void* dest,
			const void** src,
			unsigned int count, unsigned short ch);

#pragma endregion

	}
}