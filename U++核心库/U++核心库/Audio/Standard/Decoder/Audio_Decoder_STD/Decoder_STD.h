#include "Config.h"
#include "Std_Decorder.h"
#include "Decoders.h"
#include <map>
using namespace System;
using namespace System::Audio;

namespace System
{
	namespace Audio
	{
		__uclass __uobjdef(asm = Audio_Decoder_STD, ns = System::Audio, ver = 1.0)  AudioDecoder_STD : public AudioDecoder_base
		{
		private:
			static std::mutex locker;
			static AudioDecoder_STD* instance;
			static const wchar_t* _author;
			static const wchar_t* _description;
			static const wchar_t* _supports;

			std::map<std::wstring, int> _decodermaps;
			std::map<int, std::unique_ptr<Std_Decoder>> _decoders;

			static size_t pipeNum;
			AudioDecoder_STD();
			AudioDecoder_STD(const AudioDecoder_STD&) = delete;
		public:
			__udefctor static AudioDecoder_STD* Create();
			~AudioDecoder_STD();

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