#pragma once
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Lex/LexDiagnostic.h>
#include "clang/basic/TargetInfo.h"
#include "Dispatcher.h"
#include <set>
#include <map>
#include "Utils.h"
#include "Stopwatcher.hpp"

namespace uppcl
{
	class PCHConsumer : public clang::ASTConsumer
	{
	public:
		PCHConsumer() {}
	};

	struct UDiagConsumer : clang::DiagnosticConsumer
	{
		std::unique_ptr<DiagnosticConsumer> Proxy;
		std::vector<std::string> ExcludeDirs;
		std::shared_ptr<Dispatcher> Disp;
		int HadRealError = 0;
		std::string suffix;
		UDiagConsumer(std::unique_ptr<DiagnosticConsumer> Previous, std::shared_ptr<Dispatcher> disp, const std::vector<std::string>& sdirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, bool compile = true);
		void BeginSourceFile(const clang::LangOptions& LangOpts, const clang::Preprocessor* PP = 0) override;
		void clear() override;
		void EndSourceFile() override;
		void finish() override;
		void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic& Info) override;
	};

	class Precompiler
	{
	private:
		Stopwatcher watcher;
		clang::CompilerInstance compilerinstance_;
		std::vector<std::string> SysDirs;
		std::vector<std::string> UserDirs;
		std::vector<std::string> CxxDirs;
		std::shared_ptr<Dispatcher> Disp;
		bool prepareCompilerforFile(const std::string& szSourceCodeFilePath);
	public:
		static std::unique_ptr<Precompiler> Create(std::shared_ptr<Dispatcher> disp, const std::vector<std::string> incs, const std::vector<std::string>& cincs, const std::vector<std::string>& uincs, const std::map<std::string, bool>& macros);

		Precompiler(std::shared_ptr<Dispatcher> disp, const std::vector<std::string>& incs, const std::vector<std::string>& cincs, const std::vector<std::string>& uincs, const std::map<std::string, bool>& macros);

		bool Generate(const std::string& precompileheader, const std::string& pchfile);

	};
}