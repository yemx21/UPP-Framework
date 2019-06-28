#pragma once
#include "Uri.h"
namespace System
{
	class UriHelper
	{
	public:
		enum char_class_e:char
		{
			CINV = -2, ///< invalid
			CEND = -1, ///< end delimitor
			CVAL = 0, ///< valid any position
			CVA2 = 1, ///< valid anywhere but 1st position
		};

		struct Traits
		{
			const wchar_t* begin_cstring; ///< begin cstring (or 0 if none)
			const wchar_t begin_char; ///< begin char (or 0 if none)
			const wchar_t end_char; ///< end char (or 0 if none)
			wchar_t char_class[256]; ///< map of char to class
		};

		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Scheme& v);
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Authority& v);
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Path& v);
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Query& v);
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri::Fragment& v);
		static bool Parse(std::wstring::const_iterator& first, std::wstring::const_iterator last, Uri& v);
		static bool ParseLiteral(std::wstring::const_iterator& first, std::wstring::const_iterator last, const wchar_t* v);



		static bool Parse(const Traits& ts, std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& comp, wchar_t* endc = 0);

		static std::wstring Encode(const Traits& ts, const std::wstring& comp);
		static bool Decode(std::wstring& s);

		static const wchar_t ENCODE_BEGIN_CHAR; 
		static const Traits SCHEME_TRAITS; 
		static const Traits AUTHORITY_TRAITS; 
		static const Traits PATH_TRAITS; 
		static const Traits QUERY_TRAITS; 
		static const Traits FRAGMENT_TRAITS; 
	};
}