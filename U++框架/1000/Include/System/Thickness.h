#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>
namespace System
{
	/// <summary>
	/// 边距粗细类
	/// </summary>
	class RUNTIME_API  Thickness : __uobject
	{
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		double l;
		double t;
		double r;
		double b;
	public:
		/// <summary>
		/// 创建<see cref="Thickness"/>实例.
		/// </summary>
		__uref Thickness();

		/// <summary>
		/// 创建<see cref="Thickness"/>实例.
		/// </summary>
		/// <param name="left">左边距</param>
		/// <param name="top">上边距</param>
		/// <param name="right">右边距</param>
		/// <param name="bottom">底边距</param>
		__uref Thickness(double left, double top, double right, double bottom);

		__uref Thickness(const Thickness& thickness);

		/// <summary>
		/// 获取左边距
		/// </summary>
		/// <returns></returns>
		double GetLeft() const;

		/// <summary>
		/// 获取上边距
		/// </summary>
		/// <returns></returns>
		double GetTop() const;

		/// <summary>
		/// 获取右边距
		/// </summary>
		/// <returns></returns>
		double GetRight() const;

		/// <summary>
		/// 获取底边距
		/// </summary>
		/// <returns></returns>		
		double GetBottom() const;

		/// <summary>
		/// 获取左边距引用
		/// </summary>
		/// <returns></returns>
		double& GetLeftRef();

		/// <summary>
		/// 获取上边距引用
		/// </summary>
		/// <returns></returns>
		double& GetTopRef();

		/// <summary>
		/// 获取右边距引用
		/// </summary>
		/// <returns></returns>
		double& GetRightRef();

		/// <summary>
		/// 获取底边距引用
		/// </summary>
		/// <returns></returns>
		double& GetBottomRef();

		/// <summary>
		/// 设置左边距
		/// </summary>
		/// <param name="val">值</param>
		void SetLeft(double val);

		/// <summary>
		/// 设置上边距
		/// </summary>
		/// <param name="val">值</param>
		void SetTop(double val);

		/// <summary>
		/// 设置右边距
		/// </summary>
		/// <param name="val">值</param>
		void SetRight(double val);

		/// <summary>
		/// 设置底边距
		/// </summary>
		/// <param name="val">值</param>
		void SetBottom(double val);

		__uref bool operator == (const Thickness& thickness) const;

		__uref bool operator == (Thickness& thickness) const;

		__uref Thickness & operator = (const Thickness & thickness);

		__uref bool operator != (const Thickness& thickness) const;

		__uref bool operator != (Thickness& thickness) const;

		/// <summary>
		/// 左边距
		/// </summary>
		__uproperty(get = GetLeft, put = SetLeft) double Left;
		/// <summary>
		/// 上边距
		/// </summary>
		__uproperty(get = GetTop, put = SetTop) double Top;
		/// <summary>
		/// 右边距
		/// </summary>
		__uproperty(get = GetRight, put = SetRight) double Right;
		/// <summary>
		/// 底边距
		/// </summary>
		__uproperty(get = GetBottom, put = SetBottom) double Bottom;
		/// <summary>
		/// 左边距引用
		/// </summary>
		__uproperty(get = GetLeftRef) double& LeftRef;
		/// <summary>
		/// 上边距引用
		/// </summary>
		__uproperty(get = GetTopRef) double& TopRef;
		/// <summary>
		/// 右边距引用
		/// </summary>
		__uproperty(get = GetRightRef) double& RightRef;
		/// <summary>
		/// 底边距引用
		/// </summary>
		__uproperty(get = GetBottomRef) double& BottomRef;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Thickness& result);

		REFLECT_CLASS(Thickness)
	};

	class RUNTIME_API ThicknessConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<ThicknessConverter> _thisWeakPtr;
	protected:
		ThicknessConverter();
		ThicknessConverter(ThicknessConverter const&);
		void operator=(ThicknessConverter const&);
	public:
		virtual ~ThicknessConverter();

		static std::shared_ptr<ThicknessConverter> GetSharedInstance();

		static ThicknessConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};
}