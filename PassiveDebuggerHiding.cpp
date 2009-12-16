#include "Core.h"
#include "HideThreadFromDebugger.h"

PASSIVE_PROTECTION(PassiveDebuggerHiding, "Hiding the main thread...")
{
	HideThreadFromDebugger();

	return NO_HACK_DETECTED;
}
