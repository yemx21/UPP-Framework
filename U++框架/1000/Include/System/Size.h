#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>



namespace System
{
	/// <summary>
	/// Class Size.
	/// </summary>
	class RUNTIME_API Size : __uobject
	{
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		double w;
		double h;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Size"/> class.
		/// </summary>
		/// <param name="pw">The pw.</param>
		/// <param name="ph">The ph.</param>
		__uref Size(double pw, double ph);

		/// <summary>
		/// Initializes a new instance of the <see cref="Size"/> class.
		/// </summary>
		__uref Size();

		/// <summary>
		/// Finalizes an instance of the <see cref="Size"/> class.
		/// </summary>
		/// <returns>__uref.</returns>
		__uref ~Size();

		/// <summary>
		/// 获取宽度
		/// </summary>
		/// <returns></returns>
		double GetWidth() const;

		/// <summary>
		/// 获取高度
		/// </summary>
		/// <returns></returns>
		double GetHeight() const;

		/// <summary>
		/// 设置宽度
		/// </summary>
		/// <param name="val">值</param>
		void SetWidth(double val);

		/// <summary>
		/// 设置高度
		/// </summary>
		/// <param name="val">值</param>
		void SetHeight(double val);

		/// <summary>
		/// 获取宽度引用
		/// </summary>
		/// <returns></returns>

		double& GetWidthRef();

		/// <summary>
		/// 获取高度引用
		/// </summary>
		/// <returns></returns>

		double& GetHeightRef();

		/// <summary>
		/// 返回应用指定缩放值的新大小对象
		/// </summary>
		/// <param name="scale">指定缩放值</param>
		/// <returns>新大小对象</returns>
		__uref Size Scale(double scale) const;

		/// <summary>
		/// 应用指定缩放值
		/// </summary>
		/// <param name="scale">指定缩放值</param>
		/// <returns>__uref void.</returns>
		__uref void Scale(double scale);

		__uref Size(const Size& size);

		__uref Size operator + (const Size& size) const;

		__uref Size operator - (const Size& size) const;

		__uref Size& operator += (Size& size);

		__uref Size& operator -= (Size& size);

		__uref Size & operator = (const Size & size);

		__uref bool operator == (Size& size) const;

		__uref bool operator == (const Size& size) const;

		__uref bool operator != (Size& size) const;

		__uref bool operator != (const Size& size) const;

		/// <summary>
		/// 获取未知大小
		/// </summary>
		/// <returns></returns>
		__uref static Size Unknown();

		/// <summary>
		/// 获取无限大小
		/// </summary>
		/// <returns></returns>
		__uref static Size Infitite();

		/// <summary>
		/// 判断是否为空
		/// </summary>
		/// <returns></returns>
		bool GetIsUnknown() const;

		/// <summary>
		/// 判断是否宽高中至少有一个空值
		/// </summary>
		/// <returns></returns>
		bool GetIsOneDimensionalOrEmpty() const;

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
		__uproperty(get = GetWidthRef) double& WidthRef;
		/// <summary>
		/// 高度引用
		/// </summary>
		__uproperty(get = GetHeightRef) double& HeightRef;
		/// <summary>
		/// 是否未知
		/// </summary>
		__uproperty(get = GetIsUnknown) bool IsUnknown;
		/// <summary>
		/// 是否宽高至少为有一个空值
		/// </summary>
		__uproperty(get = GetIsOneDimensionalOrEmpty) bool IsOneDimensionalOrEmpty;

		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Size& result);

		REFLECT_CLASS(Size)
	};

	class RUNTIME_API SizeConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<SizeConverter> _thisWeakPtr;
	protected:
		SizeConverter();
		SizeConverter(SizeConverter const&);
		void operator=(SizeConverter const&);
	public:
		virtual ~SizeConverter();

		static std::shared_ptr<SizeConverter> GetSharedInstance();

		static SizeConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}