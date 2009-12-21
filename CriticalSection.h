#pragma once
#include <windows.h>

class CriticalSection
{
private:
	CRITICAL_SECTION cs;

public:
	CriticalSection();

	void Enter();
	void Exit();

	~CriticalSection();
};
