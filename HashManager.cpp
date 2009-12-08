#include "HashManager.h"
#include "NOP.h"
#include "HTTP.h"

using namespace std;

HashManager& HashManager::Get()
{
	static HashManager instance;
	return instance;
}

void HashManager::ParseSingleLine(const std::string& lineToParse)
{
	try {
		// Grabs the hashes based on the format of | <- 128 -> | <- 128 -> |
		memoryHashTree.insert(lineToParse.substr(1, 128));
		fileHashTree.insert(lineToParse.substr(130, 128));
	} catch(exception& ex) {
		OnFailure(ex.what());
	} catch(...) {
		OnFailure("Could not grab the hash.");
	}
}

void HashManager::InitHashTree()
{
	std::string hashInfoFile(HTTPGet("bgfx.net/wowus/hashinfo"));

	std::string currentLine;

	if(hashInfoFile != "failed")
	{
		for(size_t i = 0; i < hashInfoFile.size(); ++i)
		{
			if(hashInfoFile[i] == '\n')
			{
				ParseSingleLine(currentLine);
				currentLine.clear();
			}
			else
			{
				currentLine += hashInfoFile[i];
			}
		}

		ParseSingleLine(currentLine);
	}
}

bool HashManager::IsValidFileHash(std::string fileHash)
{
	return (fileHashTree.find(fileHash) != fileHashTree.end());
}

bool HashManager::IsValidMemoryHash(std::string memoryHash)
{
	return (memoryHashTree.find(memoryHash) != memoryHashTree.end());
}
