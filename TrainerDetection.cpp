#include "Core.h"
#include "NOP.h"
#include "defs.h"

static BOOL EnumWindowsHandler(HWND hwnd, bool* retVal)
{
	char title[0x1000];
	GetWindowText(hwnd, title, _countof(title));

	static const char* titleBlacklist[] = {
		TRAINER_WINDOW_TITLE_BLACKLIST
	};

	// If the blacklisted name is in the title, diediedie.
	for(size_t i = 0; i < _countof(titleBlacklist); ++i)
	{
		if(StrStrI(title, titleBlacklist[i]))
		{
			*retVal = true;
			return FALSE;
		}
	}

	return TRUE;
}

ACTIVE_PROTECTION(TrainerDetection)
{
	bool hackDetected = false;
	EnumWindows((WNDENUMPROC)EnumWindowsHandler, (LPARAM)(&hackDetected));
	return (hackDetected) ? HACK_DETECTED : NO_HACK_DETECTED;
}
