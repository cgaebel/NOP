#include "Core.h"
#include "ListSingletons.h"
#include "NOP.h"
#include "defs.h"

// Variables that sanity check the developer. We are all our own worst enemies!
static auto initialized = false;
static auto passived = true;

static bool IsIgnoredModule(const std::tr1::shared_ptr<Module> module)
{
	auto& ignoreList(GetIgnoreList());

	const auto moduleName = module->moduleName;
	bool ignored = false;

	std::for_each(ignoreList.begin(), ignoreList.end(),
		[moduleName, &ignored]
		(const char* currentName)
		{
			if(strcmp(currentName, moduleName) == 0)
				ignored = true;
		}
	);

	return ignored;
}

template <class ModuleClass>
static void RemoveIgnoredModules(std::list<std::tr1::shared_ptr<ModuleClass> >& toRemoveFrom)
{
	auto& ignoreList(GetIgnoreList());

	std::list<std::tr1::shared_ptr<ModuleClass> > toRemove;

	std::for_each(toRemoveFrom.begin(), toRemoveFrom.end(),
		[&]
		(std::tr1::shared_ptr<ModuleClass> currentModule)
		{
			if(IsIgnoredModule(currentModule))
				toRemove.push_back(currentModule);
		}
	);

	std::for_each(toRemove.begin(), toRemove.end(),
		[&]
		(std::tr1::shared_ptr<ModuleClass> removeTarget)
		{
			toRemoveFrom.remove(removeTarget);
		}
	);
}

void Initialize()
{
	LogInformation("Initializing...");
	auto& initList = GetInitializationList();

	RemoveIgnoredModules(initList);

	for(auto i = initList.begin(); i != initList.end(); ++i)
	{
		std::tr1::shared_ptr<InitializationModule> currentModule = *i;

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

	auto& protectionList = GetPassiveProtectionList();

	RemoveIgnoredModules(protectionList);

	for(auto i = protectionList.begin(); i != protectionList.end(); ++i)
	{
		std::tr1::shared_ptr<PassiveProtectionModule> currentModule = *i;

		LogInformation(currentModule->logMessage);

		if(currentModule->Run())
			OnHackDetected(currentModule->moduleName);
	}

	LogInformation((ConvertToString(protectionList.size()) + " passive protection module(s) loaded.").c_str());

	passived = true;
}

void BeginActiveProtection()
{
	Utilities::CreateThread(
		[]()
		{
			if(!initialized)
				OnFailure("Why, exactly, are we starting the active protection BEFORE initialization?");
			else if(!passived)
				OnFailure("Oh god. You gotta do the passive protection BEFORE active protection, bro.");

			LogInformation("Beginning active protection...");
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

			auto& activeProtectionList = GetActiveProtectionList();

			RemoveIgnoredModules(activeProtectionList);

			LogInformation((ConvertToString(activeProtectionList.size()) + " active protection module(s) loaded.").c_str());

			for(;;)
			{
				for(auto i = activeProtectionList.begin(); i != activeProtectionList.end(); ++i)
				{
					auto currentModule = *i;

					if(currentModule->Run())
						OnHackDetected(currentModule->moduleName);

					Sleep(INNER_CORE_LOOP_DELAY);
				}

				Sleep(OUTER_CORE_LOOP_DELAY);
			}
		}
	);
}

void StartAntiHack()
{
	Initialize();
	RunPassiveProtection();
	BeginActiveProtection();
}
