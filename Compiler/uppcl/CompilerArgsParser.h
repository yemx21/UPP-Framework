#pragma once
#include <set>
#include <vector>
#include <map>
#include "Dispatcher.h"

namespace uppcl
{
	class CompilerArgsParser
	{
	public:
		bool uppsdk;
		std::string uppdir;

		std::vector<std::string> inoutputs;
		std::vector<std::string> inputs;
		std::vector<std::string> outputs;
		bool pch;
		std::string pchfile;
		bool compile;
		std::string asmdef;

		std::string switchflag;

		bool customentry;
		std::string entryheader;
		std::string entrysource;
		std::string assemblyheader;
		std::string assemblysource;
		
		bool document;
		bool slience;

		std::vector<std::string> user_includedirs;
		std::vector<std::string> system_includedirs;
		std::vector<std::string> cpp_includedirs;
		std::set<std::string> org_macros;

		std::map<std::string, bool> macros;

	public:
		CompilerArgsParser();

		bool Parse(int argc, char** argv, std::shared_ptr<Dispatcher>& disp);

		void Dump();

	};
}