#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>
namespace System
{
	/// <summary>
	/// 圆角半径类
	/// </summary>
	class RUNTIME_API CornerRadius : __uobject
	{
	protected:
		double bl;
		double br;
		double tl;
		double tr;
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
	public:
		/// <summary>
		/// 创建<see cref="CornerRadius"/>实例.
		/// </summary>
		__uref CornerRadius();

		virtual ~CornerRadius();

		/// <summary>
		/// 创建<see cref="CornerRadius"/>实例.
		/// </summary>
		/// <param name="topLeft">左上角半径</param>
		/// <param name="topRight">右上角半径</param>
		/// <param name="bottomRight">右下角半径</param>
		/// <param name="bottomLeft">左下角半径</param>
		__uref CornerRadius(double topLeft, double topRight, double bottomRight, double bottomLeft);

		/// <summary>
		/// 获取左上角半径
		/// </summary>
		/// <returns></returns>
		double GetTopLeft() const;

		/// <summary>
		/// 获取右下角半径
		/// </summary>
		/// <returns></returns>
		double GetTopRight() const;

		/// <summary>
		/// 获取左下角半径
		/// </summary>
		/// <returns></returns>
		double GetBottomLeft() const;

		/// <summary>
		/// 获取右下角半径
		/// </summary>
		/// <returns></returns>
		double GetBottomRight() const;

		/// <summary>
		/// 获取左上角半径引用
		/// </summary>
		/// <returns></returns>
		double& GetTopLeftRef();

		/// <summary>
		/// 获取右上角半径引用
		/// </summary>
		/// <returns></returns>
		double& GetTopRightRef();

		/// <summary>
		/// 获取左下角半径引用
		/// </summary>
		/// <returns></returns>
		double& GetBottomLeftRef();

		/// <summary>
		/// 获取右下角半径引用
		/// </summary>
		/// <returns></returns>
		double& GetBottomRightRef();

		/// <summary>
		/// 设置左上角半径
		/// </summary>
		/// <param name="val">半径值</param>
		void SetTopLeft(double val);

		/// <summary>
		/// 设置右上角半径
		/// </summary>
		/// <param name="val">半径值</param>
		void SetTopRight(double val);

		/// <summary>
		/// 设置左下角半径
		/// </summary>
		/// <param name="val">半径值</param>
		void SetBottomLeft(double val);

		/// <summary>
		/// 设置右下角半径
		/// </summary>
		/// <param name="val">半径值</param>
		void SetBottomRight(double val);

		__uref bool operator == (const CornerRadius& cornerRadius) const;

		__uref bool operator == (CornerRadius& cornerRadius) const;

		__uref CornerRadius & operator = (const CornerRadius & cornerRadius);

		__uref bool operator != (const CornerRadius& cornerRadius) const;

		__uref bool operator != (CornerRadius& cornerRadius) const;

		/// <summary>
		/// 左上角半径
		/// </summary>
		__uproperty(get = GetTopLeft, put = SetTopLeft) double TopLeft;
		/// <summary>
		/// 右上角半径
		/// </summary>
		__uproperty(get = GetTopRight, put = SetTopRight) double TopRight;
		/// <summary>
		/// 左下角半径
		/// </summary>
		__uproperty(get = GetBottomLeft, put = SetBottomLeft) double BottomLeft;
		/// <summary>
		/// 右下角半径
		/// </summary>
		__uproperty(get = GetBottomRight, put = SetBottomRight) double BottomRight;
		/// <summary>
		/// 左上角半径引用
		/// </summary>
		__uproperty(get = GetTopLeftRef) double& TopLeftRef;
		/// <summary>
		/// 右上角半径引用
		/// </summary>
		__uproperty(get = GetTopRightRef) double& TopRightRef;
		/// <summary>
		/// 左下角半径引用
		/// </summary>
		__uproperty(get = GetBottomLeftRef) double& BottomLeftRef;
		/// <summary>
		/// 右下角半径引用
		/// </summary>
		__uproperty(get = GetBottomRightRef) double& BottomRightRef;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值.</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, CornerRadius& result);

		REFLECT_CLASS(CornerRadius)

	};

	/// <summary>
	/// 圆角半径类转换器
	/// </summary>
	class RUNTIME_API CornerRadiusConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<CornerRadiusConverter> _thisWeakPtr;
	protected:
		CornerRadiusConverter();
		CornerRadiusConverter(CornerRadiusConverter const&);
		void operator=(CornerRadiusConverter const&);
	public:
		virtual ~CornerRadiusConverter();

		static std::shared_ptr<CornerRadiusConverter> GetSharedInstance();

		static CornerRadiusConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}