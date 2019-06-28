#include "PluginCenter.h"
#include "Player.h"
#include <deque>
#include "WindowsEnvironment.h"
using namespace System::OS;
using namespace System::Audio;

#define EVENTMSG API_WM_USER+0xFF

namespace System
{
	namespace Audio
	{
		class EventPack
		{
		public:
			PluginCenter* PC;
			CoreEventFlags Flag;
			CoreEvents EventID;
			Any User;
			CoreObjects Object;
			CoreSubObjects SubObject;
			double FireTimeMs;
			UInt128 ObjectID;
			wstring Message;
			VariadicAny Args;
			EventPack(PluginCenter* pC, CoreEventFlags flag, CoreEvents eventID, const Any& userIdentity, CoreObjects object, CoreSubObjects subObject, double fireTimeMs, const UInt128& objectID, const wstring& message, const VariadicAny& args)
			{
				PC = pC; 
				Flag = flag;
				EventID = eventID;	
				User = userIdentity;
				Object = object;
				SubObject = subObject;
				FireTimeMs = fireTimeMs;
				ObjectID = objectID;
				Message = message;
				Args = args;
			}
			~EventPack()
			{
			}
		};


		typedef void(*InternalPostCallBack)(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args);
		class Core_base_impl
		{
		protected:
			friend class Core_base;
			friend class PluginCenter;
			InternalPostCallBack _post;
			API_PTR _owner;
			PluginCenter* _center;
			Core_base_impl() : _owner(0), _post(nullptr), _center(nullptr) {}
			~Core_base_impl() {}
		};

		API_INT64 API_STDCALL WndProcEx(API_PTR hwnd, API_UINT32 message, API_UINT64 wParam, API_INT64 lParam)
		{
			switch (message)
			{
			case API_WM_DESTROY:
				Microsoft::winapi_PostQuitMessage(0);
				return 0;
			}
			return Microsoft::winapi_DefWindowProc(hwnd, message, wParam, lParam);
		}

		class PluginCenterEventManager
		{
			API_PTR Dispatcher;
			bool Kill;
			std::mutex locker;
			std::deque<EventPack*> caches;
			static bool reg;
			static API_WNDCLASS wc;
		public:
			API_PTR owner;
			PluginCenter* host;
			static unsigned int _stdcall DispatchThread(void* obj);
			static API_INT64 API_STDCALL OnWndProc(API_PTR hwnd, API_UINT32 message, API_UINT64 wParam, API_INT64 lParam);
			PluginCenterEventManager(PluginCenter* pc);

			void Post(EventPack* pack)
			{
				
				if (!pack) return;
				lock_guard<std::mutex> lock(locker);
				caches.push_back(pack);
			}

			~PluginCenterEventManager()
			{
				Kill = true;
				if (Microsoft::winapi_WaitForSingleObject(Dispatcher, 1000) == API_WAIT_TIMEOUT)
					Microsoft::winapi_TerminateThread(Dispatcher, 0);
				Microsoft::winapi_CloseHandle(Dispatcher);
				while (!caches.empty())
				{
					EventPack* pack = caches.front();
					SAFE_DELETE(pack);
					caches.pop_front();
				}
			}
		};

		bool PluginCenterEventManager::reg = false;
		API_WNDCLASS PluginCenterEventManager::wc = { 0 };

		PluginCenterEventManager::PluginCenterEventManager(PluginCenter* pc) : owner(NULL)
		{
			host = pc;
			Kill = false;
			Dispatcher = (HANDLE)_beginthreadex(nullptr, 0, &DispatchThread, this, 0,0);
		}

		API_INT64 API_STDCALL PluginCenterEventManager::OnWndProc(API_PTR hwnd, API_UINT32 message, API_UINT64 wParam, API_INT64 lParam)
		{
			PluginCenterEventManager *EventMgr = (PluginCenterEventManager*)(Microsoft::winapi_GetWindowLongPtr(hwnd, API_GWLP_USERDATA));
			if (EventMgr)
			{
				switch (message)
				{
				case EVENTMSG:
				{
					EventPack* pack = (EventPack*)(wParam);
					if (pack)
					{
						double raiseMS = -1;
						UInt128 tree = 0U;
						if (pack->Args.Size > 1)
						{
							try
							{
								tree = AnyVisitor<UInt128>::Get(pack->Args.At(0));
							}
							catch (...){}
						}
						if (EventMgr)
						{
							if (pack->Object == CoreObjects::Player)
							{
								for (auto& pp : EventMgr->host->Players)
								{
									if (pp->Id == pack->ObjectID)
									{
										for (auto& AT : pp->Trees)
										{
											if (AT->Id == tree)
											{
												raiseMS = AT->GetCurrent();
												goto INVOKE;
											}
										}
									}
								}
							}
						}
					INVOKE:
						EventMgr->host->InternalEventRouter.Invoke(pack->PC, pack->Flag, pack->EventID, pack->User, pack->Object, pack->SubObject, pack->FireTimeMs, raiseMS, pack->ObjectID, pack->Message, pack->Args);
						EventMgr->host->EventRouter.Invoke(pack->PC, pack->Flag, pack->EventID, pack->User, pack->Object, pack->SubObject, pack->FireTimeMs, raiseMS, pack->ObjectID, pack->Message, pack->Args);
						EventMgr->host->Dispath(pack->Flag, pack->EventID, pack->User, pack->Object, pack->SubObject, pack->FireTimeMs, raiseMS, pack->ObjectID, pack->Message, pack->Args);
						if (pack->User.IsMissing) EventMgr->host->Capturer.Invoke(pack->PC, pack->Flag, pack->EventID, pack->Object, pack->SubObject, pack->FireTimeMs, raiseMS, pack->ObjectID, pack->Message, pack->Args);
						delete pack;
						pack = NULL;
					}
				}
					break;
				default:
					return Microsoft::winapi_DefWindowProc(hwnd, message, wParam, lParam);
				}
			}
			return 0;
		}

		unsigned int _stdcall PluginCenterEventManager::DispatchThread(void* obj)
		{
			PluginCenterEventManager* s = (PluginCenterEventManager*)obj;
			if (!reg)
			{
				auto hInstance = Microsoft::winapi_GetModuleHandle(NULL);
				wc.style = 0;
				wc.lpfnWndProc = WndProcEx;
				wc.cbClsExtra = 0;
				wc.cbWndExtra = 0;
				wc.hInstance = NULL;
				wc.hIcon = NULL;
				wc.hCursor = Microsoft::winapi_LoadCursor(hInstance, API_IDC_ARROW);
				wc.hbrBackground = NULL;
				wc.lpszMenuName = NULL;
				wc.lpszClassName = L"Audio_PluginCenter";
				Microsoft::winapi_RegisterClass(&wc);
			}
			s->owner = Microsoft::winapi_CreateWindow(NULL, L"Audio_PluginCenter", NULL, API_WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, API_HWND_MESSAGE, NULL, NULL, NULL);

			Microsoft::winapi_SetWindowLongPtr(s->owner, API_GWLP_WNDPROC, (API_INT64)OnWndProc);
			Microsoft::winapi_SetWindowLongPtr(s->owner, API_GWLP_USERDATA, (API_INT64)s);

			while (!s->Kill)
			{
				s->locker.lock();
				//lock_guard<std::mutex> lock(s->locker);
				if (s->caches.empty())
				{
					Microsoft::winapi_Sleep(4);
				}
				while (!s->caches.empty())
				{
					EventPack* pack = s->caches.front();
					Microsoft::winapi_SendNotifyMessage(s->owner, EVENTMSG, (API_UINT64)pack, NULL);
					//SAFE_DELETE(pack);
					s->caches.pop_front();
				}
				s->locker.unlock();
				Microsoft::winapi_Sleep(2);
			}
			Microsoft::winapi_DestroyWindow(s->owner);
			s->owner = NULL;
			_endthreadex(0);
			return 0;
		}
	}
}

void PluginCenterEvent::Post(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	double fireMS = -1;
	UInt128 tree = 0U;
	if (args.Size > 1)
	{
		try
		{
			tree = AnyVisitor<UInt128>::Get(args.At(0));
		}
		catch (...){}
	}
	if (mobj == CoreObjects::Player)
	{
		for (auto& pp : sender->Players)
		{
			if (pp->Id == obj)
			{
				for (auto& AT : pp->Trees)
				{
					if (AT->Id == tree)
					{
						fireMS = AT->GetCurrent();
						goto POST;
					}
				}
			}
		}
	}
POST:
	EventPack* xwep = new EventPack(sender, flag, id, userIdentity, mobj, sobj, fireMS, obj, msg, args);
	sender->EventMgr->Post(xwep);
}

void PluginCenterEvent::Invoke(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& user, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	for (auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++)
	{
		(*i).second(sender, flag, id, user, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
	}
}

void PluginCenterCapturer::Invoke(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	for (auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++)
	{
		(*i).second(sender, flag, id, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
	}
}

void PluginCenter::Post(CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	lock_guard<std::mutex> lock(locker);
	EventRouter.Post(this, flag, id, Any(_missing), mobj, sobj, obj, msg, args);
}

void PluginCenter::UserPost(CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	EventRouter.Post(this, flag, id, userIdentity, mobj, sobj, obj, msg, args);
}

void PluginCenter::Dispath(CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	for (auto& dec : AudioDecoders)
	{
		Core_base* base = dec.second.obj;
		if (base)
		{
			base->OnCapture(base, flag, id, userIdentity, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
		}
	}	
	for (auto& dsp : AudioDSPs)
	{
		Core_base* base = dsp.second.obj;
		if (base)
		{
			base->OnCapture(base, flag, id, userIdentity, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
		}
	}

	for (auto& dev : AudioDevices)
	{
		Core_base* base = dev.second.obj;
		if (base)
		{
			base->OnCapture(base, flag, id, userIdentity, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
		}
	}
	for (auto& enc :AudioEncoders)
	{
		Core_base* base = enc.second.obj;
		if (base)
		{
			base->OnCapture(base, flag, id, userIdentity, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
		}
	}
	for (auto& vis: AudioVisuals)
	{
		Core_base* base = vis.second.obj;
		if (base)
		{
			base->OnCapture(base, flag, id, userIdentity, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
		}
	}
}

PluginCenter::PluginCenter()
{
	EventMgr = new PluginCenterEventManager(this);
}

PluginCenter::~PluginCenter()
{
	SAFE_DELETE(EventMgr);
}

void OnPostCallBack(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args)
{
	if (!OSHelper::IsValidAddress(sender, sizeof(PluginCenter), false)) return;
	sender->UserPost(flag, id, userIdentity, mobj, sobj, obj, msg, args);
}

Result<FactoryDllError, safe_void_ptr> PluginCenter::AddSection(const wchar_t* path, std::vector<std::pair<UInt128, AddinTypes>>& results)
{
	lock_guard<std::mutex> lock(locker);
	Result<FactoryDllError, safe_void_ptr> result = AppDomain::LoadDll(path);
	if (result.Value == FactoryDllError::OK)
	{
		map<const ClassInfo*, AddinTypes> tc;
		auto assenum = AppDomain::GetAssemblyEnumerator(result.State);
		if (assenum)
		{
			while (!assenum->IsEnd)
			{
				const Assembly* cass = assenum->GetCurrent();

				if (cass)
				{
					auto ass_name = cass->Name;

					auto modenum = cass->GetNameSpaceEnumerator(true);

					if (modenum)
					{
						while (!modenum->IsEnd)
						{
							const NameSpace* cmod = modenum->Current;
							if (cmod)
							{
								auto mod_name = cmod->Name;

								auto typeenum = cmod->GetTypeEnumerator();
								if (typeenum)
								{
									while (!typeenum->IsEnd)
									{
										const Type* ctype = typeenum->Current;
										if (ctype)
										{
											if (ctype->IsConvertible(TypeFactory::Create<AudioDecoder_base>()))
											{
												try
												{
													const ClassInfo* ci = dynamic_cast<const ClassInfo*>(ctype);
													if (ci)
													{
														if (Addins.find(ci) != Addins.end())
														{
															typeenum->Next();
															continue;
														}
														auto defc = ci->DefaultConstructor;
														if (defc)
														{
															if (!defc->HasParameters)
															{
																Object* obj = defc->Invoke();
																if (obj)
																{
																	try
																	{
																		AudioDecoder_base* base = dynamic_cast<AudioDecoder_base*>(obj);
																		if (base)
																		{
																			base->impl->_post = &OnPostCallBack;
																			base->impl->_center = this;
																			base->impl->_owner = EventMgr->owner;
																			UInt128 id = AppDomain::GenerateHashId128();
																			AudioDecoders.insert(make_pair(id, AddinDes<AddinTypes::AudioDecoder>(ci, base)));
																			results.push_back(make_pair(id, AddinTypes::AudioDecoder));
																			Addins.insert(make_pair(ci, id));
																			tc.insert(make_pair(ci, AddinTypes::AudioDecoder));
																		}
																	}
																	catch (...){}
																}
															}
														}
													}
												}
												catch (...){}
											}
											else if (ctype->IsConvertible(TypeFactory::Create<AudioDevice_base>()))
											{
												try
												{
													const ClassInfo* ci = dynamic_cast<const ClassInfo*>(ctype);
													if (ci)
													{
														if (Addins.find(ci) != Addins.end())
														{
															typeenum->Next();
															continue;
														}
														auto defc = ci->DefaultConstructor;
														if (defc)
														{
															if (!defc->HasParameters)
															{
																Object* obj = defc->Invoke();
																if (obj)
																{
																	try
																	{
																		AudioDevice_base* base = dynamic_cast<AudioDevice_base*>(obj);
																		if (base)
																		{
																			base->impl->_post = &OnPostCallBack;
																			base->impl->_center = this;
																			base->impl->_owner = EventMgr->owner;
																			UInt128 id = AppDomain::GenerateHashId128();
																			AudioDevices.insert(make_pair(id, AddinDes<AddinTypes::AudioDevice>(ci, base)));
																			results.push_back(make_pair(id, AddinTypes::AudioDevice));
																			Addins.insert(make_pair(ci, id));
																			tc.insert(make_pair(ci, AddinTypes::AudioDevice));
																		}
																	}
																	catch (...){}
																}
															}
														}
													}
												}
												catch (...){}
											}
											else if (ctype->IsConvertible(TypeFactory::Create<AudioDSP_base>()))
											{
												try
												{
													const ClassInfo* ci = dynamic_cast<const ClassInfo*>(ctype);
													if (ci)
													{
														if (Addins.find(ci) != Addins.end())
														{
															typeenum->Next();
															continue;
														}
														auto defc = ci->DefaultConstructor;
														if (defc)
														{
															if (!defc->HasParameters)
															{
																Object* obj = defc->Invoke();
																if (obj)
																{
																	try
																	{
																		AudioDSP_base* base = dynamic_cast<AudioDSP_base*>(obj);
																		if (base)
																		{
																			base->impl->_post = &OnPostCallBack;
																			base->impl->_center = this;
																			base->impl->_owner = EventMgr->owner;
																			UInt128 id = AppDomain::GenerateHashId128();
																			AudioDSPs.insert(make_pair(id, AddinDes<AddinTypes::AudioDSP>(ci, base)));
																			results.push_back(make_pair(id, AddinTypes::AudioDSP));
																			Addins.insert(make_pair(ci, id));
																			tc.insert(make_pair(ci, AddinTypes::AudioDSP));
																		}
																	}
																	catch (...){}
																}
															}
														}
													}
												}
												catch (...){}
											}
											else if (ctype->IsConvertible(TypeFactory::Create<AudioEncoder_base>()))
											{
												try
												{
													const ClassInfo* ci = dynamic_cast<const ClassInfo*>(ctype);
													if (ci)
													{
														if (Addins.find(ci) != Addins.end())
														{
															typeenum->Next();
															continue;
														}
														auto defc = ci->DefaultConstructor;
														if (defc)
														{
															if (!defc->HasParameters)
															{
																Object* obj = defc->Invoke();
																if (obj)
																{
																	try
																	{
																		AudioEncoder_base* base = dynamic_cast<AudioEncoder_base*>(obj);
																		if (base)
																		{
																			base->impl->_post = &OnPostCallBack;
																			base->impl->_center = this;
																			base->impl->_owner = EventMgr->owner;
																			UInt128 id = AppDomain::GenerateHashId128();
																			AudioEncoders.insert(make_pair(id, AddinDes<AddinTypes::AudioEncoder>(ci, base)));
																			results.push_back(make_pair(id, AddinTypes::AudioEncoder));
																			Addins.insert(make_pair(ci, id));
																			tc.insert(make_pair(ci, AddinTypes::AudioEncoder));
																		}
																	}
																	catch (...){}
																}
															}
														}
													}
												}
												catch (...){}
											}
											else if (ctype->IsConvertible(TypeFactory::Create<AudioVisual_base>()))
											{
												try
												{
													const ClassInfo* ci = dynamic_cast<const ClassInfo*>(ctype);
													if (ci)
													{
														if (Addins.find(ci) != Addins.end())
														{
															typeenum->Next();
															continue;
														}
														auto defc = ci->DefaultConstructor;
														if (defc)
														{
															if (!defc->HasParameters)
															{
																Object* obj = defc->Invoke();
																if (obj)
																{
																	try
																	{
																		AudioVisual_base* base = dynamic_cast<AudioVisual_base*>(obj);
																		if (base)
																		{
																			base->impl->_post = &OnPostCallBack; 
																			base->impl->_center = this;
																			base->impl->_owner = EventMgr->owner;
																			UInt128 id = AppDomain::GenerateHashId128();
																			AudioVisuals.insert(make_pair(id, AddinDes<AddinTypes::AudioVisual>(ci, base)));
																			results.push_back(make_pair(id, AddinTypes::AudioVisual));
																			Addins.insert(make_pair(ci, id));
																			tc.insert(make_pair(ci, AddinTypes::AudioVisual));
																		}
																	}
																	catch (...){}
																}
															}
														}
													}
												}
												catch (...){}
											}
										}
										typeenum->Next();
									}
								}
							}
							modenum->Next();
						}
					}
				}
				assenum->Next();
			}
		}
		if (tc.size())
		{
			vector<AddinClsDes> clss;
			for (auto& t : tc)
			{
				auto iter = Addins.find(t.first);
				if (iter != Addins.end())
				{
					clss.push_back(AddinClsDes(t.first, iter->second,t.second));
				}
			}
			if (clss.size())
			{
				Dlls.insert(make_pair(result.State, clss));
			}
		}
	}
	return result;
}

Result<bool, std::vector<UInt128>> PluginCenter::CanRemoveSection(const safe_void_ptr& section)
{
	lock_guard<std::mutex> lock(locker);
	Result<bool, std::vector<UInt128>> result(true, std::vector<UInt128>());
	auto iter = Dlls.find(section);
	if (iter != Dlls.end())
	{
		auto clss = iter->second;
		for (auto& id : clss)
		{
			switch (id.addin)
			{
			case AddinTypes::AudioDecoder:
			{
											 auto iter = AudioDecoders.find(id.id);
											 if (iter != AudioDecoders.end())
											 {
												 AudioDecoder_base* obj = iter->second.obj;
												 if (obj)
												 {
													 if (obj->GetNumberOfPipes() == 0)
													 {
														 result.State.push_back(iter->first);
													 }
													 else
														 result.Value = false;
												 }
											 }
			}
				break;
			case AddinTypes::AudioDevice:
			{
											auto iter = AudioDevices.find(id.id);
											if (iter != AudioDevices.end())
											{
												AudioDevice_base* obj = iter->second.obj;
												if (obj)
												{
													if (obj->GetNumberOfPipes() == 0)
													{
														result.State.push_back(iter->first);
													}
													else
														result.Value = false;
												}
											}
			}
				break;
			case AddinTypes::AudioEncoder:
			{
											 auto iter = AudioEncoders.find(id.id);
											 if (iter != AudioEncoders.end())
											 {
												 AudioEncoder_base* obj = iter->second.obj;
												 if (obj)
												 {
													 if (obj->GetNumberOfPipes() == 0)
													 {
														 result.State.push_back(iter->first);
													 }
													 else
														 result.Value = false;
												 }
											 }
			}
				break;
			case AddinTypes::AudioDSP:
			{
										 auto iter = AudioDSPs.find(id.id);
										 if (iter != AudioDSPs.end())
										 {
											 AudioDSP_base* obj = iter->second.obj;
											 if (obj)
											 {
												 if (obj->GetNumberOfPipes() == 0)
												 {
													 result.State.push_back(iter->first);
												 }
												 else
													 result.Value = false;
											 }
										 }
			}
				break;
			case AddinTypes::AudioVisual:
			{
											auto iter = AudioVisuals.find(id.id);
											if (iter != AudioVisuals.end())
											{
												AudioVisual_base* obj = iter->second.obj;
												if (obj)
												{
													if (obj->GetNumberOfPipes() == 0)
													{
														result.State.push_back(iter->first);
													}
													else
														result.Value = false;
												}
											}
			}
				break;
			}
		}
	}
	else
		result.Value = false;
	return result;
}

Result<bool, std::vector<UInt128>> PluginCenter::RemoveSection(const safe_void_ptr& section)
{
	lock_guard<std::mutex> lock(locker);
	Result<bool, std::vector<UInt128>> result(true, std::vector<UInt128>());
	auto iter = Dlls.find(section);
	if (iter != Dlls.end())
	{
		auto clss = iter->second;
		for (auto& id : clss)
		{
			switch (id.addin)
			{
			case AddinTypes::AudioDecoder:
			{
											 auto iter = AudioDecoders.find(id.id);
											 if (iter != AudioDecoders.end())
											 {
												 AudioDecoder_base* obj = iter->second.obj;
												 if (obj)
												 {
													 if (obj->GetNumberOfPipes() == 0)
													 {
														 delete obj;
														 obj = nullptr;
														 Addins.erase(iter->second.type);
														 iter->second.type = nullptr;	
														 result.State.push_back(iter->first);
														 AudioDecoders.erase(iter); 
													 }
													 else
														 result.Value = false;
												 }
											 }
			}
				break;
			case AddinTypes::AudioDevice:
			{
											auto iter = AudioDevices.find(id.id);
											if (iter != AudioDevices.end())
											{
												AudioDevice_base* obj = iter->second.obj;
												if (obj)
												{
													if (obj->GetNumberOfPipes() == 0)
													{
														delete obj;
														obj = nullptr;
														Addins.erase(iter->second.type);
														iter->second.type = nullptr;
														result.State.push_back(iter->first);
														AudioDevices.erase(iter); 
													}
													else
														result.Value = false;
												}
											}
			}
				break;
			case AddinTypes::AudioEncoder:
			{
											 auto iter = AudioEncoders.find(id.id);
											 if (iter != AudioEncoders.end())
											 {
												 AudioEncoder_base* obj = iter->second.obj;
												 if (obj)
												 {
													 if (obj->GetNumberOfPipes() == 0)
													 {	 
														 delete obj;
														 obj = nullptr;
														 Addins.erase(iter->second.type);
														 iter->second.type = nullptr;
														 result.State.push_back(iter->first);
														 AudioEncoders.erase(iter); 
													 }
													 else
														 result.Value = false;
												 }
											 }
			}
				break;
			case AddinTypes::AudioDSP:
			{
										 auto iter = AudioDSPs.find(id.id);
										 if (iter != AudioDSPs.end())
										 {
											 AudioDSP_base* obj = iter->second.obj;
											 if (obj)
											 {
												 if (obj->GetNumberOfPipes() == 0)
												 {
													 delete obj;
													 obj = nullptr;
													 Addins.erase(iter->second.type);
													 iter->second.type = nullptr;
													 result.State.push_back(iter->first);
													 AudioDSPs.erase(iter);														
												 }
												 else
													 result.Value = false;
											 }
										 }
			}
				break;
			case AddinTypes::AudioVisual:
			{
											auto iter = AudioVisuals.find(id.id);
											if (iter != AudioVisuals.end())
											{
												AudioVisual_base* obj = iter->second.obj;
												if (obj)
												{
													if (obj->GetNumberOfPipes() == 0)
													{
														delete obj;
														obj = nullptr;
														Addins.erase(iter->second.type);
														iter->second.type = nullptr;
														result.State.push_back(iter->first);
														AudioVisuals.erase(iter);
													}
													else
														result.Value = false;
												}
											}
			}
				break;
			}
		}
		if (result.Value)
		{
			iter->second.clear();
			auto unload_res = AppDomain::UnLoadDll(iter->first);
			if (unload_res.Value == FactoryDllError::OK)
				Dlls.erase(iter);
			else
				result.WriteMessage(unload_res.Message);
		}
		else
		{
			for (auto it = iter->second.begin(); it != iter->second.end();)
			{
				if (find(result.State.begin(), result.State.end(), it->id) != result.State.end())
				{
					it = iter->second.erase(it);
				}
				else
					++it;
			}
		}
	}
	else
	{
		result.Value = false;
	}

	return result;
}

Result<bool> PluginCenter::CanRemoveAddin(const UInt128& addinId)
{
	lock_guard<std::mutex> lock(locker);
	Result<bool> result(false);
	for (auto& dec : AudioDecoders)
	{
		AudioDecoder_base* base = dec.second.obj;
		if (base)
		{
			result.Value = base->GetNumberOfPipes() == 0;
			return result;
		}
	}
	for (auto& dsp : AudioDSPs)
	{
		AudioDSP_base* base = dsp.second.obj;
		if (base)
		{
			result.Value = base->GetNumberOfPipes() == 0;
			return result;
		}
	}

	for (auto& dev : AudioDevices)
	{
		AudioDevice_base* base = dev.second.obj;
		if (base)
		{
			result.Value = base->GetNumberOfPipes() == 0;
			return result;
		}
	}
	for (auto& enc : AudioEncoders)
	{
		AudioEncoder_base* base = enc.second.obj;
		if (base)
		{
			result.Value = base->GetNumberOfPipes() == 0;
			return result;
		}
	}
	for (auto& vis : AudioVisuals)
	{
		AudioVisual_base* base = vis.second.obj;
		if (base)
		{
			result.Value = base->GetNumberOfPipes() == 0;
			return result;
		}
	}
	return result;
}

Result<bool, safe_void_ptr> PluginCenter::RemoveAddin(const UInt128& addinId)
{
	lock_guard<std::mutex> lock(locker);
	Result<bool, safe_void_ptr> result(false, nullptr);
	for (auto& dec : AudioDecoders)
	{
		AudioDecoder_base* base = dec.second.obj;
		if (base)
		{
			if (base->GetNumberOfPipes() == 0)
			{
				delete base;
				base = nullptr;
				auto iter = Addins.find(dec.second.type);
				if (iter != Addins.end()) Addins.erase(iter);
				for (auto& dll : Dlls)
				{
					auto dliter = find_if(dll.second.begin(), dll.second.end(), AddinClsDes_Equ(dec.first));
					if (dliter != dll.second.end()) dll.second.erase(dliter);
				}
				for (auto dlliter = Dlls.begin(); dlliter != Dlls.end();)
				{
					if (dlliter->second.size() == 0)
					{
						result.State = dlliter->first;
						dlliter = Dlls.erase(dlliter);
					}
					else
						++dlliter;
				}
				AudioDecoders.erase(dec.first);
				result.Value = true;
			}
			return result;
		}
	}
	for (auto& dsp : AudioDSPs)
	{
		AudioDSP_base* base = dsp.second.obj;
		if (base)
		{
			if (base->GetNumberOfPipes() == 0)
			{
				delete base;
				base = nullptr;
				auto iter = Addins.find(dsp.second.type);
				if (iter != Addins.end()) Addins.erase(iter);
				for (auto& dll : Dlls)
				{
					auto dliter = find_if(dll.second.begin(), dll.second.end(), AddinClsDes_Equ(dsp.first));
					if (dliter != dll.second.end()) dll.second.erase(dliter);
				}
				for (auto dlliter = Dlls.begin(); dlliter != Dlls.end();)
				{
					if (dlliter->second.size() == 0)
					{
						result.State = dlliter->first;
						dlliter = Dlls.erase(dlliter);
					}
					else
						++dlliter;
				}
				AudioDSPs.erase(dsp.first);
				result.Value = true;
			}
			return result;
		}
	}

	for (auto& dev : AudioDevices)
	{
		AudioDevice_base* base = dev.second.obj;
		if (base)
		{
			if (base->GetNumberOfPipes() == 0)
			{
				delete base;
				base = nullptr;
				auto iter = Addins.find(dev.second.type);
				if (iter != Addins.end()) Addins.erase(iter);
				for (auto& dll : Dlls)
				{
					auto dliter = find_if(dll.second.begin(), dll.second.end(), AddinClsDes_Equ(dev.first));
					if (dliter != dll.second.end()) dll.second.erase(dliter);
				}
				for (auto dlliter = Dlls.begin(); dlliter != Dlls.end();)
				{
					if (dlliter->second.size() == 0)
					{
						result.State = dlliter->first;
						dlliter = Dlls.erase(dlliter);
					}
					else
						++dlliter;
				}
				AudioDevices.erase(dev.first);
				result.Value = true;
			}
			return result;
		}
	}
	for (auto& enc : AudioEncoders)
	{
		AudioEncoder_base* base = enc.second.obj;
		if (base)
		{
			if (base->GetNumberOfPipes() == 0)
			{
				delete base;
				base = nullptr;
				auto iter = Addins.find(enc.second.type);
				if (iter != Addins.end()) Addins.erase(iter);
				for (auto& dll : Dlls)
				{
					auto dliter = find_if(dll.second.begin(), dll.second.end(), AddinClsDes_Equ(enc.first));
					if (dliter != dll.second.end()) dll.second.erase(dliter);
				}
				for (auto dlliter = Dlls.begin(); dlliter != Dlls.end();)
				{
					if (dlliter->second.size() == 0)
					{
						result.State = dlliter->first;
						dlliter = Dlls.erase(dlliter);
					}
					else
						++dlliter;
				}
				AudioEncoders.erase(enc.first);
				result.Value = true;
			}
			return result;
		}
	}
	for (auto& vis : AudioVisuals)
	{
		AudioVisual_base* base = vis.second.obj;
		if (base)
		{
			if (base->GetNumberOfPipes() == 0)
			{
				delete base;
				base = nullptr;
				auto iter = Addins.find(vis.second.type);
				if (iter != Addins.end()) Addins.erase(iter);
				for (auto& dll : Dlls)
				{
					auto dliter = find_if(dll.second.begin(), dll.second.end(), AddinClsDes_Equ(vis.first));
					if (dliter != dll.second.end()) dll.second.erase(dliter);
				}
				for (auto dlliter = Dlls.begin(); dlliter != Dlls.end();)
				{
					if (dlliter->second.size() == 0)
					{
						result.State = dlliter->first;
						dlliter = Dlls.erase(dlliter);
					}
					else
						++dlliter;
				}
				AudioVisuals.erase(vis.first);
				result.Value = true;
			}
			return result;
		}
	}
	return result;
}

bool PluginCenter::GetVersion(const UInt128& addin, Version& ver)
{
	try
	{
		auto dec = AudioDecoders.find(addin);
		if (dec != AudioDecoders.end())
		{
			AudioDecoder_base* base = dec->second.obj;
			if (base)
			{
				ver = base->GetVersion();
				return true;
			}
		}
		auto dsp = AudioDSPs.find(addin);
		if (dsp != AudioDSPs.end())
		{
			AudioDSP_base* base = dsp->second.obj;
			if (base)
			{
				ver = base->GetVersion();
				return true;
			}
		}

		auto dev = AudioDevices.find(addin);
		if (dev != AudioDevices.end())
		{
			AudioDevice_base* base = dev->second.obj;
			if (base)
			{
				ver = base->GetVersion();
				return true;
			}
		}
		auto enc = AudioEncoders.find(addin);
		if (enc != AudioEncoders.end())
		{
			AudioEncoder_base* base = enc->second.obj;
			if (base)
			{
				ver = base->GetVersion();
				return true;
			}
		}
		auto vis = AudioVisuals.find(addin);
		if (vis != AudioVisuals.end())
		{
			AudioVisual_base* base = vis->second.obj;
			if (base)
			{
				ver = base->GetVersion();
				return true;
			}
		}
	}
	catch (...){ return false; }
	return false;
}

const wchar_t* PluginCenter::GetAuthor(const UInt128& addin)
{
	try
	{
		auto dec = AudioDecoders.find(addin);
		if (dec != AudioDecoders.end())
		{
			AudioDecoder_base* base = dec->second.obj;
			if (base)
			{
				return base->GetAuthor();
			}
		}
		auto dsp = AudioDSPs.find(addin);
		if (dsp != AudioDSPs.end())
		{
			AudioDSP_base* base = dsp->second.obj;
			if (base)
			{
				return base->GetAuthor();
			}
		}

		auto dev = AudioDevices.find(addin);
		if (dev != AudioDevices.end())
		{
			AudioDevice_base* base = dev->second.obj;
			if (base)
			{
				return base->GetAuthor();
			}
		}
		auto enc = AudioEncoders.find(addin);
		if (enc != AudioEncoders.end())
		{
			AudioEncoder_base* base = enc->second.obj;
			if (base)
			{
				return base->GetAuthor();
			}
		}
		auto vis = AudioVisuals.find(addin);
		if (vis != AudioVisuals.end())
		{
			AudioVisual_base* base = vis->second.obj;
			if (base)
			{
				return base->GetAuthor();
			}
		}
	}
	catch (...){ return nullptr; }
	return nullptr;
}

const wchar_t* PluginCenter::GetDescription(const UInt128& addin)
{
	try
	{
		auto dec = AudioDecoders.find(addin);
		if (dec != AudioDecoders.end())
		{
			AudioDecoder_base* base = dec->second.obj;
			if (base)
			{
				return base->GetDescription();
			}
		}
		auto dsp = AudioDSPs.find(addin);
		if (dsp != AudioDSPs.end())
		{
			AudioDSP_base* base = dsp->second.obj;
			if (base)
			{
				return base->GetDescription();
			}
		}

		auto dev = AudioDevices.find(addin);
		if (dev != AudioDevices.end())
		{
			AudioDevice_base* base = dev->second.obj;
			if (base)
			{
				return base->GetDescription();
			}
		}
		auto enc = AudioEncoders.find(addin);
		if (enc != AudioEncoders.end())
		{
			AudioEncoder_base* base = enc->second.obj;
			if (base)
			{
				return base->GetDescription();
			}
		}
		auto vis = AudioVisuals.find(addin);
		if (vis != AudioVisuals.end())
		{
			AudioVisual_base* base = vis->second.obj;
			if (base)
			{
				return base->GetDescription();
			}
		}
	}
	catch (...){ return nullptr; }
	return nullptr;
}

const wchar_t* PluginCenter::GetSupportFormat(const UInt128& addin)
{
	try
	{
		auto dec = AudioDecoders.find(addin);
		if (dec != AudioDecoders.end())
		{
			AudioDecoder_base* base = dec->second.obj;
			if (base)
			{
				return base->GetSupportFormat();
			}
		}
		auto dsp = AudioDSPs.find(addin);
		if (dsp != AudioDSPs.end())
		{
			AudioDSP_base* base = dsp->second.obj;
			if (base)
			{
				return base->GetSupportFormat();
			}
		}

		auto dev = AudioDevices.find(addin);
		if (dev != AudioDevices.end())
		{
			AudioDevice_base* base = dev->second.obj;
			if (base)
			{
				return base->GetSupportFormat();
			}
		}
		auto enc = AudioEncoders.find(addin);
		if (enc != AudioEncoders.end())
		{
			AudioEncoder_base* base = enc->second.obj;
			if (base)
			{
				return base->GetSupportFormat();
			}
		}
		auto vis = AudioVisuals.find(addin);
		if (vis != AudioVisuals.end())
		{
			AudioVisual_base* base = vis->second.obj;
			if (base)
			{
				return base->GetSupportFormat();
			}
		}
	}
	catch (...){ return nullptr; }
	return nullptr;
}

bool PluginCenter::GetNumPipes(const UInt128& addin, UInt& num)
{
	try
	{
		auto dec = AudioDecoders.find(addin); 
		if (dec != AudioDecoders.end())
		{
			AudioDecoder_base* base = dec->second.obj;
			if (base)
			{
				num= base->GetNumberOfPipes();
				return true;
			}
		}
		auto dsp = AudioDSPs.find(addin);
		if (dsp != AudioDSPs.end())
		{
			AudioDSP_base* base = dsp->second.obj;
			if (base)
			{
				num = base->GetNumberOfPipes();
				return true;
			}
		}

		auto dev = AudioDevices.find(addin);
		if (dev != AudioDevices.end())
		{
			AudioDevice_base* base = dev->second.obj;
			if (base)
			{
				num = base->GetNumberOfPipes();
				return true;
			}
		}
		auto enc = AudioEncoders.find(addin);
		if (enc != AudioEncoders.end())
		{
			AudioEncoder_base* base = enc->second.obj;
			if (base)
			{
				num = base->GetNumberOfPipes();
				return true;
			}
		}
		auto vis = AudioVisuals.find(addin);
		if (vis != AudioVisuals.end())
		{
			AudioVisual_base* base = vis->second.obj;
			if (base)
			{
				num = base->GetNumberOfPipes();
				return true;
			}
		}
	}
	catch (...){ return false; }
	return false;
}