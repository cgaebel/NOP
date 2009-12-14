#include "Core.h"
#include "ListSingletons.h"
#include "NOP.h"

static bool IsIgnoredModule(const Module* module)
{
	std::list<const char*>& ignoreList(GetIgnoreList());

	std::list<const char*>::iterator begin = ignoreList.begin();
	std::list<const char*>::iterator end = ignoreList.end();

	const char* moduleName = module->moduleName;

	for(std::list<const char*>::iterator i = begin; i != end; ++i)
		if(strcmp(*i, moduleName) == 0)
			return true;

	return false;
}

void Initialize()
{
	LogInformation("Initializing...");
	std::list<InitializationModule*> initList = GetInitializationList();

	for(std::list<InitializationModule*>::iterator i = initList.begin(); i != initList.end(); ++i)
	{
		InitializationModule* currentModule = *i;

		if(IsIgnoredModule(currentModule))
			continue;

		LogInformation(currentModule->logMessage);

		currentModule->Run();
	}

	LogInformation((ConvertToString(initList.size()) + " initialization module(s) loaded.").c_str());
}

void RunPassiveProtection()
{
	LogInformation("Beginning passive protection...");

	std::list<PassiveProtectionModule*> protectionList = GetPassiveProtectionList();

	for(std::list<PassiveProtectionModule*>::iterator i = protectionList.begin(); i != protectionList.end(); ++i)
	{
		PassiveProtectionModule* currentModule = *i;

		if(IsIgnoredModule(currentModule))
			continue;

		LogInformation(currentModule->logMessage);

		if(currentModule->Run())
			OnHackDetected(currentModule->moduleName);
	}

	LogInformation((ConvertToString(protectionList.size()) + " passive protection module(s) loaded.").c_str());
}

int __stdcall BeginActiveProtection_Proxy()
{
	LogInformation("Beginning active protection...");
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

	std::list<ActiveProtectionModule*> activeProtectionList = GetActiveProtectionList();

	LogInformation((ConvertToString(activeProtectionList.size()) + " active protection module(s) loaded.").c_str());

	for(;;)
	{
		for(std::list<ActiveProtectionModule*>::iterator i = activeProtectionList.begin(); i != activeProtectionList.end(); ++i)
		{
			ActiveProtectionModule* currentModule = *i;

			if(IsIgnoredModule(currentModule))
				continue;

			if(currentModule->Run())
				OnHackDetected(currentModule->moduleName);

			Sleep(20);
		}

		Sleep(2000);
	}
}

void BeginActiveProtection()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BeginActiveProtection_Proxy, NULL, NULL, NULL);
}

void StartAntiHack()
{
	Initialize();
	RunPassiveProtection();
	BeginActiveProtection();
}
