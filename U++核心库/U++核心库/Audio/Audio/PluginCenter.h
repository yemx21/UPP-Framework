#pragma once
#include "Config.h"
#include <map>
#include <vector>
#include <mutex>

namespace System
{
	namespace Audio
	{
		class Player;
		class DSPManager;
		class PluginCenter;
		class AudioTree;

		enum class AddinTypes
		{
			AudioDecoder,
			AudioDSP,
			AudioDevice,
			AudioVisual,
			AudioEncoder,

			LryicDecoder,
			LryicEncoder,
			LryicVisual,
		};

		template<AddinTypes T>
		struct AddinDes;
		
		
		template<>
		struct AddinDes<AddinTypes::AudioDecoder> : ZeroStruct<AddinDes<AddinTypes::AudioDecoder>>
		{
			const ClassInfo* type;
			AudioDecoder_base* obj;

			AddinDes() :ZeroStruct<AddinDes>() {}
			AddinDes(const AddinDes& des)
			{
				type = des.type;
				obj = des.obj;
			}
			AddinDes(const ClassInfo* Otype, AudioDecoder_base* base) :type(Otype), obj(base)
			{
			}

			bool operator ==(const AddinDes& des) const
			{
				return type == des.type && obj == des.obj;
			}

			bool operator !=(const AddinDes& des) const
			{
				return type != des.type || obj != des.obj;
			}

			friend bool operator == (const AddinDes &ls, const AddinDes &rs);
			friend bool operator != (const AddinDes &ls, const AddinDes &rs);
		};


		template<>
		struct AddinDes<AddinTypes::AudioDevice> : ZeroStruct<AddinDes<AddinTypes::AudioDevice>>
		{
			const ClassInfo* type;
			AudioDevice_base* obj;

			AddinDes() :ZeroStruct<AddinDes>() {}
			AddinDes(const AddinDes& des)
			{
				type = des.type;
				obj = des.obj;
			}
			AddinDes(const ClassInfo* Otype, AudioDevice_base* base) :type(Otype), obj(base)
			{
			}

			bool operator ==(const AddinDes& des) const
			{
				return type == des.type && obj == des.obj;
			}

			bool operator !=(const AddinDes& des) const
			{
				return type != des.type || obj != des.obj;
			}

			friend bool operator == (const AddinDes &ls, const AddinDes &rs);
			friend bool operator != (const AddinDes &ls, const AddinDes &rs);
		};

		inline bool operator == (const AddinDes<AddinTypes::AudioDevice> &ls, const AddinDes<AddinTypes::AudioDevice> &rs)
		{
			return ls.type == rs.type && ls.obj == rs.obj;
		}

		inline bool operator != (const AddinDes<AddinTypes::AudioDevice> &ls, const AddinDes<AddinTypes::AudioDevice> &rs)
		{
			return ls.type != rs.type || ls.obj != rs.obj;
		}

		template<>
		struct AddinDes<AddinTypes::AudioDSP> : ZeroStruct<AddinDes<AddinTypes::AudioDSP>>
		{
			const ClassInfo* type;
			AudioDSP_base* obj;

			AddinDes() :ZeroStruct<AddinDes>() {}
			AddinDes(const AddinDes& des)
			{
				type = des.type;
				obj = des.obj;
			}
			AddinDes(const ClassInfo* Otype, AudioDSP_base* base) :type(Otype), obj(base)
			{
			}

			bool operator ==(const AddinDes& des) const
			{
				return type == des.type && obj == des.obj;
			}

			bool operator !=(const AddinDes& des) const
			{
				return type != des.type || obj != des.obj;
			}

			friend bool operator == (const AddinDes &ls, const AddinDes &rs);
			friend bool operator != (const AddinDes &ls, const AddinDes &rs);
		};

		inline bool operator == (const AddinDes<AddinTypes::AudioDSP> &ls, const AddinDes<AddinTypes::AudioDSP> &rs)
		{
			return ls.type == rs.type && ls.obj == rs.obj;
		}

		inline bool operator != (const AddinDes<AddinTypes::AudioDSP> &ls, const AddinDes<AddinTypes::AudioDSP> &rs)
		{
			return ls.type != rs.type || ls.obj != rs.obj;
		}

		template<>
		struct AddinDes<AddinTypes::AudioEncoder> : ZeroStruct<AddinDes<AddinTypes::AudioEncoder>>
		{
			const ClassInfo* type;
			AudioEncoder_base* obj;

			AddinDes() :ZeroStruct<AddinDes>() {}
			AddinDes(const AddinDes& des)
			{
				type = des.type;
				obj = des.obj;
			}
			AddinDes(const ClassInfo* Otype, AudioEncoder_base* base) :type(Otype), obj(base)
			{
			}

			bool operator ==(const AddinDes& des) const
			{
				return type == des.type && obj == des.obj;
			}

			bool operator !=(const AddinDes& des) const
			{
				return type != des.type || obj != des.obj;
			}

			friend bool operator == (const AddinDes &ls, const AddinDes &rs);
			friend bool operator != (const AddinDes &ls, const AddinDes &rs);
		};

		inline bool operator == (const AddinDes<AddinTypes::AudioEncoder> &ls, const AddinDes<AddinTypes::AudioEncoder> &rs)
		{
			return ls.type == rs.type && ls.obj == rs.obj;
		}

		inline bool operator != (const AddinDes<AddinTypes::AudioEncoder> &ls, const AddinDes<AddinTypes::AudioEncoder> &rs)
		{
			return ls.type != rs.type || ls.obj != rs.obj;
		}

		template<>
		struct AddinDes<AddinTypes::AudioVisual> : ZeroStruct<AddinDes<AddinTypes::AudioVisual>>
		{
			const ClassInfo* type;
			AudioVisual_base* obj;

			AddinDes() :ZeroStruct<AddinDes>() {}
			AddinDes(const AddinDes& des)
			{
				type = des.type;
				obj = des.obj;
			}
			AddinDes(const ClassInfo* Otype, AudioVisual_base* base) :type(Otype), obj(base)
			{
			}

			bool operator ==(const AddinDes& des) const
			{
				return type == des.type && obj == des.obj;
			}

			bool operator !=(const AddinDes& des) const
			{
				return type != des.type || obj != des.obj;
			}

			friend bool operator == (const AddinDes &ls, const AddinDes &rs);
			friend bool operator != (const AddinDes &ls, const AddinDes &rs);
		};

		inline bool operator == (const AddinDes<AddinTypes::AudioVisual> &ls, const AddinDes<AddinTypes::AudioVisual> &rs)
		{
			return ls.type == rs.type && ls.obj == rs.obj;
		}

		inline bool operator != (const AddinDes<AddinTypes::AudioVisual> &ls, const AddinDes<AddinTypes::AudioVisual> &rs)
		{
			return ls.type != rs.type || ls.obj != rs.obj;
		}

		template<AddinTypes T>
		using AddinCollection = unordered_map<UInt128, AddinDes<T>>;

		struct AddinClsDes : ZeroStruct<AddinClsDes>
		{
			const ClassInfo* type;
			UInt128 id;
			AddinTypes addin;
			AddinClsDes() :ZeroStruct<AddinClsDes>() {}
			AddinClsDes(const AddinClsDes& des)
			{
				type = des.type;
				id=des.id;
				addin = des.addin;
			}
			AddinClsDes(const ClassInfo* Otype, const UInt128& Oid, AddinTypes Atype) :type(Otype), id(Oid), addin(Atype)
			{
			}

			bool operator ==(const AddinClsDes& des) const
			{
				return type == des.type && id == des.id && addin == des.addin;
			}

			bool operator !=(const AddinClsDes& des) const
			{
				return type != des.type || id != des.id || addin != des.addin;
			}

			friend bool operator == (const AddinClsDes &ls, const AddinClsDes &rs) ;
			friend bool operator != (const AddinClsDes &ls, const AddinClsDes &rs);
		};

		inline bool operator == (const AddinClsDes &ls, const AddinClsDes &rs)
		{
			return ls.type == rs.type && ls.id == rs.id && ls.addin == rs.addin;
		}

		inline bool operator != (const AddinClsDes &ls, const AddinClsDes &rs)
		{
			return ls.type != rs.type || ls.id != rs.id || ls.addin != rs.addin;
		}

		class AddinClsDes_Equ
		{
		public:
			UInt128 i;
			explicit AddinClsDes_Equ(const UInt128& s) : i(s) {}
			bool  operator ()(const AddinClsDes& e) const { return e.id == i; }
		};

		class PluginCenterEventManager;

		class PluginCenterEvent final
		{
		public:
			PluginCenterEvent() : m_token(0)
			{

			}
			typedef std::function<void(PluginCenter*, CoreEventFlags, CoreEvents, const Any&, CoreObjects, CoreSubObjects, double, double, const UInt128&, const wstring&, const VariadicAny&)> CallbackFunction;
		protected:
			friend class PluginCenter;
			friend class PluginCenterEventManager;
			void Post(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args);
			void Invoke(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, const UInt128& obj, const wstring& msg, const VariadicAny& args);
		public:

			EventToken operator += (CallbackFunction f) const
			{
				intptr_t token = m_token++;

				m_handlerMap[token] = f;
				return token;
			}

			EventToken operator -= (EventToken token) const
			{
				m_handlerMap.erase(token);
				return token;
			}

		private:
			mutable atomic<intptr_t> m_token;
			mutable std::map<EventToken, CallbackFunction> m_handlerMap;
		};

		class PluginCenterCapturer
		{
		public:
			PluginCenterCapturer() : m_token(0)
			{

			}
			typedef std::function<void(const safe_void_ptr&, CoreEventFlags, CoreEvents, CoreObjects, CoreSubObjects, double, double, const UInt128&, const wstring&, const VariadicAny&)> CallbackFunction;
		protected:
			friend class PluginCenter;
			friend class PluginCenterEventManager;
			void Invoke(PluginCenter* sender, CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, const UInt128& obj, const wstring& msg, const VariadicAny& args);
		public:

			EventToken operator += (CallbackFunction f) const
			{
				EventToken token = m_token++;

				m_handlerMap[token] = f;
				return token;
			}

			EventToken operator -= (EventToken token) const
			{
				m_handlerMap.erase(token);
				return token;
			}

		private:
			mutable atomic<EventToken> m_token;
			mutable std::map<EventToken, CallbackFunction> m_handlerMap;

		};

		class PluginCenter
		{
		private:
			std::mutex locker;
			friend class DSPManager;
			friend class Player;
			friend class PluginCenterEvent;
			friend class PluginCenterEventManager;
			std::vector<Player*> Players;
			AddinCollection<AddinTypes::AudioDecoder> AudioDecoders;
			AddinCollection<AddinTypes::AudioDevice> AudioDevices;
			AddinCollection<AddinTypes::AudioDSP> AudioDSPs;
			AddinCollection<AddinTypes::AudioEncoder> AudioEncoders;
			AddinCollection<AddinTypes::AudioVisual> AudioVisuals;

			std::unordered_map<const ClassInfo*, UInt128> Addins;
			std::unordered_map<safe_void_ptr, std::vector<AddinClsDes>> Dlls;

			void Dispath(CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, const UInt128& obj, const wstring& msg, const VariadicAny& args);

			PluginCenterEvent InternalEventRouter;

			PluginCenterEventManager* EventMgr;

		public:

			UInt128 GenerateHashId128() const
			{
				return AppDomain::GenerateHashId128();
			}

			void DestoryHashId128(const UInt128& id) const
			{
				return AppDomain::DestoryHashId128(id);
			}

			PluginCenter();
			~PluginCenter();

			PluginCenterEvent EventRouter;

			PluginCenterCapturer Capturer;

			Result<FactoryDllError, safe_void_ptr> AddSection(const wchar_t* Path, std::vector<std::pair<UInt128, AddinTypes>>& results);

			Result<bool, std::vector<UInt128>> CanRemoveSection(const safe_void_ptr& section);

			Result<bool, std::vector<UInt128>> RemoveSection(const safe_void_ptr& section);
			
			Result<bool> CanRemoveAddin(const UInt128& addinId);

			Result<bool, safe_void_ptr> RemoveAddin(const UInt128& addinId);

			void Post(CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args);
			void UserPost(CoreEventFlags flag, CoreEvents id, const Any& userIdentity, CoreObjects mobj, CoreSubObjects sobj, const UInt128& obj, const wstring& msg, const VariadicAny& args);
			
			AudioDecoder_base* GetAudioDecoder(const UInt128& id)
			{
				lock_guard<std::mutex> lock(locker);
				auto iter = AudioDecoders.find(id);
				if (iter != AudioDecoders.end()) return iter->second.obj;
				return nullptr;
			}

			AudioDSP_base* GetAudioDSP(const UInt128& id)
			{
				lock_guard<std::mutex> lock(locker);
				auto iter = AudioDSPs.find(id);
				if (iter != AudioDSPs.end()) return iter->second.obj;
				return nullptr;
			}

			AudioDevice_base* GetAudioDevice(const UInt128& id)
			{
				lock_guard<std::mutex> lock(locker);
				auto iter = AudioDevices.find(id);
				if (iter != AudioDevices.end()) return iter->second.obj;
				return nullptr;
			}

			AudioEncoder_base* GetAudioEncoder(const UInt128& id)
			{
				lock_guard<std::mutex> lock(locker);
				auto iter = AudioEncoders.find(id);
				if (iter != AudioEncoders.end()) return iter->second.obj;
				return nullptr;
			}

			AudioVisual_base* GetAudioVisual(const UInt128& id)
			{
				lock_guard<std::mutex> lock(locker);
				auto iter = AudioVisuals.find(id);
				if (iter != AudioVisuals.end()) return iter->second.obj;
				return nullptr;
			}

			bool GetVersion(const UInt128&, Version&);

			const wchar_t* GetAuthor(const UInt128&);

			const wchar_t* GetDescription(const UInt128&);

			const wchar_t* GetSupportFormat(const UInt128&);

			bool GetNumPipes(const UInt128&, UInt&);

		};

	}
}

inline bool operator == (const AddinDes<AddinTypes::AudioDecoder> &ls, const AddinDes<AddinTypes::AudioDecoder> &rs)
{
	return ls.type == rs.type && ls.obj == rs.obj;
}

inline bool operator != (const AddinDes<AddinTypes::AudioDecoder> &ls, const AddinDes<AddinTypes::AudioDecoder> &rs)
{
	return ls.type != rs.type || ls.obj != rs.obj;
}