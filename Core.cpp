#include "Core.h"
#include "ListSingletons.h"
#include "NOP.h"
#include "defs.h"

// Variables that sanity check the developer. We are all our own worst enemies!
static bool initialized = false;
static bool passived = true;

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

template <class ModuleClass>
static void RemoveIgnoredModules(std::list<ModuleClass*>& toRemoveFrom)
{
	std::list<const char*>& ignoreList(GetIgnoreList());

	std::list<ModuleClass*> toRemove;

	for(std::list<ModuleClass*>::iterator i = toRemoveFrom.begin(); i != toRemoveFrom.end();++i)
		for(std::list<const char*>::iterator j = ignoreList.begin(); j != ignoreList.end(); ++j)
			if(strcmp(*j, (**i).moduleName) == 0)
				toRemove.push_back(*i);

	for(std::list<ModuleClass*>::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
		toRemoveFrom.remove(*i);
}

void Initialize()
{
	LogInformation("Initializing...");
	std::list<InitializationModule*> initList = GetInitializationList();

	RemoveIgnoredModules(initList);

	for(std::list<InitializationModule*>::iterator i = initList.begin(); i != initList.end(); ++i)
	{
		InitializationModule* currentModule = *i;

		LogInformation(currentModule->logMessage);

		currentModule->Run();
	}

	LogInformation((ConvertToString(initList.size()) + " initialization module(s) loaded.").c_str());

	initialized = true;
}

void RunPassiveProtection()
{
	if(!initialized)
		OnFailure("Wtf we're starting protection without initialization? Yarite.");

	LogInformation("Beginning passive protection...");

	std::list<PassiveProtectionModule*> protectionList = GetPassiveProtectionList();

	RemoveIgnoredModules(protectionList);

	for(std::list<PassiveProtectionModule*>::iterator i = protectionList.begin(); i != protectionList.end(); ++i)
	{
		PassiveProtectionModule* currentModule = *i;

		LogInformation(currentModule->logMessage);

		if(currentModule->Run())
			OnHackDetected(currentModule->moduleName);
	}

	LogInformation((ConvertToString(protectionList.size()) + " passive protection module(s) loaded.").c_str());

	passived = true;
}

int __stdcall BeginActiveProtection_Proxy()
{
	if(!initialized)
		OnFailure("Why, exactly, are we starting the active protection BEFORE initialization?");
	else if(!passived)
		OnFailure("Oh god. You gotta do the passive protection BEFORE active protection, bro.");

	LogInformation("Beginning active protection...");
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

	std::list<ActiveProtectionModule*> activeProtectionList = GetActiveProtectionList();

	RemoveIgnoredModules(activeProtectionList);

	LogInformation((ConvertToString(activeProtectionList.size()) + " active protection module(s) loaded.").c_str());

	for(;;)
	{
		for(std::list<ActiveProtectionModule*>::iterator i = activeProtectionList.begin(); i != activeProtectionList.end(); ++i)
		{
			ActiveProtectionModule* currentModule = *i;

			if(currentModule->Run())
				OnHackDetected(currentModule->moduleName);

			Sleep(INNER_CORE_LOOP_DELAY);
		}

		Sleep(OUTER_CORE_LOOP_DELAY);
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
