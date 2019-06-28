#include "Converter.h"
using namespace System;

bool Converter::Convert(const char* str, std::wstring& wresult, const std::locale& loc)
{
	std::char_traits<wchar_t>::state_type state = { 0 };

	typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	size_t ssize = strlen(str);
	char const* nextToRead = str;
	wchar_t buffer[BUFSIZ];
	wchar_t* nextToWrite;
	std::codecvt_base::result result;
	std::wstring wresult_0;

	while ((result
		= converter.in
		(state,
		nextToRead, str + ssize, nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		wresult_0.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return false;
	}
	wresult_0.append(buffer, nextToWrite);
	wresult.append(wresult_0);
	return true;
}

bool Converter::Convert(const wchar_t* str, std::string& aresult, const std::locale& loc)
{
	std::char_traits<char>::state_type state = { 0 };

	typedef std::codecvt<char, wchar_t, std::char_traits<char>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	size_t ssize = wcslen(str);
	wchar_t const* nextToRead = str;
	char buffer[BUFSIZ];
	char* nextToWrite;
	std::codecvt_base::result result;
	std::string aresult_0;

	while ((result
		= converter.in
		(state,
		nextToRead, str + ssize, nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		aresult_0.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return false;
	}
	aresult_0.append(buffer, nextToWrite);
	aresult.append(aresult_0);
	return true;
}

bool Converter::Convert(std::string const& str, std::wstring& wresult, const std::locale& loc)
{
	std::char_traits<wchar_t>::state_type state = { 0 };

	typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	char const* nextToRead = str.data();
	wchar_t buffer[BUFSIZ];
	wchar_t* nextToWrite;
	std::codecvt_base::result result;
	std::wstring wresult_0;

	while ((result
		= converter.in
		(state,
		nextToRead, str.data() + str.size(), nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		wresult_0.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return false;
	}
	wresult_0.append(buffer, nextToWrite);
	wresult.append(wresult_0);
	return true;
}

bool Converter::Convert(std::wstring const& str, std::string& aresult, const std::locale& loc)
{
	std::char_traits<char>::state_type state = { 0 };

	typedef std::codecvt<char, wchar_t, std::char_traits<char>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	wchar_t const* nextToRead = str.data();
	char buffer[BUFSIZ];
	char* nextToWrite;
	std::codecvt_base::result result;
	std::string aresult_0;

	while ((result
		= converter.in
		(state,
		nextToRead, str.data() + str.size(), nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		aresult_0.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return false;
	}
	aresult_0.append(buffer, nextToWrite);
	aresult.append(aresult_0);
	return true;
}

wchar_t* Converter::CPlusNew(const char* str, const _locale_t & loc_t)
{
	
	size_t convertedChars = 0;
	if(_mbstowcs_s_l(&convertedChars, nullptr, 0, str, _TRUNCATE, loc_t) != 0) return nullptr;
	wchar_t * wcstring = new wchar_t[convertedChars+1];
	if (_mbstowcs_s_l(&convertedChars, wcstring, convertedChars, str, _TRUNCATE, loc_t) != 0)
	{
		SAFE_DELETE(wcstring);
	}
	return wcstring;
}

char* Converter::CPlusNew(const wchar_t* str, const _locale_t & loc_t)
{
	size_t convertedChars = 0;
	if (_wcstombs_s_l(&convertedChars, nullptr, 0, str, _TRUNCATE, loc_t) != 0) return nullptr;
	char* cstring = new char[convertedChars+1];
	if (_wcstombs_s_l(&convertedChars, cstring, convertedChars, str, _TRUNCATE, loc_t) != 0)
	{
		SAFE_DELETE(cstring);
	}
	return cstring;
}

std::wstring Converter::Convert(const char* str, const std::locale& loc, const _locale_t& loc_t)
{
	std::wstring tmp;
	std::char_traits<wchar_t>::state_type state = { 0 };

	typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	size_t ssize = strlen(str);
	char const* nextToRead = str;
	wchar_t buffer[BUFSIZ];
	wchar_t* nextToWrite;
	std::codecvt_base::result result;
	while ((result
		= converter.in
		(state,
		nextToRead, str + ssize, nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		tmp.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return std::wstring(CPlusNew(str, loc_t));
	}

	tmp.append(buffer, nextToWrite);
	return tmp;

}

std::string Converter::Convert(const wchar_t* str, const std::locale& loc, const _locale_t& loc_t)
{
	std::string tmp;
	std::char_traits<char>::state_type state = { 0 };

	typedef std::codecvt<char, wchar_t, std::char_traits<char>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	size_t ssize = wcslen(str);
	wchar_t const* nextToRead = str;
	char buffer[BUFSIZ];
	char* nextToWrite;
	std::codecvt_base::result result;

	while ((result
		= converter.in
		(state,
		nextToRead, str + ssize, nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		tmp.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return std::string(CPlusNew(str, loc_t));
	}
	tmp.append(buffer, nextToWrite);
	return tmp;
}

wchar_t* Converter::ConvertNew(const char* str, const std::locale& loc, const _locale_t& loc_t)
{
	std::wstring tmp;
	std::char_traits<wchar_t>::state_type state = { 0 };

	typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	size_t ssize = strlen(str);
	char const* nextToRead = str;
	wchar_t buffer[BUFSIZ];
	wchar_t* nextToWrite;
	std::codecvt_base::result result;
	while ((result
		= converter.in
		(state,
		nextToRead, str + ssize, nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		tmp.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return CPlusNew(str, loc_t);
	}

	tmp.append(buffer, nextToWrite);

	size_t nsize = tmp.size();
	wchar_t* restr = new wchar_t[nsize+1];
	wcsncpy_s(restr, nsize, tmp.data(), _TRUNCATE);
	return restr;

}

char* Converter::ConvertNew(const wchar_t* str, const std::locale& loc, const _locale_t& loc_t)
{
	std::string tmp;
	std::char_traits<char>::state_type state = { 0 };

	typedef std::codecvt<char, wchar_t, std::char_traits<char>::state_type >
		ConverterFacet;

	ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

	size_t ssize = wcslen(str);
	wchar_t const* nextToRead = str;
	char buffer[BUFSIZ];
	char* nextToWrite;
	std::codecvt_base::result result;

	while ((result
		= converter.in
		(state,
		nextToRead, str + ssize, nextToRead,
		buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
		== std::codecvt_base::partial)
	{
		tmp.append(buffer, nextToWrite);
	}

	if (result == std::codecvt_base::error)
	{
		return CPlusNew(str, loc_t);
	}
	tmp.append(buffer, nextToWrite);
	size_t nsize = tmp.size();
	char* restr = new char[nsize+1];
	strncpy_s(restr, nsize, tmp.data(), _TRUNCATE);
	return restr;
}



std::string Converter::StringToUTF8(const std::wstring& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(src);
}

std::wstring Converter::UTF8ToString(const std::string& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(src);
}

std::string Converter::StringToUTF16(const std::wstring& src)
{
	std::wstring_convert<std::codecvt_utf16<wchar_t>> conv;
	return conv.to_bytes(src);
}

std::wstring Converter::UTF16ToString(const std::string& src)
{
	std::wstring_convert<std::codecvt_utf16<wchar_t> > conv;
	return conv.from_bytes(src);
}
