#include <Windows.h>
#include "Precompiler.h"
#include <sstream>
#include "clang/Parse/ParseAST.h"
#include "llvm/Bitcode/BitstreamWriter.h"
#include "clang/Serialization/ASTWriter.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/CrashRecoveryContext.h"
#include <clang/Serialization/ModuleFileExtension.h>
#include <filesystem>

using namespace uppcl;

UDiagConsumer::UDiagConsumer(std::unique_ptr<DiagnosticConsumer> Previous, std::shared_ptr<Dispatcher> disp, const std::vector<std::string>& sdirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, bool compile) : Disp(disp)
{
	Proxy = std::move(Previous);
	for (auto s : sdirs)
	{
		ExcludeDirs.push_back(utils::string_To_UTF8(s));
	}
	for (auto c : cdirs)
	{
		ExcludeDirs.push_back(utils::string_To_UTF8(c));
	}
	for (auto u : udirs)
	{
		ExcludeDirs.push_back(utils::string_To_UTF8(u));
	}
	suffix = compile ? "%(Compiler): " : "%(PreCompiler): ";
}

void UDiagConsumer::BeginSourceFile(const clang::LangOptions& LangOpts, const clang::Preprocessor* PP)
{
	Proxy->BeginSourceFile(LangOpts, PP);
}

void UDiagConsumer::clear()
{
	Proxy->clear();
}

void UDiagConsumer::EndSourceFile()
{
	Proxy->EndSourceFile();
}

void UDiagConsumer::finish()
{
	Proxy->finish();
}

void UDiagConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic& Info)
{
	auto DiagId = Info.getID();
	auto Cat = Info.getDiags()->getDiagnosticIDs()->getCategoryNumberForDiag(DiagId);


	bool ShouldReset = false;

	if (DiagLevel >= clang::DiagnosticsEngine::Error)
	{
		auto fileid = Info.getSourceManager().getFileID(Info.getLocation());
		auto chara = Info.getSourceManager().getFileCharacteristic(Info.getLocation());
		if (chara != clang::SrcMgr::CharacteristicKind::C_User)
		{
			if (!HadRealError)
				ShouldReset = true;
			DiagLevel = clang::DiagnosticsEngine::Ignored;
		}
		else
		{
			const clang::FileEntry* fileentry = Info.getSourceManager().getFileEntryForID(fileid);
			if (fileentry)
			{
				auto filedir = fileentry->getDir();
				if (filedir)
				{
					const char* dir_str = filedir->getName().data();
					if (dir_str)
					{
						for (std::string& d : ExcludeDirs)
						{
							if (::memcmp(dir_str, d.c_str(), d.length()) == 0)
							{
								if (!HadRealError)
									ShouldReset = true;
								DiagLevel = clang::DiagnosticsEngine::Ignored;
								break;
							}
						}
					}
				}
			}
			else
			{
				if (!HadRealError)
					ShouldReset = true;
				DiagLevel = clang::DiagnosticsEngine::Ignored;
			}
		}
		if (DiagLevel != clang::DiagnosticsEngine::Ignored)
		{
			switch (DiagId)
			{
			case clang::diag::err_defined_macro_name:
			case clang::diag::err_expected:
			{
				if (!HadRealError)
					ShouldReset = true;
				DiagLevel = clang::DiagnosticsEngine::Ignored;
			}
			break;
			}
		}
		if (DiagLevel != clang::DiagnosticsEngine::Ignored)
		{
			if (Cat == 2 || Cat == 4 || DiagId == clang::diag::err_param_redefinition || DiagId == clang::diag::err_pp_expr_bad_token_binop)
			{
				if (!HadRealError)
					ShouldReset = true;
				DiagLevel = clang::DiagnosticsEngine::Ignored;
			}
			else
			{
				HadRealError++;
			}
		}
	}

	DiagnosticConsumer::HandleDiagnostic(DiagLevel, Info);
	if (DiagLevel != clang::DiagnosticsEngine::Ignored)
	{
		Proxy->HandleDiagnostic(DiagLevel, Info);

		auto filename = Info.getSourceManager().getFilename(Info.getLocation());

		auto src_loc = Info.getSourceManager().getSpellingLoc(Info.getLocation());

		int line = -1;
		int column = -1;
		bool line_success = false;
		int tmp_line = Info.getSourceManager().getSpellingLineNumber(src_loc, &line_success);
		if (line_success) line = tmp_line;

		bool column_success = false;
		int tmp_column = Info.getSourceManager().getSpellingColumnNumber(src_loc, &column_success);
		if (column_success) column = tmp_column;

		std::string path;
		if (!filename.empty()) path = filename.str();

		llvm::SmallString<128> des;
		Info.FormatDiagnostic(des);

		Disp->SendOutput(path + " " + std::to_string(line) + " " + std::to_string(column) + " " + suffix + des.str().str());

		if (DiagLevel == clang::DiagnosticsEngine::Error)
			Disp->SendError(path, line, column, suffix + des.str().str());
		else if (DiagLevel == clang::DiagnosticsEngine::Warning)
			Disp->SendWarning(path, line, column, suffix + des.str().str());
	}

	if (ShouldReset)
	{
		const_cast<clang::DiagnosticsEngine *>(Info.getDiags())->Reset();
	}
}


bool Precompiler::prepareCompilerforFile(const std::string& szSourceCodeFilePath)
{
	compilerinstance_.getSourceManager().clearIDTables();
	const clang::FileEntry * file = compilerinstance_.getFileManager().getFile(szSourceCodeFilePath);
	if (!file) return false;
	clang::FileID fileID = compilerinstance_.getSourceManager().createFileID(file, clang::SourceLocation(), clang::SrcMgr::CharacteristicKind::C_User);
	if (fileID.isInvalid()) return false;
	compilerinstance_.getSourceManager().setMainFileID(fileID);
	clang::FrontendOptions& feOptions = compilerinstance_.getFrontendOpts();
	feOptions.Inputs.clear();
	feOptions.Inputs.push_back(clang::FrontendInputFile(szSourceCodeFilePath, clang::InputKind::CXX, false));
	return true;
}

std::unique_ptr<Precompiler> Precompiler::Create(std::shared_ptr<Dispatcher> disp, const std::vector<std::string> incs, const std::vector<std::string>& cincs, const std::vector<std::string>& uincs, const std::map<std::string, bool>& macros)
{
	return std::make_unique<Precompiler>(disp, incs, cincs, uincs, macros);
}

Precompiler::Precompiler(std::shared_ptr<Dispatcher> disp, const std::vector<std::string>& incs, const std::vector<std::string>& cincs, const std::vector<std::string>& uincs, const std::map<std::string, bool>& macros): Disp(disp)
{
	SysDirs = incs;
	UserDirs = uincs;
	CxxDirs = cincs;

	clang::CompilerInvocation& invocation = compilerinstance_.getInvocation();

	compilerinstance_.createDiagnostics();

	clang::HeaderSearchOptions & headerSearchOptions = compilerinstance_.getHeaderSearchOpts();

	for (auto inc : SysDirs)
	{
		headerSearchOptions.AddPath(utils::string_To_UTF8(inc), clang::frontend::IncludeDirGroup::System, false, false);
	}

	for (auto inc : CxxDirs)
	{
		headerSearchOptions.AddPath(utils::string_To_UTF8(inc), clang::frontend::IncludeDirGroup::CXXSystem, false, false);
	}

	for (auto inc : UserDirs)
	{
		headerSearchOptions.AddPath(utils::string_To_UTF8(inc), clang::frontend::IncludeDirGroup::Angled, false, false);
	}

	headerSearchOptions.UseStandardCXXIncludes = 1;
	headerSearchOptions.UseStandardSystemIncludes = 1;

	clang::LangOptions& langOptions = compilerinstance_.getLangOpts();
	langOptions.CPlusPlus = 1;
	langOptions.Bool = 1;
	langOptions.CPlusPlus11 = 1;
	langOptions.DollarIdents = 1;
	langOptions.CPlusPlus14 = 1;
	langOptions.CPlusPlus17 = 1;
#if defined(_MSC_VER)
	langOptions.MicrosoftExt = 1;
	langOptions.UPlusPlusExt = 1;
	langOptions.MSVCCompat = 1;
	langOptions.DeclSpecKeyword = 1;
	langOptions.MSCompatibilityVersion = 190000000;
	langOptions.AsmBlocks = 1;
	langOptions.CXXExceptions = 1;
	langOptions.CXXOperatorNames = 1;
	langOptions.DelayedTemplateParsing = 1;
	langOptions.RelaxedTemplateTemplateArgs = 1;

	compilerinstance_.getDiagnosticOpts().setFormat(clang::TextDiagnosticFormat::MSVC);
#endif
	compilerinstance_.getDiagnosticOpts().IgnoreWarnings = 1;
	compilerinstance_.getDiagnosticOpts().ShowNoteIncludeStack = 0;
	compilerinstance_.getDiagnosticOpts().NoRewriteMacros = 1;
	compilerinstance_.getDiagnostics().setIgnoreAllWarnings(true);
	compilerinstance_.getDiagnostics().setExtensionHandlingBehavior(clang::diag::Severity::Ignored);
	if (disp->GetSlience())
	{
		compilerinstance_.getDiagnostics().setSeverityForAll(clang::diag::Flavor::WarningOrError, clang::diag::Severity::Ignored);
		compilerinstance_.getDiagnostics().setSuppressSystemWarnings(true);
		compilerinstance_.getDiagnostics().setSuppressAllDiagnostics(true);
	}
	compilerinstance_.getDiagnostics().setClient(new UDiagConsumer{ compilerinstance_.getDiagnostics().takeClient(), Disp, SysDirs, CxxDirs, UserDirs, false });
	compilerinstance_.getFrontendOpts().ProgramAction = clang::frontend::ActionKind::GeneratePCH;
	compilerinstance_.getFrontendOpts().SkipFunctionBodies = 1;
	compilerinstance_.getFrontendOpts().RelocatablePCH = 1;

	compilerinstance_.createFileManager();
	compilerinstance_.createSourceManager(compilerinstance_.getFileManager());

	compilerinstance_.getTargetOpts().Triple = llvm::sys::getDefaultTargetTriple();
	compilerinstance_.setTarget(clang::TargetInfo::CreateTargetInfo(compilerinstance_.getDiagnostics(), std::make_shared<clang::TargetOptions>(compilerinstance_.getTargetOpts())));

	for (const auto& macro : macros)
	{
		if (macro.second)
			compilerinstance_.getPreprocessorOpts().addMacroDef(macro.first);
		else
			compilerinstance_.getPreprocessorOpts().addMacroUndef(macro.first);
	}

	compilerinstance_.createPreprocessor(clang::TranslationUnitKind::TU_Module);

	compilerinstance_.getPreprocessor().enableIncrementalProcessing(true);
	compilerinstance_.getPreprocessor().SetSuppressIncludeNotFoundError(true);


	compilerinstance_.createASTContext();

	if (compilerinstance_.hasPreprocessor())
	{
		clang::Preprocessor & preprocessor = compilerinstance_.getPreprocessor();
		preprocessor.getBuiltinInfo().initializeBuiltins(preprocessor.getIdentifierTable(), preprocessor.getLangOpts());
	}
}


bool Precompiler::Generate(const std::string& precompileheader, const std::string& pchfile)
{
	watcher.Restart();
	Disp->SendTip(precompileheader, -1, -1, std::string("%(PreCompiler): %(Start)"));

	std::stringstream outstr;
	outstr << "%(PreCompiler)%(StartAnalysis)%(Header) " << precompileheader;
	Disp->SendOutput(outstr.str());

	auto outdir = std::filesystem::path(pchfile).parent_path().string();

	DWORD dwAttrib = GetFileAttributesA(pchfile.c_str());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(outdir.c_str()))
		{
			Disp->SendError("", -1, -1, std::string("%(PreCompiler): %(Folder)%(CreationFailed) ") + outdir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr1;
			outstr1 << "%(Folder) " << outdir << " %(CreationFailed)";
			Disp->SendOutput(outstr1.str());

			return false;
		}
	}

	if (!prepareCompilerforFile(utils::string_To_UTF8(precompileheader)))
	{
		Disp->SendError(precompileheader, -1, -1, "%(PreCompiler): %(Header)%(NotExisted) ", "%(CheckUPPPreCompileHeaderConfiguration)");

		std::stringstream outstr2;
		outstr2 << "%(CanNotFind)%(Header) " << precompileheader;
		Disp->SendOutput(outstr2.str());

		return false;
	}

	std::unique_ptr<clang::ASTConsumer> pAstConsumer(new PCHConsumer());

	clang::DiagnosticConsumer& diagConsumer = compilerinstance_.getDiagnosticClient();
	diagConsumer.BeginSourceFile(compilerinstance_.getLangOpts(), &compilerinstance_.getPreprocessor());

	std::unique_ptr<clang::Sema> S(new clang::Sema(compilerinstance_.getPreprocessor(), compilerinstance_.getASTContext(), *pAstConsumer, clang::TranslationUnitKind::TU_Complete, nullptr));

	llvm::CrashRecoveryContextCleanupRegistrar<clang::Sema> CleanupSema(S.get());

	clang::ParseAST(*S.get(), false, true);

	llvm::SmallVector<char, 0> Buffer;
	llvm::BitstreamWriter Stream(Buffer);
	clang::ASTWriter Writer(Stream, Buffer, S->getPreprocessor().getPCMCache(), llvm::ArrayRef<std::shared_ptr<clang::ModuleFileExtension>>());

	Writer.WriteAST(*S.get(), std::string(), nullptr, "", false);

	diagConsumer.EndSourceFile();

	if (diagConsumer.getNumErrors() == 0)
	{
		if (!Buffer.empty())
		{
			std::string output_utf8 = utils::string_To_UTF8(pchfile);
			llvm::SmallString<128> TempPath;
			TempPath = output_utf8;
			TempPath += "-%%%%%%%%";
			int fd;
			std::error_code errcd = llvm::sys::fs::createUniqueFile(TempPath.str(), fd, TempPath);
			if (!errcd)
			{
				llvm::raw_fd_ostream Out(fd, true);

				Out.write(Buffer.data(), Buffer.size());

				Out.close();
				if (Out.has_error())
				{
					Out.clear_error();
				}

				if (llvm::sys::fs::rename(TempPath.str(), output_utf8))
				{
					llvm::sys::fs::remove(TempPath.str());

				}

				Disp->SendTip("", -1, -1, std::string("%(PreCompiler): %(PreCompiler)%(Header)%(Generated): ") + pchfile);

				std::stringstream finsh_str;
				finsh_str << "%(PreCompiler): %(UseDuration) " << watcher.Elapsed() << " %(Milliseconds)";
				Disp->SendTip("", -1, -1, finsh_str.str());

				std::stringstream finsh_str1;
				finsh_str1 << "%(PreCompiler)%(Header) " << pchfile << "%(Generated)" << ", %(UseDuration) " << watcher.Elapsed() << " %(Milliseconds)";
				Disp->SendOutput(finsh_str1.str());

			}
			else
			{
				std::string temppath_acsii = utils::UTF8_To_string(TempPath.str());

				std::stringstream tmp_str;
				tmp_str << "%(PreCompiler): %(TempFile)%(CreationFailed): " << temppath_acsii;
				Disp->SendError("", -1, -1, tmp_str.str());

				std::stringstream finsh_str2;
				finsh_str2 << "%(PreCompiler)%(TempFile) " << temppath_acsii << "%(CreationFailed)";
				Disp->SendOutput(finsh_str2.str());
			}
		}
		return true;
	}
	else
	{
		std::stringstream tmp_str;
		tmp_str << "%(PreCompiler) returns " << diagConsumer.getNumErrors() << " errors and " << diagConsumer.getNumWarnings() << " warnings";
		Disp->SendOutput(tmp_str.str());
		
	}
	return false;
}