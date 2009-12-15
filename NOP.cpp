#include "stdafx.h"
#include "NOP.h"
#include "Core.h"

using namespace std;

HINSTANCE g_hInstance;

bool DllMain(HINSTANCE hDllHandle, DWORD reason, void*)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		LogInformation("Attached!");

		DisableThreadLibraryCalls(hDllHandle);
		g_hInstance = hDllHandle;

		try {
			StartAntiHack();

		} catch(exception& ex) {
			OnFailure(ex.what());
		} catch(...) {
			OnFailure("Something that should not have gone wrong, did go wrong.");
		}

		LogInformation("Done!");
	}
	else if(reason == DLL_PROCESS_DETACH)
	{
		ExitProcess(0);
	}

	return true;
}
