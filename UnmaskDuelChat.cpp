#include "Core.h"
#include "Patching.h"

INITIALIZER(UnmaskDuelChat, "Unmasking duel chat...")
{
	static const BYTE JMP = 0xEB;

	Patching::Patch((void*)0x004A9256, &JMP, sizeof(JMP));		// Normal chat.
	Patching::Patch((void*)0x004C7202, &JMP, sizeof(JMP));		// Whisper chat.
	Patching::Patch((void*)0x004CA582, &JMP, sizeof(JMP));		// Clan chat.
}
