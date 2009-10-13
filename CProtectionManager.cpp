#include "CProtectionManager.h"
#include "NOP.h"

using namespace std;

CProtectionManager* CProtectionManager::Get()
{
	static CProtectionManager* ptr = NULL;

	if(ptr == NULL)
	{
		try {
			ptr = new CProtectionManager;
		} catch(...) {
			OnFailure("Could not allocate the protection manager.");
		}
	}

	return ptr;
}

void CProtectionManager::AddActiveProtection(ActiveProtectionFunc* functor)
{
	activeProtectionMechanisms.push_back(functor);
}

void CProtectionManager::AddPassiveProtection(PassiveProtectionFunc* functor)
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

DWORD CProtectionManager::ActiveProtectionThread(CProtectionManager* simulatedThis)
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
			}

			// Run the checks every 3 seconds.
			Sleep(3000);
		}
	} catch(...) {
		OnHackDetected("Unhandled C++ exception occured in one of the protection functions.");
	}
}

void CProtectionManager::BeginActiveProtection()
{
	ONCE(
		if(!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ActiveProtectionThread, this, NULL, NULL))
			OnFailure("Could not start the active protection thread.");
	);
}
