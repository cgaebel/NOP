#include "CProtectionManager.h"
#include "OnionAPI.h"
#include "defs.h"

const char* ModuleCount()
{
	DWORD moduleCount = EnumerateModules();

	if(moduleCount != CORRECT_INVALID_MODULE_COUNT)
	{
		std::ofstream out(INVALID_MODULE_COUNT_FILENAME);
		out << moduleCount << std::endl;

		return __FUNCTION__;
	}
	
	return NULL;
}