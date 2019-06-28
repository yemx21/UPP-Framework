#pragma once
#include "Point.h"
#include "Size.h"
#include "Rect.h"
namespace System
{	
	/// <summary>
	/// 浮点数字类
	/// </summary>
	 class RUNTIME_API FloatingHelper
	{
	public :
		/// <summary>
		/// 判断是否接近
		/// </summary>
		/// <param name="a">单精度浮点数A</param>
		/// <param name="b">单精度浮点数B</param>
		/// <returns></returns>
		static bool AreClose(float a, float b) ;

		/// <summary>
		/// 判断是否接近被零除的数
		/// </summary>
		/// <param name="numerator">分子</param>
		/// <param name="denominator">分母</param>
		static bool IsCloseToDivideByZero(float numerator, float denominator) ;

		/// <summary>
		/// 判断是否为一
		/// </summary>
		/// <param name="a">单精度浮点数A</param>
		static bool IsOne(float a) ;

		/// <summary>
		/// 判断是否为零
		/// </summary>
		/// <param name="a">单精度浮点数A</param>
		static bool IsZero(float a) ;

		/// <summary>
		/// 判断是否接近
		/// </summary>
		/// <param name="a">双精度浮点数A</param>
		/// <param name="b">双精度浮点数B</param>
		/// <returns></returns>
		static bool AreClose(double value1, double value2) ;

		/// <summary>
		/// 判断是否接近
		/// </summary>
		/// <param name="point1">点1</param>
		/// <param name="point2">点2</param>
		/// <returns></returns>
		static bool AreClose(const Point& point1, const Point& point2) ;

		/// <summary>
		/// 判断是否接近
		/// </summary>
		/// <param name="size1">大小1</param>
		/// <param name="size2">大小2</param>
		/// <returns></returns>
		static bool AreClose(const Size& size1, const Size& size2) ;

		/// <summary>
		/// 判断是否接近
		/// </summary>
		/// <param name="rect1">矩形1</param>
		/// <param name="rect2">矩形2</param>
		/// <returns></returns>
		static bool AreClose(const Rect& rect1, const Rect& rect2) ;

		/// <summary>
		/// 双精度浮点数转换到整数
		/// </summary>
		/// <param name="val">双精度浮点数</param>
		/// <returns>int.</returns>
		static int DoubleToInt(double val) ;

		/// <summary>
		/// 判断是否大于
		/// </summary>
		/// <param name="value1">双精度浮点数1</param>
		/// <param name="value2">双精度浮点数2</param>
		/// <returns></returns>
		static bool GreaterThan(double value1, double value2) ;

		/// <summary>
		/// 判断是否大于约等于
		/// </summary>
		/// <param name="value1">双精度浮点数1</param>
		/// <param name="value2">双精度浮点数2</param>
		/// <returns></returns>
		static bool GreaterThanOrClose(double value1, double value2) ;

		/// <summary>
		/// 判断是否在零和一之间
		/// </summary>
		/// <param name="val">双精度浮点数</param>
		static bool IsBetweenZeroAndOne(double val) ;

		/// <summary>
		/// 判断是否为非数字
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsNaN(double value) ;

		/// <summary>
		/// 判断是否无限
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsInfinity(double value) ;

		/// <summary>
		///判断是否为非数字或无限
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsNaNOrInfinity(double value) ;

		/// <summary>
		/// 判断是否正无限
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsPositiveInfinity(double value) ;

		/// <summary>
		/// 判断是否负无限
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsNegativeInfinity(double value) ;

		/// <summary>
		/// 判断是否为一
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsOne(double value) ;

		/// <summary>
		/// 判断是否为零
		/// </summary>
		/// <param name="value">双精度浮点数</param>
		static bool IsZero(double value) ;

		/// <summary>
		/// 判断小于
		/// </summary>
		/// <param name="value1">双精度浮点数1</param>
		/// <param name="value2">双精度浮点数2</param>
		/// <returns></returns>
		static bool LessThan(double value1, double value2) ;

		/// <summary>
		/// 判断小于约等于
		/// </summary>
		/// <param name="value1">双精度浮点数1</param>
		/// <param name="value2">双精度浮点数2</param>
		/// <returns></returns>
		static bool LessThanOrClose(double value1, double value2) ;

		/// <summary>
		/// 判断矩形是否包含非数字
		/// </summary>
		/// <param name="rect">矩形</param>
		/// <returns></returns>
		static bool RectHasNaN(const Rect& rect) ;
	};
}