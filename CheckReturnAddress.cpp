#include "Core.h"
#include "Patching.h"
#include "defs.h"

PASSIVE_PROTECTION(CheckReturnAddress, "Patching protected functions...")
{
	// We patch all the functions listed by creating codecaves to check the return value and
	// jumping to it in the epiloge of each function. The codecave checks to see if the return
	// value of the function is within Gunz.exe's code segment. If it isn't, it calls OnHackDetected
	// with a parameter of "Dll Detected!". We have two codecaves - the first for just "retn" and
	// the other is for "retn 4". This is to allow for a not only a calling convention of cdecl,
	// but also stdcall (with 1 DWORD-sized arg).

#define PATCH_STRING(addr, str)		Patching::Patch((void*)(addr), str, strlen(str))

	PATCH_STRING(0x005E5EC0, DLL_NAME);
	PATCH_STRING(0x005E5ED1, "OnHackDetected");
	PATCH_STRING(0x005E5EE0, "Dll Detected!");

#undef PATCH_STRING

	// Build the codecaves.
	Patching::Patch((void*)0x005E5EEF,
		"\x81\xC4\x14\x01\x00\x00"			// add esp, 114								; 0x005E5EEF
		"\xEB\x1D"							// jmp short 0x005E5F14						; 0x005E5EF5
		"\x8B\x00"							// mov eax, dword ptr ds:[eax]				; 0x005E5EF7
		"\xEB\x19"							// jmp short 0x005E5F14						; 0x005E5EF9
		"\x83\xC0\x54"						// add eax, 54								; 0x005E5EFB
		"\xEB\x14"							// jmp short 0x005E5F14						; 0x005E5EFE
		"\x83\xC4\x20"						// add esp, 20								; 0x005E5F00
		"\xEB\x0F"							// jmp short 0x005E5F14						; 0x005E5F03
		"\x64\x89\x0D\x00\x00\x00\x00"		// mov dword ptr fs:[0], ecx				; 0x005E5F05
		"\x83\xC4\x1C"						// add esp, 1C								; 0x005E5F0C
		"\xEB\x03"							// jmp short 0x005E5F14						; 0x005E5F0F
		"\x83\xC4\x10"						// add esp, 10								; 0x005E5F11
	,37);

	const char patchBuffer[] = 
		"\x3E\x81\x3C\xE4\xFF\x5F\x5E\x00\x7F\x0B\x3E\x81\x3C\xE4\x00\x00"
		"\x40\x00\x7C\x01\xC3\xB8\xD0\x5E\x5E\x00\x40\x50\xB8\xBF\x5E\x5E"
		"\x00\x40\x50\xE8"
			"\x00\x00\x00\x00"	// LoadLibraryA
		"\x50\xE8"
			"\x00\x00\x00\x00"	// GetProcAddress
		"\x83\xF8"
		"\x00\x0F\x84\xB5\xA0\xE1\xFF\xBB\xDF\x5E\x5E\x00\x43\x53\xFF\xD0"
		"\x83\xC4\x04\xE9\xA4\xA0\xE1\xFF\x00\x00";

	Patching::Patch((void*)0x005E5F14, patchBuffer, sizeof(patchBuffer) - 1);

	/*
		The following patch is a bit confusing, so bear with me.
		
		HACK:		Calls a jump into LoadLibraryA and GetProcAddress.

		RATIONALE:	Since the locations of LoadLibraryA and GetProcAddress are not static, the memory
					checksum will be different for each computer and after every reboot.

		METHOD:		I put the actual adresses at the end of the code segment for easy exclusion from
					the hash. Then, the codecave itself "calls" the end of the code segment, which
					redirects the call with a jmp to the correct address.
	*/
	{
		DWORD tempDistance;

		tempDistance = Patching::GetDistanceToAddress(0x005E5F37, 0x005E5FF6);
		Patching::Patch((void*)(0x005E5F37 + 1), &tempDistance, sizeof(tempDistance));

		tempDistance = Patching::GetDistanceToAddress(0x005E5F3D, 0x005E5FFB);
		Patching::Patch((void*)(0x005E5F3D + 1), &tempDistance, sizeof(tempDistance));

		Patching::PatchUnconditionalJump(0x005E5FF6, (DWORD)LoadLibraryA);
		Patching::PatchUnconditionalJump(0x005E5FFB, (DWORD)GetProcAddress);
	}

	Patching::Patch((void*)0x005E5F7A,
		"\x83\xC4\x20"					// add esp, 20								; 0x005E5F7A
		"\xEB\x0F"						// jmp short 0x005E5F8E						; 0x005E5F7D
		"\x64\x89\x0D\x00\x00\x00\x00"	// mov dword ptr fs:[0], ecx				; 0x005E5F7F
		"\x83\xC4\x1C"					// add esp, 1C								; 0x005E5F86
		"\xEB\x03"						// jmp short 0x005E5F8E						; 0x005E5F89
		"\x83\xC4\x10"					// add esp, 10								; 0x005E5F8B
	, 20);

	Patching::Patch((void*)0x005E5F8E,
		"\x3E\x81\x3C\xE4\xFF\x5F\x5E\x00\x7F\x91\x3E\x81\x3C\xE4\x00\x00"
		"\x40\x00\x7C\x87\xC2\x04\x00"
	, 23);

// The locations of various code blocks we jump into:
#define RETN				0x005E5F14
#define FS_RETN				0x005E5F05
#define ADD_ESP_10_RETN		0x005E5F11
#define ADD_ESP_1C_RETN		0x005E5F0C
#define ADD_ESP_20_RETN		0x005E5F00
#define ADD_EAX_54_RETN		0x005E5EFB
#define DEREF_EAX_RETN		0x005E5EF7
#define MOV_ECX_FS_RETN		0x005E5F05
#define ADD_ESP_114_RETN	0x005E5EEF
#define RETN4				0x005E5F8E

#define PATCH_RETN_XOR_RETN(src)	Patching::Patch((void*)src, "\xEB\x02\x31\xC0", 4); \
								Patching::PatchUnconditionalJump(src + 4, RETN)

	// Now we patch the various functions to jump to the codecaves. Feel free to comment out any of these
	// lines if you need access to any of these functions. Bear in mind that if you can call it, so can
	// other DLLs. Lines that are commented out are purposely unpatched because they're used by this, or
	// other "safe" DLLs. Lines that are empty have not yet been patched. Feel free to build these yourself.
	Patching::PatchUnconditionalJump(0x00429A09, ADD_ESP_10_RETN);		// ZPostChannelChat
	Patching::PatchUnconditionalJump(0x004C4114, ADD_ESP_10_RETN);		// ZPostChannelRequestJoin
	Patching::PatchUnconditionalJump(0x004489E8, ADD_ESP_10_RETN);		// ZPostCreateMyChar
	Patching::PatchUnconditionalJump(0x0047C801, ADD_ESP_1C_RETN);		// ZPostDash
	Patching::PatchUnconditionalJump(0x004A0883, ADD_ESP_10_RETN);		// ZPostGameKill
	Patching::PatchUnconditionalJump(0x004298C8, ADD_ESP_10_RETN);		// ZPostPeerChat
	Patching::PatchUnconditionalJump(0x0045D8D3, ADD_ESP_10_RETN);		// ZPostQuestReadToNewSector
	Patching::PatchUnconditionalJump(0x0048AB4D, ADD_ESP_10_RETN);		// ZPostQuestRequestNPCDead
	Patching::PatchUnconditionalJump(0x004325A3, RETN);					// ZPostReload
	Patching::PatchUnconditionalJump(0x004CAD2E, ADD_ESP_10_RETN);		// ZPostRequestAgreedJoinClan
	Patching::PatchUnconditionalJump(0x00425068, ADD_ESP_10_RETN);		// ZPostRequestExpelClanMember
	Patching::PatchUnconditionalJump(0x004B286F, ADD_ESP_1C_RETN);		// ZPostRequestSpawn
	Patching::PatchUnconditionalJump(0x004B47C5, FS_RETN);				// ZPostRequestSpawnWorldItem
	Patching::PatchUnconditionalJump(0x004258E4, ADD_ESP_10_RETN);		// ZPostRequestStageJoin
	Patching::PatchUnconditionalJump(0x0047E019, ADD_ESP_20_RETN);		// ZPostShot
	Patching::PatchUnconditionalJump(0x00475EBF, ADD_ESP_1C_RETN);		// ZPostShotSp
	Patching::PatchUnconditionalJump(0x0047E1C7, ADD_ESP_1C_RETN);		// ZPostShotMelee
	Patching::PatchUnconditionalJump(0x0047DF27, RETN);					// ZPostSkill
	Patching::PatchUnconditionalJump(0x004180B9, ADD_ESP_10_RETN);		// ZPostStageChat
	Patching::PatchUnconditionalJump(0x0042AAB6, ADD_ESP_10_RETN);		// ZPostStageCreate
	Patching::PatchUnconditionalJump(0x004A06C4, ADD_ESP_10_RETN);		// ZPostStageEnterBattle
	Patching::PatchUnconditionalJump(0x00433584, ADD_ESP_10_RETN);		// ZPostStageLeaveBattle
	Patching::PatchUnconditionalJump(0x0045BF6B, RETN);					// ZPostStageSetting
	Patching::PatchUnconditionalJump(0x00448F90, ADD_ESP_10_RETN);		// ZPostStageStart
	Patching::PatchUnconditionalJump(0x004485E3, ADD_ESP_10_RETN);		// ZPostStageTeam
	Patching::PatchUnconditionalJump(0x00485E10, RETN4);				// SetHP
	Patching::Patch((void*)0x00485DB7, "\xEB\x57\x90" , 3);				// SetHP Part 2
	Patching::PatchUnconditionalJump(0x00485EE0, RETN4);				// SetAP
	Patching::Patch((void*)0x00485E87, "\xEB\x57\x90" , 3);				// SetAP Part 2
	Patching::PatchUnconditionalJump(0x004213F8, ADD_ESP_10_RETN);		// ZPostRequestCharInfoDetail
	Patching::PatchUnconditionalJump(0x0042A254, RETN);					// ZChatOutput
	Patching::PatchUnconditionalJump(0x004B3812, RETN4);				// ZRuleBerserker::BonusHealth
																		// ZRuleBerserker::PenaltyHealth
																		// ZCharacter::Destroy
																		// ZCharacter::InitRound
																		// ZPostChangeWeapon
	Patching::PatchUnconditionalJump(0x00432667, ADD_ESP_114_RETN);		// ZPostConnect
																		// ZPostChannelRequestJoinFromChannelName
																		// ZPostStageStart
																		// ZPostStageLeave
																		// ZPostStageState
																		// ZPostAccountCharList
																		// ZPostAccountCharInfo
	Patching::PatchUnconditionalJump(0x00433747, RETN);					// ZPostRequestBuyItem
																		// ZPostRequestSellItem
																		// ZPostRequestEquipItem
	Patching::PatchUnconditionalJump(0x00433DB7, RETN);					// ZPostRequestBuyQuestItem
	Patching::PatchUnconditionalJump(0x00433F30, RETN);					// ZPostRequestSellQuestItem
	Patching::PatchUnconditionalJump(0x00458060, RETN);					// ZPostRequestDropSacrificeItem
																		// ZModule_HPAP::SetHP
																		// ZModule_HPAP::SetAP
																		// ZModule_HPAP::InitStatus
	Patching::PatchUnconditionalJump(0x0049E87A, ADD_EAX_54_RETN);		// ZGetCharacterManager
	Patching::PatchUnconditionalJump(0x004ABD45, RETN);					// ZGetMeshMgr
	Patching::PatchUnconditionalJump(0x004245D5, RETN);					// ZPostWhere
	Patching::PatchUnconditionalJump(0x004ABD87, RETN);					// ZGetCombatInterface
	Patching::PatchUnconditionalJump(0x004970CA, DEREF_EAX_RETN);		// ZGetGameInterface
	PATCH_RETN_XOR_RETN(0x004ABE23);									// ZGetGameTypeManager
																		// ZGetCamera
	Patching::PatchUnconditionalJump(0x004ABD55, RETN);					// ZGetWeaponMeshMgr
	Patching::PatchUnconditionalJump(0x0049A885, RETN);					// ZGetConfiguration
	//Patching::PatchUnconditionalJump(0x004ABDCB, RETN);					// ZGetScreenEffectManager
	Patching::PatchUnconditionalJump(0x00465465, RETN);					// ZGetFlashBangEffect
																		// ZGetIsCashItem
																		// ZGetDirectInput
	Patching::PatchUnconditionalJump(0x0041BA29, ADD_ESP_10_RETN);		// ZGetInterfaceSkinPath
	Patching::PatchUnconditionalJump(0x00494415, RETN);					// ZGetSoundFMod

	Patching::Patch((void*)0x004ABDD7, "\x74\x14", 2);					// ZGetGame
	Patching::PatchUnconditionalJump(0x004ABDE8, RETN);					// ...
	Patching::Patch((void*)0x004ABDED, "\x31\xC0\xC3", 3);				// ...

																		// ZGetCharacterManager
	Patching::PatchUnconditionalJump(0x004BDFD5, RETN);					// ZGetWorldItemManager
																		// ZGetObjectManager
																		// ZGetCharacterViewList
	Patching::PatchUnconditionalJump(0x00497115, RETN);					// ZGetSoundEngine
	Patching::PatchUnconditionalJump(0x004970F5, RETN);					// ZGetFileSystem
																		// ZGetMZFileChecksum
	Patching::PatchUnconditionalJump(0x004ABDBB, RETN);					// ZGetEffectManager
	PATCH_RETN_XOR_RETN(0x004ABE03);									// ZGetQuest

																		// ZGetClockDistance
	Patching::PatchUnconditionalJump(0x004244D5, RETN);					// ZPostFriendAdd
	//Patching::PatchUnconditionalJump(0x0042AAB9, RETN);				// ZPostWhisper
	Patching::PatchUnconditionalJump(0x00497588, RETN);					// ZGetGameClient
	Patching::Patch((void*)0x00484533, "\xEB\x04\x90", 3);				// ZObjectManager::GetObjectA
	Patching::PatchUnconditionalJump(0x00484539, RETN4);				// ...

#undef RETN4
#undef ADD_ESP_114_RETN
#undef MOV_ECX_FS_RETN
#undef DEREF_EAX_RETN
#undef ADD_EAX_54_RETN
#undef ADD_ESP_20_RETN
#undef ADD_ESP_1C_RETN
#undef ADD_ESP_10_RETN
#undef FS_RETN
#undef RETN

	// Hacks aren't detected here. The patch does it itself.
	return NO_HACK_DETECTED;
}
