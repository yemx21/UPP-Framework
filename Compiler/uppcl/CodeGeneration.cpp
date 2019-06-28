#include "Types.h"
#include "CodeGeneration.h"
#include "Utils.h"
#include <sstream>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Type.h>
#include <filesystem>
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/QualTypeNames.h"
#include <filesystem>
using namespace uppcl;

#pragma region UNamespace
void UNamespace::LoopWriteAddNameSpace(llvm::raw_fd_ostream& Out, const std::string& suffix, const std::string& splitver, int& count)
{
	Out << suffix << FullName << "_" << splitver << ");\n";
	count++;
	if (Children.size() > 0)
	{
		for (auto& ns : Children)
		{
			if (ns.Children.size() > 0)
			{
				std::string suffix1 = "\tns_";
				suffix1 += ns.FullName;
				suffix1 += "_";
				suffix1 += splitver;
				suffix1 += "->AddNameSpace(ns_";
				for (auto& ns : ns.Children)
				{
					ns.LoopWriteAddNameSpace(Out, suffix1, splitver, count);
				}
			}
		}
	}
}


#pragma endregion

#pragma region UAssembly 
void UAssembly::LoopWriteAddNameSpace(llvm::raw_fd_ostream& Out, const std::string& suffixr, int& count)
{
	for (auto& ass : Children)
	{
		Out << suffixr << "ns_" << ass.FullName << "_" << SplitedVersion << ");\n";
		count++;
		if (ass.Children.size() > 0)
		{
			std::string suffix1 = "\tns_";
			suffix1 += ass.FullName;
			suffix1 += "_";
			suffix1 += SplitedVersion;
			suffix1 += "->AddNameSpace(ns_";
			for (auto& ns : ass.Children)
			{
				ns.LoopWriteAddNameSpace(Out, suffix1, SplitedVersion, count);
			}
		}
	}
}
#pragma endregion

#pragma region UAssemblyMap
void UAssemblyMap::LoopWriteAddNameSpace(llvm::raw_fd_ostream& Out, int& count)
{
	for (auto& ass : Children)
	{
		std::string suffix = "\tasm_";
		suffix += ass.SplitedVersion;
		suffix += "->AddNameSpace(";

		ass.LoopWriteAddNameSpace(Out, suffix, count);
	}
}
#pragma endregion

#pragma region UCodeGenerator
std::string UCodeGenerator::GetTypeString(const clang::QualType& ty, const clang::ASTContext& ctx)
{
	std::string ptyn = clang::TypeName::getFullyQualifiedName(ty, ctx, ctx.getPrintingPolicy(), false);

	if (ty.split().Ty != nullptr)
	{
		ptyn = utils::ReplaceAllDistinct(ptyn, "_BOOL", "bool");
		ptyn = utils::ReplaceAllDistinct(ptyn, "_Bool", "bool");
	}
	return ptyn;
}

std::string UCodeGenerator::GetTypeString(const clang::QualType& ty, clang::PrintingPolicy policy)
{
	std::string ptyn = clang::QualType::getAsString(ty.split(), policy);

	if (ty.split().Ty != nullptr)
	{
		ptyn = utils::ReplaceAllDistinct(ptyn, "_BOOL", "bool");
		ptyn = utils::ReplaceAllDistinct(ptyn, "_Bool", "bool");
	}
	return ptyn;
}

bool UCodeGenerator::Generate(std::shared_ptr<Dispatcher> disp, const std::string& dest_h, const std::string& dest_cpp, bool customentry, const std::string& asmfile, UAssemblyPool& pool, const std::string& switchflag)
{
	auto sr_destdir = std::filesystem::path(dest_h).parent_path().string();
	DWORD dwAttrib = GetFileAttributesA(sr_destdir.data());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(sr_destdir.data()))
		{
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(Folder)%(CreationFailed) ") + sr_destdir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr;
			outstr << "%(Folder) " << sr_destdir << " %(CreationFailed)";

			disp->SendOutput(outstr.str());

			return false;
		}
	}

	sr_destdir = std::filesystem::path(dest_cpp).parent_path().string();
	dwAttrib = GetFileAttributesA(sr_destdir.data());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(sr_destdir.data()))
		{
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(Folder)%(CreationFailed) ") + sr_destdir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr;
			outstr << "%(Folder) " << sr_destdir << " %(CreationFailed)";

			disp->SendOutput(outstr.str());

			return false;
		}
	}

	std::string dest_h_utf8 = utils::string_To_UTF8(dest_h);
	std::string dest_cpp_utf8 = utils::string_To_UTF8(dest_cpp);

	llvm::SmallString<128> TempPath;

#pragma region Header
	TempPath = dest_h_utf8;
	TempPath += "-%%%%%%%%";
	int fd;
	if (!llvm::sys::fs::createUniqueFile(TempPath.str(), fd, TempPath))
	{
		llvm::raw_fd_ostream Out(fd, true);
		Out << "#pragma once\n";
		Out << "#include \"" << asmfile << "\"\n";
		Out << "#ifdef " << switchflag << "\n";

		Out << "/****************************************************************************\n"
			"** uassembly entry header\n"
			"**\n"
			"** Created by ucomplier version: 1.0 \n"
			"** WARNING! All changes made in this file will be lost!\n"
			"*****************************************************************************/\n\n";

		Out << "#include <System\\System.h>\n\n";
		Out << "#define _UCUSTOMENTRY " << (customentry ? "1" : "0") << "\n";

		Out << "#if _UCUSTOMENTRY\n";
		Out << "extern System::Result<bool, void*> __uentry_before(void*, const System::safe_void_ptr&);\n";
		Out << "extern System::Result<bool> __uentry_after(bool, void*, const System::safe_void_ptr&, void*);\n";
		Out << "extern System::Result<bool, void*> __uexit_before(void*);\n";
		Out << "extern System::Result<bool> __uexit_after(bool, void*, void*);\n";
		Out << "#endif\n";
		Out << "#endif";

		Out.flush();
		Out.close();
		if (Out.has_error())
		{
			Out.clear_error();
		}

		if (llvm::sys::fs::rename(TempPath.str(), dest_h_utf8))
		{
			llvm::sys::fs::remove(TempPath.str());
		}

		if (llvm::sys::fs::exists(dest_h_utf8))
			pool.GeneratedPool.push_back(dest_h);
		else
		{
			std::string temp_h_acsii = utils::UTF8_To_string(TempPath.str());
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(File)%(CreationFailed) ") + temp_h_acsii);

			std::stringstream outstr1;
			outstr1 << "%(File) " << temp_h_acsii << " %(CreationFailed)";

			disp->SendOutput(outstr1.str());

			return false;
		}
	}
	else
	{
		std::string temp_h_acsii = utils::UTF8_To_string(TempPath.str());
		disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(TempFile)%(CreationFailed): ") + temp_h_acsii);

		std::stringstream outstr2;
		outstr2 << "%(TempFile) " << temp_h_acsii << " %(CreationFailed)";

		disp->SendOutput(outstr2.str());

		return false;
	}

#pragma endregion

#pragma region EntryCode

	TempPath = dest_cpp_utf8;
	TempPath += "-%%%%%%%%";
	if (!llvm::sys::fs::createUniqueFile(TempPath.str(), fd, TempPath))
	{
		llvm::raw_fd_ostream Out(fd, true);

		Out << "/****************************************************************************\n"
			"** uassembly entry implement\n"
			"**\n"
			"** Created by ucomplier version: 1.0 \n"
			"** WARNING! All changes made in this file will be lost!\n"
			"*****************************************************************************/\n\n";

		Out << "#include <Windows.h>\n";
		Out << "#include \"" << dest_h << "\"\n";
		Out << "#include \"" << asmfile << "\"\n\n";
		Out << "#ifdef " << switchflag << "\n";

		Out << "System::Result<bool, safe_void_ptr> dll_load();\n";
		Out << "System::Result<bool> dll_unload();\n\n";

		Out << "System::CoreDllMain _udllmain\n{\n";
		Out << "\tdll_load,\n\tdll_unload,\n\tnullptr\n};\n\n";

		Out << "HMODULE _ugetcurrentmodulehandle()\n{\n";
		Out << "\tHMODULE hmod = NULL;\n\tGetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,reinterpret_cast<LPCWSTR>(&_ugetcurrentmodulehandle),&hmod);\n\treturn hmod;\n}\n\n";


		Out << "System::Result<bool, safe_void_ptr> dll_load()\n{\n\tSystem::Result<bool, safe_void_ptr> result(false, nullptr);\n";
		Out << "\tHMODULE hmod=_ugetcurrentmodulehandle();\n\tif(!hmod)\n\t{\n\t\tresult.WriteMessage(L\"can not find module handle\");\n\t\treturn result;\n\t}\n";
		Out << "\tauto modres = _udllmain.Helper->RegisiterModule(hmod);\n";
		Out << "\tif(modres.Value == System::FactoryError::OK || modres.Value == System::FactoryError::AlreadyExist)\n\t{\n";
		Out << "#if _UCUSTOMENTRY\n";
		Out << "\t\tauto _uentry_before_res = __uentry_before(hmod, modres.State);\n";
		Out << "\t\tif(!_uentry_before_res.Value)\n\t\t{\n";
		Out << "\t\t\tresult.WriteMessage(_uentry_before_res.Message);\n\t\t\treturn result;\n\t\t}\n";
		Out << "#endif\n";

		for (auto ass : pool.Pool)
		{
			std::vector<UVersion> asms = pool.Query(ass.Name);
			if (asms.empty()) continue;
			std::string cls = "_uassembly_" + ass.Name;
			Out << "\t\t" << cls << "& " << ass.Name << "_instance = " << cls << "::GetInstance();\n";
			Out << "\t\tauto " << ass.Name << "_reg = " << ass.Name << "_instance.Add(modres.State, _udllmain);\n";
			Out << "\t\tif(" << ass.Name << "_reg != System::FactoryError::OK && " << ass.Name << "_reg != System::FactoryError::AlreadyExist)\n\t\t{\n";
			Out << "#if _UCUSTOMENTRY\n";
			Out << "\t\t__uentry_after(false, hmod, modres.State, _uentry_before_res.State);\n";
			Out << "#endif\n";
			Out << "\t\t\treturn result;\n\t\t}\n";
		}


		Out << "#if _UCUSTOMENTRY\n";
		Out << "\t\tauto _uentry_after_res = __uentry_after(true, hmod, modres.State, _uentry_before_res.State);\n";
		Out << "\t\tif(!_uentry_after_res.Value)\n\t\t{\n";
		Out << "\t\t\tresult.WriteMessage(_uentry_after_res.Message);\n\t\t}\n";
		Out << "\t\telse\n\t\t{\n";
		Out << "\t\t\tresult.Value=true;\n\t\t\tresult.State=modres.State;\n\t\t}\n";
		Out << "#else\n\t\tresult.Value=true;\n\t\tresult.State=modres.State;\n";
		Out << "#endif\n";
		Out << "\t}\n";
		Out << "\treturn result;\n}\n\n";


		Out << "System::Result<bool> dll_unload()\n{\n";
		Out << "\tSystem::Result<bool> result(false);\n";
		Out << "#if _UCUSTOMENTRY\n";
		Out << "\tHMODULE hmod=_ugetcurrentmodulehandle();\n\tif(!hmod)\n\t{\n\t\tresult.WriteMessage(L\"can not find module handle\");\n\t\treturn result;\n\t}\n";
		Out << "\tauto _uexit_before_res = __uexit_before(hmod);\n";
		Out << "\tif(!_uexit_before_res.Value)\n\t{\n";
		Out << "\t\tresult.WriteMessage(_uexit_before_res.Message);\n\t\treturn result;\n\t}\n";
		Out << "#endif\n";

		for (auto ass : pool.Pool)
		{
			std::vector<UVersion> asms = pool.Query(ass.Name);
			if (asms.empty()) continue;
			std::string cls = "_uassembly_" + ass.Name;
			Out << "\t" << cls << "& " << ass.Name << "_instance = " << cls << "::GetInstance();\n";
			Out << "\tauto " << ass.Name << "_unreg = " << ass.Name << "_instance.Remove(_udllmain);\n";
			Out << "\tif(" << ass.Name << "_unreg != System::FactoryError::OK && " << ass.Name << "_unreg != System::FactoryError::Nonexistent)\n\t{\n";
			Out << "#if _UCUSTOMENTRY\n";
			Out << "\t\t__uexit_after(false, hmod, _uexit_before_res.State);\n";
			Out << "#endif\n";
			Out << "\t\treturn result;\n\t}\n";
		}

		Out << "#if _UCUSTOMENTRY\n";
		Out << "\tauto _uexit_after_res = __uexit_after(true, hmod, _uexit_before_res.State); \n";
		Out << "\tif(!_uexit_after_res.Value)\n\t{\n";
		Out << "\t\tresult.WriteMessage(_uexit_after_res.Message);\n\t}\n";
		Out << "\telse\n\t{\n";
		Out << "\t\tresult.Value=true;\n\t}\n";
		Out << "#else\n\tresult.Value=true;\n";
		Out << "#endif\n";
		Out << "\treturn result;\n}\n\n";


		Out << "extern \"C\" __declspec(dllexport) System::CoreDllMain* CoreDllMain() { return &_udllmain;}\n";

		Out << "#endif";
		Out.flush();
		Out.close();
		if (Out.has_error())
		{
			Out.clear_error();
		}

		if (llvm::sys::fs::rename(TempPath.str(), dest_cpp_utf8))
		{
			llvm::sys::fs::remove(TempPath.str());
		}

		if (llvm::sys::fs::exists(dest_cpp_utf8))
			pool.GeneratedPool.push_back(dest_cpp);
		else
		{
			std::string temp_cpp_acsii = utils::UTF8_To_string(TempPath.str());
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(File)%(CreationFailed) ") + temp_cpp_acsii);

			std::stringstream outstr1;
			outstr1 << "%(File) " << temp_cpp_acsii << " %(CreationFailed)";

			disp->SendOutput(outstr1.str());

			return false;
		}
	}
	else
	{
		std::string temp_cpp_acsii = utils::UTF8_To_string(TempPath.str());
		disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(TempFile)%(CreationFailed): ") + temp_cpp_acsii);

		std::stringstream outstr2;
		outstr2 << "%(TempFile) " << temp_cpp_acsii << " %(CreationFailed)";

		disp->SendOutput(outstr2.str());

		return false;
	}

#pragma endregion

	return true;
}


bool UCodeGenerator::Generate(std::shared_ptr<Dispatcher> disp, const std::string& dest_h, const std::string& dest_cpp, UAssemblyPool& pool, const std::string& switchflag, const std::string& switch_definition)
{
	auto sr_destdir = std::filesystem::path(dest_h).parent_path().string();
	DWORD dwAttrib = GetFileAttributesA(sr_destdir.data());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(sr_destdir.data()))
		{
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(Folder)%(CreationFailed) ") + sr_destdir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr;
			outstr << "%(Folder) " << sr_destdir << " %(CreationFailed)";

			disp->SendOutput(outstr.str());

			return false;
		}
	}

	sr_destdir = std::filesystem::path(dest_cpp).parent_path().string();
	dwAttrib = GetFileAttributesA(sr_destdir.data());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(sr_destdir.data()))
		{
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(Folder)%(CreationFailed) ") + sr_destdir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr;
			outstr << "%(Folder) " << sr_destdir << " %(CreationFailed)";

			disp->SendOutput(outstr.str());

			return false;
		}
	}

	std::string dest_h_utf8 = utils::string_To_UTF8(dest_h);
	std::string dest_cpp_utf8 = utils::string_To_UTF8(dest_cpp);

	llvm::SmallString<128> TempPath;

#pragma region Header
	TempPath = dest_h_utf8;
	TempPath += "-%%%%%%%%";
	int fd;
	if (!llvm::sys::fs::createUniqueFile(TempPath.str(), fd, TempPath))
	{
		llvm::raw_fd_ostream Out(fd, true);

		Out << "#pragma once\n";
		Out << switch_definition;
		Out << "#ifdef " << switchflag << "\n";

		Out << "/****************************************************************************\n"
			"** uassembly object header\n"
			"**\n"
			"** Created by ucomplier version: 1.0 \n"
			"** WARNING! All changes made in this file will be lost!\n"
			"** TIP! If you want to modify the code, please change file extension to \"h\" and add its path to UCompiler setting in the project property page.\n"
			"*****************************************************************************/\n\n";

		Out << "#include <System\\System.h>\n\n";

		for (auto ass : pool.Pool)
		{
			std::vector<UVersion> asms = pool.Query(ass.Name);

			if (asms.empty()) continue;  //无任何版本的程序集忽略

			std::string cls = "_uassembly_" + ass.Name;

			Out << "class " << cls << "\n{\nprivate:\n";
			if (!ass.Children.empty())
			{
				for (auto subass : ass.Children)
				{
					if (std::find(asms.begin(), asms.end(), subass.Ver) != asms.end())
					{
						Out << "\tSystem::Assembly* asm_" << subass.SplitedVersion << ";\n";
					}
				}
				Out << "\n";
			}

			Out << "\tSystem::safe_void_ptr module;\n\t" << cls << "();\n\n";

			if (!ass.Children.empty())
			{
				for (auto subass : ass.Children)
				{
					if (std::find(asms.begin(), asms.end(), subass.Ver) != asms.end())
					{
						std::set<std::string> xnss;
						subass.GetAllNamespace(xnss);
						for (const std::string& ns : xnss)
						{
							Out << "\tSystem::NameSpace* ns_" << ns << "_" << subass.SplitedVersion << ";\n";
						}
					}
					Out << "\n";
				}
			}

			Out << "public:\n";

			std::set<std::string> nss;
			ass.GetAllNamespace(nss);

			Out << "\tenum class Nodes\n\t{\n";
			if (!nss.empty())
			{
				for (const std::string& ns : nss)
				{
					Out << "\t\t" << ns << ",\n";
				}
			}
			Out << "\t};\n\n";

			Out << "\tenum class Versions\n\t{\n";
			if (!ass.Children.empty())
			{
				for (auto subass : ass.Children)
				{
					if (std::find(asms.begin(), asms.end(), subass.Ver) != asms.end())
					{
						Out << "\t\t_" << subass.SplitedVersion << ",\n";
					}
				}
			}
			Out << "\t};\n\n";

			Out << "\tstatic " << cls << "& GetInstance();\n";
			Out << "\tstatic System::Type* Regisiter(" << cls << "::Versions ver, " << cls << "::Nodes node, System::Type* type); \n";
			Out << "\tstatic System::FactoryError Add(const System::safe_void_ptr& mod, System::CoreDllMain dll);\n";
			Out << "\tstatic System::FactoryError Remove(System::CoreDllMain dll);\n";

			Out << "};\n\n";
		}
		Out << "#endif";
		Out.flush();
		Out.close();
		if (Out.has_error())
		{
			Out.clear_error();
		}

		if (llvm::sys::fs::rename(TempPath.str(), dest_h_utf8))
		{
			llvm::sys::fs::remove(TempPath.str());
		}

		if (llvm::sys::fs::exists(dest_h_utf8))
		{
			pool.GeneratedPool.push_back(dest_h);
		}
		else
		{
			std::string temp_h_acsii = utils::UTF8_To_string(TempPath.str());
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(File)%(CreationFailed) ") + temp_h_acsii);

			std::stringstream outstr1;
			outstr1 << "%(File) " << temp_h_acsii << " %(CreationFailed)";

			disp->SendOutput(outstr1.str());

			return false;
		}
	}
	else
	{
		std::string temp_h_acsii = utils::UTF8_To_string(TempPath.str());
		disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(TempFile)%(CreationFailed): ") + temp_h_acsii);

		std::stringstream outstr2;
		outstr2 << "%(TempFile) " << temp_h_acsii << " %(CreationFailed)";

		disp->SendOutput(outstr2.str());

		return false;
	}

#pragma endregion

	TempPath = dest_cpp_utf8;
	TempPath += "-%%%%%%%%";
	if (!llvm::sys::fs::createUniqueFile(TempPath.str(), fd, TempPath))
	{
		llvm::raw_fd_ostream Out(fd, true);
		
		Out << "/****************************************************************************\n"
			"** uassembly object implement\n"
			"**\n"
			"** Created by ucomplier version: 1.0 \n"
			"** WARNING! All changes made in this file will be lost!\n"
			"** TIP! If you want to modify the code, please change file extension to \"cpp\" and add its path to UCompiler setting in the project property page..\n"
			"*****************************************************************************/\n\n";

		Out << "#include <System\\System.h>\n";
		Out << "#include \"" << dest_h << "\"\n";

		Out << "#ifdef " << switchflag << "\n";

		for (auto ass : pool.Pool)
		{
			std::vector<UVersion> asms = pool.Query(ass.Name);

			if (asms.empty()) continue;  //无任何版本的程序集忽略

			std::string cls = "_uassembly_" + ass.Name;
			Out << "#pragma region " << cls << "\n";
#pragma region Assembly::Assembly() 

			Out << cls << "::" << cls << "()\n{\n";

			if (!ass.Children.empty())
			{
				bool has_subdef = false;
				for (auto subass : ass.Children)
				{
					UVersion ver = subass.Ver;
					UAssemblyDef* subdef = pool.Defs.Query(subass.Name, ver);
					if (subdef)
					{
						has_subdef = true;
						Out << "\tasm_" << subass.SplitedVersion << " = System::Assembly::Create(L\"" << ass.Name << "\", Version(" << ver.Major << ", " << ver.Minor << ", " << ver.Build << ", " << ver.Revision << "), " << subdef->PublicKey() << ");\n";
					}
				}
				if (has_subdef) Out << "\n";
			}

			if (!ass.Children.empty())
			{
				for (auto subass : ass.Children)
				{
					if (std::find(asms.begin(), asms.end(), subass.Ver) != asms.end())
					{
						std::set<std::string> xnss;
						subass.GetAllNamespace(xnss, false);
						for (const std::string& ns : xnss)
						{
							Out << "\tns_" << ns << "_" << subass.SplitedVersion << " = new System::NameSpace(L\"" << ns << "\");\n";
						}
						Out << "\n";
					}
				}
				Out << "\n";
			}

			if (!ass.Children.empty())
			{
				int subns = 0;
				ass.LoopWriteAddNameSpace(Out, subns);
				if (subns) Out << "\n";
			}
			Out << "}\n\n";
#pragma endregion
#pragma region Assembly& Assembly::GetInstance()
			Out << cls << "& " << cls << "::GetInstance()\n{\n\tstatic " << cls << " instance;\n\treturn instance;\n}\n\n";
#pragma endregion

#pragma region System::FactoryError Add(const System::safe_void_ptr& mod, System::CoreDllMain dll)
			Out << "System::FactoryError " << cls << "::Add(const System::safe_void_ptr& mod, System::CoreDllMain dll)\n{\n";

			bool has_asm = false;
			if (!ass.Children.empty())
			{
				for (auto subass : ass.Children)
				{
					UVersion ver = subass.Ver;
					UAssemblyDef* subdef = pool.Defs.Query(subass.Name, ver);
					if (subdef)
					{
						has_asm = true;
						break;
					}
				}
			}

			if (has_asm)
			{
				Out << "\tif(mod)\n\t{\n";
				Out << "\t\t" << cls << "& instance = " << cls << "::GetInstance();\n";
				Out << "\t\tinstance.module=mod;\n\n";

				if (!ass.Children.empty())
				{
					for (auto subass : ass.Children)
					{
						UVersion ver = subass.Ver;
						UAssemblyDef* subdef = pool.Defs.Query(subass.Name, ver);
						if (subdef)
						{
							Out << "\t\tif(instance.asm_" << subass.SplitedVersion << ")\n\t\t{\n";
							Out << "\t\t\tSystem::FactoryError err = dll.Helper->AddAssembly(instance.module, instance.asm_" << subass.SplitedVersion << ", nullptr, 0);\n";
							Out << "\t\t\tif(err != System::FactoryError::None) return err;\n\t\t}\n";
						}
					}
				}
				Out << "\t}\n\n";
			}

			Out << "\treturn System::FactoryError::None;\n}\n\n";
#pragma endregion
#pragma region System::FactoryError Remove(System::CoreDllMain dll)
			Out << "System::FactoryError " << cls << "::Remove(System::CoreDllMain dll)\n{\n";
			if (has_asm)
			{
				Out << "\t" << cls << "& instance = " << cls << "::GetInstance();\n";
				Out << "\tif(instance.module)\n\t{\n";
				if (!ass.Children.empty())
				{
					for (auto subass : ass.Children)
					{
						UVersion ver = subass.Ver;
						UAssemblyDef* subdef = pool.Defs.Query(subass.Name, ver);
						if (subdef)
						{
							Out << "\t\tif(instance.asm_" << subass.SplitedVersion << ")\n\t\t{\n";
							Out << "\t\t\tSystem::FactoryError err = dll.Helper->RemoveAssembly(instance.module, instance.asm_" << subass.SplitedVersion << ", nullptr, 0);\n";
							Out << "\t\t\tif(err != System::FactoryError::None) return err;\n\t\t}\n\n";
						}
					}
				}
				Out << "\t\tinstance.module = nullptr;\n\t}\n\n";
			}

			Out << "\treturn System::FactoryError::None;\n}\n\n";
#pragma endregion

#pragma region System::Type* Regisiter(Versions ver, Nodes node, System::Type* type)
			Out << "System::Type* " << cls << "::Regisiter(" << cls << "::Versions ver, " << cls << "::Nodes node, System::Type* type)\n{\n";

			if (has_asm)
			{
				Out << "\t" << cls << "& instance = " << cls << "::GetInstance();\n";
				Out << "\tswitch(ver)\n\t{\n";
				if (!ass.Children.empty())
				{
					for (auto subass : ass.Children)
					{
						std::string sux = "instance.asm_";
						sux += subass.SplitedVersion;
						if (std::find(asms.begin(), asms.end(), subass.Ver) != asms.end())
						{
							std::set<std::string> nss;
							subass.GetAllNamespace(nss);
							if (!nss.empty())
							{
								Out << "\t\tcase " << cls << "::Versions::_" << subass.SplitedVersion << ":\n\t\t{\n";

								Out << "\t\t\tswitch(node)\n\t\t\t{\n";

								for (const std::string& nsx : nss)
								{
									Out << "\t\t\t\tcase " << cls << "::Nodes::" << nsx << ":\n\t\t\t\t{\n";
									Out << "\t\t\t\t\tSystem::NameSpaceError err = instance.ns_" << nsx << "_" << subass.SplitedVersion << "->Register(type);\n";
									Out << "\t\t\t\t\tif(err == System::NameSpaceError::Success || err == System::NameSpaceError::Existed) return type;\n";
									Out << "\t\t\t\t}\n\t\t\t\tbreak;\n";
								}

								Out << "\t\t\t}\n\t\t}\n\t\tbreak;\n";

							}
						}
					}
					Out << "\t}\n";
				}
			}

			Out << "\treturn nullptr;\n}\n\n";
#pragma endregion

		}
		Out << "#endif";
		Out.flush();
		Out.close();
		if (Out.has_error())
		{
			Out.clear_error();
		}

		if (llvm::sys::fs::rename(TempPath.str(), dest_cpp_utf8))
		{
			llvm::sys::fs::remove(TempPath.str());
		}

		if (llvm::sys::fs::exists(dest_cpp_utf8))
			pool.GeneratedPool.push_back(dest_cpp);
		else
		{
			std::string temp_cpp_acsii = utils::UTF8_To_string(TempPath.str());

			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(File)%(CreationFailed) ") + temp_cpp_acsii);

			std::stringstream outstr3;
			outstr3 << "%(File) " << temp_cpp_acsii << " %(CreationFailed)";

			disp->SendOutput(outstr3.str());
			return false;
		}
	}
	else
	{
		std::string temp_cpp_acsii = utils::UTF8_To_string(TempPath.str());
		disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(TempFile)%(CreationFailed): ") + temp_cpp_acsii);

		std::stringstream outstr4;
		outstr4 << "%(TempFile) " << temp_cpp_acsii << " %(CreationFailed)";

		disp->SendOutput(outstr4.str());

		return false;
	}


	return true;
}


bool UCodeGenerator::Generate(std::shared_ptr<Dispatcher> disp, const std::string& dest, const std::pair< clang::StringRef, std::vector<UClass>>& file, clang::CompilerInstance& ci, UAssemblyPool& pool, const std::string& asm_file, const std::vector<std::string>& extraheaders, const std::string& mode, const std::string& switchflag)
{
	if (std::filesystem::exists(dest) && llvm::StringRef(mode).trim().lower() == "none")
		return true;

	clang::ASTContext& astctx = ci.getASTContext();

	std::string file_acsii = utils::UTF8_To_string(file.first);
	std::string fn = std::string(file.first);
	const size_t last_slash_idx = fn.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		fn.erase(0, last_slash_idx + 1);
	}
	const size_t period_idx = fn.rfind('.');
	if (std::string::npos != period_idx)
	{
		fn.erase(period_idx);
	}
	auto sr_destdir = std::filesystem::path(dest).parent_path().string();

	DWORD dwAttrib = GetFileAttributesA(sr_destdir.data());
	if (!(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
	{
		if (!utils::CreateFolder(sr_destdir.data()))
		{
			disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(Folder)%(CreationFailed) ") + sr_destdir, std::string("%(ReConfigUGenDir)"));

			std::stringstream outstr;
			outstr << "%(Folder) " << sr_destdir << " %(CreationFailed)";

			disp->SendOutput(outstr.str());
			return false;
		}
	}



	std::string dest_utf8 = utils::string_To_UTF8(dest);

	std::string temp = dest + ".tmp";
	std::string temp_utf8 = dest_utf8 + ".tmp";

	auto OS = ci.createOutputFile(temp_utf8, false, true, "", "", false, false);

	if (!OS)
	{
		disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(TempFile)%(CreationFailed): ") + temp);

		std::stringstream outstr1;
		outstr1 << "%(TempFile) " << temp << " %(CreationFailed)";

		disp->SendOutput(outstr1.str());
		return false;
	}

	llvm::raw_ostream& writer = *OS;

	writer << "/****************************************************************************\n"
		"** uobject code from reading C++ file '" << file_acsii << "'\n"
		"**\n"
		"** Created by ucomplier version: 1.0 \n"
		"** WARNING! All changes made in this file will be lost!\n"
		"** TIP! If you want to modify the code, please change file extension to \"cpp\" and the responding definations \"__uclass\" or \"__uenum\" to \"class\".\n"
		"*****************************************************************************/\n\n";

	writer << "#include \"" << file_acsii << "\"\n";
	writer << "#include \"" << asm_file << "\"\n";
	writer << "#ifdef " << switchflag << "\n";

	for (const auto& extraheader : extraheaders)
	{
		writer << "#include \"" << extraheader << "\"\n";
	}
	writer << "\n\n";
	
	size_t obj_upper = file.second.size() - 1;
	size_t obj_counter = 0;
	for (auto obj : file.second)
	{
		writer << "#pragma region " << obj.FullName << "\n";

		/*if (!obj.Values.empty())
		{
			for (auto val : obj.Values)
			{
				writer << obj.FullName << " const " << obj.FullName << "::" << val.first->getNameAsString() << " = " << val.second << ";\n";
			}
			writer << "\n";
		}*/

		bool have_ctor_impl = false;
		if (obj.DefaultConstructor)
		{
			writer << "__inline static System::Object* Factory_" << obj.SplitedQualifiedName << "_def() { return new " << obj.FullName << "(); }\n";
			have_ctor_impl = true;
		}
		else if (obj.StaticDefaultConstructor)
		{
			writer << "__inline static System::Object* Factory_" << obj.SplitedQualifiedName << "_def() { return " << obj.FullName << "::" << obj.StaticDefaultConstructor->getName() << "(); }\n";
			have_ctor_impl = true;
		}

		int ctor_counter = 0;
		for (auto ctor : obj.Constructors)
		{
			writer << "__inline static System::Object* Factory_" << obj.SplitedQualifiedName << "_" << ctor_counter++ << "(";

			size_t ctor_param_counter = 0;
			size_t ctor_param_unnamed_counter = 0;
			size_t ctor_param_upper = ctor->param_size() - 1;
			std::string pr;
			llvm::raw_string_ostream prs(pr);
			for (auto param : ctor->parameters())
			{
				std::string ptyn = GetTypeString(param->getType(), astctx);
				if (!ptyn.empty())
				{
					utils::Trim(ptyn);
					if (ptyn.size() > 5)
						if (ptyn.find("class") == 0 || ptyn.find("struct") == 0)
							ptyn = ptyn.substr(5);
					ptyn = utils::Trim(ptyn);
					std::string pn = param->getQualifiedNameAsString();
					if (pn.empty())
					{
						writer << ptyn << " unnamed_" << ctor_param_unnamed_counter;
						prs << "unnamed_" << ctor_param_unnamed_counter;
					}
					else
					{
						writer << ptyn << " " << pn;
						prs << pn;
					}
					if (ctor_param_counter < ctor_param_upper)
					{
						writer << ", ";
						prs << ", ";
					}
				}
				ctor_param_counter++;
			}

			writer << ") { return new " << obj.FullName << "(" << prs.str() << "); }\n";
			have_ctor_impl = true;
		}

		for (auto ctor : obj.StaticConstructors)
		{
			writer << "__inline static System::Object* Factory_" << obj.SplitedQualifiedName << "_" << ctor_counter++ << "(";

			size_t ctor_param_counter = 0;
			size_t ctor_param_unnamed_counter = 0;
			size_t ctor_param_upper = ctor->param_size() - 1;
			std::string pr;
			llvm::raw_string_ostream prs(pr);
			for (auto param : ctor->parameters())
			{
				std::string ptyn = GetTypeString(param->getType(), astctx);
				if (!ptyn.empty())
				{
					utils::Trim(ptyn);
					if (ptyn.size() > 5)
						if (ptyn.find("class") == 0 || ptyn.find("struct") == 0)
							ptyn = ptyn.substr(5);
					ptyn = utils::Trim(ptyn);
					std::string pn = param->getQualifiedNameAsString();
					if (pn.empty())
					{
						writer << ptyn << " unnamed_" << ctor_param_unnamed_counter;
						prs << "unnamed_" << ctor_param_unnamed_counter;
					}
					else
					{
						writer << ptyn << " " << pn;
						prs << pn;
					}
					if (ctor_param_counter < ctor_param_upper)
					{
						writer << ", ";
						prs << ", ";
					}
				}
				ctor_param_counter++;
			}

			writer << ") { return " << obj.FullName << "::" << ctor->getNameAsString() << "(" << prs.str() << "); }\n";
			have_ctor_impl = true;
		}


		if (have_ctor_impl) writer << "\n\n";

		if (obj.FullName.find(':') == std::string::npos)
		{
			if (obj.Values.size() == 0 && obj.Values.empty())
				writer << "__uregister_class(" << obj.FullName << ", System::ClassFlags::Normal)\n{\n\n";
			else
				writer << "__uregister_enum(" << obj.FullName << ")\n{\n\n";
		}
		else
		{
			if (obj.Values.size() == 0 && obj.Values.empty())
				writer << "__uregister_class1(" << obj.FullName << ", " << obj.Name << ", System::ClassFlags::Normal)\n{\n\n";
			else
				writer << "__uregister_enum1(" << obj.FullName << ", " << obj.Name << ")\n{ \n\n";
		}

		size_t base_counter = 0;

		for (auto bs : obj.Bases)
		{
			writer << "\ttype.AddBaseClass<" << bs->getQualifiedNameAsString() << ">();\n";
			base_counter++;
		}

		if (base_counter > 0) writer << "\n";

		if (obj.DefaultConstructor)
		{
			writer << "\ttype.AddDefaultConstructor<" << obj.FullName << ">(L\"" << obj.Name << "\", ConstructorFlags::Default, &Factory_" << obj.SplitedQualifiedName << "_def);\n\n";
		}
		else if (obj.StaticDefaultConstructor)
		{
			writer << "\ttype.AddDefaultConstructor<" << obj.FullName << ">(L\"" << obj.Name << "\", ConstructorFlags::Default, &Factory_" << obj.SplitedQualifiedName << "_def);\n\n";
		}

		int ctor_counter_ex = 0;
		for (auto ctor : obj.Constructors)
		{
			if (ctor != obj.DefaultConstructor)
			{
				writer << "\ttype.AddConstructor(" << "L\"" << obj.Name << "\", ConstructorFlags::Default, System::ConstructorTypeAdapter<" << ctor->param_size() << ", " << obj.FullName << ">::Create(&Factory_" << obj.SplitedQualifiedName << "_" << ctor_counter_ex++ << "));\n";
			}
		}

		for (auto ctor : obj.StaticConstructors)
		{
			if (ctor != obj.StaticDefaultConstructor)
			{
				writer << "\ttype.AddConstructor(" << "L\"" << obj.Name << "\", ConstructorFlags::Default, System::ConstructorTypeAdapter<" << ctor->param_size() << ", " << obj.FullName << ">::Create(&Factory_" << obj.SplitedQualifiedName << "_" << ctor_counter_ex++ << "));\n";
			}
		}

		if (ctor_counter_ex > 0) writer << "\n";

		size_t count_msprop = 0;
		for (auto msprop : obj.MSProperties)
		{
			auto getter = msprop->getGetterId();
			auto setter = msprop->getSetterId();
			if (getter && setter)
			{
				writer << "\ttype.AddProperty(L\"" << msprop->getName() << "\", PropertyFlags::Public, System::PropertyTypeOf(&" << obj.FullName << "::" << getter->getName() << ", &" << obj.FullName << "::" << setter->getName() << "));\n";
				count_msprop++;
			}
			else if (getter)
			{
				writer << "\ttype.AddProperty(L\"" << msprop->getName() << "\", PropertyFlags::Public, System::PropertyTypeOf(&" << obj.FullName << "::" << getter->getName() << "));\n";
				count_msprop++;
			}
			else if (setter)
			{
				writer << "\ttype.AddProperty(L\"" << msprop->getName() << "\", PropertyFlags::Public, System::PropertyTypeOf(&" << obj.FullName << "::" << setter->getName() << "));\n";
				count_msprop++;
			}
		}

		if (count_msprop > 0) writer << "\n";

		size_t count_val = 0;
		for (auto val : obj.Values)
		{
			auto vads = val.first->getQualifiedNameAsString();
			if (!vads.empty())
			{
				utils::Trim(vads);
				if (vads.size() > 5)
					if (vads.find("class") == 0 || vads.find("struct") == 0)
						vads = vads.substr(5);
				vads = utils::Trim(vads);

				writer << "\ttype.AddValue(L\"" << val.first->getNameAsString() << "\", " << vads << ", __uhashstr(" << vads << "));\n";
				count_val++;
			}
		}

		if (count_val > 0) writer << "\n";

		size_t count_method = 0;
		for (auto m : obj.Methods)
		{
			std::string pr;
			llvm::raw_string_ostream prs(pr);

			std::string rettyn;

			bool noreturn = false;
			if (!m->isNoReturn())
			{
				auto retty = m->getReturnType();
				rettyn = GetTypeString(retty, astctx);
				if (!rettyn.empty())
				{
					utils::Trim(rettyn);
					if (rettyn.compare("void") == 0)
					{
						noreturn = true;
					}
				}
			}

			size_t m_param_upper = m->param_size() - 1;
			size_t m_param_counter = 0;
			for (auto param : m->parameters())
			{
				std::string ptyn = GetTypeString(param->getType(), astctx);
				if (!ptyn.empty())
				{
					utils::Trim(ptyn);
					if (ptyn.size() > 5)
						if (ptyn.find("class") == 0 || ptyn.find("struct") == 0)
							ptyn = ptyn.substr(5);
					ptyn = utils::Trim(ptyn);
					prs << ptyn;
				}
				if (m_param_counter < m_param_upper)
				{
					prs << ", ";
				}
				m_param_counter++;
			}

			if (m_param_counter == 0)
			{
				if (noreturn)
					writer << "\ttype.AddMethod(L\"" << m->getName() << "\", MethodFlags::Public, System::MethodTypeAdapter<" << m->param_size() << ", " << obj.FullName << ">::Create(&" << obj.FullName << "::" << m->getName() << "));\n";
				else
					writer << "\ttype.AddMethod(L\"" << m->getName() << "\", MethodFlags::Public, System::MethodTypeAdapter<" << m->param_size() << ", " << obj.FullName << ", " << rettyn << ">::Create(&" << obj.FullName << "::" << m->getName() << "));\n";
			}
			else
			{
				if (noreturn)
					writer << "\ttype.AddMethod(L\"" << m->getName() << "\", MethodFlags::Public, System::MethodTypeAdapter<" << m->param_size() << ", " << obj.FullName << ">::Create<" << prs.str() << ">(&" << obj.FullName << "::" << m->getName() << "));\n";
				else
					writer << "\ttype.AddMethod(L\"" << m->getName() << "\", MethodFlags::Public, System::MethodTypeAdapter<" << m->param_size() << ", " << obj.FullName << ", " << rettyn << ">::Create<" << prs.str() << ">(&" << obj.FullName << "::" << m->getName() << "));\n";
			}
			count_method++;
		}
		if (count_method > 0) writer << "\n";

		size_t count_operator = 0;
		for (auto o : obj.Operators)
		{
			std::string opname;
			llvm::raw_string_ostream opnames(opname);
			std::string oname;
			llvm::raw_string_ostream onames(oname);

#pragma region Operators
			auto okind = o->getOverloadedOperator();
			switch (okind)
			{
			case clang::OverloadedOperatorKind::OO_Comma:/*  ,  */
			{
				writer << "\ttype.AddOperator(L\"Comma\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Comma";
				onames << ",";
			}
			break;
			case clang::OverloadedOperatorKind::OO_ExclaimEqual:/*  !=  */
			{
				writer << "\ttype.AddOperator(L\"Inequality\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Inequality";
				onames << "!=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Percent:/*  %  */
			{
				writer << "\ttype.AddOperator(L\"Modulus\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Modulus";
				onames << "%";
			}
			break;
			case clang::OverloadedOperatorKind::OO_PercentEqual:/*  %=  */
			{
				writer << "\ttype.AddOperator(L\"ModulusAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::ModulusAssignment";
				onames << "%=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Amp:/*  &  */
			{
				if (o->param_size() > 0)
				{
					writer << "\ttype.AddOperator(L\"BitwiseAND\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::BitwiseAND";
				}
				else
				{
					writer << "\ttype.AddOperator(L\"AddressOf\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::AddressOf";
				}
				onames << "&";
			}
			break;
			case clang::OverloadedOperatorKind::OO_AmpAmp:/*  &&  */
			{
				writer << "\ttype.AddOperator(L\"LogicalAND\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LogicalAND";
				onames << "&&";
			}
			break;
			case clang::OverloadedOperatorKind::OO_AmpEqual:/*  &=  */
			{
				writer << "\ttype.AddOperator(L\"BitwiseANDAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::BitwiseANDAssignment";
				onames << "&=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Star:/*  *  */
			{
				if (o->param_size() > 0)
				{
					writer << "\ttype.AddOperator(L\"Multiplication\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::Multiplication";
				}
				else
				{
					writer << "\ttype.AddOperator(L\"PointerDereference\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::PointerDereference";
				}
				onames << "*";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Plus:/*  +  */
			{
				if (o->param_size() > 0)
				{
					writer << "\ttype.AddOperator(L\"Addition\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::Addition";
				}
				else
				{
					writer << "\ttype.AddOperator(L\"UnaryPlus\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::UnaryPlus";
				}
				onames << "+";
			}
			break;
			case clang::OverloadedOperatorKind::OO_PlusEqual:/*  +=  */
			{
				writer << "\ttype.AddOperator(L\"AdditionAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::AdditionAssignment";
				onames << "+=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Minus:/*  -  */
			{
				if (o->param_size() > 0)
				{
					writer << "\ttype.AddOperator(L\"Subtraction\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::Subtraction";
				}
				else
				{
					writer << "\ttype.AddOperator(L\"UnaryNegation\", System::OperatorTypeAdapter<";
					opnames << "System::Operators::UnaryNegation";
				}
				onames << "-";
			}
			break;
			case clang::OverloadedOperatorKind::OO_MinusEqual:/*  -=  */
			{
				writer << "\ttype.AddOperator(L\"SubtractionAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::SubtractionAssignment";
				onames << "-=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Arrow:/*  ->  */
			{
				writer << "\ttype.AddOperator(L\"MemberSelection\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::MemberSelection";
				onames << "->";
			}
			break;
			case clang::OverloadedOperatorKind::OO_ArrowStar:/*  ->*  */
			{
				writer << "\ttype.AddOperator(L\"PointerToMemberSelection\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::PointerToMemberSelection";
				onames << "->*";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Slash:/*  /  */
			{
				writer << "\ttype.AddOperator(L\"Division\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Division";
				onames << "/";
			}
			break;
			case clang::OverloadedOperatorKind::OO_SlashEqual:/*  /=  */
			{
				writer << "\ttype.AddOperator(L\"DivisionAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::DivisionAssignment";
				onames << "/=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Less:/*  <  */
			{
				writer << "\ttype.AddOperator(L\"LessThan\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LessThan";
				onames << "<";
			}
			break;
			case clang::OverloadedOperatorKind::OO_LessLess: /*  <<  */
			{
				writer << "\ttype.AddOperator(L\"LeftShift\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LeftShift";
				onames << "<<";
			}
			break;
			case clang::OverloadedOperatorKind::OO_LessLessEqual: /*  <<=  */
			{
				writer << "\ttype.AddOperator(L\"LeftShiftAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LeftShiftAssignment";
				onames << "<<=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_LessEqual:/*  <=  */
			{
				writer << "\ttype.AddOperator(L\"LessThanOrEqualTo\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LessThanOrEqualTo";
				onames << "<=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Equal: /*  =  */
			{
				writer << "\ttype.AddOperator(L\"Assignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Assignment";
				onames << "=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_EqualEqual: /*  ==  */
			{
				writer << "\ttype.AddOperator(L\"Equality\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Equality";
				onames << "==";
			}
			break;

			case clang::OverloadedOperatorKind::OO_Greater:/*  <  */
			{
				writer << "\ttype.AddOperator(L\"GreaterThan\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::GreaterThan";
				onames << ">";
			}
			break;
			case clang::OverloadedOperatorKind::OO_GreaterGreater: /*  <<  */
			{
				writer << "\ttype.AddOperator(L\"RightShift\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::RightShift";
				onames << ">>";
			}
			break;
			case clang::OverloadedOperatorKind::OO_GreaterGreaterEqual: /*  >>=  */
			{
				writer << "\ttype.AddOperator(L\"RightShiftAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::RightShiftAssignment";
				onames << ">>=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_GreaterEqual:/*  >=  */
			{
				writer << "\ttype.AddOperator(L\"GreaterThanOrEqualTo\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::GreaterThanOrEqualTo";
				onames << ">=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Caret:/*  ^  */
			{
				writer << "\ttype.AddOperator(L\"ExclusiveOR\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::ExclusiveOR";
				onames << "^";
			}
			break;
			case clang::OverloadedOperatorKind::OO_CaretEqual:/*  ^=  */
			{
				writer << "\ttype.AddOperator(L\"ExclusiveORAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::ExclusiveORAssignment";
				onames << "^=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Pipe:/*  |  */
			{
				writer << "\ttype.AddOperator(L\"BitwiseInclusiveOR\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::BitwiseInclusiveOR";
				onames << "|";
			}
			break;
			case clang::OverloadedOperatorKind::OO_PipeEqual:/*  |=  */
			{
				writer << "\ttype.AddOperator(L\"BitwiseInclusiveORAssignment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::BitwiseInclusiveORAssignment";
				onames << "|=";
			}
			break;
			case clang::OverloadedOperatorKind::OO_PipePipe:/*  ||  */
			{
				writer << "\ttype.AddOperator(L\"LogicalOR\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LogicalOR";
				onames << "||";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Exclaim:/*  !  */
			{
				writer << "\ttype.AddOperator(L\"LogicalNOT\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::LogicalNOT";
				onames << "!";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Tilde:/*  ~  */
			{
				writer << "\ttype.AddOperator(L\"OnesComplement\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::OnesComplement";
				onames << "~";
			}
			break;
			case clang::OverloadedOperatorKind::OO_PlusPlus:/*  ++  */
			{
				writer << "\ttype.AddOperator(L\"Increment\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Increment";
				onames << "++";
			}
			break;
			case clang::OverloadedOperatorKind::OO_MinusMinus:/*  --  */
			{
				writer << "\ttype.AddOperator(L\"Decrement\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Decrement";
				onames << "--";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Call:/*  ()  */
			{
				writer << "\ttype.AddOperator(L\"ConversionOperators\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::ConversionOperators";
				onames << "()";
			}
			break;
			case clang::OverloadedOperatorKind::OO_Subscript:/*  []  */
			{
				writer << "\ttype.AddOperator(L\"Subscripting\", System::OperatorTypeAdapter<";
				opnames << "System::Operators::Subscripting";
				onames << "[]";
			}
			break;
			}
#pragma endregion

			if (!onames.str().empty())
			{
				std::string pr;
				llvm::raw_string_ostream prs(pr);
				std::string rettyn;

				bool noreturn = false;
				if (!o->isNoReturn())
				{
					auto retty = o->getReturnType();
					rettyn = GetTypeString(retty, astctx);
					if (!rettyn.empty())
					{
						utils::Trim(rettyn);
						if (rettyn.compare("void") == 0)
						{
							noreturn = true;
						}
					}
				}

				size_t m_param_upper = o->param_size() - 1;
				size_t m_param_counter = 0;
				for (auto param : o->parameters())
				{
					std::string ptyn = GetTypeString(param->getType(), astctx);
					if (!ptyn.empty())
					{
						utils::Trim(ptyn);
						if (ptyn.size() > 5)
							if (ptyn.find("class") == 0 || ptyn.find("struct") == 0)
								ptyn = ptyn.substr(5);
						ptyn = utils::Trim(ptyn);
						prs << ptyn;
					}
					if (m_param_counter < m_param_upper)
					{
						prs << ", ";
					}
					m_param_counter++;
				}
				if (m_param_counter == 0)
				{
					if (noreturn)
						writer << o->param_size() << ", " << opnames.str() << ", " << obj.FullName << ">::Create(&" << obj.FullName << "::operator" << onames.str() << "));\n";
					else
						writer << o->param_size() << ", " << opnames.str() << ", " << obj.FullName << ", " << rettyn << ">::Create(&" << obj.FullName << "::operator" << onames.str() << "));\n";
				}
				else
				{
					if (noreturn)
						writer << o->param_size() << ", " << opnames.str() << ", " << obj.FullName << ">::Create<" << prs.str() << "(&" << obj.FullName << "::operator" << onames.str() << "));\n";
					else
						writer << o->param_size() << ", " << opnames.str() << ", " << obj.FullName << ", " << rettyn << ">::Create<" << prs.str() << ">(&" << obj.FullName << "::operator" << onames.str() << "));\n";
				}
			}
		}

		if (count_operator > 0) writer << "\n";

		pool.Add(obj.Assembly, obj.Version.ToString(), obj.Namespace);

		writer << "\t_uassembly_" << obj.Assembly << "::Regisiter(_uassembly_" << obj.Assembly << "::Versions::_" << obj.SplitedVersion << ", _uassembly_" << obj.Assembly << "::Nodes::" << obj.SplitedNamespace << ", &type);\n\n";

		writer << "}\n#pragma endregion\n";

		if (obj_counter < obj_upper) writer << "\n\n\n";

		obj_counter++;

	}
	writer << "#endif";
	writer.flush();

	ci.clearOutputFiles(false);

	if (llvm::sys::fs::rename(temp_utf8, dest_utf8))
	{
		llvm::sys::fs::remove(temp_utf8);
	}

	if (llvm::sys::fs::exists(dest_utf8))
		pool.GeneratedPool.push_back(dest);
	else
	{
		disp->SendError("", -1, -1, std::string("%(CodeGenerator): %(File)%(CreationFailed) ") + dest);

		std::stringstream outstr2;
		outstr2 << "%(File) " << dest << " %(CreationFailed)";

		disp->SendOutput(outstr2.str());

		return false;
	}

	std::stringstream outstr3;
	outstr3 << "%(CodeFile) " << dest << " %(Created)";
	disp->SendOutput(outstr3.str());

	return true;
}

#pragma endregion