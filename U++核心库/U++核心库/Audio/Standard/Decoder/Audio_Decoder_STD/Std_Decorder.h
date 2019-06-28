#pragma once
#include "Config.h"

namespace System
{
	namespace Audio
	{
		class Std_Decoder
		{
		public:
			virtual std::vector<std::wstring> GetSupportFormats() const { return {}; }

		public:

			virtual void OpenLocal(const wchar_t* localPath, const char* localPath2, Result<AudioDecoderPipe, AudioDecoderError>& ret) = 0;///加载读取本地音频文件

			virtual void CreateStream(Result<AudioDecoderPipe, AudioDecoderError>& ret) = 0;///创建流式解码管线

			virtual bool IsStream(const AudioDecoderPipe& pipe) = 0; ///获取是否为流式解码管线

			virtual bool TryGetPipeInfo(const AudioDecoderPipe& pipe, AudioInfo& info) = 0; ///尝试读取音频信息

			virtual AudioDecoderError TryGetLocalAudioInfo(const wchar_t *path, const char* path2, AudioInfo& info) = 0;  ///尝试直接获取本地音频信息

			virtual void Read(const AudioDecoderPipe& pipe, AudioBuffer& buffer, size_t desiredNumSamples, Result<UInt64, AudioDecoderError>& ret) = 0;///读取音频PCM数据,返回实际读取的样本数

			virtual bool TrySeek(const AudioDecoderPipe& pipe, double posMS) = 0;///设置读取进度

			virtual bool TryClose(const AudioDecoderPipe& pipe) = 0;///关闭读取

			virtual double GetCurrentMS(const AudioDecoderPipe& pipe) = 0;///返回读取进度

		};
	}
}

