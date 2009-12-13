#include "HashManager.h"
#include "NOP.h"
#include "HTTP.h"

using namespace std;

HashManager& HashManager::Get()
{
	static HashManager instance;
	return instance;
}

void HashManager::ParseSingleLine(const string& lineToParse)
{
	try {
		string::size_type locationOfFirstMarker  = lineToParse.find("|");
		string::size_type locationOfSecondMarker = lineToParse.find("|", locationOfFirstMarker  + 1);
		string::size_type locationOfThirdMarker  = lineToParse.find("|", locationOfSecondMarker + 1);

		// Grabs the hashes based on the format of | <- n -> | <- n -> |
		memoryHashTree.insert	(lineToParse.substr(locationOfFirstMarker  + 1, locationOfSecondMarker - (locationOfFirstMarker + 1)));
		fileHashTree.insert		(lineToParse.substr(locationOfSecondMarker + 1, locationOfThirdMarker -  (locationOfSecondMarker + 1)));

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
