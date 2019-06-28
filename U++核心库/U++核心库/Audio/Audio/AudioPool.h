#ifndef AudioPool_H
#define AudioPool_H
#include "Config.h"
#include <queue>

class ALCache
{
public:
	double TimeMs;
	float SampleRate;
	float* Mem[CHANNELS_MAX];
	UShort Channel;
	UInt Count;
	ALCache* Previous;
	ALCache* Next;
	ALCache(double timeMs, const float* mem, UShort channel, UInt count, ALCache* prev, float sr) : Previous(prev), Next(NULL), SampleRate(sr)
	{
		TimeMs=timeMs;
		Channel=channel;
		Count=count;
		UInt ch=0;
		for(ch=0;ch<CHANNELS_MAX;ch++)
		{
			if(ch<Channel)
			{
				Mem[ch]=(float*)malloc(sizeof(float)* Count);
			}
			else
			{
				Mem[ch]=NULL;
			}
		}
		count*=channel;
		for(UInt i=0, n=0;i<count;i+=channel, n++)
		{
			for(ch=0; ch<channel;ch++)
			{
				Mem[ch][n]=mem[i+ch];
			}
		}
	}
	void ReWrite(double timeMs, const float* mem, UShort channel, UInt count, ALCache* prev, float sr)
	{
		UShort oldchannel=Channel;
		UInt oldcount=Count;
		TimeMs=timeMs;
		Channel=channel;		
		Count=count;
		SampleRate=sr;
		UInt ch=0;
		if(Count>oldcount)
		{
			for(ch=0;ch<CHANNELS_MAX;ch++)
			{
				if(ch<Channel)
				{
					Mem[ch]=(float*)realloc(Mem[ch],sizeof(float)* Count);
				}
				else
				{
					if (Mem[ch]) { free(Mem[ch]); Mem[ch] = NULL; }
				}
			}
		}
		count*=channel;
		for(UInt i=0, n=0;i<count;i+=channel, n++)
		{
			for(ch=0; ch<channel;ch++)
			{
				Mem[ch][n]=mem[i+ch];
			}
		}
	}
	~ALCache()
	{
		for(UInt ch=0;ch<CHANNELS_MAX;ch++)
		{
			if(Mem[ch])
			{
				delete Mem[ch];
				Mem[ch]=NULL;
			}
		}
	}
};

class AudioPool
{
private:
	CriticalSection cs;
	std::deque<ALCache*> mem;
	std::deque<ALCache*> available;
	UInt samples;
	UShort maxchannel;
	ALCache* readcue;
public:

	UShort GetMaxChannel() {return maxchannel;}

	_declspec(property(get= GetMaxChannel)) UShort MaxChannel;

	AudioPool() : samples(0), maxchannel(0), readcue(NULL)
	{
	}

	~AudioPool() 
	{
		readcue=NULL;
		while(!mem.empty())
		{
			ALCache* cache= mem.front();
			if(cache)
			{
				delete cache;
				cache=NULL;
			}
			mem.pop_front();	
			if(!mem.empty())
			{
				cache=mem.front();
				cache->Previous=NULL;
			}
		}
		while(!available.empty())
		{
			ALCache* cache= available.front();
			if(cache)
			{
				delete cache;
				cache=NULL;
			}
			available.pop_front();	
			if(!available.empty())
			{
				cache=available.front();
				cache->Previous=NULL;
			}
		}
	}

	void Flush()
	{
		cs.Enter();
		while(!mem.empty())
		{
			ALCache* cache= mem.front();
			if(cache)
			{
				delete cache;
				cache=NULL;
			}
			mem.pop_front();	
			if(!mem.empty())
			{
				cache=mem.front();
				cache->Previous=NULL;
			}
		}
		while(!available.empty())
		{
			ALCache* cache= available.front();
			if(cache)
			{
				delete cache;
				cache=NULL;
			}
			available.pop_front();	
			if(!available.empty())
			{
				cache=available.front();
				cache->Previous=NULL;
			}
		}
		samples=0;
		maxchannel=0;
		cs.Leave();
	}

	void CollectBins(double timeMs)
	{
		cs.Enter();
		while(!mem.empty())
		{
			ALCache* cache= mem.front();
			if(!cache) break;
			if(cache->TimeMs<timeMs)
			{
				samples-=cache->Count;

				if(cache->Previous) cache->Previous->Next=NULL;
				if(cache->Next) cache->Next->Previous=NULL;
				cache->Next=NULL;
				cache->Previous=NULL;
				available.push_back(cache);
				mem.pop_front();	
				if(!mem.empty())
				{
					cache=mem.front();
					cache->Previous=NULL;
				}
			}
			else
			{
				break;
			}
		}
		if(mem.empty()) maxchannel=0;
		cs.Leave();
	}

	void Write(AudioBuffer* buffer)
	{
		if(!buffer) return;
		auto res = buffer->EnsureFloatPool();
		if(res.Value)
		{
			UInt bytes=buffer->FloatPoolBytes;
			UShort chs=buffer->Channels;
			if(chs>maxchannel) maxchannel=chs;
			UInt count=bytes/sizeof(float)/chs;
			UInt ch=0;
			UInt done=0;
			UInt size=count;
			const float* floatmem=buffer->GetFloatPool();
			cs.Enter();
			while(count)
			{
				size=count;
				if(size>512) size=512;
				ALCache* end= mem.empty()? NULL: mem.back();
				ALCache* next=NULL;
				if(available.empty())
					next=new ALCache(buffer->StartTimeMs, floatmem+done, chs, size, end, buffer->SampleRate);
				else
				{
					bool fromavailable=false;
					while(!available.empty())
					{
						next=available.front();
						fromavailable=(next!=NULL);
						available.pop_front();
						if(fromavailable)
						{
							next->ReWrite(buffer->StartTimeMs, floatmem+done, chs, size, end, buffer->SampleRate);
							break;
						}
					}
					if(!fromavailable)
					{
						next=new ALCache(buffer->StartTimeMs, floatmem+done, chs, size, end, buffer->SampleRate);
					}
				}
				mem.push_back(next);
				if(end) end->Next=next;
				
				count-=size;
				done+=size;
			}
			cs.Leave();
		}
	}

	UInt Read(float** dest, UShort channel, UInt count, double timeMs, float* maxSampleRate)
	{
		if(!dest) return 0;
		cs.Enter();
		UInt done=0;
		UInt size=count;
		UInt ch=0;
		bool forward= readcue!=NULL ? readcue->TimeMs<timeMs : true;
		ALCache* cache= mem.empty()? NULL:  ( readcue!=NULL ? readcue  : mem.front());
		if(maxSampleRate) *maxSampleRate=0.0f;
		if(forward)
		{
			while(cache && count> done)
			{
				if(cache->TimeMs<timeMs)
				{
					cache= cache->Next;
				}
				else
				{
					if(size>cache->Count) size=cache->Count;
					for(ch=0;ch<cache->Channel;ch++)
					{
						if(ch<channel)
						{
							memcpy(dest[ch]+done, cache->Mem[ch], sizeof(float)* size);
						}
					}
					if(maxSampleRate)
						if(cache->SampleRate> *maxSampleRate) 
							*maxSampleRate=cache->SampleRate;
					done+=size;
				}
			}
		}
		else
		{
			while(cache && count> done)
			{
				if(cache->TimeMs<timeMs)
				{
					cache= cache->Previous;
				}
				else
				{
					if(size>cache->Count) size=cache->Count;
					for(ch=0;ch<cache->Channel;ch++)
					{
						if(ch<channel)
						{
							for(UInt i=0;i< size;i++)
							{
								memcpy(dest[ch]+done, cache->Mem[ch], sizeof(float)* size);
							}
						}
					}
					if(maxSampleRate)
						if(cache->SampleRate> *maxSampleRate) 
							*maxSampleRate=cache->SampleRate;					
					done+=size;
				}
			}
		}
		cs.Leave();
		return done;
	}
};


#endif