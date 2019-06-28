#include "Encodings.h"
#include <codecvt>
#include <fstream>
#include <stdlib.h>

using namespace System;

#pragma warning(disable: 4996)
template<class Facet>
struct deletable_facet : Facet
{
	template<class ...Args>
	deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};

namespace Conversion
{
	template<class _Elem,
		unsigned long _Mymax = 0x10ffff,
		std::codecvt_mode _Mymode = (std::codecvt_mode)0>
	class codecvt_utf32
		: public std::codecvt<_Elem, char, std::_Statype>
	{	// facet for converting between _Elem and UTF-32 multibyte sequences
		enum { _Bytes_per_word = 4 };
	public:
		typedef std::codecvt<_Elem, char, std::_Statype> _Mybase;
		typedef typename _Mybase::result result;
		typedef char _Byte;
		typedef _Elem intern_type;
		typedef _Byte extern_type;
		typedef std::_Statype state_type;

		explicit codecvt_utf32(size_t _Refs = 0)
			: _Mybase(_Refs)
		{	// construct with ref count
		}

		virtual ~codecvt_utf32() noexcept
		{	// destroy the object
		}

	protected:
		virtual result do_in(std::_Statype& _State,
			const _Byte *_First1, const _Byte *_Last1, const _Byte *& _Mid1,
			_Elem *_First2, _Elem *_Last2, _Elem *& _Mid2) const
		{	// convert bytes [_First1, _Last1) to [_First2, _Last)
			char *_Pstate = (char *)&_State;
			_Mid1 = _First1;
			_Mid2 = _First2;

			for (; _Bytes_per_word <= _Last1 - _Mid1 && _Mid2 != _Last2;)
			{	// convert a multibyte sequence
				unsigned char *_Ptr = (unsigned char *)_Mid1;
				unsigned long _Ch;
				unsigned short _Ch0, _Ch1;

				if (*_Pstate == _LITTLE_FIRST)
					_Ch0 = (unsigned short)(_Ptr[1] << 8 | _Ptr[0]);
				else if (*_Pstate == _BIG_FIRST)
					_Ch0 = (unsigned short)(_Ptr[0] << 8 | _Ptr[1]);
				else
				{	// no header seen yet, try preferred mode
					unsigned char _Default_endian = (_Mymode & std::codecvt_mode::little_endian) != 0
						? _LITTLE_FIRST : _BIG_FIRST;

					if ((_Mymode & std::codecvt_mode::little_endian) != 0)
						_Ch0 = (unsigned short)(_Ptr[1] << 8 | _Ptr[0]);
					else
						_Ch0 = (unsigned short)(_Ptr[0] << 8 | _Ptr[1]);
					if ((_Mymode & std::codecvt_mode::consume_header) == 0
						|| (_Ch0 != 0xfeff && _Ch0 != 0xfffe))
						*_Pstate = _Default_endian;
					else
					{	// consume header, fixate on endianness, and retry
						_Mid1 += _Bytes_per_word;
						*_Pstate = (char)(_Ch0 == 0xfeff
							? _Default_endian
							: (unsigned char)(3 - _Default_endian));
						result _Ans = do_in(_State, _Mid1, _Last1, _Mid1,
							_First2, _Last2, _Mid2);

						if (_Ans == _Mybase::partial)
						{	// not enough bytes, roll back header
							*_Pstate = 0;
							_Mid1 = _First1;
						}
						return (_Ans);
					}
				}

				if (_Ch0 < 0xd800 || 0xdc00 <= _Ch0)
				{	// one word, consume bytes
					_Mid1 += _Bytes_per_word;
					_Ch = _Ch0;
				}
				else if (_Last1 - _Mid1 < 2 * _Bytes_per_word)
					break;
				else
				{	// get second word
					if (*_Pstate == _LITTLE_FIRST)
						_Ch1 = (unsigned short)(_Ptr[3] << 8 | _Ptr[2]);
					else
						_Ch1 = (unsigned short)(_Ptr[2] << 8 | _Ptr[3]);

					if (_Ch1 < 0xdc00 || 0xe000 <= _Ch1)
						return (_Mybase::error);

					_Mid1 += 2 * _Bytes_per_word;
					_Ch = (unsigned long)(_Ch0 - 0xd800 + 0x0040) << 10
						| (_Ch1 - 0xdc00);
				}

				if (_Mymax < _Ch)
					return (_Mybase::error);	// code too large
				*_Mid2++ = (_Elem)_Ch;
			}

			return (_First1 == _Mid1 ? _Mybase::partial : _Mybase::ok);
		}

		virtual result do_out(std::_Statype& _State,
			const _Elem *_First1, const _Elem *_Last1, const _Elem *& _Mid1,
			_Byte *_First2, _Byte *_Last2, _Byte *& _Mid2) const
		{	// convert [_First1, _Last1) to bytes [_First2, _Last)
			char *_Pstate = (char *)&_State;
			_Mid1 = _First1;
			_Mid2 = _First2;

			if (*_Pstate == 0)
			{	// determine endianness once, maybe generate header
				*_Pstate = (_Mymode & std::codecvt_mode::little_endian) != 0
					? _LITTLE_FIRST : _BIG_FIRST;
				if ((_Mymode & std::codecvt_mode::generate_header) == 0)
					;
				else if (_Last2 - _Mid2 < 3 * _Bytes_per_word)
					return (_Mybase::partial);	// not enough room for all
				else if (*_Pstate == _LITTLE_FIRST)
				{	// put header LS byte first
					*_Mid2++ = (_Byte)(unsigned char)0xff;
					*_Mid2++ = (_Byte)(unsigned char)0xfe;
				}
				else
				{	// put header MS byte first
					*_Mid2++ = (_Byte)(unsigned char)0xfe;
					*_Mid2++ = (_Byte)(unsigned char)0xff;
				}
			}

			for (; _Mid1 != _Last1 && _Bytes_per_word <= _Last2 - _Mid2;)
			{	// convert and put a wide char
				bool _Extra = false;
				unsigned long _Ch = (unsigned long)*_Mid1++;

				if ((_Mymax < 0x10ffff ? _Mymax : 0x10ffff) < _Ch)
					return (_Mybase::error);	// value too large
				else if (_Ch <= 0xffff)
				{	// one word, can't be code for first of two
					if (0xd800 <= _Ch && _Ch < 0xdc00)
						return (_Mybase::error);
				}
				else if (_Last2 - _Mid2 < 2 * _Bytes_per_word)
				{	// not enough room for two-word output, back up
					--_Mid1;
					return (_Mybase::partial);
				}
				else
					_Extra = true;

				if (*_Pstate == _LITTLE_FIRST)
				if (!_Extra)
				{	// put a single word LS byte first
					*_Mid2++ = (_Byte)_Ch;
					*_Mid2++ = (_Byte)(_Ch >> 8);
				}
				else
				{	// put a pair of words LS byte first
					unsigned short _Ch0 = (unsigned short)(0xd800
						| (unsigned short)(_Ch >> 10) - 0x0040);
					*_Mid2++ = (_Byte)_Ch0;
					*_Mid2++ = (_Byte)(_Ch0 >> 8);

					_Ch0 = (unsigned short)(0xdc00
						| ((unsigned short)_Ch & 0x03ff));
					*_Mid2++ = (_Byte)_Ch0;
					*_Mid2++ = (_Byte)(_Ch0 >> 8);
				}
				else
				if (!_Extra)
				{	// put a single word MS byte first
					*_Mid2++ = (_Byte)(_Ch >> 8);
					*_Mid2++ = (_Byte)_Ch;
				}
				else
				{	// put a pair of words MS byte first
					unsigned short _Ch0 = (unsigned short)(0xd800
						| (unsigned short)(_Ch >> 10) - 0x0040);
					*_Mid2++ = (_Byte)(_Ch0 >> 8);
					*_Mid2++ = (_Byte)_Ch0;

					_Ch0 = (unsigned short)(0xdc00
						| ((unsigned short)_Ch & 0x03ff));
					*_Mid2++ = (_Byte)(_Ch0 >> 8);
					*_Mid2++ = (_Byte)_Ch0;
				}
			}

			return (_First1 == _Mid1 ? _Mybase::partial : _Mybase::ok);
		}

		virtual result do_unshift(std::_Statype&,
			_Byte *_First2, _Byte *, _Byte *& _Mid2) const
		{	// generate bytes to return to default shift state
			_Mid2 = _First2;
			return (_Mybase::ok);
		}

		virtual int do_length(std::_Statype& _State, const _Byte *_First1,
			const _Byte *_Last1, size_t _Count) const noexcept
		{	// return min(_Count, converted length of bytes [_First1, _Last1))
				size_t _Wchars = 0;
				std::_Statype _Mystate = _State;

				for (; _Wchars < _Count && _First1 != _Last1;)
				{	// convert another wide char
					const _Byte *_Mid1;
					_Elem *_Mid2;
					_Elem _Ch;

					switch (do_in(_Mystate, _First1, _Last1, _Mid1,
						&_Ch, &_Ch + 1, _Mid2))
					{	// test result of single wide-char conversion
					case _Mybase::noconv:
						return ((int)(_Wchars + (_Last1 - _First1)));

					case  _Mybase::ok:
						if (_Mid2 == &_Ch + 1)
							++_Wchars;	// replacement do_in might not convert one
						_First1 = _Mid1;
						break;

					default:
						return ((int)_Wchars);	// error or partial
					}
				}

				return ((int)_Wchars);
			}

		virtual bool do_always_noconv() const noexcept
		{	// return true if conversions never change input
			return (false);
		}

		virtual int do_max_length() const noexcept
		{	// return maximum length required for a conversion
			return ((_Mymode & (std::codecvt_mode::consume_header | std::codecvt_mode::generate_header)) != 0
				? 3 * _Bytes_per_word : 6 * _Bytes_per_word);
		}

		virtual int do_encoding() const noexcept
		{	// return length of code sequence (from codecvt)
			return ((_Mymode & (std::codecvt_mode::consume_header | std::codecvt_mode::generate_header)) != 0
				? -1 : 0);	// -1 => state dependent, 0 => varying length
		}
	};

	template<class _Codecvt,
	class _Elem = wchar_t,
	class _Walloc = std::allocator<_Elem>,
	class _Balloc = std::allocator<char> >
	class wstring_convert
	{	// converts between _Elem (wide) and char (byte) strings
		enum { _BUF_INC = 8, _BUF_MAX = 16 };
		void _Init(const std::locale& loc, const _Codecvt *_Pcvt_arg = new _Codecvt)
		{	// initialize the object
			static state_type _State0;
			_State = _State0;
			_Pcvt = _Pcvt_arg;
			_Loc = std::locale(loc, _Pcvt);
			_Nconv = 0;
		}

	public:
		typedef std::basic_string<char, std::char_traits<char>, _Balloc> byte_string;
		typedef std::basic_string<_Elem, std::char_traits<_Elem>, _Walloc> wide_string;
		typedef typename _Codecvt::state_type state_type;
		typedef typename wide_string::traits_type::int_type int_type;

		wstring_convert(const std::locale& loc)
			: _Has_berr(false), _Has_werr(false), _Has_state(false)
		{	// construct with no error strings
			_Init(loc);
		}

		wstring_convert(const std::locale& loc, const _Codecvt *_Pcvt_arg)
			: _Has_berr(false), _Has_werr(false), _Has_state(false)
		{	// construct with no error strings and codecvt
			_Init(loc, _Pcvt_arg);
		}

		wstring_convert(const std::locale& loc, const _Codecvt *_Pcvt_arg, state_type _State_arg)
			: _Has_berr(false), _Has_werr(false), _Has_state(true)
		{	// construct with no error strings, codecvt, and state
			_Init(loc, _Pcvt_arg);
			_State = _State_arg;
		}

		wstring_convert(const std::locale& loc, const byte_string& _Berr_arg)
			: _Has_berr(true), _Has_werr(false), _Has_state(false),
			_Berr(_Berr_arg)
		{	// construct with byte error string
			_Init(loc);
		}

		wstring_convert(const std::locale& loc, const byte_string& _Berr_arg,
			const wide_string& _Werr_arg)
			: _Has_berr(true), _Has_werr(true), _Has_state(false),
			_Berr(_Berr_arg), _Werr(_Werr_arg)
		{	// construct with byte and wide error strings
			_Init(loc);
		}

		virtual ~wstring_convert() noexcept
		{	// destroy the object
		}

		size_t converted() const
		{	// get conversion count
			return (_Nconv);
		}

		state_type state() const
		{	// get state
			return (_State);
		}

		wide_string from_bytes(char _Byte)
		{	// convert a byte to a wide string
			return (from_bytes(&_Byte, &_Byte + 1));
		}

		wide_string from_bytes(const char *_Ptr)
		{	// convert a NTBS to a wide string
			return (from_bytes(_Ptr, _Ptr + strlen(_Ptr)));
		}

		wide_string from_bytes(const byte_string& _Bstr)
		{	// convert a byte string to a wide string
			const char *_Ptr = _Bstr.c_str();
			return (from_bytes(_Ptr, _Ptr + _Bstr.size()));
		}

		wide_string from_bytes(const char *_First, const char *_Last)
		{	// convert byte sequence [_First, _Last) to a wide string
			static state_type _State0;
			wide_string _Wbuf, _Wstr;
			const char *_First_sav = _First;

			if (!_Has_state)
				_State = _State0;	// reset state if not remembered
			_Wbuf.append((_CSTD size_t)_BUF_INC, (_Elem)'\0');
			for (_Nconv = 0; _First != _Last; _Nconv = _First - _First_sav)
			{	// convert one or more bytes
				_Elem *_Dest = &*_Wbuf.begin();
				_Elem *_Dnext;

				switch (_Pcvt->in(_State,
					_First, _Last, _First,
					_Dest, _Dest + _Wbuf.size(), _Dnext))
				{	// test result of converting one or more bytes
				case _Codecvt::partial:
				case _Codecvt::ok:
					if (_Dest < _Dnext)
						_Wstr.append(_Dest, (_CSTD size_t)(_Dnext - _Dest));
					else if (_Wbuf.size() < _BUF_MAX)
						_Wbuf.append((_CSTD size_t)_BUF_INC, '\0');
					else if (_Has_werr)
						return (_Werr);
					else
						throw std::range_error("bad conversion");
					break;

				case _Codecvt::noconv:
					for (; _First != _Last; ++_First)
						_Wstr.append((_CSTD size_t)1,
						(_Elem)(unsigned char)*_First);
					break;	// no conversion, just copy code values

				default:
					if (_Has_werr)
						return (_Werr);
					else
						throw std::range_error("bad conversion");
				}
			}
			return (_Wstr);
		}

		byte_string to_bytes(_Elem _Char)
		{	// convert a wide char to a byte string
			return (to_bytes(&_Char, &_Char + 1));
		}

		byte_string to_bytes(const _Elem *_Wptr)
		{	// convert a NTWCS to a byte string
			const _Elem *_Next = _Wptr;
			for (; (int_type)*_Next != 0; ++_Next)
				;
			return (to_bytes(_Wptr, _Next));
		}

		byte_string to_bytes(const wide_string& _Wstr)
		{	// convert a wide string to a byte string
			const _Elem *_Wptr = _Wstr.c_str();
			return (to_bytes(_Wptr, _Wptr + _Wstr.size()));
		}

		byte_string to_bytes(const _Elem *_First, const _Elem *_Last)
		{	// convert wide sequence [_First, _Last) to a byte string
			static state_type _State0;
			byte_string _Bbuf, _Bstr;
			const _Elem *_First_sav = _First;

			if (!_Has_state)
				_State = _State0;	// reset state if not remembered
			_Bbuf.append((_CSTD size_t)_BUF_INC, '\0');
			for (_Nconv = 0; _First != _Last; _Nconv = _First - _First_sav)
			{	// convert one or more wide chars
				char *_Dest = &*_Bbuf.begin();
				char *_Dnext;

				switch (_Pcvt->out(_State,
					_First, _Last, _First,
					_Dest, _Dest + _Bbuf.size(), _Dnext))
				{	// test result of converting one or more wide chars
				case _Codecvt::partial:
				case _Codecvt::ok:
					if (_Dest < _Dnext)
						_Bstr.append(_Dest, (_CSTD size_t)(_Dnext - _Dest));
					else if (_Bbuf.size() < _BUF_MAX)
						_Bbuf.append((_CSTD size_t)_BUF_INC, '\0');
					else if (_Has_berr)
						return (_Berr);
					else
						throw std::range_error("bad conversion");
					break;

				case _Codecvt::noconv:
					for (; _First != _Last; ++_First)
						_Bstr.append((_CSTD size_t)1,
						(char)(int_type)*_First);
					break;	// no conversion, just copy code values

				default:
					if (_Has_berr)
						return (_Berr);
					else
						throw std::range_error("bad conversion");
				}
			}
			return (_Bstr);
		}

		wstring_convert(const wstring_convert&) = delete;
		wstring_convert& operator=(const wstring_convert&) = delete;

	private:
		const _Codecvt *_Pcvt;	// the codecvt facet
		std::locale _Loc;	// manages reference to codecvt facet
		byte_string _Berr;
		wide_string _Werr;
		state_type _State;	// the remembered state
		bool _Has_state;
		bool _Has_berr;
		bool _Has_werr;
		size_t _Nconv;
	};
}


unsigned long EncodingHelper::GetMaxCharCount(unsigned int byteCount, Encodings encoding)
{
	unsigned long long maxCharCount = (unsigned long long) byteCount;
	switch (encoding)
	{
	case Encodings::Unicode:
	case Encodings::Utf16BE:
	{
							   maxCharCount = (unsigned long long)(byteCount >> 1) + (unsigned long long)(byteCount & 1) + 1ULL;
	}
		break;
	case Encodings::Utf8:
	{
							maxCharCount = (unsigned long long)(byteCount)+1ULL;
	}
		break;
	case Encodings::Utf32LE:
	case Encodings::Utf32BE:
	{
							   maxCharCount = (unsigned long long)(byteCount) / 2 + 2;
	}
		break;
	case Encodings::Ansi:
		break;
	}
	if (maxCharCount > LONG_MAX)
	{
		throw std::exception("out of long");
	}
	return (unsigned long)maxCharCount;
}

unsigned long EncodingHelper::Utf8GetUCS(char* bytes, unsigned long byteCount, wchar_t* chars, unsigned long charCount, const CultureInfo& loc, bool nothrow)
{
	try
	{
		Conversion::wstring_convert<std::codecvt_utf8<wchar_t>> conv(loc.CLocale);
		std::wstring result = conv.from_bytes(bytes, bytes + byteCount);
		result.copy(chars, result.size());
		return result.size();
	}
	catch (...)
	{
		if (!nothrow) throw;
		return 0UL;
	}
}

unsigned long EncodingHelper::Utf16LEGetUCS(char* bytes, unsigned long byteCount, wchar_t* chars, unsigned long charCount, const CultureInfo& loc, bool nothrow)
{
	try
	{
		Conversion::wstring_convert<std::codecvt_utf16<wchar_t,1114111UL, std::codecvt_mode::little_endian>> conv(loc.CLocale);
		std::wstring result = conv.from_bytes(bytes, bytes + byteCount);
		result.copy(chars, result.size());
		return result.size();
	}
	catch (...)
	{
		if (!nothrow) throw;
		return 0UL;
	}
}

unsigned long EncodingHelper::Utf16BEGetUCS(char* bytes, unsigned long byteCount, wchar_t* chars, unsigned long charCount, const CultureInfo& loc, bool nothrow)
{
	try
	{
		Conversion::wstring_convert<std::codecvt_utf16<wchar_t>> conv(loc.CLocale);
		std::wstring result = conv.from_bytes(bytes, bytes + byteCount);
		result.copy(chars, result.size());
		return result.size();
	}
	catch (...)
	{
		if (!nothrow) throw;
		return 0UL;
	}
}

unsigned long EncodingHelper::Utf32LEGetUCS(char* bytes, unsigned long byteCount, wchar_t* chars, unsigned long charCount, const CultureInfo& loc, bool nothrow)
{
	try
	{
		Conversion::wstring_convert<Conversion::codecvt_utf32<wchar_t, 1114111UL, std::codecvt_mode::little_endian>> conv(loc.CLocale);
		std::wstring result = conv.from_bytes(bytes, bytes + byteCount);
		result.copy(chars, result.size());
		return result.size();
	}
	catch (...)
	{
		if (!nothrow) throw;
		return 0UL;
	}
}

unsigned long EncodingHelper::Utf32BEGetUCS(char* bytes, unsigned long byteCount, wchar_t* chars, unsigned long charCount, const CultureInfo& loc, bool nothrow)
{
	try
	{
		Conversion::wstring_convert<Conversion::codecvt_utf32<wchar_t>> conv(loc.CLocale);
		std::wstring result = conv.from_bytes(bytes, bytes + byteCount);
		result.copy(chars, result.size());
		return result.size();
	}
	catch (...)
	{
		if (!nothrow) throw;
		return 0UL;
	}
}

unsigned long EncodingHelper::GetUCS(char* bytes, unsigned long byteSize, unsigned long byteIndex, unsigned int byteCount, wchar_t* chars, unsigned long charSize, unsigned long charIndex, Encodings encoding, const CultureInfo& loc, bool nothrow)
{
	try
	{
		switch (encoding)
		{
		case Encodings::Ansi:
		{		
								/*ÓÐ´íÎó*/
				size_t convertedChars = 0;
				if (_mbstowcs_s_l(&convertedChars, nullptr, 0, bytes + byteIndex, charSize - charIndex, loc.CPlusLocale) != 0) return 0;
				if (convertedChars > charSize - charIndex) return 0; 
				if (_mbstowcs_s_l(&convertedChars, chars + charIndex, convertedChars, bytes + byteIndex, _TRUNCATE, loc.CPlusLocale) != 0) return 0;
				return charSize - charIndex;
		}
			break;
		case Encodings::Utf8:
		{
								return Utf8GetUCS(bytes + byteIndex, byteCount, chars + charIndex, charSize - charIndex, loc, nothrow);
		}
		case Encodings::Utf16LE:
		{
								   return Utf16LEGetUCS(bytes + byteIndex, byteCount, chars + charIndex, charSize - charIndex, loc, nothrow);
		}
		case Encodings::Utf16BE:
		{
								   return Utf16BEGetUCS(bytes + byteIndex, byteCount, chars + charIndex, charSize - charIndex, loc, nothrow);
		}
		case Encodings::Utf32LE:
		{
								   return Utf32LEGetUCS(bytes + byteIndex, byteCount, chars + charIndex, charSize - charIndex, loc, nothrow);
		}
		case Encodings::Utf32BE:
		{
								   return Utf32BEGetUCS(bytes + byteIndex, byteCount, chars + charIndex, charSize - charIndex, loc, nothrow);
		}
		default:
			return 0UL;
		}
		return 0UL;
	}
	catch (...)
	{
		if (!nothrow) throw;
		return 0UL;
	}
}