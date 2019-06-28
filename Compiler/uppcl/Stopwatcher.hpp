#pragma once
#include <profileapi.h>

namespace uppcl
{
	class Stopwatcher
	{
		LARGE_INTEGER tmp;
		double freq;
		__int64 t1;
		__int64 t2;
	public:
		Stopwatcher()
		{
			QueryPerformanceFrequency(&tmp);
			freq = tmp.QuadPart;
		}

		void Restart()
		{
			QueryPerformanceCounter(&tmp);
			t1 = tmp.QuadPart;
		}

		double Elapsed()
		{
			QueryPerformanceCounter(&tmp);
			t2 = tmp.QuadPart;

			return (t2 - t1) * 1000 / freq;
		}
	};
}