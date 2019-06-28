#include "CriticalSection.h"
#include "WindowsEnvironment.h"

using namespace System;
using namespace System::OS;

CriticalSection::CriticalSection()
{
	impl = Microsoft::winapi_InitializeCriticalSection();
}

CriticalSection::~CriticalSection()
{
	Microsoft::winapi_DeleteCriticalSection(impl);
}

bool CriticalSection::TryEnter()
{
	return Microsoft::winapi_TryEnterCriticalSection(impl);
}

void CriticalSection::Enter()
{
	Microsoft::winapi_EnterCriticalSection(impl);
}

void CriticalSection::Leave()
{
	Microsoft::winapi_LeaveCriticalSection(impl);
}