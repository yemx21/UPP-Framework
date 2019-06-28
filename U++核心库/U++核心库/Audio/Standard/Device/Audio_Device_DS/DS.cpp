#include "DS.h"
#include <Windows.h>
#include <MMReg.h>
#include <Ks.h>
#include <Ksmedia.h>
#include <dsound.h>
#include <tchar.h>
#include <strsafe.h>
#include "WindowsEnvironment.h"
using namespace System;
using namespace System::Audio;

DS_GUID Microsoft_DS::GetSubFormat(bool isFloat)
{
	auto ret = isFloat ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM;
	DS_GUID conv_ret;
	memcpy(&conv_ret, &ret, sizeof(GUID));
	return conv_ret;
}

unsigned int Microsoft_DS::GetChannelMask(unsigned short nCh)
{
	if (nCh == 8)
		return KSAUDIO_SPEAKER_7POINT1;
	else if (nCh == 6)
		return KSAUDIO_SPEAKER_5POINT1;
	else if (nCh == 4)
		return KSAUDIO_SPEAKER_QUAD;
	else if (nCh == 3)
		return KSAUDIO_SPEAKER_3POINT0;
	else if (nCh == 2)
		return KSAUDIO_SPEAKER_STEREO;
	else
		return KSAUDIO_SPEAKER_DIRECTOUT;
}

void* Microsoft_DS::dsapi_CreateDirectSound(const DS_GUID* guid)
{
	LPDIRECTSOUND8 ds_ptr = nullptr;
	auto ret = ::DirectSoundCreate8((LPCGUID)guid, &ds_ptr, NULL);
	if (ret == DS_OK) return ds_ptr;
	return nullptr;
}

void Microsoft_DS::dsapi_ReleaseDirectSound(void*& ds)
{
	LPDIRECTSOUND8 ds_ptr = (LPDIRECTSOUND8)ds;
	if (ds_ptr) { ds_ptr->Release(); ds_ptr = nullptr; ds = nullptr; }
}

bool Microsoft_DS::dsapi_SetCooperativeLevel(void* ds, void* hwnd, int lvl)
{
	LPDIRECTSOUND8 ds_ptr = (LPDIRECTSOUND8)ds;
	return ds_ptr->SetCooperativeLevel((HWND)hwnd, (DWORD)lvl) == DS_OK;
}

void Microsoft_DS::dsapi_ReleaseBuffer(void*& dsb)
{
	LPDIRECTSOUNDBUFFER8 dsb_ptr = (LPDIRECTSOUNDBUFFER8)dsb;
	if (dsb_ptr)
	{
		dsb_ptr->Release();
		dsb_ptr->Release();
		//dsb_ptr->Release();
		dsb_ptr = nullptr;
		dsb = nullptr;
	}
}

void* Microsoft_DS::dsapi_CreatePrimaryBuffer(void* ds, DS_WAVEFORMATEXTENSIBLE& wfe)
{
	LPDIRECTSOUND8 ds_ptr = (LPDIRECTSOUND8)ds;

	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	LPDIRECTSOUNDBUFFER dsb = NULL;
	if (FAILED(ds_ptr->CreateSoundBuffer(&dsbd, &dsb, NULL))) return nullptr;
	if (FAILED(dsb->SetFormat((LPCWAVEFORMATEX)&wfe)))
	{
		if (dsb) { dsb->Release(); dsb = nullptr; }
	}

	return dsb;
}

void* Microsoft_DS::dsapi_CreateSecondaryBuffer(void* ds, unsigned int buffersize, DS_WAVEFORMATEXTENSIBLE& wfe)
{
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);

	if (OS::Microsoft::winapi_IsWindowsVistaOrGreater())
		dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_TRUEPLAYPOSITION | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
	else
		dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;

	dsbd.dwBufferBytes = (DWORD)buffersize;
	dsbd.lpwfxFormat = (LPWAVEFORMATEX)&wfe;

	LPDIRECTSOUND8 ds_ptr = (LPDIRECTSOUND8)ds;

	LPDIRECTSOUNDBUFFER dsb = NULL;

	if (FAILED(ds_ptr->CreateSoundBuffer(&dsbd, &dsb, NULL))) return nullptr;

	LPDIRECTSOUNDBUFFER8 dsb8 = NULL;
	if (FAILED(dsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&dsb8)))
	dsb->Release(); 
	dsb = nullptr;
	return dsb8;
}

void* Microsoft_DS::dsapi_QueryNotify(void* dsb)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	LPDIRECTSOUNDNOTIFY8 dsnotify = NULL;
	if (dsb8->QueryInterface(IID_IDirectSoundNotify8, (void**)&dsnotify) != DS_OK) return nullptr;
	return dsnotify;
}

void Microsoft_DS::dsapi_ReleaseNotify(void*& dsn)
{
	LPDIRECTSOUNDNOTIFY8 dsnotify = (LPDIRECTSOUNDNOTIFY8)dsn;
	if (dsnotify)
	{
		dsnotify->Release();
		//dsnotify->Release();
		dsnotify = nullptr;
		dsn = nullptr;
	}
}

bool Microsoft_DS::dsapi_SetNotificationPositions(void* dsn, int offset, void* evt)
{
	LPDIRECTSOUNDNOTIFY8 dsnotify = (LPDIRECTSOUNDNOTIFY8)dsn;

	DSBPOSITIONNOTIFY NotifyPos[1];
	NotifyPos[0].dwOffset = offset;
	NotifyPos[0].hEventNotify = evt;

	return dsnotify->SetNotificationPositions(1, NotifyPos) == DS_OK;
}

bool Microsoft_DS::dsapi_GetStatus(void* dsb, unsigned int& status)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->GetStatus((LPDWORD)&status) == DS_OK;
}

bool Microsoft_DS::dsapi_Restore(void* dsb)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->Restore() == DS_OK;
}

bool Microsoft_DS::dsapi_Stop(void* dsb)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->Stop() == DS_OK;
}

bool Microsoft_DS::dsapi_GetCurrentPosition(void* dsb, unsigned int& pdwCurrentPlayCursor, unsigned int& pdwCurrentWriteCursor)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->GetCurrentPosition((LPDWORD)&pdwCurrentPlayCursor, (LPDWORD)&pdwCurrentWriteCursor) == DS_OK;
}

bool Microsoft_DS::dsapi_SetCurrentPosition(void* dsb, unsigned int pdwCurrentCursor)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->SetCurrentPosition((DWORD)pdwCurrentCursor) == DS_OK;
}

bool Microsoft_DS::dsapi_Play(void* dsb, unsigned int dwReserved1, unsigned int dwPriority, unsigned int dwFlags)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->Play((DWORD)dwReserved1, (DWORD)dwPriority, (DWORD)dwFlags) == DS_OK;
}

long Microsoft_DS::dsapi_BufferLock(void* dsb, unsigned int dwOffset, unsigned int dwBytes, 
	void*&pvAudioPtr1, unsigned int& dwAudioBytes1,
	void*&pvAudioPtr2, unsigned int& dwAudioBytes2, unsigned int dwFlags)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return (long)dsb8->Lock((DWORD)dwOffset, (DWORD)dwBytes,
		(LPVOID*)&pvAudioPtr1, (LPDWORD)&dwAudioBytes1,
		(LPVOID*)&pvAudioPtr2, (LPDWORD)&dwAudioBytes2, (DWORD)dwFlags);
}

long Microsoft_DS::dsapi_BufferUnlock(void* dsb, void* pvAudioPtr1, unsigned int dwAudioBytes1,
	void*pvAudioPtr2, unsigned int dwAudioBytes2)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return (long)dsb8->Unlock((LPVOID)pvAudioPtr1, (DWORD)dwAudioBytes1,
		(LPVOID)pvAudioPtr2, (DWORD)dwAudioBytes2);
}

bool Microsoft_DS::dsapi_SetVolume(void* dsb, int vol)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->SetVolume(vol) == DS_OK;
}

bool Microsoft_DS::dsapi_SetPan(void* dsb, int pan)
{
	LPDIRECTSOUNDBUFFER8 dsb8 = (LPDIRECTSOUNDBUFFER8)dsb;
	return dsb8->SetPan(pan) == DS_OK;
}

#pragma region AudioDevice

#define MAX_DS_DEVICES 60
#define WAIT_BETWEEN_CACHE_REFRESH_MILLIS 30000

struct DS_AudioDeviceCache
{
	INT32 mixerIndex;
	BOOL isSource;
	void* dev;
	INT32 refCount;
	GUID guid;
};

struct DS_RefreshCacheStruct
{
	INT32 currMixerIndex;
	BOOL isSource;
};

static DS_AudioDeviceCache g_audioDeviceCache[MAX_DS_DEVICES];
static GUID CLSID_DAUDIO_Zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static int g_cacheCount = 0;
static UINT64 g_lastCacheRefreshTime = 0ul;
static int g_mixerCount = 0;

__inline bool isEqualGUID(LPGUID lpGuid1, LPGUID lpGuid2)
{
	if (lpGuid1 == NULL || lpGuid2 == NULL)
	{
		if (lpGuid1 == lpGuid2) return TRUE;

		if (lpGuid1 == NULL)
			lpGuid1 = (LPGUID)(&CLSID_DAUDIO_Zero);
		else
			lpGuid2 = (LPGUID)(&CLSID_DAUDIO_Zero);
	}
	return memcmp(lpGuid1, lpGuid2, sizeof(GUID)) == 0;
}

int findCacheItemByGUID(LPGUID lpGuid, BOOL isSource)
{
	int i;
	for (i = 0; i < g_cacheCount; i++)
		if (isSource == g_audioDeviceCache[i].isSource
			&& isEqualGUID(lpGuid, &(g_audioDeviceCache[i].guid)))
			return i;
	return -1;
}

int findCacheItemByMixerIndex(int mixerIndex)
{
	int i;
	for (i = 0; i < g_cacheCount; i++)
		if (g_audioDeviceCache[i].mixerIndex == mixerIndex)
			return i;
	return -1;
}

BOOL CALLBACK DS_RefreshCacheEnum(LPGUID lpGuid, LPCSTR lpstrDescription,
	LPCSTR lpstrNameSpace, DS_RefreshCacheStruct* rs)
{
	INT32 cacheIndex = findCacheItemByGUID(lpGuid, rs->isSource);
	if (cacheIndex == -1)
	{
		if (g_cacheCount < MAX_DS_DEVICES - 1)
		{
			g_audioDeviceCache[g_cacheCount].mixerIndex = rs->currMixerIndex;
			g_audioDeviceCache[g_cacheCount].isSource = rs->isSource;
			g_audioDeviceCache[g_cacheCount].dev = NULL;
			g_audioDeviceCache[g_cacheCount].refCount = 0;

			if (lpGuid == NULL)
			{
				memset(&(g_audioDeviceCache[g_cacheCount].guid), 0, sizeof(GUID));
			}
			else
			{
				memcpy(&(g_audioDeviceCache[g_cacheCount].guid), lpGuid, sizeof(GUID));
			}

			g_cacheCount++;
			rs->currMixerIndex++;
		}
		else
		{
			/* failure case: more than MAX_DS_DEVICES available... */
		}
	}
	else
	{
		g_audioDeviceCache[cacheIndex].mixerIndex = rs->currMixerIndex;
		rs->currMixerIndex++;
	}
	return TRUE;
}

BOOL CALLBACK DS_GetDescEnum(LPGUID lpGuid, LPCTSTR lpstrDescription,
	LPCTSTR lpstrNameSpace, DirectAudioDeviceDescription* desc)
{
	INT32 cacheIndex = findCacheItemByGUID(lpGuid, g_audioDeviceCache[desc->deviceID].isSource);
	if (cacheIndex == desc->deviceID) 
	{
		StringCchCopy(desc->name, DAUDIO_STRING_LENGTH, lpstrDescription);
		memcpy(&desc->device, lpGuid, sizeof(GUID));
		StringCchCopy(desc->vendor, DAUDIO_STRING_LENGTH, lpstrNameSpace);
		desc->maxSimulLines = -1;
		return FALSE;
	}
	return TRUE;
}

int Microsoft_DS::GetDirectAudioDeviceCount()
{
	DS_RefreshCacheStruct rs;
	INT32 cacheIndex;
	if (g_lastCacheRefreshTime == 0
		|| (UINT64)timeGetTime() > (UINT64)(g_lastCacheRefreshTime + WAIT_BETWEEN_CACHE_REFRESH_MILLIS)) {

		for (cacheIndex = 0; cacheIndex < g_cacheCount; cacheIndex++)
			g_audioDeviceCache[cacheIndex].mixerIndex = -1;

		rs.currMixerIndex = 0;
		rs.isSource = TRUE;
		DirectSoundEnumerate((LPDSENUMCALLBACK)DS_RefreshCacheEnum, &rs);
		/* if we only got the Primary Sound Driver (GUID=NULL),
		* then there aren't any playback devices installed */
		if (rs.currMixerIndex == 1) 
		{
			cacheIndex = findCacheItemByGUID(NULL, TRUE);
			if (cacheIndex == 0) {
				rs.currMixerIndex = 0;
				g_audioDeviceCache[0].mixerIndex = -1;
			}
		}
		g_mixerCount = rs.currMixerIndex;
		g_lastCacheRefreshTime = (UINT64)timeGetTime();
	}
	return g_mixerCount;
}

bool Microsoft_DS::GetDirectAudioDeviceDescription(int mixerIndex, DirectAudioDeviceDescription& desc)
{
	desc.deviceID = findCacheItemByMixerIndex(mixerIndex);
	if (desc.deviceID < 0) return false;
	desc.maxSimulLines = 0;
	desc.isplayback = g_audioDeviceCache[desc.deviceID].isSource;
	if (desc.isplayback) DirectSoundEnumerate((LPDSENUMCALLBACK)DS_GetDescEnum, &desc);
	return desc.maxSimulLines == -1;
}
#pragma endregion