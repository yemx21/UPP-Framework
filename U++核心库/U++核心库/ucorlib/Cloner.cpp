#include "Cloner.h"

using namespace System;

__inline wchar_t* Cloner::Clone(const wchar_t* str)
{
	if (!str) return NULL;
	size_t memsize = wcslen(str)+1;
	wchar_t* mem = new wchar_t[memsize + 1];
	wcsncpy_s(mem, memsize, str, _TRUNCATE);
	return mem;
}

__inline char* Cloner::Clone(const char* str)
{
	if (!str) return NULL;
	size_t memsize = strlen(str)+1;
	char* mem = new char[memsize];
	strncpy_s(mem, memsize, str, _TRUNCATE);
	return mem;
}

__inline wchar_t* Cloner::Clone(const std::wstring& str)
{
	if (str.empty()) return NULL; 
	size_t size = str.length()+1;
	wchar_t* mem = new wchar_t[size];
	wcsncpy_s(mem, size, str.data(), _TRUNCATE);

	return mem;
}

__inline char* Cloner::Clone(const std::string& str)
{
	if (str.empty()) return NULL;
	size_t size = str.length()+1;
	char* mem = new char[size]; 
	strncpy_s(mem, size, str.data(), _TRUNCATE);
	return mem;
}

__inline wchar_t* Cloner::Clone(const wchar_t* str, UInt32& memSize)
{
	if (!str) return NULL;
	size_t memsize = wcslen(str) + 1;
	wchar_t* mem = new wchar_t[memsize];
	if (&memSize)
	{
		memSize = sizeof(wchar_t)*memsize;
		wcsncpy_s(mem, memsize, str, _TRUNCATE);
	}
	else
		wcsncpy_s(mem, memsize, str, _TRUNCATE);
	return mem;
}

__inline char* Cloner::Clone(const char* str, UInt32& memSize)
{
	if (!str) return NULL;
	size_t memsize = strlen(str) + 1;
	char* mem = new char[memsize];
	if (&memSize)
	{
		memSize = memsize;
		strncpy_s(mem, memsize, str, _TRUNCATE);
	}
	else
		strncpy_s(mem, memsize, str, _TRUNCATE);
	return mem;
}

__inline wchar_t* Cloner::Clone(const std::wstring& str, UInt32& memSize)
{
	if (str.empty()) return NULL;
	size_t size = str.length();
	wchar_t* mem = new wchar_t[size + 1];
	wcsncpy_s(mem, size+1, str.data(), _TRUNCATE);
	if (&memSize) memSize = sizeof(wchar_t)* size;
	return mem;
}


__inline char* Cloner::Clone(const std::string& str, UInt32& memSize)
{
	if (str.empty()) return NULL;
	size_t size = str.length();
	char* mem = new char[size + 1];
	strncpy_s(mem, size+1, str.data(), _TRUNCATE);
	if (&memSize) memSize = size;
	return mem;
}