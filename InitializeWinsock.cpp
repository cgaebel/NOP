#include "Core.h"
#include "NOP.h"

INITIALIZER(Winsock, "Initializing Winsock...")
{
	static WSADATA wsd;

	if(WSAStartup(0x0202, &wsd))
		OnFailure("Could not initialize winsock.");
}
