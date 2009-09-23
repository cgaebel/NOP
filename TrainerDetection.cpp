#include "CProtectionManager.h"
#include "NOP.h"
#include "defs.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Handles the EnumWindows callback to stop any trainers with "ZxPwd" in the title.
/// 
/// @author	Clark Gaebel
/// @date	8/10/2009
/// 
/// @param	hwnd	Handle to the window.
/// @param	retVal	The return value.
/// 
/// @return			true to keep searching, false to stop.
////////////////////////////////////////////////////////////////////////////////////////////////////
static BOOL EnumWindowsHandler(HWND hwnd, bool* retVal)
{
	char title[0x1000];
	GetWindowText(hwnd, title, _countof(title));

	// Add any more blacklisted titles here:
	const char* titleBlacklist[] = {
		TRAINER_WINDOW_TITLE_BLACKLIST
	};

	// If the blacklisted name is in the title, diediedie.
	for(size_t i = 0; i < _countof(titleBlacklist); ++i)
	{
		if(strstr(title, titleBlacklist[i]))
		{
			*retVal = true;
			return FALSE;
		}
	}

	return TRUE;
}

const char* TrainerDetection()
{
	bool hackDetected = false;
	EnumWindows((WNDENUMPROC)EnumWindowsHandler, (LPARAM)(&hackDetected));
	return (hackDetected) ? __FUNCTION__ : NULL;
}
