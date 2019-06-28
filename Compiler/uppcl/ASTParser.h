#pragma once
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Lex/LexDiagnostic.h>
#include "clang/basic/TargetInfo.h"
#include <clang/Basic/SourceLocation.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/Version.h>

#include "Dispatcher.h"
#include <set>
#include <map>
#include "Utils.h"
#include "Types.h"
#include "Stopwatcher.hpp"

namespace uppcl
{
	class UPPCallbacks : public clang::PPCallbacks
	{
		clang::Preprocessor &PP;

		bool IncludeNotFoundSupressed = false;
		bool ShouldWarnHeaderNotFound = false;

	public:
		struct PragmaUCompiler : public clang::PragmaHandler {
			UPPCallbacks* uppowner;
			explicit PragmaUCompiler() : clang::PragmaHandler("ucompiler"), uppowner(nullptr) {}
			void HandlePragma(clang::Preprocessor &PP, clang::PragmaIntroducerKind Introducer,
				clang::Token &FirstToken) override;
			void SetOwner(UPPCallbacks* callback) { uppowner = callback; }
		};

		PragmaUCompiler ucompilerhandler;

		std::vector<std::string> uimports;
		std::string compilemode;
		

		UPPCallbacks(clang::Preprocessor &PP) : PP(PP)
		{
			ucompilerhandler.SetOwner(this);
			PP.AddPragmaHandler(&ucompilerhandler);
		}

		~UPPCallbacks()
		{
			PP.RemovePragmaHandler(&ucompilerhandler);
		}

	protected:
		bool FileNotFound(llvm::StringRef FileName, llvm::SmallVectorImpl< char >& RecoveryPath) override;
		void InclusionDirective(clang::SourceLocation HashLoc,
			const clang::Token &IncludeTok,
			StringRef FileName,
			bool IsAngled,
			clang::CharSourceRange FilenameRange,
			const clang::FileEntry *File,
			StringRef SearchPath,
			StringRef RelativePath,
			const clang::Module *Imported,
			clang::SrcMgr::CharacteristicKind FileType) override;
	};

	class UASTConsumer_base : public clang::ASTConsumer
	{
	protected:
		clang::CompilerInstance &ci;
		clang::ASTContext *ctx = nullptr;
		clang::PPCallbacks* PPCallbacks = nullptr;
		std::shared_ptr<Dispatcher> Disp;
		std::map<clang::StringRef, std::vector<UClass>> objects;
	public:
		UASTConsumer_base(clang::CompilerInstance &ci, std::shared_ptr<Dispatcher> disp);
		virtual void Initialize(clang::ASTContext& Ctx) override;
		void HandleTagDeclDefinition(clang::TagDecl* D) override;

		UClass ParseClass(clang::CXXRecordDecl* RD, clang::ASTContext* ast, std::shared_ptr<Dispatcher> disp);
	};

	struct UASTConsumer : public UASTConsumer_base
	{
		std::string output;
		std::string header;
		UAssemblyPool& pool;
		std::string AsmFile;
		std::string SwitchFlag;
		HANDLE flagevt;
		std::shared_ptr<Stopwatcher> Counter;

		UASTConsumer(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, clang::CompilerInstance& ci, const std::string out, UAssemblyPool& ass, const std::string& file, HANDLE evt, const std::string& asm_filename, const std::string& switchflag);

		void Initialize(clang::ASTContext& Ctx) override;

		void HandleTagDeclDefinition(clang::TagDecl* D) override;
		void HandleTranslationUnit(clang::ASTContext& Ctx) override;

	};

	class ClangParser
	{
		class Pipe
		{
			std::vector<std::string> SysDirs;
			std::vector<std::string> UserDirs;
			std::vector<std::string> CxxDirs;
			std::shared_ptr<Dispatcher> Disp;
			std::shared_ptr<Stopwatcher> Counter;
			std::string SwitchFlag;
			bool prepareCompilerforFile(const std::string& szSourceCodeFilePath);
		public:
			clang::CompilerInstance compilerinstance_;
			Pipe(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, const std::vector<std::string>& dirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, const std::map<std::string, bool>& macros, const std::string& switchflag);

			bool parseAST(const std::string& input, const std::string& output, llvm::StringRef pch, UAssemblyPool& AssemblyPool, const std::string& asm_filename);
		};

		std::vector<std::string> UserDirs;
		std::vector<std::string> CxxDirs;
		std::vector<std::string> SysDirs;
		std::vector<std::string> UserDirs_utf8;
		std::vector<std::string> CxxDirs_utf8;
		std::vector<std::string> SysDirs_utf8;
		std::map<std::string, bool> Macros;
		UAssemblyPool AssemblyPool;
		std::shared_ptr<Dispatcher> Disp;
		std::shared_ptr<Stopwatcher> Counter;
		std::string SwitchFlag;
		std::string Switch_Definition;
	public:
		ClangParser(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, const std::vector<std::string>& dirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, const std::map<std::string, bool>& macros, const std::string& asmdef, const std::string& asmexclude, const std::string& switchflag);
	public:
		static std::unique_ptr<ClangParser> Create(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, const std::vector<std::string>& dirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, const std::map<std::string, bool>& macros, const std::string& asmdef, const std::string& asmexclude, const std::string& switchflag);

		bool parseAST(std::vector<std::string>& inputs, std::vector<std::string>& outputs, llvm::StringRef pch, const std::string& asmheader);

		bool GenerateAssembly(const std::string& dest_h, const std::string& dest_cpp);
		bool GenerateEntry(const std::string& dest_h, const std::string& dest_cpp, bool customentry, const std::string& asmheader);

		std::vector<std::string>& GeneratedFiles();
	};
}