#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>
#include "Point.h"
#include "Size.h"
#include <math.h>


#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

namespace System
{
	/// <summary>
	/// 矩阵类
	/// </summary>
	class RUNTIME_API Matrix final: __uobject
	{
	private:
		unsigned char flag;
		double _11;
		double _12;
		double _21;
		double _22;
		double _31;
		double _32;
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
	public:
		/// <summary>
		/// 创建<see cref="Matrix"/>实例.
		/// </summary>
		/// <param name="m11">M11</param>
		/// <param name="m12">M12</param>
		/// <param name="m21">M21</param>
		/// <param name="m22">M22</param>
		/// <param name="m31">M31</param>
		/// <param name="m32">M32</param>
		__uref Matrix(double m11, double m12, double m21, double m22, double m31, double m32);

		/// <summary>
		/// 创建<see cref="Matrix"/>实例.
		/// </summary>
		__uref Matrix();

		__uref Matrix(const Matrix& matrix);

		virtual ~Matrix();

		/// <summary>
		/// 获取单位矩阵
		/// </summary>
		__uref static Matrix Identity();

		/// <summary>
		/// 判断是否为单位矩阵
		/// </summary>
		/// <returns></returns>
		bool GetIsIdentity() const;

		/// <summary>
		/// 获取行列式值
		/// </summary>
		/// <returns></returns>
		double GetDeterminant() const;

		/// <summary>
		/// 判断是否可逆
		/// </summary>
		/// <returns></returns>
		bool GetHasInverse() const;

		/// <summary>
		/// 获取M11
		/// </summary>
		/// <returns></returns>
		double GetM11() const;

		/// <summary>
		/// 设置M11
		/// </summary>
		/// <param name="val">值</param>
		void SetM11(double val);

		/// <summary>
		/// 获取M12
		/// </summary>
		/// <returns></returns>
		double GetM12() const;

		/// <summary>
		/// 设置M12
		/// </summary>
		/// <param name="val">值</param>
		void SetM12(double val);

		/// <summary>
		/// 获取M21
		/// </summary>
		/// <returns></returns>
		double GetM21() const;

		/// <summary>
		/// 设置M21
		/// </summary>
		/// <param name="val">值</param>
		void SetM21(double val);

		/// <summary>
		/// 获取M22
		/// </summary>
		/// <returns></returns>
		double GetM22() const;

		/// <summary>
		/// 设置M22
		/// </summary>
		/// <param name="val">值</param>
		void SetM22(double val);

		/// <summary>
		/// 获取M31
		/// </summary>
		/// <returns></returns>
		double GetM31() const;

		/// <summary>
		/// 设置M31
		/// </summary>
		/// <param name="val">值</param>
		void SetM31(double val);

		/// <summary>
		/// 获取M32
		/// </summary>
		/// <returns></returns>
		double GetM32() const;

		/// <summary>
		/// 设置M32
		/// </summary>
		/// <param name="val">值</param>
		void SetM32(double val);

		/// <summary>
		/// 判断是否定性为单位矩阵
		/// </summary>
		/// <returns></returns>
		bool GetIsDistinguishedIdentity() const;

		__uref Matrix operator*(const Matrix &matrix) const;

		__uref bool operator == (const Matrix &matrix) const;

		__uref bool operator != (const Matrix &matrix) const;

		/// <summary>
		/// 设置为单位矩阵
		/// </summary>
		__uref void SetIdentity();

		/// <summary>
		/// 创建旋转矩阵
		/// </summary>
		/// <param name="angle">旋转角度</param>
		/// <param name="centerX">中心点X轴坐标值</param>
		/// <param name="centerY">中心点Y轴坐标值</param>
		/// <returns></returns>
		__uref static Matrix CreateRotationRadians(double angle, double centerX, double centerY);

		/// <summary>
		/// 创建旋转矩阵
		/// </summary>
		/// <param name="angle">旋转角度</param>
		/// <returns></returns>
		__uref static Matrix CreateRotationRadians(double angle);

		/// <summary>
		/// 创建缩放矩阵
		/// </summary>
		/// <param name="scaleX">缩放X轴比例</param>
		/// <param name="scaleY">缩放Y轴比例</param>
		/// <param name="centerX">中心点X轴坐标值</param>
		/// <param name="centerY">中心点Y轴坐标值</param>
		/// <returns></returns>
		__uref static Matrix CreateScaling(double scaleX, double scaleY, double centerX, double centerY);

		/// <summary>
		/// 创建缩放矩阵
		/// </summary>
		/// <param name="centerX">中心点X轴坐标值</param>
		/// <param name="centerY">中心点Y轴坐标值</param>
		/// <returns></returns>
		__uref static Matrix CreateScaling(double scaleX, double scaleY);

		/// <summary>
		/// 创建斜弧度矩阵
		/// </summary>
		/// <param name="skewX">倾斜X轴偏移值</param>
		/// <param name="skewY">倾斜Y轴偏移值</param>
		/// <returns></returns>
		__uref static Matrix CreateSkewRadians(double skewX, double skewY);

		/// <summary>
		/// 创建斜弧度矩阵
		/// </summary>
		/// <param name="skewX">倾斜X轴偏移值</param>
		/// <param name="skewY">倾斜Y轴偏移值</param>
		/// <param name="centerX">中心点X轴坐标值</param>
		/// <param name="centerY">中心点Y轴坐标值</param>
		/// <returns></returns>
		__uref static Matrix CreateSkewRadians(double skewX, double skewY, double centerX, double centerY);

		/// <summary>
		/// 创建偏移矩阵
		/// </summary>
		/// <param name="offsetX">X轴偏移值</param>
		/// <param name="offsetY">Y轴偏移值</param>
		/// <returns></returns>
		__uref static Matrix CreateTranslation(double offsetX, double offsetY);

		/// <summary>
		/// 与指定矩阵相乘
		/// </summary>
		/// <param name="matrix">指定矩阵</param>
		__uref void Multiply(const Matrix& matrix);

		/// <summary>
		/// 旋转指定角度
		/// </summary>
		/// <param name="angle">指定角度</param>
		__uref void Rotate(double angle);

		/// <summary>
		/// 已指定中心点旋转一定角度
		/// </summary>
		/// <param name="angle">指定角度</param>
		/// <param name="centerX">指定中心点X轴坐标值</param>
		/// <param name="centerY">指定中心点Y轴坐标值</param>
		__uref void RotateAt(double angle, double centerX, double centerY);

		/// <summary>
		/// 应用指定缩放值
		/// </summary>
		/// <param name="scaleX">X轴缩放值</param>
		/// <param name="scaleY">Y轴缩放值</param>
		__uref void Scale(double scaleX, double scaleY);

		/// <summary>
		/// 已指定中心点应用指定缩放值
		/// </summary>
		/// <param name="scaleX">X轴缩放值</param>
		/// <param name="scaleY">Y轴缩放值</param>
		/// <param name="centerX">指定中心点X轴坐标值</param>
		/// <param name="centerY">指定中心点Y轴坐标值</param>
		__uref void ScaleAt(double scaleX, double scaleY, double centerX, double centerY);

		/// <summary>
		/// 应用指定倾斜值
		/// </summary>
		/// <param name="skewX">X轴倾斜值</param>
		/// <param name="skewY">Y轴倾斜值</param>
		__uref void Skew(double skewX, double skewY);

		/// <summary>
		/// 应用指定偏移值
		/// </summary>
		/// <param name="offsetX">X轴偏移值</param>
		/// <param name="offsetY">Y轴偏移值</param>
		__uref void Translate(double offsetX, double offsetY);

		/// <summary>
		/// 与指定点相乘
		/// </summary>
		/// <param name="x">指定点X轴坐标值</param>
		/// <param name="y">指定点Y轴坐标值</param>
		__uref void MultiplyPoint(double& x, double& y) const;

		/// <summary>
		/// 以当前矩阵变换指定点
		/// </summary>
		/// <param name="point">指定点</param>
		/// <returns></returns>
		__uref Point Transform(const Point& point) const;

		/// <summary>
		/// 以当前矩阵变换指定点集合
		/// </summary>
		/// <param name="points">点集合.</param>
		/// <param name="pointCount">点个数</param>
		__uref void Transform(Point* points, size_t pointCount) const;

		/// <summary>
		/// 以当前矩阵变换指定点集合
		/// </summary>
		/// <param name="points">The points.</param>
		__uref void Transform(std::vector<Point>& points) const;

		/// <summary>
		/// 逆转矩阵
		/// </summary>
		__uref void Invert();

		/// <summary>
		/// M11
		/// </summary>
		__uproperty(get = GetM11, put = SetM11) double M11;
		/// <summary>
		/// M12
		/// </summary>
		__uproperty(get = GetM12, put = SetM12) double M12;
		/// <summary>
		/// M21
		/// </summary>
		__uproperty(get = GetM21, put = SetM21) double M21;
		/// <summary>
		/// M22
		/// </summary>
		__uproperty(get = GetM22, put = SetM22) double M22;
		/// <summary>
		/// M31
		/// </summary>
		__uproperty(get = GetM31, put = SetM31) double M31;
		/// <summary>
		/// M32
		/// </summary>
		__uproperty(get = GetM32, put = SetM32) double M32;
		/// <summary>
		/// 行列式值
		/// </summary>
		__uproperty(get = GetDeterminant) double Determinant;
		/// <summary>
		/// 是否为单位矩阵
		/// </summary>
		__uproperty(get = GetIsIdentity) bool IsIdentity;
		/// <summary>
		///  是否定性为单位矩阵
		/// </summary>
		__uproperty(get = GetIsDistinguishedIdentity) bool IsDistinguishedIdentity;
		/// <summary>
		/// 是否可逆
		/// </summary>
		__uproperty(get = GetHasInverse) bool HasInverse;
		
		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值.</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Matrix& result);

		REFLECT_CLASS(Matrix)
	};

	class RUNTIME_API MatrixConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<MatrixConverter> _thisWeakPtr;
	protected:
		MatrixConverter();
		MatrixConverter(MatrixConverter const&);
		void operator=(MatrixConverter const&);
	public:
		virtual ~MatrixConverter();

		static std::shared_ptr<MatrixConverter> GetSharedInstance();

		static MatrixConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};
}