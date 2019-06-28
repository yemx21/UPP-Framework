#pragma once
#include "UICore_Config.h"
#include <functional>
#include <vector>
#include <map>
#include <memory>

using namespace std;

namespace System
{
	namespace UI
	{
		class DependencyObject;

		template<typename... Args> class MulticastDelegate;

		template<typename T1>
		class MulticastDelegate <T1>
		{
		public:
			MulticastDelegate() : tokenCounter(0)
			{

			}
			typedef std::function<void(std::shared_ptr<DependencyObject>, T1)> CallbackFunction;

		public:
			void Invoke(const std::shared_ptr<DependencyObject>& sender, const T1& arg)
			{
				for (auto i = handlerMap.begin(); i != handlerMap.end(); i++)
				{
					(*i).second(sender, arg);
				}
			}

			void operator()(const std::shared_ptr<DependencyObject>& sender, const T1& arg)
			{
				Invoke(sender, arg);
			}

			EventToken operator += (CallbackFunction f) const
			{
				EventToken token = tokenCounter++;

				m_handlerMap[token] = f;
				return token;
			}

			MulticastDelegate& operator -= (CallbackFunction f)
			{
				for (auto i = handlerMap.begin(); i != handlerMap.end(); i++)
				{
					if ((*i).target<void(T1)>() == f.target<void(T1)>())
					{
						handlerMap.erase(i);
						break;
					}
				}

				return *this;
			}

			EventToken operator -= (EventToken token) const
			{
				m_handlerMap.erase(token);
				return token;
			}

		private:
			atomic<EventToken> tokenCounter;
			mutable std::map<EventToken, CallbackFunction> handlerMap;
		};
	}
}