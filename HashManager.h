#pragma once

class HashManager
{
public:
	static HashManager* Get();
private:
	inline HashManager() { hasValidHashTree = false; }

	bool hasValidHashTree;

	std::set<std::string> fileHashTree;
	std::set<std::string> memoryHashTree;

	void ParseSingleLine(const std::string& lineToParse);

public:
	void InitHashTree();

	bool IsValidFileHash(std::string fileHash);
	bool IsValidMemoryHash(std::string memoryHash);
};
