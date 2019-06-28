#include "Color.h"
#include "BuiltInAssembly.h"
#include "Formatter.h"
#include "StringHelper.h"
#include "Parser.h"
#include "Converter.h"
using namespace System;


namespace System
{
	class ColorParse
	{
	protected:
		friend class Color;
		friend class ColorConverter;

		static Color Parse(const std::wstring& input, const CultureInfo& loc);
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Color& result);
	};
}


__inline static float sRgbToScRgb(Byte bval)
{
	float val = ((float)bval / 255.0f);
	if (val < 0.0)
		return 0.0f;
	else if (val <= 0.04045)
		return (val / 12.92f);
	else if (val < 1.0f)
		return powf((val + 0.055f) / 1.055f, 2.4f);
	else
		return 1.0f;
}

__inline static Byte ScRgbTosRgb(float val)
{
	if (val < 0.0f)   return 0;
	else if (val <= 0.0031308f)
		return (Byte)((255.0f * val * 12.92f) + 0.5f);
	else if (val < 1.0f)
		return (Byte)((255.0f * ((1.055f *powf(val, 1.0f / 2.4f)) - 0.055f)) + 0.5f);
	else
		return 255;
}

Color::Color() :_R(0), _G(0), _B(0), _A(0), _ScR(0.0f), _ScG(0.0f), _ScB(0.0f), _ScA(0.0f), isFromSc(false) {}

Color::Color(const Color& color)
{
	_A = color._A;
	_R = color._R;
	_G = color._G;
	_B = color._B;
	_ScA = color._ScA;
	_ScR = color._ScR;
	_ScG = color._ScG;
	_ScB = color._ScB;
	isFromSc = color.isFromSc;
}

Color::Color(unsigned int argb) : isFromSc(false)
{
	_A = (Byte)((argb & 0xff000000) >> 24);
	_R = (Byte)((argb & 0x00ff0000) >> 16);
	_G = (Byte)((argb & 0x0000ff00) >> 8);
	_B = (Byte)(argb & 0x000000ff);
	_ScA = (float)_A / 255.0f;
	_ScR = sRgbToScRgb(_R);
	_ScG = sRgbToScRgb(_G);
	_ScB = sRgbToScRgb(_B);
}

Color::Color(Byte a, Byte r, Byte g, Byte b) : _A(a), _R(r), _G(g), _B(b), isFromSc(false)
{
	_ScA = (float)_A / 255.0f;
	_ScR = sRgbToScRgb(_R);
	_ScG = sRgbToScRgb(_G);
	_ScB = sRgbToScRgb(_B);
}

Color::Color(float a, float r, float g, float b) : _ScA(a), _ScR(r), _ScG(g), _ScB(b), isFromSc(true)
{
	_A = (Byte)((a * 255.0f) + 0.5f);
	_R = ScRgbTosRgb(_ScR);
	_G = ScRgbTosRgb(_ScG);
	_B = ScRgbTosRgb(_ScB);
}

Color::~Color()
{
}

Color& Color::operator = (const Color& color)
{
	_A = color._A;
	_B = color._B;
	_G = color._G;
	_R = color._R;
	_ScA = color._ScA;
	_ScR = color._ScR;
	_ScG = color._ScG;
	_ScB = color._ScB;
	isFromSc = color.isFromSc;
	return *this;
}

bool Color::operator == (const Color& color) const
{
	return  color._A == _A && color._B == _B && color._G == _G;
}

bool Color::operator != (const Color& color) const
{
	return  color._A != _A || color._B != _B || color._G != _G;
}

std::wstring Color::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	if (fmt)
	{
		if (isFromSc || wcscmp(fmt, L"R") == 0)
			return Formatter::Format(L"{0:r},{1:r},{2:r},{3:r}", loc, (VariadicAny(ScA), ScR, ScG, ScB));
		else
			return  Formatter::Format(L"#{0:X2}{1:X2}{2:X2}{3:X2}", loc, (VariadicAny(A), R, G, B));
	}
	return  Formatter::Format(L"#{0:X2}{1:X2}{2:X2}{3:X2}", loc, (VariadicAny(A), R, G, B));
}

Color Color::FromColors(const Colors& known)
{
	return Color(known);
}

Color Color::FromArgb(Byte a, Byte r, Byte g, Byte b)
{
	return Color(a, r, g, b);
}

Color Color::FromRgb(Byte r, Byte g, Byte b)
{
	return Color(255, r, g, b);
}

Color Color::FromScRgb(float r, float g, float b)
{
	return Color(1.0f, r, g, b);
}

Color Color::FromScArgb(float a, float r, float g, float b)
{
	return Color(a, r, g, b);
}

Byte Color::GetAlpha() const
{
	return _A;
}

float Color::GetScAlpha() const
{
	return _ScA;
}

Byte Color::GetRed() const
{
	return _R;
}

float Color::GetScRed() const
{
	return _ScR;
}

Byte Color::GetGreen() const
{
	return _G;
}

float Color::GetScGreen() const
{
	return _ScG;
}

Byte Color::GetBlue() const
{
	return _B;
}

float Color::GetScBlue() const
{
	return _ScB;
}

void Color::SetAlpha(Byte value)
{
	_A = value;
	_ScA = (float)_A / 255.0f;
}

void Color::SetScAlpha(float value)
{
	_ScA = value <0.0f ? 0.0f : (value>1.0f ? 1.0f : value);
	_A = (Byte)((value * 255.0f) + 0.5f);
}

void Color::SetRed(Byte value)
{
	_R = value;
	_ScR = sRgbToScRgb(_R);
}

void Color::SetScRed(float value)
{
	_ScR = value <0.0f ? 0.0f : (value>1.0f ? 1.0f : value);
	_R = ScRgbTosRgb(_ScR);
}

void Color::SetGreen(Byte value)
{
	_G = value;
	_ScG = sRgbToScRgb(_G);
}

void Color::SetScGreen(float value)
{
	_ScG = value <0.0f ? 0.0f : (value>1.0f ? 1.0f : value);
	_G = ScRgbTosRgb(_ScG);
}

void Color::SetBlue(Byte value)
{
	_B = value;
	_ScB = sRgbToScRgb(_B);
}

void Color::SetScBlue(float value)
{
	_ScB = value <0.0f ? 0.0f : (value>1.0f ? 1.0f : value);
	_B = ScRgbTosRgb(_ScB);
}

static const std::wstring Token_sc = L"sc#";

static std::wstring MatchColor(const std::wstring& colorString, bool& isNumericColor, bool& isScRgbColor)
{
	std::wstring str =  StringHelper::Trim(colorString);
	size_t len = str.length();
	if ((len == 4 || len == 5 || len == 7 || len == 9) && str[0] == '#')
	{
		isNumericColor = true;
		isScRgbColor = false;
		return str;
	}
	isNumericColor = false;
	if (!StringHelper::StartsWith(str, Token_sc, StringComparison::InvariantCultureIgnoreCase))
	{
		isScRgbColor = false;
	}
	else
	{
		isNumericColor = false;
		isScRgbColor = true;
	}
	isScRgbColor = false;
	return str;
}

__inline static int ParseHexChar(wchar_t c)
{
	int num = c;
	if (num >= 48 && num <= 57)
	{
		return num - 48;
	}
	if (num >= 97 && num <= 102)
	{
		return num - 97 + 10;
	}
	if (num < 65 || num > 70)
	{
		throw std::exception("illegal hex");
	}
	return num - 65 + 10;
}

static Color ParseHexColor(std::wstring trimmedColor)
{
	int num;
	int num1;
	int num2;
	int num3 = 255;
	size_t len = trimmedColor.length();
	if (len > 7)
	{
		num3 = ParseHexChar(trimmedColor[1]) * 16 + ParseHexChar(trimmedColor[2]);
		num2 = ParseHexChar(trimmedColor[3]) * 16 + ParseHexChar(trimmedColor[4]);
		num1 = ParseHexChar(trimmedColor[5]) * 16 + ParseHexChar(trimmedColor[6]);
		num = ParseHexChar(trimmedColor[7]) * 16 + ParseHexChar(trimmedColor[8]);
	}
	else if (len > 5)
	{
		num2 = ParseHexChar(trimmedColor[1]) * 16 + ParseHexChar(trimmedColor[2]);
		num1 = ParseHexChar(trimmedColor[3]) * 16 + ParseHexChar(trimmedColor[4]);
		num = ParseHexChar(trimmedColor[5]) * 16 + ParseHexChar(trimmedColor[6]);
	}
	else if (len <= 4)
	{
		num2 = ParseHexChar(trimmedColor[1]);
		num2 = num2 + num2 * 16;
		num1 = ParseHexChar(trimmedColor[2]);
		num1 = num1 + num1 * 16;
		num = ParseHexChar(trimmedColor[3]);
		num = num + num * 16;
	}
	else
	{
		num3 = ParseHexChar(trimmedColor[1]);
		num3 = num3 + num3 * 16;
		num2 = ParseHexChar(trimmedColor[2]);
		num2 = num2 + num2 * 16;
		num1 = ParseHexChar(trimmedColor[3]);
		num1 = num1 + num1 * 16;
		num = ParseHexChar(trimmedColor[4]);
		num = num + num * 16;
	}
	return Color::FromArgb((Byte)num3, (Byte)num2, (Byte)num1, (Byte)num);
}

static Color ParseScRgbColor(const std::wstring& trimmedColor, const CultureInfo& loc)
{
	std::wstring str = trimmedColor.substr(3);
	std::vector<std::wstring> substrs;
	StringHelper::Split(str, L',', substrs);
	size_t len = substrs.size();
	if (len == 3)
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		if (Parser::TryParseFloat32(substrs[0], r) && Parser::TryParseFloat32(substrs[1], g) && Parser::TryParseFloat32(substrs[2], b)) return Color::FromScRgb(r, g, b);
	}
	else if (len == 4)
	{
		float a = 0.0f;
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		if (Parser::TryParseFloat32(substrs[0], a) && Parser::TryParseFloat32(substrs[1], r) && Parser::TryParseFloat32(substrs[2], g) && Parser::TryParseFloat32(substrs[3], b)) return Color::FromScArgb(a, r, g, b);
	}
	throw std::exception("illegal expression");
	return Color();
}

static Color ColorStringToKnownColor(const std::wstring& colorString, bool& failed)
{
#pragma region static colors
	static Color Colors_AliceBlue = Color::Color(4293982463U);

	static Color Colors_AntiqueWhite = Color::Color(4294634455U);

	static Color Colors_Aqua = Color::Color(4278255615U);

	static Color Colors_Aquamarine = Color::Color(4286578644U);

	static Color Colors_Azure = Color::Color(4293984255U);

	static Color Colors_Beige = Color::Color(4294309340U);

	static Color Colors_Bisque = Color::Color(4294960324U);

	static Color Colors_Black = Color::Color(4278190080U);

	static Color Colors_BlanchedAlmond = Color::Color(4294962125U);

	static Color Colors_Blue = Color::Color(4278190335U);

	static Color Colors_BlueViolet = Color::Color(4287245282U);

	static Color Colors_Brown = Color::Color(4289014314U);

	static Color Colors_BurlyWood = Color::Color(4292786311U);

	static Color Colors_CadetBlue = Color::Color(4284456608U);

	static Color Colors_Chartreuse = Color::Color(4286578432U);

	static Color Colors_Chocolate = Color::Color(4291979550U);

	static Color Colors_Coral = Color::Color(4294934352U);

	static Color Colors_CornflowerBlue = Color::Color(4284782061U);

	static Color Colors_Cornsilk = Color::Color(4294965468U);

	static Color Colors_Crimson = Color::Color(4292613180U);

	static Color Colors_Cyan = Color::Color(4278255615U);

	static Color Colors_DarkBlue = Color::Color(4278190219U);

	static Color Colors_DarkCyan = Color::Color(4278225803U);

	static Color Colors_DarkGoldenrod = Color::Color(4290283019U);

	static Color Colors_DarkGray = Color::Color(4289309097U);

	static Color Colors_DarkGreen = Color::Color(4278215680U);

	static Color Colors_DarkKhaki = Color::Color(4290623339U);

	static Color Colors_DarkMagenta = Color::Color(4287299723U);

	static Color Colors_DarkOliveGreen = Color::Color(4283788079U);

	static Color Colors_DarkOrange = Color::Color(4294937600U);

	static Color Colors_DarkOrchid = Color::Color(4288230092U);

	static Color Colors_DarkRed = Color::Color(4287299584U);

	static Color Colors_DarkSalmon = Color::Color(4293498490U);

	static Color Colors_DarkSeaGreen = Color::Color(4287609999U);

	static Color Colors_DarkSlateBlue = Color::Color(4282924427U);

	static Color Colors_DarkSlateGray = Color::Color(4281290575U);

	static Color Colors_DarkTurquoise = Color::Color(4278243025U);

	static Color Colors_DarkViolet = Color::Color(4287889619U);

	static Color Colors_DeepPink = Color::Color(4294907027U);

	static Color Colors_DeepSkyBlue = Color::Color(4278239231U);

	static Color Colors_DimGray = Color::Color(4285098345U);

	static Color Colors_DodgerBlue = Color::Color(4280193279U);

	static Color Colors_Firebrick = Color::Color(4289864226U);

	static Color Colors_FloralWhite = Color::Color(4294966000U);

	static Color Colors_ForestGreen = Color::Color(4280453922U);

	static Color Colors_Fuchsia = Color::Color(4294902015U);

	static Color Colors_Gainsboro = Color::Color(4292664540U);

	static Color Colors_GhostWhite = Color::Color(4294506751U);

	static Color Colors_Gold = Color::Color(4294956800U);

	static Color Colors_Goldenrod = Color::Color(4292519200U);

	static Color Colors_Gray = Color::Color(4286611584U);

	static Color Colors_Green = Color::Color(4278222848U);

	static Color Colors_GreenYellow = Color::Color(4289593135U);

	static Color Colors_Honeydew = Color::Color(4293984240U);

	static Color Colors_HotPink = Color::Color(4294928820U);

	static Color Colors_IndianRed = Color::Color(4291648604U);

	static Color Colors_Indigo = Color::Color(4283105410U);

	static Color Colors_Ivory = Color::Color(4294967280U);

	static Color Colors_Khaki = Color::Color(4293977740U);

	static Color Colors_Lavender = Color::Color(4293322490U);

	static Color Colors_LavenderBlush = Color::Color(4294963445U);

	static Color Colors_LawnGreen = Color::Color(4286381056U);

	static Color Colors_LemonChiffon = Color::Color(4294965965U);

	static Color Colors_LightBlue = Color::Color(4289583334U);

	static Color Colors_LightCoral = Color::Color(4293951616U);

	static Color Colors_LightCyan = Color::Color(4292935679U);

	static Color Colors_LightGoldenrodYellow = Color::Color(4294638290U);

	static Color Colors_LightGray = Color::Color(4292072403U);

	static Color Colors_LightGreen = Color::Color(4287688336U);

	static Color Colors_LightPink = Color::Color(4294948545U);

	static Color Colors_LightSalmon = Color::Color(4294942842U);

	static Color Colors_LightSeaGreen = Color::Color(4280332970U);

	static Color Colors_LightSkyBlue = Color::Color(4287090426U);

	static Color Colors_LightSlateGray = Color::Color(4286023833U);

	static Color Colors_LightSteelBlue = Color::Color(4289774814U);

	static Color Colors_LightYellow = Color::Color(4294967264U);

	static Color Colors_Lime = Color::Color(4278255360U);

	static Color Colors_LimeGreen = Color::Color(4281519410U);

	static Color Colors_Linen = Color::Color(4294635750U);

	static Color Colors_Magenta = Color::Color(4294902015U);

	static Color Colors_Maroon = Color::Color(4286578688U);

	static Color Colors_MediumAquamarine = Color::Color(4284927402U);

	static Color Colors_MediumBlue = Color::Color(4278190285U);

	static Color Colors_MediumOrchid = Color::Color(4290401747U);

	static Color Colors_MediumPurple = Color::Color(4287852763U);

	static Color Colors_MediumSeaGreen = Color::Color(4282168177U);

	static Color Colors_MediumSlateBlue = Color::Color(4286277870U);

	static Color Colors_MediumSpringGreen = Color::Color(4278254234U);

	static Color Colors_MediumTurquoise = Color::Color(4282962380U);

	static Color Colors_MediumVioletRed = Color::Color(4291237253U);

	static Color Colors_MidnightBlue = Color::Color(4279834992U);

	static Color Colors_MintCream = Color::Color(4294311930U);

	static Color Colors_MistyRose = Color::Color(4294960309U);

	static Color Colors_Moccasin = Color::Color(4294958765U);

	static Color Colors_NavajoWhite = Color::Color(4294958765U);

	static Color Colors_Navy = Color::Color(4278190208U);

	static Color Colors_OldLace = Color::Color(4294833638U);

	static Color Colors_Olive = Color::Color(4286611456U);

	static Color Colors_OliveDrab = Color::Color(4285238819U);

	static Color Colors_Orange = Color::Color(4294944000U);

	static Color Colors_OrangeRed = Color::Color(4294919424U);

	static Color Colors_Orchid = Color::Color(4292505814U);

	static Color Colors_PaleGoldenrod = Color::Color(4293847210U);

	static Color Colors_PaleGreen = Color::Color(4288215960U);

	static Color Colors_PaleTurquoise = Color::Color(4289720046U);

	static Color Colors_PaleVioletRed = Color::Color(4292571283U);

	static Color Colors_PapayaWhip = Color::Color(4294963157U);

	static Color Colors_PeachPuff = Color::Color(4294957753U);

	static Color Colors_Peru = Color::Color(4291659071U);

	static Color Colors_Pink = Color::Color(4294951115U);

	static Color Colors_Plum = Color::Color(4292714717U);

	static Color Colors_PowderBlue = Color::Color(4289781990U);

	static Color Colors_Purple = Color::Color(4286578816U);

	static Color Colors_Red = Color::Color(4294901760U);

	static Color Colors_RosyBrown = Color::Color(4290547599U);

	static Color Colors_RoyalBlue = Color::Color(4282477025U);

	static Color Colors_SaddleBrown = Color::Color(4287317267U);

	static Color Colors_Salmon = Color::Color(4294606962U);

	static Color Colors_SandyBrown = Color::Color(4294222944U);

	static Color Colors_SeaGreen = Color::Color(4281240407U);

	static Color Colors_SeaShell = Color::Color(4294964718U);

	static Color Colors_Sienna = Color::Color(4288696877U);

	static Color Colors_Silver = Color::Color(4290822336U);

	static Color Colors_SkyBlue = Color::Color(4287090411U);

	static Color Colors_SlateBlue = Color::Color(4285160141U);

	static Color Colors_SlateGray = Color::Color(4285563024U);

	static Color Colors_Snow = Color::Color(4294966010U);

	static Color Colors_SpringGreen = Color::Color(4278255487U);

	static Color Colors_SteelBlue = Color::Color(4282811060U);

	static Color Colors_Tan = Color::Color(4291998860U);

	static Color Colors_Teal = Color::Color(4278222976U);

	static Color Colors_Thistle = Color::Color(4292394968U);

	static Color Colors_Tomato = Color::Color(4294927175U);

	static Color Colors_Transparent = Color::Color(16777215U);

	static Color Colors_Turquoise = Color::Color(4282441936U);

	static Color Colors_Violet = Color::Color(4293821166U);

	static Color Colors_Wheat = Color::Color(4294303411U);

	static Color Colors_White = Color::Color(4294967295U);

	static Color Colors_WhiteSmoke = Color::Color(4294309365U);

	static Color Colors_Yellow = Color::Color(4294967040U);

	static Color Colors_YellowGreen = Color::Color(4288335154U);

#pragma endregion

	failed = false;
	if (!colorString.empty())
	{
		std::wstring upper = StringHelper::ToUpper(colorString); 
		switch (upper.length())
		{
		case 3:
		{
				  if (upper == L"RED")
				  {
					  return Colors_Red;
				  }
				  if(upper!= L"TAN")
				  {
					  break;
				  }
				  return Colors_Tan;
		}
		case 4:
		{
				  wchar_t chr = upper[0];
				  switch (chr)
				  {
				  case 'A':
				  {
							  if (upper != L"AQUA")
							  {
								  break;
							  }
							  return Colors_Aqua;
				  }
				  case 'B':
				  {
							  if (upper != L"BLUE")
							  {
								  break;
							  }
							  return Colors_Blue;
				  }
				  case 'C':
				  {
							  if (upper != L"CYAN")
							  {
								  break;
							  }
							  return Colors_Aqua;
				  }
				  case 'D':
				  case 'E':
				  case 'F':
				  {
							  break;
				  }
				  case 'G':
				  {
							  if (upper == L"GOLD")
							  {
								  return Colors_Gold;
							  }
							  if (upper != L"GRAY")
							  {
								  break;
							  }
							  return Colors_Gray;
				  }
				  default:
				  {
							 switch (chr)
							 {
							 case 'L':
							 {
										 if (upper != L"LIME")
										 {
											 break;
										 }
										 return Colors_Lime;
							 }
							 case 'N':
							 {
										 if (upper != L"NAVY")
										 {
											 break;
										 }
										 return Colors_Navy;
							 }
							 case 'P':
							 {
										 if (upper == L"PERU")
										 {
											 return Colors_Peru;
										 }
										 if (upper == L"PINK")
										 {
											 return Colors_Pink;
										 }
										 if (upper != L"PLUM")
										 {
											 break;
										 }
										 return Colors_Plum;
							 }
							 case 'S':
							 {
										 if (upper != L"SNOW")
										 {
											 break;
										 }
										 return Colors_Snow;
							 }
							 case 'T':
							 {
										 if (upper != L"TEAL")
										 {
											 break;
										 }
										 return Colors_Teal;
							 }
							 }
							 break;
				  }
				  }
				  break;
		}
		case 5:
		{
				  wchar_t chr1 = upper[0];
				  switch (chr1)
				  {
				  case 'A':
				  {
							  if (upper != L"AZURE")
							  {
								  break;
							  }
							  return Colors_Azure;
				  }
				  case 'B':
				  {
							  if (upper == L"BEIGE")
							  {
								  return Colors_Beige;
							  }
							  if (upper == L"BLACK")
							  {
								  return Colors_Black;
							  }
							  if (upper != L"BROWN")
							  {
								  break;
							  }
							  return Colors_Brown;
				  }
				  case 'C':
				  {
							  if (upper != L"CORAL")
							  {
								  break;
							  }
							  return Colors_Coral;
				  }
				  case 'D':
				  case 'E':
				  case 'F':
				  case 'H':
				  case 'J':
				  case 'M':
				  case 'N':
				  {
							  break;
				  }
				  case 'G':
				  {
							  if (upper != L"GREEN")
							  {
								  break;
							  }
							  return Colors_Green;
				  }
				  case 'I':
				  {
							  if (upper != L"IVORY")
							  {
								  break;
							  }
							  return Colors_Ivory;
				  }
				  case 'K':
				  {
							  if (upper != L"KHAKI")
							  {
								  break;
							  }
							  return Colors_Khaki;
				  }
				  case 'L':
				  {
							  if (upper != L"LINEN")
							  {
								  break;
							  }
							  return Colors_Linen;
				  }
				  case 'O':
				  {
							  if (upper != L"OLIVE")
							  {
								  break;
							  }
							  return Colors_Olive;
				  }
				  default:
				  {
							 if (chr1 == 'W')
							 {
								 if (upper == L"WHEAT")
								 {
									 return Colors_Wheat;
								 }
								 if (upper != L"WHITE")
								 {
									 break;
								 }
								 return Colors_White;
							 }
							 break;
				  }
				  }
				  break;
		}
		case 6:
		{
				  wchar_t chr2 = upper[0];
				  if (chr2 == 'B')
				  {
					  if (upper != L"BISQUE")
					  {
						  break;
					  }
					  return Colors_Bisque;
				  }
				  else if (chr2 == 'I')
				  {
					  if (upper != L"INDIGO")
					  {
						  break;
					  }
					  return Colors_Indigo;
				  }
				  else
				  {
					  switch (chr2)
					  {
					  case 'M':
					  {
								  if (upper != L"MAROON")
								  {
									  break;
								  }
								  return Colors_Maroon;
					  }
					  case 'O':
					  {
								  if (upper == L"ORANGE")
								  {
									  return Colors_Orange;
								  }
								  if (upper != L"ORCHID")
								  {
									  break;
								  }
								  return Colors_Orchid;
					  }
					  case 'P':
					  {
								  if (upper != L"PURPLE")
								  {
									  break;
								  }
								  return Colors_Purple;
					  }
					  case 'S':
					  {
								  if (upper == L"SALMON")
								  {
									  return Colors_Salmon;
								  }
								  if (upper == L"SIENNA")
								  {
									  return Colors_Sienna;
								  }
								  if (upper != L"SILVER")
								  {
									  break;
								  }
								  return Colors_Silver;
					  }
					  case 'T':
					  {
								  if (upper != L"TOMATO")
								  {
									  break;
								  }
								  return Colors_Tomato;
					  }
					  case 'V':
					  {
								  if (upper != L"VIOLET")
								  {
									  break;
								  }
								  return Colors_Violet;
					  }
					  case 'Y':
					  {
								  if (upper != L"YELLOW")
								  {
									  break;
								  }
								  return Colors_Yellow;
					  }
					  }
				  }
				  break;
		}
		case 7:
		{
				  wchar_t chr3 = upper[0];
				  switch (chr3)
				  {
				  case 'C':
				  {
							  if (upper != L"CRIMSON")
							  {
								  break;
							  }
							  return Colors_Crimson;
				  }
				  case 'D':
				  {
							  if (upper == L"DARKRED")
							  {
								  return Colors_DarkRed;
							  }
							  if (upper != L"DIMGRAY")
							  {
								  break;
							  }
							  return Colors_DimGray;
				  }
				  case 'E':
				  case 'G':
				  {
							  break;
				  }
				  case 'F':
				  {
							  if (upper != L"FUCHSIA")
							  {
								  break;
							  }
							  return Colors_Fuchsia;
				  }
				  case 'H':
				  {
							  if (upper != L"HOTPINK")
							  {
								  break;
							  }
							  return Colors_HotPink;
				  }
				  default:
				  {
							 switch (chr3)
							 {
							 case 'M':
							 {
										 if (upper != L"MAGENTA")
										 {
											 break;
										 }
										 return Colors_Fuchsia;
							 }
							 case 'N':
							 {
										 break;
							 }
							 case 'O':
							 {
										 if (upper != L"OLDLACE")
										 {
											 break;
										 }
										 return Colors_OldLace;
							 }
							 default:
							 {
										switch (chr3)
										{
										case 'S':
										{
													if (upper != L"SKYBLUE")
													{
														break;
													}
													return Colors_SkyBlue;
										}
										case 'T':
										{
													if (upper != L"THISTLE")
													{
														break;
													}
													return Colors_Thistle;
										}
										}
										break;
							 }
							 }
							 break;
				  }
				  }
				  break;
		}
		case 8:
		{
				  wchar_t chr4 = upper[0];
				  if (chr4 > 'H')
				  {
					  switch (chr4)
					  {
					  case 'L':
					  {
								  if (upper != L"LAVENDER")
								  {
									  break;
								  }
								  return Colors_Lavender;
					  }
					  case 'M':
					  {
								  if (upper != L"MOCCASIN")
								  {
									  break;
								  }
								  return Colors_Moccasin;
					  }
					  default:
					  {
								 if (chr4 == 'S')
								 {
									 if (upper == L"SEAGREEN")
									 {
										 return Colors_SeaGreen;
									 }
									 if (upper != L"SEASHELL")
									 {
										 break;
									 }
									 return Colors_SeaShell;
								 }
								 break;
					  }
					  }
				  }
				  else
				  {
					  switch (chr4)
					  {
					  case 'C':
					  {
								  if (upper != L"CORNSILK")
								  {
									  break;
								  }
								  return Colors_Cornsilk;
					  }
					  case 'D':
					  {
								  if (upper == L"DARKBLUE")
								  {
									  return Colors_DarkBlue;
								  }
								  if (upper == L"DARKCYAN")
								  {
									  return Colors_DarkCyan;
								  }
								  if (upper == L"DARKGRAY")
								  {
									  return Colors_DarkGray;
								  }
								  if (upper != L"DEEPPINK")
								  {
									  break;
								  }
								  return Colors_DeepPink;
					  }
					  default:
					  {
								 if (chr4 == 'H')
								 {
									 if (upper != L"HONEYDEW")
									 {
										 break;
									 }
									 return Colors_Honeydew;
								 }
								 break;
					  }
					  }
				  }
				  break;
		}
		case 9:
		{
				  switch (upper[0])
				  {
				  case 'A':
				  {
							  if (upper != L"ALICEBLUE")
							  {
								  break;
							  }
							  return Colors_AliceBlue;
				  }
				  case 'B':
				  {
							  if (upper != L"BURLYWOOD")
							  {
								  break;
							  }
							  return Colors_BurlyWood;
				  }
				  case 'C':
				  {
							  if (upper == L"CADETBLUE")
							  {
								  return Colors_CadetBlue;
							  }
							  if (upper != L"CHOCOLATE")
							  {
								  break;
							  }
							  return Colors_Chocolate;
				  }
				  case 'D':
				  {
							  if (upper == L"DARKGREEN")
							  {
								  return Colors_DarkGreen;
							  }
							  if (upper != L"DARKKHAKI")
							  {
								  break;
							  }
							  return Colors_DarkKhaki;
				  }
				  case 'F':
				  {
							  if (upper != L"FIREBRICK")
							  {
								  break;
							  }
							  return Colors_Firebrick;
				  }
				  case 'G':
				  {
							  if (upper == L"GAINSBORO")
							  {
								  return Colors_Gainsboro;
							  }
							  if (upper != L"GOLDENROD")
							  {
								  break;
							  }
							  return Colors_Goldenrod;
				  }
				  case 'I':
				  {
							  if (upper != L"INDIANRED")
							  {
								  break;
							  }
							  return Colors_IndianRed;
				  }
				  case 'L':
				  {
							  if (upper == L"LAWNGREEN")
							  {
								  return Colors_LawnGreen;
							  }
							  if (upper == L"LIGHTBLUE")
							  {
								  return Colors_LightBlue;
							  }
							  if (upper == L"LIGHTCYAN")
							  {
								  return Colors_LightCyan;
							  }
							  if (upper == L"LIGHTGRAY")
							  {
								  return Colors_LightGray;
							  }
							  if (upper == L"LIGHTPINK")
							  {
								  return Colors_LightPink;
							  }
							  if (upper != L"LIMEGREEN")
							  {
								  break;
							  }
							  return Colors_LimeGreen;
				  }
				  case 'M':
				  {
							  if (upper == L"MINTCREAM")
							  {
								  return Colors_MintCream;
							  }
							  if (upper != L"MISTYROSE")
							  {
								  break;
							  }
							  return Colors_MistyRose;
				  }
				  case 'O':
				  {
							  if (upper == L"OLIVEDRAB")
							  {
								  return Colors_OliveDrab;
							  }
							  if (upper != L"ORANGERED")
							  {
								  break;
							  }
							  return Colors_OrangeRed;
				  }
				  case 'P':
				  {
							  if (upper == L"PALEGREEN")
							  {
								  return Colors_PaleGreen;
							  }
							  if (upper != L"PEACHPUFF")
							  {
								  break;
							  }
							  return Colors_PeachPuff;
				  }
				  case 'R':
				  {
							  if (upper == L"ROSYBROWN")
							  {
								  return Colors_RosyBrown;
							  }
							  if (upper != L"ROYALBLUE")
							  {
								  break;
							  }
							  return Colors_RoyalBlue;
				  }
				  case 'S':
				  {
							  if (upper == L"SLATEBLUE")
							  {
								  return Colors_SlateBlue;
							  }
							  if (upper == L"SLATEGRAY")
							  {
								  return Colors_SlateGray;
							  }
							  if (upper != L"STEELBLUE")
							  {
								  break;
							  }
							  return Colors_SteelBlue;
				  }
				  case 'T':
				  {
							  if (upper != L"TURQUOISE")
							  {
								  break;
							  }
							  return Colors_Turquoise;
				  }
				  }
				  break;
		}
		case 10:
		{
				   wchar_t chr5 = upper[0];
				   if (chr5 <= 'P')
				   {
					   switch (chr5)
					   {
					   case 'A':
					   {
								   if (upper != L"AQUAMARINE")
								   {
									   break;
								   }
								   return Colors_Aquamarine;
					   }
					   case 'B':
					   {
								   if (upper != L"BLUEVIOLET")
								   {
									   break;
								   }
								   return Colors_BlueViolet;
					   }
					   case 'C':
					   {
								   if (upper != L"CHARTREUSE")
								   {
									   break;
								   }
								   return Colors_Chartreuse;
					   }
					   case 'D':
					   {
								   if (upper == L"DARKORANGE")
								   {
									   return Colors_DarkOrange;
								   }
								   if (upper == L"DARKORCHID")
								   {
									   return Colors_DarkOrchid;
								   }
								   if (upper == L"DARKSALMON")
								   {
									   return Colors_DarkSalmon;
								   }
								   if (upper == L"DARKVIOLET")
								   {
									   return Colors_DarkViolet;
								   }
								   if (upper != L"DODGERBLUE")
								   {
									   break;
								   }
								   return Colors_DodgerBlue;
					   }
					   case 'E':
					   case 'F':
					   case 'H':
					   case 'I':
					   case 'J':
					   case 'K':
					   {
								   break;
					   }
					   case 'G':
					   {
								   if (upper != L"GHOSTWHITE")
								   {
									   break;
								   }
								   return Colors_GhostWhite;
					   }
					   case 'L':
					   {
								   if (upper == L"LIGHTCORAL")
								   {
									   return Colors_LightCoral;
								   }
								   if (upper != L"LIGHTGREEN")
								   {
									   break;
								   }
								   return Colors_LightGreen;
					   }
					   case 'M':
					   {
								   if (upper != L"MEDIUMBLUE")
								   {
									   break;
								   }
								   return Colors_MediumBlue;
					   }
					   default:
					   {
								  if (chr5 == 'P')
								  {
									  if (upper == L"PAPAYAWHIP")
									  {
										  return Colors_PapayaWhip;
									  }
									  if (upper != L"POWDERBLUE")
									  {
										  break;
									  }
									  return Colors_PowderBlue;
								  }
								  break;
					   }
					   }
				   }
				   else if (chr5 == 'S')
				   {
					   if (upper != L"SANDYBROWN")
					   {
						   break;
					   }
					   return Colors_SandyBrown;
				   }
				   else if (chr5 == 'W')
				   {
					   if (upper != L"WHITESMOKE")
					   {
						   break;
					   }
					   return Colors_WhiteSmoke;
				   }
				   else
				   {
					   break;
				   }
				   break;
		}
		case 11:
		{
				   wchar_t chr6 = upper[0];
				   if (chr6 > 'N')
				   {
					   switch (chr6)
					   {
					   case 'S':
					   {
								   if (upper == L"SADDLEBROWN")
								   {
									   return Colors_SaddleBrown;
								   }
								   if (upper != L"SPRINGGREEN")
								   {
									   break;
								   }
								   return Colors_SpringGreen;
					   }
					   case 'T':
					   {
								   if (upper != L"TRANSPARENT")
								   {
									   break;
								   }
								   return Colors_Transparent;
					   }
					   default:
					   {
								  if (chr6 == 'Y')
								  {
									  if (upper != L"YELLOWGREEN")
									  {
										  break;
									  }
									  return Colors_YellowGreen;
								  }
								  break;
					   }
					   }
				   }
				   else
				   {
					   switch (chr6)
					   {
					   case 'D':
					   {
								   if (upper == L"DARKMAGENTA")
								   {
									   return Colors_DarkMagenta;
								   }
								   if (upper != L"DEEPSKYBLUE")
								   {
									   break;
								   }
								   return Colors_DeepSkyBlue;
					   }
					   case 'E':
					   {
								   break;
					   }
					   case 'F':
					   {
								   if (upper == L"FLORALWHITE")
								   {
									   return Colors_FloralWhite;
								   }
								   if (upper != L"FORESTGREEN")
								   {
									   break;
								   }
								   return Colors_ForestGreen;
					   }
					   case 'G':
					   {
								   if (upper != L"GREENYELLOW")
								   {
									   break;
								   }
								   return Colors_GreenYellow;
					   }
					   default:
					   {
								  switch (chr6)
								  {
								  case 'L':
								  {
											  if (upper == L"LIGHTSALMON")
											  {
												  return Colors_LightSalmon;
											  }
											  if (upper != L"LIGHTYELLOW")
											  {
												  break;
											  }
											  return Colors_LightYellow;
								  }
								  case 'N':
								  {
											  if (upper != L"NAVAJOWHITE")
											  {
												  break;
											  }
											  return Colors_NavajoWhite;
								  }
								  }
								  break;
					   }
					   }
				   }
				   break;
		}
		case 12:
		{
				   wchar_t chr7 = upper[0];
				   if (chr7 == 'A')
				   {
					   if (upper != L"ANTIQUEWHITE")
					   {
						   break;
					   }
					   return Colors_AntiqueWhite;
				   }
				   else if (chr7 == 'D')
				   {
					   if (upper != L"DARKSEAGREEN")
					   {
						   break;
					   }
					   return Colors_DarkSeaGreen;
				   }
				   else
				   {
					   switch (chr7)
					   {
					   case 'L':
					   {
								   if (upper == L"LIGHTSKYBLUE")
								   {
									   return Colors_LightSkyBlue;
								   }
								   if (upper != L"LEMONCHIFFON")
								   {
									   break;
								   }
								   return Colors_LemonChiffon;
					   }
					   case 'M':
					   {
								   if (upper == L"MEDIUMORCHID")
								   {
									   return Colors_MediumOrchid;
								   }
								   if (upper == L"MEDIUMPURPLE")
								   {
									   return Colors_MediumPurple;
								   }
								   if (upper != L"MIDNIGHTBLUE")
								   {
									   break;
								   }
								   return Colors_MidnightBlue;
					   }
					   }
				   }
				   break;
		}
		case 13:
		{
				   wchar_t chr8 = upper[0];
				   if (chr8 == 'D')
				   {
					   if (upper == L"DARKSLATEBLUE")
					   {
						   return Colors_DarkSlateBlue;
					   }
					   if (upper == L"DARKSLATEGRAY")
					   {
						   return Colors_DarkSlateGray;
					   }
					   if (upper == L"DARKGOLDENROD")
					   {
						   return Colors_DarkGoldenrod;
					   }
					   if (upper != L"DARKTURQUOISE")
					   {
						   break;
					   }
					   return Colors_DarkTurquoise;
				   }
				   else if (chr8 == 'L')
				   {
					   if (upper == L"LIGHTSEAGREEN")
					   {
						   return Colors_LightSeaGreen;
					   }
					   if (upper != L"LAVENDERBLUSH")
					   {
						   break;
					   }
					   return Colors_LavenderBlush;
				   }
				   else if (chr8 == 'P')
				   {
					   if (upper == L"PALEGOLDENROD")
					   {
						   return Colors_PaleGoldenrod;
					   }
					   if (upper == L"PALETURQUOISE")
					   {
						   return Colors_PaleTurquoise;
					   }
					   if (upper != L"PALEVIOLETRED")
					   {
						   break;
					   }
					   return Colors_PaleVioletRed;
				   }
				   else
				   {
					   break;
				   }
		}
		case 14:
		{
				   wchar_t chr9 = upper[0];
				   switch (chr9)
				   {
				   case 'B':
				   {
							   if (upper != L"BLANCHEDALMOND")
							   {
								   break;
							   }
							   return Colors_BlanchedAlmond;
				   }
				   case 'C':
				   {
							   if (upper != L"CORNFLOWERBLUE")
							   {
								   break;
							   }
							   return Colors_CornflowerBlue;
				   }
				   case 'D':
				   {
							   if (upper != L"DARKOLIVEGREEN")
							   {
								   break;
							   }
							   return Colors_DarkOliveGreen;
				   }
				   default:
				   {
							  switch (chr9)
							  {
							  case 'L':
							  {
										  if (upper == L"LIGHTSLATEGRAY")
										  {
											  return Colors_LightSlateGray;
										  }
										  if (upper != L"LIGHTSTEELBLUE")
										  {
											  break;
										  }
										  return Colors_LightSteelBlue;
							  }
							  case 'M':
							  {
										  if (upper != L"MEDIUMSEAGREEN")
										  {
											  break;
										  }
										  return Colors_MediumSeaGreen;
							  }
							  }
							  break;
				   }
				   }
				   break;
		}
		case 15:
		{
				   if (upper == L"MEDIUMSLATEBLUE")
				   {
					   return Colors_MediumSlateBlue;
				   }
				   if (upper == L"MEDIUMTURQUOISE")
				   {
					   return Colors_MediumTurquoise;
				   }
				   if (upper != L"MEDIUMVIOLETRED")
				   {
					   break;
				   }
				   return Colors_MediumVioletRed;
		}
		case 16:
		{
				   if (upper != L"MEDIUMAQUAMARINE")
				   {
					   break;
				   }
				   return Colors_MediumAquamarine;
		}
		case 17:
		{
				   if (upper != L"MEDIUMSPRINGGREEN")
				   {
					   break;
				   }
				   return Colors_MediumSpringGreen;
		}
		case 20:
		{
				   if (upper != L"LIGHTGOLDENRODYELLOW")
				   {
					   break;
				   }
				   return Colors_LightGoldenrodYellow;
		}
		}
	}
	failed = true;
	return Color();
}

bool Color::TryParse(const std::wstring& input, const CultureInfo& loc, Color& result)
{
	return ColorParse::TryParse(input, loc, result);
}

Color ColorParse::Parse(const std::wstring& input, const CultureInfo& loc)
{
	bool isNum;
	bool isSc;
	std::wstring str = MatchColor(input, isNum, isSc);
	if (!isNum && !isSc)
	{
		throw std::exception("illegal expression");
	}
	if (isNum)
	{
		return ParseHexColor(str);
	}
	if (isSc)
	{
		return ParseScRgbColor(str, loc);
	}
	bool failed = false;
	Color knownColor = ColorStringToKnownColor(str, failed);
	if (failed)
	{
		throw std::exception("unknown color token");
	}
	return knownColor;
}

bool ColorParse::TryParse(const std::wstring& input, const CultureInfo& loc, Color& result)
{
	bool isNum;
	bool isSc;
	std::wstring str = MatchColor(input, isNum, isSc);
	if (!isNum && !isSc)
	{
		return false;
	}
	if (isNum)
	{
		try
		{
			result = ParseHexColor(str);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	if (isSc)
	{
		try
		{
			result = ParseScRgbColor(str, loc);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	bool failed = false;
	Color knownColor = ColorStringToKnownColor(str, failed);
	if (!failed)
	{
		result = knownColor;
		return true;
	}
	return false;

}

std::weak_ptr<ColorConverter> ColorConverter::_thisWeakPtr;
std::mutex& ColorConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

ColorConverter::ColorConverter() {}
ColorConverter::~ColorConverter() {}

std::shared_ptr<ColorConverter> ColorConverter::GetSharedInstance()
{
	std::shared_ptr<ColorConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new ColorConverter());
	_thisWeakPtr = p;
	return p;
}

ColorConverter& ColorConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool ColorConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Color))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool ColorConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(Color))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any ColorConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(Color)))
		{
			if (type->IsReference)
			{
				return Color(AnyVisitor<Color&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			Color* p = obj;
			if (p)
				return *p;
			else
			{
				Color res = obj.as<Color>();
				return res;
			}
		}
		else if (type->IsType(_typeof(Colors)))
		{
			if (type->IsReference)
			{
				return AnyVisitor<Colors&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Colors* p = obj;
			if (p)
				return *p;
			else
			{
				Color res = obj.as<Color>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				Color result = Color();
				if (Color::TryParse(std::wstring(tmp), loc, result))
				{
					return result;
				}
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any ColorConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
{
	if (!type)return _missing;
	_type otype = obj.Type;
	if (!otype) return _missing;
	try
	{
		bool iswiden = false;
		if (type->GetIsSTDString(iswiden))
		{
			if (iswiden)
			{
				if (otype->IsReference)
				{
					return AnyVisitor<Color&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				Color* p = obj;
				if (p)
					return p->ToString();
				else
				{
					Color v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(Color)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<Color&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			Color* p = obj;
			if (p)
				return *p;
			else
			{
				Color v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any ColorConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	Color result = Color();
	if (Color::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring ColorConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(Color)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<Color&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			Color* p = obj;
			if (p)
				return p->ToString();
			else
			{
				Color v = obj;
				return v.ToString();
			}
		}
		return std::wstring();
	}
	catch (...)
	{
		return std::wstring();
	}
}

Colors::operator Color() const
{
	return Color(value);
}

__inline static Object* Factory_Color_0() { return new Color(); }
__inline static Object* Factory_Color_1(unsigned int argb) { return new Color(argb); }
__inline static Object* Factory_Color_2(Byte a, Byte r, Byte g, Byte b) { return new Color(a, r, g, b); }
__inline static Object* Factory_Color_3(float a, float r, float g, float b) { return new Color(a, r, g, b); }


__uregister_class(Color, ClassFlags::Normal)
{
	type.AddDefaultConstructor<Color>(L"Color", ConstructorFlags::Default, &Factory_Color_0);
	type.AddConstructor(L"Color", ConstructorFlags::Default, ConstructorTypeAdapter<1, Color>::Create(&Factory_Color_1));
	type.AddConstructor(L"Color", ConstructorFlags::Default, ConstructorTypeAdapter<4, Color>::Create(&Factory_Color_2));
	type.AddConstructor(L"Color", ConstructorFlags::Default, ConstructorTypeAdapter<4, Color>::Create(&Factory_Color_3));

	type.AddMethod(L"FromColors", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<1, Colors, Color>::Create(&Color::FromColors));
	type.AddMethod(L"FromArgb", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<4, Color, Color>::Create(&Color::FromArgb));
	type.AddMethod(L"FromArgb", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<3, Color, Color>::Create(&Color::FromRgb));
	type.AddMethod(L"FromArgb", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<4, Color, Color>::Create(&Color::FromScArgb));
	type.AddMethod(L"FromArgb", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<3, Color, Color>::Create(&Color::FromScRgb));

	type.AddProperty(L"A", PropertyFlags::Public, PropertyTypeOf(&Color::GetAlpha, &Color::SetAlpha));
	type.AddProperty(L"R", PropertyFlags::Public, PropertyTypeOf(&Color::GetRed, &Color::SetRed));
	type.AddProperty(L"G", PropertyFlags::Public, PropertyTypeOf(&Color::GetGreen, &Color::SetGreen));
	type.AddProperty(L"B", PropertyFlags::Public, PropertyTypeOf(&Color::GetBlue, &Color::SetBlue));
	type.AddProperty(L"ScA", PropertyFlags::Public, PropertyTypeOf(&Color::GetScAlpha, &Color::SetScAlpha));
	type.AddProperty(L"ScR", PropertyFlags::Public, PropertyTypeOf(&Color::GetScRed, &Color::SetScRed));
	type.AddProperty(L"ScG", PropertyFlags::Public, PropertyTypeOf(&Color::GetScGreen, &Color::SetScGreen));
	type.AddProperty(L"ScB", PropertyFlags::Public, PropertyTypeOf(&Color::GetScBlue, &Color::SetScBlue));

	type.AddOperator(L"Assign", OperatorTypeAdapter<1, Operators::Assignment, Color, Color&>::Create(&Color::operator=));


	type.AddOperator(L"Equal", OperatorTypeAdapter<1, Operators::Equality, Color, bool>::Create(&Color::operator==));
	type.AddOperator(L"NotEqual", OperatorTypeAdapter<1, Operators::Inequality, Color, bool>::Create(&Color::operator!=));

	type.AddMethod(L"TryParse", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<3, Color, bool>::Create(&Color::TryParse));

	type.AddAttribute(new TypeConverterAttribute(L"ColorConverter", ColorConverter::GetSharedInstance()));

	BuiltInAssemblyFactory::Regisiter(&type);
};


__uregister_enum(Colors)
{
	type.AddValue(L"AliceBlue", Colors::AliceBlue, __uhashstr(System::Colors::AliceBlue));
	type.AddValue(L"AntiqueWhite", Colors::AntiqueWhite, __uhashstr(System::Colors::AntiqueWhite));
	type.AddValue(L"Aqua", Colors::Aqua, __uhashstr(System::Colors::Aqua));
	type.AddValue(L"Aquamarine", Colors::Aquamarine, __uhashstr(System::Colors::Aquamarine));
	type.AddValue(L"Azure", Colors::Azure, __uhashstr(System::Colors::Azure));
	type.AddValue(L"Beige", Colors::Beige, __uhashstr(System::Colors::Beige));
	type.AddValue(L"Bisque", Colors::Bisque, __uhashstr(System::Colors::Bisque));
	type.AddValue(L"Black", Colors::Black, __uhashstr(System::Colors::Black));
	type.AddValue(L"BlanchedAlmond", Colors::BlanchedAlmond, __uhashstr(System::Colors::BlanchedAlmond));
	type.AddValue(L"Blue", Colors::Blue, __uhashstr(System::Colors::Blue));
	type.AddValue(L"BlueViolet", Colors::BlueViolet, __uhashstr(System::Colors::BlueViolet));
	type.AddValue(L"Brown", Colors::Brown, __uhashstr(System::Colors::Brown));
	type.AddValue(L"BurlyWood", Colors::BurlyWood, __uhashstr(System::Colors::BurlyWood));
	type.AddValue(L"CadetBlue", Colors::CadetBlue, __uhashstr(System::Colors::CadetBlue));
	type.AddValue(L"Chartreuse", Colors::Chartreuse, __uhashstr(System::Colors::Chartreuse));
	type.AddValue(L"Chocolate", Colors::Chocolate, __uhashstr(System::Colors::Chocolate));
	type.AddValue(L"Coral", Colors::Coral, __uhashstr(System::Colors::Coral));
	type.AddValue(L"CornflowerBlue", Colors::CornflowerBlue, __uhashstr(System::Colors::CornflowerBlue));
	type.AddValue(L"Cornsilk", Colors::Cornsilk, __uhashstr(System::Colors::Cornsilk));
	type.AddValue(L"Crimson", Colors::Crimson, __uhashstr(System::Colors::Crimson));
	type.AddValue(L"Cyan", Colors::Cyan, __uhashstr(System::Colors::Cyan));
	type.AddValue(L"DarkBlue", Colors::DarkBlue, __uhashstr(System::Colors::DarkBlue));
	type.AddValue(L"DarkCyan", Colors::DarkCyan, __uhashstr(System::Colors::DarkCyan));
	type.AddValue(L"DarkGoldenrod", Colors::DarkGoldenrod, __uhashstr(System::Colors::DarkGoldenrod));
	type.AddValue(L"DarkGray", Colors::DarkGray, __uhashstr(System::Colors::DarkGray));
	type.AddValue(L"DarkGreen", Colors::DarkGreen, __uhashstr(System::Colors::DarkGreen));
	type.AddValue(L"DarkKhaki", Colors::DarkKhaki, __uhashstr(System::Colors::DarkKhaki));
	type.AddValue(L"DarkMagenta", Colors::DarkMagenta, __uhashstr(System::Colors::DarkMagenta));
	type.AddValue(L"DarkOliveGreen", Colors::DarkOliveGreen, __uhashstr(System::Colors::DarkOliveGreen));
	type.AddValue(L"DarkOrange", Colors::DarkOrange, __uhashstr(System::Colors::DarkOrange));
	type.AddValue(L"DarkOrchid", Colors::DarkOrchid, __uhashstr(System::Colors::DarkOrchid));
	type.AddValue(L"DarkRed", Colors::DarkRed, __uhashstr(System::Colors::DarkRed));
	type.AddValue(L"DarkSalmon", Colors::DarkSalmon, __uhashstr(System::Colors::DarkSalmon));
	type.AddValue(L"DarkSeaGreen", Colors::DarkSeaGreen, __uhashstr(System::Colors::DarkSeaGreen));
	type.AddValue(L"DarkSlateBlue", Colors::DarkSlateBlue, __uhashstr(System::Colors::DarkSlateBlue));
	type.AddValue(L"DarkSlateGray", Colors::DarkSlateGray, __uhashstr(System::Colors::DarkSlateGray));
	type.AddValue(L"DarkTurquoise", Colors::DarkTurquoise, __uhashstr(System::Colors::DarkTurquoise));
	type.AddValue(L"DarkViolet", Colors::DarkViolet, __uhashstr(System::Colors::DarkViolet));
	type.AddValue(L"DeepPink", Colors::DeepPink, __uhashstr(System::Colors::DeepPink));
	type.AddValue(L"DeepSkyBlue", Colors::DeepSkyBlue, __uhashstr(System::Colors::DeepSkyBlue));
	type.AddValue(L"DimGray", Colors::DimGray, __uhashstr(System::Colors::DimGray));
	type.AddValue(L"DodgerBlue", Colors::DodgerBlue, __uhashstr(System::Colors::DodgerBlue));
	type.AddValue(L"Firebrick", Colors::Firebrick, __uhashstr(System::Colors::Firebrick));
	type.AddValue(L"FloralWhite", Colors::FloralWhite, __uhashstr(System::Colors::FloralWhite));
	type.AddValue(L"ForestGreen", Colors::ForestGreen, __uhashstr(System::Colors::ForestGreen));
	type.AddValue(L"Fuchsia", Colors::Fuchsia, __uhashstr(System::Colors::Fuchsia));
	type.AddValue(L"Gainsboro", Colors::Gainsboro, __uhashstr(System::Colors::Gainsboro));
	type.AddValue(L"GhostWhite", Colors::GhostWhite, __uhashstr(System::Colors::GhostWhite));
	type.AddValue(L"Gold", Colors::Gold, __uhashstr(System::Colors::Gold));
	type.AddValue(L"Goldenrod", Colors::Goldenrod, __uhashstr(System::Colors::Goldenrod));
	type.AddValue(L"Gray", Colors::Gray, __uhashstr(System::Colors::Gray));
	type.AddValue(L"Green", Colors::Green, __uhashstr(System::Colors::Green));
	type.AddValue(L"GreenYellow", Colors::GreenYellow, __uhashstr(System::Colors::GreenYellow));
	type.AddValue(L"Honeydew", Colors::Honeydew, __uhashstr(System::Colors::Honeydew));
	type.AddValue(L"HotPink", Colors::HotPink, __uhashstr(System::Colors::HotPink));
	type.AddValue(L"IndianRed", Colors::IndianRed, __uhashstr(System::Colors::IndianRed));
	type.AddValue(L"Indigo", Colors::Indigo, __uhashstr(System::Colors::Indigo));
	type.AddValue(L"Ivory", Colors::Ivory, __uhashstr(System::Colors::Ivory));
	type.AddValue(L"Khaki", Colors::Khaki, __uhashstr(System::Colors::Khaki));
	type.AddValue(L"Lavender", Colors::Lavender, __uhashstr(System::Colors::Lavender));
	type.AddValue(L"LavenderBlush", Colors::LavenderBlush, __uhashstr(System::Colors::LavenderBlush));
	type.AddValue(L"LawnGreen", Colors::LawnGreen, __uhashstr(System::Colors::LawnGreen));
	type.AddValue(L"LemonChiffon", Colors::LemonChiffon, __uhashstr(System::Colors::LemonChiffon));
	type.AddValue(L"LightBlue", Colors::LightBlue, __uhashstr(System::Colors::LightBlue));
	type.AddValue(L"LightCoral", Colors::LightCoral, __uhashstr(System::Colors::LightCoral));
	type.AddValue(L"LightCyan", Colors::LightCyan, __uhashstr(System::Colors::LightCyan));
	type.AddValue(L"LightGoldenrodYellow", Colors::LightGoldenrodYellow, __uhashstr(System::Colors::LightGoldenrodYellow));
	type.AddValue(L"LightGray", Colors::LightGray, __uhashstr(System::Colors::LightGray));
	type.AddValue(L"LightGreen", Colors::LightGreen, __uhashstr(System::Colors::LightGreen));
	type.AddValue(L"LightPink", Colors::LightPink, __uhashstr(System::Colors::LightPink));
	type.AddValue(L"LightSalmon", Colors::LightSalmon, __uhashstr(System::Colors::LightSalmon));
	type.AddValue(L"LightSeaGreen", Colors::LightSeaGreen, __uhashstr(System::Colors::LightSeaGreen));
	type.AddValue(L"LightSkyBlue", Colors::LightSkyBlue, __uhashstr(System::Colors::LightSkyBlue));
	type.AddValue(L"LightSlateGray", Colors::LightSlateGray, __uhashstr(System::Colors::LightSlateGray));
	type.AddValue(L"LightSteelBlue", Colors::LightSteelBlue, __uhashstr(System::Colors::LightSteelBlue));
	type.AddValue(L"LightYellow", Colors::LightYellow, __uhashstr(System::Colors::LightYellow));
	type.AddValue(L"Lime", Colors::Lime, __uhashstr(System::Colors::Lime));
	type.AddValue(L"LimeGreen", Colors::LimeGreen, __uhashstr(System::Colors::LimeGreen));
	type.AddValue(L"Linen", Colors::Linen, __uhashstr(System::Colors::Linen));
	type.AddValue(L"Magenta", Colors::Magenta, __uhashstr(System::Colors::Magenta));
	type.AddValue(L"Maroon", Colors::Maroon, __uhashstr(System::Colors::Maroon));
	type.AddValue(L"MediumAquamarine", Colors::MediumAquamarine, __uhashstr(System::Colors::MediumAquamarine));
	type.AddValue(L"MediumBlue", Colors::MediumBlue, __uhashstr(System::Colors::MediumBlue));
	type.AddValue(L"MediumOrchid", Colors::MediumOrchid, __uhashstr(System::Colors::MediumOrchid));
	type.AddValue(L"MediumPurple", Colors::MediumPurple, __uhashstr(System::Colors::MediumPurple));
	type.AddValue(L"MediumSeaGreen", Colors::MediumSeaGreen, __uhashstr(System::Colors::MediumSeaGreen));
	type.AddValue(L"MediumSlateBlue", Colors::MediumSlateBlue, __uhashstr(System::Colors::MediumSlateBlue));
	type.AddValue(L"MediumSpringGreen", Colors::MediumSpringGreen, __uhashstr(System::Colors::MediumSpringGreen));
	type.AddValue(L"MediumTurquoise", Colors::MediumTurquoise, __uhashstr(System::Colors::MediumTurquoise));
	type.AddValue(L"MediumVioletRed", Colors::MediumVioletRed, __uhashstr(System::Colors::MediumVioletRed));
	type.AddValue(L"MidnightBlue", Colors::MidnightBlue, __uhashstr(System::Colors::MidnightBlue));
	type.AddValue(L"MintCream", Colors::MintCream, __uhashstr(System::Colors::MintCream));
	type.AddValue(L"MistyRose", Colors::MistyRose, __uhashstr(System::Colors::MistyRose));
	type.AddValue(L"Moccasin", Colors::Moccasin, __uhashstr(System::Colors::Moccasin));
	type.AddValue(L"NavajoWhite", Colors::NavajoWhite, __uhashstr(System::Colors::NavajoWhite));
	type.AddValue(L"Navy", Colors::Navy, __uhashstr(System::Colors::Navy));
	type.AddValue(L"OldLace", Colors::OldLace, __uhashstr(System::Colors::OldLace));
	type.AddValue(L"Olive", Colors::Olive, __uhashstr(System::Colors::Olive));
	type.AddValue(L"OliveDrab", Colors::OliveDrab, __uhashstr(System::Colors::OliveDrab));
	type.AddValue(L"Orange", Colors::Orange, __uhashstr(System::Colors::Orange));
	type.AddValue(L"OrangeRed", Colors::OrangeRed, __uhashstr(System::Colors::OrangeRed));
	type.AddValue(L"Orchid", Colors::Orchid, __uhashstr(System::Colors::Orchid));
	type.AddValue(L"PaleGoldenrod", Colors::PaleGoldenrod, __uhashstr(System::Colors::PaleGoldenrod));
	type.AddValue(L"PaleGreen", Colors::PaleGreen, __uhashstr(System::Colors::PaleGreen));
	type.AddValue(L"PaleTurquoise", Colors::PaleTurquoise, __uhashstr(System::Colors::PaleTurquoise));
	type.AddValue(L"PaleVioletRed", Colors::PaleVioletRed, __uhashstr(System::Colors::PaleVioletRed));
	type.AddValue(L"PapayaWhip", Colors::PapayaWhip, __uhashstr(System::Colors::PapayaWhip));
	type.AddValue(L"PeachPuff", Colors::PeachPuff, __uhashstr(System::Colors::PeachPuff));
	type.AddValue(L"Peru", Colors::Peru, __uhashstr(System::Colors::Peru));
	type.AddValue(L"Pink", Colors::Pink, __uhashstr(System::Colors::Pink));
	type.AddValue(L"Plum", Colors::Plum, __uhashstr(System::Colors::Plum));
	type.AddValue(L"PowderBlue", Colors::PowderBlue, __uhashstr(System::Colors::PowderBlue));
	type.AddValue(L"Purple", Colors::Purple, __uhashstr(System::Colors::Purple));
	type.AddValue(L"Red", Colors::Red, __uhashstr(System::Colors::Red));
	type.AddValue(L"RosyBrown", Colors::RosyBrown, __uhashstr(System::Colors::RosyBrown));
	type.AddValue(L"RoyalBlue", Colors::RoyalBlue, __uhashstr(System::Colors::RoyalBlue));
	type.AddValue(L"SaddleBrown", Colors::SaddleBrown, __uhashstr(System::Colors::SaddleBrown));
	type.AddValue(L"Salmon", Colors::Salmon, __uhashstr(System::Colors::Salmon));
	type.AddValue(L"SandyBrown", Colors::SandyBrown, __uhashstr(System::Colors::SandyBrown));
	type.AddValue(L"SeaGreen", Colors::SeaGreen, __uhashstr(System::Colors::SeaGreen));
	type.AddValue(L"SeaShell", Colors::SeaShell, __uhashstr(System::Colors::SeaShell));
	type.AddValue(L"Sienna", Colors::Sienna, __uhashstr(System::Colors::Sienna));
	type.AddValue(L"Silver", Colors::Silver, __uhashstr(System::Colors::Silver));
	type.AddValue(L"SkyBlue", Colors::SkyBlue, __uhashstr(System::Colors::SkyBlue));
	type.AddValue(L"SlateBlue", Colors::SlateBlue, __uhashstr(System::Colors::SlateBlue));
	type.AddValue(L"SlateGray", Colors::SlateGray, __uhashstr(System::Colors::SlateGray));
	type.AddValue(L"Snow", Colors::Snow, __uhashstr(System::Colors::Snow));
	type.AddValue(L"SpringGreen", Colors::SpringGreen, __uhashstr(System::Colors::SpringGreen));
	type.AddValue(L"SteelBlue", Colors::SteelBlue, __uhashstr(System::Colors::SteelBlue));
	type.AddValue(L"Tan", Colors::Tan, __uhashstr(System::Colors::Tan));
	type.AddValue(L"Teal", Colors::Teal, __uhashstr(System::Colors::Teal));
	type.AddValue(L"Thistle", Colors::Thistle, __uhashstr(System::Colors::Thistle));
	type.AddValue(L"Tomato", Colors::Tomato, __uhashstr(System::Colors::Tomato));
	type.AddValue(L"Transparent", Colors::Transparent, __uhashstr(System::Colors::Transparent));
	type.AddValue(L"Turquoise", Colors::Turquoise, __uhashstr(System::Colors::Turquoise));
	type.AddValue(L"Violet", Colors::Violet, __uhashstr(System::Colors::Violet));
	type.AddValue(L"Wheat", Colors::Wheat, __uhashstr(System::Colors::Wheat));
	type.AddValue(L"White", Colors::White, __uhashstr(System::Colors::White));
	type.AddValue(L"WhiteSmoke", Colors::WhiteSmoke, __uhashstr(System::Colors::WhiteSmoke));
	type.AddValue(L"Yellow", Colors::Yellow, __uhashstr(System::Colors::Yellow));
	type.AddValue(L"YellowGreen", Colors::YellowGreen, __uhashstr(System::Colors::YellowGreen));

	type.AddOperator(L"ConversionOperators", OperatorTypeAdapter<0, Operators::ConversionOperators, Colors, Color>::Create(&Colors::operator System::Color));

	BuiltInAssemblyFactory::Regisiter(&type);
};