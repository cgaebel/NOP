#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&cs);
}

void CriticalSection::Enter()
{
	EnterCriticalSection(&cs);
}

void CriticalSection::Exit()
{
	LeaveCriticalSection(&cs);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&cs);
}
