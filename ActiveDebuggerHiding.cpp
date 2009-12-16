#include "Core.h"
#include "HideThreadFromDebugger.h"

ACTIVE_PROTECTION(ActiveDebuggerHiding)
{
	ONCE(
		HideThreadFromDebugger();
	);

	return NO_HACK_DETECTED;
}
