#include "Core.h"
#include "Patching.h"
#include "config.h"

/*
	By the way, GPP = Gunz Packet Protocol :)

	CLIENT: 0052F30B
	SERVER: 004534BB

	On the client, the function is called from the
	following locations:
	0x005098E9		0x0050A153		0x00518902
	Seems pretty thunkable, no?

	FGUNZ KEY:
	0x37, 0x53, 0x04, 0x5D, 0x2E, 0x43, 0x38, 0xC9,
	0x49, 0x50, 0x05, 0x13, 0x28, 0xA4, 0x4D, 0x05

	NEW KEY:
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90

	Please note that the second byte in the
	disasesmbly is the "key".
   
        ASSEMBLY CLIENT:
	0052F308   B0 90            MOV AL,90
	0052F30A   5E               POP ESI
	0052F30B   52               PUSH EDX
	0052F30C   BA 0F000000      MOV EDX,0F
	0052F311   83C2 01          ADD EDX,1
	0052F314   880411           MOV BYTE PTR DS:[ECX+EDX],AL
	0052F317   83FA 1F          CMP EDX,1F
	0052F31A  ^75 F5            JNZ SHORT GunzA.0052F311
	0052F31C   5A               POP EDX
	0052F31D   8B4C24 18        MOV ECX,DWORD PTR SS:[ESP+18]
	0052F321   5D               POP EBP
	0052F322   5B               POP EBX
	0052F323   E8 061E0400      CALL GunzA.0057112E
	0052F328   83C4 14          ADD ESP,14
	0052F32B   C3               RETN
 
	ON THE CLIENT:
	@ 0x0052F308	(65 bytes)
	B0 90 5E 52 BA 0F 00 00 00 83 C2 01 88 04 11 83 
	FA 1F 75 F5 5A 8B 4C 24 18 5D 5B E8 06 1E 04 00
	83 C4 14 C3 CC CC CC CC CC CC CC CC CC CC CC CC 
	CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
	CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC

	ON THE SERVER:
	@ 0x004534B8	(65 bytes)
	B0 90 5E 52 BA 0F 00 00 00 83 C2 01 88 04 11 83 
        FA 1F 75 F5 5A 8B 4C 24 18 5D 5B E8 1F D8 05 00
        83 C4 14 C3 CC CC CC CC CC CC CC CC CC CC CC CC
        CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
        CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC

*/

INITIALIZER(ProtocolUpdate, "Updating GPP...")
{
	Patching::Patch((void*)0x0052F308,
		"\xB0" GUNZ_PROTOCOL_KEY "\x5E\x52\xBA\x0F\x00\x00\x00\x83\xC2\x01\x88\x04\x11\x83"
                "\xFA\x1F\x75\xF5\x5A\x8B\x4C\x24\x18\x5D\x5B\xE8\x06\x1E\x04\x00"
                "\x83\xC4\x14\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"
                "\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"
                "\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"
		, 4 * 16 + 1);
}
