#include "AntiHackCore.h"
#include "NOP.h"
#include "HideThreadFromDebugger.h"

using namespace std;

AntiHackCore& AntiHackCore::Get()
{
	static AntiHackCore instance;
	return instance;
}

void AntiHackCore::AddActiveProtection(ActiveProtectionFunc* functor)
{
	activeProtectionMechanisms.push_back(functor);
}

void AntiHackCore::AddPassiveProtection(PassiveProtectionFunc* functor)
{
	const char* retval = NULL;
	try {
		if((retval = functor()) != NULL)
			OnHackDetected(retval);
	} catch(exception& ex) {
		OnFailure(ex.what());
	} catch(...) {
		OnFailure(retval);
	}
}

DWORD AntiHackCore::ActiveProtectionThread(AntiHackCore* simulatedThis)
{
	HideThreadFromDebugger();
	::SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

	try {
		for(;;)
		{
			for(
				vector<ActiveProtectionFunc*>::iterator i = simulatedThis->activeProtectionMechanisms.begin();
				i != simulatedThis->activeProtectionMechanisms.end();
				++i
			)
			{
				const char* retval = (*i)();

				if(retval != NULL)
					OnHackDetected(retval);

				Sleep(500);
			}

			// Run the checks every 3 seconds.
			Sleep(2000);
		}
	} catch(...) {
		OnHackDetected("Unhandled C++ exception occured in one of the protection functions.");
	}
}

void AntiHackCore::BeginActiveProtection()
{
	ONCE(
		if(!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ActiveProtectionThread, this, NULL, NULL))
			OnFailure("Could not start the active protection thread.");
	);
}