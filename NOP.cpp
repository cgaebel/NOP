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

/*
	TODO:

	Replace the HashManager's backend with : ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vclib/html/f86552e8-8acd-4b23-bdc5-0c3a247ebd74.htm
	instead of winsock. I hear its faster, and easier.

	Create a universal banning system. When a hack is detected, the person gets banned. When the game starts up, it does an HTTP pull from
	my server to see if the person is banned. If banned, die, else, continue.
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
