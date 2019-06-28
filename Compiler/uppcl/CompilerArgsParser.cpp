#include "CompilerArgsParser.h"
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Config.h"
#include <filesystem>
#include "tinyxml2.h"
#include "Utils.h"
#include "args.h"

using namespace uppcl;

#define NSec100_Per_Sec        10000000
#define USec_Per_Sec        1000000
#define USec_Per_MSec        1000
#define NSec_Since_1582        ((unsigned long long)(0x01B21DD213814000))
#include <random>
static GUID TimeGenerateGUID()
{
	GUID guid = {};
	static BOOL        initialized = FALSE;
	static long long    timestamp;
	static unsigned int    advance;
	static unsigned short    clockseq;
	static unsigned short    node_high;
	static unsigned int    node_low;
	long long            time;
	unsigned short            nowseq;
	int                r;
	std::mt19937 tokenizer;
	std::random_device rd;
	tokenizer.seed(rd());
	SYSTEMTIME  tv;

	GetLocalTime(&tv);

	time = ((unsigned long long)tv.wSecond) * USec_Per_Sec + ((unsigned long long)tv.wMilliseconds*USec_Per_MSec);
	time = time * 10 + NSec_Since_1582;

	if (!initialized)
	{
		timestamp = time;
		advance = 0;

		r = tokenizer();

		clockseq = r >> 16;
		node_high = r | 0x0100;

		node_low = tokenizer();

		initialized = TRUE;
	}
	else if (time < timestamp)
	{
		timestamp = time;
		advance = 0;
		clockseq++;
	}
	else if (time == timestamp)
	{
		advance++;
		time += advance;
	}
	else
	{
		timestamp = time;
		advance = 0;
	}
	nowseq = clockseq;

	unsigned int tmp1 = (unsigned int)time;
	unsigned short tmp2 = (unsigned short)((time >> 32) & 0xffff);
	unsigned short tmp3 = (unsigned short)(((time >> 48) & 0x0ffff) | 0x1000);
	unsigned short tmp4 = (unsigned short)((nowseq & 0x3fff) | 0x8000);

	memcpy(&guid, &tmp1, sizeof(unsigned int));
	memcpy(&guid + sizeof(unsigned int), &tmp2, sizeof(unsigned short));
	memcpy(&guid + sizeof(unsigned int) + sizeof(unsigned short), &tmp3, sizeof(unsigned short));
	memcpy(&guid + sizeof(unsigned int) + sizeof(unsigned short) + sizeof(unsigned short), &node_low, sizeof(unsigned int));
	memcpy(&guid + sizeof(unsigned int) + sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned int), &node_high, sizeof(unsigned short));
	memcpy(&guid + sizeof(unsigned int) + sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned int) + sizeof(unsigned short), &tmp4, sizeof(unsigned short));

	return guid;
}

static __inline GUID GenerateHashId128()
{
	GUID result;
	if (S_OK != ::CoCreateGuid(&result)) return TimeGenerateGUID();
	return result;
}

static bool AnalysisSDKDir(const std::string& udir, std::string& err, std::string& diag)
{
	int flag = 0;
	std::string udir_sdk = udir.find_last_of("\\/") != std::string::npos ? udir + "SDK.config" : udir + "\\SDK.config";
	std::string sdkconfig_path = udir_sdk;
	std::string inc_dir;
	std::string inc_sys_dir;
	std::string inc_audio_dir;
	std::string lib_dir;
	if (_access(udir_sdk.c_str(), 4) != -1)
	{
		tinyxml2::XMLDocument doc;
		bool bad = doc.LoadFile(udir_sdk.c_str()) != tinyxml2::XMLError::XML_SUCCESS;
		if (bad)
		{
			flag = 1;
		}
		else
		{
			auto root = doc.RootElement();
			if (!root)
			{
				flag = 2;
			}
			else
			{
				int ver = -1;
				if (root->QueryIntAttribute("Version", &ver) != tinyxml2::XML_NO_ATTRIBUTE)
				{
					if (ver < 0)
					{
						flag = 3;
					}
					else
					{
						if (ver >= USDK_VERSION_MIN && ver <= USDK_VERSION_MAX)
						{
							auto ucompiler = root->FirstChildElement("UCompiler");
							if (ucompiler)
							{
								auto uver = ucompiler->Attribute("Version");
								if (!uver)
								{
									flag = 6;
								}
								else
								{
									if (_stricoll(uver, UCOMPILER_VERSION) != 0)
									{
										flag = 7;
									}
									else
									{
										auto inc = root->FirstChildElement("Include");
										if (inc)
										{
											auto inc_url = inc->Attribute("Url");
											if (inc_url)
											{
												inc_dir = udir + inc_url;
												DWORD dwAttrib = GetFileAttributesA(inc_dir.c_str());
												if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
												{
													flag = 8;
												}
											}
											else
											{
												flag = 9;
											}

											auto inc_sys = inc->FirstChildElement("System");
											if (inc_sys)
											{
												auto inc_sys_url = inc_sys->Attribute("Url");
												if (inc_sys_url)
												{
													inc_sys_dir = udir + inc_sys_url;
													DWORD dwAttrib = GetFileAttributesA(inc_sys_dir.c_str());
													if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
													{
														flag = 10;
													}
												}
												else
												{
													flag = 11;
												}
											}
											else
											{
												flag = 12;
											}


											auto inc_audio = inc->FirstChildElement("Audio");
											if (inc_audio)
											{
												auto inc_audio_url = inc_audio->Attribute("Url");
												if (inc_audio_url)
												{
													inc_audio_dir = udir + inc_audio_url;
													DWORD dwAttrib = GetFileAttributesA(inc_audio_dir.c_str());
													if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
													{
														flag = 13;
													}
												}
												else
												{
													flag = 14;
												}
											}
											else
											{
												flag = 15;
											}

										}
										else
										{
											flag = 16;
										}

										auto lib = root->FirstChildElement("Library");
										if (lib)
										{
											auto lib_url = lib->Attribute("Url");
											if (lib_url)
											{
												lib_dir = udir + lib_url;
												DWORD dwAttrib = GetFileAttributesA(lib_dir.c_str());
												if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
												{
													flag = 17;
												}
											}
											else
											{
												flag = 18;
											}
										}
									}
								}
							}
							else
							{
								flag = 5;
							}
						}
						else
						{
							flag = 4;
						}
					}
				}
				else
				{
					flag = 3;
				}
			}
		}
	}
	else
	{
		err = "can not find file '" + sdkconfig_path + "'";
		return false;
	}
	switch (flag)
	{
	case 1:
		err = "sdk configuration file '" + sdkconfig_path + "' is corrupted, please fix it by advance elsewise install or repair U++SDK";
		break;
	case 2:
		err = "sdk configuration file '" + sdkconfig_path + "' is corrupted, please fix it by looking at its missing root element 'USDK' elsewise install or repair U++SDK";
		break;
	case 3:
		err = "sdk configuration file '" + sdkconfig_path + "' is corrupted, please fix it by looking at its root element's missing attribute 'Version' elsewise install or repair U++SDK";
		break;
	case 4:
		err = "sdk configuration file '" + sdkconfig_path + "' shows unsupported version of U++SDK, please fix it by advance elsewise  install or repair U++SDK";
		break;
	case 5:
		err = "sdk configuration file '" + sdkconfig_path + "' missing ucompiler defination, please fix it by advance elsewise install or repair U++SDK";
		break;
	case 6:
		err = "sdk configuration file '" + sdkconfig_path + "' ucompiler defination missing attribute 'Version', please fix it by advance elsewise install or repair U++SDK";
		break;
	case 7:
		err = "sdk configuration file '" + sdkconfig_path + "' ucompiler defination is not the same version of current running compiler, please fix it by advance elsewise install or repair U++SDK";
		break;
	case 16:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'Include' element";
		break;
	case 8:
		diag = "sdk configuration file '" + sdkconfig_path + "' can not find Url '" + inc_dir + "' in 'Include' element";
		break;
	case 9:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'Url' attribute in 'Include' element";
		break;
	case 10:
		diag = "sdk configuration file '" + sdkconfig_path + "' can not find Url '" + inc_sys_dir + "' in 'System' element of 'Include' element";
		break;
	case 11:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'Url' attribute in 'System' element of 'Include' element";
		break;
	case 12:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'System' element in 'Include' element";
		break;
	case 13:
		diag = "sdk configuration file '" + sdkconfig_path + "' can not find Url '" + inc_audio_dir + "' in 'System' element of 'Include' element";
		break;
	case 14:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'Url' attribute in 'Audio' element of 'Include' element";
		break;
	case 15:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'Audio' element in 'Include' element";
		break;
	case 17:
		diag = "sdk configuration file '" + sdkconfig_path + "' can not find Url '" + lib_dir + "' in 'Library' element";
		break;
	case 18:
		diag = "sdk configuration file '" + sdkconfig_path + "' missing 'Url' attribute in 'Library' element";
		break;
	default:
		break;
	}
	return flag == 0 || flag > 7;
}

static __inline std::string GetExeDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	size_t pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

static bool FindSDKDir(const std::string& udir, const std::string& exclude, std::string& result, std::vector<std::string>& err, std::string& diag)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATAA FindFileData;

	strcpy(szFind, udir.c_str());
	strcat(szFind, "\\*.*");

	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind) return false;

	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				if (_stricoll(exclude.c_str(), FindFileData.cFileName) != 0)
				{
					char szFile[MAX_PATH];
					strcpy(szFile, udir.c_str());
					strcat(szFile, FindFileData.cFileName);
					strcat(szFile, "\\");
					std::string tmp_err;
					std::string tmp_diag;
					if (AnalysisSDKDir(szFile, tmp_err, tmp_diag))
					{
						diag = tmp_diag;
						result = szFile;
						break;
					}
					else
					{
						err.push_back(tmp_err);
					}
				}
			}
		}
		if (!FindNextFileA(hFind, &FindFileData))    break;
	}
	FindClose(hFind);
	return !result.empty();
}

static bool GetSDKDir(std::string& sdir, std::vector<std::string>& err, std::vector<std::string>& diag)
{
	int flag = 0;
	std::string upp_dir = getenv("UPPSDK_DIR");
	utils::RTrim(upp_dir);
	if (upp_dir.empty()) flag = 1;
	else
	{
		if (upp_dir.find_last_of("\\") != std::string::npos)
		{
			upp_dir += "\\";
		}

		DWORD dwAttrib = GetFileAttributesA(upp_dir.c_str());
		if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
		{
			flag = 2;
		}

		std::vector<std::string> t_err;
		std::string t_diag;
		std::string dir_find;
		if (FindSDKDir(upp_dir, "", dir_find, t_err, t_diag))
		{
			if (!t_diag.empty()) diag.push_back(t_diag);
			sdir = dir_find;
			flag = 0;
		}
		else
		{
			std::copy(t_err.begin(), t_err.end(), std::back_inserter(err));
			flag = -1;
		}
	}

	if (flag != 0)
	{
		std::string exeDir = GetExeDir();
		size_t pos = std::string(exeDir).find_last_of("\\/");
		if (pos != std::string::npos)
		{
			std::string udir = exeDir.substr(0, pos) + "\\";
			std::string tmp_err;
			std::string tmp_diag;
			if (AnalysisSDKDir(udir, tmp_err, tmp_diag))
			{
				sdir = udir;
				if (!tmp_diag.empty()) diag.push_back(tmp_diag);
			}
			else
			{
				err.push_back(tmp_err);
				size_t pos_prev = std::string(exeDir.substr(0, pos)).find_last_of("\\/");
				if (pos_prev != std::string::npos)
				{
					std::string subver_udir = exeDir.substr(pos + 1, pos - pos_prev - 1);
					std::string udir_prev = exeDir.substr(0, pos_prev);
					std::string result_udir;
					std::vector<std::string> tmp_err1;
					std::string tmp_diag1;
					if (FindSDKDir(udir_prev, subver_udir, result_udir, tmp_err1, tmp_diag1))
					{
						if (!tmp_diag1.empty())diag.push_back(tmp_diag1);
						sdir = result_udir;
						flag = 0;
					}
					else
					{
						std::copy(tmp_err1.begin(), tmp_err1.end(), std::back_inserter(err));
						flag = 4;
					}
				}
			}
		}
		else
		{
			flag = 3;
		}
	}

	switch (flag)
	{
	case 1:
		err.push_back("system enviornment variable 'UPPSDK_DIR' not defined, please install or repair U++SDK");
		break;
	case 2:
		err.push_back("system enviornment variable 'UPPSDK_DIR' is not existed, please install or repair U++SDK");
		break;
	case 3:
		err.push_back("no relevent directory for auto scan");
		break;
	case 4:
		err.push_back("no relevent directory for current ucompiler");
		break;
	default:
		break;
	}

	return flag == 0;
}

CompilerArgsParser::CompilerArgsParser(): pch(false), compile(false), customentry(false), document(false), uppsdk(false), slience(false)
{

}

bool CompilerArgsParser::Parse(int argc, char** argv, std::shared_ptr<Dispatcher>& disp)
{
	cl::Cmdline cli("UPP Compiler", "This a source to source compiler for reflection and documentation", false);
	cli.Add("I", "directory of include headers", cl::Multiple::yes | cl::MayGroup::yes | cl::Arg::optional, cl::Var(user_includedirs));
	cli.Add("IS", "directory of operating system include headers", cl::Multiple::yes | cl::MayGroup::yes | cl::Arg::optional, cl::Var(system_includedirs));
	cli.Add("IC", "directory of C++ include headers", cl::Multiple::yes | cl::MayGroup::yes | cl::Arg::optional, cl::Var(cpp_includedirs));
	cli.Add("D", "predefined macro", cl::Multiple::yes | cl::Arg::optional, cl::Var(org_macros));
	cli.Add("inoutput", "input output file pair or input output file pairs", cl::Arg::required | cl::Multiple::yes, cl::Var(inoutputs));
	cli.Add("emitpch", "generate PCH file", cl::Arg::optional, cl::Var(pch));
	cli.Add("pch", "generated PCH file", cl::Arg::optional, cl::Var(pchfile));
	cli.Add("switchflag", "unique flag for this specified settings", cl::Arg::optional, cl::Var(switchflag));
	cli.Add("gen", "generate upp headers and code files", cl::Arg::optional, cl::Var(compile));
	cli.Add("asm", "assembly", cl::Arg::optional, cl::Var(asmdef));
	cli.Add("customentry", "enable custom entry functions", cl::Arg::optional, cl::Var(customentry));
	cli.Add("entryheader", "main entry header file", cl::Arg::optional, cl::Var(entryheader));
	cli.Add("entrysource", "main entry source file", cl::Arg::optional, cl::Var(entrysource));
	cli.Add("asmheader", "assembly entry header file", cl::Arg::optional, cl::Var(assemblyheader));
	cli.Add("asmsource", "assembly entry source file", cl::Arg::optional, cl::Var(assemblysource));
	cli.Add("doc", "generate documentation", cl::Arg::optional, cl::Var(document));
	cli.Add("check", "check status of SDK", cl::Arg::optional, cl::Var(uppsdk));
	cli.Add("slience", "set all warnings off", cl::Arg::optional, cl::Var(slience));

	auto res = cli.Parse(argv + 1, argv + argc, cl::CheckMissingOptions::yes);
	if (!res)
	{
		const auto diags = cli.Diag();
		for (const auto& d : diags)
		{
			switch (d.type)
			{
			case cl::Diagnostic::error:
				disp->SendError("", -1, -1, d.message);
				break;
			case cl::Diagnostic::warning:
				disp->SendWarning("", -1, -1, d.message);
				break;
			case cl::Diagnostic::note:
				disp->SendTip("", -1, -1, d.message);
				break;
			default:
				disp->SendOutput(d.message);
				break;
			}
		}
	}
	else
	{
		disp->SetSlience(slience);
		if (uppsdk)
		{
			std::vector<std::string> sdk_err, sdk_diag;
			if (!GetSDKDir(uppdir, sdk_err, sdk_diag))
			{
				for (const auto& msg : sdk_err)
					disp->SendError("", -1, -1, msg);
				for (const auto& msg : sdk_diag)
					disp->SendWarning("", -1, -1, msg);

				return false;
			}
		}
	}
	
	inputs.clear();
	outputs.clear();
	for (const auto& inout_str : inoutputs)
	{
		std::vector<std::string> in_out;
		utils::Split(inout_str, std::string(">"), in_out);
		if (in_out.size() == 2)
		{
			inputs.push_back(utils::Trim(in_out[0]));
			outputs.push_back(utils::Trim(in_out[1]));
		}
		else
		{
			disp->SendError("", -1, -1, "value of inoutput " + inout_str + " is invalid");
			return false;
		}
	}

	bool marco_success = true;
	macros.clear();
	for (const auto& macro_str : org_macros)
	{
		auto marco = utils::Trim(macro_str);
		std::queue<std::string> kv;
		utils::Split<char>(marco, "=", kv);
		if (kv.size() == 1)
		{
			macros.insert(std::make_pair(utils::Trim(kv.front()), true));
		}
		else if (kv.size() == 2)
		{
			bool vsuccess = true;
			std::string vstr = utils::Trim(kv.back());
			bool val = false;
			if (vstr == "true" || vstr == "True" || vstr == "TRUE")
				val = true;
			else if (vstr == "false" || vstr == "False" || vstr == "FALSE")
				val = false;
			else
			{
				int vint = -1;
				if (utils::TryParseInt32(vstr, vint))
				{
					if (vint == 1)
						val = true;
					else if (vint == 0)
						val = false;
					else
					{
						disp->SendWarning("", -1, -1, "value of macro " + macro_str + " is invalid and hence ignored");
						val = false;
					}
				}
				else
				{
					vsuccess = false;
				}
			}

			if (vsuccess) macros.insert(std::make_pair(utils::Trim(kv.front()), val));
		}
		else
		{
			disp->SendError("", -1, -1, "macro " + macro_str + " is invalid");
			marco_success = false;
		}
	}
	if (!marco_success) return false;

	if (compile)
	{
		bool success = true;
		if (inputs.size() != outputs.size())
		{
			disp->SendError("", -1, -1, "input size is not equal to output size");
			success = true;
		}
		if (switchflag.empty())
		{
			disp->SendError("", -1, -1, "switchflag is not specified");
			success = false;
		}
		if (entryheader.empty())
		{
			disp->SendError("", -1, -1, "entryheader is not specified");
			success = false;
		}
		if (entrysource.empty())
		{
			disp->SendError("", -1, -1, "entrysource is not specified");
			success = false;
		}
		if (assemblyheader.empty())
		{
			disp->SendError("", -1, -1, "asmheader is not specified");
			success = false;
		}
		if (assemblysource.empty())
		{
			disp->SendError("", -1, -1, "asmsource is not specified");
			success = false;
		}
		if (!success) return false;
	}

	return res.operator bool();
}

void CompilerArgsParser::Dump()
{
	std::cout << "uppsdk: " << uppsdk <<std::endl;
	std::cout << "uppdir: " << uppdir << std::endl;

	std::cout << "switchflag: " << switchflag << std::endl;

	std::cout << "inputs: " << std::endl;
	for (const auto& input : inputs)
		std::cout << input << std::endl;

	std::cout << "outputs: " << std::endl;
	for (const auto& output : outputs)
		std::cout << output << std::endl;

	std::cout << "pch: " << pch << std::endl;
	std::cout << "pch file: " << pchfile << std::endl;
	std::cout << "compile: " << compile << std::endl;
	std::cout << "assembly definition: " << asmdef << std::endl;
	std::cout << "document: " << document << std::endl;
	std::cout << "slience: " << slience << std::endl;

	std::cout << "assembly header file: " << assemblyheader << std::endl;
	std::cout << "assembly source file: " << assemblysource << std::endl;

	std::cout << "custom entry: " << customentry << std::endl;
	std::cout << "entry header file: " << entryheader << std::endl;
	std::cout << "entry source file: " << entrysource << std::endl;

	std::cout << "user include dirs:" << std::endl;
	for (const auto& dir : user_includedirs)
		std::cout << dir << std::endl;

	std::cout << "system include dirs:" << std::endl;
	for (const auto& dir : system_includedirs)
		std::cout << dir << std::endl;

	std::cout << "cpp include dirs:" << std::endl;
	for (const auto& dir : cpp_includedirs)
		std::cout << dir << std::endl;

	std::cout << "macros:" << std::endl;
	for (const auto& marco : macros)
		std::cout << marco.first << '=' << marco.second<< std::endl;

}