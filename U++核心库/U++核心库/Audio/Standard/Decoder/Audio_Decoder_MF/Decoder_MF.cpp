#include "Decoder_MF.h"
#include <stdio.h>
#include <string>
#include <filesystem>

using namespace System::Audio;
template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class MFCache //必须单线程安全调用
{
private:
	UShort BlockAlign;
public:
	size_t size;
	size_t valid;
	char* pool;
	size_t AvailableSamples() { return valid / BlockAlign; }
	MFCache(UShort blockalign) :size(0), pool(NULL), valid(0)
	{
		BlockAlign = blockalign;
	}

	~MFCache()
	{
		Clear();
	}

	void Clear()
	{
		if (pool)
		{
			free(pool);
			pool = NULL;
			valid = 0;
			size = 0;
		}
	}


	size_t Read(char* Buffer, size_t bytes)
	{
		if (bytes)
		{
			if (valid>bytes)
			{
				memcpy(Buffer, pool, bytes);
				valid -= bytes;
				memmove(pool, pool + bytes, valid);
				return bytes;
			}
			else
			{
				memcpy(Buffer, pool, valid);
				size_t temp = valid;
				valid = 0;
				return temp;
			}
		}
		return 0;
	}

	void Write(char* Buffer, size_t bytes)
	{
		if (bytes)
		{
			if (!pool)
			{
				size = bytes;
				pool = (char*)malloc(size);
				memcpy(pool, Buffer, bytes);
				valid = bytes;
			}
			else
			{
				if (size - valid<bytes) pool = (char*)realloc(pool, valid + bytes);
				memcpy(pool + valid, Buffer, bytes);
				valid += bytes;
				size = valid;
			}
		}
	}

	void CollectBins()
	{
		pool = (char*)realloc(pool, valid);
		size = valid;
	}

};

class MFStream :public IMFSourceReaderCallback
{
public:
	IMFSourceReader* MF;
	AudioInfo Info;
	MFCache* Buffer;
	int DecodePosMs;
	bool SeekNow;

	char* OutBuffer;
	size_t OutBufferSize;


	HRESULT WaitRead(DWORD dwMilliseconds, BOOL *pbEOS)
	{
		*pbEOS = FALSE;

		DWORD dwResult = WaitForSingleObject(m_ReadEvent, dwMilliseconds);
		if (dwResult == WAIT_TIMEOUT)
		{
			return E_PENDING;
		}
		else if (dwResult != WAIT_OBJECT_0)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		*pbEOS = m_bEOS;
		return m_hrStatus;
	}
	MFStream() :
		m_nRefCount(1), m_bEOS(FALSE), m_hrStatus(S_OK), MF(NULL), Buffer(NULL), DecodePosMs(0), SeekNow(false), OutBuffer(nullptr), OutBufferSize(0)
	{
		InitializeCriticalSection(&m_critsec);
		m_ReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~MFStream()
	{
		LeaveCriticalSection(&m_critsec);
		DeleteCriticalSection(&m_critsec);
		if (Buffer) { delete Buffer; Buffer = NULL; }
		if (OutBuffer) { free(OutBuffer); OutBuffer = NULL; }
		SafeRelease(&MF);
		CloseHandle(m_ReadEvent);
	}
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv)
	{
		static const QITAB qit[] =
		{
			QITABENT(MFStream, IMFSourceReaderCallback),
			{ 0 },
		};
		HRESULT hr = QISearch(this, qit, iid, ppv);
		return hr;
	}
	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_nRefCount);
	}
	STDMETHODIMP_(ULONG) Release()
	{
		ULONG uCount = InterlockedDecrement(&m_nRefCount);
		if (uCount == 0)
		{
			delete this;
		}
		return uCount;
	}
	STDMETHODIMP OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex,
		DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample *pSample)
	{
		EnterCriticalSection(&m_critsec);
		if (SUCCEEDED(hrStatus))
		{
			if (pSample)
			{
				IMFMediaBuffer *pMBuffer(NULL);
				if (SUCCEEDED(pSample->ConvertToContiguousBuffer(&pMBuffer)))
				{
					short * buffer(NULL);
					size_t bufferLength(0);
					HRESULT hr = pMBuffer->Lock(reinterpret_cast<UChar**>(&buffer), NULL,
						reinterpret_cast<DWORD*>(&bufferLength));
					if (SUCCEEDED(hr))
					{
						Buffer->Write((char*)buffer, bufferLength);
					}
					hr = pMBuffer->Unlock();
					SafeRelease(&pMBuffer);
				}
			}
		}
		if (MF_SOURCE_READERF_ENDOFSTREAM & dwStreamFlags)
		{
			m_bEOS = TRUE;
		}
		LeaveCriticalSection(&m_critsec);
		SetEvent(m_ReadEvent);
		ResetEvent(m_ReadEvent);
		return S_OK;
	}

	STDMETHODIMP OnEvent(DWORD dwStreamIndex, IMFMediaEvent *pEvent)
	{
		return S_OK;
	}

	STDMETHODIMP OnFlush(DWORD dwStreamIndex)
	{
		return S_OK;
	}

private:
	long                m_nRefCount;
	CRITICAL_SECTION    m_critsec;
	HANDLE              m_ReadEvent;
	BOOL                m_bEOS;
	HRESULT             m_hrStatus;

};

HRESULT ConfigureAudioStream(
	IMFSourceReader *pReader,   // Pointer to the source reader.
	IMFMediaType **ppPCMAudio   // Receives the audio format.
	)
{
	HRESULT hr = S_OK;

	IMFMediaType *pUncompressedAudioType = NULL;
	IMFMediaType *pPartialType = NULL;

	// Create a partial media type that specifies uncompressed PCM audio.

	hr = MFCreateMediaType(&pPartialType);
	if (SUCCEEDED(hr))
	{
		hr = pPartialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	}

	if (SUCCEEDED(hr))
	{
		hr = pPartialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	}
	// Set this type on the source reader. The source reader will
	// load the necessary decoder.
	if (SUCCEEDED(hr))
	{
		hr = pReader->SetCurrentMediaType(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			NULL,
			pPartialType
			);
	}
	// Get the complete uncompressed format.
	if (SUCCEEDED(hr))
	{
		hr = pReader->GetCurrentMediaType(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			&pUncompressedAudioType
			);
	}

	// Ensure the stream is selected.
	if (SUCCEEDED(hr))
	{
		hr = pReader->SetStreamSelection(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			TRUE
			);
	}

	// Return the PCM format to the caller.
	if (SUCCEEDED(hr))
	{
		*ppPCMAudio = pUncompressedAudioType;
		(*ppPCMAudio)->AddRef();
	}

	SafeRelease(&pUncompressedAudioType);
	SafeRelease(&pPartialType);
	return hr;
}

bool SetPosition(IMFSourceReader *pReader, const LONGLONG& hnsPosition)
{
	PROPVARIANT var;
	HRESULT hr = InitPropVariantFromInt64(hnsPosition, &var);
	if (SUCCEEDED(hr))
	{
		hr = pReader->SetCurrentPosition(GUID_NULL, var);
		PropVariantClear(&var);
	}
	return SUCCEEDED(hr);
}

_inline size_t readnumsamples(MFStream* Stream, char* Buffer, size_t numsamples)
{
	BOOL bEOS = FALSE;
	while (Stream->Buffer->AvailableSamples()<numsamples)
	{
		Stream->MF->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, NULL, NULL, NULL);
		Stream->WaitRead(INFINITE, &bEOS);
		if (bEOS)break;
	}
	size_t m_bytes = 0;
	if (Stream->Buffer->AvailableSamples()<numsamples)
		m_bytes = Stream->Buffer->valid;
	else
		m_bytes = numsamples*Stream->Info.BlockAlign;
	if (m_bytes) m_bytes = Stream->Buffer->Read(Buffer, m_bytes);
	return m_bytes;
}

void Close(MFStream* Handle)
{
	if (Handle){ delete Handle; Handle = NULL; }
}
mutex AudioDecoder_MF::locker;
AudioDecoder_MF* AudioDecoder_MF::instance = nullptr;
const wchar_t* AudioDecoder_MF::_author = L"xworkteam powered by Microsoft Media Foundation";
const wchar_t* AudioDecoder_MF::_description=L"media foundation解码库";
const wchar_t* AudioDecoder_MF::_supports = L".wma;.aac;.adts;.mp4;.m4a";
size_t AudioDecoder_MF::pipeNum = 0U;

AudioDecoder_MF::AudioDecoder_MF() : AudioDecoder_base()
{

}

AudioDecoder_MF* AudioDecoder_MF::Create()
{
	if (!instance)
	{
		lock_guard<mutex> lock(locker);
		if (!instance)
		{
			instance = new AudioDecoder_MF();
		}
	}
	return instance;
}

AudioDecoder_MF::~AudioDecoder_MF()
{
}

Version AudioDecoder_MF::GetVersion() const
{
	return Version(1, 0);
}

const wchar_t* AudioDecoder_MF::GetAuthor() const
{
	return _author;
}

const wchar_t* AudioDecoder_MF::GetDescription() const
{
	return _description;
}

const wchar_t* AudioDecoder_MF::GetSupportFormat() const
{
	return _supports;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_MF::OpenLocal(const wchar_t* localPath)
{
	Result<AudioDecoderPipe, AudioDecoderError> result(nullptr, AudioDecoderError::UnSupported);

	if (!experimental::filesystem::exists(experimental::filesystem::path(localPath)))
	{
		result.State = AudioDecoderError::InvalidPath;
		return result;
	}
	MFStream* Handle = new MFStream();
	IMFSourceResolver* ISR = NULL;
	MFCreateSourceResolver(&ISR);
	MF_OBJECT_TYPE type;
	IUnknown* spUnkSource = NULL;
	ISR->CreateObjectFromURL(localPath, MF_RESOLUTION_MEDIASOURCE, NULL, &type, &spUnkSource);
	IMFMediaSource* IMS = NULL;
	spUnkSource->QueryInterface(&IMS);
	IMFPresentationDescriptor* IPD = NULL;
	IMS->QueryInterface(&Handle->MF);
	IMS->CreatePresentationDescriptor(&IPD);
	BOOL selected;
	IMFStreamDescriptor* spStreamDesc = NULL;
	IPD->GetStreamDescriptorByIndex(0, &selected, &spStreamDesc);
	IMFMediaTypeHandler* spHandler = NULL;
	spStreamDesc->GetMediaTypeHandler(&spHandler);
	IMFMediaType* spMediaType = NULL;
	spHandler->GetCurrentMediaType(&spMediaType);

	IMFAttributes* spAttributes = NULL;
	MFCreateAttributes(&spAttributes, 1);
	HRESULT hr = spAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, (IMFSourceReaderCallback *)(Handle));
	if (FAILED(MFCreateSourceReaderFromMediaSource(IMS, spAttributes, &Handle->MF)))
	{
		SafeRelease(&spAttributes);
		if ((DWORD)spMediaType == (DWORD)0xcccccccc)spMediaType = NULL; else SafeRelease(&spMediaType);
		SafeRelease(&spHandler);
		SafeRelease(&spStreamDesc);
		SafeRelease(&IPD);
		SafeRelease(&IMS);
		SafeRelease(&spUnkSource);
		SafeRelease(&ISR);
		SAFE_DELETE(Handle);
		result.State = AudioDecoderError::UnSupported;
		return result;
	}
	IMFMediaType *pAudioType = NULL;
	ConfigureAudioStream(Handle->MF, &pAudioType);
	WAVEFORMATEX * Format = nullptr;
	UINT32 cbFormat = 0;
	MFCreateWaveFormatExFromMFMediaType(pAudioType, &Format, &cbFormat);
	Handle->Info.SampleRate = Format->nSamplesPerSec;

	switch (Format->nChannels)
	{
	case 1u:
		Handle->Info.ChannelLayout = ChannelLayout::Mono;
		break;
	case 2u:
		Handle->Info.ChannelLayout = ChannelLayout::Stereo;
		break;
	case 3u:
		Handle->Info.ChannelLayout = ChannelLayout::Stereo + ChannelLayout::FrontCenter;
		break;
	case 4u:
		Handle->Info.ChannelLayout = ChannelLayout::Quad;
		break;
	case 6u:
		Handle->Info.ChannelLayout = ChannelLayout::Point5_1;
		break;
	case 8u:
		Handle->Info.ChannelLayout = ChannelLayout::Point7_1;
		break;
	default:
		Handle->Info.ChannelLayout = ChannelLayout::Unknown;
		break;
	}
	bool floating = false;
	if (Format->cbSize == sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) && Format->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		WAVEFORMATEXTENSIBLE* wwfx = (WAVEFORMATEXTENSIBLE*)Format;
		floating = IsEqualGUID(wwfx->SubFormat, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT);
	}
	switch (Format->wBitsPerSample)
	{
	case 8u:
		Handle->Info.BitsPerSample = BitsPerSample::SInt8;
		break;
	case 16u:
		Handle->Info.BitsPerSample = BitsPerSample::SInt16LE;
		break;
	case 24u:
		Handle->Info.BitsPerSample = BitsPerSample::SInt24LE;
		break;
	case 32u:
	{
		if (floating)
		{
			Handle->Info.BitsPerSample = BitsPerSample::Float32LE;
		}
		else
		{
			Handle->Info.BitsPerSample = BitsPerSample::SInt32LE;
		}
	}
	break;
	case 64u:
	{
		if (floating)
		{
			Handle->Info.BitsPerSample = BitsPerSample::Float64LE;
		}
	}
	break;
	}
	PROPVARIANT m_pbitrate;
	Handle->MF->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_AUDIO_ENCODING_BITRATE, &m_pbitrate);
	Handle->Info.BitRate = ((double)m_pbitrate.hVal.QuadPart / 1000.0 + 0.5);
	if (Handle->Info.BitRate == 0)
	{
		IPropertyStore* IPS = NULL;
		MFGetService(IMS, MF_PROPERTY_HANDLER_SERVICE, __uuidof(IPropertyStore), (void**)&IPS);
		if (IPS)
		{
			PROPVARIANT value;
			if (SUCCEEDED(IPS->GetValue(PKEY_Audio_EncodingBitrate, &value)) && value.intVal)
			{
				Handle->Info.BitRate = ((double)value.intVal / 1000.0 + 0.5);
			}
		}
		SafeRelease(&IPS);
	}
	UInt64 pDuration = 0;
	IPD->GetUINT64(MF_PD_DURATION, &pDuration);
	PROPVARIANT m_pduration;
	Int64 m_duration = 0;
	Handle->MF->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &m_pduration);
	m_duration = m_pduration.hVal.QuadPart;
	Handle->Info.TimeSec = (double)m_duration / 1e7;
	Handle->Info.TimeMs = Handle->Info.TimeSec * 1000;
	Handle->Info.SampleLength = Handle->Info.TimeSec*Handle->Info.SampleRate;
	Handle->Buffer = new MFCache(Handle->Info.BlockAlign);

	if (Format) {CoTaskMemFree(Format); Format = nullptr;}
	SafeRelease(&pAudioType);

	SafeRelease(&spAttributes);
	if ((DWORD)spMediaType == (DWORD)0xcccccccc)spMediaType = NULL; else SafeRelease(&spMediaType);
	SafeRelease(&spHandler);
	SafeRelease(&spStreamDesc);
	SafeRelease(&IPD);
	SafeRelease(&IMS);
	SafeRelease(&spUnkSource);
	SafeRelease(&ISR);

	Handle->DecodePosMs = 0.0;
	result.Value = Handle;
	result.State = AudioDecoderError::OK;
	pipeNum++;
	return result;
}

Result<AudioDecoderPipe, AudioDecoderError> AudioDecoder_MF::CreateStream()
{
	return Result<AudioDecoderPipe, AudioDecoderError>(nullptr, AudioDecoderError::UnSupported);
}

bool AudioDecoder_MF::IsStream(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MFStream));
		if (pipePtr)
		{
			MFStream* m_MF = static_cast<MFStream*>(pipePtr);
			if (m_MF) return false;
		}
	}
	catch (...){}
	return false;
}

UInt64 AudioDecoder_MF::GetNumberOfPipes()
{
	return pipeNum;
}

bool AudioDecoder_MF::TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MFStream));
		if (pipePtr)
		{
			MFStream* m_MF = static_cast<MFStream*>(pipePtr);
			if (m_MF)
			{
				info = m_MF->Info;
				return true;
			}
		}
	}
	catch (...){}
	return false;

}

AudioDecoderError AudioDecoder_MF::TryGetLocalAudioInfo(const wchar_t * localPath, AudioInfo& info)
{
	if (!experimental::filesystem::exists(experimental::filesystem::path(localPath)))
	{
		return AudioDecoderError::InvalidPath;
	}
	MFStream* Handle = new MFStream();
	IMFSourceReader* MF = NULL;
	IMFSourceResolver* ISR = NULL;
	MFCreateSourceResolver(&ISR);
	MF_OBJECT_TYPE type;
	IUnknown* spUnkSource = NULL;
	ISR->CreateObjectFromURL(localPath, MF_RESOLUTION_MEDIASOURCE, NULL, &type, &spUnkSource);
	IMFMediaSource* IMS = NULL;
	spUnkSource->QueryInterface(&IMS);
	IMFPresentationDescriptor* IPD = NULL;
	IMS->QueryInterface(&Handle->MF);
	IMS->CreatePresentationDescriptor(&IPD);
	BOOL selected;
	IMFStreamDescriptor* spStreamDesc = NULL;
	IPD->GetStreamDescriptorByIndex(0, &selected, &spStreamDesc);
	IMFMediaTypeHandler* spHandler = NULL;
	spStreamDesc->GetMediaTypeHandler(&spHandler);
	IMFMediaType* spMediaType = NULL;
	spHandler->GetCurrentMediaType(&spMediaType);

	IMFAttributes* spAttributes = NULL;
	MFCreateAttributes(&spAttributes, 1);
	HRESULT hr = spAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, (IMFSourceReaderCallback *)(Handle));
	if (FAILED(MFCreateSourceReaderFromMediaSource(IMS, spAttributes, &Handle->MF)))
	{
		SafeRelease(&spAttributes);
		if ((DWORD)spMediaType == (DWORD)0xcccccccc)spMediaType = NULL; else SafeRelease(&spMediaType);
		SafeRelease(&spHandler);
		SafeRelease(&spStreamDesc);
		SafeRelease(&IPD);
		SafeRelease(&IMS);
		SafeRelease(&spUnkSource);
		SafeRelease(&ISR);
		SAFE_DELETE(Handle);
		return AudioDecoderError::UnSupported;
	}
	IMFMediaType *pAudioType = NULL;
	ConfigureAudioStream(Handle->MF, &pAudioType);
	WAVEFORMATEX * Format;
	UINT32 cbFormat = 0;
	MFCreateWaveFormatExFromMFMediaType(pAudioType, &Format, &cbFormat);
	info.SampleRate = Format->nSamplesPerSec;
	switch (Format->nChannels)
	{
	case 1u:
		info.ChannelLayout = ChannelLayout::Mono;
		break;
	case 2u:
		info.ChannelLayout = ChannelLayout::Stereo;
		break;
	case 3u:
		info.ChannelLayout = ChannelLayout::Surround;
		break;
	case 4u:
		info.ChannelLayout = ChannelLayout::Quad;
		break;
	case 6u:
		info.ChannelLayout = ChannelLayout::Point5_1;
		break;
	case 8u:
		info.ChannelLayout = ChannelLayout::Point7_1;
		break;
	default:
		info.ChannelLayout = ChannelLayout::Unknown;
		break;
	}
	bool floating = false;
	if (Format->cbSize == sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) && Format->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		WAVEFORMATEXTENSIBLE* wwfx = (WAVEFORMATEXTENSIBLE*)Format;
		floating = IsEqualGUID(wwfx->SubFormat, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT);
	}
	switch (Format->wBitsPerSample)
	{
	case 8u:
		info.BitsPerSample = BitsPerSample::SInt8;
		break;
	case 16u:
		info.BitsPerSample = BitsPerSample::SInt16LE;
		break;
	case 24u:
		info.BitsPerSample = BitsPerSample::SInt24LE;
		break;
	case 32u:
	{
		if (floating)
		{
			info.BitsPerSample = BitsPerSample::Float32LE;
		}
		else
		{
			info.BitsPerSample = BitsPerSample::SInt32LE;
		}
	}
	break;
	case 64u:
	{
		if (floating)
		{
			info.BitsPerSample = BitsPerSample::Float64LE;
		}
	}
	break;
	}
	PROPVARIANT m_pbitrate;
	Handle->MF->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_AUDIO_ENCODING_BITRATE, &m_pbitrate);
	info.BitRate = ((double)m_pbitrate.hVal.QuadPart / 1000.0 + 0.5);
	if (info.BitRate == 0)
	{
		IPropertyStore* IPS = NULL;
		MFGetService(IMS, MF_PROPERTY_HANDLER_SERVICE, __uuidof(IPropertyStore), (void**)&IPS);
		if (IPS)
		{
			PROPVARIANT value;
			if (SUCCEEDED(IPS->GetValue(PKEY_Audio_EncodingBitrate, &value)) && value.intVal)
			{
				info.BitRate = ((double)value.intVal / 1000.0 + 0.5);
			}
		}
		SafeRelease(&IPS);
	}
	UInt64 pDuration = 0;
	IPD->GetUINT64(MF_PD_DURATION, &pDuration);
	PROPVARIANT m_pduration;
	Int64 m_duration = 0;
	Handle->MF->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &m_pduration);
	m_duration = m_pduration.hVal.QuadPart;
	info.TimeSec = (double)m_duration / 1e7;
	info.TimeMs = info.TimeSec * 1000;
	info.SampleLength = info.TimeSec*info.SampleRate;

	if (Format) { CoTaskMemFree(Format); Format = nullptr; }
	SafeRelease(&pAudioType);

	SafeRelease(&spAttributes);
	if ((DWORD)spMediaType == (DWORD)0xcccccccc)spMediaType = NULL; else SafeRelease(&spMediaType);
	SafeRelease(&spHandler);
	SafeRelease(&spStreamDesc);
	SafeRelease(&IPD);
	SafeRelease(&IMS);
	SafeRelease(&spUnkSource);
	SafeRelease(&ISR);
	return AudioDecoderError::OK;
}

Result<UInt64, AudioDecoderError> AudioDecoder_MF::Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples)
{
	Result<UInt64, AudioDecoderError> result(0ull, AudioDecoderError::End);
	try
	{
		void* pipePtr = pipe.get(sizeof(MFStream));
		if (pipePtr)
		{
			MFStream* m_MF = static_cast<MFStream*>(pipePtr);
			if (m_MF)
			{
				UInt64 Done = 0;
				UInt64 bytes = 0;
				bool end = false;
				while (Done<desiredNumSamples)
				{
					size_t NSamples = desiredNumSamples - Done;
					if (NSamples>512) NSamples = 512;
					UInt64 need = bytes + NSamples*m_MF->Info.BlockAlign;
					if (m_MF->OutBuffer)
					{
						if (m_MF->OutBufferSize<need)
						{
							char* oldmem = m_MF->OutBuffer;
							if ((m_MF->OutBuffer = (char*)realloc(m_MF->OutBuffer, need)) == NULL)
							{
								m_MF->OutBuffer = (char*)malloc(need);
								if (!m_MF->OutBuffer)
								{
									m_MF->OutBufferSize = 0;
									result.Value = 0;
									result.State = AudioDecoderError::Failed;
									return result;
								}
								memcpy(m_MF->OutBuffer, oldmem, m_MF->OutBufferSize);
								free(oldmem);
							}
						}
					}
					else
					{
						m_MF->OutBuffer = (char*)malloc(need);
						m_MF->OutBufferSize = need;
					}
					if (!m_MF->OutBuffer)
					{
						result.Value = 0;
						result.State = AudioDecoderError::Failed;
						return result;
					}
					UInt tbytes = readnumsamples(m_MF, m_MF->OutBuffer + Done*m_MF->Info.BlockAlign, NSamples);
					if (tbytes == 0)
					{
						end = true;
						break;
					}
					bytes += tbytes;
					Done += (tbytes / m_MF->Info.BlockAlign);
				}
				buffer.SetData(m_MF->OutBuffer, m_MF->Info.SampleRate, m_MF->Info.BitsPerSample, Done, m_MF->Info.ChannelLayout, m_MF->DecodePosMs);

				m_MF->DecodePosMs += (double)Done*1000.0 / (double)m_MF->Info.SampleRate;

				result.Value = Done;
				result.State = end ? AudioDecoderError::End : AudioDecoderError::OK;
			}
		}
	}
	catch (...){}
	return result;

}

bool AudioDecoder_MF::TrySeek(const AudioDecoderPipe& pipe, double posMS)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MFStream));
		if (pipePtr)
		{
			MFStream* m_MF = static_cast<MFStream*>(pipePtr);
			if (m_MF)
			{
				Int64 cc = posMS / 1000.0*1e7;
				if (SetPosition(m_MF->MF, cc))
				{
					m_MF->SeekNow = true;
					m_MF->DecodePosMs = posMS;
					m_MF->MF->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, NULL, NULL, NULL);
					BOOL bEOS = NULL;
					m_MF->WaitRead(INFINITE, &bEOS);
					return true;
				}
			}
		}
	}
	catch (...){}
	return false;
}

bool AudioDecoder_MF::TryClose(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MFStream));
		if (pipePtr)
		{
			MFStream* m_MF = (MFStream*)(pipePtr);
			if (m_MF)
			{
				Close(m_MF);
				pipeNum--;
			}
		}
	}
	catch (...){}
	return true;
}

double AudioDecoder_MF::GetCurrentMS(const AudioDecoderPipe& pipe)
{
	try
	{
		void* pipePtr = pipe.get(sizeof(MFStream));
		if (pipePtr)
		{
			MFStream* m_MF = static_cast<MFStream*>(pipePtr);
			if (m_MF) return m_MF->DecodePosMs;
		}
	}
	catch (...){}
	return 0.0;
}
