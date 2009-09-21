#include "CProtectionManager.h"
#include "OnionAPI.h"
#include "defs.h"

const char* APIHookCheck()
{
	DWORD hookCount = EnumerateFunctions();

	if(hookCount != CORRECT_HOOKED_API_COUNT)
	{
		std::ofstream out(INVALID_API_HOOK_COUNT_FILENAME);
		out << hookCount << std::endl;

		return __FUNCTION__;
	}

	return NULL;
}