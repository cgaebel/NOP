#include "Core.h"
#include "HashManager.h"
#include "HMD6.h"
#include "NOP.h"

static std::string GetFileHash()
{
	static const char* protectedFiles[] = { FILE_CHECKSUM_PROTECTED_FILES };

	std::tr1::shared_ptr<IHash> hashContext(new HMD6);

	char dstBuf[0x1000] = { 0 };
	std::ifstream currentFile;

	for(size_t i = 0; i < _countof(protectedFiles); ++i)
	{
		currentFile.open(protectedFiles[i], std::ios_base::in | std::ios_base::binary);

		if(currentFile.bad())
		{
			LogInformation((std::string("Failed to open file: ") + protectedFiles[i]).c_str());
			continue;
		}

		if(currentFile == NULL)
		{
			LogInformation((std::string("Failed to open file: ") + protectedFiles[i]).c_str());
			continue;
		}

		LogInformation((std::string("Current file: ") + protectedFiles[i]).c_str());

		do {
			currentFile.read(dstBuf, _countof(dstBuf));
			hashContext->Update((const BYTE*)dstBuf, _countof(dstBuf));
		} while(currentFile);

		hashContext->Update((const BYTE*)dstBuf, currentFile.gcount());

		currentFile.close();
	}

	hashContext->Finalize();

	return hashContext->GetHash();
}

PASSIVE_PROTECTION(FileHash, "Checking file integrity...")
{
	auto hash = GetFileHash();
	bool invalid = !(HashManager::Get().IsValidFileHash(hash));

	if(invalid)
		LogInformation(hash.c_str());

	return (invalid) ? HACK_DETECTED : NO_HACK_DETECTED;
}
