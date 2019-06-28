#pragma once
#include "AudioTypes.h"
namespace System
{
	namespace Audio
	{
		class AudioTree;
		class ReplayGainTask;
		class AudioBuffer;
		struct DitherGenerator;
		class AudioChunk_impl;

		#pragma region AudioInterleavedChunk

		template<typename T, class ALLOC>
		class AudioInterleavedChunk;

		template<>
		class AudioInterleavedChunk <UInt8, AlignedMemoryBlock<UInt8, 16>>
		{
			AlignedMemoryBlock<UInt8, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			UInt8* Ptr()
			{
				return block.ptr();
			}

			const UInt8* Ptr() const
			{
				return block.ptr();
			}

			UInt8* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const UInt8* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(UInt8));
			}
			
			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(UInt8));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <UInt16, AlignedMemoryBlock<UInt16, 16>>
		{
			AlignedMemoryBlock<UInt16, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			UInt16* Ptr()
			{
				return block.ptr();
			}

			const UInt16* Ptr() const
			{
				return block.ptr();
			}

			UInt16* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const UInt16* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(UInt16));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(UInt16));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <UInt24, AlignedMemoryBlock<UInt24, 16>>
		{
			AlignedMemoryBlock<UInt24, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			UInt24* Ptr()
			{
				return block.ptr();
			}

			const UInt24* Ptr() const
			{
				return block.ptr();
			}

			UInt24* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const UInt24* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(UInt24));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(UInt24));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <UInt32, AlignedMemoryBlock<UInt32, 16>>
		{
			AlignedMemoryBlock<UInt32, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}
			UInt32* Ptr()
			{
				return block.ptr();
			}

			const UInt32* Ptr() const
			{
				return block.ptr();
			}


			UInt32* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const UInt32* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(UInt32));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(UInt32));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <Int8, AlignedMemoryBlock<Int8, 16>>
		{
			AlignedMemoryBlock<Int8, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			Int8* Ptr()
			{
				return block.ptr();
			}

			const Int8* Ptr() const
			{
				return block.ptr();
			}

			Int8* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const Int8* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(Int8));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(Int8));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <Int16, AlignedMemoryBlock<Int16, 16>>
		{
			AlignedMemoryBlock<Int16, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) =default;
			~AudioInterleavedChunk() {}

			Int16* Ptr()
			{
				return block.ptr();
			}

			const Int16* Ptr() const
			{
				return block.ptr();
			}

			Int16* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const Int16* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(Int16));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(Int16));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <Int24, AlignedMemoryBlock<Int24, 16>>
		{
			AlignedMemoryBlock<Int24, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}
			Int24* Ptr()
			{
				return block.ptr();
			}

			const Int24* Ptr() const
			{
				return block.ptr();
			}

			Int24* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const Int24* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(Int24));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(Int24));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <Int32, AlignedMemoryBlock<Int32, 16>>
		{
			AlignedMemoryBlock<Int32, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			Int32* Ptr()
			{
				return block.ptr();
			}

			const Int32* Ptr() const
			{
				return block.ptr();
			}

			Int32* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const Int32* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(Int32));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(Int32));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <Float32, AlignedMemoryBlock<Float32, 16>>
		{
			AlignedMemoryBlock<Float32, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			Float32* Ptr()
			{
				return block.ptr();
			}

			const Float32* Ptr() const
			{
				return block.ptr();
			}

			Float32* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const Float32* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(Float32));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(Float32));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		template<>
		class AudioInterleavedChunk <Float64, AlignedMemoryBlock<Float64, 16>>
		{
			AlignedMemoryBlock<Float64, 16> block;
			size_t blockvalid;
		public:

			AudioInterleavedChunk() :blockvalid(0) {}
			AudioInterleavedChunk(const AudioInterleavedChunk& chunk)
			{
				block = chunk.block;
				blockvalid = chunk.blockvalid;
			}
			AudioInterleavedChunk(AudioInterleavedChunk&& chunk)
			{
				block = std::move(chunk.block);
				blockvalid = chunk.blockvalid;
			}
			inline AudioInterleavedChunk& operator=(const AudioInterleavedChunk&) = default;
			~AudioInterleavedChunk() {}

			Float64* Ptr()
			{
				return block.ptr();
			}

			const Float64* Ptr() const
			{
				return block.ptr();
			}

			Float64* Ptr(size_t& bytes)
			{
				bytes = blockvalid;
				return block.ptr();
			}

			const Float64* Ptr(size_t& bytes) const
			{
				bytes = blockvalid;
				return block.ptr();
			}

			size_t Size() const
			{
				return block.size();
			}

			size_t Valid() const
			{
				return blockvalid;
			}

			void EnsureSize(size_t bytes)
			{
				if (bytes > block.size()) block.resize(bytes / sizeof(Float64));
			}

			void SetValid(size_t bytes)
			{
				blockvalid = bytes;
			}

			void Write(void* data, size_t bytes)
			{
				size_t need = blockvalid + bytes;
				if (need > block.size()) block.resize(bytes / sizeof(Float64));
				memcpy((char*)block.ptr() + blockvalid, data, bytes);
				blockvalid += bytes;
			}

			void Clear()
			{
				blockvalid = 0;
			}

			void Reset()
			{
				block.resize(0);
				blockvalid = 0;
			}
		};

		typedef  AudioInterleavedChunk <Int8, AlignedMemoryBlock<Int8, 16>> IChunk;
		typedef  AudioInterleavedChunk <UInt8, AlignedMemoryBlock<UInt8, 16>> IChunkU8;
		typedef  AudioInterleavedChunk <UInt16, AlignedMemoryBlock<UInt16, 16>> IChunkU16;
		typedef  AudioInterleavedChunk <UInt24, AlignedMemoryBlock<UInt24, 16>> IChunkU24;
		typedef  AudioInterleavedChunk <UInt32, AlignedMemoryBlock<UInt32, 16>> IChunkU32;
		typedef  AudioInterleavedChunk <Int8, AlignedMemoryBlock<Int8, 16>> IChunk8;
		typedef  AudioInterleavedChunk <Int16, AlignedMemoryBlock<Int16, 16>> IChunk16;
		typedef  AudioInterleavedChunk <Int24, AlignedMemoryBlock<Int24, 16>> IChunk24;
		typedef  AudioInterleavedChunk <Int32, AlignedMemoryBlock<Int32, 16>> IChunk32;
		typedef  AudioInterleavedChunk <Float32, AlignedMemoryBlock<Float32, 16>> IChunkF32;
		typedef  AudioInterleavedChunk <Float64, AlignedMemoryBlock<Float64, 16>> IChunkF64;

		#pragma endregion

		#pragma region AudioSplitedChunk
		template<typename T, class ALLOC>
		class AudioSplitedChunk;

		template<>
		class AudioSplitedChunk <UInt8, AlignedMemoryBlock<UInt8, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<UInt8*, 16> blocks;
			AlignedMemoryBlock<UInt8, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				UInt8** bs = blocks.ptr();
				if (bs)
				{
					UInt8* rawptr = block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = rawptr + r* perbytes;
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			UInt8** Ptr()
			{
				return blocks.ptr();
			}

			const UInt8** Ptr() const
			{
				return const_cast<const UInt8**>(blocks.ptr());
			}

			UInt8** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const UInt8** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const UInt8**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			UInt8* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const UInt8* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					UInt8* rawptr = block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				UInt8** bs = blocks.ptr();
				if (bs)
				{
					UInt8* rawptr = block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = rawptr + r* perbytes;
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}

			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				UInt8* rawptr = block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <UInt16, AlignedMemoryBlock<UInt16, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<UInt16*, 16> blocks;
			AlignedMemoryBlock<UInt16, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				UInt16** bs = blocks.ptr();
				if (bs)
				{
					UChar* rawptr = (UChar*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] =(UInt16*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			UInt16** Ptr()
			{
				return blocks.ptr();
			}

			const UInt16** Ptr() const
			{
				return const_cast<const UInt16**>(blocks.ptr());
			}

			UInt16** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const UInt16** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const UInt16**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			UInt16* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const UInt16* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					UChar* rawptr = (UChar*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				UInt16** bs = blocks.ptr();
				if (bs)
				{
					UChar* rawptr = (UChar*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (UInt16*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}

			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				UChar* rawptr = (UChar*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <UInt24, AlignedMemoryBlock<UInt24, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<UInt24*, 16> blocks;
			AlignedMemoryBlock<UInt24, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				UInt24** bs = blocks.ptr();
				if (bs)
				{
					UChar* rawptr = (UChar*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (UInt24*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			UInt24** Ptr()
			{
				return blocks.ptr();
			}

			const UInt24** Ptr() const
			{
				return const_cast<const UInt24**>(blocks.ptr());
			}

			UInt24** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const UInt24** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const UInt24**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			UInt24* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const UInt24* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					UChar* rawptr = (UChar*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				UInt24** bs = blocks.ptr();
				if (bs)
				{
					UChar* rawptr = (UChar*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (UInt24*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}

			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				UChar* rawptr = (UChar*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <UInt32, AlignedMemoryBlock<UInt32, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<UInt32*, 16> blocks;
			AlignedMemoryBlock<UInt32, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				UInt32** bs = blocks.ptr();
				if (bs)
				{
					UChar* rawptr = (UChar*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (UInt32*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			UInt32** Ptr()
			{
				return blocks.ptr();
			}

			const UInt32** Ptr() const
			{
				return const_cast<const UInt32**>(blocks.ptr());
			}

			UInt32** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const UInt32** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const UInt32**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			UInt32* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const UInt32* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					UChar* rawptr = (UChar*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				UInt32** bs = blocks.ptr();
				if (bs)
				{
					UChar* rawptr = (UChar*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (UInt32*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}

			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				UChar* rawptr = (UChar*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <Int8, AlignedMemoryBlock<Int8, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<Int8*, 16> blocks;
			AlignedMemoryBlock<Int8, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				Int8** bs = blocks.ptr();
				if (bs)
				{
					Int8* rawptr = block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = rawptr + r* perbytes;
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			Int8** Ptr()
			{
				return blocks.ptr();
			}

			const Int8** Ptr() const
			{
				return const_cast<const Int8**>(blocks.ptr());
			}

			Int8** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const Int8** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const Int8**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			Int8* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const Int8* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					Int8* rawptr = block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				Int8** bs = blocks.ptr();
				if (bs)
				{
					Int8* rawptr = block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = rawptr + r* perbytes;
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				Int8* rawptr = block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <Int16, AlignedMemoryBlock<Int16, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<Int16*, 16> blocks;
			AlignedMemoryBlock<Int16, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				Int16** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (Int16*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			Int16** Ptr()
			{
				return blocks.ptr();
			}

			const Int16** Ptr() const
			{
				return const_cast<const Int16**>(blocks.ptr());
			}

			Int16** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const Int16** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const Int16**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			Int16* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const Int16* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					Char* rawptr = (Char*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				Int16** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (Int16*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				Char* rawptr = (Char*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <Int24, AlignedMemoryBlock<Int24, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<Int24*, 16> blocks;
			AlignedMemoryBlock<Int24, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				Int24** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (Int24*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			Int24** Ptr()
			{
				return blocks.ptr();
			}

			const Int24** Ptr() const
			{
				return const_cast<const Int24**>(blocks.ptr());
			}

			Int24** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const Int24** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const Int24**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			Int24* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const Int24* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					Char* rawptr = (Char*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				Int24** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (Int24*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}

			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				Char* rawptr = (Char*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <Int32, AlignedMemoryBlock<Int32, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<Int32*, 16> blocks;
			AlignedMemoryBlock<Int32, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				Int32** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (Int32*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			Int32** Ptr()
			{
				return blocks.ptr();
			}

			const Int32** Ptr() const
			{
				return const_cast<const Int32**>(blocks.ptr());
			}

			Int32** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const Int32** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const Int32**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			Int32* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const Int32* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					Char* rawptr = (Char*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				Int32** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (Int32*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}

			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				Char* rawptr = (Char*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <Float32, AlignedMemoryBlock<Float32, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<Float32*, 16> blocks;
			AlignedMemoryBlock<Float32, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				Float32** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (Float32*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			Float32** Ptr()
			{
				return blocks.ptr();
			}

			const Float32** Ptr() const
			{
				return const_cast<const Float32**>(blocks.ptr());
			}

			Float32** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const Float32** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const Float32**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			Float32* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const Float32* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					Char* rawptr = (Char*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				Float32** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (Float32*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				Char* rawptr = (Char*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		template<>
		class AudioSplitedChunk <Float64, AlignedMemoryBlock<Float64, 16>>
		{
			AlignedMemoryBlock<size_t, 16> record;
			AlignedMemoryBlock<Float64*, 16> blocks;
			AlignedMemoryBlock<Float64, 16> block;
		public:

			AudioSplitedChunk() {}

			AudioSplitedChunk(const AudioSplitedChunk& chunk)
			{
				block = chunk.block;
				record = chunk.record;
				blocks.resize(record.count());
				Float64** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr && record.count())
					{
						size_t perbytes = block.size() / record.count();
						for (size_t r = 0; r < record.count(); r++)
						{
							bs[r] = (Float64*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			AudioSplitedChunk(AudioSplitedChunk&& chunk)
			{
				block = std::move(chunk.block);
				record = std::move(chunk.record);
				blocks = std::move(chunk.blocks);
			}

			inline AudioSplitedChunk& operator=(const AudioSplitedChunk&) = default;
			~AudioSplitedChunk() {}

			Float64** Ptr()
			{
				return blocks.ptr();
			}

			const Float64** Ptr() const
			{
				return const_cast<const Float64**>(blocks.ptr());
			}

			Float64** Ptr(size_t& rcount)
			{
				rcount = record.count();
				return blocks.ptr();
			}

			const Float64** Ptr(size_t& rcount) const
			{
				rcount = record.count();
				return const_cast<const Float64**>(blocks.ptr());
			}

			size_t Size(size_t index = 0) const
			{
				return index < record.count() ? record.ptr()[index] : 0;
			}

			Float64* Ptr(size_t index, size_t& bytes)
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			const Float64* Ptr(size_t index, size_t& bytes) const
			{
				return index < record.count() ? blocks.ptr()[index] : nullptr;
			}

			void EnsureSize(size_t perbytes, size_t recordcount)
			{
				size_t need = perbytes*recordcount;
				size_t lastrecordcount = record.count();
				size_t lastblocksize = block.size();
				if (need > block.size()) block.resize(need);
				if (lastrecordcount == 0)
				{
					if (recordcount > 0)
					{
						record.resize(recordcount);
						memset(record.ptr(), 0, sizeof(size_t)* recordcount);
					}
					else
					{
						record.resize(0);
						blocks.resize(0);
					}
				}
				else
				{
					Char* rawptr = (Char*)block.ptr();
					size_t* recordptr = record.ptr();
					size_t lastperbytes = lastblocksize / lastrecordcount;
					for (int r = (lastrecordcount <recordcount ? lastrecordcount : recordcount) - 1; r >-1; r--)
					{
						memmove(rawptr + r* perbytes, rawptr + r* lastperbytes, recordptr[r]);
					}
					record.resize(recordcount);
					if (recordcount > lastrecordcount)
					{
						recordptr = record.ptr();
						for (size_t nr = lastrecordcount; nr < recordcount; nr++)
						{
							recordptr[nr] = 0;
						}
					}
				}
				if (recordcount >lastrecordcount) blocks.resize(recordcount);
				Float64** bs = blocks.ptr();
				if (bs)
				{
					Char* rawptr = (Char*)block.ptr();
					if (rawptr)
					{
						for (size_t r = 0; r < recordcount; r++)
						{
							bs[r] = (Float64*)(rawptr + r* perbytes);
						}
					}
					else
					{
						for (size_t bsc = 0; bsc < blocks.count(); bsc++)
						{
							bs[bsc] = 0;
						}
					}
				}
			}

			void SetValid(size_t perbytes, size_t recordcount)
			{
				if (recordcount <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < recordcount; r++)
					{
						rs[r] = perbytes;
					}
				}
			}

			void SetValid(const std::vector<size_t>& bytes)
			{
				if (bytes.size() <= record.count())
				{
					size_t* rs = record.ptr();
					for (size_t r = 0; r < bytes.size(); r++)
					{
						rs[r] = bytes[r];
					}
				}
			}

			size_t Valid() const
			{
				size_t ret = 0;
				const size_t* rs = record.ptr();
				for (size_t r = 0; r < record.count(); r++)
				{
					ret += rs[r];
				}
				return ret;
			}

			void Write(void* data, size_t bytes, size_t index)
			{
				size_t needperbytes = bytes;
				size_t* rs = record.ptr();
				for (size_t v = 0; v< record.count(); v++)
				{
					size_t r = rs[v];
					if (v == index) r += bytes;
					if (r>needperbytes) needperbytes = r;
				}
				EnsureSize(needperbytes, index + 1> record.count() ? index + 1 : record.count());
				Char* rawptr = (Char*)block.ptr();
				if (rawptr)
				{
					size_t perbytes = block.size() / record.count();
					rs = record.ptr();
					memcpy(rawptr + index* perbytes + rs[index], data, bytes);
					rs[index] += bytes;
				}
			}

			void Clear()
			{
				size_t* rs = record.ptr();
				if (rs) memset(rs, 0, record.size());
			}

			void Reset()
			{
				block.resize(0);
				record.resize(0);
			}
		};

		typedef AudioSplitedChunk <Int8, AlignedMemoryBlock<Int8, 16>> SChunk;
		typedef AudioSplitedChunk <UInt8, AlignedMemoryBlock<UInt8, 16>> SChunkU8;
		typedef AudioSplitedChunk <UInt16, AlignedMemoryBlock<UInt16, 16>> SChunkU16;
		typedef AudioSplitedChunk <UInt24, AlignedMemoryBlock<UInt24, 16>> SChunkU24;
		typedef AudioSplitedChunk <UInt32, AlignedMemoryBlock<UInt32, 16>> SChunkU32;
		typedef AudioSplitedChunk <Int8, AlignedMemoryBlock<Int8, 16>> SChunk8;
		typedef AudioSplitedChunk <Int16, AlignedMemoryBlock<Int16, 16>> SChunk16;
		typedef AudioSplitedChunk <Int24, AlignedMemoryBlock<Int24, 16>> SChunk24;
		typedef AudioSplitedChunk <Int32, AlignedMemoryBlock<Int32, 16>> SChunk32;
		typedef AudioSplitedChunk <Float32, AlignedMemoryBlock<Float32, 16>> SChunkF32;
		typedef AudioSplitedChunk <Float64, AlignedMemoryBlock<Float64, 16>> SChunkF64;
		#pragma endregion

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioChunk final: _noncopyable, __uobject
		{
		private:
			friend AudioChunk_impl;
			friend AudioBuffer;
			AudioChunk_impl* impl;
			AudioChunk();
		public:
			~AudioChunk();

			__uref void* Interleaved();
			__uref void** Splitted();
			__uref bool GetInterleaved() const;
			__uref void SetInterleaved(bool interleaved = true);

			__uref Result<bool> EnsureSampleCount(UInt samplecount, bool interleaved = true);

			__uref BitsPerSample GetBitsPerSample()const;
			__uref UShort GetChannels() const;
			__uref UInt GetSampleCount() const;
			__uref void SetSampleCount(UInt samplecount);
			__uref UInt GetSampleRate() const;
			__uref Result<bool> SetSampleRate(UInt samplecount);


			Result<bool> Reset(UInt sampleRate, BitsPerSample bitsPerSample, Audio::ChannelLayout channelLayout, bool interleaved = true);

			REFLECT_CLASS(AudioChunk)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API AudioBuffer final: _noncopyable,__uobject
		{
		private:
			friend AudioChunk_impl;
			friend class AudioTree;
			friend class ReplayGainTask;

			Optional<bool> interleavedchunk;
			Audio::BitsPerSample bps;
			Audio::ChannelLayout chlayout;
			UShort bpsnum;
			UShort ch;
			UInt sr;
			
			DitherGenerator* dither;
			void* internal;

			IChunk8 ipool8;
			IChunk16 ipool16;
			IChunk24 ipool24;
			IChunk32 ipool32;
			IChunkF32 ipoolf32;
			IChunkF64 ipoolf64;

			SChunk8 spool8;
			SChunk16 spool16;
			SChunk24 spool24;
			SChunk32 spool32;
			SChunkF32 spoolf32;
			SChunkF64 spoolf64;

			IChunk ipoolout;
			SChunk spoolout;

			UInt samplecount;
			double duration;
			double startms;

			AudioBuffer(UInt sampleRate, BitsPerSample bitsPerSample, Audio::ChannelLayout channelLayout);

			Result<bool> Reset(UInt sampleRate, BitsPerSample bitsPerSample, Audio::ChannelLayout channelLayout);
		public:
			~AudioBuffer();

			static bool IsValid_SampleRate(UInt sampleRate) { return sampleRate >= SAMPLERATE_MIN && sampleRate <= SAMPLERATE_MAX; }
			static bool IsValid_Channels(UShort channel) { return channel >= CHANNELS_MIN && channel <= CHANNELS_MAX; }

			__uref Result<bool> SetData(void* data, UInt sampleRate, Audio::BitsPerSample bitsPerSample, UInt samples, Audio::ChannelLayout channelLayout, double startMs);
			__uref Result<bool> SetSplittedData(void** data, UInt sampleRate, Audio::BitsPerSample bitsPerSample, UInt samples, Audio::ChannelLayout channelLayout, double startMs);

			Result<bool> StartResetData(UInt sampleRate, Audio::BitsPerSample bitsPerSample, Audio::ChannelLayout channelLayout, double startMs);
			Result<bool> AddData(void* data, UInt samples);
			Result<bool> AddSplittedData(void** data, UInt samples);
			Result<bool> CloseResetData(bool rollBackBeforeReset = false);

			__uref void Clear();

			Result<void*> GetPool(UInt64& bytes, Audio::SampleInfo& OutInfo);
			Result<void**> GetSplittedPool(UInt64& bytes, Audio::SampleInfo& OutInfo);

			__uref Result<bool> EnsurePool8(bool interleaved = true);
			__uref Result<bool> EnsurePool16(bool interleaved = true);
			__uref Result<bool> EnsurePool24(bool interleaved = true);
			__uref Result<bool> EnsurePool32(bool interleaved = true);
			__uref Result<bool> EnsureFloatPool(bool interleaved = true);
			__uref Result<bool> EnsureDoublePool(bool interleaved = true);

			UInt GetSampleRate()const;
			UShort GetChannels()const;
			void SetChannelLayout(Audio::ChannelLayout channelLayout);
			Audio::ChannelLayout GetChannelLayout()const;
			BitsPerSample GetBitsPerSample()const;
			UShort GetBitsPerSampleNum()const;
			UInt GetSampleNum()const;
			double GetStartTimeMs()const;
			void SetStartTimeMs(double startMs);

			UInt64 GetPool8Bytes()const;
			UInt64 GetPool16Bytes()const;
			UInt64 GetPool24Bytes()const;
			UInt64 GetPool32Bytes()const;
			UInt64 GetFloatPoolBytes()const;
			UInt64 GetDoublePoolBytes()const;

			const Int8* GetPool8()const;
			const Int16* GetPool16()const;
			const Int24* GetPool24()const;
			const Int32* GetPool32()const;
			const Float32* GetFloatPool()const;
			const Float64* GetDoublePool()const;

			const Int8** GetSplittedPool8()const;
			const Int16** GetSplittedPool16()const;
			const Int24** GetSplittedPool24()const;
			const Int32** GetSplittedPool32()const;
			const Float32** GetSplittedFloatPool()const;
			const Float64** GetSplittedDoublePool()const;

			double GetDuration()const;

			__uref Result<AudioChunk*> BeginChunk(UShort bps, bool isfloating, bool enableRollBack = false);
			__uref Result<bool> EndChunk(AudioChunk*& chunk, bool rollBackBeforeReset = false);

			__uproperty(get = GetSampleRate)UInt SampleRate;
			__uproperty(get = GetChannels)UShort Channels;
			__uproperty(get = GetChannelLayout, put = SetChannelLayout) Audio::ChannelLayout ChannelLayout;
			__uproperty(get = GetBitsPerSample) Audio::BitsPerSample BitsPerSample;
			__uproperty(get = GetBitsPerSampleNum) UShort BitsPerSampleNum;
			__uproperty(get = GetSampleNum) UInt SampleNum;
			__uproperty(get = GetStartTimeMs, put = SetStartTimeMs) double StartTimeMs;

			__uproperty(get = GetPool8Bytes) UInt64 Pool8Bytes;
			__uproperty(get = GetPool16Bytes) UInt64 Pool16Bytes;
			__uproperty(get = GetPool24Bytes) UInt64 Pool24Bytes;
			__uproperty(get = GetPool32Bytes) UInt64 Pool32Bytes;
			__uproperty(get = GetFloatPoolBytes) UInt64 FloatPoolBytes;
			__uproperty(get = GetDoublePoolBytes) UInt64 DoublePoolBytes;

			__uproperty(get = GetPool8) const Int8* Pool8;
			__uproperty(get = GetPool16) const Int16* Pool16;
			__uproperty(get = GetPool24) const Int24* Pool24;
			__uproperty(get = GetPool32) const Int32* Pool32;
			__uproperty(get = GetFloatPool) const Float32* FloatPool;
			__uproperty(get = GetDoublePool) const Float64* DoublePool;

			__uproperty(get = GetSplittedPool8) const Int8** SplittedPool8;
			__uproperty(get = GetSplittedPool16) const Int16** SplittedPool16;
			__uproperty(get = GetSplittedPool24) const Int24** SplittedPool24;
			__uproperty(get = GetSplittedPool32) const Int32** SplittedPool32;
			__uproperty(get = GetSplittedFloatPool) const Float32** SplittedFloatPool;
			__uproperty(get = GetSplittedDoublePool) const Float64** SplittedDoublePool;

			__uproperty(get = GetDuration) double Duration;

			REFLECT_CLASS(AudioBuffer)
		};
	}
}