#include "Core.h"
#include "HashManager.h"
#include "HMD6.h"
#include "NOP.h"

static std::string GetFileHash()
{
	static const char* protectedFiles[] = { FILE_CHECKSUM_PROTECTED_FILES };

	IHash* hashContext = NULL;

	try {
		hashContext = new HMD6;
	} catch(...) {
		OnFailure("Could not allocate the file hash. Out of memory?");
	}

	char dstBuf[1024] = { 0 };

	for(size_t i = 0; i < _countof(protectedFiles); ++i)
	{
		FILE* currentFile = NULL;
		size_t bytesRead;

		if(fopen_s(&currentFile, protectedFiles[i], "rb") || (currentFile == NULL))
			continue;

		LogInformation((std::string("Current file: ") + protectedFiles[i]).c_str());

		while((bytesRead = fread_s(dstBuf, _countof(dstBuf), sizeof(dstBuf[0]), _countof(dstBuf), currentFile)) > 0)
			hashContext->Update((const BYTE*)dstBuf, bytesRead);

		fclose(currentFile);
	}

	hashContext->Finalize();
	auto clientHash = hashContext->GetHash();
	delete hashContext;

	return clientHash;
}

#ifdef _DEBUG
	IGNORE_MODULE(FileHash);
#endif

PASSIVE_PROTECTION(FileHash, "Checking file integrity...")
{
	auto hash = GetFileHash();
	bool invalid = !(HashManager::Get().IsValidFileHash(hash));

	if(invalid)
		LogInformation(hash.c_str());

	return (invalid) ? HACK_DETECTED : NO_HACK_DETECTED;
}
