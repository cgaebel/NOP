#include "HashManager.h"
#include "NOP.h"
#include "HTTP.h"

using namespace std;

HashManager& HashManager::Get()
{
	static HashManager instance;
	return instance;
}

// Grabs the hashes based on the format of | <- n -> | <- n -> |
void HashManager::ParseSingleLine(const string& lineToParse)
{
	try {
		std::string::size_type markers[3];
			
		markers[0] = lineToParse.find("|");
		markers[1] = lineToParse.find("|", markers[0] + 1);
		markers[2] = lineToParse.find("|", markers[1] + 1);

		for(int i = 0; i < _countof(markers); ++i)
			if(markers[i] == string::npos)
				OnFailure("Invalid hash format.");

		std::string hashes[_countof(markers) - 1];

		for(int i = 0; i < _countof(hashes); ++i)
			hashes[i] = lineToParse.substr(markers[i] + 1, markers[i + 1] - (markers[i] + 1));

		memoryHashTree.insert(hashes[0]);
		fileHashTree.insert(hashes[1]);

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
