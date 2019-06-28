#pragma once
namespace System
{
	namespace Audio
	{

#define DAUDIO_STRING_LENGTH 200

#define DS_DSSCL_NORMAL 0x00000001
#define DS_DSSCL_PRIORITY 0x00000002
#define DS_DSSCL_EXCLUSIVE 0x00000003
#define DS_DSSCL_WRITEPRIMARY 0x00000004

#define DS_WAVE_FORMAT_EXTENSIBLE  0xFFFE

#define DS_DSBSTATUS_TERMINATED 0x00000020

#define DS_DSBPLAY_LOOPING  0x00000001

#define DS_MAKE_HRESULT(sev,fac,code) \
    ((long) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )

#define DS_FACDS  0x878   /* DirectSound's facility code */
#define DS_MAKE_DSHRESULT(code)  DS_MAKE_HRESULT(1, DS_FACDS, code)
#define DS_DSERR_BUFFERLOST  DS_MAKE_DSHRESULT(150)

		#pragma pack(push, 1)
		struct DS_GUID
		{
			unsigned long  Data1;
			unsigned short Data2;
			unsigned short Data3;
			unsigned char Data4[8];
		};
		#pragma pack(pop)

		struct DirectAudioDeviceDescription
		{
			int deviceID;
			DS_GUID device;
			wchar_t name[DAUDIO_STRING_LENGTH + 1];
			wchar_t vendor[DAUDIO_STRING_LENGTH + 1];
			bool isplayback;
			int maxSimulLines;
		};

#pragma pack(push, 1)
		struct DS_WAVEFORMATEX
		{
			unsigned short wFormatTag;
			unsigned short nChannels;
			unsigned long nSamplesPerSec;
			unsigned long nAvgBytesPerSec;
			unsigned short nBlockAlign;
			unsigned short wBitsPerSample;
			unsigned short cbSize;	
		};
		#pragma pack(pop)

		struct DS_WAVEFORMATEXTENSIBLE
		{
			DS_WAVEFORMATEX Format;
			union{
				unsigned short wValidBitsPerSample;
				unsigned short wSamplesPerBlock;
				unsigned short wReserved;
			} Samples;
			unsigned int dwChannelMask;
			DS_GUID SubFormat;
		};

		class Microsoft_DS
		{
		public:
			static int GetDirectAudioDeviceCount();
			static bool GetDirectAudioDeviceDescription(int mixerIndex, DirectAudioDeviceDescription& desc);
		
			static DS_GUID GetSubFormat(bool isFloat);
			static unsigned int GetChannelMask(unsigned short nCh);

			static void* dsapi_CreateDirectSound(const DS_GUID* guid);
			static void dsapi_ReleaseDirectSound(void*& ds);
			static bool dsapi_SetCooperativeLevel(void* ds, void* hwnd, int lvl);

			static void dsapi_ReleaseBuffer(void*& dsb);

			static void* dsapi_CreatePrimaryBuffer(void* ds, DS_WAVEFORMATEXTENSIBLE& wfe);
			static void* dsapi_CreateSecondaryBuffer(void* ds, unsigned int buffersize, DS_WAVEFORMATEXTENSIBLE& wfe);

			static void* dsapi_QueryNotify(void* dsb);
			static void dsapi_ReleaseNotify(void*& dsnotify);
			static bool dsapi_SetNotificationPositions(void* dsnotify, int offset, void* evt);

			static bool dsapi_SetVolume(void* dsb, int vol);
			static bool dsapi_SetPan(void* dsb, int pan);

			static bool dsapi_GetStatus(void* dsb, unsigned int& status);

			static bool dsapi_Restore(void* dsb);

			static bool dsapi_Stop(void* dsb);

			static bool dsapi_Play(void* dsb, unsigned int dwReserved1, unsigned int dwPriority, unsigned int dwFlags);

			static bool dsapi_GetCurrentPosition(void* dsb, unsigned int& pdwCurrentPlayCursor, unsigned int& pdwCurrentWriteCursor);
			static bool dsapi_SetCurrentPosition(void* dsb, unsigned int pdwCurrentCursor);

			static long dsapi_BufferLock(void* dsb, unsigned int dwOffset, unsigned int dwBytes,
				void*&pvAudioPtr1, unsigned int& dwAudioBytes1,
				void*&pvAudioPtr2, unsigned int& dwAudioBytes2, unsigned int dwFlags);

			static long dsapi_BufferUnlock(void* dsb,
				void* pvAudioPtr1, unsigned int dwAudioBytes1,
				void* pvAudioPtr2, unsigned int dwAudioBytes2);
		};
	}
}