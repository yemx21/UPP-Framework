#pragma once
#include "Config.h"
namespace System
{
	namespace Audio
	{
		class CoreEvent
		{
		public:
			CoreEvent() : m_token(0)
			{

			}
			typedef std::function<void(std::shared_ptr<Core_base>, CoreEventFlags, CoreEvents, CoreObjects, CoreSubObjects, double, double, intptr_t, const wstring&, const VariadicAny&)> CallbackFunction;

		public:
			void Invoke(std::shared_ptr<Core_base> sender, CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, intptr_t obj, const wstring& msg, const VariadicAny& args)
			{
				for (auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++)
				{
					(*i).second(sender, flag, id, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
				}
			}

			void operator()(std::shared_ptr<Core_base> sender, CoreEventFlags flag, CoreEvents id, CoreObjects mobj, CoreSubObjects sobj, double fireTimeMs, double raiseTimeMs, intptr_t obj, const wstring& msg, const VariadicAny& args)
			{
				Invoke(sender, flag, id, mobj, sobj, fireTimeMs, raiseTimeMs, obj, msg, args);
			}

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
	}
}

