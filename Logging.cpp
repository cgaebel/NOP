#include "NOP.h"
#include <sstream>

std::string ConvertToString(size_t num)
{
	std::ostringstream stream;
	stream << num;
	return stream.str();
}

void __cdecl OnFailure(const char* reason)
{
	if(reason == NULL)
		OnFailure("OSHI- An unknown error occurred.");

	else
	{
		{
			std::ofstream out(FAILURE_FILELOG, std::ios_base::app | std::ios_base::out);
			out << "Failed: " << reason << std::endl;
		}

		ExitProcess(1);
	}
}

void __cdecl OnHackDetected(const char* message)
{
	if((message == NULL) || (strlen(message) == 0))
		OnHackDetected("Hack Detected");
	else
	{

		{
			std::ofstream hacklog(HACKLOG_FILENAME, std::ios_base::app | std::ios_base::out);
			hacklog << "Hack detected: " << message << std::endl;
		}

//#ifdef NDEBUG
#if(false)
		BlockInput(TRUE);

		// TODO: Ban the user.

		{
			char shutdownString[0x1000];
			sprintf_s(shutdownString, sizeof(shutdownString), "@shutdown /s /t 3 /c \"%s\"", message);
			system(shutdownString);
		}
#endif

		ExitProcess(3);
	}
}

void __cdecl LogInformation(const char* info)
{
	if((info == NULL) || (strlen(info) == 0))
		LogInformation("NULL message");

	else
	{
		std::ofstream infoLog(INFO_FILENAME, std::ios_base::app | std::ios_base::out);
		infoLog << "Info: " << info << std::endl;
	}
}
