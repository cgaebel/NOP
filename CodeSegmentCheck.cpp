#include "Core.h"
#include "NOP.h"
#include "HashManager.h"
#include "defs.h"
#include "CCRC32.h"

static std::string MemoryChecksum()
{
	static CCRC32 runningHash;
	return runningHash.GetHashAndReset(CODE_SEGMENT_BEGIN, CODE_SEGMENT_END - CODE_SEGMENT_BEGIN);
}

#ifdef _DEBUG
	IGNORE_MODULE(CodeSegmentCheck);
#endif

ACTIVE_PROTECTION(CodeSegmentCheck)
{
	std::string checksum;

	if(HashManager::Get().IsValidMemoryHash(checksum = MemoryChecksum()))
		return NO_HACK_DETECTED;

	else
	{
		LogInformation(checksum.c_str());
		return HACK_DETECTED;
	}
}
