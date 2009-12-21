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

	ON THE CLIENT:
	@ 0x0052F308	(65 bytes)
	B0 90 5E 88 41 10 88 41 11 88 41 12 88 41 13 88
	41 14 88 41 15 88 41 16 88 41 17 88 41 18 88 41
	19 88 41 1A 88 41 1B 88 41 1C 88 41 1D 88 41 1E
	88 41 1F 90 90 90 90 90 90 90 90 90 90 90 90 90
	90

	ON THE SERVER:
	@ 0x004534B8	(65 bytes)
	B0 90 5E 88 41 10 88 41 11 88 41 12 88 41 13 88
	41 14 88 41 15 88 41 16 88 41 17 88 41 18 88 41
	19 88 41 1A 88 41 1B 88 41 1C 88 41 1D 88 41 1E
	88 41 1F 90 90 90 90 90 90 90 90 90 90 90 90 90
	90
*/

INITIALIZER(ProtocolUpdate, "Updating GPP...")
{
	Patching::Patch((void*)0x0052F308,
		"\xB0" GUNZ_PROTOCOL_KEY "\x5E\x88\x41\x10\x88\x41\x11\x88\x41\x12\x88\x41\x13\x88"
		"\x41\x14\x88\x41\x15\x88\x41\x16\x88\x41\x17\x88\x41\x18\x88\x41"
		"\x19\x88\x41\x1A\x88\x41\x1B\x88\x41\x1C\x88\x41\x1D\x88\x41\x1E"
		"\x88\x41\x1F\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
		"\x90"
		, 4 * 16 + 1);
}
