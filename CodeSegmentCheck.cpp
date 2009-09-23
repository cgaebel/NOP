#include "CProtectionManager.h"
#include "NOP.h"
#include "HashManager.h"
#include "defs.h"

static std::string MemoryChecksum()
{
	IHash* runningHash = NULL;

	try {
		runningHash = new HMD6;
	} catch(...) {	// Out of memory.
		OnFailure("Could not allocate enough memory for the hash.");
	}

	std::string hash(runningHash->GetHash(CODE_SEGMENT_BEGIN, CODE_SEGMENT_END - CODE_SEGMENT_BEGIN));

	delete runningHash;
	return hash;
}

const char* CodeSegmentCheck()
{
	std::string checksum = MemoryChecksum();
	bool invalid = !IsValidMemoryHash(checksum);

	if(invalid)
	{
		std::ofstream out(MEMORY_HASH_FAILED_FILENAME);
		out << checksum << std::endl;

		ExitProcess(0);
	}

	return invalid ? __FUNCTION__ : NULL;
}
