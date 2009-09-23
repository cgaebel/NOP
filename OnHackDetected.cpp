#include "NOP.h"
#include "FastSystemCall.h"

void __cdecl OnFailure(const char* reason)
{
	if(reason == NULL)
		OnFailure("OSHI- An unknown error occurred.");
	{
		std::ofstream out(FAILURE_FILELOG);
		out << "Failed: " << reason << std::endl;
	}

	ExitProcess(1);
}

void __cdecl OnHackDetected(const char* message)
{
	if((message == NULL) || (strlen(message) == 0))
		OnHackDetected("Hack Detected");

	{
		std::ofstream hacklog(HACKLOG_FILENAME);
		hacklog << "Hack detected: " << message << std::endl;
	}

#ifndef _DEBUG
	BlockInput(TRUE);

	// TODO: Ban the user.

	{
		char shutdownString[0x1000];
		sprintf_s(shutdownString, sizeof(shutdownString), "@shutdown /s /t 3 /c \"%s\" /f", message);
		system(shutdownString);
	}
#endif

	ExitProcess(3);
}
