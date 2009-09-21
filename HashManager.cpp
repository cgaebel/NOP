#include "HashManager.h"
#include "NOP.h"
#include "HTTP.h"

using namespace std;

static set<string> fileHashTree;
static set<string> memoryHashTree;

static void ParseSingleLine(const std::string& lineToParse)
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

void InitHashTree()
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

bool IsValidFileHash(std::string fileHash)
{
	return (fileHashTree.find(fileHash) != fileHashTree.end());
}

bool IsValidMemoryHash(std::string memoryHash)
{
#ifdef _DEBUG
	return true;
#endif
	return (memoryHashTree.find(memoryHash) != memoryHashTree.end());
}