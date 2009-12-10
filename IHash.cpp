#include "IHash.h"

std::string IHash::GetHashAndReset(const BYTE *data, const size_t len)
{
	Update(data, len);
	Finalize();

	std::string retVal = GetHash();
	Reset();
	return retVal;
}
