#pragma once
namespace System
{
	template <long long key, long long val>
	struct CompileTimeMapPair
	{
		static const long long k = key, v = val;
	};

	template <long long defv, typename...>
	struct CompileTimeMap;

	template <long long defv>
	struct CompileTimeMap<defv>
	{
		template<long long>
		struct get
		{
			static const long long val = defv;
		};

		static constexpr long long find(long long key)
		{
			return defv;
		}
	};

	template<long long defv, long long k, long long v, typename... rest>
	struct CompileTimeMap<defv, CompileTimeMapPair<k, v>, rest...>
	{
		template<long long key>
		struct get
		{
			static const long long val =
				(key == k) ?
				v :
				CompileTimeMap<defv, rest...>::template get<key>::val;
		};

		static constexpr long long find(long long key)
		{
			return key == k ? v : CompileTimeMap<defv, rest...>::find(key);
		}
	};
}