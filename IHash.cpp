#include "IHash.h"

std::string IHash::GetHash(const BYTE *data, const size_t len)
{
	Update(data, len);
	Finalize();
	return GetHash();
}
