#include "Audio.h"
#include "Player.h"
using namespace System::Audio;
using namespace System::Internal::Audio;

#pragma region Center

/*创建插件中心*/
AddinCenter AddinCenter_Create();

/*销毁插件中心*/
void AddinCenter_Destory(AddinCenter&);

/*加载插件至指定插件中心并返回插件全局唯一标识*/
Result<bool, std::vector<std::pair<UInt128, addin_types>>, safe_void_ptr> AddinCenter_AddSection(const AddinCenter&, const wchar_t*);

/*获取是否可移除指定标识部分所有插件*/
Result<bool, std::vector<UInt128>>AddinCenter_CanRemoveSection(const AddinCenter&, const safe_void_ptr&);

/*移除指定标识部分所有插件并返回实际移除的插件*/
Result<bool, std::vector<UInt128>>AddinCenter_RemoveSection(const AddinCenter&, const safe_void_ptr&);

/*获取是否可移除指定标识的插件*/
Result<bool>AddinCenter_CanRemove(const AddinCenter&, const UInt128&);

/*移除指定标识的插件*/
Result<bool, safe_void_ptr>AddinCenter_Remove(const AddinCenter&, const UInt128&);


/*获取指定标识的插件版本信息*/
Result<bool, Version> AddinCenter_GetVersion(const AddinCenter&, const UInt128&);

/*获取指定标识的插件作者*/
Result<bool, const wchar_t*> AddinCenter_GetAuthor(const AddinCenter&, const UInt128&);

/*获取指定标识的插件描述*/
Result<bool, const wchar_t*> AddinCenter_GetDescription(const AddinCenter&, const UInt128&);

/*获取指定标识的插件格式支持信息*/
Result<bool, const wchar_t*> AddinCenter_GetSupportFormat(const AddinCenter&, const UInt128&);

/*获取指定标识的插件当前持有的有效管线数量*/
Result<bool, UInt> AddinCenter_GetNumPipes(const AddinCenter&, const UInt128&);

/*向指定插件中心注册事件回调函数并返回事件标识*/
Result<bool, EventToken> AddinCenter_RegisiterEventCallBack(const AddinCenter&, AddinCenterEventCallBack);

/*从指定插件中心移除指定标识事件回调函数*/
Result<bool> AddinCenter_UnRegisiterEventCallBack(const AddinCenter&, EventToken);


AddinCenterMgr CenterMgr=
{
	AddinCenter_Create,
	AddinCenter_Destory,
	AddinCenter_AddSection,
	AddinCenter_CanRemoveSection,
	AddinCenter_RemoveSection,
	AddinCenter_CanRemove,
	AddinCenter_Remove,
	AddinCenter_GetVersion,
	AddinCenter_GetAuthor,
	AddinCenter_GetDescription,
	AddinCenter_GetSupportFormat,
	AddinCenter_GetNumPipes,
	AddinCenter_RegisiterEventCallBack,
	AddinCenter_UnRegisiterEventCallBack,
};
/*获取音频库插件中心管理器接口*/
AddinCenterMgr* Interface_GetCenterMgr()
{
	return &CenterMgr;
}

/*创建插件中心*/
AddinCenter AddinCenter_Create()
{
	try
	{
		return new PluginCenter();
	}
	catch (...)
	{
		return nullptr;
	}
}

/*销毁插件中心*/
void AddinCenter_Destory(AddinCenter& center)
{
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			delete pc;
			pc = nullptr;
			center = nullptr;
		}
	}
	catch (...){}
}

/*加载插件至指定插件中心并返回插件全局唯一标识*/
Result<bool, std::vector<std::pair<UInt128, addin_types>>, safe_void_ptr> AddinCenter_AddSection(const AddinCenter& center, const wchar_t* path)
{
	Result<bool, std::vector<std::pair<UInt128, addin_types>>, safe_void_ptr> result(false, std::vector<std::pair<UInt128, addin_types>>(), nullptr);
	std::vector<std::pair<UInt128, AddinTypes>> addins;
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			auto error = pc->AddSection(path, addins);
			result.State = error.State;
  			if (error.Value==FactoryDllError::OK)
			{
				for (auto& addin : addins)
				{
					result.Value2.push_back(make_pair(addin.first, (addin_types)addin.second));
				}
				result.Value1 = true;
			}
			else if (error.Value == FactoryDllError::DllLoadFailed)
			{
				result.WriteMessage(L"dll load failed");
			}
			else if (error.Value == FactoryDllError::EntryNotFound)
			{
				result.WriteMessage(L"dll entry not found");
			}
			else if (error.Value == FactoryDllError::FileNotFound)
			{
				result.WriteMessage(L"dll file not found");
			}
			else if (error.Value == FactoryDllError::InitExitEmpty)
			{
				result.WriteMessage(L"dll init function is empty");
			}
			else if (error.Value == FactoryDllError::Unknown)
			{
				result.WriteMessage(L"internal error");
			}
		}
	}
	catch (...)
	{
		result.WriteMessage(L"critical error");
	}
	return result;
}

/*获取是否可移除指定标识部分所有插件*/
Result<bool, std::vector<UInt128>>AddinCenter_CanRemoveSection(const AddinCenter& center, const safe_void_ptr& section)
{
	std::vector<std::pair<UInt128, AddinTypes>> addins;
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return Result<bool, std::vector<UInt128>>(false, std::vector<UInt128>());
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			return pc->CanRemoveSection(section);
		}
	}
	catch (...){}
	return Result<bool, std::vector<UInt128>>(false, std::vector<UInt128>());
}

/*移除指定标识部分所有插件并返回实际移除的插件*/
Result<bool, std::vector<UInt128>>AddinCenter_RemoveSection(const AddinCenter& center, const safe_void_ptr& section)
{
	std::vector<std::pair<UInt128, AddinTypes>> addins;
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return Result<bool, std::vector<UInt128>>(false, std::vector<UInt128>());
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			return pc->RemoveSection(section);
		}
	}
	catch (...){}
	return Result<bool, std::vector<UInt128>>(false, std::vector<UInt128>());
}

/*获取是否可移除指定标识的插件*/
Result<bool>AddinCenter_CanRemove(const AddinCenter& center, const UInt128& addin)
{
	std::vector<std::pair<UInt128, AddinTypes>> addins;
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return Result<bool>(false);
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			return pc->CanRemoveAddin(addin);
		}
	}
	catch (...){}
	return Result<bool>(false);
}

/*移除指定标识的插件*/
Result<bool, safe_void_ptr>AddinCenter_Remove(const AddinCenter& center, const UInt128& addin)
{
	std::vector<std::pair<UInt128, AddinTypes>> addins;
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return Result<bool, safe_void_ptr>(false, nullptr);
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			return pc->RemoveAddin(addin);
		}
	}
	catch (...){}
	return Result<bool, safe_void_ptr>(false,  nullptr);
}

/*获取指定标识的插件版本信息*/
Result<bool, Version> AddinCenter_GetVersion(const AddinCenter& center, const UInt128& addin)
{
	Result<bool, Version> result(false, Version());
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.Value = pc->GetVersion(addin, result.State);
			result.Value = true;
		}
	}
	catch (...){}
	return result;
}

/*获取指定标识的插件作者*/
Result<bool, const wchar_t*> AddinCenter_GetAuthor(const AddinCenter& center, const UInt128& addin)
{
	Result<bool, const wchar_t*> result(false, nullptr);
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.State = pc->GetAuthor(addin);
			result.Value = true;
		}
	}
	catch (...){}
	return result;
}

/*获取指定标识的插件描述*/
Result<bool, const wchar_t*> AddinCenter_GetDescription(const AddinCenter& center, const UInt128& addin)
{
	Result<bool, const wchar_t*> result(false, nullptr);
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.State = pc->GetDescription(addin);
			result.Value = true;
		}
	}
	catch (...){}
	return result;
}

/*获取指定标识的插件格式支持信息*/
Result<bool, const wchar_t*> AddinCenter_GetSupportFormat(const AddinCenter& center, const UInt128& addin)
{
	Result<bool, const wchar_t*> result(false, nullptr);
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.State = pc->GetSupportFormat(addin);
			result.Value = true;
		}
	}
	catch (...){}
	return result;
}

/*获取指定标识的插件当前持有的有效管线数量*/
Result<bool, UInt> AddinCenter_GetNumPipes(const AddinCenter& center, const UInt128& addin)
{
	Result<bool, UInt> result(false, 0U);
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.State = pc->GetNumPipes(addin, result.State); 
			result.Value = true;
		}
	}
	catch (...){}
	return result;
}

/*向指定插件中心注册事件回调函数并返回事件标识*/
Result<bool, EventToken> AddinCenter_RegisiterEventCallBack(const AddinCenter& center, AddinCenterEventCallBack call)
{
	Result<bool, EventToken> result(false, 0);
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.State = pc->Capturer += call;
			result.Value = true;
		}
	}
	catch (...){}
	return result;
}

/*从指定插件中心移除指定标识事件回调函数*/
Result<bool> AddinCenter_UnRegisiterEventCallBack(const AddinCenter& center, EventToken event)
{
	Result<bool> result(false);
	void* centerptr = center.get(sizeof(PluginCenter));
	if (!centerptr) return result;
	try
	{
		PluginCenter* pc = static_cast<PluginCenter*>(centerptr);
		if (OSHelper::IsValidAddress(pc, sizeof(PluginCenter)))
		{
			result.Value = ((pc->Capturer -= event) == event);
		}
	}
	catch (...){}
	return result;
}

#pragma endregion

#pragma region Player

/*创建播放器*/
Result<bool, AudioPlayer, UInt128> Player_Create(const AddinCenter&, UInt64);

/*销毁播放器*/
void Player_Destory(AudioPlayer&);

/*获取指定播放器缓冲区长度*/
Result<bool, UInt64> Player_GetBufferTimeMs(const AudioPlayer&);

/*设置指定播放器缓冲区长度*/
Result<bool> Player_SetBufferTimeMs(const AudioPlayer&, UInt64);

/*打开指定音频文件并返回音频树标识*/
Result<UInt128, audio_tree_error, CoreSubObjects, int> Player_Open(const AudioPlayer&, const wchar_t*, const UInt128&, const UInt128&, int);

/*获取指定音频树音频信息*/
Result<bool, AudioInfo> Player_GetInfo(const AudioPlayer&, const UInt128&);

/*播放指定音频树*/
Result<bool> Player_Play(const AudioPlayer&, const UInt128&);

/*暂停指定音频树*/
Result<bool> Player_Pause(const AudioPlayer&, const UInt128&);

/*停止指定的音频树并在播放结束事件中指定辨认标识*/
Result<bool> Player_Stop(const AudioPlayer&, const UInt128&, int);

/*获取指定音频树当前播放进度*/
Result<bool, double> Player_GetCurrent(const AudioPlayer&, const UInt128&);

/*设置指定音频树当前播放进度*/
Result<bool> Player_SetCurrent(const AudioPlayer&, const UInt128&, double);

/*获取指定音频树均衡器配置*/
Result<bool, int, int> Player_GetEqConfig(const AudioPlayer& );

/*设置指定音频树均衡器配置*/
Result<bool, int> Player_SetEqConfig(const AudioPlayer&, int, int);

/*获取指定音频树均衡器状态*/
Result<bool, bool> Player_GetEqActive(const AudioPlayer&);

/*设置指定音频树均衡器状态*/
Result<bool>Player_SetEqActive(const AudioPlayer&, bool);

/*获取指定音频树均衡器指定频段增益值*/
Result<bool, double> Player_GetEqGain(const AudioPlayer&, int);

/*设置指定音频树均衡器指定频段增益值*/
Result<bool, double> Player_SetEqGain(const AudioPlayer&, int, double);

/*获取指定音频树音调增益值*/
Result<bool, double> Player_GetPitch(const AudioPlayer&);

/*设置指定音频树音调增益值*/
Result<bool> Player_SetPitch(const AudioPlayer&, double);

/*获取指定音频树节奏增益值*/
Result<bool, double> Player_GetTempo(const AudioPlayer&);

/*设置指定音频树节奏增益值*/
Result<bool> Player_SetTempo(const AudioPlayer&, double);

/*获取指定音频树频率增益值*/
Result<bool, double> Player_GetRate(const AudioPlayer&);

/*设置指定音频树频率增益值*/
Result<bool> Player_SetRate(const AudioPlayer&, double);

/*获取指定音频树音量值*/
Result<bool, double> Player_GetVolume(const AudioPlayer&);

/*设置指定音频树音量值*/
Result<bool> Player_SetVolume(const AudioPlayer&, double);

/*获取指定音频树声道平衡值*/
Result<bool, double> Player_GetPan(const AudioPlayer&);

/*设置指定音频树声道平衡值*/
Result<bool> Player_SetPan(const AudioPlayer&, double);

/*获取指定音频树快速傅里叶变换数据*/
Result<bool, float**> Player_GetFFTData(const AudioPlayer&, UShort, UInt, audio_fft_window, bool, unsigned short&, unsigned int&);

/*获取指定音频树音量单位数据*/
Result<bool, float*> Player_GetVUData(const AudioPlayer&, UShort, UInt, unsigned short&, unsigned int&);



AudioPlayerMgr PlayerMgr=
{
	Player_Create,
	Player_Destory,
	Player_GetBufferTimeMs,
	Player_SetBufferTimeMs,
	Player_Open,
	Player_GetInfo,
	Player_Play,
	Player_Pause,
	Player_Stop,
	Player_GetCurrent,
	Player_SetCurrent,
	Player_GetEqConfig,
	Player_SetEqConfig,
	Player_GetEqActive,
	Player_SetEqActive,
	Player_GetEqGain,
	Player_SetEqGain,
	Player_GetPitch,
	Player_SetPitch,
	Player_GetTempo,
	Player_SetTempo,
	Player_GetRate,
	Player_SetRate,
	Player_GetVolume,
	Player_SetVolume,
	Player_GetPan,
	Player_SetPan,
	Player_GetFFTData,
	Player_GetVUData,
};

/*创建播放器*/
Result<bool, AudioPlayer, UInt128> Player_Create(const AddinCenter& center, UInt64 bufferTimeMs)
{
	Result<bool, AudioPlayer, UInt128> result(false, nullptr, 0U);
	try
	{
		void* centerPtr = center.get(sizeof(PluginCenter));
		if (centerPtr)
		{
			PluginCenter* pc = static_cast<PluginCenter*>(centerPtr);
			if (pc)
			{
				Player* pl=new Player(pc, bufferTimeMs);
				if (pl)
				{
					result.Value1 = true;
					result.Value2 = pl;
					result.State = pl->Id;
				}
			}
		}
	}
	catch (...)
	{
		result.Value1 = false;
		result.Value2 = nullptr;
		result.State = 0U;
	}
	return result;
}

/*销毁播放器*/
void Player_Destory(AudioPlayer& player)
{
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			SAFE_DELETE(p);
			player = nullptr;
		}
	}
	catch (...){}
}

/*获取指定播放器缓冲区长度*/
Result<bool, UInt64> Player_GetBufferTimeMs(const AudioPlayer& player)
{
	Result<bool, UInt64> result(false, 0U);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = true;
				result.State = p->BufferTime;
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定播放器缓冲区长度*/
Result<bool> Player_SetBufferTimeMs(const AudioPlayer& player, UInt64 bufferTimeMs)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->BufferTime = bufferTimeMs;
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*打开指定音频文件并返回音频树标识*/
Result<UInt128, audio_tree_error, CoreSubObjects, int> Player_Open(const AudioPlayer& player, const wchar_t* path, const UInt128& dec, const UInt128& dev, int timeout)
{
	Result<UInt128, audio_tree_error, CoreSubObjects, int> result(0U, audio_tree_error::Unknown, CoreSubObjects::None, 0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				auto tmp = p->Open(path, dec, dev, timeout);
				result.Value1 = tmp.Value1;
				result.Value2 = (audio_tree_error)(size_t)tmp.Value2;
				result.Value3 = tmp.Value3;
				result.State = tmp.State;
			}
		}
	}
	catch (...){}
	return  result;
}

/*获取指定音频树音频信息*/
Result<bool, AudioInfo> Player_GetInfo(const AudioPlayer& player, const UInt128& id)
{
	Result<bool, AudioInfo> result(false, AudioInfo());
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = true;
				result.State = p->GetInfo(id);
			}
		}
	}
	catch (...){}
	return result;
}

/*播放指定音频树*/
Result<bool> Player_Play(const AudioPlayer& player, const UInt128& id)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = p->Play(id);
			}
		}
	}
	catch (...){}
	return result;
}

/*暂停指定音频树*/
Result<bool> Player_Pause(const AudioPlayer& player, const UInt128& id)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = p->Pause(id);
			}
		}
	}
	catch (...){}
	return result;

}

/*停止指定的音频树并在播放结束事件中指定辨认标识*/
Result<bool> Player_Stop(const AudioPlayer& player, const UInt128& id, int msg)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = p->Stop(id, msg);
			}
		}
	}
	catch (...){}
	return result;
}


/*获取指定音频树当前播放进度*/
Result<bool, double> Player_GetCurrent(const AudioPlayer& player, const UInt128& id)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = true;
				result.State = p->GetCurrent(id);
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树当前播放进度*/
Result<bool> Player_SetCurrent(const AudioPlayer& player, const UInt128& id, double pos)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = p->SetCurrent(id, pos);
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树均衡器配置*/
Result<bool, int, int> Player_GetEqConfig(const AudioPlayer& player)
{
	Result<bool, int, int> result(false, 0, 0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				if (p->EqConfig)
				{
					result.Value1 = true;
					result.Value2 = p->EqConfig->Bands;
					result.State = p->EqConfig->Channels;
				}
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树均衡器配置*/
Result<bool, int> Player_SetEqConfig(const AudioPlayer& player, int band, int ch)
{
	Result<bool, int> result(false, 0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.Value = true;
				result.State = p->SetEqConfig(band, ch);
			}
		}
	}
	catch (...){}
	return result;
}


/*获取指定音频树均衡器状态*/
Result<bool, bool> Player_GetEqActive(const AudioPlayer& player)
{
	Result<bool, bool> result(false, false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				if (p->EqConfig)
				{
					result.Value = true;
					result.State = p->EqConfig->Active;
				}
			}
		}
	}
	catch (...){}
	return result;

}

/*设置指定音频树均衡器状态*/
Result<bool>Player_SetEqActive(const AudioPlayer& player, bool active)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->SetEqActive(active);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树均衡器指定频段增益值*/
Result<bool, double> Player_GetEqGain(const AudioPlayer& player, int index)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				if (p->EqConfig)
				{
					if (index < 0)
					{
						result.State = p->EqConfig->Preamp;
						result.Value = true;
					}
					else if (index >= 0 && index < p->EqConfig->Gains.size())
					{
						result.State = p->EqConfig->Gains.at(index);
						result.Value = true;
					}			
				}
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树均衡器指定频段增益值*/
Result<bool, double> Player_SetEqGain(const AudioPlayer& player, int index, double gain)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->SetEqGain(index, gain);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树音调增益值*/
Result<bool, double> Player_GetPitch(const AudioPlayer& player)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->m_Pitch;
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树音调增益值*/
Result<bool> Player_SetPitch(const AudioPlayer& player, double gain)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->SetPitch(gain);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树节奏增益值*/
Result<bool, double> Player_GetTempo(const AudioPlayer& player)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->m_Tempo;
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树节奏增益值*/
Result<bool> Player_SetTempo(const AudioPlayer& player, double gain)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->SetTempo(gain);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树频率增益值*/
Result<bool, double> Player_GetRate(const AudioPlayer& player)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->m_Rate;
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树频率增益值*/
Result<bool> Player_SetRate(const AudioPlayer& player, double gain)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->SetRate(gain);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树音量值*/
Result<bool, double> Player_GetVolume(const AudioPlayer& player)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->m_Volume;
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树音量值*/
Result<bool> Player_SetVolume(const AudioPlayer& player, double gain)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->SetVolume(gain);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树声道平衡值*/
Result<bool, double> Player_GetPan(const AudioPlayer& player)
{
	Result<bool, double> result(false, 0.0);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->m_Pan;
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*设置指定音频树声道平衡值*/
Result<bool> Player_SetPan(const AudioPlayer& player, double gain)
{
	Result<bool> result(false);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				p->SetPan(gain);
				result.Value = true;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树快速傅里叶变换数据*/
Result<bool, float**> Player_GetFFTData(const AudioPlayer& player, UShort dch, UInt dml, audio_fft_window fft, bool mono, unsigned short& ochm, unsigned int& oml)
{
	Result<bool, float**> result(false, nullptr);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->GetFFTData(dch, dml, (UInt)fft, mono, ochm, oml);
				if (result.State != nullptr && ochm >= CHANNELS_MIN && ochm <= CHANNELS_MAX && oml != 0)
					result.Value = true;
				else
					result.State = nullptr;
			}
		}
	}
	catch (...){}
	return result;
}

/*获取指定音频树音量单位数据*/
Result<bool, float*> Player_GetVUData(const AudioPlayer& player, UShort dch, UInt dml, unsigned short& ochm, unsigned int& oml)
{
	Result<bool, float*> result(false, nullptr);
	try
	{
		void* playerPtr = player.get(sizeof(Player));
		if (playerPtr)
		{
			Player* p = static_cast<Player*>(playerPtr);
			if (p)
			{
				result.State = p->GetVUData(dch, dml, ochm, oml);
				if (result.State != nullptr && ochm >= CHANNELS_MIN && ochm <= CHANNELS_MAX && oml != 0) 
					result.Value = true; 
				else 
					result.State = nullptr;
			}
		}
	}
	catch (...){}
	return result;
}

#pragma endregion
/*获取音频播放器管理器接口*/
AudioPlayerMgr* Interface_GetPlayerMgr()
{
	return &PlayerMgr;
}

AudioInterface Interface
{
	Interface_GetCenterMgr,
	Interface_GetPlayerMgr,
};

AUDIO_CAPI  AudioInterface* GetAudioInterface()
{
	return &Interface;
}

