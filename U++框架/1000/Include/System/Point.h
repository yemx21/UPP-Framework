#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>



namespace System
{

	/// <summary>
	/// 点类
	/// </summary>
	class RUNTIME_API Point : __uobject
	{
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;

		double x;
		double y;
	public:
		/// <summary>
		/// 创建 <see cref="Point"/>实例.
		/// </summary>
		/// <param name="px">X轴坐标值</param>
		/// <param name="py">Y轴坐标值</param>
		__uref Point(double px, double py);

		/// <summary>
		/// 创建 <see cref="Point"/>实例.
		/// </summary>
		__uref Point();

		virtual ~Point();

		/// <summary>
		/// 获取X轴坐标值
		/// </summary>
		/// <returns></returns>
		double GetX() const;

		/// <summary>
		/// 获取Y轴坐标值
		/// </summary>
		/// <returns></returns>
		double GetY() const;

		/// <summary>
		/// 获取X轴坐标值引用
		/// </summary>
		/// <returns></returns>
		double& GetXRef();

		/// <summary>
		/// 获取Y轴坐标值引用
		/// </summary>
		/// <returns></returns>
		double& GetYRef();

		/// <summary>
		/// 设置X轴坐标值
		/// </summary>
		/// <param name="val">值</param>
		void SetX(double val);

		/// <summary>
		/// 设置Y轴坐标值
		/// </summary>
		/// <param name="val">值</param>
		void SetY(double val);

		__uref Point(const Point& point);

		__uref Point operator + (const Point& point) const;

		__uref Point operator - (const Point& point) const;

		__uref Point& operator += (Point& point);

		__uref Point& operator -= (Point& point);

		__uref Point & operator = (const Point & point);

		__uref bool operator == (Point& point) const;

		__uref bool operator == (const Point& point) const;

		__uref bool operator != (Point& point) const;

		__uref bool operator != (const Point& point) const;

		/// <summary>
		/// 获取未知点
		/// </summary>
		/// <returns></returns>
		__uref static Point Unknown();

		/// <summary>
		/// 判断是否未知
		/// </summary>
		/// <returns>bool.</returns>
		__uref bool GetIsUnknown() const;

		/// <summary>
		/// X轴坐标值
		/// </summary>
		__uproperty(get = GetX, put = SetX) double X;

		/// <summary>
		/// Y轴坐标值
		/// </summary>
		__uproperty(get = GetY, put = SetY) double Y;

		/// <summary>
		/// X轴坐标值引用
		/// </summary>
		__uproperty(get = GetXRef) double& XRef;

		/// <summary>
		/// Y轴坐标值引用
		/// </summary>
		__uproperty(get = GetYRef) double& YRef;

		/// <summary>
		/// 是否未知
		/// </summary>
		__uproperty(get = GetIsUnknown) bool IsUnknown;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Point& result);

		REFLECT_CLASS(Point)
	};


	class RUNTIME_API PointConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<PointConverter> _thisWeakPtr;
	protected:
		PointConverter();
		PointConverter(PointConverter const&);
		void operator=(PointConverter const&);
	public:
		virtual ~PointConverter();

		static std::shared_ptr<PointConverter> GetSharedInstance();

		static PointConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}