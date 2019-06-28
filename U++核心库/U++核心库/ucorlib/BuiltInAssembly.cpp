#include "BuiltInAssembly.h"
#include "Assembly_impl.h"
#include "AppDomain_impl.h"
#include "AppDomain.h"
#include "NameSpace_impl.h"
//#include "GuidHelper.h"
#include "Type.h"
#include <Aclapi.h>
using namespace System;
#define PROTECTED_DACL_SECURITY_INFORMATION (0x80000000L)
#pragma init_seg( ".CRT$XCB" )

static struct LOCK
{
	LOCK()
	{
		HANDLE hProcess = ::GetCurrentProcess();
		SID_IDENTIFIER_AUTHORITY sia = SECURITY_WORLD_SID_AUTHORITY;
		PSID pSid;
		BOOL bSus = FALSE;
		bSus = ::AllocateAndInitializeSid(&sia, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &pSid);
		if (!bSus) goto Cleanup;
		HANDLE hToken;
		bSus = ::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);
		if (!bSus) goto Cleanup;
		DWORD dwReturnLength;
		::GetTokenInformation(hToken, TokenUser, NULL, NULL, &dwReturnLength);
		if (dwReturnLength > 0x400) goto Cleanup;
		LPVOID TokenInformation;
		TokenInformation = ::LocalAlloc(LPTR, 0x400);//这里就引用SDK的函数不引用CRT的了
		DWORD dw;
		bSus = ::GetTokenInformation(hToken, TokenUser, TokenInformation, 0x400, &dw);
		if (!bSus) goto Cleanup;
		PTOKEN_USER pTokenUser = (PTOKEN_USER)TokenInformation;
		BYTE Buf[0x200];
		PACL pAcl = (PACL)&Buf;
		bSus = ::InitializeAcl(pAcl, 1024, ACL_REVISION);
		if (!bSus) goto Cleanup;
		bSus = ::AddAccessDeniedAce(pAcl, ACL_REVISION, 0x000000FA, pSid);
		if (!bSus) goto Cleanup;
		bSus = ::AddAccessAllowedAce(pAcl, ACL_REVISION, 0x00100701, pTokenUser->User.Sid);
		if (!bSus) goto Cleanup;
		if (::SetSecurityInfo(hProcess, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION, NULL, NULL, pAcl, NULL) == 0)
			bSus = TRUE;
	Cleanup:
		if (hProcess != NULL)
			::CloseHandle(hProcess);
		if (pSid != NULL)
			::FreeSid(pSid);
	}
}ULOCK;

AppDomain_impl* AppDomain::impl = new AppDomain_impl();

const Assembly* BuiltInAssemblyFactory::System = Assembly_impl::Create(L"ucorlib", Version(1, 0), { 0x1c112294, 0xb1b8, 0x4288, { 0x98, 0xfa, 0xb5, 0x34, 0x1, 0x43, 0x15, 0xc } }, new NameSpace(L"System"));



BuiltInAssemblyFactory::BuiltInAssemblyFactory() 
{

}

BuiltInAssemblyFactory& BuiltInAssemblyFactory::GetBuiltIn()
{
	static BuiltInAssemblyFactory instance;
	return instance;
}

Type* BuiltInAssemblyFactory::Regisiter(Type* type)
{
	return NameSpace_impl::InternalRegisiter(Assembly_impl::Get(System, L"System"), type);
}
