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

// The one and only application object.
CNop theApp;

#define ANNOY(num) MessageBoxA(NULL, #num, "Beep!", MB_OK)

#pragma warning(push)
#pragma warning(disable:6309)
#pragma warning(disable:6387)
BOOL CNop::InitInstance()
{
	try {
		HideThreadFromDebugger();

		InitOverlay();

		// initialize features and shit. MFC/ATL/Winsock
		if(WSAStartup(0x0202, &wsd))
			OnFailure("Could not initialize winsock.");
		InitHashTree();

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

#ifndef _DEBUG
		CProtectionManager::Get()->AddPassiveProtection(ModuleHiding);
#endif
		CProtectionManager::Get()->AddPassiveProtection(RestoreZPostConnect);	// The restore MUST be done BEFORE the return address check.
		//CProtectionManager::Get()->AddPassiveProtection(RestorePostBasicInfo);
		CProtectionManager::Get()->AddPassiveProtection(CheckReturnAddress);
#ifndef _DEBUG
		CProtectionManager::Get()->AddPassiveProtection(FileHash);
#endif

#ifndef _DEBUG
		CProtectionManager::Get()->AddActiveProtection(DetectDebuggers);
#endif
		CProtectionManager::Get()->AddActiveProtection(TrainerDetection);
		CProtectionManager::Get()->AddActiveProtection(CodeSegmentCheck);
		//CProtectionManager::Get()->AddActiveProtection(APIHookCheck);

		CProtectionManager::Get()->BeginActiveProtection();
	} catch(...) {
		OnFailure("Something that should not have gone wrong, did go wrong.");
	}

	return TRUE;
}
#pragma warning(pop)

int CNop::ExitInstance()
{
	return __super::ExitInstance();
}