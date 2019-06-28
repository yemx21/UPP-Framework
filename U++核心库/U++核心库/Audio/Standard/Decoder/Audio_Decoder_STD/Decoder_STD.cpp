#include "Decoder_STD.h"
#include <stdio.h>
#include <string>
#include <filesystem>
using namespace System::Audio;

struct STD_Pipe :ZeroStruct<STD_Pipe>
{
	AudioDecoderPipe pipe;
	int decoder_id;
};

std::mutex AudioDecoder_STD::locker;
AudioDecoder_STD* AudioDecoder_STD::instance = nullptr;
const wchar_t* AudioDecoder_STD::_author = L"xworkteam powered by Xiph.Org Foundation";
const wchar_t* AudioDecoder_STD::_description=L"±ê×¼½âÂë¿â";
const wchar_t* AudioDecoder_STD::_supports=L".ogg;.flac;.fla;.mpc;.ape;.mac;.opus;.mv";
UInt64 AudioDecoder_STD::pipeNum = 0U;

AudioDecoder_STD::AudioDecoder_STD() : AudioDecoder_base()
{

}

AudioDecoder_STD* AudioDecoder_STD::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			instance = new AudioDecoder_STD();

			int id_counter = 0;

			auto vorbis = std::make_unique<Std_Vorbis>();
			if (vorbis)
			{
				auto vorbis_sfs = vorbis->GetSupportFormats();
				if (vorbis_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(vorbis);
					for (const auto& vorbis_sf : vorbis_sfs)
					{
						instance->_decodermaps[vorbis_sf] = id_counter;
					}
					id_counter++;
				}
			}
			auto flac = std::make_unique<Std_FLAC>();
			if (flac)
			{
				auto flac_sfs = flac->GetSupportFormats();
				if (flac_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(flac);
					for (const auto& flac_sf : flac_sfs)
					{
						instance->_decodermaps[flac_sf] = id_counter;
					}
					id_counter++;
				}
			}
			auto mpc = std::make_unique<Std_MPC>();
			if (mpc)
			{
				auto mpc_sfs = mpc->GetSupportFormats();
				if (mpc_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(mpc);
					for (const auto& mpc_sf : mpc_sfs)
					{
						instance->_decodermaps[mpc_sf] = id_counter;
					}
					id_counter++;
				}
			}
			
			auto ape = std::make_unique<Std_APE>();
			if (ape)
			{
				auto ape_sfs = ape->GetSupportFormats();
				if (ape_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(ape);
					for (const auto& ape_sf : ape_sfs)
					{
						instance->_decodermaps[ape_sf] = id_counter;
					}
					id_counter++;
				}
			}
			auto opus = std::make_unique<Std_OPUS>();
			if (opus)
			{
				auto opus_sfs = opus->GetSupportFormats();
				if (opus_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(opus);
					for (const auto& opus_sf : opus_sfs)
					{
						instance->_decodermaps[opus_sf] = id_counter;
					}
					id_counter++;
				}
			}
			auto mv = std::make_unique<Std_MV>();
			if (mv)
			{
				auto mv_sfs = mv->GetSupportFormats();
				if (mv_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(mv);
					for (const auto& mv_sf : mv_sfs)
					{
						instance->_decodermaps[mv_sf] = id_counter;
					}
					id_counter++;
				}
			}
			auto ffmpeg = std::make_unique<Std_FFMPEG>();
			if (ffmpeg)
			{
				auto ffmpeg_sfs = ffmpeg->GetSupportFormats();
				if (ffmpeg_sfs.size() > 0)
				{
					instance->_decoders[id_counter] = std::move(ffmpeg);
					for (const auto& ffmpeg_sf : ffmpeg_sfs)
					{
						instance->_decodermaps[ffmpeg_sf] = id_counter;
					}
					id_counter++;
				}
			}
		}
	}
	return instance;
}

AudioDecoder_STD::~AudioDecoder_STD()
{
	
}

Version AudioDecoder_STD::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDecoder_STD::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDecoder_STD::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDecoder_STD::GetSupportFormat() const
{
	return _supports;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_STD::OpenLocal(const wchar_t* localPath)
{
	Result<AudioDecoderPipe, AudioDecoderError> result(nullptr, AudioDecoderError::UnSupported);

	const char* path = Converter::CPlusNew(localPath, CultureInfo::Current().CPlusLocale);
	if (!path)
	{
		result.State = AudioDecoderError::InvalidPath;
		SAFE_DELETE(path);
		return result;
	}

	auto fs_path = experimental::filesystem::path(path);

	if (!experimental::filesystem::exists(fs_path))
	{
		result.State = AudioDecoderError::InvalidPath;
		SAFE_DELETE(path);
		return result;
	}

	auto ext = fs_path.extension().wstring();

	auto ext_iter = _decodermaps.find(ext);
	if (ext_iter != _decodermaps.end())
	{
		auto decoder_ptr = _decoders[ext_iter->second].get();
		if (decoder_ptr)
		{
			decoder_ptr->OpenLocal(localPath, path, result);
			if (result.State == AudioDecoderError::OK)
			{
				STD_Pipe* pipe = new STD_Pipe();

				pipe->pipe = result.Value;
				pipe->decoder_id = ext_iter->second;

				result.Value = pipe;

				pipeNum++;
			}
		}
		else
		{
			result.State = AudioDecoderError::UnSupported;
		}
	}
	else
	{
		result.State = AudioDecoderError::UnSupported;
	}

	SAFE_DELETE(path);

	return result;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_STD::CreateStream()
{
	return Result<AudioDecoderPipe, AudioDecoderError>(nullptr, AudioDecoderError::UnSupported);
}

bool AudioDecoder_STD::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(STD_Pipe));
		if (pipePtr)
		{
			STD_Pipe* m_std = static_cast<STD_Pipe*>(pipePtr);
			if (m_std) return false;
		}
	}
	catch (...){}
	return false;
}

UInt64 AudioDecoder_STD::GetNumberOfPipes()
{
	return pipeNum;
}

bool AudioDecoder_STD::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(STD_Pipe));
		if (pipePtr)
		{
			STD_Pipe* m_std = static_cast<STD_Pipe*>(pipePtr);
			if (m_std)
			{
				return _decoders[m_std->decoder_id]->TryGetPipeInfo(m_std->pipe, info);
			}
		}
	}
	catch (...){}
	return false;

}

AudioDecoderError AudioDecoder_STD::TryGetLocalAudioInfo(const wchar_t * localPath, AudioInfo& info)
{
	AudioDecoderError ret = AudioDecoderError::Failed;

	const char* path = Converter::CPlusNew(localPath, CultureInfo::Current().CPlusLocale);
	if (!path)
	{ 
		SAFE_DELETE(path);
		return AudioDecoderError::InvalidPath;
	}

	auto fs_path = experimental::filesystem::path(path);

	if (!experimental::filesystem::exists(fs_path))
	{
		SAFE_DELETE(path);
		return AudioDecoderError::InvalidPath;
	}

	auto ext = fs_path.extension().wstring();

	auto ext_iter = _decodermaps.find(ext);
	if (ext_iter != _decodermaps.end())
	{
		auto decoder_ptr = _decoders[ext_iter->second].get();
		if (decoder_ptr)
		{
			ret = decoder_ptr->TryGetLocalAudioInfo(localPath, path, info);
		}
		else
		{
			ret = AudioDecoderError::UnSupported;
		}
	}
	else
	{
		ret = AudioDecoderError::UnSupported;
	}

	SAFE_DELETE(path);
	return ret;
}

Result<UInt64, AudioDecoderError> AudioDecoder_STD::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples)
{
	Result<UInt64, AudioDecoderError> result(0ull, AudioDecoderError::End);
	try
	{
		void* pipePtr = pipe.get(sizeof(STD_Pipe));
		if (pipePtr)
		{
			STD_Pipe* m_std = static_cast<STD_Pipe*>(pipePtr);
			if (m_std)
			{
				_decoders[m_std->decoder_id]->Read(m_std->pipe, buffer, desiredNumSamples, result);
			}
		}
	}
	catch (...){}
	return result;

}

bool AudioDecoder_STD::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(STD_Pipe));
		if (pipePtr)
		{
			STD_Pipe* m_std = static_cast<STD_Pipe*>(pipePtr);
			if (m_std)
			{
				return _decoders[m_std->decoder_id]->TrySeek(m_std->pipe, posMS);
			}
		}
	}
	catch (...){}
	return false;
}

bool AudioDecoder_STD::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(STD_Pipe));
		if (pipePtr)
		{
			STD_Pipe* m_std = (STD_Pipe*)(pipePtr);
			if (m_std)
			{
				if (_decoders[m_std->decoder_id]->TryClose(m_std->pipe))
				{
					SAFE_DELETE(m_std);
					pipeNum--;
					return true;
				}
			}
		}
	}
	catch (...){}
	return false;
}

double AudioDecoder_STD::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(STD_Pipe));
		if (pipePtr)
		{
			STD_Pipe* m_std = static_cast<STD_Pipe*>(pipePtr);
			if (m_std)
			{
				return _decoders[m_std->decoder_id]->GetCurrentMS(m_std->pipe);
			}
		}
	}
	catch (...){}
	return 0.0;
}
