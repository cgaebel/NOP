#include "Core.h"
#include "ListSingletons.h"
#include "NOP.h"

void Initialize()
{
	LogInformation("Initializing...");
	std::list<InitializationModule*> initList = GetInitializationList();

	for(std::list<InitializationModule*>::iterator i = initList.begin(); i != initList.end(); ++i)
	{
		InitializationModule* currentModule = *i;

		LogInformation(currentModule->logMessage);

		currentModule->Run();
	}
}

void RunPassiveProtection()
{
	LogInformation("Beginning passive protection...");

	std::list<PassiveProtectionModule*> protectionList = GetPassiveProtectionList();

	for(std::list<PassiveProtectionModule*>::iterator i = protectionList.begin(); i != protectionList.end(); ++i)
	{
		PassiveProtectionModule* currentModule = *i;

		LogInformation(currentModule->logMessage);

		if(currentModule->Run())
			OnHackDetected(currentModule->moduleName);
	}
}

int __stdcall BeginActiveProtection_Proxy()
{
	LogInformation("Beginning active protection...");
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

	std::list<ActiveProtectionModule*> activeProtectionList = GetActiveProtectionList();

	for(;;)
	{
		for(std::list<ActiveProtectionModule*>::iterator i = activeProtectionList.begin(); i != activeProtectionList.end(); ++i)
		{
			ActiveProtectionModule* currentModule = *i;

			LogInformation(currentModule->logMessage);

			const char* retVal = currentModule->Run();

			if(retVal != NULL)
				OnHackDetected(retVal);

			Sleep(20);
		}

		Sleep(2000);
	}
}

void BeginActiveProtection()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BeginActiveProtection_Proxy, NULL, NULL, NULL);
}
