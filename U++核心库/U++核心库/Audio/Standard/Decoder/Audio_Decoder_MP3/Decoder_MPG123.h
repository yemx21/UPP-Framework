#include "Config.h"
#include "Extras\mpg123.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;
using namespace System::Audio;

namespace System
{
	namespace Audio
	{
		class MPG123API
		{
		public:
			static int (*mpg123_init)();
			static void(*mpg123_exit)(); 
			static mpg123_handle* (*mpg123_new)(const char* decoder, int *error);
			static int(*mpg123_open)(mpg123_handle *mh, const char *path);
			static int(*mpg123_close)(mpg123_handle *mh);
			static int (*mpg123_format_none)(mpg123_handle *mh);
			static int (*mpg123_format)(mpg123_handle *mh, long rate, int channels, int encodings);
			static int(*mpg123_getformat)(mpg123_handle *mh, long *rate, int *channels, int *encoding);

			static int(*mpg123_info)(mpg123_handle *mh, struct mpg123_frameinfo *mi);
			static int(*mpg123_scan)(mpg123_handle *mh);
			static off_t (*mpg123_length)(mpg123_handle *mh);

			static off_t(*mpg123_tell)(mpg123_handle *mh);
			static void (*mpg123_delete)(mpg123_handle *mh);
			static int (*mpg123_read)(mpg123_handle *mh, unsigned char *outmemory, size_t outmemsize, size_t *done);
			static off_t (*mpg123_seek)(mpg123_handle *mh, off_t sampleoff, int whence);

			#define CHECK_API(test) if((test)==nullptr) goto ERR

			static API_PTR module;
			static Result<bool> DynmaicInit()
			{
				Result<bool> result(false);
				module = Microsoft::winapi_LoadLibrary(L"libmpg123-0.dll");
				if (module)
				{
					CHECK_API(mpg123_init = (int(*)()) Microsoft::winapi_GetProcAddress(module, "mpg123_init"));
					CHECK_API(mpg123_exit = (void(*)()) Microsoft::winapi_GetProcAddress(module, "mpg123_exit"));
					CHECK_API(mpg123_new = (mpg123_handle* (*)(const char*, int *)) Microsoft::winapi_GetProcAddress(module, "mpg123_new"));
					CHECK_API(mpg123_open = (int(*)(mpg123_handle *, const char *)) Microsoft::winapi_GetProcAddress(module, "mpg123_open"));
					CHECK_API(mpg123_close = (int(*)(mpg123_handle *)) Microsoft::winapi_GetProcAddress(module, "mpg123_close"));
					CHECK_API(mpg123_format_none = (int(*)(mpg123_handle *)) Microsoft::winapi_GetProcAddress(module, "mpg123_format_none"));
					CHECK_API(mpg123_format = (int(*)(mpg123_handle *, long, int, int)) Microsoft::winapi_GetProcAddress(module, "mpg123_format"));
					CHECK_API(mpg123_getformat = (int(*)(mpg123_handle *, long *, int *, int *)) Microsoft::winapi_GetProcAddress(module, "mpg123_getformat"));

					CHECK_API(mpg123_info = (int(*)(mpg123_handle *, struct mpg123_frameinfo *mi)) Microsoft::winapi_GetProcAddress(module, "mpg123_info"));
					CHECK_API(mpg123_scan = (int(*)(mpg123_handle *)) Microsoft::winapi_GetProcAddress(module, "mpg123_scan"));
					CHECK_API(mpg123_length = (off_t(*)(mpg123_handle *)) Microsoft::winapi_GetProcAddress(module, "mpg123_length"));

					CHECK_API(mpg123_tell = (off_t(*)(mpg123_handle *)) Microsoft::winapi_GetProcAddress(module, "mpg123_tell"));
					CHECK_API(mpg123_delete = (void(*)(mpg123_handle *)) Microsoft::winapi_GetProcAddress(module, "mpg123_delete"));
					CHECK_API(mpg123_read = (int(*)(mpg123_handle *, unsigned char *, size_t, size_t *)) Microsoft::winapi_GetProcAddress(module, "mpg123_read"));
					CHECK_API(mpg123_seek = (off_t(*)(mpg123_handle *, off_t, int)) Microsoft::winapi_GetProcAddress(module, "mpg123_seek"));

					result.Value = true;
					return result;
				ERR:
					if (Microsoft::winapi_FreeLibrary(module))
						module = NULL;
					else
						result.WriteMessage(L"can not free libmpg123-0.dll");
					return result;
				}
				else
				{
					result.WriteMessage(L"can not find or load libmpg123-0.dll");
				}
				return result;
			}

			static Result<bool> DynamicExit()
			{
				Result<bool> result(false);
				if (module)
				{
					if (Microsoft::winapi_FreeLibrary(module))
					{
						module = NULL;
						result.Value = true;
					}
					else
						result.WriteMessage(L"can not free libmpg123-0.dll");
				}
				else
				{
					result.Value = true;
					result.WriteMessage(L"already free libmpg123-0.dll");
				}
				return result;
			}
		};

		__uclass __uobjdef(asm = Audio_Decoder_MP3, ns = System::Audio, ver = 1.0)  AudioDecoder_MPG123 : public AudioDecoder_base
		{
		private:
			static UShort mp1;
			static mutex locker;
			static AudioDecoder_MPG123* instance;
			static const wchar_t* _author;
			static const wchar_t* _description;
			static const wchar_t* _supports;
			static size_t pipeNum;
			AudioDecoder_MPG123();
			AudioDecoder_MPG123(const AudioDecoder_MPG123&) = delete;
		public:
			__udefctor static AudioDecoder_MPG123* Create();
			~AudioDecoder_MPG123();

			__uref Version GetVersion() const  override;
			__uref const wchar_t* GetAuthor() const  override;
			__uref const wchar_t* GetDescription() const  override;
			__uref const wchar_t* GetSupportFormat() const  override;

		public:

			__uref Result<AudioDecoderPipe, AudioDecoderError> OpenLocal(const wchar_t* localPath) override;///加载读取本地音频文件

			__uref Result<AudioDecoderPipe, AudioDecoderError> CreateStream() override;///创建流式解码管线

			__uref bool IsStream(const AudioDecoderPipe& pipe) override; ///获取是否为流式解码管线

			__uref UInt64 GetNumberOfPipes() override;///获取当前有效管线数量

			__uref bool TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info) override; ///尝试读取音频信息

			__uref AudioDecoderError TryGetLocalAudioInfo(const wchar_t *path, AudioInfo& info) override;  ///尝试直接获取本地音频信息

			__uref Result<UInt64, AudioDecoderError> Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples) override;///读取音频PCM数据,返回实际读取的样本数

			__uref bool TrySeek(const AudioDecoderPipe& pipe, double posMS) override;///设置读取进度

			__uref bool TryClose(const AudioDecoderPipe& pipe) override;///关闭读取

			__uref double GetCurrentMS(const AudioDecoderPipe& pipe) override;///返回读取进度

			REFLECT_CLASS(AudioDecoder_base)

		};
	}
}