#pragma once

#include <objbase.h>
#include <string>
#include <array>
#define GUID_HELPER_DEFINE_GUIDSTRUCT(g, n) struct __declspec(uuid(g)) n

#define GUID_HELPER_DEFINE_GUIDNAMED(n) n

GUID_HELPER_DEFINE_GUIDSTRUCT("00000000-0000-0000-0000-000000000000", GUID_NULL);

#define GUID_HELPER_GUID_NULL GUID_HELPER_DEFINE_GUIDNAMED(GUID_NULL)

namespace System
{
	 class GuidHelper
	{
	public:

		template<class T>
		static basic_string<T> ToString(const GUID guid);

		/// <summary>
		/// 转换为字符串
		/// </summary>
		/// <param name="guid">唯一标识</param>
		/// <returns></returns>
		template<>
		static basic_string<char> ToString<char>(const GUID guid)
		{
			char output[37];
			_snprintf_s(output, sizeof(output), "%08X-%04hX-%04hX-%02X%02X-%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
			return basic_string<char>(output);
		}

		/// <summary>
		/// 转换为字符串
		/// </summary>
		/// <param name="guid">唯一标识</param>
		/// <returns></returns>
		template<>
		static basic_string<wchar_t> ToString<wchar_t>(const GUID guid)
		{
			wchar_t output[37];
			_snwprintf_s(output, sizeof(output), L"%08X-%04hX-%04hX-%02X%02X-%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
			return basic_string<wchar_t>(output);
		}

		template<class T>
		static bool FromString(GUID& guid, basic_string<T> str);

		/// <summary>
		/// 从字符串转化
		/// </summary>
		/// <param name="guid">唯一标识</param>
		/// <param name="str">字符串</param>
		/// <returns></returns>
		template<>
		static bool FromString<char>(GUID& guid, basic_string<char> str)
		{
			unsigned long p0;
			int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
			int err = sscanf_s(str.c_str(), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);
			if (err == 11)
			{
				guid.Data1 = p0;
				guid.Data2 = p1;
				guid.Data3 = p2;
				guid.Data4[0] = p3;
				guid.Data4[1] = p4;
				guid.Data4[2] = p5;
				guid.Data4[3] = p6;
				guid.Data4[4] = p7;
				guid.Data4[5] = p8;
				guid.Data4[6] = p9;
				guid.Data4[7] = p10;
				return true;
			}
			return false;
		}

		/// <summary>
		/// 从字符串转换
		/// </summary>
		/// <param name="guid">唯一标识</param>
		/// <param name="str">字符串</param>
		/// <returns></returns>
		template<>
		static bool FromString<wchar_t>(GUID& guid, basic_string<wchar_t> str)
		{
			unsigned long p0;
			int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

			int err = swscanf_s(str.c_str(), L"%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);
			if (err == 11)
			{
				guid.Data1 = p0;
				guid.Data2 = p1;
				guid.Data3 = p2;
				guid.Data4[0] = p3;
				guid.Data4[1] = p4;
				guid.Data4[2] = p5;
				guid.Data4[3] = p6;
				guid.Data4[4] = p7;
				guid.Data4[5] = p8;
				guid.Data4[6] = p9;
				guid.Data4[7] = p10;
				return true;
			}
			return false;
		}
	};
}

