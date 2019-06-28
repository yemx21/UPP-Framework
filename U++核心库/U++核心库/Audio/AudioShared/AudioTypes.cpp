#include "AudioTypes.h"
#include <random>
using namespace System::Audio;


static std::vector<InertiaString> CodecDescriptions = { L"Vorbis", L"MP1", L"MP2", L"MP3", L"AAC", L"Monkey's Audio", L"WMA", L"MP4", L"Musepack", L"FLAC", L"WAV", L"AIFF", L"AU", L"MAT", L"WavPack", L"Opus", L"TAK" };


const wchar_t* Codec::GetDescription() const
{
	if (value < CodecDescriptions.size()) return CodecDescriptions[value].get().c_str();
	return nullptr;
}

Result<bool, UShort> Codec::Register(const wchar_t* description)
{ 
	Result<bool, UShort> result(false, 0);
	UShort nvalue = CodecDescriptions.size();
	if (nvalue < 65535)
	{
		result.Value = true;
		result.State = nvalue;
		CodecDescriptions.push_back(description != nullptr ? wstring(description) : InertiaString());
	}
	else
	{
		result.WriteMessage(L"reach the upper limitation");
	}
	return result;
}


bool BitsPerSample::GetIsValid() const
{
	return value >= 0U && value<10U;
}

bool BitsPerSample::GetIsFloating() const
{
	return value > 7U && value < 10U;
}

UShort BitsPerSample::GetBPS() const
{
	switch (value)
	{
	case BitsPerSample::SInt8:
	case BitsPerSample::UInt8:
		return 8U;
	case BitsPerSample::SInt16:
	case BitsPerSample::UInt16:
		return 16U;
	case BitsPerSample::SInt24:
	case BitsPerSample::UInt24:
		return 24U;
	case BitsPerSample::SInt32:
	case BitsPerSample::UInt32:
	case BitsPerSample::Float32:
		return 32U;
	case BitsPerSample::Float64:
		return 64U;
	case 65535U:
	default:
		return 0U;
	}
}

BitsPerSample BitsPerSample::GetOSBitsPerSample(UShort bps, bool issign, bool floating)
{
	switch (bps)
	{
	case 8U:
		return issign ? BitsPerSample::SInt8 : BitsPerSample::UInt8;
	case 16U:
		return issign ? BitsPerSample::SInt16 : BitsPerSample::UInt16;
	case 24U:
		return issign ? BitsPerSample::SInt24 : BitsPerSample::UInt24;
	case 32U:
		if (!floating)
		{
			return issign ? BitsPerSample::SInt32 : BitsPerSample::UInt32;
		}
		return BitsPerSample::Float32;
	case 64U:
		return BitsPerSample::Float64;
	case 65535U:
	default:
		return BitsPerSample::Unknown;
	}

}

bool ChannelLayout::GetIsValid() const
{
	return (value != 0U) && ((value & 262143U) == value);
}

UShort ChannelLayout::GetCount() const
{
	if (value != 0U && ((value & 262143U) == value))
	{
		UInt v = value;
		v = v - ((v >> 1) & 0x55555555);
		v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
		return  ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
	}
	return 0U;
}

ChannelLayout ChannelLayout::GetDefaultLayout(UShort channels)
{
	switch (channels)
	{
	case 1u:
		return ChannelLayout::Mono;
	case 2u:
		return ChannelLayout::Stereo;
	case 3u:
		return ChannelLayout::Surround;
	case 4u:
		return ChannelLayout::Quad;
	case 5u:
		return ChannelLayout::Point5_0;
	case 6u:
		return ChannelLayout::Point5_1;
	case 7u:
		return ChannelLayout::Point6_1;
	case 8u:
		return ChannelLayout::Point7_1;
	}
	return ChannelLayout::Unknown;
}

void SampleInfo::Clear()
{
	_chcfg = ChannelLayout::Unknown;
	_sr = 0;
	_abps = 0;
	_ch = 0;
	_blkalign = 0; 
	_bps = BitsPerSample::Unknown;
}

bool SampleInfo::GetIsValid() const
{
	UShort bps = _bps.BPS;
	return ((bps == 8 || bps == 16 || bps == 24 || bps == 32 || bps == 64) && (_sr >= SAMPLERATE_MIN && _sr <= SAMPLERATE_MAX) && (_ch >= CHANNELS_MIN && _ch <= CHANNELS_MAX));
}

bool SampleInfo::GetIsEmpty()const
{
	return !(_sr || _abps || _ch || _blkalign);
}

bool SampleInfo::operator==(const SampleInfo& compare) const
{
	return compare._chcfg == _chcfg && compare._bps == _bps && compare._sr == _sr;
}

bool SampleInfo::operator!=(const SampleInfo& compare) const
{
	return compare._chcfg != _chcfg || compare._bps != _bps || compare._sr != _sr;
}

SampleInfo::SampleInfo() : _chcfg(ChannelLayout::Unknown), _sr(0), _abps(0), _ch(0), _blkalign(0), _bps(BitsPerSample::Unknown)
{
}

SampleInfo::SampleInfo(const SampleInfo& info)
{
	_chcfg = info._chcfg;
	_ch = info._ch;
	_sr = info._sr;
	_abps = info._abps;
	_blkalign = info._blkalign;
	_bps = info._bps;
}

SampleInfo::SampleInfo(UInt sr, Audio::BitsPerSample bps, Audio::ChannelLayout chcfg)
{
	_chcfg = chcfg;
	_ch = chcfg.Count;
	_sr = sr;
	_bps = bps;
	_blkalign = _bps.BPS * _ch / 8;
	_abps = _sr * _blkalign;
}

UInt SampleInfo::GetSampleRate() const
{
	return _sr;
}

void SampleInfo::SetSampleRate(UInt val)
{
	_sr = val;
	_abps = _blkalign* _sr;
}

UInt SampleInfo::GetAvgBytesPerSec() const
{
	return _abps;
}

UShort SampleInfo::GetChannels() const
{
	return _ch;
}

UShort SampleInfo::GetBlockAlign() const
{
	return _blkalign;
}

Audio::BitsPerSample SampleInfo::GetBitsPerSample() const
{
	return _bps;
}

void SampleInfo::SetBitsPerSample(Audio::BitsPerSample val)
{
	_bps = val;
	_blkalign = _bps.BPS* _ch / 8;
	_abps = _blkalign* _sr;
}

Audio::ChannelLayout SampleInfo::GetChannelLayout() const
{
	return _chcfg;
}

void SampleInfo::SetChannelLayout(Audio::ChannelLayout val)
{
	_chcfg = val;
	_ch = _chcfg.Count;
	_blkalign = _bps.BPS* _ch / 8;
	_abps = _blkalign* _sr;
}

bool ResampleConfig::GetIsValid() const
{
	UShort bps = _bps.BPS;
	return ((bps == 8 || bps == 16 || bps == 24 || bps == 32 || bps == 64) && (_sr >= SAMPLERATE_MIN && _sr <= SAMPLERATE_MAX) && (_ch >= CHANNELS_MIN && _ch <= CHANNELS_MAX));
}

bool ResampleConfig::GetIsEmpty()const
{
	return !(_sr || _abps || _ch || _blkalign);
}

bool ResampleConfig::operator==(const ResampleConfig& compare) const
{
	return compare._chcfg == _chcfg && compare._bps == _bps && compare._sr == _sr;
}

bool ResampleConfig::operator!=(const ResampleConfig& compare) const
{
	return compare._chcfg != _chcfg || compare._bps != _bps || compare._sr != _sr;
}

ResampleConfig::ResampleConfig() : _chcfg(ChannelLayout::Unknown), _sr(0), _abps(0), _ch(0), _blkalign(0), _bps(BitsPerSample::Unknown), _ensr(false), _ench(false), _enbps(false)
{
}

ResampleConfig::ResampleConfig(const ResampleConfig& info)
{
	_chcfg = info._chcfg;
	_ch = info._ch;
	_sr = info._sr;
	_abps = info._abps;
	_blkalign = info._blkalign;
	_bps = info._bps;
	_ensr = info._ensr;
	_ench = info._ench;
	_enbps = info._enbps;
}

ResampleConfig::ResampleConfig(UInt sr, Audio::BitsPerSample bps, Audio::ChannelLayout chcfg, bool ensr, bool ench, bool enbps)
{
	_chcfg = chcfg;
	_ch = chcfg.Count;
	_sr = sr;
	_bps = bps;
	_blkalign = _bps.BPS * _ch / 8;
	_abps = _sr * _blkalign;
	_ensr = ensr;
	_ench = ench;
	_enbps = enbps;
}

UInt ResampleConfig::GetSampleRate() const
{
	return _sr;
}

void ResampleConfig::SetSampleRate(UInt val)
{
	_sr = val;
	_abps = _blkalign* _sr;
}

UInt ResampleConfig::GetAvgBytesPerSec() const
{
	return _abps;
}

UShort ResampleConfig::GetChannels() const
{
	return _ch;
}

UShort ResampleConfig::GetBlockAlign() const
{
	return _blkalign;
}

Audio::BitsPerSample ResampleConfig::GetBitsPerSample() const
{
	return _bps;
}

void ResampleConfig::SetBitsPerSample(Audio::BitsPerSample val)
{
	_bps = val;
	_blkalign = _bps.BPS* _ch / 8;
	_abps = _blkalign* _sr;
}

Audio::ChannelLayout ResampleConfig::GetChannelLayout() const
{
	return _chcfg;
}



void ResampleConfig::SetChannelLayout(Audio::ChannelLayout val)
{
	_chcfg = val;
	_ch = _chcfg.Count;
	_blkalign = _bps.BPS* _ch / 8;
	_abps = _blkalign* _sr;
}

bool ResampleConfig::GetEnableSampleRate() const
{
	return _ensr;
}

void ResampleConfig::SetEnableSampleRate(bool val) 
{
	_ensr = val;
}

bool ResampleConfig::GetEnableChannels() const
{
	return _ench;
}

void ResampleConfig::SetEnableChannels(bool val) 
{
	_ench = val;
}

bool ResampleConfig::GetEnableBitsPerSample() const
{
	return _enbps;
}

void ResampleConfig::SetEnableBitsPerSample(bool val)
{
	_enbps = val;
}

bool AudioInfo::GetIsValid() const
{
	UShort bps = _bps.BPS;
	return ((bps == 8 || bps == 16 || bps == 24 || bps == 32 || bps == 64) && (_sr >= SAMPLERATE_MIN && _sr <= SAMPLERATE_MAX) && (_ch >= CHANNELS_MIN && _ch <= CHANNELS_MAX) && _codec != Audio::Codec::Unknown);
}

bool AudioInfo::GetIsEmpty()const
{
	return !(_sr || _abps || _ch || _blkalign || _sl || _br || _timeMs);
}

bool AudioInfo::operator==(const AudioInfo& compare) const
{
	return compare._chcfg == _chcfg && compare._bps == _bps && compare._sr == _sr && compare._sl ==_sl && FloatingHelper::AreClose(compare._timeMs,_timeMs) && compare._br ==_br && compare._codec == _codec && FloatingHelper::AreClose(compare._gain, _gain);
}

bool AudioInfo::operator!=(const AudioInfo& compare) const
{
	return compare._chcfg != _chcfg || compare._bps != _bps || compare._sr != _sr && compare._sl == _sl || !FloatingHelper::AreClose(compare._timeMs, _timeMs) || compare._br != _br && compare._codec != _codec || !FloatingHelper::AreClose(compare._gain, _gain);
}

AudioInfo::AudioInfo() : _chcfg(ChannelLayout::Unknown), _sr(0), _abps(0), _br(0), _ch(0), _blkalign(0), _bps(Audio::BitsPerSample::Unknown), _timeSec(0.0), _timeMs(0.0), _codec(Audio::Codec::Unknown), _sl(0), _gain(0.0)
{
}

AudioInfo::~AudioInfo()
{
}

AudioInfo::AudioInfo(const AudioInfo& info)
{
	_codec = info._codec;
	_chcfg = info._chcfg;
	_ch = info._ch;
	_sr = info._sr;
	_abps = info._abps;
	_blkalign = info._blkalign;
	_bps = info._bps;
	_timeMs = info._timeMs;
	_timeSec = info._timeSec;
	_sl = info._sl;
	_br = info._br;
	_gain = info._gain;
}

AudioInfo::AudioInfo(AudioInfo&& info)
{
	_codec = info._codec;
	_chcfg = info._chcfg;
	_ch = info._ch;
	_sr = info._sr;
	_abps = info._abps;
	_blkalign = info._blkalign;
	_bps = info._bps;
	_timeMs = info._timeMs;
	_timeSec = info._timeSec;
	_sl = info._sl;
	_br = info._br;
	_gain = info._gain;
}

AudioInfo& AudioInfo::operator =(const AudioInfo& info)
{
	_codec = info._codec;
	_chcfg = info._chcfg;
	_ch = info._ch;
	_sr = info._sr;
	_abps = info._abps;
	_blkalign = info._blkalign;
	_bps = info._bps;
	_timeMs = info._timeMs;
	_timeSec = info._timeSec;
	_sl = info._sl;
	_br = info._br;
	_gain = info._gain;
	return *this;
}

AudioInfo::AudioInfo(Audio::Codec codec, UInt sr, Audio::BitsPerSample bps, Audio::ChannelLayout chcfg, double timeMs, size_t sampleLen, UInt bitRate, double playGain)
{
	_codec = codec;
	_chcfg = chcfg;
	_ch = chcfg.Count;
	_sr = sr;
	_bps = bps;
	_blkalign = _bps.BPS * _ch / 8;
	_abps = _sr * _blkalign;
	_timeMs = timeMs;
	_timeSec = _timeMs / 1000.0;
	_sl = sampleLen;
	_br = bitRate;
	_gain = playGain;
}

UInt AudioInfo::GetSampleRate() const
{
	return _sr;
}

void AudioInfo::SetSampleRate(UInt val)
{
	_sr = val;
	_abps = _blkalign* _sr;
}

UInt AudioInfo::GetAvgBytesPerSec() const
{
	return _abps;
}

UShort AudioInfo::GetChannels() const
{
	return _ch;
}

UShort AudioInfo::GetBlockAlign() const
{
	return _blkalign;
}

Audio::BitsPerSample AudioInfo::GetBitsPerSample() const
{
	return _bps;
}

void AudioInfo::SetBitsPerSample(Audio::BitsPerSample val)
{
	_bps = val;
	_blkalign = _bps.BPS* _ch / 8;
	_abps = _blkalign* _sr;
}

Audio::ChannelLayout AudioInfo::GetChannelLayout() const
{
	return _chcfg;
}

void AudioInfo::SetChannelLayout(Audio::ChannelLayout val)
{
	_chcfg = val;
	_ch = _chcfg.Count;
	_blkalign = _bps.BPS* _ch / 8;
	_abps = _blkalign* _sr;
}

Audio::Codec AudioInfo::GetCodec() const
{
	return _codec;
}

void AudioInfo::SetCodec(Audio::Codec val)
{
	_codec = val;
}

Int64 AudioInfo::GetSampleLength() const
{
	return _sl;
}

void AudioInfo::SetSampleLength(Int64 val)
{
	_sl = val;
}

double AudioInfo::GetTimeSec() const
{
	return _timeSec;
}

void AudioInfo::SetTimeSec(double val)
{
	_timeSec = val;
	_timeMs = _timeSec*1000.0;
}

double AudioInfo::GetTimeMs() const
{
	return _timeMs;
}

void AudioInfo::SetTimeMs(double val)
{
	_timeMs = val;
	_timeSec = _timeMs / 1000.0;
}

double AudioInfo::GetPlayGain() const
{
	return _gain;
}

void AudioInfo::SetPlayGain(double val)
{
	_gain= val;
}

UInt AudioInfo::GetBitRate() const
{
	return _br;
}

void AudioInfo::SetBitRate(UInt val)
{
	_br = val;
}
