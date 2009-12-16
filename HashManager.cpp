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
		auto locationOfFirstMarker  = lineToParse.find("|");
		auto locationOfSecondMarker = lineToParse.find("|", locationOfFirstMarker  + 1);
		auto locationOfThirdMarker  = lineToParse.find("|", locationOfSecondMarker + 1);

		if((locationOfFirstMarker == string::npos) || (locationOfSecondMarker == string::npos) || (locationOfThirdMarker == string::npos))
			OnFailure("Invalid hash format.");

		// Grabs the hashes based on the format of | <- n -> | <- n -> |
		auto memoryHash = lineToParse.substr(locationOfFirstMarker  + 1, locationOfSecondMarker - (locationOfFirstMarker + 1));
		auto fileHash = lineToParse.substr(locationOfSecondMarker + 1, locationOfThirdMarker -  (locationOfSecondMarker + 1));

		memoryHashTree.insert(memoryHash);
		fileHashTree.insert(fileHash);

	} catch(exception& ex) {
		OnFailure(ex.what());
	} catch(...) {
		OnFailure("Could not grab the hash.");
	}
}

void HashManager::InitHashTree()
{
	auto hashInfoFile(HTTPGet("bgfx.net/wowus/hashinfo.v2"));

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
