#pragma once
#include "Config.h"
#include "Std_Decorder.h"

using namespace System;
using namespace System::Audio;

namespace System
{
	namespace Audio
	{
		class Std_MV : public Std_Decoder
		{
		public:
			Std_MV();
			Std_MV(const Std_MV&) = delete;
			~Std_MV();

			std::vector<std::wstring> GetSupportFormats() const override;

		public:

			void OpenLocal(const wchar_t* localPath, const char* localPath2, Result<AudioDecoderPipe, AudioDecoderError>& ret) override;///加载读取本地音频文件

			void CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& ret) override;///创建流式解码管线

			bool IsStream(const AudioDecoderPipe& pipe) override; ///获取是否为流式解码管线

			bool TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info) override; ///尝试读取音频信息

			AudioDecoderError TryGetLocalAudioInfo(const wchar_t *path, const char* path2, AudioInfo& info) override;  ///尝试直接获取本地音频信息

			void Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& ret) override;///读取音频PCM数据,返回实际读取的样本数

			bool TrySeek(const AudioDecoderPipe& pipe, double posMS) override;///设置读取进度

			bool TryClose(const AudioDecoderPipe& pipe) override;///关闭读取

			double GetCurrentMS(const AudioDecoderPipe& pipe) override;///返回读取进度
		};
	}
}
