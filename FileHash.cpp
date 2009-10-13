#include "CProtectionManager.h"
#include "HashManager.h"
#include "defs.h"

static std::string GetFileHash()
{
	static const char* protectedFiles[] = { FILE_CHECKSUM_PROTECTED_FILES };

	IHash* hashContext = NULL;

	try {
		hashContext = new HMD6;
	} catch(...) {
		return "";
	}

	char dstBuf[md6_w*md6_w] = { 0 };

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
	std:: string clientHash = hashContext->GetHash();
	delete hashContext;

	return clientHash;
}

const char* FileHash()
{
	std::string hash = GetFileHash();
	bool invalid = !(HashManager::Get()->IsValidFileHash(hash));

	if(invalid)
	{
		std::ofstream out(FILE_HASH_FAILED_FILENAME);
		out << hash << std::endl;
	}

	return (invalid) ? __FUNCTION__ : NULL;
}
