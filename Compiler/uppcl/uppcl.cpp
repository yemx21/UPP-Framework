#include <iostream>
#include "CompilerArgsParser.h"
#include "Precompiler.h"
#include "ASTParser.h"

using namespace uppcl;


int main(int argc, char** argv)
{
	std::shared_ptr<Dispatcher> dispatcher = std::make_shared<StdWriter>();

	auto argparser = CompilerArgsParser();

	if (!argparser.Parse(argc, argv, dispatcher))
	{
		return -1;
	}
	else
	{
		//argparser.Dump();
		if (argparser.pch)
		{
			auto precompiler = Precompiler::Create(dispatcher, argparser.system_includedirs, argparser.cpp_includedirs, argparser.user_includedirs, argparser.macros);
			std::string outputpch;
			if (!precompiler->Generate(argparser.inputs[0], argparser.outputs[0]))
			{
				return -2;
			}
		}
		else if (argparser.compile)
		{
			std::shared_ptr<Stopwatcher> counter = std::make_shared<Stopwatcher>();
			auto compiler = ClangParser::Create(dispatcher, counter, argparser.system_includedirs, argparser.cpp_includedirs, argparser.user_includedirs, argparser.macros, argparser.asmdef, "", argparser.switchflag);
			std::string outputpch;
			if (!compiler->parseAST(argparser.inputs, argparser.outputs, argparser.pchfile, argparser.assemblyheader))
			{
				return -2;
			}
			else
			{
				if (!compiler->GenerateAssembly(argparser.assemblyheader, argparser.assemblysource))
				{
					return -3;
				}
				else
				{
					if (!compiler->GenerateEntry(argparser.entryheader, argparser.entrysource, argparser.customentry, argparser.assemblyheader))
					{
						return -5;
					}
				}
			}
		}
	}
	return 0;
}

