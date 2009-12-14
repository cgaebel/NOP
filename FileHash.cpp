#include "Core.h"
#include "HashManager.h"
#include "CCRC32.h"

static std::string GetFileHash()
{
	static const char* protectedFiles[] = { FILE_CHECKSUM_PROTECTED_FILES };

	IHash* hashContext = NULL;

	try {
		hashContext = new CCRC32;
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

		while((bytesRead = fread_s(dstBuf, _countof(dstBuf), sizeof(dstBuf[0]), _countof(dstBuf), currentFile)) > 0)
			hashContext->Update((const BYTE*)dstBuf, bytesRead);

		fclose(currentFile);
	}

	hashContext->Finalize();
	std::string clientHash = hashContext->GetHash();
	delete hashContext;

	return clientHash;
}

PASSIVE_PROTECTION(FileHash, "Checking file integrity...")
{
	std::string hash = GetFileHash();
	bool invalid = !(HashManager::Get().IsValidFileHash(hash));

	if(invalid)
		LogInformation(hash.c_str());

#ifdef _DEBUG
		return NO_HACK_DETECTED;
#else
		return (invalid) ? HACK_DETECTED : NO_HACK_DETECTED;
#endif	
}
