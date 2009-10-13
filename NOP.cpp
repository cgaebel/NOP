#include "stdafx.h"
#include "NOP.h"
#include "HashManager.h"
#include "FastSystemCall.h"
#include "Patching.h"
#include "CProtectionManager.h"
#include "Overlay.h"

using namespace std;

/*
	Integration steps:

	Start up the gunzlauncher
	Replace the string [x]Freestyle.dll with NOP.dll in fmod.dll
	Replace ZPostConnect with retn
	//Replace ZGame::PostBasicInfo with retn
	Pack and crypt my dll
	Put all relevant files in the gunz directory
	Fix the checksums on the server.
*/

/*
	TODO:

	Replace the HashManager's backend with : ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vclib/html/f86552e8-8acd-4b23-bdc5-0c3a247ebd74.htm
	instead of winsock. I hear its faster, and easier.

	Create a universal banning system. When a hack is detected, the person gets banned. When the game starts up, it does an HTTP pull from
	my server to see if the person is banned. If banned, die, else, continue.

	Use http://forum.ragezone.com/f245/unlinking-modules-from-the-peb-595839/ as another anti-cheat method.
*/

HINSTANCE g_hInstance;
static WSADATA wsd;

#define ANNOY(num) MessageBoxA(NULL, #num, "Beep!", MB_OK)

bool DllMain(HINSTANCE hDllHandle, DWORD reason, void*)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hDllHandle);
		g_hInstance = hDllHandle;

		try {
			HideThreadFromDebugger();

			InitOverlay();

			// initialize features and shit. MFC/ATL/Winsock
			if(WSAStartup(0x0202, &wsd))
				OnFailure("Could not initialize winsock.");
			HashManager::Get()->InitHashTree();

			/*
				Protection modules:

				Active...
					Trainer detection (Icky implementation)		- DONE!
					Code segment hashing						- DONE!
					Debugger detection (Siberian Tiger by gWX0)	- DONE!
					API Checking (See Fury by gWX0)				- DONE!
					Threads with bad entry points (Fury, gWX0)	- DONE!
					Modules with bad base address (Fury, gWX0)	- DONE!

					Check if the rootkit is loaded (optional)	- 0xFF
				Passive...
					File hashing								- DONE!
					Return address checking						- DONE!
					Hide the module								- DONE!
					Restore ZPostConnect after removal			- DONE!

					Chat hooks (overflow & spam prevention)		- 0xFD
					HexVoid's restart method to stop injection	- 0x100
					Load the rootkit (optional)					- 0xFE
			*/

	#ifdef NDEBUG
			CProtectionManager::Get()->AddPassiveProtection(ModuleHiding);
			//CProtectionManager::Get()->AddPassiveProtection(HideFromPEB);		// <- UNTESTED.
	#endif
			CProtectionManager::Get()->AddPassiveProtection(RestoreZPostConnect);	// The restore MUST be done BEFORE the return address check.
			//CProtectionManager::Get()->AddPassiveProtection(RestorePostBasicInfo);	// <- TODO
			CProtectionManager::Get()->AddPassiveProtection(CheckReturnAddress);
	#ifdef NDEBUG
			CProtectionManager::Get()->AddPassiveProtection(FileHash);
	#endif

	#ifdef NDEBUG
			//CProtectionManager::Get()->AddActiveProtection(DetectDebuggers);	// <- BROKEN MODULE.
	#endif
			CProtectionManager::Get()->AddActiveProtection(TrainerDetection);
			CProtectionManager::Get()->AddActiveProtection(CodeSegmentCheck);
			//CProtectionManager::Get()->AddActiveProtection(APIHookCheck);		// <- BROKEN MODULE.

			CProtectionManager::Get()->BeginActiveProtection();
		} catch(...) {
			OnFailure("Something that should not have gone wrong, did go wrong.");
		}
	}
	else if(reason == DLL_PROCESS_DETACH)
	{
		ExitProcess(0);
	}

	return true;
}
