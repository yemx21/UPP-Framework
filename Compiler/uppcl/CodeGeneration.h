#pragma once
#include <string>
#include "Types.h"
#include "Dispatcher.h"
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>

namespace uppcl
{
	class UCodeGenerator
	{
	public:
		static std::string GetTypeString(const clang::QualType& ty, const clang::ASTContext& ctx);
		static std::string GetTypeString(const clang::QualType& ty, clang::PrintingPolicy policy);

		static bool Generate(std::shared_ptr<Dispatcher> disp, const std::string& dest, const std::pair< clang::StringRef, std::vector<UClass>>& file, clang::CompilerInstance& ci, UAssemblyPool& pool, const std::string& asm_file, const std::vector<std::string>& extraheaders, const std::string& mode, const std::string& switchflag);
		static bool Generate(std::shared_ptr<Dispatcher> disp, const std::string& dest_h, const std::string& dest_cpp, UAssemblyPool& pool, const std::string& switchflag, const std::string& switch_definition);
		static bool Generate(std::shared_ptr<Dispatcher> disp, const std::string& dest_h, const std::string& dest_cpp, bool customentry, const std::string& asmfile, UAssemblyPool& pool, const std::string& switchflag);
	};
}