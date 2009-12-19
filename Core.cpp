#include "Core.h"
#include "ListSingletons.h"
#include "NOP.h"
#include "defs.h"
#include "Utilities.h"

using namespace Utilities;

// Variables that sanity check the developer. We are all our own worst enemies!
static bool initialized = false;
static bool passived = true;

void Initialize()
{
	LogInformation("Initializing...");

	auto initList = GetInitializationList();

	std::for_each(initList.begin(), initList.end(),
		[]
		(std::tr1::shared_ptr<InitializationModule>& currentModule)
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

	auto protectionList = GetPassiveProtectionList();

	std::for_each(protectionList.begin(), protectionList.end(),
		[]
		(std::tr1::shared_ptr<PassiveProtectionModule>& currentModule)
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

			auto activeProtectionList = GetActiveProtectionList();

			LogInformation((ConvertSizeTToString(activeProtectionList.size()) + " active protection module(s) loaded.").c_str());

			for(;;)
			{
				std::for_each(activeProtectionList.begin(), activeProtectionList.end(),
					[](std::tr1::shared_ptr<ActiveProtectionModule>& currentModule)
					{
						if(currentModule->Run())
							OnHackDetected(currentModule->moduleName);

						Sleep(INNER_CORE_LOOP_DELAY);
					}
				);

				Sleep(OUTER_CORE_LOOP_DELAY);
			}
		};

	CreateThread(activeProtectionThread);
}

void StartAntiHack()
{
	LogInformation("Anti-hack core starting...");
	try
	{
		Initialize();
		RunPassiveProtection();
		BeginActiveProtection();
	}
	catch(std::exception& ex)
	{
		OnFailure(ex.what());
	}
	catch(...)
	{
		OnFailure("Something that should not have gone wrong in the anti-hack core, did go wrong.");
	}
}
