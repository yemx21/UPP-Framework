#include "AudioTree.h"
#include "Player.h"
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;

#define BUFFER_SIZE 512u

AudioTree::AudioTree(Player* PlayerHost, const UInt128& D, const UInt128& R, PluginCenter* Plugins, DSPManager* DSPMgr, AudioAnalyzer* StdSA, UInt64 BufferTime) :DecodeHandle(INVALID_API_PTR), Active(false), m_StopParam(0), Decoder(0), Device(nullptr), Decoder_ptr(0), Device_ptr(0), DSPMgr_ptr(0), Visual(0), Visual_ptr(0), IsThreadActive(false), Buffer(nullptr), KillDecodeThread(0), IsPlaying(false), SeekNeeded(-1), RenderReSample(false), Operation_Path(nullptr), RenderBufferTime(0), BackResampleRefresh(false)
{
	_id = Plugins->GenerateHashId128();
	Host=PlayerHost;
	DecoderID=D;
	DeviceID=R;
	VisualID = 0U;
	Center=Plugins;
	m_BufferTime=BufferTime;
	m_StdSA=StdSA;
	m_DSPMg=DSPMgr;
	DSPMgr_ptr=m_DSPMg->CreateTree();
	StdTimeEQ=new AudioTimeEQ();
	StdDSP=new AudioSTDDSP();
	StdLimiter=new AudioLimiter();
	BackResampler = new AudioResampler();
	RenderResampler = new AudioResampler();
	Operation_Decoder = Microsoft::winapi_CreateEvent(NULL, true, false, NULL);
	Operation_Device = Microsoft::winapi_CreateEvent(NULL, true, false, NULL);
}
AudioTree::~AudioTree()
{
	Center->DestoryHashId128(_id);
	if (StdDSP) { StdDSP->Clear();  delete StdDSP; StdDSP = nullptr; }
	if (StdTimeEQ){ delete StdTimeEQ; StdTimeEQ = nullptr; }
	if (StdLimiter){ delete StdLimiter; StdLimiter = nullptr; }
	if (Buffer){ delete Buffer; Buffer = nullptr; }
	if (Operation_Path) { delete Operation_Path; Operation_Path = nullptr; }
	if (BackResampler) { delete BackResampler; BackResampler = nullptr; }
	if (RenderResampler) { delete RenderResampler; RenderResampler = nullptr; }
	if (Operation_Decoder) 
	{
		Microsoft::winapi_CloseHandle(*Operation_Decoder);
		Operation_Decoder = NULL;
	}

	if (Operation_Device)
	{
		Microsoft::winapi_CloseHandle(*Operation_Device);
		Operation_Device = NULL;
	}
	if (*DecodeHandle != INVALID_API_PTR) 
	{
		KillDecodeThread = 1;		
		if (IsThreadActive)
		{
			if (Microsoft::winapi_WaitForSingleObject(*DecodeHandle, API_INFINITE) == API_WAIT_TIMEOUT)
				Microsoft::winapi_TerminateThread(*DecodeHandle, 0);
			Microsoft::winapi_CloseHandle(*DecodeHandle);
			DecodeHandle = INVALID_API_PTR;
		}
		else
		{ 
			Microsoft::winapi_TerminateThread(*DecodeHandle, 0);
			Microsoft::winapi_CloseHandle(*DecodeHandle);
			DecodeHandle = INVALID_API_PTR;
		}
	}
	{
		std::lock_guard<std::mutex> lock(Disposing_Pipes);
		if (Device && Device_ptr)
		{
			if (!Device->Stop(Device_ptr))  Center->Post(CoreEventFlags::Error, CoreEvents::MemoryError, CoreObjects::Player, CoreSubObjects::AudioDevice, Host->Id, L"device instance memory leak", (VariadicAny(DeviceID), Device_ptr));
			Device_ptr = 0;
		}

		if (Decoder && Decoder_ptr)
		{
			if (!Decoder->TryClose(Decoder_ptr)) Center->Post(CoreEventFlags::Error, CoreEvents::MemoryError, CoreObjects::Player, CoreSubObjects::AudioDecoder, Host->Id, L"decoder instance memory leak", (VariadicAny(DecoderID), Decoder_ptr));
			Decoder_ptr = 0;
		}
	}
	m_DSPMg->DestoryTree(DSPMgr_ptr);
	DSPMgr_ptr = 0;
}


bool AudioTree::SetReSampleConfig(const ResampleConfig& reSampleSetting)
{
	if (reSampleSetting.EnableSampleRate)
		if (reSampleSetting.SampleRate< SAMPLERATE_MIN || reSampleSetting.SampleRate> SAMPLERATE_MAX) return false;
	if (reSampleSetting.EnableChannels)
		if (!reSampleSetting.ChannelLayout.IsValid)  return false;
	if (reSampleSetting.EnableBitsPerSample)
		if (!reSampleSetting.BitsPerSample.IsValid) return false;

	if (BackResampleSetting != reSampleSetting)
	{
		BackResampleSetting = reSampleSetting;
		BackResampleRefresh = true;
	}
	return true;
}

Result<bool, AudioTreeError, CoreSubObjects, int> AudioTree::Open(const wchar_t* Path, int Timeout)
{
	Operation_OpenResult = Result<bool, AudioTreeError, CoreSubObjects, int>(false, AudioTreeError::Unknown, CoreSubObjects::None, 0);

	Operation_Path = Cloner::Clone(Path);

	if (!Operation_Path)
	{
		Operation_OpenResult.Value2 = AudioTreeError::InvalidPath;
		goto DIRECT_RETURN;
	}
	if(!Center)
	{
		Operation_OpenResult.Value2 = AudioTreeError::InvalidCenter;
		goto DIRECT_RETURN;
	}
	if(IsThreadActive) 
	{
		Operation_OpenResult.Value2 = AudioTreeError::AlreadyOnThread;
		goto DIRECT_RETURN;
	}

	if(!DecoderID.IsZero && !DeviceID.IsZero)
	{
		Decoder = (AudioDecoder_base*)Center->GetAudioDecoder(DecoderID);	
		Device = (AudioDevice_base*)Center->GetAudioDevice(DeviceID);
		if (!Decoder || !Device)
		{
			Operation_OpenResult.Value2 = AudioTreeError::Invalid;
			Operation_OpenResult.WriteMessage(L"empty Device ID or Decoder ID ");
			return Operation_OpenResult;
		}
		DecodeHandle = (HANDLE)_beginthreadex(NULL, 0, &DecodeThread, this, API_CREATE_SUSPENDED, 0);
		Microsoft::winapi_SetThreadPriority(*DecodeHandle, API_THREAD_PRIORITY_HIGHEST);
		if (Microsoft::winapi_ResumeThread(*DecodeHandle) == -1)
		{
			Microsoft::winapi_TerminateThread(*DecodeHandle, 0);
			Operation_OpenResult.Value2 = AudioTreeError::Failed;
			Operation_OpenResult.WriteMessage(L"thread internal error");
			goto DIRECT_RETURN;
		}
		if (Microsoft::winapi_WaitForSingleObject(*Operation_Decoder, API_INFINITE) == API_WAIT_OBJECT_0)
		{
			if (Decoder_ptr)
			{
				if (Microsoft::winapi_WaitForSingleObject(*Operation_Device, API_INFINITE) == API_WAIT_OBJECT_0)
				{
					if (Device_ptr)
					{
						Operation_OpenResult.Value1 = true;
						Operation_OpenResult.Value2 = AudioTreeError::None;
						goto DIRECT_RETURN;
					}
					else
						goto TERMINATE;
				}
				else
					goto TERMINATE;
			}
			else
				goto TERMINATE;
		}
		else
			goto TERMINATE;
	}
	else
	{
		Operation_OpenResult.Value2 = AudioTreeError::Invalid;
		Operation_OpenResult.WriteMessage(L"empty Device ID or Decoder ID ");
		return Operation_OpenResult;
	}

DIRECT_RETURN:
	return Operation_OpenResult;
TERMINATE:	
	KillDecodeThread = 1;


	return Operation_OpenResult;
}

Result<bool, AudioTreeError, CoreSubObjects, int> AudioTree::BindVisual(const UInt128& V, safe_void_ptr Owner)
{
	Result<bool, AudioTreeError, CoreSubObjects, int> error(false, AudioTreeError::Unknown, CoreSubObjects::AudioVisual, 0);
	UInt128 oldV=VisualID;
	if(!V.IsZero)
	{		
		VisualID=V;
		Visual=(AudioVisual_base*)Center->GetAudioVisual(VisualID);
		if(Visual_ptr)
		{
			if(!Visual->Destory(Visual_ptr))
			{
				Center->Post(CoreEventFlags::Error, CoreEvents::Exception, CoreObjects::Player, CoreSubObjects::AudioVisual, Host->Id, L"Error release resource", (VariadicAny(Id), oldV, Visual_ptr));
			}
		}
		else
		{
			if(Visual) 
			{
				Result<AudioVisualPipe> result= Visual->Create();
				if(result.Value)
					Visual_ptr = result.Value;
				else
				{
					Visual_ptr = 0;
					error.Value2=AudioTreeError::Unsupported;
					error.WriteMessage(result.Message);
					return error;
				}
			}
			else
			{
				error.Value2 = AudioTreeError::Invalid;
			}
		}
	}
	else
	{
		if (Visual_ptr)
			if(!Visual->Destory(Visual_ptr))
			{
				Center->Post(CoreEventFlags::Error, CoreEvents::Exception, CoreObjects::Player, CoreSubObjects::AudioVisual, Host->Id, L"Error release resource", (VariadicAny(Id), oldV, Visual_ptr));
				error.Value2 = AudioTreeError::Failed;
			}
			else
			{
				error.Value1=true;
				error.Value2=AudioTreeError::None;
				return error;
			}
	}
	if (Visual_ptr)
	{
		error.Value1 = true;
		error.Value2 = AudioTreeError::None;
	}
	return error;
}

bool AudioTree::SetCurrent(double PosMS)
{
	if(!Center)return false;
	if(!DecoderID.IsZero)
	{
		if(PosMS<0) return false;
		if(PosMS>Info.TimeMs)return false;
		SeekNeeded=PosMS;
		return true;
	}
	else
		return false;
}

unsigned int _stdcall AudioTree::DecodeThread(void* Obj)
{
	AudioTree* Tree=(AudioTree*)Obj;
	
	Result<AudioDecoderPipe, AudioDecoderError> result = Tree->Decoder->OpenLocal(Tree->Operation_Path);
	if (result.Value && result.State == AudioDecoderError::OK)
	{
		Tree->Decoder_ptr = result.Value;
		Microsoft::winapi_SetEvent(*Tree->Operation_Decoder);
		if (Tree->Decoder->TryGetPipeInfo(Tree->Decoder_ptr, Tree->Info))
		{
			if (Tree->Buffer)
				Tree->Buffer->Reset(Tree->Info.SampleRate, Tree->Info.BitsPerSample, Tree->Info.ChannelLayout);
			else
				Tree->Buffer = new AudioBuffer(Tree->Info.SampleRate, Tree->Info.BitsPerSample, Tree->Info.ChannelLayout);
		}
		else
		{
			Tree->Operation_OpenResult.Value2 = AudioTreeError::Unsupported;
			Tree->Operation_OpenResult.Value3 = CoreSubObjects::AudioDecoder;
			Tree->Operation_OpenResult.WriteMessage(L"can not get info of this audio");
			_endthreadex(10);
			return 10;
		}
	}
	else
	{
		Tree->Operation_OpenResult.Value2 = AudioTreeError::Unsupported;
		Tree->Operation_OpenResult.Value3 = CoreSubObjects::AudioDecoder;
		Tree->Operation_OpenResult.State = (int)result.State;
		Tree->Operation_OpenResult.WriteMessage(result.Message);
		Microsoft::winapi_SetEvent(*Tree->Operation_Decoder);
		_endthreadex(11);
		return 11;
	}
	Tree->IsThreadActive=true;
	double temp=0.0;
	bool DecodeEnd=false;

	while(Tree->KillDecodeThread==0)
	{
		if(Tree->SeekNeeded!=-1)
		{
			if(Tree->SeekNeeded<=Tree->Info.TimeMs)
			{
				if(Tree->Decoder->TrySeek(Tree->Decoder_ptr,Tree->SeekNeeded))
				{
					temp=Tree->Decoder->GetCurrentMS(Tree->Decoder_ptr);
					Tree->Device->Flush(Tree->Device_ptr,temp);
					Tree ->m_StdSA->Flush();
					Tree->BackResampler->Flush();
					Tree->RenderResampler->Flush();
					Tree->StdLimiter->Flush();
					Tree->Center->Post(CoreEventFlags::Info, CoreEvents::SeekCompleted, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", (VariadicAny(Tree->Id), temp)); 
					Tree->SeekNeeded=-1;
				}
			}
			else
				Tree->SeekNeeded=-1;
		}
		else if(DecodeEnd)
		{
			if (!Tree->Device->Drain(Tree->Device_ptr)) 
			{
				Tree->Center->Post(CoreEventFlags::Info, CoreEvents::PlayCompleted, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
				Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
				Tree->IsThreadActive=false;
				_endthreadex(0);
				return 0;
			}
		}
		else
		{
			int LastDecodeMS=Tree->Decoder->GetCurrentMS(Tree->Decoder_ptr);
			Result<UInt64, AudioDecoderError> result = Tree->Decoder->Read(Tree->Decoder_ptr, *Tree->Buffer, BUFFER_SIZE);
			Tree->Buffer->startms=LastDecodeMS;
			size_t NumSamples=0u;
			if (result.State == AudioDecoderError::OK)
			{
				NumSamples=Tree->Buffer->SampleNum;
			}
			else
			{
				DecodeEnd = result.State == AudioDecoderError::End;
				if (!DecodeEnd)  Tree->Center->Post(CoreEventFlags::Info, CoreEvents::Exception, CoreObjects::Player, CoreSubObjects::AudioDecoder, Tree->Host->Id, L"", (VariadicAny(Tree->Id), Tree->DecoderID, Tree->Decoder_ptr));
			}
			if(NumSamples)
			{
				#pragma region 效果处理
				Tree->StdDSP->SampleDSP(Tree->Buffer);
				Tree->m_DSPMg->DoDSP(Tree->DSPMgr_ptr, *Tree->Buffer,false);
				Tree->StdTimeEQ->DoEQ(Tree->Buffer);
				Tree->StdLimiter->Process(Tree->Buffer);
				Tree->m_StdSA->Write(Tree->Buffer);
				#pragma endregion			
			}
			bool Cross=false;
			///自动切换
			if (Cross) Tree->Center->Post(CoreEventFlags::Info, CoreEvents::PlayAutoChangeStart, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
			if(Tree->Buffer->SampleNum)
			{
				size_t bytes=0;
				if (Tree->BackResampleRefresh)
				{
					Tree->BackResampler->Config(Tree->BackResampleSetting);
					Tree->BackResampleRefresh = false;
				}
				bool direct = false;
				const void* mem = Tree->BackResampler->Write(Tree->Buffer, bytes, direct, Tree->CurrentInfo);
				bool moredata1 = false;
				if (!direct)
				{
					mem = Tree->BackResampler->Read(bytes, Tree->CurrentInfo, moredata1);
				}
				if (moredata1)
				{
					Tree->Buffer->Clear();
					continue;
				}
				if (Tree->CurrentInfo.IsValid)
				{
					if (Tree->CurrentInfo != Tree->LastInfo)
					{
						if (Tree->Device)
						{
							if (Tree->Device_ptr)
							{
								if (Tree->Device->Stop(Tree->Device_ptr))
								{
									Tree->Device_ptr = 0;
									Result<AudioDecoderPipe, AudioDeviceError> result = Tree->Device->Open(Tree->Id, Tree->m_BufferTime, Tree->CurrentInfo, Tree->RenderInfo, Tree->RenderBufferTime);
									if (result.Value && result.State == AudioDeviceError::OK)
										Tree->Device_ptr = result.Value;
									else
									{
										Tree->Device_ptr = 0;
										Tree->Center->Post(CoreEventFlags::Error, CoreEvents::Exception, CoreObjects::Player, CoreSubObjects::AudioDevice, Tree->Host->Id, L"Error audio info", (VariadicAny(Tree->Id), Tree->DeviceID, Tree->Device_ptr));
										Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
										Tree->IsThreadActive = false;
										Microsoft::winapi_SetEvent(*Tree->Operation_Device);
										_endthreadex(12);
										return 12;
									}
								}
							}
							else
							{
								Result<AudioDecoderPipe, AudioDeviceError > result = Tree->Device->Open(Tree->Id, Tree->m_BufferTime, Tree->CurrentInfo, Tree->RenderInfo, Tree->RenderBufferTime);
								if (result.Value && result.State == AudioDeviceError::OK)
								{
									if (Tree->RenderInfo != Tree->CurrentInfo)
									{
										Tree->RenderReSample = true;
										if (Tree->RenderInfo.SampleRate != Tree->CurrentInfo.SampleRate)
										{
											Tree->RenderReSampleSetting.EnableSampleRate = true;
											Tree->RenderReSampleSetting.SampleRate = Tree->RenderInfo.SampleRate;
										}

										if (Tree->RenderInfo.BitsPerSample != Tree->CurrentInfo.BitsPerSample)
										{
											Tree->RenderReSampleSetting.EnableBitsPerSample = true;
											Tree->RenderReSampleSetting.BitsPerSample = Tree->RenderInfo.BitsPerSample;
										}

										if (Tree->RenderInfo.ChannelLayout != Tree->CurrentInfo.ChannelLayout)
										{
											Tree->RenderReSampleSetting.EnableChannels = true;
											Tree->RenderReSampleSetting.ChannelLayout = Tree->RenderInfo.ChannelLayout;
										}
										Tree->RenderResampler->unsafe_SetInputInfo(Tree->CurrentInfo);
										Tree->RenderResampler->Config(Tree->RenderReSampleSetting);
									}
									Tree->Device_ptr = result.Value;
									Microsoft::winapi_SetEvent(*Tree->Operation_Device);
								}
								else
								{
									Tree->Device_ptr = 0;
									Tree->Operation_OpenResult.Value2 = AudioTreeError::Unsupported;
									Tree->Operation_OpenResult.Value3 = CoreSubObjects::AudioDevice;
									Tree->Operation_OpenResult.State = (int)result.State;
									Tree->Operation_OpenResult.WriteMessage(result.Message);
									Tree->Center->Post(CoreEventFlags::Error, CoreEvents::Exception, CoreObjects::Player, CoreSubObjects::AudioDevice, Tree->Host->Id, L"Error audio info", (VariadicAny(Tree->Id), Tree->DeviceID, Tree->Device_ptr));
									Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
									Tree->IsThreadActive = false;
									Microsoft::winapi_SetEvent(*Tree->Operation_Device);
									_endthreadex(12);
									return 12;
								}
							}
						}
						Tree->LastInfo = Tree->CurrentInfo;
					}
				}

				if(Tree->Device && Tree->Device_ptr)
				{
					if (Tree->RenderReSample && bytes && mem)
					{
						bool renderdirect = false;
						mem = Tree->RenderResampler->Write(mem, bytes, Tree->CurrentInfo, bytes, renderdirect, Tree->BeforeRenderInfo);
						bool needmore2 = false;
						if (!renderdirect)
						{
							mem = Tree->RenderResampler->Read(bytes, Tree->BeforeRenderInfo, needmore2);
						}
						if (needmore2)
						{
							Tree->Buffer->Clear();   
							continue;
						}
						if (Tree->BeforeRenderInfo != Tree->RenderInfo)
						{
							Tree->Buffer->Clear();
							if (Tree->Device)
							{
								if (Tree->Device_ptr)
								{
									if (Tree->Device->Stop(Tree->Device_ptr))
									{
										Tree->Device_ptr = 0;
										Tree->Center->Post(CoreEventFlags::Error, CoreEvents::Exception, CoreObjects::Player, CoreSubObjects::AudioDevice, Tree->Host->Id, L"unsupported sample conversion", (VariadicAny(Tree->Id), Tree->DeviceID, Tree->Device_ptr));
										Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
										Tree->IsThreadActive = false;
										_endthreadex(20);
										return 20;
									}
								}
							}
						}
					}
					bool nonewrite=false;
					if(mem && bytes)
					{
						UInt maxstep = (Tree->RenderReSample ? Tree->RenderInfo.BlockAlign:  Tree->CurrentInfo.BlockAlign)* BUFFER_SIZE;
						UInt step=bytes;
						UInt left=bytes;
						UInt done=0;
						while(left>0)
						{
							step=left;
							if(step>maxstep) step=maxstep;
							if(Tree->SeekNeeded!=-1)
							{
								if(Tree->SeekNeeded<=Tree->Info.TimeMs)
								{
									if(Tree->Decoder->TrySeek(Tree->Decoder_ptr,Tree->SeekNeeded))
									{
										temp=Tree->Decoder->GetCurrentMS(Tree->Decoder_ptr);
										Tree->Device->Flush(Tree->Device_ptr,temp);
										Tree ->m_StdSA->Flush();
										Tree->StdLimiter->Flush();
										Tree->BackResampler->Flush();
										Tree->RenderResampler->Flush();
										Tree->Center->Post(CoreEventFlags::Info, CoreEvents::SeekCompleted, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", (VariadicAny(Tree->Id), temp));
										Tree->SeekNeeded=-1;
										nonewrite=true;
										break;
									}
								}
								else
									Tree->SeekNeeded=-1;
							}
							if(Tree->KillDecodeThread==1)
							{ 
								Tree->Center->Post(CoreEventFlags::Info, CoreEvents::PlayCompleted, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", (VariadicAny(Tree->Id), Tree->m_StopParam));
								Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));								
								Tree->IsThreadActive = false;
								_endthreadex(2);
								return 2;
							}
							else if(Tree->KillDecodeThread==2)
							{
								Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
								Tree->IsThreadActive = false;
								_endthreadex(3);
								return 3;
							}
							bool secbreak=false;
							bool wait=false;
							while (Tree->Device->CanWrite(Tree->Device_ptr) < step)
							{
								Microsoft::winapi_Sleep(10);
								wait=true;
								if(Tree->SeekNeeded!=-1)
								{
									if(Tree->SeekNeeded<=Tree->Info.TimeMs)
									{
										if(Tree->Decoder->TrySeek(Tree->Decoder_ptr,Tree->SeekNeeded))
										{
											temp=Tree->Decoder->GetCurrentMS(Tree->Decoder_ptr);
											Tree->Device->Flush(Tree->Device_ptr,temp);
											Tree ->m_StdSA->Flush();
											Tree->StdLimiter->Flush();
											Tree->BackResampler->Flush();
											Tree->RenderResampler->Flush();
											Tree->Center->Post(CoreEventFlags::Info, CoreEvents::SeekCompleted, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", (VariadicAny(Tree->Id), temp));
											Tree->SeekNeeded = -1;
											nonewrite=true;
											secbreak=true;
											break;
										}
									}
									else
										Tree->SeekNeeded=-1;
								}
								if(Tree->KillDecodeThread==1)
								{ 
									Tree->Center->Post(CoreEventFlags::Info, CoreEvents::PlayCompleted, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", (VariadicAny(Tree->Id), Tree->m_StopParam));
									Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
									Tree->IsThreadActive = false;
									_endthreadex(2);
									return 2;
								}
								else if(Tree->KillDecodeThread==2)
								{
									Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
									Tree->IsThreadActive=false;
									_endthreadex(3);
									return 3;
								}
							}

							UInt64 actualstep = 0ull;
							if (!nonewrite)
							{
								auto wres = Tree->Device->Write(Tree->Device_ptr, (char*)mem + done, step);
								if (wres.Value) actualstep = wres.State;
							}
							if (wait)
							{
								auto tt = Tree->Device->GetWrittenTime(Tree->Device_ptr);
								if (tt.Value)
									Tree->m_StdSA->CollectBins(tt.State - Tree->m_BufferTime);
								else
									Tree->m_StdSA->Flush();
							}
							done += actualstep;
							left -= actualstep;
							
							if(secbreak) break;
						}
					}
					else 
						Microsoft::winapi_Sleep(2);
				}
				if(Tree->Buffer) Tree->Buffer->Clear();
			}
		}
	}
	if (Tree->KillDecodeThread == 1)  Tree->Center->Post(CoreEventFlags::Info, CoreEvents::ThreadExit, CoreObjects::Player, CoreSubObjects::None, Tree->Host->Id, L"", VariadicAny(Tree->Id));
	Tree->IsThreadActive=false;
	_endthreadex(0); 
	return 0;
}
bool AudioTree::Play()
{
	if (!Center)return false;
	std::lock_guard<std::mutex> lock(Disposing_Pipes);
	if(Device && Decoder)
	{
		if (Device_ptr && Decoder_ptr && CurrentInfo.IsValid)
		{
			return Device->Play(Device_ptr);
		}
	}
	return false;
}
bool AudioTree::Pause()
{
	if(!Center)return false;
	std::lock_guard<std::mutex> lock(Disposing_Pipes);
	if(Device && Device_ptr) return Device->Pause(Device_ptr); 
	return false;
}
bool AudioTree::Stop(int Message)
{
	if(!Center)return false;
	if (Message!=-1 && *DecodeHandle != INVALID_API_PTR) 
	{
		m_StopParam=Message;
		KillDecodeThread = 1;
		if (IsThreadActive)
		{
			if (Microsoft::winapi_WaitForSingleObject(*DecodeHandle, API_INFINITE) == API_WAIT_TIMEOUT)
			{
				Microsoft::winapi_TerminateThread(*DecodeHandle, 0);
			}
			if (!Microsoft::winapi_CloseHandle(*DecodeHandle))
				Microsoft::winapi_TerminateThread(*DecodeHandle, 0);
			DecodeHandle = INVALID_API_PTR;
		}
		else
		{
			Microsoft::winapi_TerminateThread(*DecodeHandle, 0);
			Microsoft::winapi_CloseHandle(*DecodeHandle);
			DecodeHandle = INVALID_API_PTR;
		}
	}

	{
		std::lock_guard<std::mutex> lock(Disposing_Pipes);
		if (Device && Device_ptr)
		{
			if (!Device->Stop(Device_ptr)) return false;
			Device_ptr = 0;
		}
		if (Decoder && Decoder_ptr)
		{
			if (Decoder->TryClose(Decoder_ptr))
			{
				Decoder_ptr = 0;
				return true;
			}
		}
	}
	return false;
}
double AudioTree::GetCurrent()
{
	if(!Center)return 0;
	std::lock_guard<std::mutex> lock(Disposing_Pipes);
	if(Decoder&&Device)
	{
		if (SeekNeeded != -1) return SeekNeeded;
		double c1 = Decoder->GetCurrentMS(Decoder_ptr);
		auto c2r = Device->GetOutputTime(Device_ptr);
		auto c3r = Device->GetWrittenTime(Device_ptr);
		if (c2r.Value && c3r.Value)
		{
			if (c2r.State<m_BufferTime && c3r.State<c1) return c2r.State;
			double c = c2r.State == 0.0 ? 0.0 : c1 + c2r.State - c3r.State;
			if (c<0)c = 0;
			if (c>Info.TimeMs)c = Info.TimeMs;
			return c;
		}
	}
	return 0;
}

void AudioTree::SetVolume(double Volume)
{
	if(!Center)return; 
	std::lock_guard<std::mutex> lock(Disposing_Pipes);
	if (Device) Device->SetVolume(Device_ptr, CLAMP<double>(Volume, 0.0, 1.0));
}

void AudioTree::SetPan(double Pan)
{
	if(!Center)return;
	std::lock_guard<std::mutex> lock(Disposing_Pipes);
	if(Device) Device->SetPan(Device_ptr, CLAMP<double>(Pan, -1.0, 1.0));
}

