#include "Config.h"
#include <mfapi.h>
#include <mfidl.h>
#include <Shlwapi.h>
#include <Wmcontainer.h>
#include <mfreadwrite.h>
#include <stdio.h>
#include <mferror.h>
#include <string>
#include <Propvarutil.h>
#include <Propkey.h>

using namespace System;
using namespace System::Audio;

namespace System
{
	namespace Audio
	{
		__uclass __uobjdef(asm = Audio_Decoder_MF, ns = System::Audio, ver = 1.0)  AudioDecoder_MF : public AudioDecoder_base
		{
		private:
			static mutex locker;
			static AudioDecoder_MF* instance;
			static const wchar_t* _author;
			static const wchar_t* _description;
			static const wchar_t* _supports;
			static size_t pipeNum;
			AudioDecoder_MF();
			AudioDecoder_MF(const AudioDecoder_MF&) = delete;
		public:
			__udefctor static AudioDecoder_MF* Create();
			~AudioDecoder_MF();

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