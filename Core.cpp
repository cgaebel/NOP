#include "Core.h"
#include "ListSingletons.h"
#include "NOP.h"
#include "defs.h"
#include "Utilities.h"

using namespace Utilities;

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
			LogInformation((std::string("Warning: Ignoring module ") + removeTarget->moduleName).c_str());
			toRemoveFrom.remove(removeTarget);
		}
	);
}

void Initialize()
{
	LogInformation("Initializing...");

	auto& initList = GetInitializationList();
	RemoveIgnoredModules(initList);

	std::for_each(initList.begin(), initList.end(),
		[]
		(std::tr1::shared_ptr<InitializationModule> currentModule)
		{
			LogInformation(currentModule->logMessage);
			currentModule->Run();
		}
	);

	LogInformation((Utilities::ConvertSizeTToString(initList.size()) + " initialization module(s) loaded.").c_str());

	initialized = true;
}

void RunPassiveProtection()
{
	if(!initialized)
		OnFailure("Wtf we're starting protection without initialization? Yarite.");

	LogInformation("Beginning passive protection...");

	auto& protectionList = GetPassiveProtectionList();
	RemoveIgnoredModules(protectionList);

	std::for_each(protectionList.begin(), protectionList.end(),
		[]
		(std::tr1::shared_ptr<PassiveProtectionModule> currentModule)
		{
			LogInformation(currentModule->logMessage);

			if(currentModule->Run())
				OnHackDetected(currentModule->moduleName);
		}
	);

	LogInformation((ConvertSizeTToString(protectionList.size()) + " passive protection module(s) loaded.").c_str());

	passived = true;
}

void BeginActiveProtection()
{
	auto activeProtectionThread =
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

			LogInformation((ConvertSizeTToString(activeProtectionList.size()) + " active protection module(s) loaded.").c_str());

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
		};

	::Utilities::CreateThread(activeProtectionThread);
}

void StartAntiHack()
{
	Initialize();
	RunPassiveProtection();
	BeginActiveProtection();
}
