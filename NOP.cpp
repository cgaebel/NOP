#include "stdafx.h"
#include "NOP.h"
#include "Core.h"

using namespace std;

/*
	Integration steps:

	Start up the gunzlauncher
	Pack and crypt my dll
	Put all relevant files in the gunz directory
	Use COFF explorer to add OnHackDetected as an import to gunz.exe
	Replace ZPostConnect with retn
	//Replace ZGame::PostBasicInfo with retn
	Fix the checksums on the server.
*/

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
