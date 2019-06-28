#include "ASTParser.h"
#include <clang/Basic/Version.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/LexDiagnostic.h>

#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Type.h>
#include <clang/Sema/Sema.h>
#include <clang/Sema/Lookup.h>
#include <clang/AST/Attr.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLParser.h>
#include <llvm/Support/SourceMgr.h>

#include <iostream>
#include <ostream>
#include <sstream>

#include <Windows.h>
#include "Utils.h"

using namespace uppcl;

static bool isbaseofobject(const clang::CXXRecordDecl* RD)
{
	if (!RD) return false;
	if (RD->isClass())
	{
		if (RD->getQualifiedNameAsString() == "System::Object") return true;
		for (auto bs = RD->bases_begin(); bs != RD->bases_end(); ++bs)
		{
			auto bsty = bs->getType().getTypePtr();
			auto bs_decl = bsty->getAsCXXRecordDecl();
			if (bs_decl)
			{
				if (isbaseofobject(bs_decl)) return true;
			}
		}
	}
	return false;
}
enum class ETypes
{
	unsupported,
	boolean,
	uint8,
	int8,
	uint16,
	int16,
	uint32,
	int32,
	slong,
	ulong,
	uint64,
	int64
};

template <class X>
inline typename X Casting(long long val)
{
	if (val > (long long)std::numeric_limits<X>::max())
	{
		return std::numeric_limits<X>::max();
	}
	else if (val < (long long)std::numeric_limits<X>::min())
	{
		return std::numeric_limits<X>::min();
	}
	return (X)val;
}

__inline std::string Generate(long long val, ETypes etype)
{
	std::string result = "0";
	switch (etype)
	{
	case ETypes::unsupported:
		break;
	case ETypes::boolean:
		result = val > 0 ? "true" : "false";
		break;
	case ETypes::int8:
	{
		result = "'";
		result.push_back(Casting<char>(val));
		result += "'";
	}
	break;
	case ETypes::int16:
		result = std::to_string(Casting<short>(val));
		break;
	case ETypes::int32:
		result = std::to_string(Casting<int>(val));
		break;
	case ETypes::slong:
		result = std::to_string(Casting<long>(val)) + "l";
		break;
	case ETypes::int64:
		result = std::to_string(val) + "ull";
		break;
	default:
		break;
	}
	return result;
}

template <class X>
inline typename X CastingU(unsigned long long val)
{
	if (val > (unsigned long long)std::numeric_limits<X>::max())
	{
		return std::numeric_limits<X>::max();
	}
	return (X)val;
}

__inline std::string Generate(unsigned long long val, ETypes etype)
{
	std::string result = "0";
	switch (etype)
	{
	case ETypes::unsupported:
		break;
	case ETypes::uint8:
	{
		result = "'";
		result.push_back(CastingU<unsigned char>(val));
		result += "'";
	}
	break;
	case ETypes::uint16:
		result = std::to_string(CastingU<unsigned short>(val)) + "u";
		break;
	case ETypes::uint32:
		result = std::to_string(CastingU<unsigned int>(val)) + "u";
		break;
	case ETypes::ulong:
		result = std::to_string(CastingU<unsigned long>(val)) + "ul";
		break;
	case ETypes::uint64:
		result = std::to_string(val) + "ull";
		break;
	default:
		break;
	}
	return result;
}

__inline long long Parse(const std::string& str, ETypes etype, bool& re)
{
	long long result = 0;
	switch (etype)
	{
	case ETypes::unsupported:
		break;
	case ETypes::boolean:
	{
		size_t csize = str.size();
		if (csize < 1)
		{
			re = false;
		}
		else
		{
			if (csize == 1)
			{
				wchar_t fC = str[0];
				if (fC == 'T' || fC == 't' || fC == '1')
				{
					result = true;
				}
				else if (fC == 'F' || fC == 'f' || fC == '0')
				{
					result = false;
				}
				else
				{
					re = false;
				}
			}
			if (csize == 4)
			{
				if (str == "True" || str == "true")
				{
					result = true;
				}
				else
				{
					re = false;
				}
			}
			if (csize == 5)
			{
				if (str == "False" || str == "false")
				{
					result = true;
				}
				else
				{
					re = false;
				}
			}
		}
	}
	break;
	case ETypes::int8:
	{
		try
		{
			result = (char)std::stoi(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::int16:
	{
		try
		{
			result = (short)std::stoi(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::int32:
	{
		try
		{
			result = std::stoi(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::slong:
	{
		try
		{
			result = std::stol(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::int64:
	{
		try
		{
			result = std::stoll(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	default:
		break;
	}
	return result;
}
__inline unsigned long long ParseU(const std::string& str, ETypes etype, bool& re)
{
	long long result = 0;
	switch (etype)
	{
	case ETypes::unsupported:
		re = false;
		break;
	case ETypes::boolean:
	{
		size_t csize = str.size();
		if (csize < 1)
		{
			re = false;
		}
		else
		{
			if (csize == 1)
			{
				wchar_t fC = str[0];
				if (fC == 'T' || fC == 't' || fC == '1')
				{
					result = true;
				}
				else if (fC == 'F' || fC == 'f' || fC == '0')
				{
					result = false;
				}
				else
				{
					re = false;
				}
			}
			if (csize == 4)
			{
				if (str == "True" || str == "true")
				{
					result = true;
				}
				else
				{
					re = false;
				}
			}
			if (csize == 5)
			{
				if (str == "False" || str == "false")
				{
					result = true;
				}
				else
				{
					re = false;
				}
			}
		}
	}
	break;
	case ETypes::uint8:
	{
		try
		{
			result = (unsigned char)std::stoi(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::uint16:
	{
		try
		{
			result = (unsigned short)std::stoi(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::uint32:
	{
		try
		{
			result = (unsigned int)std::stoul(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::ulong:
	{
		try
		{
			result = std::stoul(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	case ETypes::uint64:
	{
		try
		{
			result = std::stoull(str);
			return true;
		}
		catch (...)
		{
			re = false;
		}
	}
	break;
	default:
		break;
	}
	return result;
}

UClass UASTConsumer_base::ParseClass(clang::CXXRecordDecl* RD, clang::ASTContext* ast, std::shared_ptr<Dispatcher> disp)
{
	UClass Def;

	//if (RD->isClass())
	//{
	//	auto attrs = RD->getAttrs();
	//	for (auto att : attrs)
	//	{
	//		//clang::attr::ms
	//		printf("%s,%d\n", att->getSpelling(), (int)att->getKind());
	//	}
	//}

	
	if (RD->isClass())
	{
		clang::UXXObjectDefinationAttr* uobjdef = RD->getAttr<clang::UXXObjectDefinationAttr>();
		if (uobjdef)
		{
			if (!uobjdef->getAssembly().empty())
			{
				Def.Assembly = std::string(uobjdef->getAssembly());
				if (!uobjdef->getNameSpace().empty())
				{
					Def.Namespace = std::string(uobjdef->getNameSpace());
					std::vector<std::string> nssplits;

					Def.SplitedNamespace = Def.Namespace;
					Def.SplitedNamespace = utils::ReplaceAllDistinct(Def.SplitedNamespace, std::string("::"), std::string("_"));

					UVersion::TryParse(uobjdef->getVersion(), Def.Version);

					Def.SplitedVersion = Def.Version.ToString();
					Def.SplitedVersion = utils::ReplaceAllDistinct(Def.SplitedVersion, std::string("."), std::string("_"));

					clang::UXXRefAttr* uref = RD->getAttr<clang::UXXRefAttr>();
					if (uref)
					{
						Def.Record = RD;

						Def.FullName = RD->getQualifiedNameAsString();
						Def.Name = RD->getNameAsString();

						std::vector<std::string> splits;

						Def.SplitedQualifiedName = Def.FullName;
						Def.SplitedQualifiedName = utils::ReplaceAllDistinct(Def.SplitedQualifiedName, std::string("::"), std::string("_"));

						for (auto bs = RD->bases_begin(); bs != RD->bases_end(); ++bs)
						{
							auto bsty = bs->getType().getTypePtr();
							auto bs_decl = bsty->getAsCXXRecordDecl();
							if (bs_decl)
							{
								if (bs_decl->getQualifiedNameAsString() != "System::Object")
								{
									if (isbaseofobject(bs_decl))
									{
										Def.Bases.push_back(bs_decl);
									}
								}
							}
						}

						ETypes etype = ETypes::unsupported;
						bool useunsigned = false;
						for (auto it = RD->decls_begin(); it != RD->decls_end(); ++it)
						{
							clang::Decl::Kind k = it->getKind();
							if (clang::TypedefDecl* tydef = llvm::dyn_cast<clang::TypedefDecl>(*it))
							{
								if (tydef->getName().trim().compare("enumbase") == 0)
								{
									auto ety = tydef->getUnderlyingType().getUnqualifiedType().getTypePtr();
									if (ety)
									{
										const clang::BuiltinType *bt = clang::dyn_cast<clang::BuiltinType>(ety->getCanonicalTypeUnqualified());
										if (bt)
										{
											switch (bt->getKind())
											{
											case clang::BuiltinType::Bool:
												etype = ETypes::boolean;
												break;
											case clang::BuiltinType::Char_S:
												etype = ETypes::int8;
												break;
											case clang::BuiltinType::Char_U:
											{
												etype = ETypes::uint8;
												useunsigned = true;
											}
											break;
											case clang::BuiltinType::Char16:
											case clang::BuiltinType::Short:
												etype = ETypes::int16;
												break;
											case clang::BuiltinType::UShort:
											{
												etype = ETypes::uint16;
												useunsigned = true;
											}
											break;
											case clang::BuiltinType::Char32:
											case clang::BuiltinType::Int:
												etype = ETypes::int32;
												break;
											case clang::BuiltinType::UInt:
											{
												etype = ETypes::uint32;
												useunsigned = true;
											}
											break;
											case clang::BuiltinType::Long:
												etype = ETypes::slong;
												break;
											case clang::BuiltinType::ULong:
											{
												etype = ETypes::ulong;
												useunsigned = true;
											}
											break;
											case clang::BuiltinType::LongLong:
												etype = ETypes::int64;
												break;
											case clang::BuiltinType::ULongLong:
											{
												etype = ETypes::uint64;
												useunsigned = true;
											}
											break;
											}
										}
									}
								}
							}
						}
						for (auto it = RD->decls_begin(); it != RD->decls_end(); ++it)
						{
							clang::Decl::Kind k = it->getKind();
							if (clang::EnumDecl *E = llvm::dyn_cast<clang::EnumDecl>(*it))
							{
								clang::UXXValueAttr* vattr = E->getAttr<clang::UXXValueAttr>();
								if (vattr)
								{
									for (auto it2 = E->decls_begin(); it2 != E->decls_end(); ++it2)
									{
										if (clang::EnumConstantDecl *V = llvm::dyn_cast<clang::EnumConstantDecl>(*it2))
										{
											Def.Values.insert(std::make_pair(V, std::string(V->getInitVal().toString(10, true))));
										}
									}

								}
							}
							else if (clang::CXXConstructorDecl *C = llvm::dyn_cast<clang::CXXConstructorDecl>(*it))
							{
								clang::UXXRefAttr* cattr = C->getAttr<clang::UXXRefAttr>();
								if (cattr)
								{
									clang::UXXDefaultConstructorAttr* defctor_attr = C->getAttr<clang::UXXDefaultConstructorAttr>();
									if (defctor_attr)
										Def.DefaultConstructor = C;
									else
									{
										clang::UXXConstructorAttr* ctor_attr = C->getAttr<clang::UXXConstructorAttr>();
										if (ctor_attr) Def.Constructors.push_back(C);
									}
								}
							}
							else if (clang::MSPropertyDecl *MSP = llvm::dyn_cast<clang::MSPropertyDecl>(*it))
							{
								clang::UXXRefAttr* cattr = MSP->getAttr<clang::UXXRefAttr>();
								if (cattr)
								{
									Def.MSProperties.push_back(MSP);
								}
							}
							else if (clang::CXXMethodDecl *M = llvm::dyn_cast<clang::CXXMethodDecl>(*it))
							{
								clang::UXXRefAttr* mattr = M->getAttr<clang::UXXRefAttr>();
								if (mattr)
								{
									if (M->isStatic())
									{
										if (!M->isNoReturn())
										{
											clang::UXXConstructorAttr* ctor_attr = M->getAttr<clang::UXXConstructorAttr>();
											if (ctor_attr)
											{
												auto rety = M->getReturnType().getTypePtr();
												if (rety->isPointerType())
												{
													auto re_decl = rety->getPointeeCXXRecordDecl();
													if (re_decl)
													{
														bool same = re_decl == RD;
														if (!same) same = re_decl->getQualifiedNameAsString().compare(RD->getQualifiedNameAsString()) == 0;
														if (same) Def.StaticConstructors.push_back(M);
													}
												}
											}
										}
									}
									auto oper = M->getOverloadedOperator();
									switch (oper)
									{
									case clang::OverloadedOperatorKind::OO_None:
										Def.Methods.push_back(M);
										break;
									case clang::OverloadedOperatorKind::OO_New:
										break;
									default:
										Def.Operators.push_back(M);
									}
								}
							}
						}

						if (!Def.DefaultConstructor)
						{
							for (auto it = RD->method_begin(); it != RD->method_end(); ++it)
							{
								clang::UXXRefAttr* cattr = it->getAttr<clang::UXXRefAttr>();
								if (cattr)
								{
									clang::UXXDefaultConstructorAttr* defctor_attr = it->getAttr<clang::UXXDefaultConstructorAttr>();
									if (it->param_size() == 0)
									{
										if (it->isStatic())
										{
											if (!it->isNoReturn())
											{
												auto rety = it->getReturnType().getTypePtr();
												if (rety->isPointerType())
												{
													auto re_decl = rety->getPointeeCXXRecordDecl();
													if (re_decl)
													{
														bool same = re_decl == RD;
														if (!same)
														{
															same = re_decl->getQualifiedNameAsString().compare(RD->getQualifiedNameAsString()) == 0;
														}
														if (same)
														{
															Def.StaticDefaultConstructor = *it;
															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}

						bool val_first = true;
						if (useunsigned)
						{
							std::string uvalstr_last = "";
							bool uvalstr_last_parsed = true;
							unsigned long long uval_last = 0;
							for (auto& val : Def.Values)
							{
								if (val.second.empty())
								{
									if (val_first)
									{
										val.second = "0u";
										uval_last++;
										val_first = false;
									}
									else
									{
										if (uvalstr_last_parsed)
											val.second = Generate(uval_last, etype);
										else
										{
											switch (etype)
											{
											case ETypes::uint16:
											case ETypes::uint32:
												val.second = uvalstr_last + " + 1u";
												break;
											case ETypes::ulong:
												val.second = uvalstr_last + " + 1ul";
												break;
											case ETypes::uint64:
												val.second = uvalstr_last + " + 1ull";
												break;
											default:
												val.second = uvalstr_last + " + 1";
												break;
											}
										}
										uval_last++;
									}
								}
								else
								{
									uvalstr_last = utils::Trim(val.second);
									uvalstr_last_parsed = true;
									unsigned long long tuval = ParseU(uvalstr_last, etype, uvalstr_last_parsed);
									if (uvalstr_last_parsed) uval_last = tuval;
								}
							}
						}
						else
						{
							std::string valstr_last = "";
							bool valstr_last_parsed = true;
							long long val_last = 0;
							for (auto& val : Def.Values)
							{
								if (val.second.empty())
								{
									if (val_first)
									{
										val.second = "0";
										val_last++;
										val_first = false;
									}
									else
									{
										if (valstr_last_parsed)
											val.second = Generate(val_last, etype);
										else
										{
											switch (etype)
											{
											case ETypes::uint16:
											case ETypes::uint32:
												val.second = valstr_last + " + 1u";
												break;
											case ETypes::ulong:
												val.second = valstr_last + " + 1ul";
												break;
											case ETypes::uint64:
												val.second = valstr_last + " + 1ull";
												break;
											default:
												val.second = valstr_last + " + 1";
												break;
											}
										}
										val_last++;
									}
								}
								else
								{
									valstr_last = utils::Trim(val.second);
									valstr_last_parsed = true;
									long long tval = Parse(valstr_last, etype, valstr_last_parsed);
									if (valstr_last_parsed) val_last = tval;
								}
							}
						}

						Def.Valid = true;
					}
				}
			}
		}
	}
	
	return Def;
}