#include "stdafx.h"
#include "Core.h"
#include "DllInstance.h"

using namespace std;

static void SaveInstance(HINSTANCE dllHandle)
{
	DllInstance::GetInstance() = dllHandle;
}

bool DllMain(HINSTANCE dllHandle, DWORD reason, void*)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(dllHandle);
		SaveInstance(dllHandle);
		StartAntiHack();
	}
	else if(reason == DLL_PROCESS_DETACH)
	{
		ExitProcess(0);
	}

	return true;
}
