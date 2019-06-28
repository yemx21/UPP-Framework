#include "ASTParser.h"
#include "Utils.h"
#include "Precompiler.h"
#include <sstream>
#include "CodeGeneration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Parse/ParseAST.h"
#include <filesystem>
#include <vector>
using namespace uppcl;

#pragma region UPPCallbacks
bool UPPCallbacks::FileNotFound(llvm::StringRef FileName, llvm::SmallVectorImpl< char >& RecoveryPath)
{
	if (FileName.endswith(".uc") || FileName.endswith("_uc.cpp") || FileName.startswith("uc_")) {
		if (!PP.GetSuppressIncludeNotFoundError()) {
			PP.SetSuppressIncludeNotFoundError(true);
			IncludeNotFoundSupressed = true;
		}
	}
	else {
		if (IncludeNotFoundSupressed) {
			PP.SetSuppressIncludeNotFoundError(false);
			IncludeNotFoundSupressed = false;
		}
		else {
			ShouldWarnHeaderNotFound = true;
		}
	}
	return false;
}

void UPPCallbacks::InclusionDirective(clang::SourceLocation HashLoc,
	const clang::Token &IncludeTok,
	StringRef FileName,
	bool IsAngled,
	clang::CharSourceRange FilenameRange,
	const clang::FileEntry *File,
	StringRef SearchPath,
	StringRef RelativePath,
	const clang::Module *Imported,
	clang::SrcMgr::CharacteristicKind FileType)
{
	if (!File && ShouldWarnHeaderNotFound) {
		PP.getDiagnostics().Report(FilenameRange.getBegin(),
			PP.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Warning,
				"'%0' file not found"))
			<< FileName << FilenameRange;
	}
	ShouldWarnHeaderNotFound = false;
}

void UPPCallbacks::PragmaUCompiler::HandlePragma(clang::Preprocessor &PP, clang::PragmaIntroducerKind Introducer, clang::Token &Tok)
{
	if (!uppowner) return;
	if (Introducer == clang::PragmaIntroducerKind::PIK___pragma)
	{
		auto& sm = PP.getSourceManager();
		//auto& headersearch = PP.getHeaderSearchInfo();
		auto curfileid = sm.getMainFileID();

		std::string flag(sm.getCharacterData(Tok.getLocation()), sm.getCharacterData(Tok.getEndLoc()) - sm.getCharacterData(Tok.getLocation()));

		if (flag == "ucompiler")
		{
			PP.Lex(Tok);
			if (Tok.isNot(clang::tok::l_paren)) return;

			PP.Lex(Tok);
			if (!Tok.isAnyIdentifier()) return;
			auto option = Tok.getIdentifierInfo();
			std::string optionname = option->getName();

			if (optionname == "import")
			{
				PP.Lex(Tok);
				if (Tok.isNot(clang::tok::comma)) return;

				PP.Lex(Tok);
				if (!Tok.isLiteral()) return;

				auto tokloc = sm.getFileLoc(Tok.getLocation());
				auto tokfileid = sm.getFileID(tokloc);

				if (tokfileid != curfileid) return;

				std::string importpath(sm.getCharacterData(Tok.getLocation()), sm.getCharacterData(Tok.getEndLoc()) - sm.getCharacterData(Tok.getLocation()));

				llvm::StringRef tmpstr = importpath;
				tmpstr = tmpstr.trim();
				if (tmpstr.startswith("\"") && tmpstr.endswith("\""))
				{
					tmpstr.drop_front();
					tmpstr.drop_back();
				}

				// Search include directories.
				bool isAngled = PP.GetIncludeFilenameSpelling(Tok.getLocation(), tmpstr);

				const clang::DirectoryLookup *CurDir;
				llvm::SmallString<1024> SearchPath;
				llvm::SmallString<1024> RelativePath;
				clang::ModuleMap::KnownHeader SuggestedModule;
				llvm::SmallString<128> NormalizedPath = tmpstr;
				bool mapped = false;
				const clang::FileEntry *File = PP.LookupFile(Tok.getLocation(), NormalizedPath.c_str(), isAngled, nullptr, nullptr, CurDir, &SearchPath, &RelativePath, &SuggestedModule, &mapped);

				if (File)
				{
					tmpstr = File->getName();
					auto finalstr = utils::UTF8_To_string(tmpstr);
					uppowner->uimports.push_back(finalstr);
				}
			}
			else if (optionname == "mode")
			{
				PP.Lex(Tok);
				if (Tok.isNot(clang::tok::equal)) return;

				PP.Lex(Tok);
				if (!Tok.isLiteral()) return;

				std::string mode(sm.getCharacterData(Tok.getLocation()), sm.getCharacterData(Tok.getEndLoc()) - sm.getCharacterData(Tok.getLocation()));

				llvm::StringRef tmpstr = mode;
				tmpstr = tmpstr.trim();
				if (tmpstr.startswith("\"") && tmpstr.endswith("\""))
				{
					tmpstr.drop_front();
					tmpstr.drop_back();
				}

				uppowner->compilemode = tmpstr.str();
			}
		}
	}
}

#pragma endregion

#pragma region UASTConsumer_base
UASTConsumer_base::UASTConsumer_base(clang::CompilerInstance &ci, std::shared_ptr<Dispatcher> disp) :ci(ci), Disp(disp) 
{
}

void UASTConsumer_base::Initialize(clang::ASTContext& Ctx)
{
	ctx = &Ctx;
	PPCallbacks = new UPPCallbacks(ci.getPreprocessor());
	ci.getPreprocessor().addPPCallbacks(std::unique_ptr<clang::PPCallbacks>(PPCallbacks));
}

void UASTConsumer_base::HandleTagDeclDefinition(clang::TagDecl* D)
{
	clang::CXXRecordDecl *RD = llvm::dyn_cast<clang::CXXRecordDecl>(D);
	if (!RD) return;

	auto file = ci.getSourceManager().getFilename(D->getLocation());

	if (!file.empty())
	{
		if (file.endswith(".h") || file.endswith(".hh") || file.endswith(".hpp"))
		{
			UClass Def = ParseClass(RD, ctx, Disp);
			if (Def.Valid)
			{

				auto iter = objects.find(file);
				if (iter != objects.end())
				{
					iter->second.push_back(std::move(Def));
				}
				else
				{
					objects.insert(std::pair<clang::StringRef, std::vector<UClass>>(file, std::vector<UClass>{std::move(Def)}));
				}
				ci.getPreprocessor().enableIncrementalProcessing();
			}
		}
	}
}

#pragma endregion

#pragma region UASTConsumer
UASTConsumer::UASTConsumer(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, clang::CompilerInstance& ci, const std::string out, UAssemblyPool& ass, const std::string& file, HANDLE evt, const std::string& asm_filename, const std::string& switchflag) : UASTConsumer_base(ci, disp), Counter(counter), output(out), pool(ass), header(file), flagevt(evt), AsmFile(asm_filename), SwitchFlag(switchflag)
{
}

void UASTConsumer::Initialize(clang::ASTContext& Ctx)
{
	UASTConsumer_base::Initialize(Ctx);
}

void UASTConsumer::HandleTagDeclDefinition(clang::TagDecl* D)
{
	auto SL = D->getSourceRange().getBegin();
	SL = ci.getSourceManager().getExpansionLoc(SL);
	if (ci.getSourceManager().getFileID(SL) != ci.getSourceManager().getMainFileID())
		return;

	UASTConsumer_base::HandleTagDeclDefinition(D);
}

void UASTConsumer::HandleTranslationUnit(clang::ASTContext& Ctx)
{
	if (ci.getDiagnostics().hasErrorOccurred())
	{
		if (flagevt && flagevt != INVALID_HANDLE_VALUE) SetEvent(flagevt);
		return;
	}

	if (!objects.size())
	{
		if (flagevt && flagevt != INVALID_HANDLE_VALUE) SetEvent(flagevt);
		return;
	}
	bool result = false;
	if (!objects.empty())
	{
		for (auto objpair : objects)
		{
			std::stringstream outstr;
			outstr << "%(Header) " << utils::UTF8_To_string(objpair.first.str()) << " %(Contains)" << objpair.second.size() << "%(Unit)%(UObject):%(NewLine)";

			for (auto obj : objpair.second)
			{
				outstr << "%(Tab)> " << obj.Record->getQualifiedNameAsString() << "%(NewLine)";
			}

			Disp->SendOutput(outstr.str());

			UPPCallbacks* uppcallback = static_cast<UPPCallbacks*>(PPCallbacks);

			result = UCodeGenerator::Generate(Disp, output, objpair, ci, pool, AsmFile, uppcallback->uimports, uppcallback->compilemode, SwitchFlag);
		}
	}
	else
	{
		std::stringstream outstr;
		outstr << "%(Header) " << " % (Contains) 0 " << "%(Unit)%(UObject):%(NewLine)";
		Disp->SendOutput(outstr.str());
	}
	if (result)
	{
		double time = Counter->Elapsed();
		std::stringstream ssh;
		ssh << "%(Compiler): %(Header)%(AnalysisFinished), %(UseDuration) " << time << " %(Milliseconds)";
		Disp->SendTip(header, -1, -1, ssh.str());

		std::stringstream outstr1;
		outstr1 << "%(Header) " << header << " %(AnalysisFinished), %(UseDuration) " << time << " %(Milliseconds)";
		Disp->SendOutput(outstr1.str());
	}
	else
	{
		Disp->SendError(header, -1, -1, "%(UGenerator) : %(CodeGenerator)%(ErrorOccurred)");

		std::stringstream outstr2;
		outstr2 << "%(ForHeader) " << header << " %(CodeGenerator)%(ErrorOccurred)";

		Disp->SendOutput(outstr2.str());
	}
	if (flagevt && flagevt != INVALID_HANDLE_VALUE) SetEvent(flagevt);
}
#pragma endregion

#pragma region Pipe
ClangParser::Pipe::Pipe(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, const std::vector<std::string>& dirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, const std::map<std::string, bool>& macros, const std::string& switchflag) : Disp(disp), Counter(counter), SwitchFlag(switchflag)
{
	SysDirs = dirs;
	UserDirs = udirs;
	CxxDirs = cdirs;

	clang::CompilerInvocation& invocation = compilerinstance_.getInvocation();

	compilerinstance_.createDiagnostics();

	clang::HeaderSearchOptions & headerSearchOptions = compilerinstance_.getHeaderSearchOpts();
	for (std::string& d : SysDirs)
	{
		headerSearchOptions.AddPath(d, clang::frontend::IncludeDirGroup::System, false, false);
	}

	for (std::string& d : CxxDirs)
	{
		headerSearchOptions.AddPath(d, clang::frontend::IncludeDirGroup::CXXSystem, false, false);
	}

	for (std::string& d : UserDirs)
	{
		headerSearchOptions.AddPath(d, clang::frontend::IncludeDirGroup::Angled, false, false);
	}

	headerSearchOptions.Verbose = 1;
	headerSearchOptions.UseBuiltinIncludes = 1;
	headerSearchOptions.UseStandardCXXIncludes = 1;
	headerSearchOptions.UseStandardSystemIncludes = 1;

	clang::LangOptions& langOptions = compilerinstance_.getLangOpts();
	langOptions.CPlusPlus = 1;
	langOptions.Bool = 1;
	langOptions.CPlusPlus11 = 1;
	langOptions.DollarIdents = 1;
	langOptions.CPlusPlus14 = 1;
	langOptions.CPlusPlus17 = 1;
	//langOptions.WChar = 1;
	//langOptions.ImplicitInt = 0;
	//langOptions.GNUMode = 1;

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
	compilerinstance_.getDiagnostics().setClient(new UDiagConsumer{ compilerinstance_.getDiagnostics().takeClient(), Disp, SysDirs, CxxDirs, UserDirs });
	compilerinstance_.getFrontendOpts().ProgramAction = clang::frontend::ActionKind::ParseSyntaxOnly;
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

bool ClangParser::Pipe::parseAST(const std::string& input, const std::string& output, llvm::StringRef pch, UAssemblyPool& AssemblyPool, const std::string& asm_filename)
{
	Counter->Restart();
	std::stringstream outstr;
	outstr << "%(StartAnalysis)%(Header) " << input;
	Disp->SendOutput(outstr.str());

	auto dir = std::filesystem::path(output).parent_path().string();

	DWORD dwAttrib = GetFileAttributesA(dir.data());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(dir.data()))
		{
			Disp->SendError("", -1, -1, std::string("%(Compiler): %(Folder)%(CreationFailed) ") + dir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr1;
			outstr1 << "%(Folder) " << dir << " %(CreationFailed)";
			Disp->SendOutput(outstr1.str());

			return false;
		}
	}

	compilerinstance_.createPCHExternalASTSource(pch, false, true, nullptr, false);

	if (!prepareCompilerforFile(utils::string_To_UTF8(input)))
	{
		Disp->SendError("", -1, -1, std::string("%(Compiler): %(Header)%(NotExisted) ") + input, std::string("%(CheckVCProject)"));

		std::stringstream outstr2;
		outstr2 << "%(CanNotFind)%(Header) " << input;
		Disp->SendOutput(outstr2.str());

		return false;
	}

	HANDLE evt = CreateEventA(NULL, TRUE, FALSE, NULL);

	std::unique_ptr<clang::ASTConsumer> pAstConsumer(new UASTConsumer(Disp, Counter, compilerinstance_, output, AssemblyPool, input, evt, asm_filename, SwitchFlag));

	clang::DiagnosticConsumer& diagConsumer = compilerinstance_.getDiagnosticClient();
	diagConsumer.BeginSourceFile(compilerinstance_.getLangOpts(), &compilerinstance_.getPreprocessor());
	pAstConsumer->Initialize(compilerinstance_.getASTContext());

	clang::ParseAST(compilerinstance_.getPreprocessor(), pAstConsumer.get(), compilerinstance_.getASTContext());

	diagConsumer.EndSourceFile();

	if (diagConsumer.getNumErrors() == 0)
	{
		if (evt && evt != INVALID_HANDLE_VALUE) WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		evt = NULL;
		return true;
	}
	CloseHandle(evt);
	evt = NULL;
	return false;
}

bool ClangParser::Pipe::prepareCompilerforFile(const std::string& szSourceCodeFilePath)
{
	const clang::FileEntry * file = compilerinstance_.getFileManager().getFile(szSourceCodeFilePath);
	if (!file) return false;

	clang::FileID fileID = compilerinstance_.getSourceManager().createFileID(file, clang::SourceLocation(), clang::SrcMgr::CharacteristicKind::C_User);

	if (fileID.isInvalid()) return false;
	compilerinstance_.getSourceManager().setMainFileID(fileID);
	return true;
}

#pragma endregion

#pragma region ClangParser

ClangParser::ClangParser(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, const std::vector<std::string>& dirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, const std::map<std::string, bool>& macros, const std::string& asmdef, const std::string& asmexclude, const std::string& switchflag) : Disp(disp), Counter(counter), SysDirs(dirs), CxxDirs(cdirs), UserDirs(udirs), Macros(macros), SwitchFlag(switchflag)
{
	for (auto inc : dirs)
	{
		SysDirs_utf8.push_back(utils::string_To_UTF8(inc));
	}

	for (auto inc : cdirs)
	{
		CxxDirs_utf8.push_back(utils::string_To_UTF8(inc));
	}
	for (auto inc : udirs)
	{
		UserDirs_utf8.push_back(utils::string_To_UTF8(inc));
	}

	AssemblyPool.Parse(asmdef, asmexclude);

	if (AssemblyPool.Defs.Count() == 0U)
	{
		disp->SendWarning("", -1, -1, std::string("%(Compiler): %(EmptyAssemblyDefination)"));
	}

	std::stringstream os_switch_def;
	os_switch_def << "#if ";
	size_t macro_count = macros.size();
	size_t macro_counter = 0;
	for (const auto& macro : macros)
	{
		if (macro.second)
			os_switch_def << macro.first;
		else
			os_switch_def << macro.first << "==0";
		macro_counter++;
		if (macro_counter < macro_count)
		{
			os_switch_def << " && ";
		}
	}
	os_switch_def << std::endl;
	os_switch_def << "\t #define " << switchflag << std::endl;
	os_switch_def << "#endif" << std::endl;
	Switch_Definition = os_switch_def.str();
}

std::unique_ptr<ClangParser> ClangParser::Create(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Stopwatcher> counter, const std::vector<std::string>& dirs, const std::vector<std::string>& cdirs, const std::vector<std::string>& udirs, const std::map<std::string, bool>& macros, const std::string& asmdef, const std::string& asmexclude, const std::string& switchflag)
{
	return  std::make_unique<ClangParser>(disp, counter, dirs, cdirs, udirs, macros, asmdef, asmexclude, switchflag);
}

bool ClangParser::parseAST(std::vector<std::string>& inputs, std::vector<std::string>& outputs, llvm::StringRef pch, const std::string& asm_filename)
{
	size_t count = inputs.size();
	for (size_t i=0; i<count; i++)
	{
		auto input = inputs[i];
		auto output = outputs[i];
		Pipe pipe(Disp, Counter, SysDirs_utf8, CxxDirs_utf8, UserDirs_utf8, Macros, SwitchFlag);
		std::string pch_utf8 = utils::string_To_UTF8(pch.str());
		if (!pipe.parseAST(input, output, pch_utf8, AssemblyPool, asm_filename))
		{
			Disp->SendError(input, -1, -1, std::string("%(Compiler): %(Header)%(ParseFailed)"));
			return false;
		}
	}
	return true;
}

bool ClangParser::GenerateAssembly(const std::string& dest_h, const std::string& dest_cpp)
{
	return UCodeGenerator::Generate(Disp, dest_h, dest_cpp, AssemblyPool, SwitchFlag, Switch_Definition);
}

bool ClangParser::GenerateEntry(const std::string& dest_h, const std::string& dest_cpp, bool customentry, const std::string& asmheader)
{
	return UCodeGenerator::Generate(Disp, dest_h, dest_cpp, customentry, asmheader, AssemblyPool, SwitchFlag);
}

std::vector<std::string>& ClangParser::GeneratedFiles() { return AssemblyPool.GeneratedPool; }

#pragma endregion

