#pragma once
#include "Config.h"

using namespace std;

namespace System
{
	/// <summary>
	/// 比较类
	/// </summary>
	class RUNTIME_API ComparisonHelper
	{
	public:
		/// <summary>
		/// 最大数字
		/// </summary>
		/// <param name="num1">数字1</param>
		/// <param name="num2">数字2</param>
		/// <returns></returns>
		template<typename T>
		static T Max(T num1, T num2)
		{
			return num1 >= num2 ? num1 : num2;
		}
		/// <summary>
		/// 最小数字
		/// </summary>
		/// <param name="num1">数字1</param>
		/// <param name="num2">数字2</param>
		/// <returns></returns>
		template<typename T>
		static T Min(T num1, T num2)
		{
			return num1 < num2 ? num1 : num2;
		}
	};
}