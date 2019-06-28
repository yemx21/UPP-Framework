#define _CRTDBG_MAP_ALLOC

#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <filesystem>
#include <System\Audio\Internal\Audio.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace System;
using namespace System::Internal::Audio;


#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG

std::mutex m;
int j = 0;

AddinCenterMgr* cmgr = nullptr;
AddinCenter center = nullptr;
AudioPlayerMgr* pmgr = nullptr;
AudioPlayer player = nullptr;
UInt128 tree = 0U;
UInt128 pl = 0U;
EventToken token = 0;
vector<wstring> audioPaths;
vector<wstring>::size_type audio_Index=0;
Optional<UInt128> dec_id;
Optional<UInt128> dec_id1;
Optional<UInt128> dec_id2;
Optional<UInt128> dec_id3;
Optional<UInt128> dec_id4;
Optional<UInt128> dec_id5;
Optional<UInt128> dec_id6;
Optional<UInt128> dec_id7;
Optional<UInt128> dec_id8;
Optional<UInt128> dev_id;
Optional<UInt128> dev_id1;
Optional<UInt128> dev_id2;
HANDLE  hConsole;
WORD hColor;
template < typename C, typename T = std::char_traits<C> >
struct basic_teebuf : public std::basic_streambuf<C, T>
{
	typedef std::basic_streambuf<C, T> streambuf_type;
	typedef typename T::int_type int_type;

	basic_teebuf(streambuf_type* buff_a, streambuf_type* buff_b)
		: first(buff_a), second(buff_b)
	{
	}

protected:
	virtual int_type overflow(int_type c)
	{
		const int_type eof = T::eof();
		if (T::eq_int_type(c, eof)) return T::not_eof(c);
		else
		{
			const C ch = T::to_char_type(c);
			if (T::eq_int_type(first->sputc(ch), eof) ||
				T::eq_int_type(second->sputc(ch), eof))
				return eof;
			else return c;
		}
	}

	virtual int sync()
	{
		return !first->pubsync() && !second->pubsync() ? 0 : -1;
	}

private:
	streambuf_type* first;
	streambuf_type* second;
};

template < typename C, typename T = std::char_traits<C> >
struct basic_teestream : public std::basic_ostream<C, T>
{
	typedef std::basic_ostream<C, T> stream_type;
	typedef basic_teebuf<C, T> streambuff_type;

	basic_teestream(stream_type& first, stream_type& second)
		: stream_type(&stmbuf), stmbuf(first.rdbuf(), second.rdbuf())
	{
	}

	basic_teestream(streambuff_type* first, streambuff_type* second)
		: stream_type(&stmbuf), stmbuf(first, second)
	{
		
	}

	~basic_teestream() { stmbuf.pubsync(); }

private: streambuff_type stmbuf;
};

typedef basic_teebuf<char> teebuf;
typedef basic_teestream<char> teestream;

typedef basic_teebuf<wchar_t> wteebuf;
typedef basic_teestream<wchar_t> wteestream;

void WINAPI TimerProc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	if (pmgr)
	{
		if (player)
		{
			if (!tree.IsZero)
			{
				auto res = pmgr->GetCurrent(player, tree);

				if (res.Value)
				{
					System::TimeSpan ts = System::TimeSpan::FromMilliseconds(res.State);
					std::wstring title = L"current: " + ts.ToString(L"h\\:mm\\:ss\\.fff");
					SetConsoleTitle(title.c_str());
				}
			}
		}
	}
}

void test_play(vector<wstring>::size_type index);
#ifdef _DEBUG  
#include <crtdbg.h>
#endif
void CenterEventCallBack(const AddinCenter& sender, CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, double rMs, double fMs, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	bool playornot = false;
	if (cmgr && pmgr && player && center)
	{
		if (sender == center)
		{
			if (id == CoreEvents::PlayCompleted && mobj==CoreObjects::Player && pl==obj)
			{
				if (args.Size == 1)
				{
					try
					{
						UInt128 iid = AnyVisitor<UInt128>::Get(args.At(0));
						if (iid == tree)
						{
							pmgr->Stop(player, iid, 0);
							SetConsoleTextAttribute(hConsole, 11);
							std::wcout<< "audio stop" << endl;
							SetConsoleTextAttribute(hConsole, hColor);
							tree = 0U; 
							audio_Index++;
							if (audio_Index >= audioPaths.size()) audio_Index = 0;
							playornot = true; 
						}
					}
					catch (...)
					{

					}
				}
				std::wcout << Formatter::Format(L"EventFlag: {0}, EventId: {1}, CoreObject: {2}, SubCoreObject: {3}, RaiseMs: {4}ms, FireMs: {5}ms, Msg: {6}", CultureInfo::Current(), (VariadicAny(flag), id, mobj, sobj, rMs, fMs, msg)) << endl;
			}
		}
	}
	if (playornot)
		test_play(audio_Index);
}

int loopcounter = 0;

void loop_test_play(vector<wstring>::size_type index)
{
	//if (loopcounter == 0)
	//	MessageBoxA(0, "ok", "", 0);

	//if (loopcounter == 100)
	//	MessageBoxA(0, "ok", "", 0);

	loopcounter++;
	std::cout << loopcounter << std::endl;
#ifdef _DEBUG  
	_CrtMemState s1, s2, s3;
#endif
	if (audioPaths.size()>0)
	{
		std::wcout << "audio: " << audioPaths[index] << std::endl;

		UInt128 dec_iid = 0u;
		if (StringHelper::EndsWith(audioPaths[index], wstring(L".mp32")))
		{
			dec_iid = *dec_id;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".opus")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".ogg")) || 
			StringHelper::EndsWith(audioPaths[index], wstring(L".ape")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".wv")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".mp3")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".wma")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".m4a")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".wav")) || 
			StringHelper::EndsWith(audioPaths[index], wstring(L".aiff")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".flac")) || StringHelper::EndsWith(audioPaths[index], wstring(L".fla")) || 
			StringHelper::EndsWith(audioPaths[index], wstring(L".mpc")) || StringHelper::EndsWith(audioPaths[index], wstring(L".mp+")))
		{
			dec_iid = *dec_id1;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".wav")) || StringHelper::EndsWith(audioPaths[index], wstring(L".aiff")) || StringHelper::EndsWith(audioPaths[index], wstring(L".aifc")) || StringHelper::EndsWith(audioPaths[index], wstring(L".mat")) || StringHelper::EndsWith(audioPaths[index], wstring(L".au")))
		{
			dec_iid = *dec_id2;
		}
		else if ( StringHelper::EndsWith(audioPaths[index], wstring(L".aac")) ||  StringHelper::EndsWith(audioPaths[index], wstring(L".mp4")))
		{
			dec_iid = *dec_id6;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".tak")))
		{
			dec_iid = *dec_id8;
		}
#ifdef _DEBUG  
		_CrtMemCheckpoint(&s1);
		{
#endif
			auto ress = pmgr->Open(player, audioPaths[index].c_str(), dec_iid, *dev_id, -1);
			if (ress.Value2 != audio_tree_error::None)
				ress = pmgr->Open(player, audioPaths[index].c_str(), dec_iid, *dev_id1, -1);
			if (ress.Value2 != audio_tree_error::None)
				ress = pmgr->Open(player, audioPaths[index].c_str(), dec_iid, *dev_id2, -1);
			if (ress.Value2 == audio_tree_error::None)
			{
				auto pes = pmgr->Play(player, ress.Value1);
				if (pes.Value)
				{
					//pmgr->SetCurrent(player, ress.Value1, 15000);
					Sleep(2000);
					//pmgr->SetCurrent(player, ress.Value1, 50000);
					//Sleep(50000);
					pmgr->Stop(player, ress.Value1, 0);
				}
			}
#ifdef _DEBUG 
		}
		_CrtMemCheckpoint(&s2);
		if (s1.lTotalCount != s2.lTotalCount)
		{
			OutputDebugStringA((to_string(s2.lTotalCount - s1.lTotalCount)+"\r\n").c_str());
		}
		if (_CrtMemDifference(&s3, &s1, &s2))
		{
			_CrtMemDumpStatistics(&s3);
			//_CrtDumpMemoryLeaks();
		}	
#endif
		audio_Index++;
		if (audio_Index >= audioPaths.size()) audio_Index = 0;
		loop_test_play(audio_Index);
	}
}

int mmmm = 0;

void test_play(vector<wstring>::size_type index)
{
	if (audioPaths.size()>0)
	{
		if (!std::experimental::filesystem::exists(audioPaths[index]))
		{
			std::wcout << audioPaths[index] << " is not supported" << endl;
			tree = 0U;
			audio_Index++;
   			if (audio_Index >= audioPaths.size()) audio_Index = 0;
			test_play(audio_Index);
		}

		UInt128 dec_iid = 0u;
		if (StringHelper::EndsWith(audioPaths[index], wstring(L".mp32")))
		{
			dec_iid = *dec_id;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".opus")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".ogg")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".ape")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".wv")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".mp3")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".wma")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".m4a")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".wav")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".aiff")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".flac")) || StringHelper::EndsWith(audioPaths[index], wstring(L".fla")) ||
			StringHelper::EndsWith(audioPaths[index], wstring(L".mpc")) || StringHelper::EndsWith(audioPaths[index], wstring(L".mp+")))
		{
			dec_iid = *dec_id1;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".wav")) || StringHelper::EndsWith(audioPaths[index], wstring(L".aiff")) || StringHelper::EndsWith(audioPaths[index], wstring(L".aifc")) || StringHelper::EndsWith(audioPaths[index], wstring(L".mat")) || StringHelper::EndsWith(audioPaths[index], wstring(L".au")))
		{
			dec_iid = *dec_id2;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".aac")) || StringHelper::EndsWith(audioPaths[index], wstring(L".mp4")))
		{
			dec_iid = *dec_id6;
		}
		else if (StringHelper::EndsWith(audioPaths[index], wstring(L".tak")))
		{
			dec_iid = *dec_id8;
		}

		auto ress = pmgr->Open(player, audioPaths[index].c_str(), dec_iid, *dev_id, -1);
		if (ress.Value2 == audio_tree_error::Failed)
			ress = pmgr->Open(player, audioPaths[index].c_str(), dec_iid, *dev_id1, -1);
		if (ress.Value2 == audio_tree_error::Failed)
			ress = pmgr->Open(player, audioPaths[index].c_str(), dec_iid, *dev_id2, -1);
		if (ress.Value2 == audio_tree_error::None)
		{
			tree = ress.Value1;

			SetConsoleTextAttribute(hConsole, 10);
			std::wcout << L"audio is opened with uuid = " << Formatter::Format(ress.Value1) << endl;
			SetConsoleTextAttribute(hConsole, hColor);
			auto pes = pmgr->Play(player, ress.Value1);
			if (pes.Value)
			{
				SetConsoleTextAttribute(hConsole, 6);
				std::wcout << L"begin playering..." << endl;
				auto infores = pmgr->GetInfo(player, ress.Value1);

				if (infores.Value)
				{
					SetConsoleTextAttribute(hConsole, 6);
					std::wcout << "Codec: " << infores.State.Codec.ToString() << endl;
					std::wcout << "AvgBytesPerSec: " << infores.State.AvgBytesPerSec << endl;
					std::wcout << "BitRate: " << infores.State.BitRate << endl;
					std::wcout << "BitsPerSample: " << infores.State.BitsPerSample.ToString() << endl;
					std::wcout << "BlockAlign: " << infores.State.BlockAlign << endl;
					std::wcout << "ChannelLayout: " << infores.State.ChannelLayout.ToString() << endl;
					std::wcout << "Channels: " << infores.State.Channels << endl;
					std::wcout << "SampleLength: " << infores.State.SampleLength << endl;
					std::wcout << "SampleRate: " << infores.State.SampleRate << endl;
					std::wcout << "TimeMs: " << infores.State.TimeMs << endl;

					SetConsoleTextAttribute(hConsole, hColor);
				}
				auto re = pmgr->SetCurrent(player, ress.Value1, infores.State.TimeMs - 5000);
				if (re.Value)
				{
					std::wcout << L"seeked to " << pmgr->GetCurrent(player, ress.Value1).State << " ms" << endl;
				}
			}
			else
			{
				std::wcout <<"can not play" << endl;
				pmgr->Stop(player, tree, 1);
				tree = 0U;
				audio_Index++;
				if (audio_Index >= audioPaths.size()) audio_Index = 0;
				test_play(audio_Index);
			}
		}
		else
		{
			std::wcout << audioPaths[index] << " is not supported" << endl;
			tree = 0U;
			audio_Index++;
			if (audio_Index >= audioPaths.size()) audio_Index = 0;
			test_play(audio_Index);
		}
	}
}

#if _DEBUG
#define DLL_AUDIO L"Audiod.dll"
#define DLL_MP3 L"Audio_Decoder_MP3d.dll"
#define DLL_STD L"Audio_Decoder_STDd.dll"
#define DLL_SND L"Audio_Decoder_SNDd.dll"
#define DLL_WV L"Audio_Decoder_WVd.dll"
#define DLL_APE L"Audio_Decoder_APEd.dll"
#define DLL_MF L"Audio_Decoder_MFd.dll"
#define DLL_OPUS L"Audio_Decoder_OPUSd.dll"
#define DLL_TAK L"Audio_Decoder_TAKd.dll"
#define DLL_DS L"Audio_Device_DSd.dll"
#define DLL_XA L"Audio_Device_XA2d.dll"
#define DLL_WAS L"Audio_Device_Wasapid.dll"
#else
#define DLL_AUDIO L"Audio.dll"
#define DLL_MP3 L"Audio_Decoder_MP3.dll"
#define DLL_STD L"Audio_Decoder_STD.dll"
#define DLL_SND L"Audio_Decoder_SND.dll"
#define DLL_WV L"Audio_Decoder_WV.dll"
#define DLL_APE L"Audio_Decoder_APE.dll"
#define DLL_MF L"Audio_Decoder_MF.dll"
#define DLL_OPUS L"Audio_Decoder_OPUS.dll"
#define DLL_TAK L"Audio_Decoder_TAK.dll"
#define DLL_DS L"Audio_Device_DS.dll"
#define DLL_XA L"Audio_Device_XA2.dll"
#define DLL_WAS L"Audio_Device_Wasapi.dll"
#endif

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

std::wstring ExePath() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG  
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
#endif
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO hColorInfo = {};

	GetConsoleScreenBufferInfo(hConsole, &hColorInfo);

	hColor = hColorInfo.wAttributes;

	MMRESULT timer_id;

	timer_id = timeSetEvent(1, 1, (LPTIMECALLBACK)TimerProc, DWORD(1), TIME_PERIODIC);
	if (NULL == timer_id)
	{
		printf("timeSetEvent() failed with error %d\n", GetLastError());
		return 0;
	}

	auto path = ExePath();
	wstring _fa = path;
	wstring _pa = path;
	_fa += L"\\Test.log";
	_pa += L"\\Tests.custom";
	
//#ifdef _DEBUG
//	_fa = L"E:\\U++ºËÐÄ¿â\\Debug_x64\\Test.log";
//	_pa = L"E:\\U++ºËÐÄ¿â\\Debug_x64\\Test.custom";
//
//	SetDllDirectoryW(L"E:\\U++ºËÐÄ¿â\\Debug_x64\\");
//
//#endif

	//std::wfilebuf fbuf;
	//fbuf.open(_fa.c_str(), std::ios::out);
	//std::wstreambuf* stdoutbuf = std::std::wcout.rdbuf();
	//wteebuf tbuf(&fbuf, stdoutbuf);
	//std::std::wcout.rdbuf(&tbuf);

	auto dds = CultureInfo::Current().CLocale;

	//fbuf.pubimbue(dds);
	//stdoutbuf->pubimbue(dds);
	//tbuf.pubimbue(dds);

	std::wcout.imbue(dds);
	std::wcout.precision(10);

	HMODULE audioLib = LoadLibraryW(DLL_AUDIO);

	auto errmsg = GetLastErrorAsString();

	GetAudioInterface pInfo = (GetAudioInterface)GetProcAddress(audioLib, "GetAudioInterface");
	if (pInfo)
	{
		AudioInterface* face = pInfo();
		if (face)
		{
			cmgr = face->GetCenterMgr();
			if (cmgr)
			{
				center = cmgr->Create();
				if (center)
				{
					auto eres = cmgr->RegisiterEventCallBack(center, CenterEventCallBack);
					if (eres.Value)
					{
						token = eres.State;
						auto res = cmgr->AddSection(center, DLL_MP3);
						if (res.Value1)
						{
							for (auto& a : res.Value2)
							{
								if (!dec_id) dec_id = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res.Message << std::endl;
						}


						auto res1 = cmgr->AddSection(center, DLL_STD);
						if (res1.Value1)
						{
							for (auto& a : res1.Value2)
							{
								if (!dec_id1) dec_id1 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res1.Message << std::endl;
						}

						auto res2 = cmgr->AddSection(center, DLL_SND);
						if (res2.Value1)
						{
							for (auto& a : res2.Value2)
							{
								if (!dec_id2) dec_id2 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res2.Message << std::endl;
						}
						auto res3 = cmgr->AddSection(center, DLL_WV);
						if (res3.Value1)
						{
							for (auto& a : res3.Value2)
							{
								if (!dec_id3) dec_id3 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res3.Message << std::endl;
						}

						auto res5 = cmgr->AddSection(center, DLL_APE);
						if (res5.Value1)
						{
							for (auto& a : res5.Value2)
							{
								if (!dec_id5) dec_id5 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res5.Message << std::endl;
						}

						auto res6 = cmgr->AddSection(center, DLL_MF);
						if (res6.Value1)
						{
							for (auto& a : res6.Value2)
							{
								if (!dec_id6) dec_id6 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res6.Message << std::endl;
						}

						auto res7 = cmgr->AddSection(center, DLL_OPUS);
						if (res7.Value1)
						{
							for (auto& a : res7.Value2)
							{
								if (!dec_id7) dec_id7 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res7.Message << std::endl;
						}

						/*
						auto res8 = cmgr->AddSection(center, DLL_TAK);
						if (res8.Value1)
						{
							for (auto& a : res8.Value2)
							{
								if (!dec_id8) dec_id8 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}
						else
						{
							std::wcout << "can not load beacuse: " << res8.Message << std::endl;
						}
						*/

						auto resx = cmgr->AddSection(center, DLL_DS);
						if (resx.Value1)
						{
							for (auto& a : resx.Value2)
							{
								if (!dev_id) dev_id = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}

						auto reswas = cmgr->AddSection(center, DLL_WAS);
						if (reswas.Value1)
						{
							for (auto& a : reswas.Value2)
							{
								if (!dev_id2) dev_id2 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}

						auto resxa = cmgr->AddSection(center, DLL_XA);
						if (resxa.Value1)
						{
							for (auto& a : resxa.Value2)
							{
								if (!dev_id1) dev_id1 = a.first;
								std::wcout << L"Audio Addin Type: " << (size_t)a.second << L"   uuid=" << Formatter::Format(a.first) << endl;
								auto r1 = cmgr->GetVersion(center, a.first);
								if (r1.Value)
								{
									std::wcout << L"  Version: " << r1.State.ToString() << endl;
								}
								auto r2 = cmgr->GetAuthor(center, a.first);
								if (r2.Value)
								{
									std::wcout << L"  Author: " << r2.State << endl;
								}
								auto r3 = cmgr->GetDescription(center, a.first);
								if (r3.Value)
								{
									std::wcout << L"  Description: " << r3.State << endl;
								}
								auto r4 = cmgr->GetSupportFormat(center, a.first);
								if (r4.Value)
								{
									std::wcout << L"  SupportFormat: " << r4.State << endl;
								}
							}
						}


						//test();
						pmgr = face->GetPlayerMgr();
						if (pmgr)
						{
							auto plres = pmgr->Create(center, 1000U);
							if (plres.Value1)
							{
								player = plres.Value2;
								pl = plres.State;
								
								/*pmgr->SetEqActive(player, true);
								pmgr->SetEqConfig(player, 10, 2);
								pmgr->SetEqGain(player, 0, 3);
								pmgr->SetEqGain(player, 1, 1);
								pmgr->SetEqGain(player, 2, 0);
								pmgr->SetEqGain(player, 3, -2);
								pmgr->SetEqGain(player, 4, -4);
								pmgr->SetEqGain(player, 5, -4);
								pmgr->SetEqGain(player, 6, -2);
								pmgr->SetEqGain(player, 7, 0);
								pmgr->SetEqGain(player, 8, 1);
								pmgr->SetEqGain(player, 9, 2);*/

								pmgr->SetPitch(player, 1.1);
								pmgr->SetTempo(player, 10);

								std::wcout << _pa << endl;
								try
								{
									IO::FileStreamReader fs = IO::FileStreamReader(_pa.c_str(), Encodings::Unicode, false);
									wstring flag_path = L"audiopath=";
									while (!fs.IsEndOfStream())
									{
										wstring line = fs.ReadLine();
										if (line.size() <= flag_path.size()) continue;
										if (StringHelper::StartsWith(line, flag_path, StringComparison::CurrentCultureIgnoreCase))
										{
											audioPaths.push_back(line.substr(line.find_first_of(L'=') + 1));
											std::wcout << "audiopath: " << audioPaths[audioPaths.size() - 1] << endl;
										}
									}
									if (audioPaths.size() > 0) test_play(0);
									//if (audioPaths.size() > 0) loop_test_play(0);
								}
								catch (...)
								{
									std::wcout << "can not find Test.custom" << endl;
								}
							}
						}
					}
				}
			}
		}
	}
	MSG   msgFoo; 
	BOOL bRet;
	while ((bRet = GetMessage(&msgFoo, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit  
		}
		else
		{
			TranslateMessage(&msgFoo);
			DispatchMessage(&msgFoo);
		}
	}
	timeKillEvent(timer_id); 

	//std::std::wcout.rdbuf(stdoutbuf);

	return 0;
}

