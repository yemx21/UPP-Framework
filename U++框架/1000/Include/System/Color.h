#pragma once
#include "Shared.h"
#include "TypeConverter.h"
#include <mutex>



namespace System
{
	class Color;

	/// <summary>
	/// 颜色集合枚举
	/// </summary>
	class RUNTIME_API Colors : __uobject
	{
		__uvalue_begin(Colors, unsigned int)

		__uvalue(AliceBlue, 4293982463)

		__uvalue(AntiqueWhite, 4294634455)

		__uvalue(Aqua, 4278255615)

		__uvalue(Aquamarine, 4286578644)

		__uvalue(Azure, 4293984255)

		__uvalue(Beige, 4294309340)

		__uvalue(Bisque, 4294960324)

		__uvalue(Black, 4278190080)

		__uvalue(BlanchedAlmond, 4294962125)

		__uvalue(Blue, 4278190335)

		__uvalue(BlueViolet, 4287245282)

		__uvalue(Brown, 4289014314)

		__uvalue(BurlyWood, 4292786311)

		__uvalue(CadetBlue, 4284456608)

		__uvalue(Chartreuse, 4286578432)

		__uvalue(Chocolate, 4291979550)

		__uvalue(Coral, 4294934352)

		__uvalue(CornflowerBlue, 4284782061)

		__uvalue(Cornsilk, 4294965468)

		__uvalue(Crimson, 4292613180)

		__uvalue(Cyan, 4278255615)

		__uvalue(DarkBlue, 4278190219)

		__uvalue(DarkCyan, 4278225803)

		__uvalue(DarkGoldenrod, 4290283019)

		__uvalue(DarkGray, 4289309097)

		__uvalue(DarkGreen, 4278215680)

		__uvalue(DarkKhaki, 4290623339)

		__uvalue(DarkMagenta, 4287299723)

		__uvalue(DarkOliveGreen, 4283788079)

		__uvalue(DarkOrange, 4294937600)

		__uvalue(DarkOrchid, 4288230092)

		__uvalue(DarkRed, 4287299584)

		__uvalue(DarkSalmon, 4293498490)

		__uvalue(DarkSeaGreen, 4287609999)

		__uvalue(DarkSlateBlue, 4282924427)

		__uvalue(DarkSlateGray, 4281290575)

		__uvalue(DarkTurquoise, 4278243025)

		__uvalue(DarkViolet, 4287889619)

		__uvalue(DeepPink, 4294907027)

		__uvalue(DeepSkyBlue, 4278239231)

		__uvalue(DimGray, 4285098345)

		__uvalue(DodgerBlue, 4280193279)

		__uvalue(Firebrick, 4289864226)

		__uvalue(FloralWhite, 4294966000)

		__uvalue(ForestGreen, 4280453922)

		__uvalue(Fuchsia, 4294902015)

		__uvalue(Gainsboro, 4292664540)

		__uvalue(GhostWhite, 4294506751)

		__uvalue(Gold, 4294956800)

		__uvalue(Goldenrod, 4292519200)

		__uvalue(Gray, 4286611584)

		__uvalue(Green, 4278222848)

		__uvalue(GreenYellow, 4289593135)

		__uvalue(Honeydew, 4293984240)

		__uvalue(HotPink, 4294928820)

		__uvalue(IndianRed, 4291648604)

		__uvalue(Indigo, 4283105410)

		__uvalue(Ivory, 4294967280)

		__uvalue(Khaki, 4293977740)

		__uvalue(Lavender, 4293322490)

		__uvalue(LavenderBlush, 4294963445)

		__uvalue(LawnGreen, 4286381056)

		__uvalue(LemonChiffon, 4294965965)

		__uvalue(LightBlue, 4289583334)

		__uvalue(LightCoral, 4293951616)

		__uvalue(LightCyan, 4292935679)

		__uvalue(LightGoldenrodYellow, 4294638290)

		__uvalue(LightGray, 4292072403)

		__uvalue(LightGreen, 4287688336)

		__uvalue(LightPink, 4294948545)

		__uvalue(LightSalmon, 4294942842)

		__uvalue(LightSeaGreen, 4280332970)

		__uvalue(LightSkyBlue, 4287090426)

		__uvalue(LightSlateGray, 4286023833)

		__uvalue(LightSteelBlue, 4289774814)

		__uvalue(LightYellow, 4294967264)

		__uvalue(Lime, 4278255360)

		__uvalue(LimeGreen, 4281519410)

		__uvalue(Linen, 4294635750)

		__uvalue(Magenta, 4294902015)

		__uvalue(Maroon, 4286578688)

		__uvalue(MediumAquamarine, 4284927402)

		__uvalue(MediumBlue, 4278190285)

		__uvalue(MediumOrchid, 4290401747)

		__uvalue(MediumPurple, 4287852763)

		__uvalue(MediumSeaGreen, 4282168177)

		__uvalue(MediumSlateBlue, 4286277870)

		__uvalue(MediumSpringGreen, 4278254234)

		__uvalue(MediumTurquoise, 4282962380)

		__uvalue(MediumVioletRed, 4291237253)

		__uvalue(MidnightBlue, 4279834992)

		__uvalue(MintCream, 4294311930)

		__uvalue(MistyRose, 4294960309)

		__uvalue(Moccasin, 4294958765)

		__uvalue(NavajoWhite, 4294958765)

		__uvalue(Navy, 4278190208)

		__uvalue(OldLace, 4294833638)

		__uvalue(Olive, 4286611456)

		__uvalue(OliveDrab, 4285238819)

		__uvalue(Orange, 4294944000)

		__uvalue(OrangeRed, 4294919424)

		__uvalue(Orchid, 4292505814)

		__uvalue(PaleGoldenrod, 4293847210)

		__uvalue(PaleGreen, 4288215960)

		__uvalue(PaleTurquoise, 4289720046)

		__uvalue(PaleVioletRed, 4292571283)

		__uvalue(PapayaWhip, 4294963157)

		__uvalue(PeachPuff, 4294957753)

		__uvalue(Peru, 4291659071)

		__uvalue(Pink, 4294951115)

		__uvalue(Plum, 4292714717)

		__uvalue(PowderBlue, 4289781990)

		__uvalue(Purple, 4286578816)

		__uvalue(Red, 4294901760)

		__uvalue(RosyBrown, 4290547599)

		__uvalue(RoyalBlue, 4282477025)

		__uvalue(SaddleBrown, 4287317267)

		__uvalue(Salmon, 4294606962)

		__uvalue(SandyBrown, 4294222944)

		__uvalue(SeaGreen, 4281240407)

		__uvalue(SeaShell, 4294964718)

		__uvalue(Sienna, 4288696877)

		__uvalue(Silver, 4290822336)

		__uvalue(SkyBlue, 4287090411)

		__uvalue(SlateBlue, 4285160141)

		__uvalue(SlateGray, 4285563024)

		__uvalue(Snow, 4294966010)

		__uvalue(SpringGreen, 4278255487)

		__uvalue(SteelBlue, 4282811060)

		__uvalue(Tan, 4291998860)

		__uvalue(Teal, 4278222976)

		__uvalue(Thistle, 4292394968)

		__uvalue(Tomato, 4294927175)

		__uvalue(Transparent, 16777215)

		__uvalue(Turquoise, 4282441936)

		__uvalue(Violet, 4293821166)

		__uvalue(Wheat, 4294303411)

		__uvalue(White, 4294967295)

		__uvalue(WhiteSmoke, 4294309365)

		__uvalue(Yellow, 4294967040)

		__uvalue(YellowGreen, 4288335154)

		__uvalue_end(Colors, Transparent)

		__uref operator Color() const;
	};



	/// <summary>
	/// 颜色类
	/// </summary>
	class RUNTIME_API Color : __uobject
	{
	protected:
		friend class Colors;
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;

		bool isFromSc;

		Byte _A;
		Byte _R;
		Byte _G;
		Byte _B;

		float _ScR;
		float _ScG;
		float _ScB;
		float _ScA;
		
	public:
		/// <summary>
		/// 创建 <see cref="Color"/>实例.
		/// </summary>
		__uref Color();

		__uref Color(const Color& color);

		/// <summary>
		/// 创建 <see cref="Color"/>实例.
		/// </summary>
		/// <param name="argb">ARGB整数值</param>
		__uref Color(unsigned int argb);

		/// <summary>
		/// 创建 <see cref="Color"/>实例.
		/// </summary>
		/// <param name="a">透明通道值（8位整数 [0-255]）</param>
		/// <param name="r">红色通道值（8位整数 [0-255]）</param>
		/// <param name="g">绿色通道值（8位整数 [0-255]）</param>
		/// <param name="b">蓝色通道值（8位整数 [0-255]）</param>
		__uref Color(Byte a, Byte r, Byte g, Byte b);

		/// <summary>
		/// 创建 <see cref="Color"/>实例.
		/// </summary>
		/// <param name="scA">透明色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scR">红色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scG">绿色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scB">蓝色通道值（浮点 [0.0-1.0]）</param>
		__uref Color(float scA, float scR, float scG, float scB);

		virtual ~Color();

		__uref Color& operator = (const Color& color);
	
		__uref bool operator == (const Color& color) const;
	
		__uref bool operator != (const Color& color) const;
		
		/// <summary>
		/// 创建<see cref="Color"/>实例.
		/// </summary>
		/// <param name="known">已知颜色</param>
		__uref static Color FromColors(const Colors& known);

		/// <summary>
		/// 创建<see cref="Color"/>实例.
		/// </summary>
		/// <param name="a">透明通道值（8位整数 [0-255]）</param>
		/// <param name="r">红色通道值（8位整数 [0-255]）</param>
		/// <param name="g">绿色通道值（8位整数 [0-255]）</param>
		/// <param name="b">蓝色通道值（8位整数 [0-255]）</param>
		__uref static Color FromArgb(Byte a, Byte r, Byte g, Byte b);

		/// <summary>
		/// 创建<see cref="Color"/>实例.
		/// </summary>
		/// <param name="r">红色通道值（8位整数 [0-255]）</param>
		/// <param name="g">绿色通道值（8位整数 [0-255]）</param>
		/// <param name="b">蓝色通道值（8位整数 [0-255]）</param>
		__uref static Color FromRgb(Byte r, Byte g, Byte b);

		/// <summary>
		/// 创建 <see cref="Color"/>实例.
		/// </summary>
		/// <param name="scR">红色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scG">绿色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scB">蓝色通道值（浮点 [0.0-1.0]）</param>
		__uref static Color FromScRgb(float r, float g, float b);

		/// <summary>
		/// 创建 <see cref="Color"/>实例.
		/// </summary>
		/// <param name="scA">透明色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scR">红色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scG">绿色通道值（浮点 [0.0-1.0]）</param>
		/// <param name="scB">蓝色通道值（浮点 [0.0-1.0]）</param>
		__uref static Color FromScArgb(float a, float r, float g, float b);

		/// <summary>
		/// 获取透明色通道值（8位整数 [0-255]）
		/// </summary>
		Byte GetAlpha() const;

		/// <summary>
		/// 获取透明色通道值（浮点 [0.0-1.0]）
		/// </summary>
		float GetScAlpha() const;

		/// <summary>
		/// 获取红色通道值（8位整数 [0-255]）
		/// </summary>
		Byte GetRed() const;

		/// <summary>
		/// 获取红色通道值（浮点 [0.0-1.0]）
		/// </summary>
		float GetScRed() const;

		/// <summary>
		/// 获取绿色通道值（8位整数 [0-255]）
		/// </summary>
		Byte GetGreen() const;

		/// <summary>
		/// 获取绿色通道值（浮点 [0.0-1.0]）
		/// </summary>
		float GetScGreen() const;

		/// <summary>
		/// 获取蓝色通道值（8位整数 [0-255]）
		/// </summary>
		Byte GetBlue() const;

		/// <summary>
		/// 获取蓝色通道值（浮点 [0.0-1.0]）
		/// </summary>
		float GetScBlue() const;

		/// <summary>
		/// 设置透明色通道值（8位整数 [0-255]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetAlpha(Byte value);

		/// <summary>
		/// 设置透明色通道值（浮点 [0.0-1.0]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetScAlpha(float value);

		/// <summary>
		/// 设置红色通道值（8位整数 [0-255]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetRed(Byte value);

		/// <summary>
		/// 设置红色通道值（浮点 [0.0-1.0]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetScRed(float value);

		/// <summary>
		/// 设置绿色通道值（8位整数 [0-255]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetGreen(Byte value);

		/// <summary>
		/// 设置绿色通道值（浮点 [0.0-1.0]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetScGreen(float value);

		/// <summary>
		/// 设置蓝色通道值（8位整数 [0-255]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetBlue(Byte value);

		/// <summary>
		/// 设置蓝色通道值（浮点 [0.0-1.0]）
		/// </summary>
		/// <param name="value">透明色通道值（浮点 [0.0-1.0]）</param>
		void SetScBlue(float value);


		/// <summary>
		/// 透明色通道值（8位整数 [0-255]）
		/// </summary>
		__uproperty(get = GetAlpha, put = SetAlpha) Byte A;
		/// <summary>
		/// 透明色通道值（浮点 [0.0-1.0]）
		/// </summary>
		__uproperty(get = GetScAlpha, put = SetScAlpha) float ScA;
		/// <summary>
		/// 红色通道值（8位整数 [0-255]）
		/// </summary>
		__uproperty(get = GetRed, put = SetRed) Byte R;
		/// <summary>
		/// 红色通道值（浮点 [0.0-1.0]）
		/// </summary>
		__uproperty(get = GetScRed, put = SetScRed) float ScR;
		/// <summary>
		/// 绿色通道值（8位整数 [0-255]）
		/// </summary>
		__uproperty(get = GetGreen, put = SetGreen) Byte G;
		/// <summary>
		/// 绿色通道值（浮点 [0.0-1.0]）
		/// </summary>
		__uproperty(get = GetScGreen, put = SetScGreen) float ScG;
		/// <summary>
		/// 蓝色通道值（8位整数 [0-255]）
		/// </summary>
		__uproperty(get = GetBlue, put = SetBlue) Byte B;
		/// <summary>
		///  蓝色通道值（浮点 [0.0-1.0]）
		/// </summary>
		__uproperty(get = GetScBlue, put = SetScBlue) float ScB;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值.</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		__uref static bool TryParse(const std::wstring& input, const CultureInfo& loc, Color& result);

		REFLECT_CLASS(Color)
	};

	/// <summary>
	/// 颜色类转换器
	/// </summary>
	class RUNTIME_API ColorConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<ColorConverter> _thisWeakPtr;
	protected:
		ColorConverter();
		ColorConverter(ColorConverter const&);
		void operator=(ColorConverter const&);
	public:
		virtual ~ColorConverter();

		static std::shared_ptr<ColorConverter> GetSharedInstance();

		static ColorConverter& GetInstance();

		bool CanConvertFrom(_type type) const override;
		bool CanConvertTo(_type type) const override;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const override;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const override;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const override;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const override;
	};

}