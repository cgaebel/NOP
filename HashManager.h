#pragma once
#include "Singleton.h"

class HashManager : public Singleton<HashManager>
{
	friend class Singleton<HashManager>;

private:
	inline HashManager() { InitHashTree(); }
	void InitHashTree();

	std::set<std::string> fileHashTree;
	std::set<std::string> memoryHashTree;

	void ParseSingleLine(const std::string& lineToParse);
	static void RemoveBlankHashesFromSingleTree(std::set<std::string>& tree);
	void RemoveAllBlankHashes();

public:
	bool IsValidFileHash(std::string fileHash);
	bool IsValidMemoryHash(std::string memoryHash);
};
