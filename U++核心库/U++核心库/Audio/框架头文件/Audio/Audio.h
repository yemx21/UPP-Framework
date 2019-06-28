#pragma once
#include "Config.h"


namespace System
{
	namespace Internal
	{
		namespace Audio
		{
			/*音频库插件中心*/
			typedef safe_void_ptr AddinCenter;

			/*音频插件中心事件回调函数*/
			typedef void(*AddinCenterEventCallBack)(const AddinCenter& sender, CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, double, double, const UInt128& obj, const wstring& msg, const VariadicAny& args);

			/*音频库插件类型*/
			enum class addin_types
			{
				/*音频解码器*/
				AudioDecoder,
				/*音频音效处理器*/
				AudioDSP,
				/*音频设备*/
				AudioDevice,
				/*音频可视化*/
				AudioVisual,
				/*音频编码器*/
				AudioEncoder,

				/*歌词解码器*/
				LryicDecoder,
				/*歌词编码器*/
				LryicEncoder,
				/*歌词可视化*/
				LryicVisual,
			};
			

			/*音频库插件中心管理器*/
			struct AddinCenterMgr
			{
				/*创建插件中心*/
				AddinCenter (*Create)();

				/*销毁插件中心*/
				void(*Destory)(AddinCenter&);

				/*加载插件至指定插件中心并返回插件全局唯一标识集合及其所属部分标识*/
				Result<bool, std::vector<std::pair<UInt128, addin_types>>, safe_void_ptr>(*AddSection)(const AddinCenter&, const wchar_t*);

				/*获取是否可移除指定标识部分所有插件*/
				Result<bool, std::vector<UInt128>>(*CanRemoveSection)(const AddinCenter&, const safe_void_ptr&);

				/*移除指定标识部分所有插件并返回实际移除的插件*/
				Result<bool, std::vector<UInt128>>(*RemoveSection)(const AddinCenter&, const safe_void_ptr&);

				/*获取是否可移除指定标识的插件*/
				Result<bool>(*CanRemove)(const AddinCenter&, const UInt128&);

				/*移除指定标识的插件并返回一并移除的部分标识*/
				Result<bool, safe_void_ptr>(*Remove)(const AddinCenter&, const UInt128&);

				/*获取指定标识的插件版本信息*/
				Result<bool, Version>(*GetVersion)(const AddinCenter&, const UInt128&);

				/*获取指定标识的插件作者*/
				Result<bool, const wchar_t*>(*GetAuthor)(const AddinCenter&, const UInt128&);

				/*获取指定标识的插件描述*/
				Result<bool, const wchar_t*>(*GetDescription)(const AddinCenter&, const UInt128&);

				/*获取指定标识的插件格式支持信息*/
				Result<bool, const wchar_t*>(*GetSupportFormat)(const AddinCenter&, const UInt128&);

				/*获取指定标识的插件当前持有的有效管线数量*/
				Result<bool, UInt>(*GetNumPipes)(const AddinCenter&, const UInt128&);

				/*向指定插件中心注册事件回调函数并返回事件标识*/
				Result<bool, EventToken>(*RegisiterEventCallBack)(const AddinCenter&, AddinCenterEventCallBack);

				/*从指定插件中心移除指定标识事件回调函数*/
				Result<bool>(*UnRegisiterEventCallBack)(const AddinCenter&, EventToken);

			};

			/*音乐播放器*/
			typedef safe_void_ptr AudioPlayer;

			/*音频树错误*/
			enum class audio_tree_error
			{
				/*无*/
				None,
				/*错误*/
				Failed,
				/*非法路径*/
				InvalidPath,
				/*非法插件中心*/
				InvalidCenter,
				/*非法指定的音频插件*/
				Invalid,
				/*已处于播放状态*/
				AlreadyOnThread,
				/*不支持*/
				Unsupported,
				/*采样率不支持*/
				UnsupportedSampleRate,
				/*位深度不支持*/
				UnsupportedBitsPerSample,
				/*声道配置不支持*/
				UnsupportedChannel,
				/*流解码不支持*/
				UnsupportedStream,
				/*音频信息不支持*/
				UnsupportedInfo,
				/*未知*/
				Unknown
			};

			/*音频快速傅里叶变换数据窗类型*/
			enum class audio_fft_window
			{
				/*无*/
				None,
				/*汉明窗*/
				HammingWindow,
				/*汉宁窗*/
				HannWindow,
				/*布莱克曼-哈里斯窗*/
				BlackmannHarrisWindow,
			};

			/*音乐播放器管理器*/
			struct AudioPlayerMgr
			{
				/*创建播放器*/
				Result<bool, AudioPlayer, UInt128>(*Create)(const AddinCenter&, UInt64);

				/*销毁播放器*/
				void(*Destory)(AudioPlayer&);

				/*获取指定播放器缓冲区长度*/
				Result<bool, UInt64>(*GetBufferTimeMs)(const AudioPlayer&);

				/*设置指定播放器缓冲区长度*/
				Result<bool>(*SetBufferTimeMs)(const AudioPlayer&, UInt64);

				/*打开指定音频文件并返回音频树标识*/
				Result<UInt128, audio_tree_error, CoreSubObjects, int>(*Open)(const AudioPlayer&, const wchar_t*, const UInt128&, const UInt128&, int);

				/*获取指定音频树音频信息*/
				Result<bool, AudioInfo>(*GetInfo)(const AudioPlayer&, const UInt128&);

				/*播放指定音频树*/
				Result<bool>(*Play)(const AudioPlayer&, const UInt128&);

				/*暂停指定音频树*/
				Result<bool>(*Pause)(const AudioPlayer&, const UInt128&);

				/*停止指定的音频树并在播放结束事件中指定辨认标识*/
				Result<bool>(*Stop)(const AudioPlayer&, const UInt128&, int);

				/*获取指定音频树当前播放进度*/
				Result<bool, double>(*GetCurrent)(const AudioPlayer&, const UInt128&);

				/*设置指定音频树当前播放进度*/
				Result<bool>(*SetCurrent)(const AudioPlayer&, const UInt128&, double);

				/*获取指定音频树均衡器配置*/
				Result<bool, int, int>(*GetEqConfig)(const AudioPlayer&);

				/*设置指定音频树均衡器配置*/
				Result<bool, int>(*SetEqConfig)(const AudioPlayer&, int, int);

				/*获取指定音频树均衡器状态*/
				Result<bool, bool>(*GetEqActive)(const AudioPlayer&);

				/*设置指定音频树均衡器状态*/
				Result<bool>(*SetEqActive)(const AudioPlayer&, bool);

				/*获取指定音频树均衡器指定频段增益值*/
				Result<bool, double>(*GetEqGain)(const AudioPlayer&, int);

				/*设置指定音频树均衡器指定频段增益值*/
				Result<bool, double>(*SetEqGain)(const AudioPlayer&, int, double);

				/*获取指定音频树音调增益值*/
				Result<bool, double>(*GetPitch)(const AudioPlayer&);

				/*设置指定音频树音调增益值*/
				Result<bool>(*SetPitch)(const AudioPlayer&, double);

				/*获取指定音频树节奏增益值*/
				Result<bool, double>(*GetTempo)(const AudioPlayer&);

				/*设置指定音频树节奏增益值*/
				Result<bool>(*SetTempo)(const AudioPlayer&, double);

				/*获取指定音频树频率增益值*/
				Result<bool, double>(*GetRate)(const AudioPlayer&);

				/*设置指定音频树频率增益值*/
				Result<bool>(*SetRate)(const AudioPlayer&, double);

				/*获取指定音频树音量值*/
				Result<bool, double>(*GetVolume)(const AudioPlayer&);

				/*设置指定音频树音量值*/
				Result<bool>(*SetVolume)(const AudioPlayer&, double);

				/*获取指定音频树声道平衡值*/
				Result<bool, double>(*GetPan)(const AudioPlayer&);

				/*设置指定音频树声道平衡值*/
				Result<bool>(*SetPan)(const AudioPlayer&, double);

				/*获取指定音频树快速傅里叶变换数据*/
				Result<bool, float**>(*GetFFTData)(const AudioPlayer&, UShort, UInt, audio_fft_window, bool, unsigned short&, unsigned int&);

				/*获取指定音频树音量单位数据*/
				Result<bool, float*>(*GetVUData)(const AudioPlayer&, UShort, UInt, unsigned short&, unsigned int&);
			};

			/*音频库接口*/
			struct AudioInterface
			{
				/*获取音频库插件中心管理器接口*/
				AddinCenterMgr* (*GetCenterMgr)();
				/*获取音频播放器管理器接口*/
				AudioPlayerMgr* (*GetPlayerMgr)();
			};

			/*获取音频库接口*/
			typedef AudioInterface* (*GetAudioInterface)();
		}
	}
}