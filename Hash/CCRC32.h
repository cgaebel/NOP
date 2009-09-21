#pragma once
#include "IHash.h"

class CCRC32 : public IHash
{
private:
	static const DWORD polyTable[256];

	DWORD crc;
public:
	CCRC32();
	void Update(const BYTE* data, const size_t len);
	void Finalize();

	std::string GetHash();
};