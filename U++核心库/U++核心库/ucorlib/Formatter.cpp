#include "Formatter.h"
#include "Converter.h"

using namespace System;

std::wregex BracketRegex = std::wregex(L"\\{(\\d+)?(:)?([^{}]*)?\\}", std::wregex::ECMAScript);
std::wregex OnlyCustomIgnoreBracketRegex = std::wregex(L"([^,;]*)?", std::wregex::ECMAScript);

Formatter::pos Formatter::ResolveNumberFormat(const wchar_t* format, MemberInfo& info, Formatter::pos begin, const CultureInfo& loc, FormatStyle style, Formatter::pos& found)
{
	found = npos;
	if (!format) return npos;
	std::wstring f(format);
	if (begin > f.size() - 1) 
		return npos;
	std::wstring::const_iterator current = f.begin() + begin;
	std::wstring::const_iterator end = f.end();
	std::wsmatch result;
	pos opos = begin;
	if (std::regex_search(current, end, result, style == FormatStyle::OnlyCustomIgnoreBracket ? OnlyCustomIgnoreBracketRegex : BracketRegex))
	{
		found = std::distance<std::wstring::const_iterator>(f.begin(), result.prefix().second);
		opos = std::distance<std::wstring::const_iterator>(f.begin(), result.suffix().first) - 1;
		std::wstring tmp = result[1].str();
		if (style != FormatStyle::OnlyCustomIgnoreBracket)
		{
			info.Index = !tmp.empty() ? stoi(tmp) : -1;
			tmp = result[3].str();
		}
		if (!tmp.empty())
		{
			switch (tmp[0])
			{
			case 'C':
			case 'c':
				info.NumberFormat = MemberNumberFormats::Currency;
				break;
			case 'D':
			case 'd':
				info.NumberFormat = MemberNumberFormats::Decimal;
				break;
			case 'E':
			case 'e':
				info.NumberFormat = MemberNumberFormats::Scientific;
				break;
			case 'F':
			case 'f':
				info.NumberFormat = MemberNumberFormats::FixedPoint;
				break;
			case 'G':
			case 'g':
				info.NumberFormat = MemberNumberFormats::General;
				break;
			case 'N':
			case 'n':
				info.NumberFormat = MemberNumberFormats::NumberWithCommasForThousands;
				break;
			case 'P':
			case 'p':
				info.NumberFormat = MemberNumberFormats::Percent;
				break;
			case 'R':
			case 'r':
				info.NumberFormat = MemberNumberFormats::RoundTrippable;
				break;
			case 'X':
			case 'x':
				info.NumberFormat = MemberNumberFormats::Hexadecimal;
				break;
			default:
				info.NumberFormat = MemberNumberFormats::Custom;
				break;
			}
			if (info.NumberFormat != MemberNumberFormats::Custom)
			{
				if (tmp.size() > 1)
				{
					tmp = tmp.substr(1, tmp.size() - 1);         //ºöÂÔ³ýÊý×ÖÒÔÍâµÄ×Ö·û£¬¼´ "{0,2:C²âÊÔ5abc2}"  ="{0,2:C52}"
					int itmp = stoi(tmp);
					if (itmp>=0 && itmp<99)
						info.NumberFormatPrecision = itmp;
					else
					{
						info.NumberFormatPrecision = 0;
						info.NumberFormat = MemberNumberFormats::Custom;
					}
				}
				else
				{
					switch (info.NumberFormat)
					{
					case MemberNumberFormats::Currency:
						info.NumberFormatPrecision = (unsigned int)loc.GetMoneyConst(CultureInfoMoneyConsts::Fraction);
						break;
					case MemberNumberFormats::FixedPoint:
					case MemberNumberFormats::NumberWithCommasForThousands:
						return info.NumberFormatPrecision=2;
					case MemberNumberFormats::FloatHexadecimal:
					case MemberNumberFormats::Scientific:
						return info.NumberFormatPrecision=6;
					case MemberNumberFormats::Unknown:
					case MemberNumberFormats::General:
					case MemberNumberFormats::Decimal:
					case MemberNumberFormats::Custom:
					case MemberNumberFormats::RoundTrippable:
					case MemberNumberFormats::Hexadecimal:
					default:
						info.NumberFormatPrecision = 0;
						break;
					}
				}
			}
			if (info.NumberFormat == MemberNumberFormats::Custom)
			{
				info.CustomNumberFormatString = style != FormatStyle::OnlyCustomIgnoreBracket ? result[3].str() : result[1].str();
			}
		}
		else
		{
			info.NumberFormat = MemberNumberFormats::Unknown;
		}
	}
	else
	{
		found = npos;
		opos = npos;
	}
	return opos;
}

inline std::wstring Formatter::_format(bool  _Val, const CultureInfo& loc)
{
	std::wstringstream _Ios = std::wstringstream();
	_Ios.imbue(loc.GetLocale());
	_Ios << std::boolalpha;
	_Ios <<  _Val;
	return _Ios.str();
}

inline std::wstring Formatter::_format(long  _Val, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	std::wstringstream _Ios = std::wstringstream();
	_Ios.imbue(group ? loc.GetLocale_Group() : loc.GetLocale());
	switch (flag)
	{
	case ToStringFlags::Scientific:
		_Ios << std::ios_base::scientific;
		break;
	case ToStringFlags::Hex:
		_Ios << std::ios_base::hex;
		break;
	case ToStringFlags::Normal:
	case ToStringFlags::Significant:
	default:
		break;
	}
	_Ios << _Val;
	return _Ios.str();
}

inline std::wstring Formatter::_format(unsigned long  _Val, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	std::wstringstream _Ios = std::wstringstream();
	_Ios.imbue(group ? loc.GetLocale_Group() : loc.GetLocale());
	switch (flag)
	{
	case ToStringFlags::Scientific:
		_Ios << std::ios_base::scientific;
		break;
	case ToStringFlags::Hex:
		_Ios << std::ios_base::hex;
		break;
	case ToStringFlags::Normal:
	case ToStringFlags::Significant:
	default:
		break;
	}
	_Ios << _Val;
	return _Ios.str();
}

inline std::wstring Formatter::_format(int  _Val, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	return _format((long)_Val, loc, flag, group);
}

inline std::wstring Formatter::_format(unsigned int  _Val, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	return _format((unsigned long)_Val, loc, flag, group);
}

inline std::wstring Formatter::_format(long long  _Val, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	std::wstringstream _Ios = std::wstringstream();
	_Ios.imbue(group ? loc.GetLocale_Group() : loc.GetLocale());
	switch (flag)
	{
	case ToStringFlags::Scientific:
		_Ios << std::ios_base::scientific;
		break;
	case ToStringFlags::Hex:
		_Ios << std::ios_base::hex;
		break;
	case ToStringFlags::Normal:
	case ToStringFlags::Significant:
	default:
		break;
	}
	_Ios << _Val;
	return _Ios.str();
}

inline std::wstring Formatter::_format(unsigned long long  _Val, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	std::wstringstream _Ios = std::wstringstream();
	_Ios.imbue(group ? loc.GetLocale_Group() : loc.GetLocale());
	switch (flag)
	{
	case ToStringFlags::Scientific:
		_Ios << std::ios_base::scientific;
		break;
	case ToStringFlags::Hex:
		_Ios << std::ios_base::hex;
		break;
	case ToStringFlags::Normal:
	case ToStringFlags::Significant:
	default:
		break;
	}
	_Ios << _Val;
	return _Ios.str();
}

inline std::wstring Formatter::_format(long double _Val, unsigned int precision, const CultureInfo& loc, ToStringFlags flag, bool group)
{
	std::wstringstream _Ios = std::wstringstream();
	_Ios.imbue(group ? loc.GetLocale_Group() : loc.GetLocale());
	switch (flag)
	{
	case ToStringFlags::Scientific:
		_Ios << std::ios_base::scientific;
		break;
	case ToStringFlags::Significant:
	case ToStringFlags::Normal:
		break;
	case ToStringFlags::Hex:
		_Ios << std::ios_base::hexfloat;
		break;
	case ToStringFlags::FloatingPoint:
	default:
		_Ios.setf(std::ios_base::fixed);
		break;
	}
	_Ios.precision(precision);
	
	_Ios << _Val;
	return _Ios.str();
}

__forceinline bool Int128_bit(const Int128_t& num, unsigned int n) throw ()
{
	n &= 0x7F;

	if (n < 64)
		return num.low & (1ull << n);
	else
		return num.high & (1ull << (n - 64));
}

__forceinline void Int128_bit(Int128_t& num, unsigned int n, bool val) throw ()
{
	n &= 0x7F;

	if (val) {
		if (n < 64) num.low |= (1ull << n);
		else num.high |= (1ull << (n - 64));
	}
	else {
		if (n < 64) num.low &= ~(1ull << n);
		else num.high &= ~(1ull << (n - 64));
	}
}

__inline Int128_t Int128_div(const Int128_t& num, const Int128_t & divisor, Int128_t & remainder) throw ()
{
	if (!divisor)
		return 1u / (unsigned int)divisor.low;

	Int128_t ds = (divisor.low < 0u) ? -divisor : divisor;
	Int128_t dd = (num.low < 0u) ? -num : num;

	if (ds > dd) {
		remainder = num;
		return 0ull;
	}

	Int128_t r = 0ull;
	Int128_t q = 0ull;

	unsigned int b = 127;
	while (r < ds) {
		r <<= 1;
		if (Int128_bit(dd, b--))
			r.low |= 1;
	}
	++b;

	while (true)
	if (r < ds) {
		if (!(b--)) break;

		r <<= 1;
		if (Int128_bit(dd, b))
			r.low |= 1;

	}
	else {
		r -= ds;
		Int128_bit(q, b, true);
	}

	if ((divisor.low < 0) ^ (num.low < 0)) q = -q;
	if (num.low < 0) r = -r;

	remainder = r;
	return q;
}

inline  std::wstring  Formatter::_format(const Int128& _Val, const CultureInfo& loc, bool isHex)
{
	std::wstring _Ios = std::wstring();
	if (!isHex)
	{
		_Ios += _format(_Val.high, loc, ToStringFlags::Hex);
		_Ios += _format(_Val.low, loc, ToStringFlags::Hex);
	}
	else
	{
		if (_Val.IsZero) return _Ios;
		Int128 r;
		Int128 ii = (_Val.IsNegative) ? -_Val : _Val;
		int i = 255;

		while (!!ii && i)
		{
			ii = Int128_div(ii, 10, r);
			_Ios.insert(0, 1, (wchar_t)(r.low + ((r.low > 9) ? L'A' - 10 : L'0')));
		};

		if (_Val.IsNegative)_Ios.insert(0, 1, L'-');

	}
	return _Ios;
}

__forceinline bool UInt128_bit(const UInt128_t& num, unsigned int n) throw ()
{
	n &= 0x7F;

	if (n < 64)
		return num.low & (1ull << n);
	else
		return num.high & (1ull << (n - 64));
}

__forceinline void UInt128_bit(UInt128_t& num, unsigned int n, bool val) throw ()
{
	n &= 0x7F;

	if (val) {
		if (n < 64) num.low |= (1ull << n);
		else num.high |= (1ull << (n - 64));
	}
	else {
		if (n < 64) num.low &= ~(1ull << n);
		else num.high &= ~(1ull << (n - 64));
	}
}

__inline UInt128_t UInt128_div(const UInt128_t& num, const UInt128_t & ds, UInt128_t & remainder) throw ()
{
	if (!ds)
		return 1u / (unsigned int)ds.low;

	UInt128_t dd = num;

	if (ds > dd) {
		remainder = num;
		return 0ull;
	}

	UInt128_t r = 0ull;
	UInt128_t q = 0ull;

	unsigned int b = 127;
	while (r < ds) {
		r <<= 1;
		if (UInt128_bit(dd, b--))
			r.low |= 1;
	}
	++b;

	while (true)
	if (r < ds) {
		if (!(b--)) break;

		r <<= 1;
		if (UInt128_bit(dd, b))
			r.low |= 1;

	}
	else {
		r -= ds;
		UInt128_bit(q, b, true);
	}

	remainder = r;
	return q;

}

inline  std::wstring  Formatter::_format(const UInt128& _Val, const CultureInfo& loc, bool isHex)
{
	std::wstring _Ios = std::wstring();
	if (!isHex)
	{
		_Ios += _format(_Val.high, loc, ToStringFlags::Hex);
		_Ios += _format(_Val.low, loc, ToStringFlags::Hex);
	}
	else
	{
		if (_Val.IsZero) return _Ios;
		UInt128 r;
		UInt128 ii = _Val;
		int i = 255;

		while (!!ii && i)
		{
			ii = UInt128_div(ii, 10U, r);
			_Ios.insert(0, 1, (wchar_t)(r.low + ((r.low > 9) ? L'A' - 10 : L'0')));
		};

	}
	return _Ios;
}

void Formatter::_format(bool value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	result.append(_format(value, loc));
}

void Formatter::_format(Float32 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
		{
			std::wstring tmp = _format(abs(value), info.NumberFormatPrecision, loc);
			size_t n = info.NumberFormatPrecision != 0 ? tmp.find('.') : tmp.size();
			size_t m = n % 3;
			size_t i = 0;
			if (tmp.size())
			{
				auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
				auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
				if (value > 0.0f)
				{
					auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
					auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
					auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
					if (p_cs_precedes != 0)
					{
						if (p_sign_posn == 0)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (p_sign_posn == 1 || p_sign_posn == 3)
						{
							result.push_back('+');
							if (p_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (p_sign_posn == 2)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (p_sign_posn == 4)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
							if (p_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
					}
					else
					{
						if (p_sign_posn == 0)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space != 0) result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (p_sign_posn == 1)
						{
							result.push_back('+');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space != 0)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (p_sign_posn == 2 || p_sign_posn == 4)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 1)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (p_sign_posn == 3)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 1)result.push_back(' ');
							result.push_back('+');
							if (p_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
					}
				}
				else
				{
					auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
					auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
					auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
					if (n_cs_precedes != 0)
					{
						if (n_sign_posn == 0)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (n_sign_posn == 1 || n_sign_posn == 3)
						{
							result.push_back('+');
							if (n_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (n_sign_posn == 2)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (n_sign_posn == 4)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
							if (n_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
					}
					else
					{
						if (n_sign_posn == 0)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space != 0) result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (n_sign_posn == 1)
						{
							result.push_back('+');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space != 0)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (n_sign_posn == 2 || n_sign_posn == 4)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 1)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (n_sign_posn == 3)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 1)result.push_back(' ');
							result.push_back('+');
							if (n_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
					}
				}
			}
		}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format((long long)value, 0, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		float v = abs(value);
		int n = v == 0 ? 1 : (int)log10(v) + 1;
		int p = info.NumberFormatPrecision == 0 ? 7 : info.NumberFormatPrecision>7 ? 7 : info.NumberFormatPrecision;
		std::wstring tmp;
		if (p >= n)
		{
			p -= n;
			tmp = _format(value, p, loc);
		}
		else
		{
			tmp = _format(value, p, loc, ToStringFlags::Significant);
		}
		result.append(tmp);
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::FloatingPoint, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, info.NumberFormatPrecision, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		float v = abs(value);
		int n = v == 0 ? 1 : (int)log10(v) + 1;
		int p = 9;
		std::wstring tmp;
		if (p >= n)
		{
			p -= n;
			tmp = _format(value, p, loc);
		}
		else
		{
			tmp = _format(value, p, loc, ToStringFlags::Significant);
		}
		result.append(tmp);
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, 2, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(Float64 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		std::wstring tmp = _format(abs(value), info.NumberFormatPrecision, loc);
		size_t n = info.NumberFormatPrecision != 0 ? tmp.find('.') : tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			if (value > 0.0f)
			{
				auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
				auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
				auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
				if (p_cs_precedes != 0)
				{
					if (p_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 1 || p_sign_posn == 3)
					{
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (p_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 2 || p_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
			else
			{
				auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
				auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
				auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
				if (n_cs_precedes != 0)
				{
					if (n_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 1 || n_sign_posn == 3)
					{
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (n_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 2 || n_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format((long long)value, 0, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		double v = abs(value);
		int n = v == 0 ? 1 : (int)log10(v) + 1;
		int p = info.NumberFormatPrecision == 0 ? 7 : info.NumberFormatPrecision>7 ? 7 : info.NumberFormatPrecision;
		std::wstring tmp;
		if (p >= n)
		{
			p -= n;
			tmp = _format(value, p, loc);
		}
		else
		{
			tmp = _format(value, p, loc, ToStringFlags::Significant);
		}
		result.append(tmp);
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::FloatingPoint, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, info.NumberFormatPrecision, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		double v = abs(value);
		int n = v == 0 ? 1 : (int)log10(v) + 1;
		int p = 9;
		std::wstring tmp;
		if (p >= n)
		{
			p -= n;
			tmp = _format(value, p, loc);
		}
		else
		{
			tmp = _format(value, p, loc, ToStringFlags::Significant);
		}
		result.append(tmp);
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, 2, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(Float128 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		std::wstring tmp = _format(abs(value), info.NumberFormatPrecision, loc);
		size_t n = info.NumberFormatPrecision != 0 ? tmp.find('.') : tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			if (value > 0.0f)
			{
				auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
				auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
				auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
				if (p_cs_precedes != 0)
				{
					if (p_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 1 || p_sign_posn == 3)
					{
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (p_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 2 || p_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
			else
			{
				auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
				auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
				auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
				if (n_cs_precedes != 0)
				{
					if (n_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 1 || n_sign_posn == 3)
					{
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (n_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 2 || n_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format((long long)value, 0, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		long double v = abs(value);
		int n = v == 0 ? 1 : (int)log10(v) + 1;
		int p = info.NumberFormatPrecision == 0 ? 7 : info.NumberFormatPrecision>7 ? 7 : info.NumberFormatPrecision;
		std::wstring tmp;
		if (p >= n)
		{
			p -= n;
			tmp = _format(value, p, loc);
		}
		else
		{
			tmp = _format(value, p, loc, ToStringFlags::Significant);
		}
		result.append(tmp);
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::FloatingPoint, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, info.NumberFormatPrecision, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, info.NumberFormatPrecision, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		long double v = abs(value);
		int n = v == 0 ? 1 : (int)log10(v) + 1;
		int p = 9;
		std::wstring tmp;
		if (p >= n)
		{
			p -= n;
			tmp = _format(value, p, loc);
		}
		else
		{
			tmp = _format(value, p, loc, ToStringFlags::Significant);
		}
		result.append(tmp);
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, 2, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}


void Formatter::_format(Int8 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	_format((Int32)value, info, result, loc);
}

void Formatter::_format(UInt8 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	_format((Int32)value, info, result, loc);
}

void Formatter::_format(Int16 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	_format((Int32)value, info, result, loc);
}

void Formatter::_format(UInt16 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	_format((Int32)value, info, result, loc);
}

void Formatter::_format(const Int24& val, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	Int32 value = val;
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		
		std::wstring tmp = _format(abs(value), loc);
		size_t n = tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			if (value > 0.0f)
			{
				auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
				auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
				auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
				if (p_cs_precedes != 0)
				{
					if (p_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 1 || p_sign_posn == 3)
					{
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (p_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 2 || p_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
			else
			{
				auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
				auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
				auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
				if (n_cs_precedes != 0)
				{
					if (n_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 1 || n_sign_posn == 3)
					{
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (n_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 2 || n_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, loc,ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(const UInt24& val, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	Int32 value = val;
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		
		std::wstring tmp = _format(value, loc);
		size_t n = tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
			auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
			auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
			if (p_cs_precedes != 0)
			{
				if (p_sign_posn == 0)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1) result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
				else if (p_sign_posn == 1 || p_sign_posn == 3)
				{
					result.push_back('+');
					if (p_sep_by_space == 2)result.push_back(' ');
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1) result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
				else if (p_sign_posn == 2)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1)result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
				}
				else if (p_sign_posn == 4)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
					if (p_sep_by_space == 1)result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
			}
			else
			{
				if (p_sign_posn == 0)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space != 0) result.push_back(' ');
					result.append(_W_currency_symbol);
				}
				else if (p_sign_posn == 1)
				{
					result.push_back('+');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space != 0)result.push_back(' ');
					result.append(_W_currency_symbol);
				}
				else if (p_sign_posn == 2 || p_sign_posn == 4)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 1)result.push_back(' ');
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
				}
				else if (p_sign_posn == 3)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 1)result.push_back(' ');
					result.push_back('+');
					if (p_sep_by_space == 2)result.push_back(' ');
					result.append(_W_currency_symbol);
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(Int32 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
										  
										  std::wstring tmp = _format(abs(value), loc);
										  size_t n = tmp.size();
										  size_t m = n % 3;
										  size_t i = 0;
										  if (tmp.size())
										  {
											  auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
											  auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
											  if (value > 0.0f)
											  {
												  auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
												  auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
												  auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
												  if (p_cs_precedes != 0)
												  {
													  if (p_sign_posn == 0)
													  {
														  result.append(_W_currency_symbol);
														  if (p_sep_by_space == 1) result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
													  }
													  else if (p_sign_posn == 1 || p_sign_posn == 3)
													  {
														  result.push_back('+');
														  if (p_sep_by_space == 2)result.push_back(' ');
														  result.append(_W_currency_symbol);
														  if (p_sep_by_space == 1) result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
													  }
													  else if (p_sign_posn == 2)
													  {
														  result.append(_W_currency_symbol);
														  if (p_sep_by_space == 1)result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (p_sep_by_space == 2)result.push_back(' ');
														  result.push_back('+');
													  }
													  else if (p_sign_posn == 4)
													  {
														  result.append(_W_currency_symbol);
														  if (p_sep_by_space == 2)result.push_back(' ');
														  result.push_back('+');
														  if (p_sep_by_space == 1)result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
													  }
												  }
												  else
												  {
													  if (p_sign_posn == 0)
													  {
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (p_sep_by_space != 0) result.push_back(' ');
														  result.append(_W_currency_symbol);
													  }
													  else if (p_sign_posn == 1)
													  {
														  result.push_back('+');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (p_sep_by_space != 0)result.push_back(' ');
														  result.append(_W_currency_symbol);
													  }
													  else if (p_sign_posn == 2 || p_sign_posn == 4)
													  {
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (p_sep_by_space == 1)result.push_back(' ');
														  result.append(_W_currency_symbol);
														  if (p_sep_by_space == 2)result.push_back(' ');
														  result.push_back('+');
													  }
													  else if (p_sign_posn == 3)
													  {
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (p_sep_by_space == 1)result.push_back(' ');
														  result.push_back('+');
														  if (p_sep_by_space == 2)result.push_back(' ');
														  result.append(_W_currency_symbol);
													  }
												  }
											  }
											  else
											  {
												  auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
												  auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
												  auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
												  if (n_cs_precedes != 0)
												  {
													  if (n_sign_posn == 0)
													  {
														  result.append(_W_currency_symbol);
														  if (n_sep_by_space == 1) result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
													  }
													  else if (n_sign_posn == 1 || n_sign_posn == 3)
													  {
														  result.push_back('+');
														  if (n_sep_by_space == 2)result.push_back(' ');
														  result.append(_W_currency_symbol);
														  if (n_sep_by_space == 1) result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
													  }
													  else if (n_sign_posn == 2)
													  {
														  result.append(_W_currency_symbol);
														  if (n_sep_by_space == 1)result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (n_sep_by_space == 2)result.push_back(' ');
														  result.push_back('+');
													  }
													  else if (n_sign_posn == 4)
													  {
														  result.append(_W_currency_symbol);
														  if (n_sep_by_space == 2)result.push_back(' ');
														  result.push_back('+');
														  if (n_sep_by_space == 1)result.push_back(' ');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
													  }
												  }
												  else
												  {
													  if (n_sign_posn == 0)
													  {
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (n_sep_by_space != 0) result.push_back(' ');
														  result.append(_W_currency_symbol);
													  }
													  else if (n_sign_posn == 1)
													  {
														  result.push_back('+');
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (n_sep_by_space != 0)result.push_back(' ');
														  result.append(_W_currency_symbol);
													  }
													  else if (n_sign_posn == 2 || n_sign_posn == 4)
													  {
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (n_sep_by_space == 1)result.push_back(' ');
														  result.append(_W_currency_symbol);
														  if (n_sep_by_space == 2)result.push_back(' ');
														  result.push_back('+');
													  }
													  else if (n_sign_posn == 3)
													  {
														  for (; i < m; i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  for (; i < n; i += 3)
														  {
															  result.append(_W_mon_thousands_sep);
															  result.push_back(tmp[i]);
															  result.push_back(tmp[i + 1]);
															  result.push_back(tmp[i + 2]);
														  }
														  for (; i < tmp.size(); i++)
														  {
															  result.push_back(tmp[i]);
														  }
														  if (n_sep_by_space == 1)result.push_back(' ');
														  result.push_back('+');
														  if (n_sep_by_space == 2)result.push_back(' ');
														  result.append(_W_currency_symbol);
													  }
												  }
											  }
										  }
	}
		break;
	case MemberNumberFormats::Decimal:
	{
										 result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
										 result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
															  result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
											result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
											result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
											 result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
										 std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
										 result.append(tmp);
										 result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
												result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(UInt32 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
										  
										  std::wstring tmp = _format(value, loc);
										  size_t n = tmp.size();
										  size_t m = n % 3;
										  size_t i = 0;
										  if (tmp.size())
										  {
											  auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
											  auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
											  auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
											  auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
											  auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
											  if (p_cs_precedes != 0)
											  {
												  if (p_sign_posn == 0)
												  {
													  result.append(_W_currency_symbol);
													  if (p_sep_by_space == 1) result.push_back(' ');
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
												  }
												  else if (p_sign_posn == 1 || p_sign_posn == 3)
												  {
													  result.push_back('+');
													  if (p_sep_by_space == 2)result.push_back(' ');
													  result.append(_W_currency_symbol);
													  if (p_sep_by_space == 1) result.push_back(' ');
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
												  }
												  else if (p_sign_posn == 2)
												  {
													  result.append(_W_currency_symbol);
													  if (p_sep_by_space == 1)result.push_back(' ');
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  if (p_sep_by_space == 2)result.push_back(' ');
													  result.push_back('+');
												  }
												  else if (p_sign_posn == 4)
												  {
													  result.append(_W_currency_symbol);
													  if (p_sep_by_space == 2)result.push_back(' ');
													  result.push_back('+');
													  if (p_sep_by_space == 1)result.push_back(' ');
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
												  }
											  }
											  else
											  {
												  if (p_sign_posn == 0)
												  {
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  if (p_sep_by_space != 0) result.push_back(' ');
													  result.append(_W_currency_symbol);
												  }
												  else if (p_sign_posn == 1)
												  {
													  result.push_back('+');
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  if (p_sep_by_space != 0)result.push_back(' ');
													  result.append(_W_currency_symbol);
												  }
												  else if (p_sign_posn == 2 || p_sign_posn == 4)
												  {
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  if (p_sep_by_space == 1)result.push_back(' ');
													  result.append(_W_currency_symbol);
													  if (p_sep_by_space == 2)result.push_back(' ');
													  result.push_back('+');
												  }
												  else if (p_sign_posn == 3)
												  {
													  for (; i < m; i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  for (; i < n; i += 3)
													  {
														  result.append(_W_mon_thousands_sep);
														  result.push_back(tmp[i]);
														  result.push_back(tmp[i + 1]);
														  result.push_back(tmp[i + 2]);
													  }
													  for (; i < tmp.size(); i++)
													  {
														  result.push_back(tmp[i]);
													  }
													  if (p_sep_by_space == 1)result.push_back(' ');
													  result.push_back('+');
													  if (p_sep_by_space == 2)result.push_back(' ');
													  result.append(_W_currency_symbol);
												  }
											  }
										  }
	}
		break;
	case MemberNumberFormats::Decimal:
	{
										 result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
										 result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
															  result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
											result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
											result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
											 result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
										 std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
										 result.append(tmp);
										 result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
												result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(long value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		
		std::wstring tmp = _format(abs(value), loc);
		size_t n = tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			if (value > 0.0f)
			{
				auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
				auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
				auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
				if (p_cs_precedes != 0)
				{
					if (p_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 1 || p_sign_posn == 3)
					{
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (p_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 2 || p_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
			else
			{
				auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
				auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
				auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
				if (n_cs_precedes != 0)
				{
					if (n_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 1 || n_sign_posn == 3)
					{
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (n_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 2 || n_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(ULong value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		
		std::wstring tmp = _format(value, loc);
		size_t n = tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
			auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
			auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
			if (p_cs_precedes != 0)
			{
				if (p_sign_posn == 0)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1) result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
				else if (p_sign_posn == 1 || p_sign_posn == 3)
				{
					result.push_back('+');
					if (p_sep_by_space == 2)result.push_back(' ');
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1) result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
				else if (p_sign_posn == 2)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1)result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
				}
				else if (p_sign_posn == 4)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
					if (p_sep_by_space == 1)result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
			}
			else
			{
				if (p_sign_posn == 0)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space != 0) result.push_back(' ');
					result.append(_W_currency_symbol);
				}
				else if (p_sign_posn == 1)
				{
					result.push_back('+');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space != 0)result.push_back(' ');
					result.append(_W_currency_symbol);
				}
				else if (p_sign_posn == 2 || p_sign_posn == 4)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 1)result.push_back(' ');
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
				}
				else if (p_sign_posn == 3)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 1)result.push_back(' ');
					result.push_back('+');
					if (p_sep_by_space == 2)result.push_back(' ');
					result.append(_W_currency_symbol);
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(Int64 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		
		std::wstring tmp = _format(abs(value), loc);
		size_t n = tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			if (value > 0.0f)
			{
				auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
				auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
				auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
				if (p_cs_precedes != 0)
				{
					if (p_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 1 || p_sign_posn == 3)
					{
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (p_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (p_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (p_sign_posn == 2 || p_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (p_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (p_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (p_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (p_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
			else
			{
				auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
				auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
				auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
				if (n_cs_precedes != 0)
				{
					if (n_sign_posn == 0)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 1 || n_sign_posn == 3)
					{
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1) result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
					else if (n_sign_posn == 2)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 4)
					{
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 1)result.push_back(' ');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
					}
				}
				else
				{
					if (n_sign_posn == 0)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0) result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 1)
					{
						result.push_back('+');
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space != 0)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
					else if (n_sign_posn == 2 || n_sign_posn == 4)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.append(_W_currency_symbol);
						if (n_sep_by_space == 2)result.push_back(' ');
						result.push_back('+');
					}
					else if (n_sign_posn == 3)
					{
						for (; i < m; i++)
						{
							result.push_back(tmp[i]);
						}
						for (; i < n; i += 3)
						{
							result.append(_W_mon_thousands_sep);
							result.push_back(tmp[i]);
							result.push_back(tmp[i + 1]);
							result.push_back(tmp[i + 2]);
						}
						for (; i < tmp.size(); i++)
						{
							result.push_back(tmp[i]);
						}
						if (n_sep_by_space == 1)result.push_back(' ');
						result.push_back('+');
						if (n_sep_by_space == 2)result.push_back(' ');
						result.append(_W_currency_symbol);
					}
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(UInt64 value, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		
		std::wstring tmp = _format(value, loc);
		size_t n = tmp.size();
		size_t m = n % 3;
		size_t i = 0;
		if (tmp.size())
		{
			auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
			auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
			auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
			auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
			auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
			if (p_cs_precedes != 0)
			{
				if (p_sign_posn == 0)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1) result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
				else if (p_sign_posn == 1 || p_sign_posn == 3)
				{
					result.push_back('+');
					if (p_sep_by_space == 2)result.push_back(' ');
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1) result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
				else if (p_sign_posn == 2)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 1)result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
				}
				else if (p_sign_posn == 4)
				{
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
					if (p_sep_by_space == 1)result.push_back(' ');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
				}
			}
			else
			{
				if (p_sign_posn == 0)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space != 0) result.push_back(' ');
					result.append(_W_currency_symbol);
				}
				else if (p_sign_posn == 1)
				{
					result.push_back('+');
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space != 0)result.push_back(' ');
					result.append(_W_currency_symbol);
				}
				else if (p_sign_posn == 2 || p_sign_posn == 4)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 1)result.push_back(' ');
					result.append(_W_currency_symbol);
					if (p_sep_by_space == 2)result.push_back(' ');
					result.push_back('+');
				}
				else if (p_sign_posn == 3)
				{
					for (; i < m; i++)
					{
						result.push_back(tmp[i]);
					}
					for (; i < n; i += 3)
					{
						result.append(_W_mon_thousands_sep);
						result.push_back(tmp[i]);
						result.push_back(tmp[i + 1]);
						result.push_back(tmp[i + 2]);
					}
					for (; i < tmp.size(); i++)
					{
						result.push_back(tmp[i]);
					}
					if (p_sep_by_space == 1)result.push_back(' ');
					result.push_back('+');
					if (p_sep_by_space == 2)result.push_back(' ');
					result.append(_W_currency_symbol);
				}
			}
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::General:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::NumberWithCommasForThousands:
	{
		result.append(_format(value, loc, ToStringFlags::Normal, true));
	}
		break;
	case MemberNumberFormats::FixedPoint:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Scientific:
	{
		result.append(_format(value, loc, ToStringFlags::Scientific));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(value, loc, ToStringFlags::Hex));
	}
		break;
	case MemberNumberFormats::Percent:
	{
		std::wstring tmp = _format(value*100.0f, 0, loc, ToStringFlags::FloatingPoint);
		result.append(tmp);
		result.push_back('%');
	}
		break;
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(value, loc));
	}
		break;
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(value, loc));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(const Int128& val, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		if (val.high == 0 || val.high==-1)
		{
			Int64 value = val.IsNegative ? -(Int64)val.low : (Int64)val.low;
			
			std::wstring tmp = _format(abs(value), loc);
			size_t n = tmp.size();
			size_t m = n % 3;
			size_t i = 0;
			if (tmp.size())
			{
				auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
				auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
				if (value > 0.0f)
				{
					auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
					auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
					auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
					if (p_cs_precedes != 0)
					{
						if (p_sign_posn == 0)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (p_sign_posn == 1 || p_sign_posn == 3)
						{
							result.push_back('+');
							if (p_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (p_sign_posn == 2)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (p_sign_posn == 4)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
							if (p_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
					}
					else
					{
						if (p_sign_posn == 0)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space != 0) result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (p_sign_posn == 1)
						{
							result.push_back('+');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space != 0)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (p_sign_posn == 2 || p_sign_posn == 4)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 1)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (p_sign_posn == 3)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 1)result.push_back(' ');
							result.push_back('+');
							if (p_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
					}
				}
				else
				{
					auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
					auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
					auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
					if (n_cs_precedes != 0)
					{
						if (n_sign_posn == 0)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (n_sign_posn == 1 || n_sign_posn == 3)
						{
							result.push_back('+');
							if (n_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (n_sign_posn == 2)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (n_sign_posn == 4)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
							if (n_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
					}
					else
					{
						if (n_sign_posn == 0)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space != 0) result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (n_sign_posn == 1)
						{
							result.push_back('+');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space != 0)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (n_sign_posn == 2 || n_sign_posn == 4)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 1)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (n_sign_posn == 3)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 1)result.push_back(' ');
							result.push_back('+');
							if (n_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
					}
				}
			}
		}
		else
		{
			result.append(_format(val, loc, false));
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	case MemberNumberFormats::General:
	case MemberNumberFormats::NumberWithCommasForThousands:
	case MemberNumberFormats::FixedPoint:
	case MemberNumberFormats::Scientific:
	case MemberNumberFormats::Percent:
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(val, loc, false));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(val, loc, true));
	}
		break;
	
	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(val, loc, false));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}

void Formatter::_format(const UInt128& val, const MemberInfo& info, std::wstring& result, const CultureInfo& loc)
{
	switch (info.NumberFormat)
	{
	case MemberNumberFormats::Currency:
	{
		if (val.high == 0)
		{
			UInt64 value = val.low;
			
			std::wstring tmp = _format(value, loc);
			size_t n = tmp.size();
			size_t m = n % 3;
			size_t i = 0;
			if (tmp.size())
			{
				auto _W_mon_thousands_sep = loc.GetConstsName(CultureInfoConstNames::MoneyThousandsSeparator);
				auto _W_currency_symbol = loc.GetConstsName(CultureInfoConstNames::CurrencySymbol);
				if (value > 0.0f)
				{
					auto p_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveCurrencySymbolPos);
					auto p_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSignSymbolPos);
					auto p_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::PositiveSeparatorPos);
					if (p_cs_precedes != 0)
					{
						if (p_sign_posn == 0)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (p_sign_posn == 1 || p_sign_posn == 3)
						{
							result.push_back('+');
							if (p_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (p_sign_posn == 2)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (p_sign_posn == 4)
						{
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
							if (p_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
					}
					else
					{
						if (p_sign_posn == 0)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space != 0) result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (p_sign_posn == 1)
						{
							result.push_back('+');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space != 0)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (p_sign_posn == 2 || p_sign_posn == 4)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 1)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (p_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (p_sign_posn == 3)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (p_sep_by_space == 1)result.push_back(' ');
							result.push_back('+');
							if (p_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
					}
				}
				else
				{
					auto n_cs_precedes = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeCurrencySymbolPos);
					auto n_sign_posn = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSignSymbolPos);
					auto n_sep_by_space = loc.GetMoneyConst(CultureInfoMoneyConsts::NegativeSeparatorPos);
					if (n_cs_precedes != 0)
					{
						if (n_sign_posn == 0)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (n_sign_posn == 1 || n_sign_posn == 3)
						{
							result.push_back('+');
							if (n_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1) result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
						else if (n_sign_posn == 2)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (n_sign_posn == 4)
						{
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
							if (n_sep_by_space == 1)result.push_back(' ');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
						}
					}
					else
					{
						if (n_sign_posn == 0)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space != 0) result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (n_sign_posn == 1)
						{
							result.push_back('+');
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space != 0)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
						else if (n_sign_posn == 2 || n_sign_posn == 4)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 1)result.push_back(' ');
							result.append(_W_currency_symbol);
							if (n_sep_by_space == 2)result.push_back(' ');
							result.push_back('+');
						}
						else if (n_sign_posn == 3)
						{
							for (; i < m; i++)
							{
								result.push_back(tmp[i]);
							}
							for (; i < n; i += 3)
							{
								result.append(_W_mon_thousands_sep);
								result.push_back(tmp[i]);
								result.push_back(tmp[i + 1]);
								result.push_back(tmp[i + 2]);
							}
							for (; i < tmp.size(); i++)
							{
								result.push_back(tmp[i]);
							}
							if (n_sep_by_space == 1)result.push_back(' ');
							result.push_back('+');
							if (n_sep_by_space == 2)result.push_back(' ');
							result.append(_W_currency_symbol);
						}
					}
				}
			}
		}
		else
		{
			result.append(_format(val, loc, false));
		}
	}
		break;
	case MemberNumberFormats::Decimal:
	case MemberNumberFormats::General:
	case MemberNumberFormats::NumberWithCommasForThousands:
	case MemberNumberFormats::FixedPoint:
	case MemberNumberFormats::Scientific:
	case MemberNumberFormats::Percent:
	case MemberNumberFormats::RoundTrippable:
	{
		result.append(_format(val, loc, false));
	}
		break;
	case MemberNumberFormats::Hexadecimal:
	{
		result.append(_format(val, loc, true));
	}
		break;

	case MemberNumberFormats::Custom:
	default:
		if (info.CustomNumberFormatString.empty())
			result.append(_format(val, loc, false));
		else
			result.append(info.CustomNumberFormatString);
		break;
	}
}


std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, const VariadicAny& vals)
{
	std::wstring result = std::wstring(); 
	Formatter::pos pos(-1);
	Formatter::pos j = 0;
	try
	{
		do
		{
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::All, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			size_t index = info.Index;
			size_t count = vals.Size;
			if (index >= 0 && index < count)
			{
				const Any& dec = vals.At(index);
				const std::type_info & stdti = dec.GetSTDRTTI();
				if (stdti == typeid(bool))
				{
					_format(AnyVisitor<bool>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Int8))
				{
					_format(AnyVisitor<Int8>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(UInt8))
				{
					_format(AnyVisitor<UInt8>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Int16))
				{
					_format(AnyVisitor<Int16>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(UInt16))
				{
					_format(AnyVisitor<UInt16>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Int24))
				{
					_format(AnyVisitor<Int24>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(UInt24))
				{
					_format(AnyVisitor<UInt24>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Int32))
				{
					_format(AnyVisitor<Int32>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(UInt32))
				{
					_format(AnyVisitor<UInt32>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(long))
				{
					_format(AnyVisitor<long>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(ULong))
				{
					_format(AnyVisitor<ULong>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Int64))
				{
					_format(AnyVisitor<Int64>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(UInt64))
				{
					_format(AnyVisitor<UInt64>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Int128))
				{
					_format(AnyVisitor<Int128>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(UInt128))
				{
					_format(AnyVisitor<UInt128>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Float32))
				{
					_format(AnyVisitor<Float32>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Float64))
				{
					_format(AnyVisitor<Float64>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(Float128))
				{
					_format(AnyVisitor<Float128>::Get(dec), info, result, loc);
				}
				else if (stdti == typeid(IFormattable*))
				{
					IFormattable* ifmt = AnyVisitor<IFormattable*>::Get(dec);
					
				}
				else if (stdti == typeid(const char*))
				{
					const char* chstr = AnyVisitor<const char*>::Get(dec);
					std::wstring iout = std::wstring();
					if (Converter::Convert(chstr, iout, loc.CLocale))
					{
						if (!iout.empty()) result.append(iout);
					}
				}
				else if (stdti == typeid(const wchar_t*))
				{
					const wchar_t* chstr = AnyVisitor<const wchar_t*>::Get(dec);
					std::wstring iout = std::wstring(chstr);
					if (!iout.empty()) result.append(iout);
				}
				else if (stdti == typeid(std::string))
				{
					std::string chstr = AnyVisitor<std::string>::Get(dec);
					std::wstring iout = std::wstring();
					if (Converter::Convert(chstr, iout, loc.CLocale))
					{
						if (!iout.empty()) result.append(iout);
					}
				}
				else if (stdti == typeid(std::wstring))
				{
					std::wstring iout = AnyVisitor<std::wstring>::Get(dec);
					if (!iout.empty()) result.append(iout);
				}
				else
				{
					_type ty = dec.Type;
					if (ty)
					{
						bool checked = ty->IsObject;
						if (!checked)
						{
							if (ty->IsPointer)
							{
								const PtrType* pty = dynamic_cast<const PtrType*>(ty);
								if (pty)
								{
									checked = pty->Dimension == 1;
								}
							}
						}
						if (checked)
						{
							Object* obj = dec.ToObject<1>();
							if (obj)
							{
								std::wstring iout = obj->ToString(info.CustomNumberFormatString.c_str(), loc);
								if (!iout.empty()) result.append(iout);
							}
							else
							{
								Object* aobj = dec.AllocateObject();
								if (aobj)
								{
									std::wstring iout = aobj->ToString(info.CustomNumberFormatString.c_str(), loc);
									if (!iout.empty()) result.append(iout);
									delete aobj;
									aobj = nullptr;
								}
							}
						}
					}
				}
			}
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, bool val)
{
	std::wstring result = std::wstring();
	if (!fmt)
	{
		result.append(_format(val,loc));
		return result;
	}
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Int8 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);	
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, UInt8 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Int16 val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, UInt16 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, const Int24& val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j = 0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, const UInt24& val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j = 0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Int32 val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, UInt32 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, long val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, ULong val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Int64 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, UInt64 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, const Int128& val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j = 0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, const UInt128& val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"G";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j = 0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}
std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Float32 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"R";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Float64 val)
{
	std::wstring result = std::wstring();
	if (!fmt) fmt = L"R";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, Float128 val)
{
	std::wstring result = std::wstring(); 
	if (!fmt) fmt = L"R";
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			_format(val, info, result, loc);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::FormatWidenChar(const wchar_t* fmt, const CultureInfo& loc, wchar_t val)
{
	std::wstring result = std::wstring();
	if (!fmt)
	{
		result.push_back(val);
		return result;
	}

	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			result.push_back(val);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::FormatString(const wchar_t* fmt, const CultureInfo& loc, const std::wstring& val)
{
	std::wstring result = std::wstring();	
	if (!fmt)
	{
		result.append(val);
		return result;
	}
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			result.append(val);
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}


std::wstring Formatter::Format(const wchar_t* fmt, const CultureInfo& loc, IFormattable* refVal)
{
	std::wstring result = std::wstring();
	if (!refVal) return result;
	if (!fmt)
	{
		result.append(refVal->ToString(fmt, loc));
		return result;
	}
	Formatter::pos pos(-1);
	try
	{
		do
		{
			Formatter::pos j=0;
			MemberInfo info = MemberInfo();
			Formatter::pos found(0);
			Formatter::pos lpos = pos + 1;
			pos = Formatter::ResolveNumberFormat(fmt, info, pos + 1, loc, FormatStyle::OnlyCustomIgnoreBracket, found);
			if (found != Formatter::npos)
			{
				for (j = lpos; j < found; j++)
				{
					result.push_back(fmt[j]);
				}
			}
			if (pos == Formatter::npos)
			{
				int s = wcslen(fmt);
				for (j = lpos; j < s; j++)
				{
					result.push_back(fmt[j]);
				}
				break;
			}
			result.append(refVal->ToString(fmt, loc));
		} while (pos != Formatter::npos);
	}
	catch (...)
	{

	}
	return result;
}

std::wstring Formatter::Format(const wchar_t* fmt, bool val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Int8 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, UInt8 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Int16 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, UInt16 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, const Int24& val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, const UInt24& val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Int32 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, UInt32 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, long val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, ULong val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Int64 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, UInt64 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, const Int128& val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, const UInt128& val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Float32 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Float64 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, Float128 val)
{
	return Format(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::FormatWidenChar(const wchar_t* fmt, wchar_t val)
{
	return FormatWidenChar(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::FormatString(const wchar_t* fmt, const std::wstring& val)
{
	return FormatString(fmt, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const wchar_t* fmt, IFormattable* refVal)
{
	return Format(fmt, CultureInfo::Current(), refVal);
}

std::wstring Formatter::Format(const CultureInfo& loc,bool val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Int8 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, UInt8 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Int16 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, UInt16 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, const Int24& val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, const UInt24& val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Int32 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, UInt32 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, long val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, ULong val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Int64 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, UInt64 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, const Int128& val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, const UInt128& val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Float32 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Float64 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, Float128 val)
{
	return Format(NULL, loc, val);
}

std::wstring Formatter::Format(const CultureInfo& loc, IFormattable* refVal)
{
	return Format(NULL, loc, refVal);
}

std::wstring Formatter::Format(bool val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Int8 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(UInt8 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Int16 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(UInt16 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const Int24& val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const UInt24& val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Int32 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(UInt32 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(long val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(ULong val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Int64 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(UInt64 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const Int128& val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(const UInt128& val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Float32 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Float64 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(Float128 val)
{
	return Format(NULL, CultureInfo::Current(), val);
}

std::wstring Formatter::Format(IFormattable* refVal)
{
	return Format(NULL, CultureInfo::Current(), refVal);
}


