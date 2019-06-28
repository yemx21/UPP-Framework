#pragma once
#include "Config.h"

using namespace System;
namespace System
{
	namespace Audio
	{
		const UInt SAMPLERATE_MIN = 1000UL;
		const UInt SAMPLERATE_MAX = 2822400UL;
		const UShort CHANNELS_MIN = 1U;
		const UShort CHANNELS_MAX = 18U;

		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0) AUDIOSHARED_API Codec : __uobject
		{
			__uvalue_begin(Codec, UShort)
			__uvalue(Vorbis, 1)
			__uvalue(MP1)
			__uvalue(MP2)
			__uvalue(MP3)
			__uvalue(AAC)
			__uvalue(MonkeyAudio)
			__uvalue(WMA)
			__uvalue(MP4)
			__uvalue(Musepack)
			__uvalue(FLAC)
			__uvalue(WAV)
			__uvalue(AIFF)
			__uvalue(AU)
			__uvalue(MAT)
			__uvalue(WavPack)
			__uvalue(Opus)
			__uvalue(TAK)
			__uvalue(Unknown, 65535)
			__uvalue_end(Codec, Unknown)

			__uref const wchar_t* GetDescription() const;
			__uproperty(get = GetDescription) const wchar_t* Description;

			__uref static Result<bool, UShort> Register(const wchar_t* description);
		};

		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0)  AUDIOSHARED_API BitsPerSample : __uobject
		{
			__uvalue_begin(BitsPerSample, UShort)
			__uvalue(SInt8)
			__uvalue(UInt8)
			__uvalue(SInt16)
			__uvalue(UInt16)
			__uvalue(SInt24)
			__uvalue(UInt24)
			__uvalue(SInt32)
			__uvalue(UInt32)
			__uvalue(Float32)
			__uvalue(Float64)
			__uvalue(Unknown, 65535)
			__uvalue_end(BitsPerSample, Unknown)
			bool GetIsValid() const;
			bool GetIsFloating() const;
			UShort GetBPS() const;

			__uref static BitsPerSample GetOSBitsPerSample(UShort bps, bool issign = true, bool floating = false);

			__uproperty(get = GetIsValid) bool IsValid;
			__uproperty(get = GetBPS) UShort BPS;
			__uproperty(get = GetIsFloating) bool IsFloating;	
		};

		__uenum __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0)  AUDIOSHARED_API ChannelLayout : __uobject
		{
			__uvalue_begin(ChannelLayout, UInt)
			__uvalue(FrontLeft, 1)
			__uvalue(FrontRight, 2)
			__uvalue(FrontCenter, 4)
			__uvalue(LowFrequency, 8)
			__uvalue(BackLeft, 16)
			__uvalue(BackRight, 32)
			__uvalue(FrontLeftOrCenter, 64)
			__uvalue(FrontRightOrCenter, 128)
			__uvalue(BackCenter, 256)
			__uvalue(SideLeft, 512)
			__uvalue(SideRight, 1024)
			__uvalue(TopCenter, 2048)
			__uvalue(TopFrontLeft, 4096)
			__uvalue(TopFrontCenter, 8192)
			__uvalue(TopFrontRight, 16384)
			__uvalue(TopBackLeft, 32768)
			__uvalue(TopBackCenter, 65536)
			__uvalue(TopBackRight, 131072)

			__uvalue(Mono, 4) // FrontCenter

			__uvalue(Stereo, 3) // FrontLeft + FrontRight

			__uvalue(Point2_1, 11) // FrontLeft + FrontRight + LowFrequency
			__uvalue(Back2_1, 259) // FrontLeft + FrontRight + BackCenter
			__uvalue(Surround, 7)// FrontLeft + FrontRight + FrontCenter

			__uvalue(Point3_1, 15)// FrontLeft + FrontRight + FrontCenter + LowFrequency
			__uvalue(Point4_0, 263)// FrontLeft + FrontRight + FrontCenter + BackCenter
			__uvalue(Back2_2, 1539) // FrontLeft + FrontRight + SideLeft + SideRight
			__uvalue(Quad, 51)// FrontLeft + FrontRight + BackLeft + BackRight

			__uvalue(Point4_1, 271)// FrontLeft + FrontRight + FrontCenter + BackCenter + LowFrequency
			__uvalue(Point5_0, 1543)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight
			__uvalue(Back5_0, 55)// FrontLeft + FrontRight + FrontCenter + BackLeft + BackRight

			__uvalue(Point5_1, 1551)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight  + LowFrequency
			__uvalue(Back5_1, 63)// FrontLeft + FrontRight + FrontCenter + BackLeft + BackRight  + LowFrequency
			__uvalue(Point6_0, 1799)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight + BackCenter
			__uvalue(Front6_0, 1731)// FrontLeft + FrontRight + SideLeft + SideRight + FrontLeftOrCenter + FrontRightOrCenter
			__uvalue(Hexagonal, 311)// FrontLeft + FrontRight + FrontCenter + BackLeft + BackRight + BackCenter

			__uvalue(Point6_1, 1807)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight  + LowFrequency + BackCenter
			__uvalue(Back6_1, 319)// FrontLeft + FrontRight + FrontCenter + BackLeft + BackRight  + LowFrequency + BackCenter
			__uvalue(Front6_1, 1739)// FrontLeft + FrontRight + SideLeft + SideRight + FrontLeftOrCenter + FrontRightOrCenter + LowFrequency
			__uvalue(Point7_0, 1591)// FrontLeft + FrontRight + FrontCenter+ SideLeft + SideRight + BackLeft + BackRight
			__uvalue(Front7_0, 1735)// FrontLeft + FrontRight + FrontCenter+ SideLeft + SideRight + FrontLeftOrCenter + FrontRightOrCenter

			__uvalue(Point7_1, 1599)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight  + LowFrequency + BackLeft + BackRight
			__uvalue(Wide7_1, 1743)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight  + LowFrequency + FrontLeftOrCenter + FrontRightOrCenter
			__uvalue(WideBack7_1, 255)// FrontLeft + FrontRight + FrontCenter + BackLeft + BackRight  + LowFrequency + FrontLeftOrCenter + FrontRightOrCenter
			__uvalue(Octagonal, 1847)// FrontLeft + FrontRight + FrontCenter + SideLeft + SideRight + BackLeft + BackRight + BackCenter

			__uvalue(Unknown, 0)

			__uvalue_end(ChannelLayout,  Unknown)

			/*
			ChannelLayout::Mono;
			ChannelLayout::Stereo;
			ChannelLayout::Surround;
			ChannelLayout::Quad;
			ChannelLayout::Point5_0;
			ChannelLayout::Point5_1;
			ChannelLayout::Point6_1;
			ChannelLayout::Point7_1;*/
			__uref static ChannelLayout GetDefaultLayout(UShort channels);

			bool GetIsValid() const;
			UShort GetCount() const;

			__uproperty(get = GetCount) UShort Count;
			__uproperty(get = GetIsValid) bool IsValid;	
		};


		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0)  AUDIOSHARED_API SampleInfo : __uobject
		{
		private:
			UInt _sr;
			UInt _abps;
			UShort _ch;
			UShort _blkalign;
			Audio::BitsPerSample _bps;
			Audio::ChannelLayout _chcfg;
		public:	
			__udefctor SampleInfo();
			__uctor SampleInfo(UInt sr, Audio::BitsPerSample bps, Audio::ChannelLayout chcfg);
			__uctor SampleInfo(const SampleInfo&);

			__uref void Clear();

			bool GetIsValid() const;
			bool GetIsEmpty()const;
			bool operator==(const SampleInfo&) const;
			bool operator!=(const SampleInfo&) const;
			UInt GetSampleRate() const;
			void SetSampleRate(UInt);
			UInt GetAvgBytesPerSec() const;
			UShort GetChannels() const;
			UShort GetBlockAlign() const;
			Audio::BitsPerSample GetBitsPerSample() const;
			void SetBitsPerSample(Audio::BitsPerSample);
			Audio::ChannelLayout GetChannelLayout() const;
			void SetChannelLayout(Audio::ChannelLayout);

			__uproperty(get = GetIsValid) bool IsValid;
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			__uproperty(get = GetSampleRate, put = SetSampleRate) UInt SampleRate;
			__uproperty(get = GetAvgBytesPerSec) UInt AvgBytesPerSec;
			__uproperty(get = GetChannels) UShort Channels;
			__uproperty(get = GetBlockAlign) UShort BlockAlign;
			__uproperty(get = GetBitsPerSample, put = SetBitsPerSample) Audio::BitsPerSample BitsPerSample;
			__uproperty(get = GetChannelLayout, put = SetChannelLayout) Audio::ChannelLayout ChannelLayout;

			REFLECT_CLASS(SampleInfo)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0)  AUDIOSHARED_API ResampleConfig : __uobject
		{
		private:
			bool _ensr;
			bool _ench;
			bool _enbps;
			UShort _ch; 
			UShort _blkalign;
			Audio::BitsPerSample _bps;
			Audio::ChannelLayout _chcfg;
			UInt _sr;
			UInt _abps;
		public:
			__udefctor ResampleConfig();
			__uctor ResampleConfig(const ResampleConfig&);
			__uctor ResampleConfig(UInt sr, Audio::BitsPerSample bps, Audio::ChannelLayout chcfg, bool ensr = false, bool ench = false, bool enbps = false);
			bool GetIsValid() const;
			bool GetIsEmpty() const;
			__uref bool operator==(const ResampleConfig&)const;
			__uref bool operator!=(const ResampleConfig&)const;
			UInt GetSampleRate() const;
			void SetSampleRate(UInt);
			UInt GetAvgBytesPerSec() const;
			UShort GetChannels() const;
			UShort GetBlockAlign() const;
			Audio::BitsPerSample GetBitsPerSample() const;
			void SetBitsPerSample(Audio::BitsPerSample);
			Audio::ChannelLayout GetChannelLayout() const;
			void SetChannelLayout(Audio::ChannelLayout);
			bool GetEnableSampleRate() const;
			void SetEnableSampleRate(bool);
			bool GetEnableChannels() const;
			void SetEnableChannels(bool);
			bool GetEnableBitsPerSample() const;
			void SetEnableBitsPerSample(bool);
			__uproperty(get = GetIsValid) bool IsValid;
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			__uproperty(get = GetSampleRate, put = SetSampleRate) UInt SampleRate;
			__uproperty(get = GetAvgBytesPerSec) UInt AvgBytesPerSec;
			__uproperty(get = GetChannels) UShort Channels;
			__uproperty(get = GetBlockAlign) UShort BlockAlign;
			__uproperty(get = GetBitsPerSample, put = SetBitsPerSample) Audio::BitsPerSample BitsPerSample;
			__uproperty(get = GetChannelLayout, put = SetChannelLayout) Audio::ChannelLayout ChannelLayout;
			__uproperty(get = GetEnableSampleRate, put = SetEnableSampleRate) bool EnableSampleRate;
			__uproperty(get = GetEnableChannels, put = SetEnableChannels) bool EnableChannels;
			__uproperty(get = GetEnableBitsPerSample, put = SetEnableBitsPerSample) bool EnableBitsPerSample;
			REFLECT_CLASS(ResampleConfig)
		};

		__uclass __uobjdef(asm = AudioShared, ns = System::Audio, ver = 1.0)  AUDIOSHARED_API AudioInfo : __uobject
		{
		private:
			double _timeSec;
			double _timeMs;
			double _gain;
			UInt _sr;
			Int64 _sl;
			UInt _br;
			UInt _abps;
			Audio::Codec _codec;
			UShort _ch;
			UShort _blkalign; 
			Audio::BitsPerSample _bps;
			Audio::ChannelLayout _chcfg;
		public:
			__udefctor AudioInfo();
			__uctor AudioInfo(const AudioInfo&);
			__uctor AudioInfo(AudioInfo&&);
			__uctor AudioInfo(Audio::Codec codec, UInt sr, Audio::BitsPerSample bps, Audio::ChannelLayout chcfg, double timeMs, size_t sampleLen, UInt bitRate, double playGain = 1.0);
			~AudioInfo();
			bool GetIsValid() const;
			bool GetIsEmpty() const;
			bool operator==(const AudioInfo&)const;
			bool operator!=(const AudioInfo&)const;
			AudioInfo& operator =(const AudioInfo&);
			Audio::Codec GetCodec() const;
			void SetCodec(Audio::Codec);
			UInt GetSampleRate() const;
			void SetSampleRate(UInt);
			Int64 GetSampleLength() const;
			void SetSampleLength(Int64);
			double GetTimeSec() const;
			void SetTimeSec(double);
			double GetTimeMs() const;
			void SetTimeMs(double);
			double GetPlayGain() const;
			void SetPlayGain(double);
			UInt GetBitRate() const;
			void SetBitRate(UInt);
			UInt GetAvgBytesPerSec() const;
			UShort GetChannels() const;
			UShort GetBlockAlign() const;
			Audio::BitsPerSample GetBitsPerSample() const;
			void SetBitsPerSample(Audio::BitsPerSample);
			Audio::ChannelLayout GetChannelLayout() const;
			void SetChannelLayout(Audio::ChannelLayout);

			__uproperty(get = GetIsValid) bool IsValid;
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			__uproperty(get = GetSampleRate, put = SetSampleRate) UInt SampleRate;
			__uproperty(get = GetCodec, put = SetCodec) Audio::Codec Codec;
			__uproperty(get = GetSampleLength, put = SetSampleLength) Int64 SampleLength;
			__uproperty(get = GetBitRate, put = SetBitRate) UInt BitRate;
			__uproperty(get = GetTimeMs, put = SetTimeMs) double TimeMs;
			__uproperty(get = GetTimeSec, put = SetTimeSec) double TimeSec;
			__uproperty(get = GetPlayGain, put = SetPlayGain) double PlayGain;
			__uproperty(get = GetAvgBytesPerSec) UInt AvgBytesPerSec;
			__uproperty(get = GetChannels) UShort Channels;
			__uproperty(get = GetBlockAlign) UShort BlockAlign;
			__uproperty(get = GetBitsPerSample, put = SetBitsPerSample) Audio::BitsPerSample BitsPerSample;
			__uproperty(get = GetChannelLayout, put = SetChannelLayout) Audio::ChannelLayout ChannelLayout;

			REFLECT_CLASS(AudioInfo)
		};
	}
}
namespace System
{
	__uenum_flag(System::Audio::ChannelLayout)
}