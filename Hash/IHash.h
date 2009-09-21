#pragma once
#include <windows.h>
#include <string>

class IHash abstract
{
public:
	// Incorporates data string into hash computation.
	virtual void Update(const BYTE* data, const size_t len) = NULL;

	// Gets the final hash from all the Update()'s.
	virtual void Finalize() = NULL;

	// Should only be called until after Finalize().
	virtual std::string GetHash() = NULL;

	// Gets the hash from the data supplied all at once.
	virtual std::string GetHash(const BYTE* data, const size_t len);

	// Clears the hash. To be reused later.
	virtual void Reset() = NULL;
};
