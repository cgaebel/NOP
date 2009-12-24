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
   
	CPU Disasm
	Address      Hex dump       Command                                  Comments
	0052F308      B0 90         MOV AL,90
	0052F30A      5E            POP ESI
	0052F30B      52            PUSH EDX
	0052F30C      BA 0F000000   MOV EDX,0F
	0052F311      42            INC EDX
	0052F312      88040A        MOV BYTE PTR DS:[ECX+EDX],AL
	0052F315      83FA 1F       CMP EDX,1F
	0052F318    ^ 75 F7         JNE SHORT 0052F311
	0052F31A      5A            POP EDX
	0052F31B      8B4CE4 18     MOV ECX,DWORD PTR SS:[ESP+18]
	0052F31F      5D            POP EBP
	0052F320      5B            POP EBX
	0052F321      E8 081E0400   CALL 0057112E
	0052F326      83C4 14       ADD ESP,14
	0052F329      C3            RETN
 
	ON THE CLIENT:
	@ 0x0052F308	(65 bytes)
	B0 90 5E 52 BA 0F 00 00 00 42 88 04 0A 83 FA 1F
	75 F7 5A 8B 4C E4 18 5D 5B E8 08 1E 04 00 83 C4
	14 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC
	CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
	CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC

	ON THE SERVER:
	@ 0x004534B8	(65 bytes)
	B0 90 5E 52 BA 0F 00 00 00 42 88 04 0A 83 FA 1F
	75 F7 5A 8B 4C E4 18 5D 5B E8 08 1E 04 00 83 C4
	14 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC
	CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
	CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
*/

INITIALIZER(ProtocolUpdate, "Updating GPP...")
{
	Patching::Patch((void*)0x0052F308,
		"\xB0" GUNZ_PROTOCOL_KEY "\x5E\x52\xBA\x0F\x00\x00\x00\x42\x88\x04\x0A\x83\xFA\x1F"
		"\x75\xF7\x5A\x8B\x4C\xE4\x18\x5D\x5B\xE8\x08\x1E\x04\x00\x83\xC4"
		"\x14\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"
		"\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"
		"\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"
		, 5 * 16);
}
