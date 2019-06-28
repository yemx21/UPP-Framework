#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>
#include "Point.h"
#include "Size.h"
#include <math.h>


namespace System
{
	/// <summary>
	/// 矩形类
	/// </summary>
	class RUNTIME_API Rect : __uobject
	{
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		double l;
		double t;
		double w;
		double h;
	public:
		/// <summary>
		/// 创建<see cref="Rect"/>实例.
		/// </summary>
		/// <param name="left">左边X轴坐标值</param>
		/// <param name="top">上边Y轴坐标值</param>
		/// <param name="width">宽度</param>
		/// <param name="height">高度</param>
		__uref Rect(double left, double top, double width, double height);

		/// <summary>
		/// 创建<see cref="Rect"/>实例.
		/// </summary>
		/// <param name="xy">左上角坐标值</param>
		/// <param name="size">指定矩形大小</param>
		__uref Rect(const Point& xy, const System::Size& size);

		/// <summary>
		/// 创建<see cref="Rect"/>实例.
		/// </summary>
		/// <param name="p1">左上角坐标值</param>
		/// <param name="p2">右下角坐标值</param>
		__uref Rect(const Point& p1, const Point& p2);

		/// <summary>
		/// 创建<see cref="Rect"/>实例.
		/// </summary>
		/// <param name="size">指定矩形大小</param>
		__uref Rect(const System::Size& size);

		/// <summary>
		/// 创建<see cref="Rect"/>实例.
		/// </summary>
		__uref Rect();

		~Rect();

		/// <summary>
		/// 获取左边X轴坐标值
		/// </summary>
		/// <returns></returns>
		double GetLeft() const;

		/// <summary>
		/// 获取右边X轴坐标值
		/// </summary>
		/// <returns></returns>
		double GetRight() const;

		/// <summary>
		/// 设置右边X轴坐标值
		/// </summary>
		/// <param name="val">值</param>
		void SetRight(double val);

		/// <summary>
		/// 获取上边Y轴坐标值
		/// </summary>
		/// <returns></returns>
		double GetTop() const;

		/// <summary>
		/// 获取底边Y轴坐标值
		/// </summary>
		/// <returns></returns>
		double GetBottom() const;

		/// <summary>
		/// 设置底边Y轴坐标值
		/// </summary>
		/// <param name="val">值</param>
		void SetBottom(double val);

		/// <summary>
		/// 设置左边X轴坐标值
		/// </summary>
		/// <param name="val">值</param>
		void SetLeft(double val);

		/// <summary>
		/// 设置上边Y轴坐标值
		/// </summary>
		/// <param name="val">值</param>
		void SetTop(double val);

		/// <summary>
		/// 获取宽度
		/// </summary>
		/// <returns></returns>
		double GetWidth() const;

		/// <summary>
		/// 获取宽度引用
		/// </summary>
		/// <returns></returns>
		double& GetWidthRef();

		/// <summary>
		///设置宽度
		/// </summary>
		/// <param name="val">值</param>
		void SetWidth(double val);

		/// <summary>
		/// 获取高度
		/// </summary>
		/// <returns></returns>
		double GetHeight() const;

		/// <summary>
		/// 获取高度引用
		/// </summary>
		/// <returns></returns>
		double& GetHeightRef();

		/// <summary>
		/// 设置高度
		/// </summary>
		/// <param name="val">值</param>
		void SetHeight(double val);

		/// <summary>
		/// 获取左上角坐标值
		/// </summary>
		/// <returns></returns>
		Point GetLocation() const;

		/// <summary>
		/// 设置左上角坐标值
		/// </summary>
		/// <param name="point">指定左上角点</param>
		void SetLocation(const Point& point);

		/// <summary>
		/// 获取矩形大小
		/// </summary>
		/// <returns></returns>
		System::Size GetSize() const;

		/// <summary>
		/// 设置矩形大小
		/// </summary>
		/// <param name="size">指定大小</param>
		void SetSize(const System::Size& size);

		/// <summary>
		/// 判断矩形是否为空
		/// </summary>
		/// <returns></returns>
		bool GetIsEmpty() const;

		/// <summary>
		/// 判断是否包含指定点
		/// </summary>
		/// <param name="x">指定点所在X轴坐标值</param>
		/// <param name="y">指定点所在Y轴坐标值</param>
		__uref bool Contains(double x, double y) const;

		/// <summary>
		/// 判断是否包含指定点
		/// </summary>
		/// <param name="point">指定点</param>
		__uref bool Contains(const Point& point) const;

		/// <summary>
		/// 判断是否包含指定矩形
		/// </summary>
		/// <param name="rect">指定矩形</param>
		__uref bool Contains(const Rect& rect) const;


		/// <summary>
		/// 膨胀当前矩形
		/// </summary>
		/// <param name="size">膨胀大小</param>
		__uref void Inflate(const System::Size& size);

		/// <summary>
		/// 膨胀当前矩形
		/// </summary>
		/// <param name="width">指定膨胀宽度</param>
		/// <param name="height">指定膨胀高度</param>
		__uref void Inflate(double width, double height);

		/// <summary>
		/// 获取空矩形
		/// </summary>
		/// <returns></returns>
		__uref static Rect Empty();

		/// <summary>
		/// 膨胀指定矩形
		/// </summary>
		/// <param name="rect">指定矩形</param>
		/// <param name="size">膨胀大小</param>
		/// <returns></returns>
		__uref static Rect Inflate(const Rect& rect, const System::Size& size);

		/// <summary>
		/// 膨胀指定矩形
		/// </summary>
		/// <param name="rect">指定矩形</param>
		/// <param name="width">膨胀宽度</param>
		/// <param name="height">膨胀高度</param>
		/// <returns></returns>
		__uref static Rect Inflate(const Rect& rect, double width, double height);

		/// <summary>
		/// 应用缩放值
		/// </summary>
		/// <param name="scaleX">X轴缩放值</param>
		/// <param name="scaleY">Y轴缩放值</param>
		__uref void Scale(double scaleX, double scaleY);

		/// <summary>
		/// 判断是否与指定矩形相交
		/// </summary>
		/// <param name="rect">指定矩形</param>
		/// <returns></returns>
		__uref bool IntersectsWith(const Rect& rect);

		/// <summary>
		/// 与指定矩形取交集
		/// </summary>
		/// <param name="rect">指定矩形</param>
		__uref void Intersect(const Rect& rect);

		/// <summary>
		/// 取两个矩形的交集
		/// </summary>
		/// <param name="rect1">矩形1</param>
		/// <param name="rect2">矩形2</param>
		/// <returns></returns>
		__uref static Rect Intersect(const Rect& rect1, const Rect& rect2);

		/// <summary>
		/// 与指定矩形取合集
		/// </summary>
		/// <param name="rect">指定矩形</param>
		__uref void Union(const Rect& rect);

		/// <summary>
		/// 取两个矩形的合集
		/// </summary>
		/// <param name="rect1">矩形1</param>
		/// <param name="rect2">矩形2</param>
		/// <returns></returns>
		__uref static Rect Union(const Rect& rect1, const Rect& rect2);

		/// <summary>
		/// 与指定点取合集
		/// </summary>
		/// <param name="point">指定点</param>
		__uref void Union(const Point& point);

		/// <summary>
		/// 取指定矩形和指定点的合集
		/// </summary>
		/// <param name="rect">指定矩形</param>
		/// <param name="point">指定点</param>
		/// <returns></returns>
		__uref static Rect Union(const Rect& rect, const Point& point);

		/// <summary>
		/// 偏移当前矩形
		/// </summary>
		/// <param name="offsetX">X轴偏移值</param>
		/// <param name="offsetY">Y轴偏移值</param>
		__uref void Offset(double offsetX, double offsetY);

		/// <summary>
		/// 偏移指定矩形
		/// </summary>
		/// <param name="rect">指定矩形</param>
		/// <param name="offsetX">X轴偏移值</param>
		/// <param name="offsetY">Y轴偏移值</param>
		/// <returns></returns>
		__uref static Rect Offset(const Rect& rect, double offsetX, double offsetY);


		__uref Rect(const Rect& rect);

		__uref Rect & operator = (const Rect & rect);

		__uref bool operator == (Rect& rect) const;

		__uref bool operator == (const Rect& rect) const;

		__uref bool operator != (Rect& rect) const;

		__uref bool operator != (const Rect& rect) const;

		/// <summary>
		/// 获取未知矩形
		/// </summary>
		/// <returns></returns>
		__uref static Rect Unknown();

		/// <summary>
		/// 判断是否未知
		/// </summary>
		/// <returns>bool.</returns>
		bool GetIsUnknown() const;

		/// <summary>
		/// 左边X轴坐标值
		/// </summary>
		__uproperty(get = GetLeft, put = SetLeft) double Left;
		/// <summary>
		/// 上边Y轴坐标值
		/// </summary>
		__uproperty(get = GetTop, put = SetTop) double Top;
		/// <summary>
		/// 左边X轴坐标值引用
		/// </summary>
		__uproperty(get = GetLeftRef) double LeftRef;
		/// <summary>
		/// 上边Y轴坐标值引用
		/// </summary>
		__uproperty(get = GetTopRef) double TopRef;
		/// <summary>
		/// 右边X轴坐标值
		/// </summary>
		__uproperty(get = GetRight, put = SetRight) double Right;
		/// <summary>
		/// 底边Y轴坐标值
		/// </summary>
		__uproperty(get = GetBottom, put = SetBottom) double Bottom;
		/// <summary>
		/// 宽度
		/// </summary>
		__uproperty(get = GetWidth, put = SetWidth) double Width;
		/// <summary>
		/// 高度
		/// </summary>
		__uproperty(get = GetHeight, put = SetHeight) double Height;		
		/// <summary>
		/// 宽度引用
		/// </summary>
		__uproperty(get = GetWidthRef) double WidthRef;
		/// <summary>
		/// 高度引用
		/// </summary>
		__uproperty(get = GetHeightRef) double HeightRef;
		/// <summary>
		/// 左上角坐标值
		/// </summary>
		__uproperty(get = GetLocation, put = SetLocation) Point Location;
		/// <summary>
		/// 矩形大小
		/// </summary>
		__uproperty(get = GetSize, put = SetSize) System::Size Size;
		/// <summary>
		/// 是否未知
		/// </summary>
		__uproperty(get = GetIsUnknown) bool IsUnknown;
		/// <summary>
		/// 是否为空
		/// </summary>
		__uproperty(get = GetIsEmpty) bool IsEmpty;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		/// /
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Rect& result);

		REFLECT_CLASS(Rect)
	};

	class RUNTIME_API RectConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<RectConverter> _thisWeakPtr;
	protected:
		RectConverter();
		RectConverter(RectConverter const&);
		void operator=(RectConverter const&);
	public:
		virtual ~RectConverter();

		static std::shared_ptr<RectConverter> GetSharedInstance();

		static RectConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};


}
