#include "DSPManager.h"
#include "PluginCenter.h"
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;

DSPManager::DSPManager(PluginCenter* pc)
{
	if(!pc)throw 1;
	Center=pc;
	hMutex = Microsoft::winapi_CreateMutex(NULL, false, NULL);
}
DSPManager::~DSPManager()
{
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	Microsoft::winapi_ReleaseMutex(*hMutex);
	Microsoft::winapi_CloseHandle(*hMutex);
	hMutex = nullptr;
}

intptr_t DSPManager::CreateTree()
{
	DSPTree* tree=new DSPTree();
	for(vector<DSPOrder>::iterator iter=DSPs.begin();iter!=DSPs.end();++iter)
	{
		DSPBlock block;
		block.ID=iter->ID;
		AudioDSP_base* dsp = Center->GetAudioDSP(block.ID);
		if(dsp)
		{
			block.Handle=dsp->Create();
		}
		tree->Block.push_back(block);
	}
	return (intptr_t)tree;
}

bool DSPManager::MoveDSP(int OldIndex, int NewIndex, int Mode)
{
	if(OldIndex==NewIndex)return true;
	bool result=false;
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	int count=DSPs.size();
	if(OldIndex<count && NewIndex<count)
	{
		if(Mode==0)
		{
			if(abs(OldIndex-NewIndex)==1)
				swap(DSPs[OldIndex],DSPs[NewIndex]);
			else
			{
				if(OldIndex>NewIndex)
				{
					DSPOrder temp=DSPs[OldIndex];
					for(int i=	OldIndex;i>NewIndex+1;i--)
					{
						DSPs[i]=DSPs[i-1];
					}
					DSPs[NewIndex]=temp;
				}
				else
				{
					DSPOrder temp=DSPs[OldIndex];
					for(int i=	OldIndex;i<NewIndex-1;i++)
					{
						DSPs[i]=DSPs[i+1];
					}
					DSPs[NewIndex]=temp;
				}
			}
		}
		else if(Mode==1)
		{
			swap(DSPs[OldIndex],DSPs[NewIndex]);
			result=true;
		}
	}
	Microsoft::winapi_ReleaseMutex(*hMutex);
	return result;
}

bool DSPManager::SetDSPActive(const UInt128& ID, bool Active)
{
	bool result=false;
	Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
	vector<DSPOrder>::iterator iter=find_if(DSPs.begin(), DSPs.end(),DSPOrder_ID_Equ(ID));
	if(iter!=DSPs.end())
	{
		iter->Active=Active;
		result=true;
	}
	Microsoft::winapi_ReleaseMutex(*hMutex);
	return result;
}

void DSPManager::DoDSP(intptr_t Handle, AudioBuffer& Buffer, bool ignorewinamp)
{
	try
	{
		if(!Handle) return;
		DSPTree* tree=(DSPTree*)Handle;
		if(!tree)return;
		Microsoft::winapi_WaitForSingleObject(*hMutex, API_INFINITE);
		int dspcount=DSPs.size();
		for(int i=0;i<dspcount;i++)
		{
			DSPOrder order=DSPs[i];
			if(order.Active)
			{
				AudioDSP_base* dsp = Center->GetAudioDSP(DSPs[i].ID);
				if(dsp)
				{
					vector<DSPBlock>::iterator block=find_if(tree->Block.begin(), tree->Block.end(),DSPBlock_ID_Equ(order.ID));
					if(block!=tree->Block.end())
					{
						if(block->Handle)
						{
							dsp->Run(block->Handle, Buffer);				
						}
					}
				}
			}
		}
		Microsoft::winapi_ReleaseMutex(*hMutex);
	}
	catch(...)
	{
		Buffer.Clear();
		Microsoft::winapi_ReleaseMutex(*hMutex);
	}
}

void DSPManager::DestoryTree(intptr_t Handle)
{
	if(!Handle)return;
	DSPTree* tree=(DSPTree*)Handle;
	if(!tree)return;
	for(vector<DSPBlock>::iterator iter=tree->Block.begin();iter!=tree->Block.end();++iter)
	{
		if(iter->Handle)
		{
			AudioDSP_base* dsp = (AudioDSP_base*)Center->GetAudioDSP(iter->ID);
			if(dsp)
			{
				if (dsp->Destory(iter->Handle))
					iter->Handle = 0;
				else
					Center->Post(CoreEventFlags::Error, CoreEvents::MemoryError, CoreObjects::None, CoreSubObjects::AudioDSP, iter->ID, L"DSPTree Object Memory Leak", (VariadicAny(Handle), iter->Handle));
			}
		}
	}
	delete tree;
	tree = nullptr;
}